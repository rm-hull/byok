#ifndef _STDIO_H
#define _STDIO_H 1

#include <sys/cdefs.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

#define stderr 2
#define fputs(f,c) printf("STDERR:%s",f)

extern int vprintf(const char* __restrict, va_list);
extern int printf(const char* __restrict, ...);
extern int putchar(int);
extern int puts(const char*);

#ifdef __cplusplus
}
#endif

#endif
