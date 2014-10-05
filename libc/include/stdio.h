#ifndef _STDIO_H
#define _STDIO_H 1

#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

extern int printf(const char* __restrict, ...);
extern int putchar(int);
extern int puts(const char*);

#ifdef __cplusplus
}
#endif

#endif
