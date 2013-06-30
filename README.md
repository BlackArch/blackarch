BlackArch
=========

To make use of the pentest set of tools available in the BlackArch toolset please add the BlackArch repository to `/etc/pacman.conf`

# 32-bit

```
[blackarch]
SigLevel = Optional TrustAll
Server = http://www.blackarch.org/pub/blackarch/i686
```

# 64-bit
```
[blackarch]
SigLevel = Optional TrustAll
Server = http://www.blackarch.org/pub/blackarch/x86_64
```

It's also recommended to add `/blackarch` to your $PATH