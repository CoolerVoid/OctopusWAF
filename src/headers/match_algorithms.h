#ifndef MATCH_ALGORITHMS_H__
#define MATCH_ALGORITHMS_H__

#include <stdbool.h>
#include <pcre.h>

// DFA search algorithm
int NextMachineState ( char *pat, int M, int state, int x );
bool DFA_Search ( char *pat, int patsize, char *txt, int txtsize );

/*
 * Boyer Moore Horspool - http://en.wikipedia.org/wiki/Boyer%E2%80%93Moore%E2%80%93Horspool_algorithm
 *
 * txt_buffer = any text buffer
 * tctLen = length of text buffer
 * match = match string to find
 * matchLen = len of string match
 *
 * this function return number of ocurrences of match
 * */
bool horspool_search ( char *txt, int txtLen, char *match, int matchLen );

// Rabin–Karp algorithm - https://en.wikipedia.org/wiki/Rabin%E2%80%93Karp_algorithm
bool Rabin_Karp_search ( char *input, int input_len, char *match, int match_len );

// pcre match
bool pcre_regex_search ( const char *string, int string_len, const char *expression );
/*
  Match with GPU something with NVIDIA's CUDA or OpenCL ? relax at the future i write this... :-D
*/

#endif
