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

# Don't moan about warnings in fdlibm
#CFLAGS:=$(CFLAGS) -Wall -Wextra -ffreestanding -fno-builtin
CFLAGS:=$(CFLAGS) -ffreestanding -fno-builtin
CPPFLAGS:=$(CPPFLAGS) -D__is_byok_libc -Imath

ARCHDIR:=arch/$(HOSTARCH)

include $(ARCHDIR)/make.config

CFLAGS:=$(CFLAGS) $(ARCH_CFLAGS)
CPFLAGS:=$(CPPFLAGS) $(ARCH_CPPFLAGS)

FREEOBJS:=\
$(ARCH_FREEOBJS) \
math/k_standard.o \
math/k_rem_pio2.o \
math/k_cos.o \
math/k_sin.o \
math/k_tan.o \
math/e_acos.o \
math/e_acosh.o \
math/e_asin.o \
math/e_atan2.o \
math/e_atanh.o \
math/e_cosh.o \
math/e_exp.o \
math/e_fmod.o \
math/e_gamma.o \
math/e_gamma_r.o \
math/e_hypot.o \
math/e_j0.o \
math/e_j1.o \
math/e_jn.o \
math/e_lgamma.o \
math/e_lgamma_r.o \
math/e_log.o \
math/e_log10.o \
math/e_pow.o \
math/e_rem_pio2.o \
math/e_remainder.o  \
math/e_scalb.o \
math/e_sinh.o \
math/e_sqrt.o \
math/w_acos.o \
math/w_acosh.o \
math/w_asin.o \
math/w_atan2.o \
math/w_atanh.o \
math/w_cosh.o \
math/w_exp.o \
math/w_fmod.o \
math/w_gamma.o \
math/w_gamma_r.o \
math/w_hypot.o \
math/w_j0.o \
math/w_j1.o \
math/w_jn.o \
math/w_lgamma.o \
math/w_lgamma_r.o \
math/w_log.o \
math/w_log10.o \
math/w_pow.o \
math/w_remainder.o \
math/w_scalb.o \
math/w_sinh.o \
math/w_sqrt.o \
math/s_asinh.o \
math/s_atan.o \
math/s_cbrt.o \
math/s_ceil.o \
math/s_copysign.o \
math/s_cos.o \
math/s_erf.o \
math/s_expm1.o \
math/s_fabs.o \
math/s_finite.o \
math/s_floor.o \
math/s_frexp.o \
math/s_ilogb.o \
math/s_isnan.o \
math/s_ldexp.o \
math/s_lib_version.o \
math/s_log1p.o \
math/s_logb.o \
math/s_matherr.o \
math/s_modf.o \
math/s_nextafter.o \
math/s_rint.o \
math/s_scalbn.o \
math/s_signgam.o \
math/s_significand.o \
math/s_sin.o \
math/s_tan.o \
math/s_tanh.o \

HOSTEDOBJS:=\
$(ARCH_HOSTEDOBJS) \

OBJS:=\
$(FREEOBJS) \
$(HOSTEDOBJS) \

BINARIES=libm.a

all: $(BINARIES)

.PHONY: all clean install install-headers install-libs

libm.a: $(OBJS)
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
