#ifndef __PROXY_H__
#define __PROXY_H__

#include <stdio.h>
#include "csapp.h"
#include <string.h>
#include "sbuf.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400
#define NTHREAD 8





static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";

void clienterror(int fd, char *cause, char *errnum,
                char *shortmsg, char *longmsg);
static void * thread(void *vargp);
static void serve_client(int clientfd);


#endif