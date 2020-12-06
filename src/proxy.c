#include <assert.h>
#include <sys/socket.h>
#include <arpa/inet.h>

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

#include "blocklist.h"
#include "proxy.h"
#include "utils.h"
#include "mem_ops.h"
#include "opt_extract.h"
#include "validate.h"
#include "matchlist.h"

char *addr_2_str ( struct sockaddr *res )
{
	char *s = NULL;

	switch ( res->sa_family )
		{
			case AF_INET:
			{
				struct sockaddr_in *addr_in = ( struct sockaddr_in * ) res;

				s = xmalloc ( INET_ADDRSTRLEN );
				inet_ntop ( AF_INET, & ( addr_in->sin_addr ), s, INET_ADDRSTRLEN );
				break;
			}

			case AF_INET6:
			{
				struct sockaddr_in6 *addr_in6 = ( struct sockaddr_in6 * ) res;

				s = xmalloc ( INET6_ADDRSTRLEN );
				inet_ntop ( AF_INET6, & ( addr_in6->sin6_addr ), s, INET6_ADDRSTRLEN );
				break;
			}
		}

	return s;
}

// blockmsg
void block_msg ( struct bufferevent *bev )
{
	/* hide msg error to gain performance
	char *block_msg=
	        "HTTP/1.1 404 Not Found\r\n"
	        "Content-type: text/html\r\n"
	        "\r\n"
	        "<html>\r\n"
	        " <body>\r\n"
	        "  <h1>Blocked</h1>\r\n"
	        "  <p>The requested URL was not found on this server.</p>\r\n"
	        " </body>\r\n"
	        "</html>\r\n";
	bufferevent_write(bev, block_msg, strlen(block_msg));
	*/
	bufferevent_free ( bev );
}

// if return true, blocks...
bool filter_check ( struct bufferevent *bev )
{
	struct evbuffer *input = bufferevent_get_input ( bev );
	size_t len = evbuffer_get_length ( input );
	char *data;
	bool match = false;

	data = xmalloc ( len );

	evbuffer_copyout ( input, data, len );

	char *tmpbuf = urldecode ( data, strlen ( data ) );

	if ( is_request ( tmpbuf ) )
		{
			match = matchlist ( tmpbuf, strlen ( tmpbuf ), param.option_algorithm );

			if ( match )
				if ( param.debug )
					printf ( "input: %s\n", data );
		}

	free ( data );

	return match;
}

void readcb ( struct bufferevent *bev, void *ctx )
{
	struct bufferevent *partner = ctx;
	struct evbuffer *src, *dst;
	size_t len;
	src = bufferevent_get_input ( bev );
	len = evbuffer_get_length ( src );

	if ( !partner )
		{
			evbuffer_drain ( src, len );
			return;
		}

	if ( filter_check ( bev ) == true )
		{
			block_msg ( bev );

			if ( param.debug == true )
				puts ( "\n" RED "---===DETECT ATTACK HERE!===---\n"
				       "███████████████████████████████████" LAST );
		}
	else
		{
			dst = bufferevent_get_output ( partner );
			evbuffer_add_buffer ( dst, src );

			if ( evbuffer_get_length ( dst ) >= MAX_OUTPUT )
				{
					bufferevent_setcb ( partner, readcb, drained_writecb, eventcb, bev );
					// pass to otherside
					bufferevent_setwatermark ( partner, EV_WRITE, MAX_OUTPUT / 2, MAX_OUTPUT );
					bufferevent_disable ( bev, EV_READ );
				}
		}
}

// func from libevent examples
void drained_writecb ( struct bufferevent *bev, void *ctx )
{
	struct bufferevent *partner = ctx;

	/* We were choking the other side until we drained our outbuf a bit.
	 * Now it seems drained. */
	bufferevent_setcb ( bev, readcb, NULL, eventcb, partner );
	bufferevent_setwatermark ( bev, EV_WRITE, 0, 0 );

	if ( partner )
		bufferevent_enable ( partner, EV_READ );
}

void close_on_finished_writecb ( struct bufferevent *bev, void *ctx )
{
	struct evbuffer *b = bufferevent_get_output ( bev );

	if ( evbuffer_get_length ( b ) == 0 )
		bufferevent_free ( bev );
}

//func from libevent examples
void eventcb ( struct bufferevent *bev, short what, void *ctx )
{
	struct bufferevent *partner = ctx;

	if ( what & ( BEV_EVENT_EOF | BEV_EVENT_ERROR ) )
		{
			if ( what & BEV_EVENT_ERROR )
				{
					unsigned long err;

          // FIX: errno is not touched by the function in case of NO errors.
          //      Must zero before call.
          errno = 0;
					while ( ( err = ( bufferevent_get_openssl_error ( bev ) ) ) )
						{
							const char *msg = ( const char * )ERR_reason_error_string ( err );
							const char *lib = ( const char * )ERR_lib_error_string ( err );
							const char *func = ( const char * )ERR_func_error_string ( err );

							fprintf ( stderr,
							          "%s in %s %s\n", msg, lib, func );
						}

					if ( errno )
						perror ( "connection error" );
				}

			if ( partner )
				{
					/* Flush all pending data */
					readcb ( bev, ctx );

					if ( evbuffer_get_length ( bufferevent_get_output ( partner ) ) )
						{
							/* We still have to flush data from the other
							 * side, but when that's done, close the other
							 * side. */
							bufferevent_setcb ( partner,
							                    NULL, close_on_finished_writecb,
							                    eventcb, NULL );
							bufferevent_disable ( partner, EV_READ );
						}
					else
						{
							/* We have nothing left to say to the other
							 * side; close it. */
							bufferevent_free ( partner );
						}
				}

			bufferevent_free ( bev );
		}
}

void accept_cb ( struct evconnlistener *listener, evutil_socket_t fd,
                 struct sockaddr *a, int slen, void *p )
{
	int use_wrapper = 1;
	struct bufferevent *b_out, *b_in;

	b_in = bufferevent_socket_new ( base, fd, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_DEFER_CALLBACKS );

	if ( !ssl_ctx || use_wrapper )
		b_out = bufferevent_socket_new ( base, -1, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_DEFER_CALLBACKS );
	else
		{
			SSL *ssl = SSL_new ( ssl_ctx );
			b_out = bufferevent_openssl_socket_new ( base, -1, ssl, BUFFEREVENT_SSL_CONNECTING, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_DEFER_CALLBACKS );
		}

	assert ( b_in && b_out && a );

	if ( bufferevent_socket_connect ( b_out, ( struct sockaddr * ) &connect_to_addr, connect_to_addrlen ) < 0 )
		{
			perror ( "bufferevent_socket_connect" );
			bufferevent_free ( b_out );
			bufferevent_free ( b_in );
			return;
		}

	if ( ssl_ctx && use_wrapper )
		{
			struct bufferevent *b_ssl;
			SSL *ssl = SSL_new ( ssl_ctx );
			b_ssl = bufferevent_openssl_filter_new ( base, b_out, ssl, BUFFEREVENT_SSL_CONNECTING, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_DEFER_CALLBACKS );

			if ( !b_ssl )
				{
					perror ( "Bufferevent_openssl_new" );
					bufferevent_free ( b_out );
					bufferevent_free ( b_in );
					return;
				}

			b_out = b_ssl;
		}

	char *addr_ip = addr_2_str ( a );

	if ( param.debug == true )
		printf ( "Client ADDR: %s\n", addr_ip );

	if ( blocklist_ip ( addr_ip ) != true )
		{
			bufferevent_setcb ( b_in, readcb, NULL, eventcb, b_out );
			bufferevent_setcb ( b_out, readcb, NULL, eventcb, b_in );

			bufferevent_enable ( b_in, EV_READ | EV_WRITE );
			bufferevent_enable ( b_out, EV_READ | EV_WRITE );
		}

	free ( addr_ip );
}
