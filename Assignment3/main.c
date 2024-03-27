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


void resetMessage() {
    runningProcess->message->from = 0;
    runningProcess->message->type = 0;
    strcpy(runningProcess->message->msg, "");
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

        if (run) {
            printf("process %d has control of the CPU\n", runningProcess->pid);
        }

        if (runningProcess->message->type == REPLY) {
            printf("reply received from process %d: \n", runningProcess->message->from);
            printf("\"%s\"\n", runningProcess->message->msg);
            resetMessage();
        }
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
    init->priority = -1;
    init->currentPriority = -1;
    init->numOccurence = -1;

    init->message = (proc_message*)malloc(sizeof(proc_message));
    init->message->msg = (char*)malloc(40);
    init->message->type = 0;
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
    newProcess->message->msg = (char*)malloc(40);
    newProcess->message->type = 0;

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
        printf("Process creation failed.");
    }
    else {
        printf("Process %d successfully created.", pid);
    }
}


/**--------------------------------------------- Fork --------------------------------------------------**/

void forkReport(int pid, int priority) {
    printf("Fork command successful. Process %d has priority %d", pid, priority);
}


void forkCommandInterface() {

    if (runningProcess->pid == 0) {         // 0 = pid of init
        printf("Cannot Fork the init process.\n");
        printf("Fork command failed. ");
        return;
    }

    int pid = createNewProcess(runningProcess->priority);

    if (pid < 0) {
        printf("Fork command failed. \n");
        return;
    }

    forkReport(pid, runningProcess->priority);
}



/**--------------------------------------------- Kill --------------------------------------------------**/ 

// KILL EVERY PROCESS OCCURENCE IN ALL QUEUES

void freeProcess(pcb* p) {
    free(p->message->msg);
    free(p->message);
    free(p);
    p = NULL;
}

int removeProcessInList(List* pList, int* pid) {
    List_first(pList);
    pcb* process = List_search(pList, equalsComparator, pid);
    if (process != NULL) {
        pcb* p = List_remove(pList);
        freeProcess(p);
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


void killProcess(int pid) {

    if (pid == 0) {
        if (List_count(readyQueue[0]) == 0 && List_count(readyQueue[1]) == 0 && List_count(readyQueue[2]) == 0 
            && List_count(receiveList) == 0 && List_count(sendList) == 0
            && List_count(semaphores[0]->processList) == 0 && List_count(semaphores[1]->processList) == 0 
            && List_count(semaphores[2]->processList) == 0 && List_count(semaphores[3]->processList) == 0 
            && List_count(semaphores[4]->processList) == 0 
            && runningProcess->pid == 0) {

            freeProcess(runningProcess);
            run = false;
            killReport(1, pid);
            return;
        }

        else {
            printf("Cannot kill init process while other processes are still in the system\n");
            return;
        }
    }

    int occurenceCounter = 0;
    
    // check running process
    if (runningProcess->pid == pid) {
        occurenceCounter += 1;
        freeProcess(runningProcess);
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

    killReport(occurenceCounter, pid);

}

void killProcessInterface() {
    int pid;

    printf("input pid: ");
    scanf("%d", &pid);
    killProcess(pid);
}


/**--------------------------------------------- Exit --------------------------------------------------**/ 

void exitProcess() {
    killProcess(runningProcess->pid);
}

/**--------------------------------------------- Quantum --------------------------------------------------**/ 

void quantum() {

    if (runningProcess->pid == 0) {
        cpu_scheduler();
        return;
    }

    runningProcess->processState = READY;
    List_prepend(readyQueue[runningProcess->currentPriority], runningProcess);
    cpu_scheduler();
}

/**--------------------------------------------- Send --------------------------------------------------**/ 

pcb* attemptSend(List* pList, int pid, char* msg, bool* inList) {
    // empty list case
    if (List_first(pList) == NULL) {
        return NULL;
    }

    pcb* process = List_search(pList, equalsComparator, &pid);

    if (process != NULL) {                  // process has been found
        if (process->message->type == SEND) {
            printf("process %d already receiving a message \n", process->pid);
            *inList = true;
            return NULL;
        }
        List_remove(receiveList);
        strcpy(process->message->msg, msg);
        process->message->type = SEND;
        process->message->from = runningProcess->pid;       // came from currently running process
    }

    return process;       
}

int block(List* pList) {
    // block the process that called for a "send"
    runningProcess->processState = BLOCKED;
    if (List_prepend(pList, runningProcess) == -1) {
        printf("List_prepend failed\n");
        return -1;
    }
    int tempPid = runningProcess->pid;
    cpu_scheduler();
    printf("Process %d has been put onto blocked queue. Process %d is now running.\n", tempPid, runningProcess->pid);
    return 0;
}

// return 0 on success, -1 on failure
int sendInterface() {

    if (runningProcess->pid == 0) {     // init process not allowed to send -> init cannot be blocked.
        printf("init process cannot use the send operation.\n");
        return -1;
    }

    int pid;
    char* msg = malloc(40);
    bool inList = false;

    printf("input pid: ");
    scanf("%d", &pid);
    while(getchar() != '\n');

    
    printf("input message (40 characters max): \n");
    scanf(" %[^\n]", msg);

    if (strlen(msg) > 40) {
        printf("message too long.\n");
        free(msg);
        return -1;
    }

    if (pid == 0) {
        if (init->message->type == SEND) {
            printf("process %d already receiving a message\n", init->pid);
            free(msg);
            return -1;
        }
        else {
            strcpy(init->message->msg, msg);
            init->message->type = SEND;
            init->message->from = runningProcess->pid;       // came from currently running process
            if (block(sendList) == -1) {free(msg); return -1;}
            free(msg);
            return 0;
        }
    }

    // check queues waiting on a send (receiveList)
    pcb* process = attemptSend(receiveList, pid, msg, &inList);
    if (process != NULL) {
        // add process waiting on receive back onto the ready queue
        process->processState = READY;
        if (List_prepend(readyQueue[process->priority], process) == -1) {free(msg); return -1;}
        if (block(sendList) == -1) {free(msg); return -1;}
        free(msg);
        return 0;
    }

    // check queues waiting on a reply (sendList)
    process = attemptSend(sendList, pid, msg, &inList);
    if (process != NULL) {
        if (block(sendList) == -1) {free(msg); return -1;}
        free(msg);
        return 0;
    }

    // check ready queues for process.
    for (int i = 0; i < 3; i++) {
        pcb* process = attemptSend(readyQueue[i], pid, msg, &inList); 
        if (process != NULL) {
            if (block(sendList) == -1) {free(msg); return -1;}
            free(msg);
            return 0;
        }

    }  

    // check semaphore queues for process.
    for (int i = 0; i < 5; i++) {
        semaphore* s = semaphores[i];
        pcb* process = attemptSend(s->processList, pid, msg, &inList);
        if (process != NULL) {
            if (block(sendList) == -1) {free(msg); return -1;}
            free(msg);
            return 0;
        }
    }

    // no matches
    if (!inList) {
        printf("could not find any processes with pid %d\n", pid);
    }
    free(msg);
    return -1;
}



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


void receive() {

    // special case for init
    if (runningProcess->pid == 0) {
        // check if theres a proc message of type SEND.
        if (runningProcess->message->type == SEND) {
            printf("received a message from process %d: \n \"%s\" \n", runningProcess->message->from, 
                    runningProcess->message->msg);
            printf("Current process is still running.");
            resetMessage();
            return;
        }

        // if no message
        else {
            printf("No messages for init. Receive command failed.");
            return;
        }
    }


    // check if theres a proc message of type SEND
    if (runningProcess->message->type == SEND) {
        printf("received a message from process %d: \n \"%s\" \n", runningProcess->message->from, 
                runningProcess->message->msg);
        printf("Current process is still running.");
        resetMessage();
    }

    // if nothing to be received, throw running process back onto receive queue.
    else {
        runningProcess->processState = BLOCKED;
        int tempPid = runningProcess->pid;
        List_prepend(receiveList, runningProcess);
        cpu_scheduler();
        printf("Process %d has been put onto blocked queue. Process %d is now running.", tempPid, runningProcess->pid);
    }

}


/**--------------------------------------------- Reply --------------------------------------------------**/

int replyInterface() {
    int pid;
    char* msg = malloc(40);

    printf("input pid: ");
    scanf("%d", &pid);

    printf("input message (40 characters max): \n");
    scanf(" %[^\n]", msg);

    if (strlen(msg) > 40) {
        printf("message too long.\n");
        free(msg);
        return -1;
    }

    List_first(sendList);
    pcb* process = List_search(sendList, equalsComparator, &pid);

    if (process != NULL) {                                  // process has been found
        List_remove(sendList);
        strcpy(process->message->msg, msg);
        process->message->type = REPLY;
        process->message->from = runningProcess->pid;       // came from currently running process
        
        // add process waiting on reply back onto the ready queue
        process->processState = READY;
        List_prepend(readyQueue[process->currentPriority], process);
        free(msg);
        return 0;
    }
    free(msg);
    return -1;
}


void replyReport(int status) {
    if (status == -1) {
        printf("reply failed");
    } 
    else {
        printf("reply successful");
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
        return;
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
    printf("input a semaphore id: ");
    scanf("%d", &semaphoreID);

    if (semaphoreID < 0 || semaphoreID > 4) {
        printf("invalid input\n");
        return;
    }

    if (semaphores[semaphoreID]->initialized == false) {
        printf("semaphore %d not yet initialized.\n", semaphoreID);
        printf("P failed.");
        return;
    }

    semaphore* s = semaphores[semaphoreID];
    s->semaphoreVal--;
    if (s->semaphoreVal < 0) {

        if (runningProcess->pid == 0) {
            printf("init cannot be blocked. Current process is still running.\n");
            printf("P failed.");
            s->semaphoreVal++;
            return;
        }
        block(s->processList);
    }
    else {
        printf("Process was not blocked. Current process is still running \n");
    }

    printf("P successful. Semaphore %d value has decremented to %d", semaphoreID, s->semaphoreVal);
}


/** ------------------------------------------- Semaphore (V) ------------------------------------------ **/

void vSemaphore() {
    int semaphoreID;
    printf("input a semaphore id: ");
    scanf("%d", &semaphoreID);

    if (semaphoreID < 0 || semaphoreID > 4) {
        printf("invalid input\n");
        return;
    }

    if (semaphores[semaphoreID]->initialized == false) {
        printf("semaphore %d not yet initialized.\n", semaphoreID);
        printf("V failed.");
        return;
    }

    semaphore* s = semaphores[semaphoreID];
    s->semaphoreVal++;
    if (s->semaphoreVal <= 0) {
        
        pcb* unblockedProcess = List_trim(s->processList);
        if (unblockedProcess != NULL) {
            unblockedProcess->processState = READY;
            List_prepend(readyQueue[unblockedProcess->currentPriority], unblockedProcess);
            printf("Process %d was placed on the ready queue. \n", unblockedProcess->pid);
        }
        else {
            printf("No processes were unblocked from semaphore %d \n", semaphoreID);
        }
    }
    else {
        printf("No processes were unblocked from semaphore %d \n", semaphoreID);
    }
    printf("V Successful. Semaphore %d value has incremented to %d", semaphoreID, s->semaphoreVal);
}

/** ------------------------------------------- Procinfo ------------------------------------------ **/

void dumpInfo(pcb* process) {
    printf("---------Process %d---------- \n", process->pid);
    switch (process->currentPriority) {
        case -1:
            printf("init \n");
            break;
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

    if (pid == 0) {
        dumpInfo(init);
        return;
    }

    if (runningProcess->pid == pid) {
        dumpInfo(runningProcess);
        return;
    }

    // scan through each queue to see if the process exists
    for (int i = 0; i < 3; i++) {
        List_first(readyQueue[i]);
        pcb* p = List_search(readyQueue[i], equalsComparator, &pid);
        if (p != NULL) {
            dumpInfo(p);
            return;
        }
    }

    // scan through receive and send
    List_first(sendList);
    pcb* p = List_search(sendList, equalsComparator, &pid);
    if (p != NULL) {
        dumpInfo(p);
        return;
    }

    List_first(receiveList);
    p = List_search(receiveList, equalsComparator, &pid);
    if (p != NULL) {
        dumpInfo(p);
        return;
    }

    // scan through semaphores
    for (int i = 0; i < 5; i++) {
        semaphore* currSem = semaphores[i];
        List* pList = currSem->processList;
        List_first(pList);
        pcb* p = List_search(pList, equalsComparator, &pid);
        if (p != NULL) {
            dumpInfo(p);
            return;
        }
    }

    // its over
    printf("No processes with pid %d found", pid);

}

/** ------------------------------------------- Total Info ------------------------------------------ **/
void displayQueues() {
    // display all ready queues
    printf("~~~~~~~~~~~~~~~~~~~~ READY QUEUES: ~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    for (int i = 0; i < 3; i++) {
        List* currList = readyQueue[i];
        pcb* p = List_last(currList);
        while (p != NULL) {
            dumpInfo(p);
            p = List_prev(currList);
        }
    }

    // display all send
    printf("~~~~~~~~~~~~~~~~~~~~ SEND QUEUES: ~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    pcb* p = List_last(sendList);

    while (p != NULL) {
        dumpInfo(p);
        p = List_prev(sendList);
    }

    // display all receive
    printf("~~~~~~~~~~~~~~~~~~~~ RECEIVE QUEUES: ~~~~~~~~~~~~~~~~~~~~~~~~\n");
    p = List_last(receiveList);
    while (p != NULL) {
        dumpInfo(p);
        p = List_prev(receiveList);;
    }


    printf("~~~~~~~~~~~~~~~~~~~~ SEMAPHORE QUEUES: ~~~~~~~~~~~~~~~~~~~~~~\n");
    // display all semaphore queues
    for (int i = 0; i < 5; i++) {
        semaphore* currSem = semaphores[i];
        List* currList = currSem->processList;
        
        pcb* p = List_last(currList);
        while (p != NULL) {
            dumpInfo(p);
            p = List_prev(currList);
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
            forkCommandInterface();
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
            receive();
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
        semaphores[i]->semaphoreVal = 0;
    }
}


void freeLists() {
    for (int i = 0; i < 3; i++) {
        List_free(readyQueue[i], free);
    }

    List_free(sendList, free);
    List_free(receiveList, free);

    for (int i = 0; i < 5; i++) {
        List_free(semaphores[i]->processList, free);
        free(semaphores[i]);
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

    // free stuff
    freeLists();
    printf("skinOS simulation terminated :D\n");

}