#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>   // for SIZE_MAX

#include "mem_ops.h"
#include "utils.h"

// based in OpenBSD reallocarray() function http://man.openbsd.org/reallocarray.3
void *xmallocarray ( size_t nmemb, size_t size )
{
	if ( ( nmemb >= MUL_NO_OVERFLOW || size >= MUL_NO_OVERFLOW ) && nmemb > 0 && SIZE_MAX / nmemb < size )
		die ( "integer overflow block\n" ); 

  return xmalloc( nmemb * size );
}

/* DIE() will exit, _Noreturn is ISO 9899:1999 (and above) standard. */
static void xmalloc_fatal ( size_t size )
{ die( "\n Size dbg: %lu\nMemory FAILURE\n", size ); } 

__attribute__((noinline))
void *xmalloc ( size_t size )
{
	void *ptr = malloc ( size );

	// FIX: xmalloc_fatal() never returns.
	if ( ptr == NULL )
		xmalloc_fatal ( size );

	return ptr;
}

void *xcalloc ( size_t mem, size_t size )
{
	void *ptr = calloc ( mem, size );

	if ( ptr == NULL )
		xmalloc_fatal ( mem * size );

	return ptr;
}

void *xrealloc ( void *ptr, size_t size )
{
	void *p = realloc ( ptr, size );

	// FIX: xmalloc_fatal() never returns.
	if ( p == NULL )
		xmalloc_fatal ( size );

	return p;
}

char *xstrdup ( char *str )
{
	char *p;

	if ( ! ( p = strdup ( str ) ) )
		DIE ( "cannot duplicate string." );

	return p;
}

// FIX: This function is unecessary. Try to free a block pointed by NULL simply does nothing.
//      This behavior is part of the standard (see ISO 9899:1999 7.20.3.2 § 2).
#define xfree(pp) { free( *pp ); *pp = NULL; }
