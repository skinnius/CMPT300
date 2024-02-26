#ifndef _SOCKETS_H_
#define _SOCKETS_H_

int sockSetup(char* localPortNum);
void receive(int socketFD);
void sendToDest(int socketFD, char* ip, char* port);

#endif


