#ifndef SERVER_H__
#define SERVER_H__

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>

#include "proxy.h"
#include "mem_ops.h"
#include "validate.h"
#include "strsec.h"
#include "utils.h"
#include "opt_extract.h"

// todo iumprove that before to use
void LoadCertificates(SSL_CTX* ctx, char* CertFile, char* KeyFile);

//static SSL_CTX *ssl_ctx = NULL;
void start_octopus_server(void);

#endif
