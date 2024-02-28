#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "input.h"

#define MAX_BUFFER_LEN 1024

static List* list;
static char buffer[MAX_BUFFER_LEN];

static pthread_t threadPID;
static pthread_mutex_t inputMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t* psyncInput;



void* inputRoutine(void* unused)
{
    while (1) {
        
        // waits until go-ahead given by send().
        pthread_mutex_lock(&inputMutex);
        {
            pthread_cond_wait(psyncInput, &inputMutex);
        }
        pthread_mutex_unlock(&inputMutex);

        // empty buffer
        memset(buffer, 0, MAX_BUFFER_LEN);

        // code for reading userInput inspired from: https://www.digitalocean.com/community/tutorials/fgets-and-gets-in-c-programming
        
        if (fgets(buffer, MAX_BUFFER_LEN, 0) == NULL) {
            printf("stdin error\n");
            return NULL;
        }
        char* msg = (char*)malloc(sizeof(buffer) + 1);

        strncpy(msg, buffer, sizeof(msg));
        msg[sizeof(buffer)] = '\0';

        // check for clean shutdown to terminate
        if (sizeof(msg) == 3 && ((msg[0] == '!') && (msg[1] == '\n') && (msg[2] == '\0'))) {

            // do some shutdown stuff
            input_shutdown();
            free(msg);
            return NULL;
        }

        pthread_mutex_lock(&inputMutex);
        {
            if (List_count(list) == LIST_MAX_NUM_NODES) {
                printf("unable to add to list --- List full\n");
                free(msg);
                // wait here for free space. 
                pthread_cond_wait(psyncInput, &inputMutex);
            }
            else {
                List_prepend(list, msg);
            }
        }
        pthread_mutex_unlock(&inputMutex);
    }

    return NULL;
}



void input_init(List* myList, pthread_cond_t* cond)
{
    list = myList;
    psyncInput = cond;
    int threadStatus = pthread_create(&threadPID, NULL, inputRoutine, NULL);
    if (threadStatus != 0) {
        printf("thread() error: %s\n", strerror(errno));
    }
}

void input_shutdown(void)
{

}
