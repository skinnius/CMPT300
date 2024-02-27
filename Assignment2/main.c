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
#include "send.h"
#include "receive.h"
#include "display.h"


// ---------- initiate s-talk --------------
int main(int argc, char* argv[])
{
    // check for correct number of args in the command line
    if (argc != 5) {
        printf("Incorrect number of arguments provided");
        return -1;
    }

    // vars for connection
    char* localPortNum = argv[1];
    char* remoteHostname = argv[2];
    char* remotePortNum = argv[3];
    
    printf("your args: %s, %s, %s \n", localPortNum, remoteHostname, remotePortNum);

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
        return -1;
    }

    // create the input and output lists
    List* inputList = List_create();
    List* outputList = List_create();


    

}