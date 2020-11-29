#include <stdio.h>

#include "file_ops.h"
#include "mem_ops.h"
#include "utils.h"

// write line in file
void WriteFile ( char *file, char *str )
{
	FILE *arq;
  int status;

	arq = fopen ( file, "a" );

	if ( arq == NULL )
		{
      // fclose(arq);
			DEBUG ( "error in WriteFile() %s", file );
			exit ( 0 ); // FIXME: Shoudn't be different than 0?
		}

  // FIX: If you want really to check for write errors, remember printf() family of functions
  //      return a negative value in case of errors!
	status = fprintf ( arq, "%s\n", str );

  // FIX: || is a sequence point, so this if() is correct.
	if ( fclose ( arq ) == EOF || status < 0 )
		{
			DEBUG ( "error in Write() file %s", file );
			exit ( 0 ); // FIXME: Shouln't be different than 0?
		}
}
