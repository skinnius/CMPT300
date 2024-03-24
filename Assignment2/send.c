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

#include "send.h"

#define MAX_BUFFER_LEN 1024

static pthread_t threadPID;
static pthread_mutex_t* sendMutex;

static pthread_cond_t* syncWithInput;
static pthread_mutex_t* syncWithInputMutex;

static pthread_cond_t* shutdownCond;
static pthread_mutex_t* shutdownCondMutex;

static List* list;
static char buffer[MAX_BUFFER_LEN];
static int socketDescriptor;
static struct addrinfo* remoteAddress;

void takeMessageFromList() 
{
    pthread_mutex_lock(sendMutex);
        {
            // read from the list (consume)
            char* msg = (char*)List_trim(list);
            if (msg != NULL) {
                strcpy(buffer, msg);
                free(msg);
            }
        }
    pthread_mutex_unlock(sendMutex);
}

void waitForMessage()
{
    pthread_mutex_lock(syncWithInputMutex);
    {
        pthread_cond_wait(syncWithInput, syncWithInputMutex);
    }
    pthread_mutex_unlock(syncWithInputMutex);
}


void* sendRoutine(void* unused)
{
    while (1) {

        // wait until confirmation that there is something in the list.... 
        waitForMessage();

        // copy the last element of the list into a buffer (critical section)
        takeMessageFromList();

        // send the message out.
        int sendStatus = sendto(socketDescriptor, buffer, strlen(buffer), 0, remoteAddress->ai_addr, remoteAddress->ai_addrlen);
        
        // check for clean shutdown to terminate
        if (strlen(buffer) == 2 && ((buffer[0] == '!') && (buffer[1] == '\n'))) {

            pthread_mutex_lock(shutdownCondMutex);
            {
                pthread_cond_signal(shutdownCond);
            }
            pthread_mutex_unlock(shutdownCondMutex);
            break;
        }

        if (sendStatus < 0) {
            printf("sendto() error: %s\n", strerror(errno));
            break;
        }
    }
    return NULL;
}

void send_init(List* myList, int socket, struct addrinfo* res, pthread_cond_t* cond, pthread_mutex_t* mutex, pthread_mutex_t* syncm,
                pthread_cond_t* shutdown, pthread_mutex_t* shutdownMutex)
{
    // setting the mutexes + cond variables...
    socketDescriptor = socket;
    list = myList;
    remoteAddress = res;

    sendMutex = mutex;
    syncWithInput = cond;
    syncWithInputMutex = syncm;

    shutdownCond = shutdown;
    shutdownCondMutex = shutdownMutex;


    // creating the thread...
    int threadStatus = pthread_create(&threadPID, NULL, &sendRoutine, NULL);
    if (threadStatus != 0) {
        printf("pthread_create() failed: %s\n", strerror(errno));
    }

}

void send_shutdown(void) 
{
    // cancel thread
    pthread_cancel(threadPID);

    // wait for thread to finish
    pthread_join(threadPID, NULL);

}
