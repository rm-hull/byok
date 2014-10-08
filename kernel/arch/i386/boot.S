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
    mov $0x10, %ax        # 0x10 is the offset in the GDT to our data segment
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %es
    mov %ax, %es
    mov %ax, %es
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
# TODO: macros for these
.global divide_by_zero_exception
divide_by_zero_exception:
    cli
    push $0
    push $0
    jmp isr_common_stub

.global debug_exception
debug_exception:
    cli
    push $0
    push $1
    jmp isr_common_stub

.global non_maskable_interrupt
non_maskable_interrupt:
    cli
    push $0
    push $2
    jmp isr_common_stub

.global breakpoint
breakpoint:
    cli
    push $0
    push $3
    jmp isr_common_stub

.global overflow
overflow:
    cli
    push $0
    push $4
    jmp isr_common_stub

.global bound_exception
bound_exception:
    cli
    push $0
    push $5
    jmp isr_common_stub

.global invalid_opcode
invalid_opcode:
    cli
    push $0
    push $6
    jmp isr_common_stub

.global fpu_not_available
fpu_not_available:
    cli
    push $0
    push $7
    jmp isr_common_stub

.global double_fault
double_fault:
    cli
    push $8
    jmp isr_common_stub

.global coprocessor_segment_overrun
coprocessor_segment_overrun:
    cli
    push $0
    push $9
    jmp isr_common_stub

.global invalid_tss
invalid_tss:
    cli
    push $10
    jmp isr_common_stub

.global segment_not_present
segment_not_present:
    cli
    push $11
    jmp isr_common_stub

.global stack_exception
stack_exception:
    cli
    push $12
    jmp isr_common_stub

.global general_protection
general_protection:
    cli
    push $13
    jmp isr_common_stub

.global page_fault
page_fault:
    cli
    push $14
    jmp isr_common_stub

.global reserved_15
reserved_15:
    cli
    push $0
    push $15
    jmp isr_common_stub

.global floating_point_error
floating_point_error:
    cli
    push $0
    push $16
    jmp isr_common_stub

.global alignment_check
alignment_check:
    cli
    push $0
    push $17
    jmp isr_common_stub

.global machine_check
machine_check:
    cli
    push $0
    push $18
    jmp isr_common_stub

.global reserved_19
reserved_19:
    cli
    push $0
    push $19
    jmp isr_common_stub

.global reserved_20
reserved_20:
    cli
    push $0
    push $20
    jmp isr_common_stub

.global reserved_21
reserved_21:
    cli
    push $0
    push $21
    jmp isr_common_stub

.global reserved_22
reserved_22:
    cli
    push $0
    push $22
    jmp isr_common_stub

.global reserved_23
reserved_23:
    cli
    push $0
    push $23
    jmp isr_common_stub

.global reserved_24
reserved_24:
    cli
    push $0
    push $24
    jmp isr_common_stub

.global reserved_25
reserved_25:
    cli
    push $0
    push $25
    jmp isr_common_stub

.global reserved_26
reserved_26:
    cli
    push $0
    push $26
    jmp isr_common_stub

.global reserved_27
reserved_27:
    cli
    push $0
    push $27
    jmp isr_common_stub

.global reserved_28
reserved_28:
    cli
    push $0
    push $28
    jmp isr_common_stub

.global reserved_29
reserved_29:
    cli
    push $0
    push $29
    jmp isr_common_stub

.global reserved_30
reserved_30:
    cli
    push $0
    push $30
    jmp isr_common_stub

.global reserved_31
reserved_31:
    cli
    push $0
    push $31
    jmp isr_common_stub



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
    mov $0x10, %ax   # Load the Kernel Data Segment descriptor!
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %esp, %eax  # Push us the stack
    pushl %eax
    movl $fault_handler, %eax
    call %eax        # A special call, preserves the 'eip' register

    popl %eax
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa
    
    add $8, %esp     # Cleans up the pushed error code and pushed ISR number
    iret            # pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!

.section data
.global _end
_end:
