#ifndef __SYSTEM_H
#define __SYSTEM_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern unsigned char inportb (uint16_t port);
extern void outportb (uint16_t port, unsigned char data);

#ifdef __cplusplus
}
#endif

#endif
