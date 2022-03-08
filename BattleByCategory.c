//
// Created by Sean Hakmon on 10/12/2021.
//

#include "MaxHeap.h"
#include "LinkedList.h"
#include "Defs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "BattleByCategory.h"


/// Defining the struct.
struct battle_s
{
    MH* MaxHeapArr;
    LS LinkedList;
    int Cap;
    int NumOfCat;
    char* Cat;
    equalFunction EqEle;
    copyFunction CpyEle;
    freeFunction FreeEle;
    getCategoryFunction GetCat;
    getAttackFunction GetAtk;
    printFunction PrintEle;
};

/// Creating a generic struct that will support battle between elements.
/// It will include inside a generic linklist & a generic maxheap.
Battle createBattleByCategory(int capacity,int numberOfCategories,char* categories,equalFunction equalElement,
                              copyFunction copyElement,freeFunction freeElement,getCategoryFunction getCategory,
                              getAttackFunction getAttack,printFunction printElement)
{
    if (copyElement == NULL || freeElement == NULL || getCategory == NULL || getAttack == NULL || printElement == NULL
    || equalElement == NULL || capacity == 0 )
        return NULL;

    Battle battle = (Battle) malloc(sizeof(struct battle_s));
    if (battle == NULL)
    {
        printf("No Memory available.\n");
        return NULL;
    }

    battle->Cat = (char*) malloc(sizeof(char)* strlen(categories) + 1);
    if (battle->Cat == NULL)
    {
        printf("No Memory available.\n");
        return NULL;
    }

    strcpy(battle->Cat, categories);
    battle->Cap = capacity;
    battle->NumOfCat = numberOfCategories;
    battle->EqEle = equalElement;
    battle->CpyEle = copyElement;
    battle->FreeEle = freeElement;
    battle->GetCat = getCategory;
    battle->GetAtk = getAttack;
    battle->PrintEle = printElement;


    battle->LinkedList = createLinkedList((copyFunction) CopyEntireHeap, (equalFunction) SearchInsideHeap,
                                          (freeFunction) destroyHeap, (printFunction) printHeap,
                                          (equalFunction) CompareHeapNames);

    battle->MaxHeapArr = malloc(sizeof(MH) * battle->NumOfCat);
    if (battle->MaxHeapArr == NULL)
    {
        printf("No Memory available.\n");
        destroyBattleByCategory(battle);
        return NULL;
    }

    int i = 0;
    char* buffer = strtok(battle->Cat, ",");
    while (buffer != NULL)
    {
        battle->MaxHeapArr[i] = CreateHeap(battle->FreeEle, battle->CpyEle,
                                           battle->EqEle, battle->Cap, buffer, battle->PrintEle);
        appendNode(battle->LinkedList, battle->MaxHeapArr[i]);
        buffer = strtok(NULL, ",\r\n");
        i ++;
    }
    return battle;
}

/// Destroys the Battle Structure.
status destroyBattleByCategory(Battle b)
{
    destroyList(b->LinkedList);
    free(b->Cat);
    b->Cat = NULL;
    free(b->MaxHeapArr);
    b->MaxHeapArr = NULL;
    free(b);
    b = NULL;
    return success;
}


/// Inserting objects inside the Battle structure.
status insertObject(Battle b, element elem)
{
    int index = -1;

    if (elem == NULL)
        return failure;

    char* TempType = b->GetCat(elem);

    for (int i = 0; i < b->NumOfCat; ++i)
    {
        char* Name = getHeapId(b->MaxHeapArr[i]);
        if (CompareHeapNames(TempType,Name) == 1)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
        return failure;

    if (getHeapCurrentSize(b->MaxHeapArr[index]) > b->Cap)
        return failure;

    insertToHeap(b->MaxHeapArr[index], elem);
    return success;
}

/// using linklist printfunction to print the objects inside the Battle structure.
void displayObjectsByCategories(Battle b)
{
    displayList(b->LinkedList);
}


/// Removes an object from the Battle structure.
element removeMaxByCategory(Battle b,char* category)
{
    int index = -1;
    for (int i = 0; i < b->NumOfCat; ++i)
    {
        if (CompareHeapNames(getHeapId(b->MaxHeapArr[i]), category) == 1)
            index = i;
    }
    if (index == -1)
        return NULL;
    return PopMaxHeap(b->MaxHeapArr[index]);
}

int getNumberOfObjectsInCategory(Battle b,char* category)
{
    int index = -1;
    for (int i = 0; i < b->NumOfCat; ++i)
    {
        if (CompareHeapNames(getHeapId(b->MaxHeapArr[i]), category) == 1)
            index = i;
    }
    if (index == -1)
        return 0;
    return getHeapCurrentSize(b->MaxHeapArr[index]);
}


/// Begins the Battle Between the object that has the best chances to win against the object that was recivied as an argument.
element fight(Battle b,element elem)
{
    int Checker = 0;

    for (int i = 0; i < b->NumOfCat; ++i)
    {
        if (getNumberOfObjectsInCategory(b, getHeapId(b->MaxHeapArr[i])) > 0)
            Checker = 1;
    }
    if (Checker != 1)
    {
        printf("You choose to fight with:\n");
        b->PrintEle(elem);
        printf("There are no Pokemons to fight against you.\n");
        return NULL;
    }



    int TempAtk1 = 0; /// getting the attacks from GetAtk Function.
    int TempAtk2 = 0;
    int Atk[b->NumOfCat]; /// array of all highest attacks of each type.
    int counter = -1000000; ///// receiving the sub of 2 attacks.
    int AtkDif;
    int DrawIndex = 0;
    int index = 0;
    char* Category = b->GetCat(elem); // getting a type out of a specific Pokemon.
    int SuperEffective[b->NumOfCat]; // saving changes of effective on first arg of GetAtk func.
    int SuperEffectiveTemp[b->NumOfCat]; // saving changes of effective on second arg of GetAtk func.
    memset(SuperEffective, 0, sizeof(SuperEffective));
    memset(SuperEffectiveTemp, 0, sizeof(SuperEffectiveTemp));
    memset(Atk, 0, sizeof(Atk));

    if (b->MaxHeapArr == NULL)
    {
        printf("There are no Pokemons to fight against you.");
        return NULL;
    }

    for (int i = 0; i < b->NumOfCat; ++i)
    {
        if (getHeapCurrentSize(b->MaxHeapArr[i]) == 0)
        {
            continue;
        }
        AtkDif = b->GetAtk(TopMaxHeap(b->MaxHeapArr[i]), elem, &TempAtk1, &TempAtk2);
        if (AtkDif > counter)
        {
            counter = AtkDif;
            index = i;
            Atk[i] = TempAtk1;
            ///////// creating into 2 lists the upgraded attacks after checking effectiveness.//////////
            if (counter > (Atk[index] - TempAtk2))
            {
                SuperEffective[i] = Atk[index];
                SuperEffectiveTemp[i] = (TempAtk2 - 10);
            }
            if (counter < (Atk[index] - TempAtk2))
            {
                SuperEffective[i] = (Atk[index] - 10);
                SuperEffectiveTemp[i] = TempAtk2;
            }
            if (counter == (Atk[index] - TempAtk2))
            {
                SuperEffective[i] = Atk[index];
                SuperEffectiveTemp[i] = TempAtk2;
            }
        }
        if (counter == (Atk[index] - TempAtk2))
        {
            SuperEffective[i] = Atk[index];
            SuperEffectiveTemp[i] = TempAtk2;
        }


    }




    /////////////////////// PRINT SECTION ////////////////////////////////
    if (counter == 0) ///////// Elements are EQUAL.
        {
        printf("You choose to fight with:\n");
        b->PrintEle(elem);
        printf("The final battle between:\n");
        b->PrintEle(elem); // I will ask the user to use a print function in our case it will be printPokemon.
        printf("In this battle his attack is :%d\n\n"
               "against ", SuperEffectiveTemp[index]);
        b->PrintEle(TopMaxHeap(b->MaxHeapArr[index]));
        printf("In this battle his attack is :%d\n\n"
               "IT IS A DRAW.\n", SuperEffective[index]);
        for (int i = 0; i < b->NumOfCat; ++i)
        {
            if (CompareHeapNames(Category,getHeapId(b->MaxHeapArr[i])) == 1)
                DrawIndex = i;
        }
        if (DrawIndex < index)
            return elem;
        else
            return TopMaxHeap(b->MaxHeapArr[index]);
        }

    if (counter < 0) //////// Input Element is STRONGER.
        {
        printf("You choose to fight with:\n");
        b->PrintEle(elem);
        printf("The final battle between:\n");
        b->PrintEle(elem); // I will ask the user to use a print function in our case it will be printPokemon.
        printf("In this battle his attack is :%d\n\n"
               "against ", SuperEffectiveTemp[index]);
        b->PrintEle(TopMaxHeap(b->MaxHeapArr[index]));
        printf("In this battle his attack is :%d\n\n"
               "THE WINNER IS:\n", SuperEffective[index]);
        b->PrintEle(elem);
        return elem;
        }
    if (counter > 0) //////// Found Element is STRONGER.
        {
        printf("You choose to fight with:\n");
        b->PrintEle(elem);
        printf("The final battle between:\n");
        b->PrintEle(elem); // I will ask the user to use a print function in our case it will be printPokemon.
        printf("In this battle his attack is :%d\n\n"
               "against ", SuperEffectiveTemp[index]);
        b->PrintEle(TopMaxHeap(b->MaxHeapArr[index]));
        printf("In this battle his attack is :%d\n\n"
               "THE WINNER IS:\n", SuperEffective[index]);
        b->PrintEle(TopMaxHeap(b->MaxHeapArr[index]));
        return TopMaxHeap(b->MaxHeapArr[index]);
        }
    return NULL;
}

            
