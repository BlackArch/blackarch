## Description

Here you will find the official TODO (ROADMAP) list for BlackArch Linux. The
list contains items (tasks or projects) with a status for the current and
upcoming year. It also gives an overview of which BlackArch team member works on
what.


## Notes

- The TODO list is neither sorted nor prioritized.
- The priorities can be guessed based on the status of the item. ;-)
- Items can be removed. New items can be added.


## Legend

[x] done

[.] in progress

[ ] planned to do


## Get Involved

You can send us suggestions, ideas or even new tasks and projects.
Just check out the following:

**Please, send us pull requests!**

**Web:** https://www.blackarch.org/

**Mail:** team@blackarch.org

**IRC:** [irc://irc.freenode.net/blackarch](irc://irc.freenode.net/blackarch)


## TODO

```
===> 2019

  [.] tools:
      1. [.] fix pkgbuild style according to @FFY00 suggestions.
      [noptrix, mrsmith0x00, ?]

      2. [ ] QA of all tools: runtime + dependency checks. create a list for both in lists/
      [?]

      3. [ ] check for wrong/missing licenses files. diff against the common license
      files and install / don't install accordingly. in worst case, ask owner of
      $tool for permissions.
      [?]

      4. [ ] general QAing + add new tools (long-term). ;-)
      [all]

  [.] version-checker
      create an version-checker script (python3) for non-git tools so we can semi-automate
      the tools updating part like we do for git-tools.
      [mrsmith0x00]

  [ ] youtube
      create / add initial videos: blackarch-installation (2 parts: online and
      from live-iso)
      [?]

  [ ] blackarch-site
      create better images for the slider on index.html. any improvements are
      welcome.
      [?]

  [ ] blackarch-shop
      create t-shirts, stickers etc. for online shop. :D
      [?]

  [ ] blackarch-menus
      tweak, update, bugfix the current one
      [?]

  [ ] blackarch-virtualization
      tweak, update, bugfix the current setup for docker and vagrant.
      [?]

  [ ] sploitctl
      extend, parallelize it. takes too long.
      [noptrix]

  [ ] infrastructure
      create a new build-system environment so everyone of us can sign and
      release packages.
      [noptrix]

  [ ] blackarch-config-*
      update, tweak, improve and QA all blackarch-config-* packages.
      [?]

  [ ] ISO
      create and release a slim-ISO with xfce(?) mate(?) with top100 tools or
      something like that. note: @PedroSFreitas already worked on this.
      [psf, ?]

  [ ] blackman
      either update and fix current bugs or rewrite it from scratch. python3?
      [?]

  [ ] python-packages
      add missing python dependencies to python-packages. use setup.py and grep
      for imports.
      [?]

  [ ] linux-blackarch-hardened
      custom hardened kernel patches
      [noptrix, ?]

  [ ] linux-blackarch
      custom kernel / kernel patches / kernel modules to support wifi injection,
      network tunings, and more.
      [noptrix, ?]

  [ ] wiki
      replace the blackarch guide with a fresh new wiki. it is planned to use
      github for .md files and reference them on the blackarch website rather
      than having a complete wiki setup on blackarch site. this still needs to
      be discussed!
      [all]

  [ ] howto-dev
      update the howto for blackarch developers
      [noptrix]


===> 2018

  [x] armv6, armv7h, aarch64
      update all tools. fix installation errors.
      [noptrix]

  [.] ISO/OVA
      preparation and releases (2018.12.01)
      [noptrix]

  [x] tools
      update all tools [a-z] prior the new ISO/OVA release (2018.12.01)
      [noptrix]

  [x] howto-dev
      create an initial quick'n'dirty how-to for blackarch developers
      [noptrix]

  [.] wordlistctl
      create a similar tool like sploitctl but for wordlists containing:
      passwords, usernames, hostnames
      [sepehrdad]

  [x] blackarch-installer
      testings, bugfixes and updates prior the new ISO/OVA release (2018.12.01)
      [noptrix, mrsmith0x00]

  [.] tools
      single updates and QAs of all tools (long-term process)
      [all]
```
