PREFIX ?= /usr/local
DESTDIR ?=
MANDIR ?= man/man
INSTALLDIR = install -d
INSTALLBIN = install -m 755
INSTALLMAN = install -m 644

POSIX_SHELL ?= /bin/sh

LDFLAGS = `pkg-config --libs taglib_c`

OPTIM ?= -Os
DEBUG ?= -ggdb

CXX = clang++

PROJECT = taggit
HEADERS = taggit.h bsdgetopt.c
SOURCES = taggit.cpp info.cpp setup.cpp cmdline.cpp value.cpp
SOURCES += list.cpp list-human.cpp list-machine.cpp file-spec.cpp
OBJS = taggit.o info.o setup.o cmdline.o value.o
OBJS += list.o list-human.o list-machine.o file-spec.o
CXXFLAGS += `pkg-config --cflags taglib`
CXXFLAGS += -W -Wall -Wextra
CXXFLAGS += -Wmissing-declarations -std=c++11

all:
	$(MAKE) _info
	$(MAKE) $(PROJECT)

_info: version-magic.sh
	$(POSIX_SHELL) ./version-magic.sh

depend:
	$(MAKE) _info
	$(MAKE) _depend

-include version-magic.make

_depend: $(SOURCES)
	mkdep $(CXXFLAGS) $(SOURCES)

install:
	$(INSTALLDIR) $(DESTDIR)$(PREFIX)/bin
	$(INSTALLBIN) taggit $(DESTDIR)$(PREFIX)/bin/
	$(INSTALLDIR) $(DESTDIR)$(PREFIX)/$(MANDIR)1
	$(INSTALLMAN) taggit.1 $(DESTDIR)$(PREFIX)/$(MANDIR)1/

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/taggit
	rm -f $(DESTDIR)$(PREFIX)/$(MANDIR)1/taggit.1

.cpp.o:
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(PROJECT): $(OBJS) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(OBJS)

clean:
	rm -f *.o taggit *.1 .depend git-version.h version-magic.make

distclean: clean
	rm -f tags TAGS
	rm -Rf devdoc

doc: $(PROJECT).1

tag: tags

tags:
	ctags . *.c *.cpp *.h
	ctags -e . *.c *.cpp *.h

devdoc:
	doxygen doxygen.taggit

lint:
	-splint -preproc -linelen 128 -standard -warnposix -booltype boolean +charintliteral -nullassign $(SOURCES)

$(PROJECT).1: $(PROJECT).t2t
	txt2tags --target man -o- $(PROJECT).t2t | sed -e '/^$$/d' -e 's/^\\e$$//' > $(PROJECT).1

-include .depend

.PHONY: all depend doc clean install uninstall tags tag devdoc distclean _depend _info lint
