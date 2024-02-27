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
#include <pthread.h>

#include "sockets.h"

int getSocketDescriptor(char* localPort)
{
    // initialize...
    struct addrinfo hints; 
    struct addrinfo* res;
    bool binded = false;
    int socketDescriptor;
    

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    int addrStatus = getaddrinfo(NULL, localPort, &hints, &res);
    
    if (isError(addrStatus)) {
        printf("getaddrinfo() error: %s\n", gai_strerror(addrStatus));
        return -1;
    }

    for (struct addrinfo* p = res; p != NULL; p = p->ai_next) {
        
        socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);
        if (socketDescriptor == -1) {
            close(socketDescriptor);
            continue;
        }

        int bindStatus = bind(socketDescriptor, p->ai_addr, p->ai_addrlen);
        if (isError(bindStatus)) {
            close(socketDescriptor);            // make sure to close fd after failing to bind!
            continue;
        }

        binded = true;      // successfully binded
        break;
    }

    freeaddrinfo(res);
    
    if (!binded) {
        printf("failed to bind...\n");
        return -1;
    }

    return socketDescriptor;
}