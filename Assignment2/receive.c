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
#include <pthread.h>
#include "sockets.h"
#include "list.h"

#define MAX_LEN 1024

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_t threadPID;

static int port;
static int socketDescriptor;
static char* msg;
static List* list;


bool isError(int val) {
    if (val < 0) {
        return true;
    }
    return false;
}

void* receive(void* port)
{
    // initialize...
    struct sockaddr hints; 
    struct sockaddr* res;
    bool binded = false;
    

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    int addrStatus = getaddrinfo(NULL, port, &hints, &res);
    if (isError(addrStatus)) {
        printf("getaddrinfo(): %s\n", gai_sterror(addrStatus));
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
        printf("binded(): %s\n", strerror(errno));
        return NULL;
    }

    // begin receiving... 
    while (1) {
        struct sockaddr_in remoteAddress;
        unsigned int addr_len = sizeof(remoteAddress);
        char buffer[MAX_LEN];

        int bytesReceived = recvfrom(socketDescriptor, buffer, MAX_LEN, 0, (struct sockaddr*)&remoteAddress, &addr_len);
        int terminateIndex = (bytesReceived < MAX_LEN) ? bytesReceived : MAX_LEN - 1;

    
        buffer[terminateIndex] = 0;
        printf("%s\n", buffer);
    }






}


