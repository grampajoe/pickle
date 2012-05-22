DESTDIR =
PREFIX := $(DESTDIR)/usr
BINDIR := $(PREFIX)/bin
DATADIR := $(PREFIX)/share/pickle
BINARY := pickle

.PHONY: all install clean

all:
	$(MAKE) -C src PREFIX=$(PREFIX) DATADIR=$(DATADIR)

install: all
	install -m 0755 -d $(BINDIR)
	install -m 0755 -d $(DATADIR)/images/
	install -m 0755 -d $(DATADIR)/fonts/
	install -m 0755 src/$(BINARY) $(BINDIR)
	install -m 0644 images/* $(DATADIR)/images/
	install -m 0644 fonts/* $(DATADIR)/fonts/

uninstall:
	rm -f $(BINDIR)$(BINARY)
	rm -rf $(DATADIR)

clean:
	$(MAKE) -C src clean
