#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#include "display.h"

#define MAX_BUFFER_LEN 1024

static pthread_t threadPID;
static pthread_mutex_t* displayMutex;

static pthread_cond_t* outputSync;
static pthread_mutex_t* syncMutex;

static List* list;
static char buffer[MAX_BUFFER_LEN];


void* displayRoutine(void* unused)
{
    while (1) {
        pthread_mutex_lock(syncMutex);
        {
            pthread_cond_wait(outputSync, syncMutex);
        }
        pthread_mutex_unlock(syncMutex);

        // critical section -- taking from "output" list. 
        pthread_mutex_lock(displayMutex);
        {
            // read from the list (consume)
            char* msg = (char*)List_trim(list);
            if (msg == NULL) {
                pthread_mutex_unlock(displayMutex);
                continue;
            }
            strcpy(buffer, msg);
            
            // free the message.
            free(msg);
            msg = NULL;
        }
        pthread_mutex_unlock(displayMutex);

        // display messages
        fputs("[remote]: ", stdout);
        fputs(buffer, stdout);
        fflush(stdout);     // flush out any remaining things in the buffer
    }
    return NULL;
}



void display_init(List* myList, pthread_cond_t* cond, pthread_mutex_t* mutex, pthread_mutex_t* syncm)
{
    list = myList;
    outputSync = cond;
    displayMutex = mutex;
    syncMutex = syncm;

    // creating the thread...
    int threadStatus = pthread_create(&threadPID, NULL, &displayRoutine, NULL);
    if (threadStatus != 0) {
        printf("pthread_create() failed: %s\n", strerror(errno));
    }
}

void display_shutdown(void)
{  
    // cancel thread
    pthread_cancel(threadPID);

    // wait for thread to finish
    pthread_join(threadPID, NULL);
}


