#include <stdio.h>
#include <stdlib.h>

#include "file_ops.h"
#include "mem_ops.h"
#include "utils.h"

// write line in file
void WriteFile ( char *file, char *str )
{
	FILE *fin;
  int status;

	fin = fopen ( file, "a" );

	if ( fin == NULL )
		{
			DEBUG ( "error in WriteFile() %s", file );
			exit ( EXIT_FAILURE );
		}

  // FIX: If you want really to check for write errors, remember printf() family of functions
  //      return a negative value in case of errors!
	status = fprintf ( fin, "%s\n", str );

  // FIX: || is a sequence point, so this if() is correct.
	if ( fclose ( fin ) == EOF || status < 0 )
		{
			DEBUG ( "error in Write() file %s", file );
			exit ( EXIT_FAILURE );
		}
}
