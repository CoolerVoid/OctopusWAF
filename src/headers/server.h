#ifndef SERVER_H__
#define SERVER_H__

#include <openssl/ssl.h>

// todo iumprove that before to use
void LoadCertificates(SSL_CTX* ctx, char* CertFile, char* KeyFile);

//static SSL_CTX *ssl_ctx = NULL;
void start_octopus_server(void);

#endif
