#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>
#include "receiver.h"

#define DYNAMIC_LEN 128
#define MAX_LEN 1024
#define PORT 22110

static pthread_mutex_t dynamicMsgMutex = PTHREAD_MUTEX_INITIALIZER;

static pthread_t threadPID;
static int socketDescriptor;
static char* recvMessage; 

static char* dynamicMessage;

void* receiveThread(void* unused)
{

    // dynamically allocate a message
    pthread_mutex_lock(&dynamicMsgMutex);
    {
        strcpy(dynamicMessage, "Dynamic!");
    }
    pthread_mutex_unlock(&dynamicMsgMutex);

    // ------------------------------------- INITIAL SETUP ----------------------------------------
    struct sockaddr_in sin;

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);        // host to network long
    sin.sin_port = htons(PORT);

    // bind() really wants a sockaddr but our sockaddr_in is the same size and easier to use

    socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);
    bind(socketDescriptor, (struct sockaddr*) &sin, sizeof(sin));       // opens socket


    while (1) {
        // get the data (blocking call)
        // will change sin (address) to be the address of the client
        // note: sin passes info in and out of call ? chat what does this mean

        struct sockaddr_in sinRemote;       // whos sending the packet?
        unsigned int sin_len = sizeof(sinRemote);      
        char messageRecv[MAX_LEN];          // message buffer
        recvfrom(socketDescriptor, messageRecv, MAX_LEN, 0, (struct sockaddr*)&sinRemote, &sin_len);

        pthread_mutex_lock(&dynamicMsgMutex);
        {
            printf("%s >> %s: %s\n", dynamicMessage, recvMessage, messageRecv);
        }
        pthread_mutex_unlock(&dynamicMsgMutex);

        // printf("Message received: %s\n", messageRecv);
        // make it null terminated so string functions work
        // int terminateIdx = (bytesRecv < MAX_LEN) ? bytesRecv : MAX_LEN -1;
        // messageRecv[terminateIdx] = '\0';
        // printf("Message received (%d bytes): \n\n'%s'\n", bytesRecv, messageRecv);

        
        // char messageTx[MAX_LEN];

        // strcpy(messageTx, messageRecv);
        // printf("%s", messageTx);

        // // Transmit a reply:
        // sin_len = sizeof(sinRemote);
        // sendto(socketDescriptor, messageTx, strlen(messageTx), 0, (struct sockaddr*) &sinRemote, sin_len);
    }

    close(socketDescriptor);
}



void Receiver_init(char* rm)
{
    dynamicMessage = malloc(DYNAMIC_LEN);
    recvMessage = rm;
    pthread_create(&threadPID,          // PID (by pointer)
        NULL,                               // attributes
        &receiveThread,                    // function
        NULL);             // args
}

void Receiver_shutdown(void)
{
   // cancel thread
    pthread_cancel(threadPID);
    
    // waits for thread to finish
    pthread_join(threadPID, NULL);

    // cleanup memory
    pthread_mutex_lock(&dynamicMsgMutex);
    {
        free(dynamicMessage);
    }
    pthread_mutex_unlock(&dynamicMsgMutex);

}

void Receiver_changeDynamicMessage(char* newDynamic)
{
    pthread_mutex_lock(&dynamicMsgMutex);
    {
        strncpy(dynamicMessage, newDynamic, DYNAMIC_LEN);           // critical section! waow 
    }
    pthread_mutex_unlock(&dynamicMsgMutex);
}
