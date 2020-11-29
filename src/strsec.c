/*  $OpenBSD: strlcpy, strlcat,v 1.11 2006/05/05 15:27:38 millert Exp $ */

/*
 * Copyright (c) 1998 Todd C. Miller <Todd.Miller@courtesan.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#if !defined(__FreeBSD__) && !defined(__OpenBSD__) && !defined(__NetBSD__) && \
    !defined(__bsdi__) && !defined(__APPLE__)

#include <stddef.h>
#include <string.h>

#include "strsec.h"
#include "mem_ops.h"

/*
 * Copy src to string dst of size siz.  At most siz-1 characters
 * will be copied.  Always NUL terminates (unless siz == 0).
 * Returns strlen(src); if retval >= siz, truncation occurred.
 */
size_t strlcpy ( char *dst, const char *src, size_t siz )
{
	char *d = dst;
	const char *s = src;
	size_t n = siz;

	/* Copy as many bytes as will fit */
	if ( n != 0 )
		{
			while ( --n != 0 )
				{
					if ( ( *d++ = *s++ ) == '\0' )
						break;
				}
		}

	/* Not enough room in dst, add NUL and traverse rest of src */
	if ( n == 0 )
		{
			if ( siz != 0 )
				*d = '\0';      /* NUL-terminate dst */

			while ( *s++ )
				;
		}

	return ( s - src - 1 ); /* count does not include NUL */
}

/*
 * Appends src to string dst of size siz (unlike strncat, siz is the
 * full size of dst, not space left).  At most siz-1 characters
 * will be copied.  Always NUL terminates (unless siz <= strlen(dst)).
 * Returns strlen(src) + MIN(siz, strlen(initial dst)).
 * If retval >= siz, truncation occurred.
 */
size_t strlcat ( char *dst, const char *src, size_t siz )
{
	char *d = dst;
	const char *s = src;
	size_t n = siz;
	size_t dlen;

	/* Find the end of dst and adjust bytes left but don't go past end */
	while ( n-- != 0 && *d != '\0' )
		d++;

	dlen = d - dst;
	n = siz - dlen;

	if ( n == 0 )
		return ( dlen + strlen ( s ) );

	while ( *s != '\0' )
		{
			if ( n != 1 )
				{
					*d++ = *s;
					n--;
				}

			s++;
		}

	*d = '\0';

	return dlen + ( s - src ); /* count does not include NUL */
}

/*-strnstr() function
 * Copyright (c) 2001 Mike Barcroft <mike@FreeBSD.org>
 * Copyright (c) 1990, 1993
 *  The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Chris Torek.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
/*
 * Find the first occurrence of find in s, where the search is limited to the
 * first slen characters of s.
 */
char *strnstr ( const char *s, const char *find, size_t slen )
{
	char c, sc;
	size_t len;

	if ( ( c = *find++ ) != '\0' )
		{
			len = strlen ( find );

			do
				{
					do
						{
							if ( slen-- < 1 || ( sc = *s++ ) == '\0' )
								return NULL;
						} while ( sc != c );

					if ( len > slen )
						return NULL;
				} while ( strncmp ( s, find, len ) != 0 );

			s--;
		}

	return ( char * )s;
}

//custom func
char *xstrndup ( const char *s, size_t n )
{
	char *new = xmalloc ( n + 1 );

  // FIX: If xmalloc() fails, it never gets here, so
  //      checking for pointer validity isn't necessary.
  strncpy ( new, s, n );
	new[n] = '\0';

	return new;
}


#endif /* ! __*BSD__ */
