#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <pthread.h>
#include "list.h"

void display_init(List* myList, pthread_cond_t* cond, pthread_mutex_t* mutex, pthread_mutex_t* syncm);

void display_shutdown(void);

#endif