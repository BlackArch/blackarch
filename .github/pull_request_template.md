## Before submitting, please select the type of PR you are opening

- [ ] 📦 New tool/library submission
- [ ] 🕸️ New mirror submission

---
<!-- Fill in ONLY the section that matches your PR type above. Delete the other sections. -->

# 📦 New tool/library submission

## Before submitting, please read the requirements

> **🚫 Illegal or unethical software:**
> Examples include brute-forcing social media accounts or DoS attack scripts.

> **🚫 Abandoned or extremely outdated software:**
> Especially if it no longer builds, creates dependency conflicts, or has no active upstream maintenance.

> **🚫 Deprecated languages or libraries dependency:**
> For example, Python 2 or other retired technologies that introduce security or compatibility issues.

> **🚫 Wrapper scripts with no unique functionality:**
> Scripts that simply call another tool with preset arguments (e.g., a front-end that just runs `nmap -sV ...`).

> **🚫 Non-portable or broken build systems:**
> Tools tied to a specific non-Arch distribution or that cannot be cleanly built on Arch/BlackArch.

> **🚫 Duplicate functionality:**
> Tools already covered by better-maintained or more widely-used alternatives in the repository.

> **🚫 Unnecessary anonymization helpers:**
> Scripts such as "free VPN launcher" tools. BlackArch already includes **torctl** for legitimate anonymization.

> **🚫 PKGBUILD does not follow official templates:**
> Submissions must follow the official PKGBUILD templates available in the [BlackArch PKGBUILDs repository](https://github.com/BlackArch/blackarch-pkgbuilds). PRs that deviate significantly from the templates will be rejected or sent back for revision.

### Package name
<!-- Indicate the name of the package as it will appear in the BlackArch repository -->


### Description
<!-- Clear and concise description of what the tool does and what security use case it addresses -->


### Source URL
<!-- Link to the tool's upstream source code repository -->


### License
<!-- Indicate the tool's license, e.g. MIT, GPL-3.0. If there is no license or source is not public, indicate "custom" -->


### Tool category
<!-- Indicate the most appropriate category for this tool. Available categories:
anti-forensic, automation, automobile, backdoor, binary, bluetooth, code-audit, cracker,
crypto, database, debugger, decompiler, defensive, disassembler, dos, drone, exploitation,
fingerprint, firmware, forensic, fuzzer, hardware, honeypot, ids, keylogger, malware, misc,
mobile, networking, nfc, packer, proxy, radio, recon, reversing, scanner, sniffer, social,
spoof, stego, tunnel, voip, webapp, windows, wireless -->


### Additional context
<!-- Any other relevant information, e.g. known limitations, special dependencies, platform constraints -->


### Checklist

- [ ] I assert that this tool is not already available in BlackArch or the official [Arch Linux repositories](https://archlinux.org/packages/).
- [ ] I assert that this is not a duplicate of an [existing open PR](https://github.com/BlackArch/blackarch/pulls?q=is%3Aopen).
- [ ] I assert that I have successfully tested, built and installed the package locally.
- [ ] I assert that the PKGBUILD follows the official [BlackArch PKGBUILD templates](https://github.com/BlackArch/blackarch-pkgbuilds).
- [ ] I assert that the tool is actively maintained upstream and does not depend on deprecated languages or libraries.
- [ ] I assert that I have read the [Arch Linux Code of Conduct](https://terms.archlinux.org/docs/code-of-conduct/) and agree to abide by it.


# 🕸️ New mirror submission

## Before submitting, please read the requirements

> **🚫 Unstable or unreliable mirrors:**
> Mirrors must maintain reasonable uptime. Servers with frequent downtime or inconsistent availability will not be accepted.

> **🚫 Slow sync frequency:**
> Mirrors must sync with the upstream BlackArch repository at least once every 24 hours.

> **🚫 Incomplete sync:**
> Mirrors must host the full BlackArch repository. Partial mirrors are not accepted.

> **🚫 Missing corresponding PR in blackarch-site:**
> Both repositories must be updated simultaneously. Submissions missing the `blackarch-site` PR will be held until it is provided.
> You must edit [`mirror/mirror.lst`](https://github.com/BlackArch/blackarch/blob/master/mirror/mirror.lst) in this repository **and** [`blackarch-mirrorlist`](https://github.com/BlackArch/blackarch-site/blob/master/blackarch-mirrorlist) in the [blackarch-site repository](https://github.com/BlackArch/blackarch-site), and link both PRs in your submission.

> **🚫 No valid operator contact:**
> The BlackArch team must be able to reach the mirror operator for maintenance or takedown requests.

### Mirror URL
<!-- Provide the full URL of the mirror, e.g. https://mirror.example.com/blackarch/ -->


### Country / Region
<!-- Indicate the country or region where the mirror is hosted, e.g. DE, US - East Coast -->


### Available bandwidth
<!-- Indicate the available bandwidth for the mirror, e.g. 1 Gbps -->


### Sync frequency
<!-- How often does the mirror sync with the upstream repository? e.g. Every 6 hours -->


### Operator contact
<!-- Provide a contact email or handle for the mirror operator -->


### Corresponding blackarch-site PR
<!-- Link the corresponding PR you have opened in https://github.com/BlackArch/blackarch-site editing blackarch-mirrorlist file -->


### Additional context
<!-- Any other relevant information, e.g. IPv6 support, HTTPS-only, sponsoring organization -->


### Checklist

- [ ] I assert that this mirror is not a duplicate of an existing submission.
- [ ] I assert that the mirror syncs at least once every 24 hours and hosts the full BlackArch repository.
- [ ] I assert that I have opened a corresponding PR in [BlackArch/blackarch-site](https://github.com/BlackArch/blackarch-site) editing `blackarch-mirrorlist`, and have linked it above.
- [ ] I understand that the BlackArch team may contact me regarding this mirror and that I am responsible for its maintenance.
- [ ] I assert that I have read the [Arch Linux Code of Conduct](https://terms.archlinux.org/docs/code-of-conduct/) and agree to abide by it.
