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
    
    
    if (argc != 3) {
	    fprintf(stderr, "usage: %s <port>\n", argv[0]);
	    exit(1);
    }
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;  
    hints.ai_flags = AI_NUMERICSERV; 
    hints.ai_flags |= AI_ADDRCONFIG;
    printf("%s %s\n", argv[1], argv[2]);
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
    
    

    

    while (1){
        getline(buf, &len, stdin);
        write(serverfd, buf, len);
    }

  



    return 0;
}
