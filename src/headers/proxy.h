#ifndef PROXY_H__
#define PROXY_H__

#include <stdbool.h>
#include <sys/socket.h>

#include <openssl/ssl.h>

#include <event2/event.h>

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
