#!/bin/sh
set -e
. ./build.sh
 
mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub
 
cp sysroot/boot/hulix.kernel isodir/boot/hulix.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "hulix" {
	multiboot /boot/hulix.kernel
}
EOF
grub-mkrescue -o hulix.iso isodir
