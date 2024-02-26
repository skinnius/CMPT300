#include <stdio.h>
#include <stdlib.h>
#include "sockets.h"

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
    char* remoteMachineName = argv[2];
    char* remotePortNum = argv[3];
    
    printf("your args: %s, %s, %s \n", localPortNum, remoteMachineName, remotePortNum);

    int setup = sockSetup(localPortNum);
    
    if (setup != 0) {
        printf("socket setup failure");
        return -1;
    }
    
}