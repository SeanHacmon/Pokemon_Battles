//
// Created by Sean Hakmon on 10/12/2021.
//

#ifndef HW3_LINKEDLIST_H
#define HW3_LINKEDLIST_H

#include <stdio.h>
#include "Defs.h"

typedef struct LinkedList_t* LS;
typedef struct LSNode Node;




LS createLinkedList(copyFunction CpyLS, equalFunction SearchLS, freeFunction FreeLS, printFunction
PrintLS, equalFunction EqualElem);
GenStatus destroyList(LS);
GenStatus appendNode (LS, element);
GenStatus deleteNode (LS, element);
void displayList (LS);

int compareFunction(LS, element); // gets a specific element attribute from the user return the specific element into SearchList.
element searchByKeyInList(LS ls, element Ele);



#endif //HW3_LINKEDLIST_H
