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
#include "receive.h"

#define MAX_BUFFER_LEN 1024

static pthread_mutex_t receiveMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_t threadPID;

static char* port;
static int socketDescriptor;
static char buffer[MAX_BUFFER_LEN];
static List* list;

// sychronization
static pthread_cond_t s_syncOkToToPrintCondVar = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t s_syncCondMutex = PTHREAD_COND_INITIALIZER;


bool isError(int val) {
    if (val < 0) {
        return true;
    }
    return false;
}

void* receiveRoutine(void* unused)
{
    // begin receiving... 
    while (1) {
        struct sockaddr_in remoteAddress;
        unsigned int addr_len = sizeof(remoteAddress);
        memset(&buffer, 0, MAX_BUFFER_LEN); // reset the buffer

        int bytesReceived = recvfrom(socketDescriptor, buffer, MAX_BUFFER_LEN, 0, (struct sockaddr*)&remoteAddress, &addr_len);

        if (isError(bytesReceived)) {
            printf("recvfrom(): %s\n", strerror(errno));
            return NULL;
        }

        int terminateIndex = (bytesReceived < MAX_BUFFER_LEN) ? bytesReceived : MAX_BUFFER_LEN - 1;
        buffer[terminateIndex] = 0;

        pthread_mutex_lock(&receiveMutex);
        {
            if (List_count(list) == LIST_MAX_NUM_NODES) {
                // do something if list is full.
                printf("List is full");

                // wait for space in list. 

            }
            else {
                List_prepend(list, buffer);
            }
        }
        pthread_mutex_unlock(&receiveMutex);
    }

    close(socketDescriptor);
    return NULL;
}

void receive_init(char* myPort, List* myList, int socket) 
{
    socketDescriptor = socket;
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
