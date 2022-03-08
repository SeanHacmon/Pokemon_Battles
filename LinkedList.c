//
// Created by Sean Hakmon on 10/12/2021.
//

#include "LinkedList.h"
#include "Defs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/// Defining the structures.
struct LinkedList_t
{
    Node* Head;
    Node* Tail;
    int Size;
    copyFunction CopyEle;
    freeFunction FreeEle;
    equalFunction SearchEle;
    printFunction PrintEle;
    equalFunction EqEle;

};

struct LSNode
{
    element Val;
    Node* Next;

};

///Creating a linklist.
LS createLinkedList(copyFunction CF, equalFunction SF, freeFunction FF, printFunction PF, equalFunction EQF)
{
    if (CF == NULL || SF == NULL || FF == NULL || PF == NULL || EQF == NULL)
        return NULL;

    LS ls = (LS) malloc(sizeof(struct LinkedList_t));
    if (ls == NULL)
    {
        printf("No Memory available.\n");
        return NULL;
    }

    ls->Size = 0;
    ls->CopyEle = CF;
    ls->FreeEle = FF;
    ls->SearchEle = SF;
    ls->PrintEle = PF;
    ls->Head =NULL;
    ls->Tail = NULL;
    ls->EqEle = EQF;
    return ls;
}

GenStatus destroyList(LS ls)
{
    if (ls == NULL)
        return DeleteFailure;

    Node* TempNode = ls->Head;
    while (TempNode != NULL)
    {
        ls->Head = ls->Head->Next;
        ls->FreeEle(TempNode->Val);
        free(TempNode);
        TempNode = ls->Head;
    }
    free(ls->Head);
    free(TempNode);
    free(ls);
    ls = NULL;
    return DeleteSuccess;
}

/// Adding a node
GenStatus appendNode(LS ls, element ele)
{
    if (ele == NULL)
        return AddFailure;

    Node* NewNode = (Node*) malloc(sizeof(Node));
    if (NewNode == NULL)
    {
        printf("No Memory available.\n");
        return AddFailure;
    }
    NewNode->Val = ele;
    NewNode->Next = NULL;
    if (ls->Size == 0)
    {
        ls->Head = NewNode;
        ls->Head->Next = ls->Tail;
        ls->Tail = NewNode;
        ls->Size += 1;
        return AddSuccess;
    }
    ls->Tail->Next = NewNode;
    ls->Tail = ls->Tail->Next;
    ls->Size += 1;
    return AddSuccess;
}

/// Removing a specific node in the linklist.
GenStatus deleteNode(LS ls, element ele)
{
    Node* curr = ls->Head;
    Node* prev;
    if (curr == NULL)
        return DeleteFailure;
    if (ls->EqEle(curr->Val, ele) == 0)
    {
        ls->Head = curr->Next;
        ls->FreeEle(curr->Val);
        free(curr);
        ls->Size -= 1;
        return DeleteSuccess;
    }
    while (ls->EqEle(curr->Val,ele) != 0)
    {
        if (curr->Next == NULL)
            return DeleteFailure;
        prev = curr;
        curr = curr->Next;
    }
    prev->Next = curr->Next;
    ls->Size -= 1;
    ls->FreeEle(curr->Val);
    free(curr);
    return DeleteSuccess;
}

/// Printing the linklist.
void displayList (LS ls)
{
    if (ls->Head == NULL)
        NULL;
    Node* Curr = ls->Head;
    while (Curr != NULL)
    {
        ls->PrintEle(Curr->Val);
        Curr = Curr->Next;
    }
}

/// Searching in the linklist.
element searchByKeyInList(LS ls, element Ele)
{
    if (Ele == NULL)
        return NULL;
    Node* Curr = ls->Head;
    while (Curr)
    {
        if (ls->EqEle(Curr, Ele) == 0)
            return Curr->Val;

        if (ls->SearchEle(Curr->Val, Ele) == 1)
            return Curr->Val;

        Curr = Curr->Next;
    }
    return NULL;
}

/// Returns linklist size.
int getsize(LS linkedList){
    return linkedList->Size;
}






