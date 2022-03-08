//
// Created by Sean Hakmon on 10/12/2021.
//
#include "Pokemon.h"
#include "Defs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "BattleByCategory.h"

Battle ConfigureFile(int NumTypes, int CapacityOfTypes, char* path ,PT** MasterListTypes);
status DeletePokedex(Battle b, int NumOfTypes, PT** MasterListTypes);


int main(int argc, char* argv[])
{
    /////////////////////// Reading File ///////////////////////
    char* no1 = argv[1];
    char* no2 = argv[2];
    int NumOfTypes , Capacity;
    sscanf(no1, "%d" ,&NumOfTypes);
    sscanf(no2, "%d" ,&Capacity);
    PT* MasterListTypes[NumOfTypes];
    Battle b = ConfigureFile(NumOfTypes ,Capacity ,argv[3] ,MasterListTypes);
    if (b == NULL)
    {
        printf("No Memory available.\n");
        return 0;
    }
    ///////////////////////     MENU     ///////////////////////
    int Escape = 0, num;
    char temp[100];
    while (Escape != 6)
    {
        printf("Please choose one of the following numbers:\n");
        printf("1 : Print all Pokemons by types\n");
        printf("2 : Print all Pokemons types\n");
        printf("3 : Insert Pokemon to battles training camp\n");
        printf("4 : Remove strongest Pokemon by type\n");
        printf("5 : Fight\n");
        printf("6 : Exit\n");


        int atk = 0, flag, Checker = 0;
        float hgt = 0, wgt = 0;
        char name[300],type[300], spc[300], typeOut[300];

        scanf("%s", temp);
        num = atoi(temp);
        switch (num)
        {
            case 1:
                displayObjectsByCategories(b);
                break;
                case 2:
                    for (int i = 0; i < NumOfTypes; ++i)
                    {
                        PrintPokemonType(MasterListTypes[i]);
                    }
                    break;
                    case 3:
                        printf("Please enter Pokemon type name:\n");
                        scanf("%s", type);

                        flag = HelpPrint(type, NumOfTypes, MasterListTypes);
                        if (flag == -2)
                        {
                            printf("Type name doesn't exist.\n");
                            break;
                        }

                        if (getNumberOfObjectsInCategory(b, MasterListTypes[flag]->TypeName) == Capacity)
                        {
                            printf("Type at full capacity.\n");
                            break;
                        }

                        printf("Please enter Pokemon name:\n");
                        scanf("%s", name);

                        printf("Please enter Pokemon species:\n");
                        scanf("%s", spc);

                        memset(temp, 0 , sizeof(temp));
                        printf("Please enter Pokemon height:\n");
                        scanf("%f", &hgt);

                        memset(temp, 0 , sizeof(temp));
                        printf("Please enter Pokemon weight:\n");
                        scanf("%f", &wgt);


                        memset(temp, 0 , sizeof(temp));
                        printf("Please enter Pokemon attack:\n");
                        scanf("%d", &atk);
                        //                atk = atoi(temp);

                        P* Pokemon = CreatePokemon(name, spc, hgt, wgt, atk, MasterListTypes[flag]);
                        insertObject(b, Pokemon);
                        printf("The Pokemon was successfully added.\n");
                        PrintPokemon(Pokemon);


                        //                if (sean == failure)
                        //                    printf("Type at full capacity.\n");

                        memset(type, 0, sizeof(type));
                        //                DeletePokemon(Pokemon);
                        break;

                        case 4:
                            printf("Please enter type name:\n");
                            scanf("%s", typeOut);

                            flag = HelpPrint(typeOut, NumOfTypes, MasterListTypes);
                            if (flag == -2)
                            {
                                printf("Type name doesn't exist.\n");
                                break;
                            }
                            else
                            {
                                if (MasterListTypes[flag]->numberOfTypes == 0)
                                {
                                    printf("There is no Pokemon to remove.\n");
                                    break;
                                }
                                else
                                {
                                    //                        MasterListTypes[flag]->numberOfTypes --;
                                    P* PokemonOut = removeMaxByCategory(b,MasterListTypes[flag]->TypeName);
                                    printf("The strongest Pokemon was removed:\n");
                                    PrintPokemon(PokemonOut);
                                    DeletePokemon(PokemonOut);
                                }
                            }
                            memset(typeOut, 0, sizeof(typeOut));
                            break;

                            case 5:
                                printf("Please enter Pokemon type name:\n");
                                scanf("%s", type);

                                flag = HelpPrint(type, NumOfTypes, MasterListTypes);
                                if (flag == -2)
                                {
                                    printf("Type name doesn't exist.\n");
                                    break;
                                }

                                printf("Please enter Pokemon name:\n");
                                scanf("%s", name);

                                printf("Please enter Pokemon species:\n");
                                scanf("%s", spc);


                                printf("Please enter Pokemon height:\n");
                                scanf("%f", &hgt);


                                printf("Please enter Pokemon weight:\n");
                                scanf("%f", &wgt);


                                printf("Please enter Pokemon attack:\n");
                                scanf("%d", &atk);

                                P* TempPokemon = CreatePokemon(name, spc, hgt, wgt, atk, MasterListTypes[flag]);
                                fight(b,TempPokemon);
                                DeletePokemon(TempPokemon);
                                break;

                                case 6:
                                    Escape = 6;
                                    DeletePokedex(b, NumOfTypes ,MasterListTypes);
                                    printf("All the memory cleaned and the program is safely closed.\n");
                                    break;
                                    default:
                                        printf("Please choose a valid number.\n");
                                        break;
        }
    }



    return 0;
}


Battle ConfigureFile(int NumTypes, int CapacityOfTypes, char* path ,PT** MasterListTypes)
{
    FILE* f;
    char buffer[300];
    int Capacity = CapacityOfTypes, NumOfTypes = NumTypes;

    Battle b;

    /// TODO Opening The GivenFile;
    f = fopen(path, "r");
    int i = 0;
    int TypeIndex = 0;
    char* p;
    char* s1 = ",";

    fgets(buffer, 300, f); // Types
    fgets(buffer, 300, f); // Fire,Electric,Grass, etc...
    strtok(buffer, "\r\n");
    char TempInfo[300];
//    char* TempType;
    strcpy(TempInfo, buffer);

    b = createBattleByCategory
            (Capacity, NumOfTypes, TempInfo,(equalFunction) isEqualFunction_IfTwoPokemonsHaveTheSameAtk,
             (copyFunction) copyFunction_returnsShellCopyOfPokemon,(freeFunction) DeletePokemon,
             (getCategoryFunction)getCat,(getAttackFunction) getAttack,(printFunction) PrintPokemon);


    //    while (fgets(buffer,300,f))
    //    {
    //        if (i == 1)
    //        {

        if(strcmp(buffer, "Pokemons\r\n") == 0)
            return NULL;
        p = strstr(buffer, s1);
        if (p)
        {
            for (int j = 0; j < NumOfTypes; ++j)
            {
                char* TempType = strtok(buffer, ",");
                while (TempType != NULL)
                {
                    MasterListTypes[TypeIndex] = CreateType(TempType);
                    if (MasterListTypes[TypeIndex] == NULL)
                    {
                        DeletePokemonType(MasterListTypes[TypeIndex]);
                        printf("Memory Problem\r\n");
                    }
                    TempType = strtok(NULL, ",\r\n");
                    TypeIndex++;
                }
            }
        }
        else
        {
            char* TempType = strtok(buffer, "\r\n");
            MasterListTypes[TypeIndex] = CreateType(TempType);
        }

//    }
    //        }

    fgets(buffer, 300, f);
    int indexA = -1;
    char Type1[15];
    char AgainstList[40];
    //        if (i ==2 )
    //        {
    while (strcmp(buffer,"Pokemons\r\n") != 0)
    {
        char* TempType2 = strtok(buffer, "\t");
        sscanf(TempType2 ,"%s %s", Type1, AgainstList);
        for (int j = 0; j < NumOfTypes; ++j)
        {
            if (strcmp(Type1,MasterListTypes[j]->TypeName) == 0)
            {
                indexA = j;
            }
        }
        if (indexA == -1)
        {
            return NULL;
        }
        char* TempType3 = strtok(AgainstList, ":");
        while (TempType3 != NULL)
        {
            TempType3 = strtok(NULL, ":\r\n");
            if (TempType3 != NULL)
            {
                char* TempType4 = strtok(TempType3, ",");
                while (TempType4 != NULL)
                {
                    for (int j = 0; j < NumOfTypes; ++j)
                    {
                        if (strcmp(TempType4,MasterListTypes[j]->TypeName) == 0)
                        {
                            if (AgainstList[18] == 'm')
                            {
                                AddTypeMe(MasterListTypes[indexA],MasterListTypes[j]);
                            }
                            if (AgainstList[18] == 'o')
                            {
                                AddTypeOther(MasterListTypes[indexA],MasterListTypes[j]);
                            }
                        }
                    }
                    TempType4 = strtok(NULL, ",\r\n");
                }
            }
        }
        fgets(buffer,300,f);
        //        i++;
    }
    //        }
    char* PokeType;
    char* FinalSpc;
    char* FinalName;
    int FinalAtk, indexB;
    float FinalH, FinalW;
    char* Sep = NULL;
    char row[300];

    if ((strcmp(buffer,"Pokemons\r\n") == 0))
    {
        //            i++;
        strcpy(row, buffer);
        fgets(buffer, 300, f);
    }

    while (strcmp(buffer, row) != 0)
    {
        Sep = strtok(buffer, ",");
        FinalName = Sep;

        Sep = strtok(NULL, ",");
        FinalSpc = Sep;

        Sep = strtok(NULL, ",");
        FinalH = atof(Sep);

        Sep = strtok(NULL, ",");
        FinalW = atof(Sep);

        Sep = strtok(NULL, ",");
        FinalAtk = (int)atof(Sep);

        Sep = strtok(NULL, ",\r\n");
        PokeType = Sep;

        for (int j = 0; j < NumOfTypes; ++j)
        {
            if (strcmp(PokeType,MasterListTypes[j]->TypeName) == 0)
            {
                indexB = j;
            }
        }
        if (indexB == -1)
        {
            return NULL;
        }
        P* Pokemon = CreatePokemon(FinalName, FinalSpc, FinalH, FinalW, FinalAtk, MasterListTypes[indexB]);
        if (insertObject(b, Pokemon) == failure)
        {
            DeletePokemon(Pokemon);
        }

        strcpy(row, buffer);
        fgets(buffer, 300, f);
    }
    fclose(f);
    return b;
}




status DeletePokedex(Battle b, int NumOfTypes, PT** MasterListTypes)
{
        for (int i = 0; i < NumOfTypes; ++i)
        {
            free(MasterListTypes[i]->effective_against_me);
            free(MasterListTypes[i]->effective_against_others);
        }
        destroyBattleByCategory(b);
        for (int i = 0; i < NumOfTypes; ++i)
        {
            DeletePokemonType(MasterListTypes[i]);
        }
        MasterListTypes = NULL;
        return success;
}

