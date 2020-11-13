#ifndef PROXY_H__
#define PROXY_H__

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>

#include <event2/bufferevent_ssl.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>

#include <event2/event.h>
#include <event2/http.h>
#include <event2/keyvalq_struct.h>

#include "blocklist.h"
#include "validate.h"
#include "opt_extract.h"
#include "matchlist.h"

struct event_base *base;
struct sockaddr_storage listen_on_addr;
struct sockaddr_storage connect_to_addr;
int connect_to_addrlen;

#define MAX_OUTPUT (512*1024)
static SSL_CTX *ssl_ctx = NULL;

char *addr_2_str ( struct sockaddr *res );
void block_msg ( struct bufferevent *bev );
bool filter_check ( struct bufferevent *bev );
void readcb ( struct bufferevent *bev, void *ctx );
void drained_writecb ( struct bufferevent *bev, void *ctx );
void close_on_finished_writecb ( struct bufferevent *bev, void *ctx );
void eventcb ( struct bufferevent *bev, short what, void *ctx );
void syntax ( void );
void accept_cb ( struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *a, int slen, void *p );

#endif
