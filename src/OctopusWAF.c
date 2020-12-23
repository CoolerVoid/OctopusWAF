#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "matchlist.h"
#include "utils.h"
#include "opt_extract.h"
#include "server.h"

// FIXME: Shouldn't this be at some config file?
const char *ssl_certchain = "cert/certkey.pem";
const char *ssl_privatekey = "cert/privateKey.key";

int main ( int argc, char *argv[] )
{
	no_write_coredump();
	load_signal_alarm();

	if ( signal ( SIGPIPE, SIG_IGN ) == SIG_ERR )
		{
			perror ( "signal()" );
			return 1;
		}

	parser_opts ( argc, argv );

	init_banner_octopus();
  preload_rules();
	start_octopus_server();

	return EXIT_SUCCESS;
}
