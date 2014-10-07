# Declare constants used for creating a multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a header as in the Multiboot Standard.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Reserve a stack for the initial thread.
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

# The kernel entry point.
.section .text
.global _start
.type _start, @function
_start:
    movl $stack_top, %esp

    # Initialize the core kernel before running the global constructors.
    call kernel_early

    # Call the global constructors.
    call _init

    # Transfer control to the main kernel.
    call kernel_main

    # Hang if kernel_main unexpectedly returns.
    cli
    hlt
.Lhang:
    jmp .Lhang

# Global Descriptor Table
.global gdt_flush
.extern gdtp
gdt_flush:
    lgdt (gdtp)
    movl $0x10, %ax        # 0x10 is the offset in the GDT to our data segment
    movl %ax, %ds
    movl %ax, %es
    movl %ax, %es
    movl %ax, %es
    movl %ax, %es
    ljmp $0x08, $flush2    # 0x08 is the offset to our code segment: Far jump!
flush2:
    ret

# Interrupt Descriptor Table
.global idt_load
.extern idtp
idt_load:
    lidt (idtp)
    ret

# Interrupt Service Routines
.global divide_by_zero_exception
divide_by_zero_exception:
    cli
    pushw 0
    pushw 0
    jmp isr_common_stub

.extern fault_handler

# Common ISR stub. It saves the processor state, sets
# up for kernel mode segments, calls the C-level fault handler,
# and finally restores the stack frame.
isr_common_stub:
    pusha
    push %ds
    push %es
    push %fs
    push %gs
    mov %ax, 0x10   # Load the Kernel Data Segment descriptor!
    mov %ds, %ax
    mov %es, %ax
    mov %fs, %ax
    mov %gs, %ax
    mov %eax, %esp  # Push us the stack
    push %eax
    mov %eax, fault_handler
    call %eax        # A special call, preserves the 'eip' register
    popl %eax
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa
    add %esp, 8     # Cleans up the pushed error code and pushed ISR number
    iret            # pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!
