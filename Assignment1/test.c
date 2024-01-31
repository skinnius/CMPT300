#include "list.c"

//FREE_FN
void freeItem(void* pItem){
    pItem = NULL;
}

// Test Suite 1: List creation.
void test_create(){
    // Test case 1: testing for creation of lists
    for (int i = 0; i < LIST_MAX_NUM_HEADS; i++){
        List* newList = List_create();
        assert(newList != NULL);
        assert(newList->currStatus == LIST_OOB_START);
        assert(newList->currNode == NULL);
        assert(newList->listSize == 0);
        assert(newList->head == NULL);
        assert(newList->tail == NULL);
        assert(List_count(newList) == 0);

        if (numList == LIST_MAX_NUM_HEADS){
            assert(newList->next == NULL);
        }
        else{
            assert(newList->next != NULL);
        }
    }
    printf("current number of heads in list: %d\n",numList);

    // Test case 2: attempting to create a new list after reaching maximum capacity (boundary)
    List* failList = List_create();
    assert(failList == NULL);

    // Test case 3: extra tests for good measure
    for (numList = 10; numList < 10000; numList++){
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
    // // test case 1: testing for creation of nodes
    // for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
    //     assert(List_insert_after(testList, &i) == 0);
    //     assert(testList->currNode != NULL);
    //     assert(testList->currNode->item == &i);
    //     assert(testList->currStatus == LIST_OOB_START);

    //     // empty list case
    //     if (List_count(testList) == 1){
    //         assert(testList->head = testList->currNode);
    //         assert(testList->currStatus == LIST_OOB_START);
    //     }

    //     printf("%d\n", *(int*)(testList->currNode->item));
    // }

    // // Test case 2: attempting to insert a new node after reaching max capacity
    // assert(List_insert_after(testList, testItem) == -1);
    
    // // more test cases for good measure
    // for (numNodes = LIST_MAX_NUM_NODES;  numNodes< 10000000; numNodes++){
    //     assert(List_insert_after(testList, testItem) == -1);
    // }

    // // Test case 3: Inserting at the beginning of the list.
    // for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
    //     assert(List_insert_after(testList, &i) == 0);
    //     assert(testList->listSize == i + 1);
    //     printf("%d\n", *(int*)testList->currNode->item);

    //     assert(testList->currNode != NULL);
    //     assert(testList->currStatus == LIST_OOB_START);
    //     assert(testList->currNode == testList->head);

    //     testList->currNode = NULL;
    //     testList->currStatus = LIST_OOB_START;

    // }
    
    // // Test case 4: Inserting at the end of the list. (OOB)
    // for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
    //     assert(List_insert_after(testList, &i) == 0);
    //     printf("%d\n", *(int*)testList->currNode->item);

    //     assert(testList->currNode != NULL);
    //     if (i > 0){
    //         assert(testList->currStatus == LIST_OOB_END);
    //     }
    //     assert(testList->currNode == testList->tail);

    //     testList->currNode = NULL;
    //     testList->currStatus = LIST_OOB_END;
    // }

    // //Test case 4.1: Inserting at the end of the list. (Tail)
    // for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
    //     assert(List_insert_after(testList, &i) == 0);
    //     printf("%d\n", *(int*)testList->currNode->item);

    //     assert(testList->currNode != NULL);
    //     assert(testList->currStatus == LIST_OOB_START);
    //     assert(testList->currNode == testList->tail);

    //     testList->currNode = testList->tail;
    // }


    // // Test case 5: Inserting at the middle of the list. (General case)
    // for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
    //     assert(List_insert_after(testList, testItem) == 0);
    //     printf("%d\n", i);
    //     printf("%d\n", *(int*)testList->currNode->item);
        
    //     assert(testList->currNode != NULL);
    //     assert(testList->currStatus == LIST_OOB_START);
    //     assert(testList->listSize == i + 1);

    //     if (i <= 1){
    //         assert(testList->currNode == testList->tail);
    //     }

    //     if (i > 2){
    //         assert(testList->currNode == testList->tail->prev);
    //     }
    //     if (i > 1){
    //         testList->currNode = testList->tail->prev;
    //     }
    // }

    // Section 2: testing for insert_before()

    // // Test case 1: testing for creation of nodes
    // for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
    //     assert(List_insert_before(testList, &i) == 0);
    //     assert(testList->currNode != NULL);
    //     assert(testList->currNode->item == &i);
    //     assert(testList->currStatus == LIST_OOB_START);

    //     // empty list case
    //     if (List_count(testList) == 1){
    //         assert(testList->head = testList->currNode);
    //         assert(testList->currStatus == LIST_OOB_START);
    //     }

    //     printf("%d\n", *(int*)(testList->currNode->item));
    // }

    // // Test case 2: attempting to insert a new node after reaching max capacity
    // assert(List_insert_before(testList, testItem) == -1);
    
    // // more test cases for good measure
    // for (numNodes = LIST_MAX_NUM_NODES;  numNodes< 10000000; numNodes++){
    //     assert(List_insert_before(testList, testItem) == -1);
    // }

    // // Test case 3: Inserting at the end of the list.
    // for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
    //     testList->currNode = NULL;
    //     testList->currStatus = LIST_OOB_END;

    //     assert(List_insert_before(testList, &i) == 0);
    //     assert(testList->listSize == i + 1);
    //     printf("%d\n", *(int*)testList->currNode->item);

    //     assert(testList->currNode != NULL);
    //     assert(testList->currStatus == LIST_OOB_END);
    //     assert(testList->currNode == testList->tail);
    // }

    
    // // Test case 4: Inserting at the start of the list. (OOB)
    // for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
    //     assert(List_insert_before(testList, &i) == 0);
    //     printf("%d\n", *(int*)testList->currNode->item);

    //     assert(testList->currNode != NULL);
    //     if (i > 0){
    //         assert(testList->currStatus == LIST_OOB_START);
    //     }
    //     assert(testList->currNode == testList->head);

    //     testList->currNode = NULL;
    //     testList->currStatus = LIST_OOB_START;
    // }

    // Test case 4.1: Inserting at the end of the list. (Head)
    // for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
    //     assert(List_insert_before(testList, &i) == 0);
    //     printf("%d\n", *(int*)testList->currNode->item);

    //     assert(testList->currNode != NULL);
    //     assert(testList->currStatus == LIST_OOB_START);
    //     assert(testList->currNode == testList->head);

    //     testList->currNode = testList->head;
    // }

    // // Test case 5: Inserting at the middle of the list. (General case)
    // for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
    //     assert(List_insert_before(testList, testItem) == 0);
    //     printf("%d\n", i);
    //     printf("%d\n", *(int*)testList->currNode->item);
        
    //     assert(testList->currNode != NULL);
    //     assert(testList->currStatus == LIST_OOB_START);
    //     assert(testList->listSize == i + 1);

    //     if (i <= 1){
    //         assert(testList->currNode == testList->head);
    //     }

    //     if (i > 2){
    //         assert(testList->currNode == testList->head->next);
    //     }
    //     if (i > 1){
    //         testList->currNode = testList->head->next;
    //     }
    // }


}


void test_item(){
    // test for insert_after()
    printf("test_item() running...\n");
    List* testList = List_create();
    int a[5] = {1,2,3,4,5};
    assert(List_insert_after(testList, &a[0] ) == 0);
    assert(List_insert_after(testList, &a[1]) == 0);
    assert(List_insert_after(testList, &a[2]) == 0);
    assert(List_insert_after(testList, &a[3]) == 0);
    assert(List_insert_after(testList, &a[4]) == 0);


    assert(numList == 1);
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
    assert(numList == 1);
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


    assert(numList == 1);
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
    assert(numList == 1);
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

    assert(numList == 1);
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
    assert(numList == 1);
    assert(numNodes == 5);

    printf("List_append() tests complete \n\n");

}







// Test Suite 3: Movement of current pointer
void test_current(){
}

int main(void){

    // test_create();
    printf("All create() tests passed!\n");
    // test_insert();
    test_item();
    // test_current();
    printf("all test cases passed!");
}