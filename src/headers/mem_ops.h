#ifndef MEM_OPS_H__
#define MEM_OPS_H__

#include <stddef.h>

#define XFREE(x) xfree((void **)x);
#define MUL_NO_OVERFLOW ((size_t)1 << (sizeof(size_t)*4))

// i don't using all functions... but at the future i need it
void *xmallocarray ( size_t nmemb, size_t size );
void *xmalloc ( size_t size );
void *xcalloc ( size_t mem, size_t size );
void *xrealloc ( void *ptr, size_t size );
void xfree ( void **ptr );
char *xstrdup ( char *str );

#endif
