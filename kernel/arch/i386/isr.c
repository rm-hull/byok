#include <stdio.h>

#include <kernel/system.h>

extern void divide_by_zero_exception();
extern void debug_exception();
extern void non_maskable_interrupt();
extern void breakpoint();
extern void overflow();
extern void bound_exception();
extern void invalid_opcode();
extern void fpu_not_available();
extern void double_fault();
extern void coprocessor_segment_overrun();
extern void invalid_tss();
extern void segment_not_present();
extern void stack_exception();
extern void general_protection();
extern void page_fault();
extern void reserved_15();
extern void floating_point_error();
extern void alignment_check();
extern void machine_check();
extern void reserved_19();
extern void reserved_20();
extern void reserved_21();
extern void reserved_22();
extern void reserved_23();
extern void reserved_24();
extern void reserved_25();
extern void reserved_26();
extern void reserved_27();
extern void reserved_28();
extern void reserved_29();
extern void reserved_30();
extern void reserved_31();

char *exception_messages[] =
{
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "FPU Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved (15)",
    "Floating-Point Error",
    "Alignment Check",
    "Machine Check",
    "Reserved (19)",
    "Reserved (20)",
    "Reserved (21)",
    "Reserved (22)",
    "Reserved (23)",
    "Reserved (24)",
    "Reserved (25)",
    "Reserved (26)",
    "Reserved (27)",
    "Reserved (28)",
    "Reserved (29)",
    "Reserved (30)",
    "Reserved (31)"
};

void fault_handler(struct regs *r)
{
    if (r->int_no < 32)
    {
        printf("%s Exception. System Halted!\n", exception_messages[r->int_no]);
        printf("Error-code: %d\n", r->err_code);
        printf("Registers:\n");
        printf("  ds=0x%x, es=0x%x, fs=0x%x, gs=0x%x\n", r->ds, r->es, r->fs, r->gs);
        printf("  eax=0x%x, ebx=0x%x, ecx=0x%x, edx=0x%x\n", r->eax, r->ebx, r->ecx, r->edx);
        printf("  ebp=0x%x, esp=0x%x, esi=0x%x, edi=0x%x\n", r->ebp, r->esp, r->esi, r->edi);
        printf("  ss=0x%x, useresp=0x%x, eflags=0x%x, cs=0x%x, eip=0x%x \n", r->ss, r->useresp, r->eflags, r->cs, r->eip);
        for (;;);
    }
}

void isrs_install() {
    idt_set_gate(0, (unsigned)divide_by_zero_exception, 0x08, 0x8E);
    idt_set_gate(1, (unsigned)debug_exception, 0x08, 0x8E);
    idt_set_gate(2, (unsigned)non_maskable_interrupt, 0x08, 0x8F);
    idt_set_gate(3, (unsigned)breakpoint, 0x08, 0x8F);
    idt_set_gate(4, (unsigned)overflow, 0x08, 0x8F);
    idt_set_gate(5, (unsigned)bound_exception, 0x08, 0x8F);
    idt_set_gate(6, (unsigned)invalid_opcode, 0x08, 0x8F);
    idt_set_gate(7, (unsigned)fpu_not_available, 0x08, 0x8F);
    idt_set_gate(8, (unsigned)double_fault, 0x08, 0x8F);
    idt_set_gate(9, (unsigned)coprocessor_segment_overrun, 0x08, 0x8F);
    idt_set_gate(10, (unsigned)invalid_tss, 0x08, 0x8F);
    idt_set_gate(11, (unsigned)segment_not_present, 0x08, 0x8F);
    idt_set_gate(12, (unsigned)stack_exception, 0x08, 0x8F);
    idt_set_gate(13, (unsigned)general_protection, 0x08, 0x8F);
    idt_set_gate(14, (unsigned)page_fault, 0x08, 0x8F);
    idt_set_gate(15, (unsigned)reserved_15, 0x08, 0x8F);
    idt_set_gate(16, (unsigned)floating_point_error, 0x08, 0x8F);
    idt_set_gate(17, (unsigned)alignment_check, 0x08, 0x8F);
    idt_set_gate(18, (unsigned)machine_check, 0x08, 0x8F);
    idt_set_gate(19, (unsigned)reserved_19, 0x08, 0x8F);
    idt_set_gate(20, (unsigned)reserved_20, 0x08, 0x8F);
    idt_set_gate(21, (unsigned)reserved_21, 0x08, 0x8F);
    idt_set_gate(22, (unsigned)reserved_22, 0x08, 0x8F);
    idt_set_gate(23, (unsigned)reserved_23, 0x08, 0x8F);
    idt_set_gate(24, (unsigned)reserved_24, 0x08, 0x8F);
    idt_set_gate(25, (unsigned)reserved_25, 0x08, 0x8F);
    idt_set_gate(26, (unsigned)reserved_26, 0x08, 0x8F);
    idt_set_gate(27, (unsigned)reserved_27, 0x08, 0x8F);
    idt_set_gate(28, (unsigned)reserved_28, 0x08, 0x8F);
    idt_set_gate(29, (unsigned)reserved_29, 0x08, 0x8F);
    idt_set_gate(30, (unsigned)reserved_30, 0x08, 0x8F);
    idt_set_gate(31, (unsigned)reserved_31, 0x08, 0x8F);
}