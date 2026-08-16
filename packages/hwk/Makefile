INCLUDES    = -Iinclude
CFLAGS      = -O2 -s -std=gnu99 -Wall -Wextra -pedantic -fPIE
LDFLAGS     = -pthread -lpcap -fPIE

OBJ         = src/*.o

all: compile hawk eagle

compile:
	make -C src/

hawk: compile
	$(CC) $(CFLAGS) $(INCLUDES) -o bin/hawk  src/hawk.c  $(OBJ) $(LDFLAGS)

eagle: compile
	$(CC) $(CFLAGS) $(INCLUDES) -o bin/eagle src/eagle.c $(OBJ) $(LDFLAGS)

install: hawk eagle
	install -m 0755 bin/hawk  $(DESTDIR)/usr/bin/hawk
	install -m 0755 bin/eagle $(DESTDIR)/usr/bin/eagle

	install -m 0644 man/hawk.1.gz $(DESTDIR)/usr/share/man/man1/
	install -m 0644 man/eagle.1.gz $(DESTDIR)/usr/share/man/man1/

	setcap cap_net_raw,cap_sys_admin,cap_net_admin=ep /usr/bin/hawk
	setcap cap_net_raw,cap_sys_admin,cap_net_admin=ep /usr/bin/eagle


uninstall:
	rm -f $(DESTDIR)/usr/bin/hawk $(DESTDIR)/usr/bin/eagle

	rm -f $(DESTDIR)/usr/share/man/man1/hawk.1.gz
	rm -f $(DESTDIR)/usr/share/man/man1/eagle.1.gz

clean:
	rm -f bin/eagle bin/hawk
	make clean -C src/

