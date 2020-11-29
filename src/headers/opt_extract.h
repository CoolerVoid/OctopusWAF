#ifndef OPT_EXTRACT_H__
#define OPT_EXTRACT_H__

#include <stdbool.h>

struct choice
{
	char *hostarg;
	char *redirectarg;
	bool debug;
	int  option_algorithm;
};

extern struct choice param;

void parser_opts ( int argc, char **argv );
void init_banner_octopus ( void );

#endif
