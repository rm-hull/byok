# A simple bootloader

## Setup

For development and testing, using [QEMU](http://wiki.qemu.org/Main_Page) is recommended. 
Using bochs, virtual box or VMware should equally work.

From Ubuntu command line:

    $ sudo apt-get install qemu-system-x86 ghex nasm

## A simple bootloader

To test that the setup is working, create an empty file:

    $ dd if=/dev/zero of=img/simple_boot_sect.bin

Then edit this using _ghex_: add E9, FD, FF at position 0, and 55, AA as
last two bytes. The first few bytes are an endless loop, while 0xAA55 is
a magic number that tells the BIOS that this is a bootable device.

The output from _od_ should look like:

    $ od -t x1z img/simple_boot_sect.bin
    0000000 e9 fd ff 00 00 00 00 00 00 00 00 00 00 00 00 00  >................<
    0000020 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  >................<
    *
    0000760 00 00 00 00 00 00 00 00 00 00 00 00 00 00 55 aa  >..............U.<
    0001000

To test this:

    $ qemu-system-i386 img/simple_boot_sect.bin
