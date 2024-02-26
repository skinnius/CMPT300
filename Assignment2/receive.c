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

static pthread_mutex_t receiveMutex = PTHREAD_MUTEX_INITIALIZER;
// static pthread_mutex_t printMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_t threadPID;

static char* port;
static int socketDescriptor;
static char buffer[MAX_LEN];
static List* list;


bool isError(int val) {
    if (val < 0) {
        return true;
    }
    return false;
}

void* receiveRoutine(void* unused)
{
    // initialize...
    struct addrinfo hints; 
    struct addrinfo* res;
    bool binded = false;
    

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    int addrStatus = getaddrinfo(NULL, port, &hints, &res);
    if (isError(addrStatus)) {
        printf("getaddrinfo(): %s\n", gai_strerror(addrStatus));
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
        return NULL;
    }

    // begin receiving... 
    while (1) {
        struct sockaddr_in remoteAddress;
        unsigned int addr_len = sizeof(remoteAddress);
        memset(&buffer, 0, MAX_LEN); // reset the buffer

        int bytesReceived = recvfrom(socketDescriptor, buffer, MAX_LEN, 0, (struct sockaddr*)&remoteAddress, &addr_len);

        if (isError(bytesReceived)) {
            printf("recvfrom(): %s\n", strerror(errno));
            return NULL;
        }

        int terminateIndex = (bytesReceived < MAX_LEN) ? bytesReceived : MAX_LEN - 1;
        buffer[terminateIndex] = 0;

        pthread_mutex_lock(&receiveMutex);
        {
            if (List_append(list, buffer) == -1) {
                // do something if list is full. 
            }
            
        }
        pthread_mutex_unlock(&receiveMutex);

        // print mutex stuff i dont want to touch this in this file.. 
        // pthread_mutex_lock(&printMutex);
        // {
        //     if (List_first(list) == -1) {
        //         // do something if no elemenets in list. blocked?
        //     }
        //     else {
        //         char* msg = 
        //     }
        // }
    }
    close(socketDescriptor);
    return NULL;
}

void receive_init(char* myPort, List* myList) 
{
    port = myPort;
    list = myList;
    int threadStatus = pthread_create(&threadPID, NULL, &receiveRoutine, NULL);
    
    if (threadStatus != 0) {
        printf("pthread_create(): %s\n", strerror(errno));
    }
}


void receive_shutdown(void) 
{
    // cancel thread
    pthread_cancel(threadPID);

    // wait for threads to finish
    pthread_join(threadPID, NULL);

    // cleanup memory
    pthread_mutex_lock(&receiveMutex);
    {
        List_free(list, free);
    }
    pthread_mutex_unlock(&receiveMutex);
}

