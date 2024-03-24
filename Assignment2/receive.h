#ifndef _RECEIVE_H_
#define _RECEIVE_H_
#include <pthread.h>
#include "list.h"

void receive_init(List* myList, int socket, pthread_cond_t* cond, pthread_mutex_t* mutex, pthread_mutex_t* syncm,
                pthread_cond_t* shutdownCon, pthread_mutex_t* shutdownMutex);
                
void receive_shutdown(void);

#endif