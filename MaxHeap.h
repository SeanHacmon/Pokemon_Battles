//
// Created by Sean Hakmon on 10/12/2021.
//

#ifndef HW3_MAXHEAP_H
#define HW3_MAXHEAP_H

#include <stdio.h>
#include "Defs.h"


typedef struct MaxHeap_t* MH;


MH CreateHeap(freeFunction FreeH, copyFunction CopyH, equalFunction CompareH , int MaxCap, char* Name,
              printFunction PrintEle);
GenStatus destroyHeap(MH);
GenStatus insertToHeap(MH mh, element node); /// i is the size of the array.
void printHeap (MH);
element PopMaxHeap(MH); /// TODO Extract Max !
element* TopMaxHeap(MH); /// TODO return the highest atk (first node).
char* getHeapId(MH);
int getHeapCurrentSize(MH);
void MaxHeapify(MH, int);


int static Parent(int);
int static Left(int);
int static Right(int);
int CompareHeapNames(char* heapName1, char* heapName2);
MH CopyEntireHeap(MH originalHeap);
int SearchInsideHeap(char* name , MH heap);

#endif //HW3_MAXHEAP_H
