# Description
**BlackArch Linux** is an ArchLinux based penetration testing distribution for
penetration testers and security researchers. The repository contains
[2845 tools](https://www.blackarch.org/tools.html). You can install tools
individually or in groups. BlackArch Linux is compatible with existing Arch
installs. For more information, see the installation instructions.
## Overview
To report bugs and request new tools, please visit the issue tracker on GitHub,
stop by [Matrix](https://matrix.to/#/#BlackArch:matrix.org), or email us.  
**BlackArch Linux** is an Arch Linux-based distribution specifically designed for **penetration testers** and **security researchers**. The repository contains over **2,800 tools** organized by category, allowing users to install tools individually or in **groups**. **BlackArch** is fully compatible with existing Arch Linux installations, making it a versatile choice for those who want to enhance their existing systems with security tools.
## Download and Installation
Whether you're a cybersecurity professional, enthusiast, or researcher, **BlackArch** offers the tools you need for a variety of tasks, such as **penetration testing**, **forensics**, **reverse engineering**, **network analysis**, and much more.
BlackArch Linux only takes a moment to set up.
## Key Features
- **Over 2,800 Tools**: BlackArch offers a vast collection of tools categorized by task, ensuring you have what you need for any penetration testing scenario.
- **Arch Linux-based**: Leverages Arch's rolling release model for continuous updates and cutting-edge software.
- **Compatibility**: Seamlessly integrates with existing Arch Linux installations.
- **Customizable Installation**: Choose between **Full** or **Slim ISO** installations, or install BlackArch tools on an existing Arch system.
- **Live ISO**: Try out BlackArch without installation using the live ISO.
1. [Install on an existing Arch machine](https://www.blackarch.org/downloads.html#install-repo).
2. Use the [live ISO](https://blackarch.org/downloads.html).
3. The Full and Netinstall ISOs come with a text-based installer (`blackarch-install`). The Slim ISO comes with a GUI-based installer. You can use the installer to install BlackArch Linux onto your hard disk.
## Table of Contents
- [Description](#description)
- [Installation Instructions](#installation-instructions)
- [Usage](#usage)
- [Get Involved](#get-involved)
- [Contributing](#contributing)
- [Contact Information](#contact-information)
## Installation Instructions
**Please, send us pull requests!**  
Setting up **BlackArch Linux** is quick and easy. There are three primary installation methods, depending on your preference:
To start developing for BlackArch please refer to the [Dev-Guide](./docs/HOWTO-DEV.md).
### 1. **Install on an Existing Arch Machine**
If you're running **Arch Linux**, you can install **BlackArch tools** directly onto your system. This method requires only the **BlackArch repository**.
**Web:** https://www.blackarch.org/
```bash
# To install BlackArch tools, first sync the BlackArch repository
curl -s https://blackarch.org/strap.sh | sudo bash
```
Email: team@blackarch.org
# Then, install BlackArch tools by choosing either individual tools or groups
sudo pacman -S blackarch

2. Live ISO Installation
The Live ISO allows you to try BlackArch without installation. You can explore the pre-configured environment and tools before deciding to install.
    Download the Live ISO from the official BlackArch website.
    Boot from the live ISO to start penetration testing and security research tasks right away.

3. Install Using Full or Netinstall ISOs
BlackArch offers Full and Netinstall ISOs, both of which include a text-based installer (blackarch-install) to guide you through the installation process.
    The Full ISO comes with a complete set of tools, providing a comprehensive penetration testing environment.
    The Netinstall ISO is a minimal installation method that requires an internet connection to download tools and configure the system during installation.
    For users looking for a lightweight approach, the Slim ISO offers a GUI-based installer, enabling a smoother installation experience for those new to Linux or penetration testing.
For more detailed installation steps, visit the installation documentation.
Usage
Once installed, you can install additional tools or tool groups from the BlackArch repository using the pacman package manager. Here are some examples:
Install all BlackArch Tools
sudo pacman -S blackarch
Install Specific Tool Groups

# For example, to install Web Application Security tools:
sudo pacman -S blackarch-webapp
For a complete list of available tools and categories, refer to the official BlackArch tools list.

Running Tools
After installation, you can run tools directly from the terminal or from the application menu (if you're using a GUI). Most tools are available as command-line utilities, making them ideal for automation and scripting.

Get Involved
BlackArch is an open-source project, and we welcome contributions from the community. Whether you're a security researcher, developer, or just a passionate user, you can contribute in the following ways:

Report Bugs & Request New Tools
If you encounter a bug or have suggestions for new tools, please use the following channels:
GitHub Issues: Report issues directly on our GitHub Issue Tracker.
Matrix: Join our discussion channel for real-time communication.

Email: team@blackarch.org
    Contribute to BlackArch
    We encourage contributions in various forms:
    Bug Fixes
    Documentation
    New Tools
    Tool Groups

We are happy to accept pull requests for improvements. For more detailed information on how to contribute, please refer to the Developer Guide in the repository.
    Pull Requests
    You can submit pull requests directly to our GitHub repository:
    Fork the repository, make your changes, and submit a PR.
    Please follow our contribution guidelines to ensure your PR is well-received.

Contributing
We adhere to the Contributor Covenant Code of Conduct, which sets standards for respectful communication and collaboration. Before contributing, please review the following:
    Coding Standards
    Testing
    Documentation
    Pull Requests

For a complete guide on contributing to BlackArch, please refer to our Dev-Guide.
Contact Information
Email: team@blackarch.org
Matrix: [#BlackArch:matrix.org](https://matrix.to/#/#BlackArch:matrix.org)
For updates and announcements, check our official blog.

License
BlackArch Linux is released under the GNU General Public License v3.0. See the COPYING file for more details.

Disclaimer
BlackArch Linux is intended for ethical hacking, penetration testing, and security research purposes. It should only be used on systems where you have explicit permission to perform security assessments.

Conclusion
BlackArch Linux is a powerful, flexible, and comprehensive penetration testing platform built on the stability and versatility of Arch Linux. Whether you're conducting a security audit, performing vulnerability assessments, or learning about cybersecurity, BlackArch offers all the tools you need to succeed.
