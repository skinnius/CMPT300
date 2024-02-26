#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "receiver.h"


int main(int argc, char* argv[]) 
{
    printf("starting...\n");

    // starting modules...
    Receiver_init("A message !! WAOW"); 

    // wait for user input...
    printf("Enter something to kill the receive thread\n");
    char x;
    scanf("%c", &x);


    // shutdown module
    Receiver_shutdown();
    printf("Done!\n");
    return 0;




        
    // pthread_t threadPID2;
    // pthread_create(&threadPID2, NULL, &receiveThread, "YOOOOOOOOOOOOOOOOOOOOOOOOO WTF");
    
    
    
    // pthread_join(threadPID, 
    // NULL);                      // return arg

    // pthread_join(threadPID2, NULL);
}