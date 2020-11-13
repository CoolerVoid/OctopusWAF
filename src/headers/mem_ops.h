#ifndef MEM_OPS_H__
#define MEM_OPS_H__

#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <limits.h>
#include <stdint.h>
#include <stddef.h>

#define XFREE(x) xfree((void **)x);
#define MUL_NO_OVERFLOW ((size_t)1 << (sizeof(size_t)*4))

// i don't using all functions... but at the future i need it
void *xallocaarray ( size_t nmemb, size_t size );
void *xmallocarray ( size_t nmemb, size_t size );
void *xmalloc ( size_t size );
void *xcalloc ( size_t mem, size_t size );
void *xrealloc ( void *ptr, size_t size );
void xfree ( void **ptr );
char *xstrdup ( char *str );

volatile void *burn_mem ( volatile void *dst, int c, size_t len );
volatile void *burn_memcpy ( volatile void *dst, volatile void *src, size_t len );
volatile void *burn_memmove ( volatile void *dst, volatile void *src, size_t len );

#endif
