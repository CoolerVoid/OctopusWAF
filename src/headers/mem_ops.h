#ifndef MEM_OPS_H__
#define MEM_OPS_H__

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

// FIX: Don't need those 'volatile' qualifiers and we can use memset, memcpy and memmove!
#define burn_mem(dst,c,len) memset( (dst), (c), (len) )
#define burn_memcpy(dst,src,len) memset( (dst), (src), (len) )
#define burn_memmove(dst,src,len) memmove( (dst), (src), (len) )

#endif
