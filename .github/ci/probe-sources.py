#!/usr/bin/env python3
"""
probe-sources.py --state DIR [--workers N] [--timeout S]

Checks that every non-VCS source URL of every package is still reachable
(HEAD, falling back to a 1-byte ranged GET) and appends definite failures to
DIR/issues.json as category "source_broken". Ambiguous answers (403, 429, 5xx,
timeouts) are recorded as warnings so one flaky mirror doesn't spam the report.
VCS sources are covered by nvchecker's `git ls-remote` in the main run.
"""
import argparse
import concurrent.futures as cf
import json
import socket
import ssl
import time
import urllib.error
import urllib.request
from pathlib import Path

UA = "BlackArch-CI-source-probe/1.0 (+https://blackarch.org)"
DEFINITE = {404, 410, 451}


def probe(url: str, timeout: float) -> tuple[str, str]:
    """-> ('ok'|'broken'|'uncertain', detail)"""
    last = ""
    for method in ("HEAD", "GET"):
        req = urllib.request.Request(url, method=method, headers={"User-Agent": UA})
        if method == "GET":
            req.add_header("Range", "bytes=0-0")
        try:
            with urllib.request.urlopen(req, timeout=timeout) as r:
                return "ok", str(r.status)
        except urllib.error.HTTPError as e:
            if e.code == 416:
                return "ok", "416"
            if e.code in DEFINITE:
                return "broken", f"HTTP {e.code}"
            last = f"HTTP {e.code}"
            continue  # e.g. 403/405 on HEAD: retry with GET
        except urllib.error.URLError as e:
            r = e.reason
            if isinstance(r, socket.gaierror):
                if method == "HEAD":      # one retry: runner DNS can hiccup
                    time.sleep(3)
                    continue
                return "broken", f"DNS: {r}"
            if isinstance(r, ConnectionRefusedError):
                return "broken", "connection refused"
            if isinstance(r, ssl.SSLCertVerificationError):
                return "broken", f"TLS: {r.verify_message}"
            last = str(r)
        except (TimeoutError, socket.timeout):
            last = "timeout"
        except Exception as e:  # noqa: BLE001
            last = f"{type(e).__name__}: {e}"
    return "uncertain", last


def main() -> None:
    p = argparse.ArgumentParser()
    p.add_argument("--state", required=True)
    p.add_argument("--workers", type=int, default=24)
    p.add_argument("--timeout", type=float, default=25)
    a = p.parse_args()
    st = Path(a.state)
    meta = json.loads((st / "meta.json").read_text())
    untracked = json.loads((st / "untracked.json").read_text())

    urls: dict[str, set[str]] = {}
    for pkg, m in meta.items():
        if untracked.get(pkg, "").startswith("excluded"):
            continue
        for s in m.get("source", []):
            url = s.split("::", 1)[1] if "::" in s else s
            if url.startswith(("http://", "https://")):
                urls.setdefault(url, set()).add(pkg)

    issues = json.loads((st / "issues.json").read_text())
    counts = {"ok": 0, "broken": 0, "uncertain": 0}
    with cf.ThreadPoolExecutor(a.workers) as ex:
        futs = {ex.submit(probe, u, a.timeout): u for u in urls}
        for f in cf.as_completed(futs):
            u = futs[f]
            res, detail = f.result()
            counts[res] += 1
            if res == "ok":
                continue
            for pkg in sorted(urls[u]):
                issues.append({
                    "pkg": pkg,
                    "category": "source_broken" if res == "broken" else "upstream_check",
                    "severity": "error" if res == "broken" else "warning",
                    "check": "source-probe", "message": f"{detail}: {u}"})
    (st / "issues.json").write_text(json.dumps(issues, indent=1))
    print(f"probed {len(urls)} URLs: {counts}")


if __name__ == "__main__":
    main()
