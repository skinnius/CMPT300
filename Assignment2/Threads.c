#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>


#define MAX_LEN 1024
#define PORT 22110


void* receiveThread(void* msgArg)
{
// ------------------------------------- INITIAL SETUP ----------------------------------------
    struct sockaddr_in sin;

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);        // host to network long
    sin.sin_port = htons(PORT);

    // bind() really wants a sockaddr but our sockaddr_in is the same size and easier to use

    int socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);
    bind(socketDescriptor, (struct sockaddr*) &sin, sizeof(sin));       // opens socket


    while (1) {
        // get the data (blocking call)
        // will change sin (address) to be the address of the client
        // note: sin passes info in and out of call ? chat what does this mean

        struct sockaddr_in sinRemote;       // whos sending the packet?
        unsigned int sin_len = sizeof(sinRemote);      
        char messageRecv[MAX_LEN];          // message buffer
        int bytesRecv = recvfrom(socketDescriptor, messageRecv, MAX_LEN, 0, (struct sockaddr*)&sinRemote, &sin_len);


        // make it null terminated so string functions work
        int terminateIdx = (bytesRecv < MAX_LEN) ? bytesRecv : MAX_LEN -1;
        messageRecv[terminateIdx] = '\0';
        printf("Message received (%d bytes): \n\n'%s'\n", bytesRecv, messageRecv);

        
        char messageTx[MAX_LEN];

        strcpy(messageTx, messageRecv);
        printf("%s", messageTx);

        // Transmit a reply:
        sin_len = sizeof(sinRemote);
        sendto(socketDescriptor, messageTx, strlen(messageTx), 0, (struct sockaddr*) &sinRemote, sin_len);
    }

    close(socketDescriptor);
}




int main(int argc, char* argv[]) 
{
    printf("starting...\n");
    pthread_t threadPID;
    pthread_create(&threadPID,          // PID (by pointer)
        NULL,                               // attributes
        &receiveThread,                    // function
        NULL);             // args

    // wait for user input...
    printf("Enter something to kill the receive thread\n");
    char x;
    scanf("%c", &x);


    // cancel thread
    pthread_cancel(threadPID);
    
    // waits for thread to finish
    pthread_join(threadPID, NULL);
    
    // pthread_t threadPID2;
    // pthread_create(&threadPID2, NULL, &receiveThread, "YOOOOOOOOOOOOOOOOOOOOOOOOO WTF");
    
    
    
    // pthread_join(threadPID, 
    // NULL);                      // return arg

    // pthread_join(threadPID2, NULL);


    printf("Done!\n");
    return 0;
}