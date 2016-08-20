FROM base/archlinux
RUN pacman --noconfirm -Sy archlinux-keyring && \
    pacman --noconfirm -S expect arch-install-scripts && \
    pacman -Scc
ADD ./build.sh /usr/local/bin
ADD ./build-helper.sh /usr/local/bin
ADD ./build-pacman.conf /usr/local/etc
ADD https://raw.githubusercontent.com/BlackArch/blackarch-site/master/strap.sh /usr/local/bin
RUN chmod u+x usr/local/bin/strap.sh
VOLUME ["/output"]
WORKDIR /output
ENTRYPOINT ["/usr/local/bin/build.sh"]