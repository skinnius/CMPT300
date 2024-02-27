#include <stdio.h>
#include <stdlib.h>
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
    char* remoteIP = argv[2];
    char* remotePortNum = argv[3];
    
    printf("your args: %s, %s, %s \n", localPortNum, remoteIP, remotePortNum);

    int socket = sockSetup(localPortNum);

    if (socket < 0) {
        printf("socket setup failed. Please check arguments.");
        return -1;
    }

    List* inputList = List_create();
    List* outputList = List_create();

    



}