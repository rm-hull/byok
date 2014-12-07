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
CPPFLAGS:=$(CPPFLAGS) -D__is_byok_libc -Iinclude

ARCHDIR:=src/arch/$(HOSTARCH)

include $(ARCHDIR)/make.config

CFLAGS:=$(CFLAGS) $(ARCH_CFLAGS)
CPFLAGS:=$(CPPFLAGS) $(ARCH_CPPFLAGS)

FREEOBJS:=\
$(ARCH_FREEOBJS) \
src/stdio/printf.o \
src/stdio/putchar.o \
src/stdio/puts.o \
src/stdlib/abort.o \
src/stdlib/assert.o \
src/stdlib/itoa.o \
src/stdlib/dtoa.o \
src/stdlib/atoi.o \
src/stdlib/malloc.o \
src/string/memcmp.o \
src/string/memcpy.o \
src/string/memmove.o \
src/string/memset.o \
src/string/strlen.o \
src/string/strchr.o \
src/string/strdup.o \
src/string/strpbrk.o \
src/string/strtok.o \
src/string/strspn.o \
src/string/strtoupper.o \
src/string/trim.o \
src/regex/regex.o \
src/regex/re_fail.o \

HOSTEDOBJS:=\
$(ARCH_HOSTEDOBJS) \

OBJS:=\
$(FREEOBJS) \
$(HOSTEDOBJS) \

BINARIES=libc.a libg.a

all: $(BINARIES)

.PHONY: all clean install install-headers install-libs

libc.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)

libg.a:
	$(AR) rcs $@

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
