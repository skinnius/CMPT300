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
#include "send.h"

#define MAX_LEN 1024
#define MAX_BUFFER_LEN 256
#define IPV4_LEN 15

static pthread_t threadPID;
static pthread_mutex_t sendMutex = PTHREAD_MUTEX_INITIALIZER; 
static pthread_cond_t condSendMutex = PTHREAD_COND_INITIALIZER;

static List* list;
static char buffer[MAX_BUFFER_LEN];
static int socketDescriptor;
static struct addrinfo* remoteAddress;


void* sendRoutine(void* unused)
{
    while (1) {

        // wait until confirmation that there is space in the list.... 
        pthread_mutex_lock(&sendMutex);
        {
            pthread_cond_wait(&condSendMutex, &sendMutex);
        }
        pthread_mutex_unlock(&sendMutex);

        // copy the last element of the list into a buffer 
        pthread_mutex_lock(&sendMutex);
        {
            // read from the list (consume)
            if (List_count(list) == 0) {
                // do something if no elemenets in list. (Hopefully should never happen)
                printf("no elements in list");
                return NULL;
            }
            else {
                // extract the message
                char* msg = (char*)List_trim(list);
                strncpy(buffer, msg, MAX_BUFFER_LEN);
                free(msg);
                msg = NULL;
            }
        }
        pthread_mutex_unlock(&sendMutex);

        // send the message out.
        int sendStatus = sendto(socketDescriptor, buffer, sizeof(buffer), 0, remoteAddress->ai_addr, remoteAddress->ai_addrlen);
        
        if (sendStatus) {
            printf("sendto() error: %s\n", strerror(errno));
        }
    }
    return NULL;
}

void send_init(List* myList, int socket, struct addrinfo* res)
{
    socketDescriptor = socket;
    list = myList;
    remoteAddress = res;

    int threadStatus = pthread_create(&threadPID, NULL, &sendRoutine, NULL);
    
    if (threadStatus != 0) {
        printf("pthread_create() failed: %s\n", strerror(errno));
        // do we have to free a failed thread? google it ig
    }

}

void send_shutdown(void) 
{
    // cancel thread
    pthread_cancel(threadPID);

    // wait for thread to finish
    pthread_join(threadPID, NULL);

    // cleanup mem
    pthread_mutex_lock(&sendMutex);
    {
        free(list);
    }
    pthread_mutex_unlock(&sendMutex);
}
