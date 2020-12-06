#ifndef MATCHLIST_H__
#define MATCHLIST_H__

#include <stdbool.h>

void preload_rules( void );
bool matchlist ( char *input, int input_len, short option_algorithm );

#endif
