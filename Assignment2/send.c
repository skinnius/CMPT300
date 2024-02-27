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
// static pthread_cond_t  

static List* list;
static char* port;
static char* hostname;
static char* msg;
static int socketDescriptor;
static bool binded;
static struct addrinfo* temp;


bool isError(int val) {
    if (val < 0) {
        return true;
    }
    return false;
}


void* sendRoutine(void* unused)
{
    // struct addrinfo hint;
    // struct addrinfo* res;
    // bool binded = false;
    // char dst[IPV4_LEN];


    // memset(&hint, 0, sizeof(hint));
    // hint.ai_family = AF_INET;
    // hint.ai_socktype = SOCK_DGRAM;
    // hint.ai_flags = AI_PASSIVE;

    // int addrStatus = getaddrinfo(ip, port, &hint, &res);

    // if (addrStatus != 0) {
    //     printf("getaddrinfo() error: %s\n", strerror(errno));
    //     return NULL;
    // }

    // for (temp = res; temp != NULL; temp = temp->ai_next) {       // attempt to bind (wait hold on do we have to do this for UDP -- nope still need to bind.)

    //     socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);
    //     if (socketDescriptor == -1) {
    //         close(socketDescriptor);
    //         continue;
    //     }

    //     int bindStatus = bind(socketDescriptor, temp->ai_addr, temp->ai_addrlen);
    //     if (isError(bindStatus)) {
    //         close(socketDescriptor);
    //         continue;
    //     }

    //     binded = true;
    //     break;
    // }

    // if (!binded) {
    //     printf("failed to bind...\n");
    //     return NULL;
    // }

    // begin sending

    // setup struct for remote peer
    struct sockaddr_in sin;

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(hostname);        // host to network long
    sin.sin_port = htons(port);

    while (1) {
        pthread_mutex_lock(&sendMutex);
        {
            // read from the list (consume)
            if (List_count == 0) {
                // do something if no elemenets in list. (Hopefully should never happen)
                printf("no elements in list");
                return NULL;
            }
            else {
                // extract the message
                msg = (char*)List_trim(list);
            }

        }
        pthread_mutex_unlock(&sendMutex);

        // send the message out.
        int sendStatus = sendto(socketDescriptor, msg, sizeof(msg), 0, sin->ai_addr, temp->ai_addrlen);
        
        if (isError(sendStatus)) {
            printf("sendto() error: %s\n", strerror(errno));
        }
    }
    freeaddrinfo(res);
    close(socketDescriptor);
    return NULL;
}

void send_init(char* remotePort, char* remoteIP, List* myList, int socket)
{
    socketDescriptor = socket;
    port = remotePort;
    hostname = remoteIP;
    list = myList;

    int threadStatus = pthread_create(&threadPID, NULL, &sendRoutine, NULL);
    
    if (threadStatus != 0) {
        printf("pthread_create() failed: %s\n", strerror(errno));
        // do we have to free a thread? google it ig
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
