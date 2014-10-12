#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

#if NDEBUG
#define assert(expr) 0
#else
#define assert(expr) ((expr) ? 0 :  __assert_failed(__STRING(expr), __FILE__, __LINE__))
#endif

#define abort()       __abort(__FILE__, __LINE__)

__attribute__((__noreturn__))
extern void __assert_failed(char *expr, char *file, int line);

__attribute__((__noreturn__))
extern void __abort(char *file, int line);

// TODO: these two should be in string.h ??
extern char* itoa( int value, char* str, int base );
extern char* dtoa( double value, char* str );

extern void* malloc( unsigned nbytes );
extern void* calloc( unsigned n, unsigned size );
extern void free( void* ap );

#ifdef __cplusplus
}
#endif

#endif
