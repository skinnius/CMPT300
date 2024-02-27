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
    // initialization...

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(localPort));
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    memset(&addr, 0, sizeof(addr));
    
    int socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketDescriptor < 0) {
        printf("socket() failure: %s\n", strerror(errno));
        return -1;
    }

    int bindStatus = bind(socketDescriptor, (struct sockaddr*)&addr, sizeof(addr));
    if (bindStatus < 0) {
        printf("failed to bind. %s\n", strerror(errno));
    }

    return socketDescriptor;
}

struct addrinfo* getRemoteAddress(char* remoteHostname, char* remotePort)
{
    // initialization...
    struct addrinfo hints; 
    struct addrinfo* res;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    int addrStatus = getaddrinfo(remotePort, remoteHostname, &hints, &res);
    
    if (addrStatus < 0) {
        printf("getaddrinfo() error: %s\n", gai_strerror(addrStatus));
        return NULL;
    }

    return res;             // res may need to be cleaned up...
}





    // struct addrinfo hints; 
    // struct addrinfo* res;
    // bool binded = false;
    // int socketDescriptor;
    

    // memset(&hints, 0, sizeof(hints));
    // hints.ai_family = AF_INET;
    // hints.ai_socktype = SOCK_DGRAM;
    // hints.ai_flags = AI_PASSIVE;

    // int addrStatus = getaddrinfo(NULL, localPort, &hints, &res);
    
    // if (isError(addrStatus)) {
    //     printf("getaddrinfo() error: %s\n", gai_strerror(addrStatus));
    //     return -1;
    // }

    // for (struct addrinfo* p = res; p != NULL; p = p->ai_next) {
        
    //     socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);
    //     if (socketDescriptor == -1) {
    //         close(socketDescriptor);
    //         continue;
    //     }

    //     int bindStatus = bind(socketDescriptor, p->ai_addr, p->ai_addrlen);
    //     if (isError(bindStatus)) {
    //         close(socketDescriptor);            // make sure to close fd after failing to bind!
    //         continue;
    //     }

    //     binded = true;      // successfully binded
    //     break;
    // }

    // freeaddrinfo(res);
    
    // if (!binded) {
    //     printf("failed to bind...\n");
    //     return -1;
    // }

    // return socketDescriptor;