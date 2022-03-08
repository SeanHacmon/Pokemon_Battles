//
// Created by Sean Hakmon on 10/12/2021.
//

#include "Pokemon.h"
#include <stdlib.h>
#include <stdio.h>
#include "Defs.h"
#include <string.h>

P* CreatePokemon(char* Nme, char* Spc, float H, float W, int Atk, PT* TypePtr)
{
    P* Pokemon = (P*) malloc(sizeof(P));
    if (Pokemon == NULL)
    {
        printf("No Memory available.\n");
        return NULL;
    }
    Pokemon->Name = (char*) malloc(strlen(Nme) + 1);
    Pokemon->Species = (char*) malloc(strlen(Spc) + 1);
    if (Pokemon->Name == NULL || Pokemon->Species == NULL)
    {
        printf("No Memory available.\n");
        return NULL;
    }
    strcpy(Pokemon->Name,Nme);
    strcpy(Pokemon->Species,Spc);
    Pokemon->type = TypePtr;
    Pokemon->info = CreateBIO(H, W, Atk);
    Pokemon->type->numberOfTypes ++;
    Pokemon->info.Attack = Atk;
    Pokemon->info.Height = H;
    Pokemon->info.Weight = W;
    return Pokemon;
}

PT* CreateType(char* type)
{
    PT* Psychic = (PT*)malloc(sizeof(PT));
    if (Psychic == NULL)
    {
        printf("No Memory available.\n");
        return NULL;
    }
    Psychic->numberOfTypes = 0;
    Psychic->TypeName = (char*) malloc(strlen(type) + 1);
    if (Psychic->TypeName == NULL)
    {
        printf("No Memory available.\n");
        return NULL;
    }
    strcpy(Psychic->TypeName, type);
    Psychic->effective_against_others = NULL;
    Psychic->effective_against_me = NULL;
    Psychic->SizeMe = 0;
    Psychic->SizeOther = 0;
    return Psychic;
}

BI CreateBIO(float H,float W,int Atk)
{
    BI BPtr;
    BPtr.Height = H;
    BPtr.Weight = W;
    BPtr.Attack = Atk;
    return BPtr;
}


status AddTypeMe(PT* A , PT* B)
{
    if (A == NULL)
        return failure;
    if (B == NULL)
        return failure;

    if (A->effective_against_me == NULL)
    {
        A->effective_against_me = (PT**) malloc(sizeof(PT*));
        if (A->effective_against_me == NULL)
        {
            printf("No Memory available.\n");
            return failure;
        }
        A->SizeMe += 1;
        A->effective_against_me[0] = B;
    }
    else
    {
        A->SizeMe +=1;
        A->effective_against_me = (PT **) realloc(A->effective_against_me, sizeof(PT*)*(A->SizeMe));
        if (A->effective_against_me == NULL)
            return failure;
        A->effective_against_me[A->SizeMe - 1] = B;
    }
    return success;
}

status AddTypeOther(PT* A, PT* B)
{
    if (A == NULL)
        return failure;
    if (B == NULL)
        return failure;
    if (A->effective_against_others == NULL)
    {
        A->effective_against_others = (PT**) malloc(sizeof(PT*));
        if (A->effective_against_others == NULL)
        {
            printf("No Memory available.\n");
            return failure;
        }
        A->SizeOther += 1;
        A->effective_against_others[0] = B;
    }
    else
    {
        A->SizeOther +=1;
        A->effective_against_others = (PT **) realloc(A->effective_against_others, sizeof(PT*)*A->SizeOther);
        if (A->effective_against_others == NULL)
            return failure;
        A->effective_against_others[A->SizeOther - 1] = B;
    }
    return success;
}

status DeleteTypeMe(PT* A, char* B)
{
    int i = 0;
    while ( A->effective_against_me[i])
    {
        if (strcmp(A->effective_against_me[i]->TypeName,B) == 0)
        {
            if (A->SizeMe == 1)
            {
                A->effective_against_me[i] = NULL;
                A->SizeMe -= 1;
                free(A->effective_against_me);
                A->effective_against_me = NULL;
                return success;
            }
            for (int j = i; j < A->SizeMe - 1; ++j)
            {
                A->effective_against_me[j] = A->effective_against_me[j+1];
            }
            A->effective_against_me[A->SizeMe-1] = NULL;
            A->SizeMe -= 1;
            A->effective_against_me = (PT**) realloc(A->effective_against_me, sizeof(PT*)*A->SizeMe);
            return success;
        }
        i++;
    }
    return failure;
}

status DeleteTypeOther(PT* A, char* B)
{
    int i = 0;
    while ( A->effective_against_others[i])
    {
        if (strcmp(A->effective_against_others[i]->TypeName,B) == 0)
        {
            if (A->SizeOther == 1)
            {
                A->effective_against_others[i] = NULL;
                A->SizeOther -= 1;
                free(A->effective_against_others);
                A->effective_against_others = NULL;
                return success;
            }
            for (int j = i; j < A->SizeOther - 1; ++j)
            {
                A->effective_against_others[j] = A->effective_against_others[j+1];
            }
            A->effective_against_others[A->SizeOther-1] = NULL;
            A->SizeOther -= 1;
            A->effective_against_others = (PT**) realloc(A->effective_against_others, sizeof(PT*)*A->SizeOther);
            return success;
        }
        i++;
    }
    return failure;
}

status PrintPokemon(P* Ash)
{
    if (Ash == NULL)
        return failure;
    if (Ash->Name == NULL)
        return failure;
    if (Ash->Species == NULL)
        return failure;
    if (Ash->type->TypeName == NULL)
        return failure;
    printf("%s :\n%s, %s Type.\nHeight: %.2f m    Weight: %.2f kg    Attack: %d\n\n", Ash->Name , Ash->Species,
           Ash->type->TypeName, Ash->info.Height, Ash->info.Weight, Ash->info.Attack);
    return success;
}

status PrintPokemonType(PT* PokeType)
{
    if (PokeType == NULL)
        return failure;

    char EffOthers[sizeof(int*)*100];
    char EffMe1[sizeof(int*)*100];

    memset(EffMe1, 0, sizeof(int*)*100);
    memset(EffOthers, 0, sizeof(int*)*100);

    for (int i = 0; i < PokeType->SizeMe; ++i)
    {
        strcat(EffMe1,PokeType->effective_against_me[i]->TypeName);
        strcat(EffMe1, " ,");
    }
    EffMe1[strlen(EffMe1)-2] = '\0';


    for (int i = 0; i < PokeType->SizeOther; ++i)
    {
        strcat(EffOthers,PokeType->effective_against_others[i]->TypeName);
        strcat(EffOthers, " ,");
    }
    EffOthers[strlen(EffOthers)-2] = '\0';



    if (PokeType->SizeMe == 0 && PokeType->SizeOther == 0)
    {
        printf("Type %s -- %d pokemons\n\n", PokeType->TypeName, PokeType->numberOfTypes);
        memset(EffMe1, 0, sizeof(int*)*100);
        memset(EffOthers, 0, sizeof(int*)*100);
        return success;
    }

    if (PokeType->SizeMe == 0 && PokeType->SizeOther != 0)
    {
        printf("Type %s -- %d pokemons\n\t"
               "%s moves are super-effective against:%s\n\n",
               PokeType->TypeName, PokeType->numberOfTypes, PokeType->TypeName, EffOthers);
        memset(EffMe1, 0, sizeof(int*)*100);
        memset(EffOthers, 0, sizeof(int*)*100);
        return success;
    }

    if (PokeType->SizeOther == 0 && PokeType->SizeMe != 0 )
    {
        printf("Type %s -- %d pokemons\n\t"
               "These types are super-effective against %s:%s\n\n",
               PokeType->TypeName, PokeType->numberOfTypes, PokeType->TypeName, EffMe1);
        memset(EffMe1, 0, sizeof(int*)*100);
        memset(EffOthers, 0, sizeof(int*)*100);
        return success;
    }

    printf("Type %s -- %d pokemons\n\t"
           "These types are super-effective against %s:%s\n\t"
           "%s moves are super-effective against:%s\n\n",PokeType->TypeName, PokeType->numberOfTypes,
           PokeType->TypeName,EffMe1, PokeType->TypeName, EffOthers);

    memset(EffMe1, 0, sizeof(int*)*100);
    memset(EffOthers, 0, sizeof(int*)*100);
    return success;
}

status DeletePokemon(P* pokemon)
{
    if (pokemon == NULL)
        return failure;
    pokemon->type->numberOfTypes --;
    free(pokemon->Name);
    pokemon->Name = NULL;
    free(pokemon->Species);
    pokemon->Species = NULL;
    free(pokemon);
    pokemon = NULL;
    return success;
}

status DeletePokemonType(PT* type)
{
    if (type == NULL)
        return failure;
    free(type->TypeName);
    type->TypeName = NULL;
    free(type);
    type = NULL;
    return success;
}

int getAttack(P* pokemonFound, P* pokemonInput, int* atk1, int* atk2)
{
    int flagF = -1;
    int flagI = -1;

    /////////// #1 ///////////
    for (int i = 0; i < pokemonFound->type->SizeMe; ++i)
    {
        if (strcmp(pokemonFound->type->effective_against_me[i]->TypeName, pokemonInput->type->TypeName) == 0)
        {
            flagF = i;
            break;
        }
    }
    for (int i = 0; i < pokemonFound->type->SizeOther; ++i)
    {
        if (strcmp(pokemonFound->type->effective_against_others[i]->TypeName, pokemonInput->type->TypeName) == 0)
        {
            if (flagF == -1)
            {
                flagI = i;
                break;
            }
        }
    }


    //////// #2 /////////////
    for (int i = 0; i < pokemonInput->type->SizeMe; ++i)
    {
        if (strcmp(pokemonInput->type->effective_against_me[i]->TypeName, pokemonFound->type->TypeName) == 0)
        {
            if (flagF == -1 && flagI == -1)
            {
                flagF = i;
                break;
            }
        }
    }
    for (int i = 0; i < pokemonInput->type->SizeOther; ++i)
    {
        if (strcmp(pokemonInput->type->effective_against_others[i]->TypeName, pokemonFound->type->TypeName) == 0)
        {
            if (flagF == -1 && flagI == -1)
            {
                flagI = i;
                break;
            }

        }
    }



    *atk1 = pokemonFound->info.Attack;
    *atk2 = pokemonInput->info.Attack;
    if (flagF != -1)
        return ((*atk1 - 10) - *atk2);

    if (flagI != -1)
        return (*atk1 - (*atk2 - 10));

    return (*atk1 - *atk2);
}

char* getCat (P* pokemon)
{
    return pokemon->type->TypeName;
}

P* copyFunction_returnsShellCopyOfPokemon(P* pokemon){
    return pokemon;
}

int isEqualFunction_IfTwoPokemonsHaveTheSameAtk(P *pokemon1, P *pokemon2){
    if(pokemon1 == NULL || pokemon2 == NULL)
        return -2;

    if (pokemon1->info.Attack > pokemon2->info.Attack){
        return 1;
    }
    if (pokemon1->info.Attack < pokemon2->info.Attack){
        return -1;
    }
    else
        return 0;
}

int HelpPrint(char* TempType1 ,int NumOfTypes, PT** MasterListTypes)
{
    int TempIndex = -2;
    for (int i = 0; i < NumOfTypes; ++i)
    {
        if (strcmp(TempType1, MasterListTypes[i]->TypeName) == 0)
        {
            TempIndex = i;
        }
    }
    return TempIndex;
}
