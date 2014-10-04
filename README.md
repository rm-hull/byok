# Hulix - a bare-metal x86 Forth interpreter

A toy implementation of a [Forth](https://en.wikipedia.org/wiki/Forth_(programming_language))
interpreter, running directly on top of an x86 machine.

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


# License

The MIT License (MIT)

Copyright (c) 2014 Richard Hull

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

