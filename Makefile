PREFIX ?= /usr/local
DESTDIR ?=
MANDIR ?= man/man
INSTALLDIR = install -d
INSTALLBIN = install -m 755
INSTALLMAN = install -m 644

LDFLAGS = -ltag_c

PROJECT = taggit
HEADERS = taggit.h bsdgetopt.c
SOURCES = taggit.c list.c list_human.c list_machine.c tag.c
OBJS = taggit.o list.o list_human.o list_machine.o tag.o
CFLAGS += -Wall -Wextra -Wmissing-declarations
REALLYJUSTCFLAGS += -Wnested-externs -Wmissing-prototypes -Wstrict-prototypes
CC = cc

SRCXX = taglib_ext.cpp
OBJXX = taglib_ext.o
HDRXX = taglib_ext.h
CXXFLAGS = $(CFLAGS)
CXXFLAGS += -pedantic
CXX = c++

all: $(PROJECT)

depend: $(SOURCES) $(SRCXX)
	mkdep $(CFLAGS) $(SOURCES) $(SRCXX)

install:
	$(INSTALLDIR) $(DESTDIR)$(PREFIX)/bin
	$(INSTALLBIN) taggit $(DESTDIR)$(PREFIX)/bin/
	$(INSTALLDIR) $(DESTDIR)$(PREFIX)/$(MANDIR)1
	$(INSTALLMAN) taggit.1 $(DESTDIR)$(PREFIX)/$(MANDIR)1/

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/taggit
	rm -f $(DESTDIR)$(PREFIX)/$(MANDIR)1/taggit.1

taglib_ext.o: taglib_ext.cpp taglib_ext.h
	$(CXX) $(CXXFLAGS) -o taglib_ext.o -c taglib_ext.cpp

.c.o:
	$(CC) $(CFLAGS) $(REALLYJUSTCFLAGS) -o $@ -c $<

$(PROJECT): $(OBJS) $(OBJXX) $(HEADERS) $(HDRXX)
	$(CC) $(CFLAGS) $(REALLYJUSTCFLAGS) $(LDFLAGS) -o $@ $(OBJXX) $(OBJS)

clean:
	rm -f *.o taggit *.1 .depend

distclean: clean
	rm -f tags
	rm -Rf devdoc

doc: $(PROJECT).1

tags:
	ctags -R .

devdoc:
	doxygen doxygen.taggit

$(PROJECT).1: $(PROJECT).t2t
	txt2tags --target man -o- $(PROJECT).t2t | sed -e '/^$$/d' -e 's/^\\e$$//' > $(PROJECT).1

-include .depend

.PHONY: all depend doc clean install uninstall tags devdoc distclean
