//
// Created by Sean Hakmon on 10/12/2021.
//

#include "MaxHeap.h"
#include "Defs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


struct MaxHeap_t
{
    element* Arr;
    element Root;
    int HeapSize;
    char* HeapName;
    int MaxCapacity;
    copyFunction CopyEle;
    freeFunction FreeEle;
    equalFunction CompareEle;
    printFunction PrintEle;
};


MH CreateHeap(freeFunction FreeH, copyFunction CopyH, equalFunction CompareH ,int MaxCap, char* Name,
              printFunction PrintEle)
{
    if (CopyH == NULL || FreeH == NULL || CompareH == NULL || PrintEle == NULL || Name == NULL)
        return NULL;
    MH mh = (struct MaxHeap_t*) malloc(sizeof(struct MaxHeap_t));
    if (mh == NULL)
    {
        printf("No Memory available.\n");
        return NULL;
    }
    mh->MaxCapacity = MaxCap;
    mh->HeapSize = 0;
    mh->CopyEle = CopyH;
    mh->FreeEle = FreeH;
    mh->CompareEle = CompareH;
    mh->PrintEle = PrintEle;
    mh->Arr = (element*) calloc(mh->MaxCapacity, sizeof(element));
    if (mh->Arr == NULL)
    {
        printf("No Memory available.\n");
        mh->FreeEle(mh);
        return NULL;
    }


    mh->HeapName = (char*) malloc(sizeof(strlen(Name)) + 1);
    if (mh->HeapName == NULL)
    {
        printf("No Memory available.\n");
        return NULL;
    }

    strcpy(mh->HeapName, Name);

    return mh;
}


GenStatus destroyHeap(MH mh)
{
    if (mh == NULL)
        return DeleteFailure;

    for (int i = 0; i < mh->MaxCapacity; ++i)
    {
        mh->FreeEle(mh->Arr[i]);
    }

    free(mh->Arr);
    mh->Arr = NULL;
    free(mh->HeapName);
    mh->HeapName = NULL;
    free(mh);
    mh = NULL;
    return DeleteSuccess;
}


GenStatus insertToHeap(MH mh, element node)
{
    if (node == NULL)
        return AddFailure;

    mh->HeapSize += 1;
    int size = mh->HeapSize - 1;
//    mh->Arr[size] = mh->CopyEle(node);
    mh->Arr[size] = node;
    while (size > 0 && mh->CompareEle(mh->Arr[Parent(size)], mh->Arr[size]) == -1)
    {
        element tempEle;
        tempEle =  mh->Arr[size];
        mh->Arr[size] = mh->Arr[Parent(size)];
        mh->Arr[Parent(size)] = tempEle;
        //        Swap(&(mh->Arr[size - 1]), &(mh->Arr[Parent(size - 1)]));
        size = Parent(size);
    }
//    mh->Root = mh->CopyEle(mh->Arr[0]);
    mh->Root = mh->Arr[0];
    return AddSuccess;
}


void printHeap (MH mh)
{
    if (mh == NULL)
        return;

    MH tempHeap = CreateHeap(mh->FreeEle, mh->CopyEle, mh->CompareEle, mh->MaxCapacity, mh->HeapName, mh->PrintEle);
    if (mh->HeapSize == 0)
    {
        printf("%s:\n"
               "No elements.\n\n", mh->HeapName);
        destroyHeap(tempHeap);
    }

    else
    {
        for (int i = 0; i < mh->HeapSize; ++i) {
            element newEle = mh->CopyEle(mh->Arr[i]);
            insertToHeap(tempHeap, newEle);
        }
        printf("%s:\n", tempHeap->HeapName);
        for (int i = tempHeap->HeapSize; i > 0; --i)
        {
            element ele = PopMaxHeap(tempHeap);
            printf("%d. ", mh->HeapSize - i + 1);
            tempHeap->PrintEle(ele);
        }
        destroyHeap(tempHeap);
    }

}


element PopMaxHeap(MH mh)
{
    element newEle = TopMaxHeap(mh);
    if (newEle == NULL)
        return NULL;
    element max = mh->CopyEle(mh->Arr[0]);
    mh->Arr[0] = mh->CopyEle(mh->Arr[mh->HeapSize - 1]);
    mh->Root = mh->Arr[mh->HeapSize - 1];
    //    mh->PrintEle(mh->Arr[0]);
    mh->Arr[mh->HeapSize - 1] = NULL;
    mh->HeapSize -= 1;
    MaxHeapify(mh, 0);
    //    mh->Arr[mh->HeapSize] = NULL;
    //    printf("%d\n",mh->HeapSize);
    mh->Root = mh->CopyEle(mh->Arr[0]);
    return max;
}


element* TopMaxHeap(MH mh)
{
    if (mh->HeapSize == 0)
        return NULL;
    return mh->Arr[0];
}


char* getHeapId(MH mh)
{
    return mh->HeapName;
}


int getHeapCurrentSize(MH mh)
{
    return mh->HeapSize;
}





void MaxHeapify(MH heap, int i) //// making sure the heap will be in the correct order.
{
    int largest;
    int l = Left(i);
    int r = Right(i);
    if (l <= heap->HeapSize && heap->CompareEle(heap->Arr[l] ,heap->Arr[i]) == 1)
        largest = l;
    else largest = i;
    if (r <= heap->HeapSize && heap->CompareEle(heap->Arr[r], heap->Arr[largest]) == 1)
        largest = r;
    if (largest != i)
    {
        element* tempEle =  heap->Arr[i];
        heap->Arr[i] = heap->Arr[largest];
        heap->Arr[largest] = tempEle;
        MaxHeapify(heap, largest);
    }
}


////// Static functions that gets a index and returns the correct one by the name.

int static Parent(int i)
{
    if (i == 0)
        return 0;
    i = (int) i/2;
    return i;
}

int static Right(int i)
{
    i = 2 * i;
    return i;
}

int static Left(int i)
{
    i =(2 * i) + 1;
    return i;
}



int CompareHeapNames(char* heapName1, char* heapName2) //// Compares between 2 Heap Names.
{
    if (strcmp(heapName1, heapName2) == 0)
        return 1;
    return 0;
}

MH CopyEntireHeap(MH originalHeap) //// Copying a Heap, Shallow copy.
{
    return originalHeap;
}

int SearchInsideHeap(char* name , MH heap) //// Searching an Element inside a heap by it's name.
{
    int len = getHeapCurrentSize(heap);
    for (int i = 0; i < len; ++i)
    {
        if (strcmp(heap->Arr[i], name) == 0)
            return 1;
    }
    return 0;
}


element getObj(MH mh, int i)
{
    return mh->Arr[i];
}


              //    int index;


              //    Element Temp;
              //        Temp = mh->CopyEle(mh->Arr[size]);
              //        mh->Arr[size] = mh->CopyEle(mh->Arr[Parent(size)]);
              //        mh->Arr[Parent(size)] = mh->CopyEle(Temp);

              //        Temp = heap->Arr[i];
              //        heap->Arr[i] = heap->Arr[largest];
              //        heap->Arr[largest] = Temp;

              //void Swap(element* ele1, element* ele2)
              //{
              //    element Temp;
              //    Temp = *ele1;
              //    *ele1 = *ele2;
              //    *ele2 = Temp;
              //}