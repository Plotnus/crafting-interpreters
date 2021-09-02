#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
// API was taken from the note section of crafting interpreters

typedef struct Node {
    struct Node* prev;
    struct Node* next;
    char* payload;
} Node;

// if `prev` is NULL, we insert at the start of the list
void insert(Node** list, Node* prev, const char* s) {
    Node* new_node = (Node*) malloc(sizeof(Node));
    new_node->payload = malloc(strlen(s));
    strcpy(new_node->payload, s);

    new_node->prev = prev;

    if (prev) {
        new_node->next = prev->next;
        prev->next = new_node;
        if (new_node->next) {
            new_node->next->prev = new_node;
        }
    } else {
        new_node->next = *list;
        if (new_node->next) {
            new_node->next->prev = new_node;
        }
        *list = new_node;
    }
}

Node* find(Node* list, const char* s) {
    while (NULL != list) {
        if (strcmp(s,list->payload) == 0)
            return list;
        list = list->next;
    }
    return NULL;
}

void delete(Node** list, Node* node) {
    if (*list == node) {
        *list = node->next;
    }
    if (node->prev) {
        node->prev->next = node->next;
    }
    if (node->next) {
        node->next->prev = node->prev;
    }

    free(node->payload);
    free(node);
}

void dump(Node* list) {
    while (NULL != list) {
        printf(
            "%p: Node{ prev: %p, next: %p, payload:%s },\n"
            , list
            , list->prev
            , list->next
            , list->payload
        );
        list = list->next;
    }
}

int main(int argc, const char* argv[]) {
    Node* front = NULL;
    //======== check insert ========//
    // insert empty test
    insert(&front, NULL, "1");
    assert(0 == strcmp(front->payload, "1"));
    assert(NULL == front->next);
    assert(NULL == front->next);

    // insert back test
    insert(&front, front, "3");
    assert(0 == strcmp(front->next->payload, "3"));
    assert(front->next->prev == front);
    assert(front->next->next == NULL);

    // insert middle test
    insert(&front, front, "2");
    assert(0 == strcmp(front->payload, "1"));
    assert(0 == strcmp(front->next->payload, "2"));
    assert(0 == strcmp(front->next->next->payload, "3"));


    // insert front in initialized list
    insert(&front, NULL, "0");
    assert(0 == strcmp(front->payload, "0"));
    assert(0 == strcmp(front->next->payload, "1"));
    assert(front->next->prev == front);

    dump(front); // 0,1,2,3

    //======== check find ========//
    Node* n;
    const char* needle = "3";
    n = find(front, needle);
    assert(strcmp(n->payload, needle) == 0);

    n = find(front, "DNE");
    assert(n == NULL);


    //======== check delete ========//
    printf("Let's check deleting elements:\n");
    // checking deleting a middle node
    delete(&front, front->next);
    printf("\nAfter deleting the second node:\n");
    dump(front);
    assert(0 == strcmp(front->payload, "0"));
    assert(0 == strcmp(front->next->payload, "2"));

    // checking deleting the first node
    delete(&front, front);
    printf("\nAfter deleting the first node:\n");
    dump(front);
    assert(0 == strcmp(front->payload, "2"));

    // linked list should now be 2,3
    // let's check deleting the last one
    printf("\nNow let's check delete the last node:\n");
    delete(&front, front->next);
    dump(front);
    assert(front->next == NULL);

    // lets delete the last node
    printf("\nNow let's check delete the last remaining node:\n");
    delete(&front, front);
    dump(front);
    assert(front == NULL);

    return 0;
}