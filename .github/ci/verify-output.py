#!/usr/bin/env python3
"""
verify-output.py - runs on the runner HOST after a build container exits.

Everything the build container wrote is untrusted: upstream build code ran in
there with sudo and could rewrite any file in its output directory, plant
extra files, or leave symlinks. Before anything is kept for publishing:

  * The returned PKGBUILD must equal the one on master line by line, except:
      - exactly one `pkgver=` and one `pkgrel=` line, each of the strict form
        `pkgver=<valid version>` / `pkgrel=<digits>`, at the same position;
      - for release updates only: checksum arrays, each of which must contain
        nothing but quoted hex digests or 'SKIP' (no shell expansions).
    The allowed lines are VALIDATED, not just skipped, so nothing can hide
    inside them.
  * Every *.pkg.tar.zst must be a regular file whose .PKGINFO names a package
    of THIS PKGBUILD with exactly the expected epoch:pkgver-pkgrel and arch
    any/x86_64, and whose file name matches that .PKGINFO.
  * status.json is rewritten from host-side values (package = directory,
    kind/cur/target from the plan); free text is length-limited.
  * Results are written only to host paths the container never had access to
    (--status-out, --verified-out, --valid-list); any pre-existing file there
    is replaced.

  verify-output.py --pkg DIR --kind K --cur V --target V --orig PKGBUILD
                   --work OUTDIR --meta meta.json --status-out FILE
                   --verified-out FILE --valid-list FILE
Exit code is always 0; a rejection becomes a build_failed result.
"""
import argparse
import json
import os
import re
import select
import signal
import subprocess
import time
from pathlib import Path

RESULTS = {"ok", "build_failed", "source_broken", "pkgbuild_invalid", "no_change"}
SUMS_START = re.compile(r"^(md5|sha1|sha224|sha256|sha384|sha512|b2|ck)sums(_[A-Za-z0-9_]+)?=\(")
SUMS_STRICT = re.compile(
    r"(?:md5|sha1|sha224|sha256|sha384|sha512|b2|ck)sums(?:_[A-Za-z0-9_]+)?=\(\s*"
    r"(?:(['\"])(?:[0-9a-fA-F]{8,}|SKIP)\1\s*)*\)\s*")
PKGVER_LINE = re.compile(r"pkgver=([A-Za-z0-9._+]+)\s*")
PKGREL_LINE = re.compile(r"pkgrel=([0-9]+(?:\.[0-9]+)?)\s*")
PKGFILE = re.compile(r"^(?P<name>.+)-(?P<ver>[^-]+-[^-]+)-(?P<arch>any|x86_64)\.pkg\.tar\.zst$")
MAX_TEXT = 1 << 20          # status.json / PKGBUILD larger than 1 MiB are rejected


class Bad(Exception):
    pass


def shape(text: str, strict: bool, allow_sums: bool,
          known: set[str] | None = None
          ) -> tuple[list[str], str | None, str | None, set[str], list[str]]:
    """
    -> (lines with the mutable lines replaced by placeholders,
        pkgver, pkgrel (None when the line is not a plain value, e.g.
        pkgver=${_major}.${_minor}), the mutable lines/blocks as found,
        one SKIP/digest mask per checksum array)
    strict=True validates every mutable line (used for the untrusted file);
    a mutable line identical to one in `known` (taken from master) passes.
    """
    known = known or set()
    lines = text.split("\n")
    out, ver, rel, seen, masks = [], [], [], set(), []
    i = 0
    while i < len(lines):
        line = lines[i]
        if line.startswith("pkgver="):
            m = PKGVER_LINE.fullmatch(line)
            if strict and not m and line not in known:
                raise Bad("pkgver= line has unexpected content")
            ver.append(m.group(1) if m else None)
            seen.add(line)
            out.append("\0PKGVER")
            i += 1
            continue
        if line.startswith("pkgrel="):
            m = PKGREL_LINE.fullmatch(line)
            if strict and not m and line not in known:
                raise Bad("pkgrel= line has unexpected content")
            rel.append(m.group(1) if m else None)
            seen.add(line)
            out.append("\0PKGREL")
            i += 1
            continue
        if allow_sums and SUMS_START.match(line):
            j = i
            while ")" not in lines[j] and j + 1 < len(lines):
                j += 1
            block = "\n".join(lines[i:j + 1])
            if strict and not SUMS_STRICT.fullmatch(block) and block not in known:
                raise Bad("checksum array has unexpected content")
            seen.add(block)
            # same number of entries; check() compares the SKIP positions
            toks = re.findall(r"""['"]([^'"]*)['"]""", block)
            masks.append("".join("S" if t == "SKIP" else "h" for t in toks))
            out.append(f"\0SUMS {line.split('=', 1)[0]} {len(toks)}")
            i = j + 1
            continue
        out.append(line.rstrip())
        i += 1
    if len(ver) != 1 or len(rel) != 1:
        raise Bad("PKGBUILD must have exactly one pkgver= and one pkgrel= line")
    return out, ver[0], rel[0], seen, masks


def read_small(p: Path) -> str:
    if p.is_symlink() or not p.is_file():
        raise Bad(f"{p.name} is missing or not a regular file")
    if p.stat().st_size > MAX_TEXT:
        raise Bad(f"{p.name} is too large")
    return p.read_text(errors="replace")


def clean(s, n=300) -> str:
    s = re.sub(r"[\x00-\x08\x0b-\x1f\x7f]", "", str(s or ""))
    return s.replace("```", "'''")[:n]


# Every .PKGINFO line must be "key = value" with a plain lower-case key.
# repo-add on the server reads .PKGINFO with `IFS=' =' read var val` and then
# `declare "$var=$val"`: a key like x[$(cmd)] would run cmd there.
PKGINFO_LINE = re.compile(r"[a-z]+ = [^\x00-\x1f\x7f]*")


def _run(cmd: list[str], limit: int, timeout: int = 120) -> str:
    """
    Run cmd, keep at most `limit` bytes of stdout, and give up after `timeout`
    seconds: a hostile archive may be a bomb (size) or just very slow (time).
    The process gets its own session so tar AND its zstd child are killed.
    """
    p = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.DEVNULL,
                         start_new_session=True)
    chunks, size, deadline = [], 0, time.monotonic() + timeout
    try:
        while size <= limit:
            left = deadline - time.monotonic()
            if left <= 0:
                raise Bad("archive could not be read in time")
            if not select.select([p.stdout], [], [], left)[0]:
                raise Bad("archive could not be read in time")
            chunk = os.read(p.stdout.fileno(), 1 << 16)
            if not chunk:
                break
            chunks.append(chunk)
            size += len(chunk)
    finally:
        try:
            os.killpg(p.pid, signal.SIGKILL)
        except ProcessLookupError:
            pass
        p.wait()
    if size > limit:
        raise Bad("archive listing or .PKGINFO is too large")
    return b"".join(chunks).decode(errors="replace")


def pkginfo(path: Path) -> dict[str, str]:
    """
    Read and validate .PKGINFO exactly as repo-add will see it: the archive
    must hold one .PKGINFO (no ./.PKGINFO or duplicate copies that another tar
    implementation might pick instead), and every line must be well formed.
    """
    # both calls decompress the whole archive: scale the deadline with its size
    # (120 s + 1 s per 20 MB compressed), so a huge but honest package passes
    budget = 120 + path.stat().st_size // (20 << 20)
    members = _run(["tar", "-I", "zstd", "-tf", str(path)], 64 << 20, timeout=budget).split("\n")
    # every spelling of a top-level .PKGINFO another tar might match
    candidates = [m for m in members if re.fullmatch(r"(\./|/)*\.PKGINFO/?", m)]
    if candidates != [".PKGINFO"]:
        raise Bad("package must contain exactly one .PKGINFO at the top level")
    raw = _run(["tar", "-I", "zstd", "-xOf", str(path), ".PKGINFO"], MAX_TEXT, timeout=budget)
    info: dict[str, str] = {}
    for line in raw.split("\n"):
        if line == "" or line.startswith("#"):
            continue
        if not PKGINFO_LINE.fullmatch(line):
            raise Bad(".PKGINFO contains a malformed line")
        k, _, v = line.partition(" = ")
        if k in ("pkgname", "pkgver", "arch"):
            if k in info:
                raise Bad(f"duplicate {k} in .PKGINFO")
            info[k] = v.strip()
    return info


def check(a, meta: dict, work: Path) -> tuple[str, list[str], str]:
    """-> (pkgver, verified package paths, verified PKGBUILD text) or raise Bad"""
    orig = Path(a.orig).read_text(errors="replace")
    new = read_small(work / "PKGBUILD")
    sums_ok = a.kind == "release"
    o_shape, _, _, o_mut, o_masks = shape(orig, strict=False, allow_sums=sums_ok)
    n_shape, ver, rel, _, n_masks = shape(new, strict=True, allow_sums=sums_ok, known=o_mut)
    if o_shape != n_shape:
        raise Bad("PKGBUILD changed outside pkgver/pkgrel/checksums")
    # a bump may turn SKIP into a digest (updpkgsums does for plain tarballs),
    # never a digest into SKIP
    for om, nm in zip(o_masks, n_masks):
        if any(o == "h" and n == "S" for o, n in zip(om, nm)):
            raise Bad("a checksum was replaced by SKIP")
    if a.kind == "release" and ver != a.target:
        raise Bad(f"pkgver {ver} is not the requested {a.target}")
    if a.kind == "rebuild" and ver is not None and ver != a.cur and "has_pkgver_func" not in meta:
        raise Bad(f"pkgver changed during a rebuild ({a.cur} -> {ver})")
    # audit/ship builds must not change the PKGBUILD, except that makepkg
    # itself rewrites pkgver= for packages with a pkgver() function (VCS) when
    # upstream moved; shape() above already limits any change to pkgver/pkgrel
    if a.kind in ("test", "ship") and new != orig:
        if "has_pkgver_func" not in meta:
            raise Bad("PKGBUILD was modified by the build")
        if rel is not None and rel != meta.get("pkgrel", rel):
            raise Bad("pkgrel changed during an audit build")
    # a computed pkgver=/pkgrel= line (unchanged from master, so allowed above):
    # the expected value is the evaluated one detect read from the PKGBUILD
    if ver is None:
        ver = a.cur
    if rel is None:
        rel = meta.get("pkgrel", "")
    if not ver or not rel:
        raise Bad("cannot determine the expected version")

    names = set(meta.get("pkgname") or [a.pkg])
    epoch = meta.get("epoch", "")
    full = (f"{epoch}:" if epoch else "") + f"{ver}-{rel}"
    if a.kind == "ship" and full != a.target:
        raise Bad(f"version {full} is not the planned {a.target}")
    pdir = work / "pkgs"
    if pdir.is_symlink() or not pdir.is_dir():
        raise Bad("no packages")
    files = sorted(pdir.iterdir())
    if not files:
        raise Bad("no packages")
    valid = []
    for f in files:
        if f.is_symlink() or not f.is_file() or not PKGFILE.match(f.name):
            raise Bad(f"unexpected file {clean(f.name, 80)}")
        try:
            info = pkginfo(f)
        except Bad:
            raise
        except Exception as e:  # noqa: BLE001
            raise Bad(f"unreadable package {clean(f.name, 80)}: {clean(e, 80)}")
        if info.get("pkgname") not in names:
            raise Bad(f"package {clean(info.get('pkgname'), 60)} is not built by this PKGBUILD")
        if info.get("pkgver") != full or info.get("arch") not in ("any", "x86_64"):
            raise Bad(f"{info.get('pkgname')}: version/arch {clean(info.get('pkgver'), 40)}/"
                      f"{clean(info.get('arch'), 20)}, expected {full}")
        if f.name != f"{info['pkgname']}-{info['pkgver']}-{info['arch']}.pkg.tar.zst":
            raise Bad(f"file name {clean(f.name, 80)} does not match its .PKGINFO")
        valid.append(str(f))
    return ver, valid, new


def main() -> None:
    ap = argparse.ArgumentParser()
    for k in ("pkg", "kind", "cur", "target", "orig", "work", "meta",
              "status-out", "verified-out", "valid-list"):
        ap.add_argument("--" + k, required=True)
    ap.add_argument("--ship", default="0", help="1: planned because the tree is ahead of the repo")
    a = ap.parse_args()
    work = Path(a.work)
    meta = json.loads(Path(a.meta).read_text()).get(a.pkg, {})
    verified_out, valid_list = Path(a.verified_out), Path(a.valid_list)
    verified_out.unlink(missing_ok=True)

    try:
        st = json.loads(read_small(work / "status.json"))
        if not isinstance(st, dict):
            raise ValueError
    except Exception:  # noqa: BLE001
        st = {"result": "build_failed", "reason": "builder wrote no readable status.json"}
    if st.get("result") in RESULTS:
        result, reason = st["result"], clean(st.get("reason"))
    else:
        result, reason = "build_failed", "builder reported an unknown result"

    pkgver, valid = "", []
    if result == "ok":
        try:
            pkgver, valid, text = check(a, meta, work)
            verified_out.write_text(text)
        except Bad as e:
            result, reason, valid = "build_failed", "output rejected: " + clean(e), []

    out = {"pkg": a.pkg, "kind": a.kind, "cur": a.cur, "target": a.target,
           "ship": a.ship == "1",
           "result": result, "reason": reason, "pkgver": pkgver,
           "seconds": st.get("seconds") if isinstance(st.get("seconds"), int) else None,
           "log_tail": clean(st.get("log_tail"), 6000)}
    Path(a.status_out).write_text(json.dumps(out))
    valid_list.write_text("\n".join(valid) + ("\n" if valid else ""))


if __name__ == "__main__":
    main()
