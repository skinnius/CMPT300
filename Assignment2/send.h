#ifndef _SEND_H_
#define _SEND_H_
#include <pthread.h>
#include "list.h"

void send_init(List* myList, int socket, struct addrinfo* res, pthread_cond_t* cond, pthread_mutex_t* mutex, pthread_mutex_t* syncm,
                pthread_cond_t* shutdown, pthread_mutex_t* shutdownMutex);

void send_shutdown(void);


#endif