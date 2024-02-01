#include "list.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>


static int numNodes = 0;
static int numHeads = 0;

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

void print_list_in_order(List* pList, Node* curr){
    if (List_count(pList) == 0){
        printf("empty list %d\n", (int)pList);
        return;
    }

    while (curr != NULL){
        printf("%d", *(int*)curr->item);
        curr = curr->next;
    }
    printf("Length of list: %d\n", List_count(pList));
}

void print_list_reverse(List* pList, Node* curr){
    if (List_countt(pList) == 0){
        printf("empty list %d\n", (int)pList);
        return;
    }

    while (curr != NULL){
        printf("%d", *(int*)curr->item);
        curr = curr->prev;
    }
    printf("Length of list: %d\n", List_count(pList));
}









// Test Suite 1: List creation.
void test_create(){
    // Test case 1: testing for creation of lists
    for (int i = 0; i < LIST_MAX_NUM_HEADS - 1; i++){
        List* newList = List_create();
        assert(newList != NULL);
        assert(newList->currStatus == LIST_OOB_START);
        assert(newList->currNode == NULL);
        assert(newList->listSize == 0);
        assert(newList->head == NULL);
        assert(newList->tail == NULL);
        assert(List_count(newList) == 0);

        if (numHeads == LIST_MAX_NUM_HEADS){
            assert(newList->next == NULL);
        }
        else{
            assert(newList->next != NULL);
        }
    }
    printf("current number of heads in list: %d\n",numHeads);

    // Test case 2: attempting to create a new list after reaching maximum capacity (boundary)
    List* failList = List_create();
    assert(failList == NULL);

    // Test case 3: extra tests for good measure
    for (numHeads = 10; numHeads < 10000; numHeads++){
        List* newList = List_create();
        assert(newList == NULL);
    }
}

// Test Suite 2: Node Insertion
void test_insert(){
    List* testList = List_create();
    char x = 'w';
    void* testItem = &x;

    // Section 1: testing for insert_after()
    // test case 1: testing for creation of nodes
    for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
        assert(List_insert_after(testList, &i) == 0);
        assert(testList->currNode != NULL);
        assert(testList->currNode->item == &i);
        assert(testList->currStatus == LIST_OOB_START);

        // empty list case
        if (List_count(testList) == 1){
            assert(testList->head = testList->currNode);
            assert(testList->currStatus == LIST_OOB_START);
        }

        printf("%d\n", *(int*)(testList->currNode->item));
    }

    // Test case 2: attempting to insert a new node after reaching max capacity
    assert(List_insert_after(testList, testItem) == -1);
    
    // more test cases for good measure
    for (numNodes = LIST_MAX_NUM_NODES;  numNodes< 10000000; numNodes++){
        assert(List_insert_after(testList, testItem) == -1);
    }

    // Test case 3: Inserting at the beginning of the list.
    for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
        assert(List_insert_after(testList, &i) == 0);
        assert(testList->listSize == i + 1);
        printf("%d\n", *(int*)testList->currNode->item);

        assert(testList->currNode != NULL);
        assert(testList->currStatus == LIST_OOB_START);
        assert(testList->currNode == testList->head);

        testList->currNode = NULL;
        testList->currStatus = LIST_OOB_START;

    }
    
    // Test case 4: Inserting at the end of the list. (OOB)
    for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
        assert(List_insert_after(testList, &i) == 0);
        printf("%d\n", *(int*)testList->currNode->item);

        assert(testList->currNode != NULL);
        if (i > 0){
            assert(testList->currStatus == LIST_OOB_END);
        }
        assert(testList->currNode == testList->tail);

        testList->currNode = NULL;
        testList->currStatus = LIST_OOB_END;
    }

    //Test case 4.1: Inserting at the end of the list. (Tail)
    for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
        assert(List_insert_after(testList, &i) == 0);
        printf("%d\n", *(int*)testList->currNode->item);

        assert(testList->currNode != NULL);
        assert(testList->currStatus == LIST_OOB_START);
        assert(testList->currNode == testList->tail);

        testList->currNode = testList->tail;
    }


    // Test case 5: Inserting at the middle of the list. (General case)
    for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
        assert(List_insert_after(testList, testItem) == 0);
        printf("%d\n", i);
        printf("%d\n", *(int*)testList->currNode->item);
        
        assert(testList->currNode != NULL);
        assert(testList->currStatus == LIST_OOB_START);
        assert(testList->listSize == i + 1);

        if (i <= 1){
            assert(testList->currNode == testList->tail);
        }

        if (i > 2){
            assert(testList->currNode == testList->tail->prev);
        }
        if (i > 1){
            testList->currNode = testList->tail->prev;
        }
    }

    // Section 2: testing for insert_before()

    // Test case 1: testing for creation of nodes
    for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
        assert(List_insert_before(testList, &i) == 0);
        assert(testList->currNode != NULL);
        assert(testList->currNode->item == &i);
        assert(testList->currStatus == LIST_OOB_START);

        // empty list case
        if (List_count(testList) == 1){
            assert(testList->head = testList->currNode);
            assert(testList->currStatus == LIST_OOB_START);
        }

        printf("%d\n", *(int*)(testList->currNode->item));
    }

    // Test case 2: attempting to insert a new node after reaching max capacity
    assert(List_insert_before(testList, testItem) == -1);
    
    // more test cases for good measure
    for (numNodes = LIST_MAX_NUM_NODES;  numNodes< 10000000; numNodes++){
        assert(List_insert_before(testList, testItem) == -1);
    }

    // Test case 3: Inserting at the end of the list.
    for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
        testList->currNode = NULL;
        testList->currStatus = LIST_OOB_END;

        assert(List_insert_before(testList, &i) == 0);
        assert(testList->listSize == i + 1);
        printf("%d\n", *(int*)testList->currNode->item);

        assert(testList->currNode != NULL);
        assert(testList->currStatus == LIST_OOB_END);
        assert(testList->currNode == testList->tail);
    }

    
    // Test case 4: Inserting at the start of the list. (OOB)
    for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
        assert(List_insert_before(testList, &i) == 0);
        printf("%d\n", *(int*)testList->currNode->item);

        assert(testList->currNode != NULL);
        if (i > 0){
            assert(testList->currStatus == LIST_OOB_START);
        }
        assert(testList->currNode == testList->head);

        testList->currNode = NULL;
        testList->currStatus = LIST_OOB_START;
    }

    // Test case 4.1: Inserting at the end of the list. (Head)
    for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
        assert(List_insert_before(testList, &i) == 0);
        printf("%d\n", *(int*)testList->currNode->item);

        assert(testList->currNode != NULL);
        assert(testList->currStatus == LIST_OOB_START);
        assert(testList->currNode == testList->head);

        testList->currNode = testList->head;
    }

    // Test case 5: Inserting at the middle of the list. (General case)
    for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
        assert(List_insert_before(testList, testItem) == 0);
        printf("%d\n", i);
        printf("%d\n", *(int*)testList->currNode->item);
        
        assert(testList->currNode != NULL);
        assert(testList->currStatus == LIST_OOB_START);
        assert(testList->listSize == i + 1);

        if (i <= 1){
            assert(testList->currNode == testList->head);
        }

        if (i > 2){
            assert(testList->currNode == testList->head->next);
        }
        if (i > 1){
            testList->currNode = testList->head->next;
        }
    }


}


void test_item(List* testList){
    // test for insert_after()
    printf("test_item() running...\n");
    int a[5] = {1,2,3,4,5};
    assert(List_insert_after(testList, &a[0]) == 0);
    assert(List_insert_after(testList, &a[1]) == 0);
    assert(List_insert_after(testList, &a[2]) == 0);
    assert(List_insert_after(testList, &a[3]) == 0);
    assert(List_insert_after(testList, &a[4]) == 0);


    assert(numHeads == 1);
    assert(numNodes == 5);
    assert(testList->currNode == testList->tail);
    assert(testList->currStatus == LIST_OOB_START);

    testList->currNode = testList->head;
    int i = 1;
    while (testList->currNode != NULL){
        printf("current item %d: %d\n", i, *(int*)testList->currNode->item);
        assert(*(int*)testList->currNode->item == i);
        List_next(testList);
        i++;
    }

    printf("insert_after() tests passed\n\n");

    testList->currNode = testList->tail;
    testList->currStatus = LIST_OOB_START;

    // test for backwards remove()
    for (int i = 0; i < 5; i++){
        printf("removed item %d: %d\n", i+1, *(int*)testList->currNode->item);
        assert(*(int*)List_remove(testList) == a[4 - i]);
        assert(testList->currStatus == LIST_OOB_END);
        List_prev(testList);
    }
    assert(numHeads == 1);
    assert(numNodes == 0);
    assert(testList->currNode == NULL);
    assert(testList->currStatus == LIST_OOB_START);

    printf("remove() (backwards) tests passed\n\n");

    // test for insert_before()
    assert(List_insert_before(testList, &a[0] ) == 0);
    assert(List_insert_before(testList, &a[1]) == 0);
    assert(List_insert_before(testList, &a[2]) == 0);
    assert(List_insert_before(testList, &a[3]) == 0);
    assert(List_insert_before(testList, &a[4]) == 0);


    assert(numHeads == 1);
    assert(numNodes == 5);
    assert(testList->currNode == testList->head);
    assert(testList->currStatus == LIST_OOB_START);        

    testList->currNode = testList->head;
    i = 0;
    while (testList->currNode != NULL){
        printf("current item %d: %d\n", i, *(int*)testList->currNode->item);
        assert(*(int*)testList->currNode->item == 5-i);
        List_next(testList);
        i++;
    }
    
    testList->currNode = testList->head;
    testList->currStatus = LIST_OOB_START;
    
    printf("insert_before() tests passed\n\n");

    // test for forwards remove()
    for (int i = 0; i < 5; i++){
        printf("removed item %d: %d\n", i+1, *(int*)testList->currNode->item);
        assert(*(int*)List_remove(testList) == a[4-i]);
    }
    assert(numHeads == 1);
    assert(numNodes == 0);
    assert(testList->currNode == NULL);
    assert(testList->tail == NULL);
    assert(testList->head == NULL);
    assert(testList->currStatus == LIST_OOB_END);

    printf("remove() (forwards) tests passed\n\n");

    // test for append()
    testList->currStatus = LIST_OOB_START;   // reset the current stats
    assert(List_append(testList, &a[0] ) == 0);
    assert(List_append(testList, &a[1]) == 0);
    assert(List_append(testList, &a[2]) == 0);
    assert(List_append(testList, &a[3]) == 0);
    assert(List_append(testList, &a[4]) == 0);

    assert(numHeads == 1);
    assert(numNodes == 5);
    assert(testList->currNode == testList->tail);
    assert(testList->currStatus == LIST_OOB_START);
    assert(testList->head->item == &a[0]);
    assert(testList->tail->item == &a[4]);

    i = 0;
    while (testList->currNode != NULL){
        printf("current item %d: %d\n", i+1, *(int*)testList->currNode->item);
        assert(*(int*)testList->currNode->item == 5-i);
        List_prev(testList);
        i++;
    }
    assert(numHeads == 1);
    assert(numNodes == 5);

    List_prev(testList);
    assert(testList->currNode == NULL);
    assert(testList->currStatus == LIST_OOB_START);

    printf("List_append() tests complete \n\n");

    testList->currNode = testList->head;
    i = 1;
    while(testList->currNode != NULL){
        assert(*(int*)List_remove(testList) == i);
        i++;
    }

    assert(numHeads == 1);
    assert(numNodes == 0);

    // List_prepend() test cases
    testList->currStatus = LIST_OOB_START;   // reset the current stats
    assert(List_prepend(testList, &a[0] ) == 0);
    assert(List_prepend(testList, &a[1]) == 0);
    assert(List_prepend(testList, &a[2]) == 0);
    assert(List_prepend(testList, &a[3]) == 0);
    assert(List_prepend(testList, &a[4]) == 0);

    assert(testList->currNode == testList->head);
    assert(testList->currStatus == LIST_OOB_START);
    assert(numHeads == 1);
    assert(numNodes == 5);
    assert(testList->head->item == &a[4]);
    assert(testList->tail->item == &a[0]);

    i = 0;
    while (testList->currNode != NULL){
        printf("current item %d: %d\n", i+1, *(int*)testList->currNode->item);
        assert(*(int*)testList->currNode->item == 5-i);
        List_next(testList);
        i++;
    }
    assert(numHeads == 1);
    assert(numNodes == 5);
    
    List_next(testList);
    assert(testList->currNode == NULL);
    assert(testList->currStatus == LIST_OOB_END);
}

// Test Suite 3: Movement of current pointer
void test_currMovement(List* testList){
    assert(testList != NULL);
    int a[5] = {1,2,3,4,5};

    printf("Testing for empty case...\n");
    assert(testList->listSize == 0);
    assert(List_first(testList) == NULL);
    assert(testList->currNode == NULL);
    assert(testList->currStatus == LIST_OOB_START);

    assert(List_last(testList) == NULL);
    assert(testList->currNode == NULL);
    assert(testList->currStatus == LIST_OOB_START);

    assert(List_append(testList, &a[0]) == 0);
    assert(List_append(testList, &a[1]) == 0);
    assert(List_append(testList, &a[2]) == 0);
    assert(List_append(testList, &a[3]) == 0);
    assert(List_append(testList, &a[4]) == 0);

    printf("Testing for general case...\n");
    assert(List_first(testList) == testList->head->item);
    assert(testList->currNode == testList->head);
    assert(List_last(testList) == testList->tail->item);
    assert(testList->currNode == testList->tail);
    assert(testList->currStatus == LIST_OOB_START);

    printf("all last()/first() test cases passed!\n\n");
    printf("testing next()/prev()...\n");

    // reset....

    List_first(testList);
    int i = 0;
    while (testList->currNode != NULL){
        assert(testList->currNode->item == &a[i]);
        List_next(testList);
        i++;
    }
    assert(testList->currStatus == LIST_OOB_END);

    // reset...
    List_last(testList);
    assert(testList->currNode == testList->tail);
    i = 0;
    while (testList->currNode != NULL){
        printf("%d\n", *(int*)testList->currNode->item);
        assert(testList->currNode->item == &a[4-i]);
        List_prev(testList);
        i++;
    }
    assert(testList->currStatus == LIST_OOB_START);
    printf("all next()/prev() test cases passed! \n\n");
}

// Test suite 4: concat
void test_concat(){
    char* a[5] = {"a","b","c","d","e"};
    char* b[5] = {"f","g","h","i","j"};

    // case 1: both empty lists.
    List* pList1 = List_create();
    List* pList2 = List_create();
    assert(numHeads == 4);

    List_concat(pList1, pList2);
    assert(numHeads == 3);

    // make sure pList2 was properly freed
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

    // case 2: list1 is empty
    List* testList = List_create();
    assert(numHeads == 4);
    assert(List_append(testList, a[0]) == 0);
    assert(List_append(testList, a[1]) == 0);
    assert(List_append(testList, a[2]) == 0);
    assert(List_append(testList, a[3]) == 0);
    assert(List_append(testList, a[4]) == 0);


    List_concat(pList1, testList);
    assert(numHeads == 3);
    assert(pList1->head->item == a[0]);
    assert(pList1->tail->item == a[4]);
    assert(pList1->currNode == NULL);
    assert(pList1->currStatus == LIST_OOB_START);
    assert(pList1->listSize == 5);

    assert(testList->currNode == NULL);
    assert(testList->currStatus == LIST_OOB_START);
    assert(testList->listSize == 0);
    assert(testList->head == NULL);
    assert(testList->tail == NULL);

    // case 3: list2 is empty

    List* testList2 = List_create();
    assert(numHeads == 4);
    List_concat(pList1, pList2);
    assert(numHeads == 3);

    assert(pList1->head->item == a[0]);
    assert(pList1->tail->item == a[4]);
    assert(pList1->currNode == NULL);
    assert(pList1->currStatus == LIST_OOB_START);
    assert(pList1->listSize == 5);

    assert(testList2->currNode == NULL);
    assert(testList2->currStatus == LIST_OOB_START);
    assert(testList2->listSize == 0);
    assert(testList2->head == NULL);
    assert(testList2->tail == NULL);

    // case 4: general case

    List* testList3 = List_create();
    List_next(pList1);
    assert(List_append(testList3, b[0]) == 0);
    assert(List_append(testList3, b[1]) == 0);
    assert(List_append(testList3, b[2]) == 0);
    assert(List_append(testList3, b[3]) == 0);
    assert(List_append(testList3, b[4]) == 0);
    assert(numHeads == 4);
    List_concat(pList1, testList3);
    assert(numHeads == 3);

    assert(pList1->head->item == a[0]);
    assert(pList1->tail->item == b[4]);
    assert(pList1->currStatus == LIST_OOB_START);
    assert(pList1->listSize == 10);
    assert(pList1->currNode->item == a[0]);

    pList1->currNode = pList1->head;
    printf("%s\n", (char*)pList1->currNode->item);
    while(pList1->currNode->next != NULL){
        printf("%s\n", (char*)List_next(pList1));
    }

    printf("%d\n", numHeads);
    for (int i = numHeads; i < LIST_MAX_NUM_HEADS; i++){
        List* newList = List_create();
        assert(newList != NULL);
        
    }
    printf("so far so good...");
    List* failList = List_create();
    assert(failList == NULL);

    // cleanup
    List_free(testList3, &freeItem);
    assert(testList2->currNode == NULL);
    assert(testList2->currStatus == LIST_OOB_START);
    assert(testList2->listSize == 0);
    assert(testList2->head == NULL);
    assert(testList2->tail == NULL);
} 


static void test_node_removal(){
    List* newList = List_create();

    assert(List_trim(newList) == NULL);

    for (int i = numNodes; i < LIST_MAX_NUM_NODES; i++){
        assert(List_append(newList, "a") == 0);
    }
    assert(List_append(newList, "a") == -1);
    assert(nodeHead->next == NULL);
    assert(numNodes == 100);

    List_remove(newList);
    assert(newList->currNode == NULL);
    assert(nodeHead->next == NULL);

    List_last(newList);
    List_remove(newList);
    
    assert(numNodes == 98);
    assert(newList->currStatus == LIST_OOB_END);

    assert(List_prepend(newList,"b") == 0);
    assert(List_insert_after(newList, "c") == 0);
    assert(List_append(newList, "b") == -1);
    assert(numNodes == 100);


    assert(newList->tail->item == "a");
    assert(List_trim(newList) == "a");
}


void test_free(){
    printf("number of nodes: %d, number of heads: %d \n", numNodes, numHeads);
    List* temp = List_create();
    List_append(temp, "c");
    List_trim(temp);
    assert(temp->currNode == NULL);
    assert(temp->currStatus == LIST_OOB_END);
    assert(temp->head == NULL);
    assert(temp->tail == NULL);
    assert(temp->listSize == 0);

    List_free(temp, freeItem);
    printf("number of nodes: %d, number of heads: %d \n", numNodes, numHeads);

    for (int i = 0; i < LIST_MAX_NUM_HEADS - 1; i++){
        List* temp = List_create();
    }
    printf("number of nodes: %d, number of heads: %d \n", numNodes, numHeads);

    List* l1 = List_create();
    assert(List_create() == NULL);
    List_free(l1, freeItem);
    assert(numHeads == LIST_MAX_NUM_HEADS - 1);
    printf("number of nodes: %d, number of heads: %d \n", numNodes, numHeads);
    
    List* l2 = List_create();
    assert(l2 != NULL);
    printf("number of nodes: %d, number of heads: %d \n", numNodes, numHeads);

    for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
        List_append(l2, "a");
    }
    l2->currNode = l2->head;

    while(l2->currNode != NULL){
        printf("hello %s\n", List_next(l2));
    }

    printf("number of nodes: %d, number of heads: %d \n", numNodes, numHeads);
    assert(l2->listSize == 100);
    List_free(l2, freeItem);

    printf("number of nodes: %d, number of heads: %d \n", numNodes, numHeads);

    List* l3 = List_create();
    assert(l3 != NULL);
    printf("number of nodes: %d, number of heads: %d \n", numNodes, numHeads);

    for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
        List_append(l3, "a");
    }
    printf("number of nodes: %d, number of heads: %d \n", numNodes, numHeads);

    // while (l3->currNode != NULL){
    //     printf("hello %s\n", List_next(l3));
    // }

}

void test_search(){
    List* lst = List_create();
    List_append(lst, "a");
    List_append(lst, "b");

    assert(List_search(lst, equals, "a") != NULL);
    assert(List_search(lst, equals, "c") == NULL);
}




// int main(void){
//     // List* testList = List_create();

//     // // IMPORTANT: CAN ONLY RUN ONE FUNCTION AT A TIME, COMMENT THE REST OF THE FUNCTIONS OUT. 
//     // test_create(testList);
//     // printf("All create() tests passed!\n\n");
    
//     // // test_insert();
//     // test_item(testList);
//     // printf("All insertion/deletion tests passed!\n\n");
//     // List* testList2 = List_create();
//     // test_currMovement(testList2);
//     // printf("All current pointer movement test cases passed!\n\n");

//     // test_concat();
//     // printf("All concat tests passed!\n\n");
//     // test_node_removal();
//     // printf("All remove tests passed! \n\n");
//     // test_free();
//     test_search();
//     printf("All test cases passed!\n");
// }