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

CFLAGS:=$(CFLAGS) -Wall -Wextra
CPPFLAGS:=$(CPPFLAGS) -D__is_byok_libc -Iinclude
LIBK_CFLAGS:=$(CFLAGS) -ffreestanding -fno-builtin
LIBK_CPPFLAGS:=$(CPPFLAGS) -D__is_byok_kernel

FREEOBJS:=\
stack_machine/entry.o \
stack_machine/repl.o \
stack_machine/common.o \
stack_machine/error.o \
collections/list.o \
collections/stack.o \
collections/queue.o \
collections/hashtable.o \
words/stack_manip.o \
words/arithmetics.o \
words/io.o \

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
