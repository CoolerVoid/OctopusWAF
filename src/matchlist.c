#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "match_algorithms.h"
#include "utils.h"

/* read lines of file matchlist.txt and test if match strings

If match context with request return array of void pointer with:
if true returns matched string
*/
char *matchlist ( char *input, int input_len, short option_algorithm )
{
	FILE *fin;
	bool at_list = false;
	size_t line_len;
	char *line, *p;

	static const char *fnames[] =
	{
		"config/match_list_request.txt",
		"config/regex_rules.txt"
	};

	fin = fopen ( fnames[option_algorithm == 4], "r" );

	if ( fin == NULL )
		DIE ( "error to open() file" );

	line = NULL;
	line_len = 0;

	while ( ! at_list && getline ( &line, &line_len, fin ) != -1 )
		{
			// Discard the final '\n', if any.
			p = strrchr ( line, '\n' );

			if ( p )
				*p = 0;

			// remove \n\0 etc... sub -2 at line_len
			if ( --line_len > 4 )
				switch ( option_algorithm )
					{
						case 1:
							at_list = DFA_Search ( line, line_len - 2, input, input_len );
							break;

						case 2:
							at_list = horspool_search ( input, input_len, line, line_len - 2 );
							break;

						case 3:
							at_list = Rabin_Karp_search ( input, input_len, line, line_len - 2 );
							break;

						case 4:
							at_list = pcre_regex_search ( input, input_len, line );
					}

			// Not found, discard the line and try the next.
			if ( ! at_list )
				{
					free ( line );
					line = NULL;
					line_len = 0;
				}
		}

	fclose ( fin );

	if ( at_list )
		return line;

	free ( line );  // if line is NULL this will work fine!
	return NULL;
}
