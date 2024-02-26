// implementation inspired by brian fraser's tutorials, linux man pages, and beej's programming guide
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include "sockets.h"


bool isError(int val) {
    if (val < 0) {
        return true;
    }
    return false;
}

int sockSetup(char* localPortNum)           // return 0 on success, others mean failure. 
{
    int numBinds = 0;
    int status;
    int socketFD;
    int bindStatus;

    struct addrinfo hints;
    struct addrinfo *res;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    status = getaddrinfo(NULL, localPortNum, &hints, &res);
    if (isError(status)) {
        fprintf(stderr, "getaddrinfo(): %s\n", gai_strerror(status));
    }


    // attempt to bind to an address (code inspired by man page of addrinfo(3))
    for (struct addrinfo *p = res; p != NULL; p = p->ai_next) {
        printf("a\n");
        socketFD = socket(PF_INET, SOCK_DGRAM, 0);


        if (socketFD == -1){          // check if socket() was successful
            fprintf(stderr, "socket():%d(%s)\n", errno, strerror(errno));
            close(socketFD);
            continue;
        }

        bindStatus = bind(socketFD, p->ai_addr, p->ai_addrlen);
        
        if (isError(bindStatus)) {        // check if bind() was successful
            fprintf(stderr, "bind():%d(%s)\n", errno, strerror(errno));
            close(socketFD);
            continue;
        }
        numBinds++;
        printf("successfully binded...");
        break;
    }

    if (numBinds == 0) {                // check for failure to bind.
        printf("bind failure\n");
        return -1;
    }

    // listen 

    // int listenRet = listen(socketFD, );


    
    return 0;
}



// void freeSocket(addrinfo *serverInfo)
// {
//     freeaddrinfo(serverInfo);
// }