#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

__attribute__((__noreturn__))
extern void abort(void);
extern char* itoa( int value, char* str, int base );
extern void* malloc( unsigned nbytes );
extern void* calloc( unsigned n, unsigned size );
extern void free( void* ap );

#ifdef __cplusplus
}
#endif

#endif
