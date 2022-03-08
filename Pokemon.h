//
// Created by Sean Hakmon on 10/12/2021.
//

#ifndef HW3_POKEMON_H
#define HW3_POKEMON_H



typedef struct PokemonType PT;
typedef struct BIO BI;
typedef struct Pokemon P;
#include "Defs.h"
#include "BattleByCategory.h"

//typedef enum {AddSuccess, AddFail, DeleteSuccess, DeleteFail} StatResult;

struct PokemonType
        {
    //        PT* Psychic;
    char* TypeName;
    int numberOfTypes;
    PT** effective_against_me;
    PT** effective_against_others;
    int SizeOther;
    int SizeMe;
        };

struct BIO
        {
    float Height;
    float Weight;
    int Attack;
        };

struct Pokemon
        {
    char* Name;
    char* Species;
    BI info;
    PT* type;
        };



P* CreatePokemon(char* , char* , float, float , int, PT*);
PT* CreateType(char*);
BI  CreateBIO(float, float, int);



status AddTypeMe(PT*, PT*);
status AddTypeOther(PT* , PT*);
status DeleteTypeMe(PT*, char*);
status DeleteTypeOther(PT*, char*);
status PrintPokemon(P*);
status PrintPokemonType(PT*);
status DeletePokemonType(PT* type);
status DeletePokemon(P*);
char* getCat (P* pokemon);
int getAttack(P* pokemonFound, P* pokemonInput, int* atk1, int* atk2);



int HelpPrint(char* TempType1 ,int NumOfTypes, PT** MasterListTypes);
P* copyFunction_returnsShellCopyOfPokemon(P* pokemon);
int isEqualFunction_IfTwoPokemonsHaveTheSameAtk(P *pokemon1, P *pokemon2);










//bool searchFunction_IfPokemonHaveName(P *pokemon, char *name);
//int isEqualFunction_IfTwoPokemonsHaveTheSameName(P *pokemon1, char *name);
#endif //HW3_POKEMON_H
