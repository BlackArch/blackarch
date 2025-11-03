FROM docker.io/blackarchlinux/blackarch:base-devel

# Update Systen
RUN pacman --noconfirm --noprogressbar --needed -Syyu

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

