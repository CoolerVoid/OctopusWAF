#include <stdbool.h>
#include <pcre.h>

#include "match_algorithms.h"
#include "utils.h"

int NextMachineState ( char *pat, int M, int state, int x )
{
	// FIX: 'register' reserved word don't do what most people think it does!
	int fix, i;

	// If the character N is same as next character in pattern, then simply increment state
	if ( state < M && x == pat[state] )
		return state + 1;

	fix = state;

	while ( fix > 0 )
		{
			if ( pat[fix - 1] == x )
				{
					for ( i = 0; i < fix - 1; i++ )
						if ( pat[i] != pat[state - fix + 1 + i] )
							break;

					if ( i == fix - 1 )
						return fix;
				}

			fix--;
		}

	return 0;
}

// create automata table
static void write_tf ( char *pat, int M, int TF[][256] )
{
	int state, x;

	for ( state = 0; state <= M; ++state )

		// FIXME: Candidate for parallelism (OpenMP?).
		for ( x = 0; x < 256; ++x )
			TF[state][x] = NextMachineState ( pat, M,  state, x );
}

/* Prints all occurrences of pat in txt */
bool DFA_Search ( char *pat, int patsize, char *txt, int txtsize )
{
	// FIXME: Could be too much pressure on stack.
	//        Better to change it to heap allocation.
	int TF[patsize + 1][256];
	int i, state;

	write_tf ( pat, patsize, TF );

	i = state = 0;

	while ( i < txtsize )
		{
			state = TF[state][ ( int ) txt[i] ];

			if ( state == patsize )
				return true;

			i++;
		}

	return false;
}

/*
 * simple Boyer Moore Horspool  http://en.wikipedia.org/wiki/Boyer%E2%80%93Moore%E2%80%93Horspool_algorithm
 *
 * txt_buffer = any text buffer
 * tctLen = length of text buffer
 * match = match string to find
 * matchLen = len of string match
 *
 * */
bool horspool_search ( char *txt, int txtLen, char *match, int matchLen )
{
	int i, shift;
	int badCharHtable[256];

	for ( i = 0; i < matchLen; i++ )
		badCharHtable[ ( int ) match[i]] = i;

	while ( i < 256 )
		badCharHtable[i++] = -1;

	shift = 0;

	while ( shift <= txtLen - matchLen )
		{
			int nInd = matchLen - 1;

			while ( ( nInd >= 0 ) && ( match[nInd] == txt[shift + nInd] ) )
				nInd--;

			if ( nInd < 0 )
				{
					return true;

					// FIX: Unreachable code.

					//if ( shift + matchLen < txtLen )
					//  shift += matchLen - badCharHtable[ ( int ) txt[shift + matchLen]];
					//else
					//  shift += 1;
				}
			else
				shift += max ( 1, nInd - badCharHtable[ ( int ) txt[shift + nInd] ] );
		}

	return false;
}

/* Rabin–Karp algorithm - https://en.wikipedia.org/wiki/Rabin%E2%80%93Karp_algorithm
 * input = any text buffer
 * input_len = length of text buffer
 * match = match string to find
 * match_len = len of string match
 *
*/
bool Rabin_Karp_search ( char *input, int input_len, char *match, int match_len )
{
	// FIX: var1, var2 and z being a bitmap, should be unsigned.
	unsigned int var1, var2, z;
	int i, j, sub;

	// FIX: Don't need a loop here!
	// FIXME: What if 'match_len' is greater than 31?

	//    while ( i < match_len )
	//        {
	//            z <<= 1;
	//            i++;
	//        }
	i = match_len - 1;
	z = 1;

	if ( i > 0 )
		z <<= i;

	// FIXME: What if 'match_len' is greater than 31?
	i = 0;
	var1 = var2 = 0;

	while ( i < match_len )
		{
			var1 = ( var1 << 1 ) + match[i];
			var2 = ( var2 << 1 ) + input[i];
			i++;
		}


	sub = input_len - match_len;

	j = 0;

	while ( j <= sub )
		{
			if ( var1 == var2 && memcmp ( match, input + j, match_len ) == 0 )
				return true;

			var2 = ( ( ( var2 ) - ( input[j] ) * z ) << 1 ) + input[j + match_len];
			++j;
		}

	return false;
}

bool pcre_regex_search ( const char *string, int string_len, const char *expression )
{
	const char *err;
	int errofs = 0, offset = 0;
	int ovector[100];
	int rc;

	pcre *re = pcre_compile ( expression, 0, &err, &errofs, NULL );

	if ( re == NULL )
		{
			DEBUG ( " regex compilation failed : %s\n", expression );
			exit ( 1 );
		}

	rc = pcre_exec ( re, NULL, string, string_len, offset, 0, ovector, sizeof ovector / sizeof ovector[0] );

	pcre_free ( re );

	return rc > 0;
}

/*
  Match with GPU something with NVIDIA's CUDA or OpenCL ? relax at the future i write this... :-D
*/
