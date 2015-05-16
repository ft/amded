PREFIX ?= /usr/local
DESTDIR ?=
MANDIR ?= man/man
INSTALLDIR = install -d
INSTALLBIN = install -m 755
INSTALLMAN = install -m 644

POSIX_SHELL ?= /bin/sh

LDFLAGS = `pkg-config --libs taglib`
LDFLAGS += `pkg-config --libs jsoncpp`

OPTIM ?= -Os
DEBUG ?= -ggdb -O0

CXX = g++

PROJECT = amded
DISTHEAD = master

HEADERS = amded.h bsdgetopt.c
SOURCES = amded.cpp info.cpp setup.cpp cmdline.cpp value.cpp
SOURCES += list.cpp list-human.cpp list-machine.cpp list-json.cpp file-spec.cpp
SOURCES += file-type.cpp tag-implementation.cpp tag.cpp strip.cpp
OBJS = amded.o info.o setup.o cmdline.o value.o
OBJS += list.o list-human.o list-machine.o list-json.o file-spec.o
OBJS += file-type.o tag-implementation.o tag.o strip.o
CXXFLAGS += `pkg-config --cflags taglib`
CXXFLAGS += `pkg-config --cflags jsoncpp`
CXXFLAGS += -W -Wall -Wextra
CXXFLAGS += -Wmissing-declarations -std=c++11
CXXFLAGS += $(ADDTOCXXFLAGS)
CXXFLAGS += $(OPTIM)
#CXXFLAGS += $(DEBUG)

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
	$(INSTALLBIN) amded $(DESTDIR)$(PREFIX)/bin/
	[ -f amded.1 ] && $(INSTALLDIR) $(DESTDIR)$(PREFIX)/$(MANDIR)1 || true
	[ -f amded.1 ] && $(INSTALLMAN) amded.1 $(DESTDIR)$(PREFIX)/$(MANDIR)1/ || true

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/amded
	rm -f $(DESTDIR)$(PREFIX)/$(MANDIR)1/amded.1

.cpp.o:
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(PROJECT): $(OBJS) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

clean:
	rm -f *.o amded *.1 .depend git-version.h version-magic.make

distclean: clean
	rm -f tags TAGS
	rm -Rf devdoc

dist:
	git archive --format=tar.gz --prefix="$(PROJECT)-$(DISTHEAD)/" "$(DISTHEAD)" > "$(PROJECT)-$(DISTHEAD).tar.gz"

doc: $(PROJECT).1

tag: tags

tags:
	ctags . *.c *.cpp *.h
	ctags -e . *.c *.cpp *.h

devdoc:
	doxygen doxygen.amded

lint:
	-splint -preproc -linelen 128 -standard -warnposix -booltype boolean +charintliteral -nullassign $(SOURCES)

$(PROJECT).1: $(PROJECT).t2t
	txt2tags --target man -o- $(PROJECT).t2t | sed -e '/^$$/d' -e 's/^\\e$$//' > $(PROJECT).1

-include .depend

.PHONY: all depend dist doc clean install uninstall tags tag devdoc distclean _depend _info lint
