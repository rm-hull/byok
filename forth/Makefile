HOST?=$(shell ../scripts/default-host.sh)
HOSTARCH:=$(shell ../scripts/target-triplet-to-arch.sh $(HOST))

include ../make.config

CFLAGS?=-O2 -g
CPPFLAGS?=
LDFLAGS?=
LIBS?=

DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
INCLUDEDIR?=$(PREFIX)/include
LIBDIR?=$(EXEC_PREFIX)/lib

CFLAGS:=$(CFLAGS) -Wall -Wextra -ffreestanding -fno-builtin
CPPFLAGS:=$(CPPFLAGS) -Iinclude

FREEOBJS:=\
src/stack_machine/entry.o \
src/stack_machine/repl.o \
src/stack_machine/common.o \
src/stack_machine/error.o \
src/stack_machine/interpreter.o \
src/stack_machine/compiler.o \
src/stack_machine/slots.o \
src/util/history.o \
src/collections/list.o \
src/collections/dlist.o \
src/collections/stack.o \
src/collections/queue.o \
src/collections/hashtable.o \
src/primitives/stack_manip.o \
src/primitives/arithmetics.o \
src/primitives/io.o \
src/primitives/bit_logic.o \
src/primitives/comparison.o \
src/primitives/misc.o \
src/primitives/memory.o \
src/forth/resources.o \
src/editor/model.o \
src/editor/render.o \
src/editor/colorize.o \
src/editor/actions.o \

HOSTEDOBJS:=\

OBJS:=\
$(FREEOBJS) \
$(HOSTEDOBJS) \

BINARIES=libforth.a

all: $(BINARIES)

.PHONY: all clean install install-headers install-libs

libforth.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)

%.o: %.c
	$(CC) -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

%.o: %.S
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

clean:
	rm -f $(BINARIES) $(OBJS) *.o */*.o */*/*.o

install: install-headers install-libs

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -RT include $(DESTDIR)$(INCLUDEDIR)

install-libs: $(BINARIES)
	mkdir -p $(DESTDIR)$(LIBDIR)
	cp $(BINARIES) $(DESTDIR)$(LIBDIR)
