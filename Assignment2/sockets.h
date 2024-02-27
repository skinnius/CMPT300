#ifndef _SOCKETS_H
#define _SOCKETS_H

int getSocketDescriptor(char* localPort);

struct addrinfo* getRemoteAddress(char* remoteHostname, char* remotePort);
#endif