#ifndef _SEND_H_
#define _SEND_H_
#include "list.h"

void send_init(char* remotePort, char* remoteIP, List* myList, int socket);

void send_shutdown(void);


#endif