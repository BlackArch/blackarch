# Builder image for the auto-update pipeline: one throw-away container per
# package build. Based on the same image travis/Dockerfile uses.
FROM docker.io/blackarchlinux/blackarch:base-devel

RUN pacman -Syu --noconfirm --needed --noprogressbar \
      git jq namcap pacman-contrib openssh && \
    pacman -Scc --noconfirm

RUN useradd -m -G wheel builder && \
    echo 'builder ALL=(ALL) NOPASSWD: ALL' > /etc/sudoers.d/builder && \
    chmod 440 /etc/sudoers.d/builder && \
    mkdir -p /build /out && chown builder: /build /out

# makepkg overrides: no -debug split packages, zst output, use all cores,
# fetch PGP keys listed in validpgpkeys automatically.
RUN install -d -o builder /home/builder/.config/pacman /home/builder/.gnupg && \
    printf '%s\n' \
      'PKGEXT=".pkg.tar.zst"' \
      'OPTIONS=("${OPTIONS[@]/#debug/!debug}")' \
      'MAKEFLAGS="-j$(nproc)"' \
      'PACKAGER="BlackArch CI <team@blackarch.org>"' \
      > /home/builder/.config/pacman/makepkg.conf && \
    printf '%s\n' 'keyserver hkps://keyserver.ubuntu.com' \
      'keyserver-options auto-key-retrieve' > /home/builder/.gnupg/gpg.conf && \
    chown -R builder: /home/builder && chmod 700 /home/builder/.gnupg

COPY build-one.sh install-test.sh /usr/local/bin/
USER builder
WORKDIR /build
ENTRYPOINT ["/usr/local/bin/build-one.sh"]
