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

.section .bss

.macro isr_wrapper pad, exception, label
.global \label
\label:
    cli
.rept \pad
    push $0
.endr
    push \exception
    jmp isr_common_stub
.endm

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
    mov $0x10, %ax          # 0x10 is the offset in the GDT to our data segment
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %es
    mov %ax, %es
    mov %ax, %es
    ljmp $0x08, $flush2     # 0x08 is the offset to our code segment: Far jump!
flush2:
    ret

# Interrupt Descriptor Table
.global idt_load
.extern idtp
idt_load:
    lidt (idtp)
    ret

# Interrupt Service Routines
    isr_wrapper 1, $0,  divide_by_zero_exception
    isr_wrapper 1, $1,  debug_exception
    isr_wrapper 1, $2,  non_maskable_interrupt
    isr_wrapper 1, $3,  breakpoint
    isr_wrapper 1, $4,  overflow
    isr_wrapper 1, $5,  bound_exception
    isr_wrapper 1, $6,  invalid_opcode
    isr_wrapper 1, $7,  fpu_not_available
    isr_wrapper 0, $8,  double_fault
    isr_wrapper 1, $9,  coprocessor_segment_overrun
    isr_wrapper 0, $10, invalid_tss
    isr_wrapper 0, $11, segment_not_present
    isr_wrapper 0, $12, stack_exception
    isr_wrapper 0, $13, general_protection
    isr_wrapper 0, $14, page_fault
    isr_wrapper 1, $15, reserved_15
    isr_wrapper 1, $16, floating_point_error
    isr_wrapper 1, $17, alignment_check
    isr_wrapper 1, $18, machine_check
    isr_wrapper 1, $19, reserved_19
    isr_wrapper 1, $20, reserved_20
    isr_wrapper 1, $21, reserved_21
    isr_wrapper 1, $22, reserved_22
    isr_wrapper 1, $23, reserved_23
    isr_wrapper 1, $24, reserved_24
    isr_wrapper 1, $25, reserved_25
    isr_wrapper 1, $26, reserved_26
    isr_wrapper 1, $27, reserved_27
    isr_wrapper 1, $28, reserved_28
    isr_wrapper 1, $29, reserved_29
    isr_wrapper 1, $30, reserved_30
    isr_wrapper 1, $31, reserved_31

.extern fault_handler

# Common ISR stub. It saves the processor state, sets
# up for kernel mode segments, calls the C-level fault handler,
# and finally restores the stack frame.
isr_common_stub:
    pusha
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs
    mov $0x10, %ax      # Load the Kernel Data Segment descriptor!
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %esp, %eax      # Push us the stack
    pushl %eax
    movl $fault_handler, %eax
    call %eax           # A special call, preserves the 'eip' register

    popl %eax
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa

    add $8, %esp        # Cleans up the pushed error code and pushed ISR number
    iret                # pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!

.section data
.global _end
_end:
