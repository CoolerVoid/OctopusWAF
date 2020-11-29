#include <signal.h>

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>

#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/bufferevent_ssl.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/http.h>
#include <event2/keyvalq_struct.h>

#include "server.h"
#include "proxy.h"
#include "utils.h"
#include "mem_ops.h"
#include "opt_extract.h"

// todo use in future
void LoadCertificates ( SSL_CTX *ctx, char *CertFile, char *KeyFile )
{
	/* set the local certificate from CertFile */
	if ( SSL_CTX_use_certificate_file ( ctx, CertFile, SSL_FILETYPE_PEM ) <= 0 )
		die ( "error in load cert" );

	/* set the private key from KeyFile (may be the same as CertFile) */
	if ( SSL_CTX_use_PrivateKey_file ( ctx, KeyFile, SSL_FILETYPE_PEM ) <= 0 )
		die ( "error in load private key" );

	/* verify private key */
	if ( !SSL_CTX_check_private_key ( ctx ) )
		die ( "error in cert check" );
}

void start_octopus_server ( void )
{
	int socklen = 0;
	int use_ssl = 1;
	struct evconnlistener *listener;

//	SSL_CTX *ssl_ctx = NULL;

	if ( signal ( SIGPIPE, SIG_IGN ) == SIG_ERR )
		{
			perror ( "signal()" );
			die ( "Program closes" );
		}

	burn_mem ( &listen_on_addr, 0, sizeof ( listen_on_addr ) );
	socklen = sizeof ( listen_on_addr );

	if ( evutil_parse_sockaddr_port ( param.hostarg,
	                                  ( struct sockaddr * ) &listen_on_addr, &socklen ) < 0 )
		{
			int p = atoi ( param.hostarg );
			struct sockaddr_in *sin = ( struct sockaddr_in * ) &listen_on_addr;

			if ( p < 1 || p > 65535 )
				die ( "Port is not ok" );


			socklen = sizeof ( struct sockaddr_in );

			if ( sin->sin_family == AF_INET )
				{

					sin->sin_port = ntohs ( sin->sin_port );
					sin->sin_addr.s_addr  =  htonl ( 0x7f000001 ); //&((struct sockaddr_in*)&sin)->sin_addr;
				}
			else if ( sin->sin_family  == AF_INET6 )
				{
					struct sockaddr_in6 *sin6 = ( struct sockaddr_in6 * ) &sin;
					sin->sin_port  = ntohs ( sin6->sin6_port );
					sin->sin_addr.s_addr  =  htonl ( 0x7f000001 ); //&((struct sockaddr_in6*)&sin)->sin6_addr;
				}
		}

	burn_mem ( &connect_to_addr, 0, sizeof ( connect_to_addr ) );
	connect_to_addrlen = sizeof ( connect_to_addr );

	if ( evutil_parse_sockaddr_port ( param.redirectarg, ( struct sockaddr * ) &connect_to_addr, &connect_to_addrlen ) < 0 )
		die ( "Port is not ok in argv" );

	base = event_base_new();

	if ( !base )
		{
			perror ( "event_base_new()" );
			die ( "Try again..." );

		}

	if ( use_ssl )
		{
			int r;
#if (OPENSSL_VERSION_NUMBER < 0x10100000L) || (defined(LIBRESSL_VERSION_NUMBER) && LIBRESSL_VERSION_NUMBER < 0x20700000L)
			SSL_library_init();
//		LoadCertificates(ssl_ctx, "cert/certkey.pem","cert/privateKey.key");
			ERR_load_crypto_strings();
			SSL_load_error_strings();
			OpenSSL_add_all_algorithms();
#endif
			r = RAND_poll();

			if ( r == 0 )
				{
					DEBUG ( "RAND_poll() failed.\n" );
					die ( "Try again..." );
				}

			ssl_ctx = SSL_CTX_new ( TLS_method() );
		}

	listener = evconnlistener_new_bind ( base, accept_cb, NULL,
	                                     LEV_OPT_CLOSE_ON_FREE | LEV_OPT_CLOSE_ON_EXEC | LEV_OPT_REUSEABLE,
	                                     -1, ( struct sockaddr * ) &listen_on_addr, socklen );

	if ( ! listener )
		{
			DEBUG ( "Couldn't open listener.\n Please try another port\n" );
			event_base_free ( base );
			die ( "Try again..." );
		}

	event_base_dispatch ( base );

	evconnlistener_free ( listener );
	event_base_free ( base );


	die ( "OctopusWAF close" );
}
