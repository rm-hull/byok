#ifndef __STRUCT_REGISTERS_H
#define __STRUCT_REGISTERS_H

/* This defines what the stack looks like after an ISR was running */
typedef struct
{
    unsigned int esp;
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */
} registers_t;

#endif
