#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "match_algorithms.h"
#include "utils.h"

/* FIX: Preloading all the rules to avoid open/read/close the files
        each time mathlist() is called. */
static size_t regex_rules_count, match_rules_count; // lists.
static char **regex_rules, **match_rules;           // # of elements on lists.

// FIXME: This functions does the same thing twice. I didn't care to create
//        an auxiliary static function to do that to avoid dealing with tripple pointers.
//        feel free to change it.
//
// This function is called once. So 'match_rules' and 'regex_rules' arrays are initialized
// here. To make sure, the initial values of these pointers are assigned to NULL here. This
// is unecessary, due to the fact that static objects are always initialized to 0 (ISO 9899).
// But I did anyway...
//
// Also, preloading the lists don't cause a big impact on the heap, since the text files, even
// with hundredes or thousands of rules, are very small.
void preload_rules ( void )
{
	FILE *f;
	char *buffer, **p, *q;
	size_t size;

  // Already 0, but to be sure...
	regex_rules_count = match_rules_count = 0;

	f = fopen ( "config/match_list_request.txt", "r" );

	if ( ! f )
		die ( "match list rules file can't be read" );

  // Already NULL, but to be sure...
	match_rules = NULL;

	buffer = NULL;
	size = 0;

	while ( getline ( &buffer, &size, f ) != -1 )
		{
			p = realloc ( match_rules, sizeof ( char * ) * ( match_rules_count + 1 ) );

			if ( ! p )
				die ( "Error loading match rules" );

			match_rules = p;

			p += match_rules_count++;   // points to last element.

      // Get rid of \n at the end of the line.
      q = strrchr( buffer, '\n' );
      if ( q )
        *q = 0;

			*p = buffer;                // add last element.

			buffer = NULL;
			size = 0;
		}

	fclose ( f );

  // --- DO THE SAME, but for regexes.
	f = fopen ( "config/regex_rules.txt", "r" );
	if ( ! f )
		die ( "regex matches rules file can't be read" );

  // Already NULL, but to be sure...
	regex_rules = NULL;

	buffer = NULL;
	size = 0;

	while ( getline ( &buffer, &size, f ) != -1 )
		{
			p = realloc ( regex_rules, sizeof ( char * ) * ( regex_rules_count + 1 ) );

			if ( ! p )
				die ( "Error loading match rules" );

			regex_rules = p;
			p += regex_rules_count++;

      q = strpbrk( buffer, "\r\n" );
      if ( q )
        *q = 0;

			*p = buffer;

			buffer = NULL;
			size = 0;
		}

	fclose ( f );
}

/* read lines of file matchlist.txt and test if match strings

If match context with request return array of void pointer with:
if true returns matched string
*/
char *matchlist ( char *input, int input_len, short option_algorithm )
{
	bool at_list = false;
	size_t size;
	char **p;

	if ( option_algorithm != 4 )
		{
			p = match_rules;
			size = match_rules_count;
		}
	else
		{
			p = regex_rules;
			size = regex_rules_count;
		}

	while ( size-- && ! at_list )
		{
			switch ( option_algorithm )
				{
					case 1:
						at_list = DFA_Search ( *p, strlen( *p ), input, input_len );
						break;

					case 2:
						at_list = horspool_search ( input, input_len, *p, strlen( *p ) );
						break;

					case 3:
						at_list = Rabin_Karp_search ( input, input_len, *p, strlen( *p ) );
						break;

					case 4:
						at_list = pcre_regex_search ( input, input_len, *p );
				}

      p++;
		}

  // FIXME: This is probably WRONG!
	if ( at_list )
		return *p;

	return NULL;
}
