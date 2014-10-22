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

CFLAGS:=$(CFLAGS) -ffreestanding -fno-builtin
CPPFLAGS:=$(CPPFLAGS) -Imath

ARCHDIR:=src/arch/$(HOSTARCH)

include $(ARCHDIR)/make.config

CFLAGS:=$(CFLAGS) $(ARCH_CFLAGS)
CPFLAGS:=$(CPPFLAGS) $(ARCH_CPPFLAGS)

FREEOBJS:=\
$(ARCH_FREEOBJS) \
src/math/k_standard.o \
src/math/k_rem_pio2.o \
src/math/k_cos.o \
src/math/k_sin.o \
src/math/k_tan.o \
src/math/e_acos.o \
src/math/e_acosh.o \
src/math/e_asin.o \
src/math/e_atan2.o \
src/math/e_atanh.o \
src/math/e_cosh.o \
src/math/e_exp.o \
src/math/e_fmod.o \
src/math/e_gamma.o \
src/math/e_gamma_r.o \
src/math/e_hypot.o \
src/math/e_j0.o \
src/math/e_j1.o \
src/math/e_jn.o \
src/math/e_lgamma.o \
src/math/e_lgamma_r.o \
src/math/e_log.o \
src/math/e_log10.o \
src/math/e_pow.o \
src/math/e_rem_pio2.o \
src/math/e_remainder.o  \
src/math/e_scalb.o \
src/math/e_sinh.o \
src/math/e_sqrt.o \
src/math/w_acos.o \
src/math/w_acosh.o \
src/math/w_asin.o \
src/math/w_atan2.o \
src/math/w_atanh.o \
src/math/w_cosh.o \
src/math/w_exp.o \
src/math/w_fmod.o \
src/math/w_gamma.o \
src/math/w_gamma_r.o \
src/math/w_hypot.o \
src/math/w_j0.o \
src/math/w_j1.o \
src/math/w_jn.o \
src/math/w_lgamma.o \
src/math/w_lgamma_r.o \
src/math/w_log.o \
src/math/w_log10.o \
src/math/w_pow.o \
src/math/w_remainder.o \
src/math/w_scalb.o \
src/math/w_sinh.o \
src/math/w_sqrt.o \
src/math/s_asinh.o \
src/math/s_atan.o \
src/math/s_cbrt.o \
src/math/s_ceil.o \
src/math/s_copysign.o \
src/math/s_cos.o \
src/math/s_erf.o \
src/math/s_expm1.o \
src/math/s_fabs.o \
src/math/s_finite.o \
src/math/s_floor.o \
src/math/s_frexp.o \
src/math/s_ilogb.o \
src/math/s_isnan.o \
src/math/s_ldexp.o \
src/math/s_lib_version.o \
src/math/s_log1p.o \
src/math/s_logb.o \
src/math/s_matherr.o \
src/math/s_modf.o \
src/math/s_nextafter.o \
src/math/s_rint.o \
src/math/s_scalbn.o \
src/math/s_signgam.o \
src/math/s_significand.o \
src/math/s_sin.o \
src/math/s_tan.o \
src/math/s_tanh.o \

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
