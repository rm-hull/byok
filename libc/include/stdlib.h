#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

#define abort() __abort__(__FILE__, __LINE__)

__attribute__((__noreturn__))
extern void __abort__( char *file, int line);

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
