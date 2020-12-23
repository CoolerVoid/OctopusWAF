#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "blocklist.h"
#include "utils.h"

//read lines of file
int blocklist_ip ( char *addr )
{
	FILE *fin;
	int at_list = 0;

  // FIX: Let's allocate dynamically the line, so line of any size can be processed.
  char *line;
  size_t size;

	fin = fopen ( "config/blocklist_ip.txt", "r" );

	if ( fin == NULL )
		{
			DEBUG ( "error to open() file" );
			exit ( EXIT_FAILURE );
		}

  line = NULL; size = 0;
  while ( getline( &line, &size, fin ) != -1 && ! at_list )
    {
      if ( strstr( line, addr ) )
        at_list = 1;

      free( line );
      line = NULL;
      size = 0;
    }

	if ( fclose ( fin ) == EOF )
		{
			DEBUG ( "Error in close() file config/blocklist_ip.txt " );
			return 0;
		}

	return at_list;
}
