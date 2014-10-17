#ifndef _STRING_H
#define _STRING_H 1

#include <sys/cdefs.h>

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

extern int memcmp(const void*, const void*, size_t);
extern void* memcpy(void* __restrict, const void* __restrict, size_t);
extern void* memmove(void*, const void*, size_t);
extern void* memset(void*, int, size_t);
extern int strlen(const char*);
extern char *strdup(const char*);

#ifdef __cplusplus
}
#endif

#endif
