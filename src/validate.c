#include <string.h>

#include "utils.h"
#include "validate.h"

bool is_request ( char *ptr )
{
	const char **p;

  // FIXME: OPTIONS, CONNECT, etc?
	static const char *methods[] =
	{
		"GET ", "PUT ", "POST ", "DELETE ", NULL
	};

  // FIX: Don't need to check for spaces at begining.
  // FIXME: Why ptr must have specfically more than 9 chars?
	if ( ptr && strnlen ( ptr, 12 ) >= 10 )
		{
			p = methods;

			while ( *p )
				{
					if ( memcmp ( ptr, *p, strlen ( *p ) ) == 0 )
						return true;

					p++;
				}
		}

	return false;
}
