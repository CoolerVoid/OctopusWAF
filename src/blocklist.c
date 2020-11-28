#include "blocklist.h"
#include "mem_ops.h"
#include "utils.h"
#include "strsec.h"

//read lines of file
bool blocklist_ip ( char *addr )
{
	FILE *arq;
	bool at_list = false;

  // FIX: Let's allocate dynamically the line, so line of any size can be processed.
  char *line;
  size_t size;

	arq = fopen ( "config/blocklist_ip.txt", "r" );

	if ( arq == NULL )
		{
			DEBUG ( "error to open() file" );
			exit ( 0 );
		}

  line = NULL; size = 0;
  while ( getline( &line, &size, arq ) != -1 && ! at_list )
    {
      if ( strstr( line, addr ) )
        at_list = true;

      free( line );
      line = NULL;
      size = 0;
    }

	if ( fclose ( arq ) == EOF )
		{
			DEBUG ( "Error in close() file config/blocklist_ip.txt " );
			return false;
		}

	return at_list;
}
