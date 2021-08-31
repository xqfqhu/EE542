#include "client.h"




int main(int argc, char **argv)
{
    int serverfd;
    int len;
    socklen_t serverlen;
    struct sockaddr_storage serveraddr;
    int clientfd;
    struct addrinfo hints, *listp, *p;
    char buf[MAXLINE];
    rio_t rio;
    
    
    if (argc != 3) {
	    fprintf(stderr, "usage: %s <port>\n", argv[0]);
	    exit(1);
    }
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;  
    hints.ai_flags = AI_NUMERICSERV; 
    hints.ai_flags |= AI_ADDRCONFIG;
    getaddrinfo(argv[1], argv[2], &hints, &listp);

    for (p = listp; p; p = p->ai_next) {
        
        if ((serverfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) 
            continue; 

       
        if (connect(serverfd, p->ai_addr, p->ai_addrlen) != -1) 
            break; 
        if (close(serverfd) < 0) { 
            fprintf(stderr, "open_serverfd: close failed: %s\n", strerror(errno));
            return -1;
        } 
    } 
    freeaddrinfo(listp);
    if (!p) 
        return -1;
    

    
    Rio_readinitb(&rio, serverfd);

    while (Fgets(buf, MAXLINE, stdin) != NULL){
        
        
        Rio_writen(serverfd, buf, strlen(buf));
        Rio_readlineb(&rio, buf, MAXLINE);
        Fputs(buf, stdout);
    }
    Close(serverfd);
    exit(0);



    return 0;
}
