#ifndef OPT_EXTRACT_H__
#define OPT_EXTRACT_H__

#include <stdbool.h>

struct choice
{
	char hostarg[128];
	char redirectarg[128];
	bool debug;
	short option_algorithm;
};

struct choice param;

void parser_opts ( int argc, char **argv );
void init_banner_octopus ( void );

#endif
