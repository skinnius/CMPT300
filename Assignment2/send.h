#ifndef _SEND_H_
#define _SEND_H_
#include "list.h"

void send_init(List* myList, int socket, struct addrinfo* res);

void send_shutdown(void);


#endif