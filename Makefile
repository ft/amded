PREFIX ?= /usr/local
DESTDIR ?=
MANDIR ?= man/man
INSTALLDIR = install -d
INSTALLBIN = install -m 755
INSTALLMAN = install -m 644

PROJECT = taggit
HEADERS = taggit.h bsdgetopt.c
SOURCES = taggit.c list.c list_human.c list_machine.c tag.c
OBJS = taggit.o list.o list_human.o list_machine.o tag.o
CFLAGS = -Wall -Wextra
LDFLAGS = -ltag_c
CC = cc

all: $(PROJECT)

depend: $(SOURCES)
	mkdep $(CFLAGS) $(SOURCES)

install:
	$(INSTALLDIR) $(DESTDIR)$(PREFIX)/bin
	$(INSTALLBIN) taggit $(DESTDIR)$(PREFIX)/bin/
	$(INSTALLDIR) $(DESTDIR)$(PREFIX)/$(MANDIR)1
	$(INSTALLMAN) taggit.1 $(DESTDIR)$(PREFIX)/$(MANDIR)1/

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/taggit
	rm -f $(DESTDIR)$(PREFIX)/$(MANDIR)1/taggit.1

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<

$(PROJECT): $(OBJS) $(HEADERS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS)

clean:
	rm -f *.o taggit *.1 .depend

doc: $(PROJECT).1

$(PROJECT).1: $(PROJECT).t2t
	txt2tags --target man -o- $(PROJECT).t2t | sed -e '/^$$/d' -e 's/^\\e$$//' > $(PROJECT).1

-include .depend

.PHONY: all depend doc clean install uninstall
