## Description

A quick 'n' dirty HOW-TO for BlackArch Linux developers. It contains the most
important and relevant aspects of this project. Any other topics can and will be
discussed on the fly. This HOW-TO also contains an on-boarding guideline for
new developers.

This document contains a simple lists of information rather than detailed
descriptions. Longer text blocks should be avoided except for this description.
Further details or any questions can be discussed in IRC or Discord.

The categories are structured to have at least a little bit of an overview.


## On-Boarding for new Developers

Following information are immediately needed when you want to join the BlackArch
Linux project:

- **Name**: Your name will be used on website. Say fuckoff to stay anonym.

- **E-Mail**: For the team@blackarch.org mailinglist. You will receive any mails
sent to that list.

- **Handle**: For references in any BlackArch project and also for IRC/Discord.

- **Twitter**: Your twitter name if present. @blackarchlinux will then follow
you.

- **Github**: Your github Account name with 2-fa enabled. You will get full
read-write access to all repositories.

- **Pub-Keys**: Your GPG and SSH pubkeys. SSH Key will be used for our shared
build-system.

- **Role**: The role you want to have and where you will mostly focus on. Roles
can be multiple.

You should then continue reading the rest of this document once all information
are given and everything is setup. Anything else can be discussed on IRC.


## General Infos

- CoC for BlackArch is almost the same like:
[https://wiki.archlinux.org/index.php/Code_of_conduct](https://wiki.archlinux.org/index.php/Code_of_conduct)

- Read and refer always to ArchLinux's wiki pages.

- If you are new try to follow other developers's work as much as possible. Talk
  to us on IRC / Discord for further details. We've got our own private channels
  to discuss.

- Any work is much appreciated. Even if it's a version bump on a package or a
  typo-fix.

- Try to automate anything you can and share the tools with us.


## Packaging style

- Use our [PKGBUILD templates](https://github.com/BlackArch/blackarch-pkgbuilds)
  for creating and releasing new packagaes in order to keep the same style and
  consistency.

- If you are unsure, copy an existing PKGBUILD from a package and use that as a
  template or ask other developers.


## Creating packages

The following information was written with the goal of creating hight-quality PKGBUILDs and ensure that ever package uploaded to the BlackArch Linux repository is really working.

**Prerequisites:**

- Set up a clean chroot environment according to https://wiki.archlinux.org/index.php/DeveloperWiki:Building_in_a_clean_chroot#Setting_up_a_chroot

- A BackArch Linux system with all tools installed (`# pacman -Syu --needed blackarch`).

**Steps:**

- Create the PKGBUILD and build it in the clean chroot environment to ensure that buildtime dependeces aren't missing.

- If the package was built, install and execute it in the clean chroot environment to ensure that runtime dependencies aren't missing.

- Install the package in the full existing BlackArch Linux installation to ensure that there aren't conflicting files.

- Check the syntax to ensure that the code is the most efficient possible and upload the PKGBUILD.

- For your pull request use the syntax \<pkgname\>: \<initial commit\> or \<pkgname\>: \<change done\>

## ISO

foo


## Website

foo
