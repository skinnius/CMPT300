#include <stdio.h>
#include <assert.h>
#include "list.h"

static List availableLists[LIST_MAX_NUM_HEADS];    // global statically allocated list of possible unique Lists --> Array of List pointers
static Node availableNodes[LIST_MAX_NUM_NODES];         // global statically allocated list of nodes.
static int numList = 0;
static int numNodes = 0;
static bool initialSetup = true;
static Node* nodeHead;
static Node* nodeTail;
static List* listHead;
static List* listTail;

static Node* useNode(){
    Node* newNode = nodeHead;
    nodeHead = nodeHead->next;
    return newNode;
}

static void incrementListSize(List* pList){
    numNodes++;
    pList->listSize++;
}

static void decreaseListSize(List* pList){
    numNodes--;
    pList->listSize--;
}

static void setNewCurrent(List* pList, Node* newCurrentNode){
    pList->currNode = newCurrentNode;
}

static void insertBeforeList(List* pList, Node* newNode, void* pItem){
    newNode->next = pList->head;
    pList->head->prev = newNode;
    newNode->prev = NULL;
    newNode->item = pItem;
    pList->head = newNode;

}

static void insertAfterList(List* pList, Node* newNode, void* pItem){
    newNode->next = NULL;
    newNode->prev = pList->tail;
    pList->tail->next = newNode;
    newNode->item = pItem;
    pList->tail = newNode;
}

static void insertIntoEmptyList(List* pList, Node* newNode, void* pItem){
    newNode->item = pItem;
    newNode->next = NULL;
    newNode->prev = NULL;
    pList->tail = newNode;
    pList->head = newNode;
}

static void popHead(List* pList){
    pList->head = pList->head->next;
    pList->head->prev = NULL;
}

static void popTail(List* pList){
    pList->tail = pList->tail->prev;
    pList->tail->next = NULL;
}

static void freeNode(Node* node){                   // questionable function lmao DELETE THIS COMMENT BEFORE SUBMITTING 
    nodeTail->next = node;
    node->prev = nodeTail;
    node->next = NULL;
    nodeTail = node;
    node->item = NULL;
}

static void freeList(List* pList){
    listTail->next = pList;
    pList->next = NULL;
    listTail = pList;

    pList->currStatus = LIST_OOB_START;            // try extracting this into a new function
    pList->currNode = NULL;
    pList->head = NULL;
    pList->tail = NULL;
    pList->listSize = 0;

    numList--;
    
}
// Makes a new, empty list, and returns its reference on success. 
// Returns a NULL pointer on failure.
List* List_create(){
    List* newList = NULL;      // returns NULL if past LIST_MAX_NUM_HEADS

    // initial setup of the data structure (first call of List_create())
    if (initialSetup){
        for (int i = 0; i < LIST_MAX_NUM_HEADS; i++){
            availableLists[i].head = NULL;               // initialize everything to NULL  
            availableLists[i].tail = NULL;
            availableLists[i].currNode = NULL;
            availableLists[i].currStatus = LIST_OOB_START;
            availableLists[i].listSize = 0;
            availableLists[i].next = NULL;

            if (i != LIST_MAX_NUM_HEADS - 1){
                availableLists[i].next = &availableLists[i + 1];
            }
        }

        listHead = &availableLists[0];
        listTail = &availableLists[LIST_MAX_NUM_HEADS - 1];

        for (int i = 0; i < LIST_MAX_NUM_NODES - 1; i++){
            availableNodes[i].item = NULL;
            availableNodes[i].next = NULL;
            availableNodes[i].prev = NULL;

            if (i != 0){
                availableNodes[i].prev = &availableNodes[i - 1];
            }
            if (i != LIST_MAX_NUM_NODES - 1){
                availableNodes[i].next = &availableNodes[i + 1];
            }
            nodeHead = &availableNodes[0];
            nodeTail = &availableNodes[LIST_MAX_NUM_NODES - 1];
        }
        initialSetup = false;
    }

    if (numList < LIST_MAX_NUM_HEADS){
        newList = listHead;
        listHead = listHead->next;          // give the new list the memory position of the next available "list position"
        numList++;
    }
    
    return newList;
}

int List_count(List* pList){               // returns number of elements in the list, O(1).
    assert(pList != NULL);
    return pList->listSize;
}

// Returns a pointer to the first item in pList and makes the first item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_first(List* pList){
    assert(pList != NULL);
    if (pList->listSize != 0){
        setNewCurrent(pList, pList->head);
        return pList->currNode->item;
    }
    pList->currNode = NULL; 
    pList->currStatus = LIST_OOB_START;                                                                                 // DOUBLE CHECK IF THIS IS CORRECT
    return NULL;
}

// Returns a pointer to the last item in pList and makes the last item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_last(List* pList){
    assert(pList != NULL);
    if (pList->listSize != 0){
        setNewCurrent(pList, pList->tail);
        return pList->currNode->item;
    }
    pList->currNode = NULL;
    pList->currStatus = LIST_OOB_START;
    return NULL;
}

// Advances pList's current item by one, and returns a pointer to the new current item.
// If this operation advances the current item beyond the end of the pList, a NULL pointer 
// is returned and the current item is set to be beyond end of pList.
void* List_next(List* pList){
    assert(pList != NULL);

    if (pList->currStatus == LIST_OOB_START){          // Case 1: current pointer OOB
        if (List_count(pList) == 0){                   // Case 1.1: current pointer OOB for empty list
            pList->currStatus = LIST_OOB_END;
            return NULL;
        }

        setNewCurrent(pList, pList->head);
        return pList->currNode->item;
    }

    else if (pList->currNode != NULL && pList->currNode->next != NULL){       // General Case
        setNewCurrent(pList, pList->currNode->next);
        return pList->currNode->item;
    }

    pList->currStatus = LIST_OOB_END;                  // next node going OOB
    pList->currNode = NULL;
    return NULL;
}

// Backs up pList's current item by one, and returns a pointer to the new current item. 
// If this operation backs up the current item beyond the start of the pList, a NULL pointer 
// is returned and the current item is set to be before the start of pList.
void* List_prev(List* pList){
    assert(pList != NULL);

    if (pList->currStatus == LIST_OOB_END){
        if (List_count(pList) == 0){
            pList->currStatus = LIST_OOB_START;
            return NULL;
        }
        setNewCurrent(pList, pList->tail);
        return pList->currNode->item;
    }

    else if (pList->currNode != NULL && pList->currNode->prev != NULL){
        setNewCurrent(pList, pList->currNode->prev);
        return pList->currNode->item;
    }

    pList->currStatus = LIST_OOB_START;               
    pList->currNode = NULL;
    return NULL;
}


// Returns a pointer to the current item in pList.
void* List_curr(List* pList){           
    assert(pList != NULL);  
    return pList->currNode->item;
}

// Adds the new item to pList directly after the current item, and makes item the current item. 
// If the current pointer is before the start of the pList, the item is added at the start. If 
// the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_insert_after(List* pList, void* pItem){
    assert(pList != NULL);

    if (numNodes >= LIST_MAX_NUM_NODES){
        return LIST_FAIL;
    }

    Node* newNode = useNode();

    if (List_count(pList) == 0){
        insertIntoEmptyList(pList, newNode, pItem);
    }

    else if (pList->currNode == NULL && pList->currStatus == LIST_OOB_START){          // case 1: current pointer is before the list start
        insertBeforeList(pList, newNode, pItem);
    }

    else if (pList->currStatus == LIST_OOB_END || pList->currNode == pList->tail){            // case 2: current pointer is after the list end or current pointer is on tail
        insertAfterList(pList, newNode, pItem);
    }   

    else{                                                // general case
        Node* temp = pList->currNode;
        newNode->next = temp->next;
        newNode->prev = temp;
        temp->next->prev = newNode;
        temp->next = newNode;
        newNode->item = pItem;
    }
    incrementListSize(pList);
    setNewCurrent(pList, newNode);
    return LIST_SUCCESS;
}

// Adds item to pList directly before the current item, and makes the new item the current one. 
// If the current pointer is before the start of the pList, the item is added at the start. 
// If the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_insert_before(List* pList, void* pItem){
    assert(pList != NULL);

    if (numNodes >= LIST_MAX_NUM_NODES){
        return LIST_FAIL;
    }

    Node* newNode = useNode();

    if (List_count(pList) == 0){
        insertIntoEmptyList(pList, newNode, pItem);
    }

    else if (pList->currNode == NULL && pList->currStatus == LIST_OOB_END){          // case 1: current pointer is after the list start
        insertAfterList(pList, newNode, pItem);
    }

    else if (pList->currStatus == LIST_OOB_START || pList->currNode == pList->head){            // case 2: current pointer is after the list end
        insertBeforeList(pList, newNode, pItem);
    }   

    // general case
    else{
        Node* temp = pList->currNode;
        newNode->next = temp;
        newNode->prev = temp->prev;
        temp->prev->next = newNode;
        temp->prev = newNode;
        newNode->item = pItem;
    }
    incrementListSize(pList);
    setNewCurrent(pList, newNode);    
    return LIST_SUCCESS;
}

// Adds item to the end of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_append(List* pList, void* pItem){
    assert(pList != NULL);

    if (numNodes >= LIST_MAX_NUM_NODES){
        return LIST_FAIL;
    }
    
    Node* newNode = useNode();
    if (List_count(pList) == 0){                    // check for empty pList
        insertIntoEmptyList(pList, newNode, pItem);
    }
    else{                                           // general case
        insertAfterList(pList, newNode, pItem); 
    }
    incrementListSize(pList);
    setNewCurrent(pList, newNode);
    return LIST_SUCCESS;
}

// Adds item to the front of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_prepend(List* pList, void* pItem){
    assert(pList != NULL);
    
    if (numNodes >= LIST_MAX_NUM_NODES){
        return LIST_FAIL;
    }

    Node* newNode = useNode();
    if (List_count(pList) == 0){
        insertIntoEmptyList(pList, newNode, pItem);
    }
    else{
        insertBeforeList(pList, newNode, pItem);
    }
    incrementListSize(pList);
    setNewCurrent(pList, newNode);
    return LIST_SUCCESS;
}


// Return current item and take it out of pList. Make the next item the current one.
// If the current pointer is before the start of the pList, or beyond the end of the pList,
// then do not change the pList and return NULL.
void* List_remove(List* pList){
    assert(pList != NULL);
    void* currItem = pList->currNode->item;
    Node* nodeToBeFreed = pList->currNode;

    if (pList->currStatus == LIST_OOB_START || pList->currStatus == LIST_OOB_END){            // first check for OOB (covers empty list check)
        return NULL;
    }
    
    if (List_count(pList) == 1){                // if only one element in list
        pList->head = NULL;
        pList->tail = NULL;
        pList->currStatus = LIST_OOB_END;
        pList->currNode = NULL;
    }

    else if (pList->currNode == pList->head){                // case 1: removing head node
        popHead(pList);
        setNewCurrent(pList, pList->head);
    }

    else if (pList->currNode == pList->tail){         // case 2: removing tail node
        popTail(pList);
        pList->currNode = NULL;                         
        pList->currStatus = LIST_OOB_END;
    }

    else{                                            // general case
        Node* prevNode = pList->currNode->prev;
        Node* nextNode = pList->currNode->next;
        prevNode->next = nextNode;
        nextNode->prev = prevNode;

        setNewCurrent(pList, nextNode);
    }

    // making the nodes "free"
    freeNode(nodeToBeFreed);
    decreaseListSize(pList);
    return currItem;
}


// Return last item and take it out of pList. Make the new last item the current one.
// Return NULL if pList is initially empty.
void* List_trim(List* pList){
    assert(pList != NULL);
    if (List_count(pList) == 0){
        return NULL;
    }

    Node* nodeToBeFreed = pList->tail;
    void* currItem = nodeToBeFreed->item;
    popTail(pList);

    setNewCurrent(pList, pList->tail);
    freeNode(nodeToBeFreed);
    decreaseListSize(pList);
    
    return currItem;
}


// Adds pList2 to the end of pList1. The current pointer is set to the current pointer of pList1. 
// pList2 no longer exists after the operation; its head is available
// for future operations.
void List_concat(List* pList1, List* pList2){           // this feels a little sketchy, might be source of problem in future tests
    assert(pList1 != NULL);
    assert(pList2 != NULL);
    assert(pList1 != pList2);

    // case 1: empty pList1
    if (List_count(pList1) == 0){
        pList1->head = pList2->head;
        pList1->tail = pList2->tail;
        pList1->listSize = pList2->listSize;

        // throw pList2 back into the open list pool
        freeList(pList2);
    }

    // case 2: empty pList2
    else if (List_count(pList2) == 0){
        // throw pList2 back into the open list pool
        freeList(pList2);
    }
    // general case
    else{
        pList1->tail->next = pList2->head;
        pList2->head->prev = pList1->tail;
        pList1->tail = pList2->tail;
        pList1->listSize += pList2->listSize;

        freeList(pList2);
    }
}


// Delete pList. pItemFreeFn is a pointer to a routine that frees an item. 
// It should be invoked (within List_free) as: (*pItemFreeFn)(itemToBeFreedFromNode);
// pList and all its nodes no longer exists after the operation; its head and nodes are 
// available for future operations.
// typedef void (*FREE_FN)(void* pItem);

void List_free(List* pList, FREE_FN pItemFreeFn){
    assert(pList != NULL);

    while (pList->currNode != NULL){
        (*pItemFreeFn)(pList->currNode->item);
        pList->currNode = pList->currNode->next;
    }

    pList->head = NULL;
    pList->tail = NULL;
    pList->listSize = 0;
    pList->currNode = NULL;
    pList->currStatus = LIST_OOB_START;
}




// Search pList, starting at the current item, until the end is reached or a match is found. 
// In this context, a match is determined by the comparator parameter. This parameter is a
// pointer to a routine that takes as its first argument an item pointer, and as its second 
// argument pComparisonArg. Comparator returns 0 if the item and comparisonArg don't match, 
// or 1 if they do. Exactly what constitutes a match is up to the implementor of comparator. 
// 
// If a match is found, the current pointer is left at the matched item and the pointer to 
// that item is returned. If no match is found, the current pointer is left beyond the end of 
// the list and a NULL pointer is returned.
// 
// If the current pointer is before the start of the pList, then start searching from
// the first node in the list (if any).

void* List_search(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg){

    if (pList->currStatus == LIST_OOB_START){                          // if current pointer before the start of the list
        pList->currNode = pList->head;
    }

    while (pList->currNode != NULL){
        int match = (*pComparator)(pList->currNode->item, pComparisonArg);
        
        if (match){
            return pList->currNode->item;
        }
    }
    *(int*)pList->currStatus = LIST_OOB_END;
    return NULL;

}










