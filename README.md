# Hulix - a bare-metal x86 Forth interpreter

A toy implementation of a [Forth](https://en.wikipedia.org/wiki/Forth_(programming_language))
interpreter, running directly on top of an x86 machine.

## Setup / Building

For development and testing, using [QEMU](http://wiki.qemu.org/Main_Page) is recommended. 
Using bochs, virtual box or VMware should equally work.

From Ubuntu command line:

    $ sudo apt-get install qemu-system-x86 ghex nasm xorriso make

Building requires a barebones [i686-elf cross compiler](https://github.com/rm-hull/i686-elf) 
installing first. Follow the instructions on that page, and then check it works by running:

    $ i686-elf-gcc --version
    i686-elf-gcc (GCC) 4.9.1
    Copyright (C) 2014 Free Software Foundation, Inc.
    This is free software; see the source for copying conditions.  There is NO
    warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

Next, to completely rebuild the kernel:

    $ make clean
    $ make build

To build an ISO image of the kernel:

    $ make iso

This may then either be burned onto a CD-ROM, or loaded onto a USB stick. Alternatively, 
running it in qemu:

    $ make qemu

## Implementation Notes

Portions of infrastructure of this interpreter are derived from [osdev](http://wiki.osdev.org/Main_Page)'s 
meaty skeleton.

## TODO

Some basic I/O operations need writing before work on the interpreter can be started

* ~~Scrolling TTY~~
* ~~Control character output~~
* ~~printf %d and %x formats~~
* Debug printing
* Global Descriptor Table
* Interrupt Descriptor / ISR / Exception handling
* Keyboard entry & readline input

## Contributors

Pull requests are always welcome. There is plenty to do, please let me know
if you can help out; submit a request for commit access. 

## References

### Forth

* http://thinking-forth.sourceforge.net/
* http://www.jupiter-ace.co.uk/index_Forth_general.html
* https://en.wikipedia.org/wiki/Forth_(programming_language)
* http://wiki.forthfreak.net/index.cgi?jsforth 

### OS Development

* http://www.cs.bham.ac.uk/~exr/lectures/opsys/10_11/lectures/os-dev.pdf
* http://www.osdever.net/tutorials/view/brans-kernel-development-tutorial
* http://www.acm.uiuc.edu/sigops/roll_your_own/i386/idt.html
* http://wiki.osdev.org/Main_Page
* http://wiki.qemu.org/Main_Page

## License

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

