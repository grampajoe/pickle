PREFIX ?= /usr/local
MEDIA_PREFIX ?= /share/pickle
BINARY := pickle

.PHONY: all install clean

all:
	$(MAKE) -C src

install: all
	install -m 0755 -d $(PREFIX)/bin/
	install -m 0755 -d $(PREFIX)$(MEDIA_PREFIX)/images/
	install -m 0755 -d $(PREFIX)$(MEDIA_PREFIX)/fonts/
	install -m 0755 src/$(BINARY) $(PREFIX)/bin/
	install -m 0644 images/* $(PREFIX)$(MEDIA_PREFIX)/images/
	install -m 0644 fonts/* $(PREFIX)$(MEDIA_PREFIX)/fonts/

uninstall:
	rm -f $(PREFIX)/bin/$(BINARY)
	rm -rf $(PREFIX)$(MEDIA_PREFIX)

clean:
	$(MAKE) -C src clean
