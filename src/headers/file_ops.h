#ifndef FILE_OPS_H__
#define FILE_OPS_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "utils.h"
//#include "mem_ops.h"

char *readLine ( char *NameFile );
void WriteFile ( char *file, char *str );
long FileSize ( const char *file );
char *Random_linefile ( char *namefile );

#endif
