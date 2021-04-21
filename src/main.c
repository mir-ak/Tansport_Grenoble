#include <stdio.h>  // Pour printf, scanf, getchar,
#include <string.h> // Pour strlen, strcmp,
#include <stdlib.h> // Pour EXIT_SUCCESS, EXIT_FAILLURE,
#include <time.h>   // Pour localtime, time_t, struct tm;
#include "../include/include.h"
/***
Fonction qui prend en param�tre une station (maillon d'une liste) et l'affiche
--> Situation : Reste des modifications � ajouter pour afficher toutes les informations necessaires
**/
void changeName(LIGNE *pLigne){
    if(listeVide(pLigne))
        printf("erreur");
    else
    {
     if (pLigne->premiereStation->numLigneStation == 1)
     {
         printf("A \n|\n|");
     }
     if (pLigne->premiereStation->numLigneStation == 2)
     {
         printf("B \n|\n|");
     }
     if (pLigne->premiereStation->numLigneStation == 3)
     {
         printf("C \n|\n|");
     }
     if (pLigne->premiereStation->numLigneStation == 4)
     {
         printf("D \n|\n|");
     }
     if (pLigne->premiereStation->numLigneStation == 5)
     {
         printf("E \n|\n|");
     }
    }   
}
void printStation(STATION *station)
{
    printf("%s(poid : %lg)", station->nomStation, station->poids);
}

/***
Fonction qui prend en param�tre une ligne (liste) et l'affiche
**/
void printLigne(LIGNE *maLigne)
{
    if (listeVide(maLigne))
        printf("Ligne inexistant\n");
    else
    {
        STATION *station;
        station = maLigne->premiereStation;
        while (station != maLigne->derniereStation)
        {
            printStation(station);
            printf("-->");
            station = station->suiv;
        }
        printStation(maLigne->derniereStation);
    }
}
/***
Fonction qui affichera le trajet � suivre dans la console
**/
void printPath(LIGNE *pLigne)
{
    // temps du syst�me en utilisant time.h
    time_t temps;
    time(&temps);
    struct tm *tempsPC = localtime(&temps);
    printf(" -> Station de depart : %s à %d:%d ", pLigne->premiereStation->nomStation,tempsPC->tm_hour, tempsPC->tm_min);
    printf("On vous propose  l'itineraire suivant (%lg min): ", pLigne->derniereStation->poids);
    // Si la station de depart est la meme que la station darrive on le siganale et arrete le programme
    if (strcmp(pLigne->premiereStation->nomStation, pLigne->derniereStation->nomStation) == 0)
    {
        printf("|Arrive = Depart ");
        getchar();
        exit(EXIT_SUCCESS);
    }
    printf("| --> %d:%d depuis la station %s : prendre la ligne %d\n|\n|", tempsPC->tm_hour, tempsPC->tm_min, pLigne->premiereStation->nomStation, pLigne->premiereStation->numLigneStation);
    STATION *vparcours = pLigne->premiereStation->suiv;
    printf("  # %s \n|", vparcours->nomStation);
    while (vparcours->suiv != NULL)
    {
        if (vparcours->suiv->numLigneStation == vparcours->numLigneStation )
            printf("  # %s \n|", vparcours->suiv->nomStation);
        else
        {
            printf("\n| --> %d:%d Changement de ligne :\n|\tAller sur la ligne %d depuis la station %s\n|\n|", tempsPC->tm_hour, (tempsPC->tm_min + (int)vparcours->poids), vparcours->suiv->numLigneStation, vparcours->nomStation);
            printf("  # %s \n|", vparcours->suiv->nomStation);
        }
        vparcours = vparcours->suiv;
    }
    //printf("  # %s \n|", pLigne->derniereStation->nomStation);
    printf("\n+ --> %d:%d Arrive a : %s\n", tempsPC->tm_hour, (tempsPC->tm_min + (int)vparcours->poids), pLigne->derniereStation->nomStation);
    printf("+ - - - - - - - - - - - - - - - - - - -  - - - - - - - - - - - - - - - - - - - +\n");
}
/***
Fonction qui permet d'interagir avec l'utilsateur en prenant compte tout
ce que l'utilisateur pourra saisir ou presque qui fera planter le programme
**/
void keyBordInteraction(FILE *pFile)
{
    char choix;
    int tmp = 0;
    //FILE *pFile = fopen("tram.txt", "r"); //ouverture du fichier metro.csv en mode lecture
    if (pFile == NULL)
        printf("Impossible d'ouvrir le fichier .txt dans keyboardInteraction");
    else
    {
        //test Djikstra
        LIGNE metro[NombreLigne] = {NULL, NULL, 0};
        loadGraphe(metro, pFile);
        char *stationDepart = (char *)calloc(1, sizeof(char)), *stationDarrive = (char *)calloc(1, sizeof(char));
        do
        {
            printf("Tapez 1 pour lancer la recherche d'itineraire, 0 pour quittez : ");
            scanf("%s",&choix);
        }while (choix != '1' && choix != '0');

        if (choix == '0')
        {
            printf("Au Revoir \n");
            exit(EXIT_SUCCESS);
        }
        do
        {
            LIGNE laReserve = reserve(pFile);
            do
            { 
                if (tmp == 0)
                {
                    printf("Entrez la station de depart : ");
                }
                tmp = 1 ;
                // le buffer contient '\n' donc le programme va sauter la ligne du fgets
                fseek(stdin, 0, SEEK_END); //on retourne au debut du stdin pour eviter de sauter l'appel � fgets
                fgets(stationDepart, SIZE, stdin);
                strtok(stationDepart, "\n");                   //fgets rajoute \n aux donn�es saisies : donc on supprime le \n
                stationDepart = converrtChaine(stationDepart); //la station seras a coup sure reconnue sauf si elle n'existe pas
                
            } while (!findStationInLigne(laReserve, stationDepart));
            do
            {
                printf("Entrez la station d'arrive  : ");
                fseek(stdin, 0, SEEK_END);
                fgets(stationDarrive, SIZE, stdin);
                strtok(stationDarrive, "\n");
                stationDarrive = converrtChaine(stationDarrive); //la station seras a coup sure reconnue sauf si elle n'existe pas
            } while (!findStationInLigne(laReserve, stationDarrive));

            LIGNE *path = shortestPath(metro, laReserve, stationDepart, stationDarrive);
            printPath(path);

            choix = '1';
            do
            {
                printf("Voulez Vous continuer? Tapez 1 pour relancer 0 pour quittez :  ");
                scanf("%s",&choix);
            } while (choix != '1' && choix != '0');

            if (choix == '0')
            {
                printf("Au Revoir \n");
                exit(EXIT_SUCCESS);
             }
        } while (choix == '1');
    }
    fclose(pFile);
}

int main(int argc, char *argv[])
{
    // Utilisation des arguments en ligne de commande
    if (argc != 2)
    {
        //Au debut le programme va rentrer ici obligatoirement vu qu'on pas donne les arguments
        printf("\nTapez windows + R, tapez cmd puis entrer pour ouvrir le cmd puis aller dans :\n %s\n", argv[0]);
        printf("\nAssurez vous que le dossier contient bien le fichier metro_modifiee.csv !");
        printf("\nPuis tapez ProjetMetro.exe pathFinder metro_modifiee.csv pour executer le programme\n");
        exit(1);
    }
    FILE *pFile = fopen(argv[1], "r");
    //FILE *pFile = fopen("metro_modifiee.csv", "r");
    if (pFile == NULL)
    {
        printf(" Erreur a l'ouverture de %s\n ", argv[1]);
        exit(1);
    }
    //interaction clavier
    keyBordInteraction(pFile);
    fclose(pFile);
    return EXIT_SUCCESS;
}
