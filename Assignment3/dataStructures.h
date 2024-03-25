#include <stdio.h>

#define RUNNING 1
#define READY 0
#define BLOCKED -1


// PCB -> controls the processes
typedef struct PCB {
    long pid;           // process id
    int priority;       // prio (0, 1, 2)
    int processState;   // RUNNING, READY, BLOCKED
    char message[];     // idk whats supposed to go in here lul
} pcb;



// semaphore structure
typedef struct semaphore {
    int semaphoreVal;       // value of semaphore
    pcb* processList;       // list of processes

} semaphore;
