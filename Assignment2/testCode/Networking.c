// test implementation, code inspired by brian fraser and beej's networking guide
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>


#define MAX_LEN 1024
#define PORT 22110

int main(void) 
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

        // extract the value from the message: (processing)
        int incMe = atoi(messageRecv);      // atoi = ascii to int

        // compose the reply message:
        // WATCH FOR BUFFER OVERFLOWS
        
        char messageTx[MAX_LEN];
        sprintf(messageTx, "Math: %d + 1 = %d\n", incMe, incMe + 1);

        // Transmit a reply:

        sin_len = sizeof(sinRemote);
        sendto(socketDescriptor, messageTx, strlen(messageTx), 0, (struct sockaddr*) &sinRemote, sin_len);


    }


    // struct sockaddr_in sinRemote;
    // unsigned int sin_len = sizeof(sinRemote);
    // char messageRx[MAX_LEN];       // client buffer written into message bufer (buffer receive), make sure it does not overflow


    // // recvfrom is a blocking call!
    // int bytesRx = recvfrom(socketDescriptor, messageRx, MAX_LEN, 0, (struct sockaddr*)&sinRemote, &sin_len);

    // // sinRemote filled with sender's IP address


    // // null terminated (string):
    // int terminatedx = (bytesRx < MAX_LEN) ? bytesRx : MAX_LEN-1;
    // messageRx[terminatedx] = 0;

    // printf("Message received (%d bytes): '%s'\n", bytesRx, messageRx);


    // // -------------------------- CREATE REPLY ----------------------------

    // char messageTx[MAX_LEN];
    // sprintf(messageTx, "Hello %d\n", 42);

    // // -------------------------- SEND REPLY -------------------------------
    // sin_len = sizeof(sinRemote);
    // sendto(socketDescriptor, messageTx, strlen(messageTx), 
    // 0, 
    // (struct sockaddr*)&sinRemote, sin_len); // send to this socket. We will have client's IP address and port from receiving the message

    //------------------------- CLOSE SOCKET -------------------------------

    close(socketDescriptor);
    
}
