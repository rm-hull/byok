PROJECTS=libc fdlibm forth kernel
HOST?=$(shell ./scripts/default-host.sh)
HOSTARCH:=$(shell ./scripts/target-triplet-to-arch.sh $(HOST))

include make.config

all: clean build iso

clean:
	$(MAKE) -C fdlibm clean
	$(MAKE) -C libc clean
	$(MAKE) -C kernel clean
	$(MAKE) -C forth clean
	rm -rf sysroot
	rm -rf isodir
	rm -rf byok.iso

headers:
	DESTDIR="$(PWD)/sysroot" $(MAKE) -C fdlibm install-headers
	DESTDIR="$(PWD)/sysroot" $(MAKE) -C libc install-headers
	DESTDIR="$(PWD)/sysroot" $(MAKE) -C forth install-headers
	DESTDIR="$(PWD)/sysroot" $(MAKE) -C kernel install-headers

$(PROJECTS): headers
	DESTDIR="$(PWD)/sysroot" $(MAKE) -C $@ install

build: $(PROJECTS)

byok.iso: build
	mkdir -p isodir/boot/grub
	cp sysroot/boot/byok.kernel isodir/boot/byok.kernel
	cp boot/grub/grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o byok.iso isodir

iso: byok.iso

qemu: byok.iso
	qemu-system-$(HOSTARCH) -cdrom byok.iso

.PHONY: all $(PROJECTS)