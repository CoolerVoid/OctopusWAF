#include <unistd.h>
#include <getopt.h>

#include "opt_extract.h"
#include "validate.h"
#include "utils.h"
#include "strsec.h"
#include "mem_ops.h"

void init_banner_octopus ( void )
{
	puts (
	    GREEN
	    "\n\t\tCoded by CoolerVoid - coolerlair[at]gmail[dot]com\n\n"
	    LAST
	    CYAN
	    "       ,'''`.\n"
	    "      / _  _ \\\n"
	    "      |(*)(*)|\n"
	    "      )  __  (\n"
	    "     /,'))((`.\\\n"
	    "    (( ((  )) ))\n"
	    "     `\\ `)(' /'\n"
	    YELLOW
	    "\n\t OCTOPUS Web application firewall v0.1\n"
	    LAST
	);
}

static void option_banner_octopus ( void )
{
	puts (
	    "\tOptions argv:\n"
	    "\t--host or -h : host to protect \n"
	    "\t--redirect or -r : port to redirect HTTP or https \n"
	    "\t--debug : Write log in console \n"
	    "\t--match or -m : match algorithm you can choice (dfa, horspool,pcre(regex) or karp-rabin), example --match pcre \n\n\tNotes:\n"
	    "\tConfig Blocklist in config/blocklist_ip.txt\n"
	    "\tConfig list rule of matchs at config/match_list.txt\n"
	    "\tConfig list rules of regex matchs using PCRE at config/regex_rules.txt\n"
	    "\tExample: \n"
	    YELLOW
	    "\t$ bin/OctopusWAF -h 127.0.0.1:8090 -r 127.0.0.1:80 -m horspool --debug\n"
	    LAST
	);
}

void parser_opts ( int argc, char **argv )
{
	char c;
	short options_match = 0;

	if ( argc < 6 )
		{
			init_banner_octopus();
			option_banner_octopus();
			die ( "Need more arguments" );
		}

	const struct option long_options[] =
	{
		{ "host", required_argument, NULL, 'h' },
		{ "redirect", required_argument, NULL, 'r' },
		{ "debug", no_argument, NULL, 'd' },
		{ "match", required_argument, NULL, 'm' },
		{ NULL }
	};

	opterr = 0;

	while ( ( c = getopt_long ( argc, argv, "h:r:d:m:", long_options, NULL ) ) != -1 )
		switch ( c )
			{
        // host
				case 'h':
					if ( strnlen ( optarg, 65 ) <= 64 )
						{
							burn_mem ( param.hostarg, 0, 127 ); // preserve 1buyte to canary
							strlcpy ( param.hostarg, optarg, 128 );
						}
					else
						die ( "Error at param host" );

					break;

				case 'r':
					if ( strnlen ( optarg, 65 ) <= 64 )
						{
							burn_mem ( param.redirectarg, 0, 127 ); // stack cookie preserve
							strlcpy ( param.redirectarg, optarg, 128 );
						}
					else
						die ( "Error at param host" );

					break;


				case 'd':
					param.debug = true;
					printf ( "%s DEBUG mode ON! %s\n", YELLOW, LAST );
					break;

				case 'm':
					if ( strnlen ( optarg, 12 ) <= 11 )
						{
							char algorithm[12];

							burn_mem ( algorithm, 0, 11 );
							strlcpy ( algorithm, optarg, 11 );

							if ( strnstr ( algorithm, "dfa", 3 ) )
								options_match = 1;

							if ( strnstr ( algorithm, "horspool", 8 ) )
								options_match = 2;

							if ( strnstr ( algorithm, "karp-rabin", 10 ) )
								options_match = 3;

							if ( strnstr ( algorithm, "pcre", 4 ) )
								options_match = 4;

							if ( options_match == 0 )
								die ( "need match argv example --match dfa" );

							param.option_algorithm = options_match;
						}
					else
						die ( "Error at param Log" );

					break;

				case '?':
					if ( optopt == 'h' || optopt == 'r' || optopt == 'm' )
						{
							init_banner_octopus();
							option_banner_octopus();
							DEBUG ( "Option -%c requires an argument.\n", optopt );
							die ( "Try again using other argument..." );
						}
			}
}
