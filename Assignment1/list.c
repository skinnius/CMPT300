#include <stdio.h>
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
    pList->current = newCurrentNode->item;
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
            availableLists[i].current = LIST_OOB_START;
            availableLists[i].listSize = 0;
            availableLists[i].next = NULL;
            availableLists[i].prev = NULL;

            if (i != 0){
                availableLists[i].prev = &availableLists[i - 1];
            }
            if (i != LIST_MAX_NUM_HEADS - 1){
                availableLists[i].next = &availableLists[i + 1];
            }
            listHead = &availableLists[0];
            listTail = &availableLists[LIST_MAX_NUM_HEADS - 1];
        }

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
        listHead = listHead->next;          // give the new list the memory position of the next available "list position"
        newList = listHead;
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
        return pList->current;
    }
    pList->current = NULL;                                                                                  // DOUBLE CHECK IF THIS IS CORRECT
    return NULL;
}

// Returns a pointer to the last item in pList and makes the last item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_last(List* pList){
    assert(pList != NULL);
    if (pList->listSize != 0){
        setNewCurrent(pList, pList->tail);
        return pList->current;
    }
    pList->current = NULL;
    return NULL;
}

// Advances pList's current item by one, and returns a pointer to the new current item.
// If this operation advances the current item beyond the end of the pList, a NULL pointer 
// is returned and the current item is set to be beyond end of pList.
void* List_next(List* pList){
    assert(pList != NULL);

    if (pList->current == LIST_OOB_START){          // Case 1: current pointer OOB
        setNewCurrent(pList, pList->head);
        return pList->current;
    }

    else if (pList->currNode != NULL && pList->currNode->next != NULL){       // General Case
        setNewCurrent(pList, pList->currNode->next);
        return pList->current;
    }

    pList->current = LIST_OOB_END;                  // next node going OOB
    pList->currNode = NULL;
    return NULL;
}

// Backs up pList's current item by one, and returns a pointer to the new current item. 
// If this operation backs up the current item beyond the start of the pList, a NULL pointer 
// is returned and the current item is set to be before the start of pList.
void* List_prev(List* pList){
    assert(pList != NULL);

    
    if (pList->current == LIST_OOB_END){
        setNewCurrent(pList, pList->tail);
        return pList->current;
    }

    else if (pList->currNode != NULL && pList->currNode->prev != NULL){
        setNewCurrent(pList, pList->currNode->prev);
        return pList->current;
    }

    pList->current = LIST_OOB_START;                // consider setting currNode to NULL; however, might cause further problems
    pList->currNode = NULL;
    return NULL;
}


// Returns a pointer to the current item in pList.
void* List_curr(List* pList){           
    assert(pList != NULL);  
    return pList->current;
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

    if (pList->current == LIST_OOB_START){          // case 1: current pointer is before the list start
        insertBeforeList(pList, newNode, pItem);
    }

    else if (pList->current == LIST_OOB_END || pList->currNode == pList->tail){            // case 2: current pointer is after the list end or current pointer is on tail
        insertAfterList(pList, newNode, pItem);
    }   

    else{                                                // general case
        Node* temp = pList->currNode;
        newNode->next = temp->next;
        newNode->prev = temp;
        temp->next->prev = newNode;
        temp->next = newNode;
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

    if (pList->current == LIST_OOB_START || pList->current == pList->head){          // case 1: current pointer is before the list start or is at the head
        insertBeforeList(pList, newNode, pItem);
    }

    else if (pList->current == LIST_OOB_END){            // case 2: current pointer is after the list end
        insertAfterList(pList, newNode, pItem);
    }   

    // general case
    else{
        Node* temp = pList->currNode;
        newNode->next = temp;
        newNode->prev = temp->prev;
        temp->prev->next = newNode;
        temp->prev = newNode;
    }
    incrementListSize(pList);
    setNewCurrent(pList, newNode);    
    return LIST_SUCCESS;
}

// WHAT IF PLIST EMPTY????
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
    void* currVal = pList->current;

    if (pList->current == LIST_OOB_START || pList->current == LIST_OOB_END){            // first check for OOB
        return NULL;
    }
    
    if (pList->currNode == pList->head){                // case 1: removing head node
        Node* temp = pList->head;
        currVal = temp->item;

        // setting head to next node
        pList->head = temp->next;
        pList->head->prev = NULL;
        pList->currNode = pList->head;
        pList->current = pList->head->item;
        
    }

    else if (pList->currNode == pList->tail){         // case 2: removing tail node
        Node* temp = pList->tail;
        currVal = temp->item;

        // setting tail to the previous node
        pList->tail = temp->prev;
        pList->tail->next = NULL;
        pList->currNode = NULL;
        pList->current = LIST_OOB_END;

    }

    else{                    // general case

    }










}












