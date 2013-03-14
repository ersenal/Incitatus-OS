/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| LinkedList.h
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Doubly linked list data structure implementation.
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <Lib/LinkedList.h>
#include <Memory/HeapMemory.h>
#include <Debug.h>

/*=======================================================
    FUNCTION
=========================================================*/

PRIVATE Node* LinkedList_getNode(LinkedList* self, void* data) {

    LinkedList_FOREACH(node, self)
        if(node->data == data)
            return node;

    return NULL; /* Return if not found */

}

PRIVATE void* LinkedList_removeNode(LinkedList* self, Node* node) {

    Debug_assert(self->first != NULL && self->last != NULL);
    Debug_assert(node != NULL);

    if(node == self->first && node == self->last) { /* node is the only element */

        self->first = NULL;
        self->last = NULL;

    } else if(node == self->first) { /* node is the first element */

        self->first = node->next;
        self->first->prev = NULL;

    } else if(node == self->last) { /* node is the last element */

        self->last = node->prev;
        self->last->next = NULL;

    } else { /* node is a middle element */

        Node* next = node->next;
        Node* prev = node->prev;

        next->prev = prev;
        prev->next = next;

    }

    self->count--;
    void* val = node->data;
    HeapMemory_free(node);

    return val;

}

PUBLIC void LinkedList_add(LinkedList* self, void* data) {

    Node* node = HeapMemory_calloc(1, sizeof(Node));
    Debug_assert(node != NULL);
    Debug_assert(data != NULL);
    node->data = data;

    if(self->last == NULL) { /* linked list element count is 0 */

        self->first = node;
        self->last = node;

    } else {

        self->last->next = node;
        node->prev = self->last;
        self->last = node;

    }

    self->count++;

}

PUBLIC void* LinkedList_remove(LinkedList* self, void* data) {

    Debug_assert(data != NULL);
    Node* node = LinkedList_getNode(self, data);
    return LinkedList_removeNode(self, node);

}

PUBLIC void* LinkedList_removeFromFront(LinkedList* self) {

    return LinkedList_removeNode(self, self->first);

}

void* LinkedList_getFront(LinkedList* self) {

    return self->first->data;

}

PUBLIC LinkedList* LinkedList_new() {

    return HeapMemory_calloc(1, sizeof(LinkedList));

}

PUBLIC void LinkedList_destroy(LinkedList* self) {

    /* Free all nodes in linked list */
    Node* node = self->first;

    while(node != NULL) {

        Node* temp = node;
        node = node->next;
        HeapMemory_free(temp);

    }

    /* Free linked list */
    HeapMemory_free(self);

}