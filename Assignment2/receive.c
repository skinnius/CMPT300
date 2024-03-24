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
#include "receive.h"

#define MAX_BUFFER_LEN 1024

static pthread_t threadPID;
static pthread_mutex_t* receiveMutex;

static pthread_cond_t* syncWithDisplay;
static pthread_mutex_t* syncWithDisplayMutex;

static pthread_cond_t* shutdownCon;
static pthread_mutex_t* shutdownConMutex;

static int socketDescriptor;
static char buffer[MAX_BUFFER_LEN];
static List* list;

void signalForShutdown()
{
    pthread_mutex_lock(shutdownConMutex);
    {
        pthread_cond_signal(shutdownCon);
    }
    pthread_mutex_unlock(shutdownConMutex);
}

int addMessageToList(char* msg) 
{
    pthread_mutex_lock(receiveMutex);
    {
        if (List_prepend(list, msg) == -1) {
            return -1;
        }
    }
    pthread_mutex_unlock(receiveMutex);
    return 0;
}


void signalToDisplay()
{
    pthread_mutex_lock(syncWithDisplayMutex);
    {
        pthread_cond_signal(syncWithDisplay);
    }
    pthread_mutex_unlock(syncWithDisplayMutex);
}


void* receiveRoutine(void* unused)
{
    // begin receiving... 
    while (1) {
        struct sockaddr_in remoteAddress;
        unsigned int addr_len = sizeof(remoteAddress);
        memset(&buffer, 0, MAX_BUFFER_LEN); // reset the buffer

        int bytesReceived = recvfrom(socketDescriptor, buffer, MAX_BUFFER_LEN, 0, (struct sockaddr*)&remoteAddress, &addr_len);
        if (bytesReceived < 0) {
            printf("recvfrom(): %s\n", strerror(errno));
            break;
        }

        int terminateIndex = (bytesReceived < MAX_BUFFER_LEN) ? bytesReceived : MAX_BUFFER_LEN - 1;
        buffer[terminateIndex] = '\0';

        // check for clean shutdown to terminate
        if (strlen(buffer) == 2 && ((buffer[0] == '!') && (buffer[1] == '\n'))) {
            signalForShutdown();
            break;
        }
        
        char* msg = (char*)malloc(strlen(buffer) + 1);
        if (msg == NULL) {
            break;
        }
        strcpy(msg, buffer);
        
        if (addMessageToList(msg) == -1) {
            break;
        }
        
        signalToDisplay();
    }
    return NULL;
}



void receive_init(List* myList, int socket, pthread_cond_t* cond, pthread_mutex_t* mutex, pthread_mutex_t* syncm,
                pthread_cond_t* shutdownCondition, pthread_mutex_t* shutdownMutex) 
{
    socketDescriptor = socket;
    list = myList;
    syncWithDisplay = cond;
    receiveMutex = mutex;
    syncWithDisplayMutex = syncm;
    shutdownCon = shutdownCondition;
    shutdownConMutex = shutdownMutex;

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

}
