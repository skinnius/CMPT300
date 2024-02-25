#include "list.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>


//FREE_FN
void freeItem(void* pItem){
    pItem = NULL;
}


// COMPARATOR_FN
bool equals(void* item1, void* item2) {
    if ((int)item1 == (int)item2){
        return 1;
    } 
    return 0;
}

void printListInOrder(List* pList, Node* curr){
    if (List_count(pList) == 0){
        printf("empty list %d\n", (int)pList);
        return;
    }

    while (curr != NULL){
        printf("%d\n", (int)curr->item);
        curr = curr->next;
    }
    printf("Length of list: %d\n", List_count(pList));
}

void printListReverse(List* pList, Node* curr){
    if (List_count(pList) == 0){
        printf("empty list %d\n", (int)pList);
        return;
    }

    while (curr != NULL){
        printf("%d\n", (int)curr->item);
        curr = curr->prev;
    }
    printf("Length of list: %d\n", List_count(pList));
}

void checkEmptyListStart(List* pList){
    assert(pList->currNode == NULL);
    assert(pList->currStatus == LIST_OOB_START);
    assert(pList->tail == NULL);
    assert(pList->head == NULL);
    assert(pList->listSize == 0);
}
void checkEmptyListEnd(List* pList){
    assert(pList->currNode == NULL);
    assert(pList->currStatus == LIST_OOB_END);
    assert(pList->tail == NULL);
    assert(pList->head == NULL);
    assert(pList->listSize == 0);
}


int main(void){
    printf("STARTING TESTS...\n");
    printf("Testing for List creation... \n");
    List* pLists[LIST_MAX_NUM_HEADS];
    // Test suite 1: list creation
    for (int i = 0; i < LIST_MAX_NUM_HEADS - 1; i++){
        List* pList = List_create();
        assert(pList != NULL);
        checkEmptyListStart(pList);
        pLists[i] = pList;
    }
    List* goodList = List_create();
    assert(goodList->next == NULL);
    List* badList = List_create();
    assert(goodList != NULL);
    assert(badList == NULL);
    printf("List creation tests passed!\n");

    List_free(goodList, freeItem);

    for (int i = 0; i < LIST_MAX_NUM_HEADS - 1; i++){
        List_free(pLists[i], freeItem);
    }

    printf("Testing for Node Insertion...\n");
    
    List* pList = List_create();

    printf("Testing for insert_after()...\n");

    for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
        assert(List_insert_after(pList, (void*)i) == 0);
        
        if (List_count(pList) == 1){
            assert(pList->head = pList->currNode);
            assert(pList->currStatus == LIST_OOB_START);
        }
    }
    assert(List_insert_after(pList, (void*)100) == -1);
    printListInOrder(pList, pList->head);

    List_free(pList, freeItem);
    pList = List_create();

    assert(List_insert_after(pList, (void*)100) == 0);
    assert(List_trim(pList) == (void*)100);
    checkEmptyListEnd(pList);
    
    List_free(pList, freeItem);
    pList = List_create();

    for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
        assert(List_insert_after(pList, "d") == 0);
        assert(pList->listSize == i + 1);

        assert(pList->currNode != NULL);
        assert(pList->currStatus == LIST_OOB_START);
        assert(pList->currNode == pList->head);

        pList->currNode = NULL;
        pList->currStatus = LIST_OOB_START;
    }
    printf("passed!\n");
    
    List_free(pList, freeItem);
    pList = List_create();
    checkEmptyListStart(pList);

    for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
        assert(List_insert_before(pList, (void*)i) == 0);
        assert(pList->currStatus == LIST_OOB_START);

        if (List_count(pList) == 1){
            assert(pList->head = pList->currNode);
            assert(pList->currStatus == LIST_OOB_START);
            assert(pList->tail->next == NULL);
            assert(pList->tail == pList->currNode);
            assert(pList->tail->next == NULL);
        }
        assert(pList->head == pList->currNode);
        assert(pList->currStatus == LIST_OOB_START);
        
        assert(pList->tail->next == NULL);
    }

    
    assert(List_insert_after(pList, "e") == -1);
    assert(List_insert_before(pList, "e") == -1);
    printListInOrder(pList, pList->head);       

    List_free(pList, freeItem);
    pList = List_create();
    assert(List_insert_after(pList, "c") == 0);
    assert(List_trim(pList) == "c");
    checkEmptyListEnd(pList);
    
    pList->currStatus = LIST_OOB_START;

    for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
        assert(List_insert_after(pList, (void*)i) == 0);
        assert(pList->listSize == i + 1);

        assert(pList->currNode != NULL);
        assert(pList->currStatus == LIST_OOB_START);
        assert(pList->currNode == pList->head);

        pList->currNode = NULL;
        pList->currStatus = LIST_OOB_START;
    }
    printListInOrder(pList, pList->head->next);

    printf("more tests...\n");
    // Test case 4: Inserting at the end of the list. (OOB)
    List_free(pList, freeItem);
    pList = List_create();
    for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
        assert(List_insert_after(pList, (void*)i) == 0);
        assert(pList->currNode != NULL);
        if (i > 0){
            assert(pList->currStatus == LIST_OOB_END);
        }
        assert(pList->currNode == pList->tail);

        pList->currNode = NULL;
        pList->currStatus = LIST_OOB_END;
    }
    assert(List_insert_before(pList, "x") == -1);
    printListInOrder(pList, pList->head);

    List_free(pList, freeItem);
    pList = List_create();

    // insert end (tail)
    for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
        assert(List_insert_after(pList, (void*)i) == 0);

        assert(pList->currNode != NULL);
        assert(pList->currStatus == LIST_OOB_START);
        assert(pList->currNode == pList->tail);

        pList->currNode = pList->tail;
    }
    printListReverse(pList, pList->tail);


    List_free(pList, freeItem);
    pList = List_create();

    // Test case 5: Inserting at the middle of the list. (General case)
    for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
        assert(List_insert_after(pList, (void*)i) == 0);
        
        assert(pList->currNode != NULL);
        assert(pList->currStatus == LIST_OOB_START);
        assert(pList->listSize == i + 1);

        if (i <= 1){
            assert(pList->currNode == pList->tail);
        }

        if (i > 2){
            assert(pList->currNode == pList->tail->prev);
        }
        if (i > 1){
            pList->currNode = pList->tail->prev;
        }
    }
    
    printListInOrder(pList, pList->head);
    printf("passed!\n");

    printf("Testing for insert_before()...\n");
    List_free(pList, freeItem);
    pList = List_create();
    // Test case 3: Inserting at the end of the list.
    for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
        pList->currNode = NULL;
        pList->currStatus = LIST_OOB_END;

        assert(List_insert_before(pList, (void*)i) == 0);
        assert(pList->listSize == i + 1);

        assert(pList->currNode != NULL);
        assert(pList->currStatus == LIST_OOB_END);
        assert(pList->currNode == pList->tail);
    }
    printListInOrder(pList, pList->head);

    // Test case 4: Inserting at the start of the list. (OOB)
    List_free(pList, freeItem);
    pList = List_create();
    for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
        assert(List_insert_before(pList, (void*)i) == 0);

        assert(pList->currNode != NULL);
        if (i > 0){
            assert(pList->currStatus == LIST_OOB_START);
        }
        assert(pList->currNode == pList->head);

        pList->currNode = NULL;
        pList->currStatus = LIST_OOB_START;
    }
    printListReverse(pList, pList->tail);

    List_free(pList, freeItem);
    pList = List_create();
    // Test case 4.1: Inserting at the end of the list. (Head)
    for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
        assert(List_insert_before(pList, (void*)i) == 0);

        assert(pList->currNode != NULL);
        assert(pList->currStatus == LIST_OOB_START);
        assert(pList->currNode == pList->head);

        pList->currNode = pList->head;
    }
    printListInOrder(pList, pList->head);

    List_free(pList, freeItem);
    pList = List_create();
    // Test case 5: Inserting at the middle of the list. (General case)
    for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
        assert(List_insert_before(pList, (void*)i) == 0);
        
        assert(pList->currNode != NULL);
        assert(pList->currStatus == LIST_OOB_START);
        assert(pList->listSize == i + 1);

        if (i <= 1){
            assert(pList->currNode == pList->head);
        }

        if (i > 2){
            assert(pList->currNode == pList->head->next);
        }
        if (i > 1){
            pList->currNode = pList->head->next;
        }
    }
    printListInOrder(pList, pList->head);
    

    printf("Testing List_remove()...");
    List_free(pList, freeItem);
    pList = List_create();
    int a[5] = {1,2,3,4,5};
    assert(List_insert_after(pList, (void*)a[0]) == 0);
    assert(List_insert_after(pList, (void*)a[1]) == 0);
    assert(List_insert_after(pList, (void*)a[2]) == 0);
    assert(List_insert_after(pList, (void*)a[3]) == 0);
    assert(List_insert_after(pList, (void*)a[4]) == 0);

    assert(List_count(pList) == 5);
    assert(pList->currNode == pList->tail);
    assert(pList->currStatus == LIST_OOB_START);
    printListInOrder(pList, pList->head);
    
    for (int i = 0; i < 5; i++){
        printf("removed item %d: %d\n", i+1, (int)pList->currNode->item);
        assert(List_remove(pList) == (void*)a[4 - i]);
        assert(pList->currStatus == LIST_OOB_END);
        List_prev(pList);
    }
    assert(List_count(pList) == 0);
    pList->currNode = pList->tail;

    List_free(pList, freeItem);
    pList = List_create();
    assert(List_insert_after(pList, (void*)a[0]) == 0);
    assert(List_insert_after(pList, (void*)a[1]) == 0);
    assert(List_insert_after(pList, (void*)a[2]) == 0);
    assert(List_insert_after(pList, (void*)a[3]) == 0);
    assert(List_insert_after(pList, (void*)a[4]) == 0);
    
    pList->currStatus = LIST_OOB_START;
    pList->currNode = pList->head;
    for (int i = 0; i < 5; i++){
        assert(pList->currStatus == LIST_OOB_START);
        printf("removed item %d: %d\n", i+1, (int)pList->currNode->item);
        assert(List_remove(pList) == (void*)a[i]);

    }

    assert(pList->currNode == NULL);
    assert(pList->tail == NULL);
    assert(pList->head == NULL);
    assert(pList->currStatus == LIST_OOB_END);

    List_free(pList, freeItem);
    pList = List_create();
    
    assert(List_append(pList, (void*)a[0] ) == 0);
    assert(List_append(pList, (void*)a[1]) == 0);
    assert(List_append(pList, (void*)a[2]) == 0);
    assert(List_append(pList, (void*)a[3]) == 0);
    assert(List_append(pList, (void*)a[4]) == 0);

    assert(pList->currNode == pList->tail);
    assert(pList->currStatus == LIST_OOB_START);
    assert(pList->head->item == (void*)a[0]);
    assert(pList->tail->item == (void*)a[4]);

    for (int i = 0; i < 5; i++){
        printf("removed item %d: %d\n", i+1, (int)pList->currNode->item);
        assert(List_remove(pList) == (void*)a[4-i]);
        assert(pList->currStatus == LIST_OOB_END);
        List_prev(pList);
    }

    List_free(pList, freeItem);
    pList = List_create();

    assert(List_prepend(pList, (void*)a[0]) == 0);
    assert(List_prepend(pList, (void*)a[1]) == 0);
    assert(List_prepend(pList, (void*)a[2]) == 0);
    assert(List_prepend(pList, (void*)a[3]) == 0);
    assert(List_prepend(pList, (void*)a[4]) == 0);

    assert(pList->currNode == pList->head);
    assert(pList->currStatus == LIST_OOB_START);

    printf("All insert tests passed!\n");

    assert(pList->head->item == (void*)a[4]);
    assert(pList->tail->item == (void*)a[0]);


    for (int i = 0; i < 5; i++){
        assert(pList->currStatus == LIST_OOB_START);
        printf("removed item %d: %d\n", i+1, (int)pList->currNode->item);
        assert(List_remove(pList) == (void*)a[4-i]);

    }
    
    assert(pList->currNode == NULL);
    assert(pList->currStatus == LIST_OOB_END);
    printf("All insert test cases passed!\n");

    List_free(pList, freeItem);
    pList = List_create();

    printf("Testing for empty case...\n");

    assert(pList->listSize == 0);
    assert(List_first(pList) == NULL);
    assert(pList->currNode == NULL);
    assert(pList->currStatus == LIST_OOB_START);

    assert(List_last(pList) == NULL);
    assert(pList->currNode == NULL);
    assert(pList->currStatus == LIST_OOB_START);

    assert(List_append(pList, (void*)a[0]) == 0);
    assert(List_append(pList, (void*)a[1]) == 0);
    assert(List_append(pList, (void*)a[2]) == 0);
    assert(List_append(pList, (void*)a[3]) == 0);
    assert(List_append(pList, (void*)a[4]) == 0);

    printf("Testing for general case...\n");
    assert(List_first(pList) == pList->head->item);
    assert(pList->currNode == pList->head);
    assert(List_last(pList) == pList->tail->item);
    assert(pList->currNode == pList->tail);
    assert(pList->currStatus == LIST_OOB_START);

    List_first(pList);
    for (int i = 0; i < 5; i++){
        assert(pList->currNode->item == (void*)a[i]);
        List_next(pList);
    }
    assert(pList->currStatus == LIST_OOB_END);

    List_last(pList);
    assert(pList->currNode == pList->tail);

    for(int i = 0; i < 5; i++){
        assert(pList->currNode->item == (void*)a[4-i]);
        List_prev(pList);
    }

    assert(pList->currStatus == LIST_OOB_START);

    printf("all next()/prev() test cases passed! \n\n");

    List_free(pList, freeItem);
    List* pList1 = List_create();
    List* pList2 = List_create();

    printf("Testing concat()...\n");
    int b[5] = {1,2,3,4,5};
    
    List_concat(pList1, pList2);
    pList2 = List_create();
    assert(pList2->currNode == NULL);
    assert(pList2->currStatus == LIST_OOB_START);
    assert(pList2->listSize == 0);
    assert(pList2->head == NULL);
    assert(pList2->tail == NULL);

    assert(pList1->currNode == NULL);
    assert(pList1->currStatus == LIST_OOB_START);
    assert(pList1->listSize == 0);
    assert(pList1->head == NULL);
    assert(pList1->tail == NULL);
    
    assert(List_append(pList1, (void*)a[0]) == 0);
    assert(List_append(pList1, (void*)a[1]) == 0);
    assert(List_append(pList1, (void*)a[2]) == 0);
    assert(List_append(pList1, (void*)a[3]) == 0);
    assert(List_append(pList1, (void*)a[4]) == 0);

    List_concat(pList1, pList2);
    pList2 = List_create();
    assert(pList2->currNode == NULL);
    assert(pList2->currStatus == LIST_OOB_START);
    assert(pList2->listSize == 0);
    assert(pList2->head == NULL);
    assert(pList2->tail == NULL);

    assert(pList1->head->item == (void*)a[0]);
    assert(pList1->tail->item == (void*)a[4]);
    assert(pList1->currNode->item == (void*)a[4]);
    assert(pList1->currStatus == LIST_OOB_START);
    assert(pList1->listSize == 5);

    assert(List_append(pList2, (void*)b[0]) == 0);
    assert(List_append(pList2, (void*)b[1]) == 0);
    assert(List_append(pList2, (void*)b[2]) == 0);
    assert(List_append(pList2, (void*)b[3]) == 0);
    assert(List_append(pList2, (void*)b[4]) == 0);

    List_concat(pList1, pList2);
    assert(pList1->head->item == (void*)a[0]);
    assert(pList1->tail->item == (void*)b[4]);
    assert(pList1->currNode->item == (void*)a[4]);
    assert(pList1->currStatus == LIST_OOB_START);
    assert(pList1->listSize == 10);

    assert(pList2->currNode == NULL);
    assert(pList2->currStatus == LIST_OOB_START);
    assert(pList2->listSize == 0);
    assert(pList2->head == NULL);
    assert(pList2->tail == NULL);

    List_free(pList1, freeItem);
    pList = List_create();

    assert(List_trim(pList) == NULL);

    for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
        assert(List_append(pList, (void*)i) == 0);
    }
    assert(List_append(pList, "a") == -1);

    List_remove(pList);
    assert(pList->currNode == NULL);
    assert(pList->currStatus == LIST_OOB_END);
    List_last(pList);
    List_remove(pList);
    

    assert(List_prepend(pList, (void*)1) == 0);
    assert(List_insert_after(pList, (void*)2) == 0);
    assert(pList->currNode->item == (void*)2);
    assert(pList->currNode->prev->item == (void*)1);
    assert(List_append(pList, (void*)3) == -1);
    printListInOrder(pList, pList->head);

    assert(pList->tail->item == (void*)(LIST_MAX_NUM_NODES-3));
    assert(List_trim(pList) == (void*)(LIST_MAX_NUM_NODES-3));

    List_free(pList, freeItem);
    pList = List_create();
    
    printf("testing search....\n");

    assert(List_search(pList, equals, (void*)1) == NULL);
    assert(pList->currStatus == LIST_OOB_END);
    assert(pList->currNode == NULL);


    List_append(pList, (void*)1);
    List_append(pList, (void*)2);

    pList->currStatus = LIST_OOB_START;
    assert(List_search(pList, equals, (void*)1) != NULL);
    assert(List_search(pList, equals, (void*)2) != NULL);
    assert(List_search(pList, equals, (void*)3) == NULL);

    List_free(pList, freeItem);

    for (int i = 0; i < LIST_MAX_NUM_HEADS; i++){
        List* pList = List_create();
        assert(pList != NULL);
        checkEmptyListStart(pList);
        pLists[i] = pList;
    }

    for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
        List_append(pLists[i % LIST_MAX_NUM_HEADS], malloc(sizeof(int)));
    }

    for (int i = 0; i < LIST_MAX_NUM_HEADS; i++){
        List_free(pLists[i], free);
    }
    
    printf("ALL TESTS PASSED!!!");
}
