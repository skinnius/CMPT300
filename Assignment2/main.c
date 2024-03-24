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
#include "list.h"
#include "send.h"
#include "receive.h"
#include "display.h"
#include "input.h"

// synchronization
static pthread_cond_t syncInput = PTHREAD_COND_INITIALIZER;
static pthread_cond_t syncOutput = PTHREAD_COND_INITIALIZER;

static pthread_mutex_t inputSyncMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t outputSyncMutex = PTHREAD_MUTEX_INITIALIZER;

static pthread_mutex_t inputListMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t outputListMutex = PTHREAD_MUTEX_INITIALIZER;

static pthread_cond_t shutdownCondition = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t shutdownMutex = PTHREAD_MUTEX_INITIALIZER;

void callShutdown()
{
    pthread_mutex_lock(&shutdownMutex);
    {
        // wait for shutdown signal
        pthread_cond_wait(&shutdownCondition, &shutdownMutex);

        // shutdown...
        receive_shutdown();
        send_shutdown();
        input_shutdown();
        display_shutdown();
        
    }
    pthread_mutex_unlock(&shutdownMutex);
}


int getSocketDescriptor(char* localPort)
{
    // initialization...

    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(localPort));
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketDescriptor < 0) {
        printf("socket() failure: %s\n", strerror(errno));
        return -1;
    }

    int bindStatus = bind(socketDescriptor, (struct sockaddr*)&addr, sizeof(addr));
    if (bindStatus < 0) {
        printf("failed to bind. %s\n", strerror(errno));
        close(socketDescriptor);
        return -1;
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

    int addrStatus = getaddrinfo(remoteHostname, remotePort, &hints, &res);
    
    if (addrStatus < 0) {
        printf("getaddrinfo() error: %s\n", gai_strerror(addrStatus));
        return NULL;
    }

    return res;             // res may need to be cleaned up...
}


// ---------- initiate s-talk --------------
int main(int argc, char* argv[])
{
    // check for correct number of args in the command line
    if (argc != 4) {
        printf("Incorrect number of arguments provided");
        return -1;
    }

    // vars for connection
    char* localPortNum = argv[1];
    char* remoteHostname = argv[2];
    char* remotePortNum = argv[3];

    // setup local socket
    int socket = getSocketDescriptor(localPortNum);
    if (socket < 0) {
        printf("socket setup failed. Please check arguments.\n");
        return -1;
    }

    // grab the address of the remote address. 
    struct addrinfo* remoteAddress = getRemoteAddress(remoteHostname, remotePortNum);
    if (remoteAddress == NULL) {
        printf("setup failed. Please check your remote address and port number.\n");
        close(socket);
        return -1;
    }

    // create the input and output lists
    List* inputList = List_create();
    List* outputList = List_create();

    receive_init(outputList, socket, &syncOutput, &outputListMutex, &outputSyncMutex, &shutdownCondition, &shutdownMutex);
    send_init(inputList, socket, remoteAddress, &syncInput, &inputListMutex, &inputSyncMutex, &shutdownCondition, &shutdownMutex);
    input_init(inputList, &syncInput, &inputListMutex, &inputSyncMutex);
    display_init(outputList, &syncOutput, &outputListMutex, &outputSyncMutex);

    callShutdown();
    close(socket);
    free(remoteAddress);
    List_free(inputList, free);
    List_free(outputList, free);
}

