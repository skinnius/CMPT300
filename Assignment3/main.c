#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#include "list.h"
#include "dataStructures.h"

static List* readyQueue[3];    // 3 ready queues

static List* sendList;      // queue of processes waiting on send operation
static List* receiveList;   // queue of processes waiting on receive operation

static semaphore* semaphores[5];        // 5 semaphores
static pcb* runningProcess;
static pcb* init;
static bool run;

static int currPID = 0;

/*-------------------------------------- misc -----------------------------------------------------*/

// pid equals comparator
bool equalsComparator(void* pItem, void* pComparisonArg) {
    return (((pcb*)pItem)->pid == *(int*)pComparisonArg);
}

// free item 
void freeItem(void* pItem) {
    free(pItem);
    pItem = NULL;
}

/** ------------------------------------------- Scheduling ------------------------------------------ **/

void cpu_scheduler() {
    // go through the three different queue levels
    for (int i = 0; i < 3; i++) {
        List* currList = readyQueue[i];
        if (List_count(currList) == 0) {
            continue;
        }

        pcb* newRunningProcess = List_trim(currList);
        newRunningProcess->processState = RUNNING;

        if (newRunningProcess->numOccurence == MAX_OCCURENCES && newRunningProcess->currentPriority != 2) {
            newRunningProcess->currentPriority++;
            newRunningProcess->numOccurence = 0;
        }

        newRunningProcess->numOccurence++;
        runningProcess = newRunningProcess;
        return; 
    }
    // if nothing in queues
    runningProcess = init;
}



/* ---------------------------------------- init --------------------------------------------------*/

// creating the init process -- called once and never again. 
pcb* createInitProcess() {
    pcb* init = (pcb*)malloc(sizeof(pcb));
    init->pid = currPID;
    currPID++;
    init->processState = RUNNING;

    return init;
}



/* ---------------------------------------- Create --------------------------------------------------*/

// creating a new process -- return pid on success, -1 on failure.

int createNewProcess(int prio) {
    pcb* newProcess = (pcb*)malloc(sizeof(pcb));

    if (newProcess == NULL) {
        return -1;
    }

    newProcess->pid = currPID;
    currPID++;
    newProcess->priority = prio;
    newProcess->currentPriority = prio;
    newProcess->processState = READY;
    newProcess->numOccurence = 0;
    newProcess->message = (proc_message*)malloc(sizeof(proc_message));

    int success = List_prepend(readyQueue[prio], newProcess);
    
    if (success < 0) {
        return -1;
    }

    return newProcess->pid;
}


int createProcessInterface() {
    int prio;
    int pid;

    printf("input process priority (0 = high, 1 = norm, 2 = low): ");
    scanf("%d", &prio);
    

    if (prio < -1 || prio > 2) {
        printf("invalid input.\n");
        return -1;
    }
    
    pid = createNewProcess(prio);
    return pid;
}

void createReport(int pid) {
    if (pid < 0) {
        printf("Process creation failed.\n");
    }
    else {
        printf("Process %d successfully created.", pid);
    }
}


/**--------------------------------------------- Fork --------------------------------------------------**/

// returns -1 on failure, pid on success. 
int forkCommandInterface() {

    if (runningProcess->pid == 0) {         // 0 = pid of init
        return -1;
    }

    pcb* processCopy = (pcb*)malloc(sizeof(pcb));
    processCopy->pid = currPID;
    currPID++;
    processCopy->priority = runningProcess->currentPriority;
    processCopy->processState = READY;

    int success = List_prepend(readyQueue[processCopy->priority], processCopy);

    if (success < 0) {
        return -1;
    }

    return processCopy->pid;
}

void forkReport(int successState) {
    if (successState < 0) {
        printf("Fork command failed. \n");
    }
    else {
        printf("Fork command successful. pid of the new process is %d", successState);
    }
}


/**--------------------------------------------- Kill --------------------------------------------------**/ 

// KILL EVERY PROCESS OCCURENCE IN ALL QUEUES

int removeProcessInList(List* pList, int* pid) {
    List_first(pList);
    pcb* process = List_search(pList, equalsComparator, pid);
    if (process != NULL) {
        List_remove(pList);
        return 1;
    }
    return 0;
}


void killReport(int num, int id) {
    if (num < 0) {
        printf("Kill failed");
    }
    else if (num == 0) {
        printf("No process with matching id found");
    }
    else {
        printf("%d instances of process %d killed", num, id);
    }
}


int killProcess(int pid) {

    if (pid == 0) {
        if (List_count(readyQueue[0]) == 0 && List_count(readyQueue[1]) == 0 && List_count(readyQueue[2]) == 0) {
            for (int i = 0; i < 3; i++) {
                List_free(readyQueue[i], free);
            }
            List_free(sendList, free);
            List_free(receiveList, free);

            for (int i = 0; i < 5; i++) {
                List_free(semaphores[i]->processList, free);
                free(semaphores[i]);
            }
            run = false;
            return 1;
        }

        else {
            printf("Cannot kill init process while other processes are still on ready queue\n");
            return -1;
        }
    }

    int occurenceCounter = 0;
    
    // check running process
    if (runningProcess->pid == pid) {
        free(runningProcess);
        runningProcess = NULL;
        occurenceCounter += 1;
        cpu_scheduler();                    // get next scheduled process from queue
    }


    // check through all ready queues
    for (int i = 0; i < 3; i++) {           
        List* currList = readyQueue[i];
        occurenceCounter += removeProcessInList(currList, &pid);
    }

    // check through send list
    occurenceCounter += removeProcessInList(sendList, &pid);
    // check through receive list
    occurenceCounter += removeProcessInList(receiveList, &pid);

    // check through all semaphore queues
    for (int i = 0; i < 5; i++) {           
        semaphore* currSemaphore = semaphores[i];
        List* currList = currSemaphore->processList;
        occurenceCounter += removeProcessInList(currList, &pid);
    }

    return occurenceCounter;
}

void killProcessInterface() {
    int pid;

    printf("input pid: ");
    scanf("%d", &pid);

    int occurenceCounter = killProcess(pid);
    killReport(occurenceCounter, pid);
}




// // add old running process back into a queue
//         List_prepend(readyQueue[runningProcess->priority], runningProcess);
//         runningProcess->priority = READY;
//         // runningProcess->proc_message = //idk do something with the message



/**--------------------------------------------- Exit --------------------------------------------------**/ 

void exitProcess() {
    killProcess(runningProcess->pid);
    printf("process %d has control of the CPU", runningProcess->pid);
}

/**--------------------------------------------- Quantum --------------------------------------------------**/ 

void quantum() {

    if (runningProcess->pid == 0) {
        cpu_scheduler();
        printf("process %d has control of the CPU", runningProcess->pid);
        return;
    }

    runningProcess->processState = READY;
    List_prepend(readyQueue[runningProcess->currentPriority], runningProcess);
    cpu_scheduler();
    printf("process %d has control of the CPU", runningProcess->pid);
}

/**--------------------------------------------- Send --------------------------------------------------**/ 

int attemptSend(List* pList, int pid, char* msg) {
    List_first(pList);
    pcb* process = List_search(pList, equalsComparator, &pid);

    if (process != NULL) {                  // process has been found
        process->message->msg = msg;
        process->message->from = runningProcess->pid;       // came from currently running process
        
        // add process waiting on receive back onto the ready queue
        process->processState = READY;
        List_prepend(readyQueue[process->pid], process);

        // block the process that called for a "send"
        runningProcess->processState = BLOCKED;
        List_prepend(pList, runningProcess);
        cpu_scheduler();
        return 0;
    }
    return -1;
}



// return 0 on success, -1 on failure
int sendInterface() {
    int pid;
    char* msg = malloc(40);

    printf("input pid: ");
    scanf("%d", &pid);
    while(getchar() != '\n');

    
    printf("input message (40 characters max): \n");
    scanf("%s", msg);
    while(getchar() != '\n');

    
    // check queues waiting on a send
    if (attemptSend(receiveList, pid, msg) == 0) {
        return 0;
    }
    // check ready queues for process. 
    for (int i = 0; i < 3; i++) {
        List* pList = readyQueue[i];
        if (attemptSend(pList, pid, msg) == 0) {
            return 0;
        }
    }  

    // check semaphore queues for process.
    for (int i = 0; i < 5; i++) {
        semaphore* currSem = semaphores[i];
        List* pList = currSem->processList;
        if (attemptSend(pList, pid, msg) == 0) {
            return 0;
        } 
    }
    // no matches
    return -1;
}



// make the reports nice later
void sendReport(int status) {
    if (status == -1) {
        printf("failed to send");
    }
    else {
        printf("successfully sent");
    }
}

/**--------------------------------------------- Receive --------------------------------------------------**/
// return 0 for block, 1 for no block. 

int receive() {
    // check if theres a proc message of type RECEIVE.
    if (runningProcess->message->type == RECEIVE) {
        printf("received a message from process %d: %s", runningProcess->message->from, 
                runningProcess->message->msg);
        return 1;
    }

    // if nothing to be received, throw running process back onto ready queue.
    else {
        runningProcess->processState = BLOCKED;
        List_prepend(readyQueue[runningProcess->priority], runningProcess);
        cpu_scheduler();
        return 0;
    }

}

void receiveReport(int status) {
    if (status == 0) {
        printf("SCHEDULING WHOOO ");     // some scheduling stuff. 
    }
    else {
        printf("Current process is still running.");
    }
}

/**--------------------------------------------- Reply --------------------------------------------------**/

int attemptReply(List* pList, int pid, char* msg) {
    List_first(pList);
    pcb* process = List_search(pList, equalsComparator, &pid);

    if (process != NULL) {                                  // process has been found
        process->message->msg = msg;
        process->message->type = REPLY;
        process->message->from = runningProcess->pid;       // came from currently running process
        
        // add process waiting on reply back onto the ready queue
        process->processState = READY;
        List_prepend(readyQueue[process->pid], process);

        return 0;
    }
    return -1;
}

int replyInterface() {
    int pid;
    char* msg = malloc(40);

    printf("input pid: ");
    scanf("%d", &pid);

    printf("input message (40 characters max): \n");
    scanf("%s", msg);

    
    // check queues waiting on a send
    if (attemptSend(receiveList, pid, msg) == 0) {
        return 0;
    }
    // check ready queues for process. 
    for (int i = 0; i < 3; i++) {
        List* pList = readyQueue[i];
        if (attemptReply(pList, pid, msg) == 0) {
            return 0;
        }
    }  

    // check semaphore queues for process.
    for (int i = 0; i < 5; i++) {
        semaphore* currSem = semaphores[i];
        List* pList = currSem->processList;
        if (attemptSend(pList, pid, msg) == 0) {
            return 0;
        } 
    }
    // no matches
    return -1;
}


void replyReport(int status) {
    if (status == -1) {
        printf("reply failed\n");
    } 
    else {
        printf("reply successful\n");
    }
}


/** ------------------------------------------- New Semaphore ------------------------------------------ **/

void newSemaphore() {
    int semaphoreID;
    int initVal;

    printf("input semaphore ID: ");
    scanf("%d", &semaphoreID);

    semaphore* s = semaphores[semaphoreID];

    if (s->initialized == true) {
        printf("semaphore already initialized. Semaphore creation failed \n");
    }

    printf("input initial value: ");
    scanf("%d", &initVal);

    s->semaphoreVal = initVal;
    s->initialized = true;
    printf("semaphore successfully created\n");
}

/** ------------------------------------------- Semaphore (P) ------------------------------------------ **/

void pSemaphore() {
    int semaphoreID;
    printf("input a semaphore id: \n");
    scanf("%d", &semaphoreID);

    if (semaphoreID < 0 || semaphoreID > 4) {
        printf("invalid input\n");
        return;
    }

    semaphore* s = semaphores[semaphoreID];
    s->semaphoreVal--;
    if (s->semaphoreVal < 0) {
        List_prepend(s->processList, runningProcess);
        // block
        runningProcess->processState = BLOCKED;
        List_prepend(readyQueue[runningProcess->priority], runningProcess);
        cpu_scheduler();
        printf("Previously running process was blocked. Process %d is now running\n", runningProcess->pid);
    }
    else {
        printf("Process was not blocked. Current process is still running \n");
    }

}

/** ------------------------------------------- Semaphore (V) ------------------------------------------ **/

void vSemaphore() {
    int semaphoreID;
    printf("input a semaphore id: \n");
    scanf("%d", &semaphoreID);

    if (semaphoreID < 0 || semaphoreID > 4) {
        printf("invalid input\n");
        return;
    }
    
    semaphore* s = semaphores[semaphoreID];
    s->semaphoreVal++;
    if (s->semaphoreVal <= 0) {
        
        pcb* unblockedProcess = List_trim(s->processList);
        unblockedProcess->processState = READY;
        List_prepend(readyQueue[unblockedProcess->currentPriority], unblockedProcess);
        cpu_scheduler();
        printf("Process %d was placed on the ready queue. \n", unblockedProcess->pid);
    }
    else {
        printf("No processes are blocked on semaphore %d \n", semaphoreID);
    }
}

/** ------------------------------------------- Procinfo ------------------------------------------ **/

void dumpInfo(pcb* process) {
    printf("---------Process %d----------: \n", process->pid);
    switch (process->priority) {
        case 0:
            printf("high priority \n");
            break;
        case 1:
            printf("medium priority \n");
            break;
        case 2: 
            printf("low priority \n");
            break;
    }

    switch (process->processState) {
        case 0:
            printf("RUNNING\n");
            break;
        case 1:
            printf("READY\n");
            break;
        case 2:
            printf("BLOCKED\n");
            break;
    }    
}

void procInfo() {
    int pid;
    printf("input a process id: ");
    scanf("%d", &pid);

    if (runningProcess->pid == pid) {
        dumpInfo(runningProcess);
    }

    // scan through each queue to see if the process exists
    for (int i = 0; i < 3; i++) {
        List* currList = readyQueue[i];
        pcb* p = List_search(currList, equalsComparator, &pid);
        if (p != NULL) {
            dumpInfo(p);
        }
    }

    // scan through receive and send
    pcb* p = List_search(sendList, equalsComparator, &pid);
    if (p != NULL) {
        dumpInfo(p);
    }

    p = List_search(receiveList, equalsComparator, &pid);
    if (p != NULL) {
        dumpInfo(p);
    }

    // scan through semaphores
    for (int i = 0; i < 5; i++) {
        semaphore* currSem = semaphores[i];
        List* pList = currSem->processList;
        pcb* p = List_search(pList, equalsComparator, &pid);
        if (p != NULL) {
            dumpInfo(p);
        }
    }

    // its over
    printf("No processes with pid %d found", pid);

}

/** ------------------------------------------- Total Info ------------------------------------------ **/
void displayQueues() {
    // display all ready queues
    printf("~~~~~~~~~~~~~~~~~~~~ READY QUEUES: ~~~~~~~~~~~~~~~~~~~~~~");
    for (int i = 0; i < 5; i++) {
        List* currList = readyQueue[i];
        List_last(currList);
        

        pcb* p = List_curr(currList);
        int counter = 1;
        while (p != NULL) {
            printf("======= %d ========\n", counter);
            dumpInfo(p);
            List_prev(currList);
            counter++;
        }
    }

    // display all send
    printf("~~~~~~~~~~~~~~~~~~~~ SEND QUEUES: ~~~~~~~~~~~~~~~~~~~~~~");
    List_last(sendList);
    pcb* p = List_curr(sendList);
    int counter = 1;

    while (p != NULL) {
        printf("========= %d =========\n", counter);
        dumpInfo(p);
        List_prev(sendList);
        counter++;
    }

    // display all receive
    printf("~~~~~~~~~~~~~~~~~~~~ RECEIVE QUEUES: ~~~~~~~~~~~~~~~~~~~~~~");
    List_last(receiveList);
    p = List_curr(receiveList);
    counter = 1;

    while (p != NULL) {
        printf("========= %d =========\n", counter);
        dumpInfo(p);
        List_prev(sendList);
        counter++;
    }


    printf("~~~~~~~~~~~~~~~~~~~~ SEMAPHORE QUEUES: ~~~~~~~~~~~~~~~~~~~~~~");
    // display all semaphore queues
    for (int i = 0; i < 5; i++) {
        semaphore* currSem = semaphores[i];
        List* currList = currSem->processList;
        
        pcb* p = List_curr(currList);
        int counter = 1;
        while (p != NULL) {
            printf("======= %d ========\n", counter);
            dumpInfo(p);
            List_prev(currList);
            counter++;
        }
    }

}

void totalInfo() {
    displayQueues();
}



/** ------------------------------------------- User Input------------------------------------------ **/


// returns 0 on success, -1 on failure. 
int chooseFunction(char input) {
    int pid, fork, sendStatus, receiveStatus, replyStatus;       
    switch(input) {
        case 'C':
            pid = createProcessInterface();
            createReport(pid);
            break;

        case 'F':
            fork = forkCommandInterface();
            forkReport(fork);
            break;

        case 'K':
            killProcessInterface();
            break;
        case 'E':
            exitProcess();
            break;
        case 'Q': 
            quantum();
            break;
        case 'S': 
            sendStatus = sendInterface();
            sendReport(sendStatus);
            break;
        case 'R':
            receiveStatus = receive();
            receiveReport(receiveStatus);
            break;
        case 'Y':
            replyStatus = replyInterface();
            replyReport(replyStatus);
            break;
        case 'N':
            newSemaphore();
            break;
        case 'P':
            pSemaphore();
            break;
        case 'V':
            vSemaphore();
            break;
        case 'I':
            procInfo();
            break;
        case 'T':
            totalInfo();
            break;

        default:
            return -1;          // no matches
    }
    return 0;
}


void initializeLists() {
    for (int i = 0; i < 3; i++) {
        readyQueue[i] = List_create();
    }

    sendList = List_create();
    receiveList = List_create();

    for (int i = 0; i < 5; i++) {
        semaphores[i] = malloc(sizeof(semaphore));

        semaphores[i]->processList = List_create();
        semaphores[i]->initialized = false;
    }
}

int main() {
    char userInput;
    init = createInitProcess();
    runningProcess = init;
    
    run = true;
    initializeLists();

    while (run) {
        printf("Enter a command: ");
        scanf("%c", &userInput);

        if (chooseFunction(toupper(userInput)) == -1) {
            printf("invalid input");
        }

        while(getchar() != '\n');           // clear buffer
        printf("\n");
    }

    printf("skinOS simulation terminated :D\n");

}