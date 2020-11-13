#ifndef OPT_EXTRACT_H__
#define OPT_EXTRACT_H__

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <getopt.h>
#include "mem_ops.h"
#include "validate.h"
#include "strsec.h"
#include "utils.h"

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
void option_banner_octopus ( void );

#endif
