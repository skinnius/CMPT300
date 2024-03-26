#include <stdio.h>
#include "list.h"


#define RUNNING 0
#define READY 1
#define BLOCKED 2


#define SEND 100
#define RECEIVE 101
#define REPLY 102

#define MAX_OCCURENCES 4

// PCB -> controls the processes
typedef struct PCB {
    long pid;           // process id
    int numOccurence;   // number of occurences in current priority
    int priority;       // prio (0, 1, 2)
    int currentPriority; // currentPriority may differ from original prio.
    int processState;   // RUNNING, READY, BLOCKED
    proc_message* message; // proc message
} pcb;


// proc message struct
typedef struct proc_message {
    long from;
    long to;
    int type;
    char *msg;

} proc_message;



// semaphore structure
typedef struct semaphore {
    bool initialized;
    int semaphoreVal;        // value of semaphore
    List* processList;       // list of processes

} semaphore;
