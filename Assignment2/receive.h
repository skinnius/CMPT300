#ifndef _RECEIVE_H_
#define _RECEIVE_H_
#include "list.h"


void receive_init(char* myPort, List* myList, int socket) ;

void receive_shutdown(void);


#endif