#ifndef _SLOTS_H
#define _SLOTS_H

#include <stack_machine/context.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void slot_flush(int n);
extern char *slot_buffer(int n);
extern void slot_mark_dirty(int n);

#ifdef __cplusplus
}
#endif

#endif
