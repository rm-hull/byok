#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <stddef.h>
#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

#if NDEBUG
#define assert(expr) 0
#else
#define assert(expr) do { ((expr) ? 0 :  __assert_failed(__STRING(expr), __FILE__, __LINE__, __func__)); } while(0)
#endif

#define abort()       __abort(__FILE__, __LINE__)

__attribute__((__noreturn__))
extern void __assert_failed(const char *expr, const char *file, int line, const char *func);

__attribute__((__noreturn__))
extern void __abort(char *file, int line);

// TODO: these should be in string.h ??
extern char* itoa(int value, char *str, int base);
extern char* dtoa(double value, char *str);
extern int atoi(char *str, int base);

extern void* malloc(unsigned nbytes);
extern void* calloc(unsigned n, unsigned size);
extern void free(void *ap );
extern void qsort(void *base, unsigned num, unsigned width, int (*comp)(const void *, const void *));

#define min(a,b) (a < b ? a : b)
#define max(a,b) (a > b ? a : b)

#ifdef __cplusplus
}
#endif

#endif
