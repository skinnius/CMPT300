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

#define MAX_LEN 100

bool isError(int val) {
    if (val < 0) {
        return true;
    }
    return false;
}

int sockSetup(char* localPortNum)           // return 0 on success, others mean failure. 
{
    bool binded = false;
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
            close(socketFD);
            continue;
        }

        bindStatus = bind(socketFD, p->ai_addr, p->ai_addrlen);
        
        if (isError(bindStatus)) {        // check if bind() was successful
            close(socketFD);
            continue;
        }

        binded = true;
        printf("successfully binded... YIPPEE\n");
        break;
    }
    // free the list
    freeaddrinfo(res);

    if (!binded) {                // check for failure to bind.
        fprintf(stderr, "bind():%d(%s)\n", errno, strerror(errno));
        return -1;
    }
    
    return socketFD;
}


void receive(int socketFD) 
{
    while (1) {
        struct sockaddr_in remoteAddress;
        unsigned int sin_len = sizeof(remoteAddress);
        char messageRecv[MAX_LEN];

        int bytesReceived = recvfrom(socketFD, messageRecv, MAX_LEN, 0, (struct sockaddr*)&remoteAddress, &sin_len);
        int terminateIdx = (bytesReceived < MAX_LEN) ? bytesReceived : MAX_LEN - 1;


        messageRecv[terminateIdx] = 0;
        printf("%s\n", messageRecv);
        
        // transmit reply
        sendto(socketFD, "received", strlen("received"), 0, (struct sockaddr*)&remoteAddress, sin_len);
    }

}


void sendToDest(int socketFD, char* ip, char* port)
{
    // first connect to a remote host...
    struct sockaddr_in dest;
    memset(&dest, 0, sizeof(dest));
    dest.sin_port = htons(atoi(port));
    dest.sin_family = AF_INET;
    
    if (inet_pton(AF_INET, ip, &(dest.sin_addr)) < 1) {
        fprintf(stderr, "inet_pton(): %s\n", strerror(errno));
    }

    int connectStatus = connect(socketFD, (struct sockaddr*)&dest, sizeof(dest));
    if (isError(connectStatus)) {
        fprintf(stderr, "connect(): %s\n", strerror(errno));
    }


    char messageTx[MAX_LEN];
    sendto(socketFD, messageTx, strlen(messageTx), 0, (struct sockaddr*)&dest, sizeof(dest));
}
