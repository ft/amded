PROJECT = taggit
HEADERS = taggit.h bsdgetopt.c
SOURCES = taggit.c list_human.c list_machine.c tag.c
OBJS = taggit.o list_human.o list_machine.o tag.o
CFLAGS = -Wall -Wextra
LDFLAGS = -ltag_c

all: $(PROJECT)

depend: $(SOURCES)
	mkdep $(CFLAGS) $(SOURCES)

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

.PHONY: all depend doc clean
