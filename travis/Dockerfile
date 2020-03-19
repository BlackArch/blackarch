FROM archlinux

# Setup mirrors
RUN printf "[multilib]\nInclude = /etc/pacman.d/mirrorlist\n" \
        >> "$path/etc/pacman.conf" && \
    pacman --noconfirm --noprogressbar --needed -Sy pacman-contrib && \
    cp /etc/pacman.d/mirrorlist /etc/pacman.d/mirrorlist.bak && \
    sed -i 's/^#Server/Server/' /etc/pacman.d/mirrorlist.bak && \
    rankmirrors -n 6 /etc/pacman.d/mirrorlist.bak > /etc/pacman.d/mirrorlist && \
    curl -s https://blackarch.org/strap.sh | bash -

# Setup build dependencies
RUN pacman --noconfirm --noprogressbar --needed -Syyu base-devel

# Add builder User
RUN useradd -m -d /src -G wheel -g users builder -s /bin/bash && \
    echo "builder ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers && \
    echo "root ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers

# Change to user builder
USER builder

# Change working directory
WORKDIR /src

COPY --chown=builder:users . .

# Run entrypoint
ENTRYPOINT ["makepkg"]

CMD ["-s", "-f", "--noconfirm", "--needed", "--noprogressbar"]