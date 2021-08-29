#include "server.h"

sbuf_t sbuf;


int main(int argc, char **argv)
{
    int listenfd, clientfd;
    char client_hostname[MAXLINE], client_port[MAXLINE];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    
    pthread_t pid;
    
    if (argc != 2) {
	    fprintf(stderr, "usage: %s <port>\n", argv[0]);
	    exit(1);
    }
    
    listenfd = Open_listenfd(argv[1]);

    sbuf_init(&sbuf, NTHREAD);
    for (int i = 0; i < NTHREAD; i++){
        Pthread_create(&pid, NULL, thread, NULL);
    }

    while (1){
        /* accept request from client */
        clientlen = sizeof(clientaddr);
        clientfd = Accept(listenfd, (struct sockaddr *) &clientaddr, &clientlen);
        Getnameinfo((struct sockaddr *) &clientaddr, clientlen, client_hostname, MAXLINE, 
                    client_port, MAXLINE, 0);
        printf("Accepted connection from (%s, %s)\n", client_hostname, client_port);
        sbuf_insert(&sbuf, clientfd);
    }

    printf("%s", user_agent_hdr);



    return 0;
}
static void * thread(void *vargp){

    Pthread_detach(Pthread_self());

    while(1){
        int clientfd = sbuf_remove(&sbuf);
        serve_client(clientfd);
        Close(clientfd);
    }
}

static void serve_client(int clientfd){
    printf("serving client\n");
    rio_t rio_from;
    char buf_from[MAXLINE];
    int length;
    Rio_readinitb(&rio_from, clientfd);
    while (Fgets(buf_from, MAXLINE, stdin) != NULL) {
        printf("aaa: %s\n", buf_from);
        Rio_writen(clientfd, buf_from, strlen(buf_from));
        Rio_readlineb(&rio_from, buf_from, MAXLINE);
        Fputs(buf_from, stdout);
    }
}









void clienterror(int fd, char *cause, char *errnum, 
		 char *shortmsg, char *longmsg) 
{
    char buf[MAXLINE];

    /* Print the HTTP response headers */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n\r\n");
    Rio_writen(fd, buf, strlen(buf));

    /* Print the HTTP response body */
    sprintf(buf, "<html><title>Tiny Error</title>");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<body bgcolor=""ffffff"">\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "%s: %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<p>%s: %s\r\n", longmsg, cause);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<hr><em>The Tiny Web server</em>\r\n");
    Rio_writen(fd, buf, strlen(buf));
}


