#!/bin/bash
set -eu
echo APK Studio normally requires you to download \"vendor.zip\" and extract it.
echo It contains apktool.jar. Since that\'s a dependency of this package, this
echo script will instead create symbolic links in $HOME/.apkstudio/vendor/.
echo

mkdir -p "$HOME/.apkstudio/vendor/"
[ -e "$HOME/.apkstudio/vendor/apktool.jar" ] || ln -s "/usr/share/android-apktool/apktool.jar" "$HOME/.apkstudio/vendor/apktool.jar"
[ -e "$HOME/.apkstudio/vendor/VERSION" ] || ln -s "/usr/share/apkstudio-git/APKTOOL_VERSION" "$HOME/.apkstudio/vendor/VERSION"

echo You may now use APK Studio. Have a nice day!
