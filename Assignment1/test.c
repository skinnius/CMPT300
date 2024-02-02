#include "list.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>


//FREE_FN
void freeItem(void* pItem){
    pItem = NULL;
}

// COMPARATOR_FN
int equals(void* item1, void* item2) {
    if (*(int*)item1 == *(int*)item2){
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
    

    printf("Testing insert_after()...");
    int a[5] = {1,2,3,4,5};
    assert(List_insert_after(pList, &a[0]) == 0);
    assert(List_insert_after(pList, &a[1]) == 0);
    assert(List_insert_after(pList, &a[2]) == 0);
    assert(List_insert_after(pList, &a[3]) == 0);
    assert(List_insert_after(pList, &a[4]) == 0);


    printf("All insert tests passed!");


    printf("ALL TESTS PASSED!!!");
    
}





//     

    






// }


// void test_item(List* pList){
//     // test for insert_after()



//     assert(numHeads == 1);
//     assert(numNodes == 5);
//     assert(pList->currNode == pList->tail);
//     assert(pList->currStatus == LIST_OOB_START);

//     pList->currNode = pList->head;
//     int i = 1;
//     while (pList->currNode != NULL){
//         printf("current item %d: %d\n", i, *(int*)pList->currNode->item);
//         assert(*(int*)pList->currNode->item == i);
//         List_next(pList);
//         i++;
//     }

//     printf("insert_after() tests passed\n\n");

//     pList->currNode = pList->tail;
//     pList->currStatus = LIST_OOB_START;

//     // test for backwards remove()
//     for (int i = 0; i < 5; i++){
//         printf("removed item %d: %d\n", i+1, *(int*)pList->currNode->item);
//         assert(*(int*)List_remove(pList) == a[4 - i]);
//         assert(pList->currStatus == LIST_OOB_END);
//         List_prev(pList);
//     }
//     assert(numHeads == 1);
//     assert(numNodes == 0);
//     assert(pList->currNode == NULL);
//     assert(pList->currStatus == LIST_OOB_START);

//     printf("remove() (backwards) tests passed\n\n");

//     // test for insert_before()
//     assert(List_insert_before(pList, &a[0] ) == 0);
//     assert(List_insert_before(pList, &a[1]) == 0);
//     assert(List_insert_before(pList, &a[2]) == 0);
//     assert(List_insert_before(pList, &a[3]) == 0);
//     assert(List_insert_before(pList, &a[4]) == 0);


//     assert(numHeads == 1);
//     assert(numNodes == 5);
//     assert(pList->currNode == pList->head);
//     assert(pList->currStatus == LIST_OOB_START);        

//     pList->currNode = pList->head;
//     i = 0;
//     while (pList->currNode != NULL){
//         printf("current item %d: %d\n", i, *(int*)pList->currNode->item);
//         assert(*(int*)pList->currNode->item == 5-i);
//         List_next(pList);
//         i++;
//     }
    
//     pList->currNode = pList->head;
//     pList->currStatus = LIST_OOB_START;
    
//     printf("insert_before() tests passed\n\n");

//     // test for forwards remove()
//     for (int i = 0; i < 5; i++){
//         printf("removed item %d: %d\n", i+1, *(int*)pList->currNode->item);
//         assert(*(int*)List_remove(pList) == a[4-i]);
//     }
//     assert(numHeads == 1);
//     assert(numNodes == 0);
//     assert(pList->currNode == NULL);
//     assert(pList->tail == NULL);
//     assert(pList->head == NULL);
//     assert(pList->currStatus == LIST_OOB_END);

//     printf("remove() (forwards) tests passed\n\n");

//     // test for append()
//     pList->currStatus = LIST_OOB_START;   // reset the current stats
//     assert(List_append(pList, &a[0] ) == 0);
//     assert(List_append(pList, &a[1]) == 0);
//     assert(List_append(pList, &a[2]) == 0);
//     assert(List_append(pList, &a[3]) == 0);
//     assert(List_append(pList, &a[4]) == 0);

//     assert(numHeads == 1);
//     assert(numNodes == 5);
//     assert(pList->currNode == pList->tail);
//     assert(pList->currStatus == LIST_OOB_START);
//     assert(pList->head->item == &a[0]);
//     assert(pList->tail->item == &a[4]);

//     i = 0;
//     while (pList->currNode != NULL){
//         printf("current item %d: %d\n", i+1, *(int*)pList->currNode->item);
//         assert(*(int*)pList->currNode->item == 5-i);
//         List_prev(pList);
//         i++;
//     }
//     assert(numHeads == 1);
//     assert(numNodes == 5);

//     List_prev(pList);
//     assert(pList->currNode == NULL);
//     assert(pList->currStatus == LIST_OOB_START);

//     printf("List_append() tests complete \n\n");

//     pList->currNode = pList->head;
//     i = 1;
//     while(pList->currNode != NULL){
//         assert(*(int*)List_remove(pList) == i);
//         i++;
//     }

//     assert(numHeads == 1);
//     assert(numNodes == 0);

//     // List_prepend() test cases
//     pList->currStatus = LIST_OOB_START;   // reset the current stats
//     assert(List_prepend(pList, &a[0] ) == 0);
//     assert(List_prepend(pList, &a[1]) == 0);
//     assert(List_prepend(pList, &a[2]) == 0);
//     assert(List_prepend(pList, &a[3]) == 0);
//     assert(List_prepend(pList, &a[4]) == 0);

//     assert(pList->currNode == pList->head);
//     assert(pList->currStatus == LIST_OOB_START);
//     assert(numHeads == 1);
//     assert(numNodes == 5);
//     assert(pList->head->item == &a[4]);
//     assert(pList->tail->item == &a[0]);

//     i = 0;
//     while (pList->currNode != NULL){
//         printf("current item %d: %d\n", i+1, *(int*)pList->currNode->item);
//         assert(*(int*)pList->currNode->item == 5-i);
//         List_next(pList);
//         i++;
//     }
//     assert(numHeads == 1);
//     assert(numNodes == 5);
    
//     List_next(pList);
//     assert(pList->currNode == NULL);
//     assert(pList->currStatus == LIST_OOB_END);
// }

// // Test Suite 3: Movement of current pointer
// void test_currMovement(List* pList){
//     assert(pList != NULL);
//     int a[5] = {1,2,3,4,5};

//     printf("Testing for empty case...\n");
//     assert(pList->listSize == 0);
//     assert(List_first(pList) == NULL);
//     assert(pList->currNode == NULL);
//     assert(pList->currStatus == LIST_OOB_START);

//     assert(List_last(pList) == NULL);
//     assert(pList->currNode == NULL);
//     assert(pList->currStatus == LIST_OOB_START);

//     assert(List_append(pList, &a[0]) == 0);
//     assert(List_append(pList, &a[1]) == 0);
//     assert(List_append(pList, &a[2]) == 0);
//     assert(List_append(pList, &a[3]) == 0);
//     assert(List_append(pList, &a[4]) == 0);

//     printf("Testing for general case...\n");
//     assert(List_first(pList) == pList->head->item);
//     assert(pList->currNode == pList->head);
//     assert(List_last(pList) == pList->tail->item);
//     assert(pList->currNode == pList->tail);
//     assert(pList->currStatus == LIST_OOB_START);

//     printf("all last()/first() test cases passed!\n\n");
//     printf("testing next()/prev()...\n");

//     // reset....

//     List_first(pList);
//     int i = 0;
//     while (pList->currNode != NULL){
//         assert(pList->currNode->item == &a[i]);
//         List_next(pList);
//         i++;
//     }
//     assert(pList->currStatus == LIST_OOB_END);

//     // reset...
//     List_last(pList);
//     assert(pList->currNode == pList->tail);
//     i = 0;
//     while (pList->currNode != NULL){
//         printf("%d\n", *(int*)pList->currNode->item);
//         assert(pList->currNode->item == &a[4-i]);
//         List_prev(pList);
//         i++;
//     }
//     assert(pList->currStatus == LIST_OOB_START);
//     printf("all next()/prev() test cases passed! \n\n");
// }

// // Test suite 4: concat
// void test_concat(){
//     char* a[5] = {"a","b","c","d","e"};
//     char* b[5] = {"f","g","h","i","j"};

//     // case 1: both empty lists.
//     List* pList1 = List_create();
//     List* pList2 = List_create();
//     assert(numHeads == 4);

//     List_concat(pList1, pList2);
//     assert(numHeads == 3);

//     // make sure pList2 was properly freed
//     assert(pList2->currNode == NULL);
//     assert(pList2->currStatus == LIST_OOB_START);
//     assert(pList2->listSize == 0);
//     assert(pList2->head == NULL);
//     assert(pList2->tail == NULL);

//     assert(pList1->currNode == NULL);
//     assert(pList1->currStatus == LIST_OOB_START);
//     assert(pList1->listSize == 0);
//     assert(pList1->head == NULL);
//     assert(pList1->tail == NULL);

//     // case 2: list1 is empty
//     List* pList = List_create();
//     assert(numHeads == 4);
//     assert(List_append(pList, a[0]) == 0);
//     assert(List_append(pList, a[1]) == 0);
//     assert(List_append(pList, a[2]) == 0);
//     assert(List_append(pList, a[3]) == 0);
//     assert(List_append(pList, a[4]) == 0);


//     List_concat(pList1, pList);
//     assert(numHeads == 3);
//     assert(pList1->head->item == a[0]);
//     assert(pList1->tail->item == a[4]);
//     assert(pList1->currNode == NULL);
//     assert(pList1->currStatus == LIST_OOB_START);
//     assert(pList1->listSize == 5);

//     assert(pList->currNode == NULL);
//     assert(pList->currStatus == LIST_OOB_START);
//     assert(pList->listSize == 0);
//     assert(pList->head == NULL);
//     assert(pList->tail == NULL);

//     // case 3: list2 is empty

//     List* pList2 = List_create();
//     assert(numHeads == 4);
//     List_concat(pList1, pList2);
//     assert(numHeads == 3);

//     assert(pList1->head->item == a[0]);
//     assert(pList1->tail->item == a[4]);
//     assert(pList1->currNode == NULL);
//     assert(pList1->currStatus == LIST_OOB_START);
//     assert(pList1->listSize == 5);

//     assert(pList2->currNode == NULL);
//     assert(pList2->currStatus == LIST_OOB_START);
//     assert(pList2->listSize == 0);
//     assert(pList2->head == NULL);
//     assert(pList2->tail == NULL);

//     // case 4: general case

//     List* pList3 = List_create();
//     List_next(pList1);
//     assert(List_append(pList3, b[0]) == 0);
//     assert(List_append(pList3, b[1]) == 0);
//     assert(List_append(pList3, b[2]) == 0);
//     assert(List_append(pList3, b[3]) == 0);
//     assert(List_append(pList3, b[4]) == 0);
//     assert(numHeads == 4);
//     List_concat(pList1, pList3);
//     assert(numHeads == 3);

//     assert(pList1->head->item == a[0]);
//     assert(pList1->tail->item == b[4]);
//     assert(pList1->currStatus == LIST_OOB_START);
//     assert(pList1->listSize == 10);
//     assert(pList1->currNode->item == a[0]);

//     pList1->currNode = pList1->head;
//     printf("%s\n", (char*)pList1->currNode->item);
//     while(pList1->currNode->next != NULL){
//         printf("%s\n", (char*)List_next(pList1));
//     }

//     printf("%d\n", numHeads);
//     for (int i = numHeads; i < LIST_MAX_NUM_HEADS; i++){
//         List* newList = List_create();
//         assert(newList != NULL);
        
//     }
//     printf("so far so good...");
//     List* failList = List_create();
//     assert(failList == NULL);

//     // cleanup
//     List_free(pList3, &freeItem);
//     assert(pList2->currNode == NULL);
//     assert(pList2->currStatus == LIST_OOB_START);
//     assert(pList2->listSize == 0);
//     assert(pList2->head == NULL);
//     assert(pList2->tail == NULL);
// } 


// static void test_node_removal(){
//     List* newList = List_create();

//     assert(List_trim(newList) == NULL);

//     for (int i = numNodes; i < LIST_MAX_NUM_NODES; i++){
//         assert(List_append(newList, "a") == 0);
//     }
//     assert(List_append(newList, "a") == -1);
//     assert(nodeHead->next == NULL);
//     assert(numNodes == 100);

//     List_remove(newList);
//     assert(newList->currNode == NULL);
//     assert(nodeHead->next == NULL);

//     List_last(newList);
//     List_remove(newList);
    
//     assert(numNodes == 98);
//     assert(newList->currStatus == LIST_OOB_END);

//     assert(List_prepend(newList,"b") == 0);
//     assert(List_insert_after(newList, "c") == 0);
//     assert(List_append(newList, "b") == -1);
//     assert(numNodes == 100);


//     assert(newList->tail->item == "a");
//     assert(List_trim(newList) == "a");
// }


// void test_free(){
//     printf("number of nodes: %d, number of heads: %d \n", numNodes, numHeads);
//     List* temp = List_create();
//     List_append(temp, "c");
//     List_trim(temp);
//     assert(temp->currNode == NULL);
//     assert(temp->currStatus == LIST_OOB_END);
//     assert(temp->head == NULL);
//     assert(temp->tail == NULL);
//     assert(temp->listSize == 0);

//     List_free(temp, freeItem);
//     printf("number of nodes: %d, number of heads: %d \n", numNodes, numHeads);

//     for (int i = 0; i < LIST_MAX_NUM_HEADS - 1; i++){
//         List* temp = List_create();
//     }
//     printf("number of nodes: %d, number of heads: %d \n", numNodes, numHeads);

//     List* l1 = List_create();
//     assert(List_create() == NULL);
//     List_free(l1, freeItem);
//     assert(numHeads == LIST_MAX_NUM_HEADS - 1);
//     printf("number of nodes: %d, number of heads: %d \n", numNodes, numHeads);
    
//     List* l2 = List_create();
//     assert(l2 != NULL);
//     printf("number of nodes: %d, number of heads: %d \n", numNodes, numHeads);

//     for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
//         List_append(l2, "a");
//     }
//     l2->currNode = l2->head;

//     while(l2->currNode != NULL){
//         printf("hello %s\n", List_next(l2));
//     }

//     printf("number of nodes: %d, number of heads: %d \n", numNodes, numHeads);
//     assert(l2->listSize == 100);
//     List_free(l2, freeItem);

//     printf("number of nodes: %d, number of heads: %d \n", numNodes, numHeads);

//     List* l3 = List_create();
//     assert(l3 != NULL);
//     printf("number of nodes: %d, number of heads: %d \n", numNodes, numHeads);

//     for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
//         List_append(l3, "a");
//     }
//     printf("number of nodes: %d, number of heads: %d \n", numNodes, numHeads);

//     // while (l3->currNode != NULL){
//     //     printf("hello %s\n", List_next(l3));
//     // }

// }

// void test_search(){
//     List* lst = List_create();
//     List_append(lst, "a");
//     List_append(lst, "b");

//     assert(List_search(lst, equals, "a") != NULL);
//     assert(List_search(lst, equals, "c") == NULL);
// }




// // int main(void){
// //     // List* pList = List_create();

// //     // // IMPORTANT: CAN ONLY RUN ONE FUNCTION AT A TIME, COMMENT THE REST OF THE FUNCTIONS OUT. 
// //     // test_create(pList);
// //     // printf("All create() tests passed!\n\n");
    
// //     // // test_insert();
// //     // test_item(pList);
// //     // printf("All insertion/deletion tests passed!\n\n");
// //     // List* pList2 = List_create();
// //     // test_currMovement(pList2);
// //     // printf("All current pointer movement test cases passed!\n\n");

// //     // test_concat();
// //     // printf("All concat tests passed!\n\n");
// //     // test_node_removal();
// //     // printf("All remove tests passed! \n\n");
// //     // test_free();
// //     test_search();
// //     printf("All test cases passed!\n");
// // }