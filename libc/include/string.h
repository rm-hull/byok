#ifndef _STRING_H
#define _STRING_H 1

#include <sys/cdefs.h>
#include <ctype.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

extern int memcmp(const void*, const void*, size_t);
extern void *memcpy(void* __restrict, const void* __restrict, size_t);
extern void *memmove(void*, const void*, size_t);
extern void *memset(void*, int, size_t);
extern int strlen(const char*);
extern int strcmp(const char*, const char*);
extern char *strdup(const char*);
extern char *strpbrk(const char*, const char*);
extern char *strtok_r(const char*, const char*, char**);
extern char *strtok(const char*, const char*);
extern char *strspn(const char*, const char*);
extern char *strchr(const char*, char);
extern char *strtoupper(char*);
extern char *trim(char*);

#ifdef __cplusplus
}
#endif

#endif
