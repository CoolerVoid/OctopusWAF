#ifndef UTILS_H__
#define UTILS_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// set DEBUG ON
#define BUGVIEW 1

#define DEBUG(x, s...) do { \
			if ( ! BUGVIEW ) break; \
			time_t t = time( NULL ); \
			char *d = ctime( &t ); \
			fprintf( stderr, "\n--- DEBUG-START ---\n\n %.*s %s[%d] %s(): \n", \
			         (int)strlen(d) - 1, d, __FILE__, \
			         __LINE__, __FUNCTION__ ); \
			fprintf( stderr, x, ## s ); \
			fprintf( stderr, "\n\n--- DEBUG-END ---\n" ); \
		} while (0);

#define DIE(x) do {\
			DEBUG( x ); \
			exit( EXIT_FAILURE ); \
		} while (0);

// ANSI control sequences
#define RED "\033[22;31m"
#define YELLOW "\033[1;33m"
#define CYAN "\033[22;36m"
#define GREEN "\033[22;32m"
#define LAST "\033[m"

// NOTE: Some compilers define max() macros [not standard]
#ifdef max
	#undef max
#endif
#define max(a,b) ((a)>(b)?(a):(b))

void die ( char *fmt, ... );
void no_write_coredump ( void );
void load_signal_alarm ( void );
char *urldecode ( char *str, int size ) ;
char *deadspace ( char *str );
int test_letter ( char p );
char *all2lowcase ( char *str );

#endif
