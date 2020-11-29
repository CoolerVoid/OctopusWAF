#include <unistd.h>
#include <getopt.h>

#include "opt_extract.h"
#include "validate.h"
#include "utils.h"
#include "strsec.h"
#include "mem_ops.h"

struct choice param;

void init_banner_octopus ( void )
{
	puts (
	    GREEN
	    "\n\t\tCoded by CoolerVoid - coolerlair[at]gmail[dot]com\n\n"
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

					// FIXME: Why to allocate 128 bytes for hostarg if the option is limited to 64 chars?
					//        Yet, 'optarg' is a copy of argv[n]. Don't need to use any special strlen function!
					if ( strlen ( optarg ) <= 64 )
            param.hostarg = optarg;
					else
						die ( "Error at param host" );

					break;

				case 'r':

					// FIXME: Why to allocate 128 bytes for recirectarg if the option is limited to 64 chars?
					//        Yet, 'optarg' is a copy of argv[n]. Don't need to use any special strlen function!
					if ( strlen ( optarg ) <= 64 )
            param.redirectarg = optarg;
					else
						die ( "Error at param host" );

					break;

				case 'd':
					param.debug = true;
					puts ( YELLOW "DEBUG mode ON!" LAST );
					break;

				case 'm':
				{
          int i;
					const char **p;
					static const char *algos[] =
					{
						"dfa", "horspool", "karp-rabin", "pcre", NULL
					};

					p = algos;
					i = 1;

					while ( *p )
						{
							if ( ! strcmp ( optarg, *p ) )
								{
									param.option_algorithm = i;
									break;
								}

							++i;
							++p;
						}

					if ( ! *p )
						die ( "need match argv example --match dfa" );
				}

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
