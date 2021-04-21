#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>  // Pour printf, scanf, getchar,
#include <string.h> // Pour strlen, strcmp,
#include <stdlib.h> // Pour EXIT_SUCCESS, EXIT_FAILLURE,
#include <time.h>   // Pour localtime, time_t, struct tm;
#include "../include/include.h"
/***
  Fonction listeVide retourne 1 si la liste est vide, 0 sinon
  **/
int listeVide(LIGNE *maLigne)
{
  if (maLigne->premiereStation == NULL)
    return 1; //liste Vide
  else
    return 0;
}
/***
  Fonction qui lit les donn�es d'une station dans la ligne de m�tro
  On lit une ligne d'un fichier, on utilise strtok pour retourner les chaines de caract�res avant ":"
  puis on convertit cette chaine quand il le faut et on le stocke dans une structure qu'on retourne
  **/
STATION *getStation(FILE *file)
{
  STATION *station = (STATION *)calloc(1, sizeof(STATION));
  if (file == NULL)
    printf("Impossible d'ouvrir le fichier metro.csv");
  else
  {
    char *str = (char *)calloc(SIZE, sizeof(char));
    int n = 0, i = 0;
    while (n < 1)
    {
      fgets(str, SIZE, file);
      char *ptr = strtok(str, ":");
      while (ptr != NULL)
      {
        if (i == 0){
          station->numLigneStation = atoi(ptr); //printf("%d", station->numLigneStation);
        }
        else if (i == 2)
        {
          station->nomStation = (char *)calloc(strlen(ptr) + 1, sizeof(char));
          station->nomStation[strlen(ptr)] = '\0';
          strcpy(station->nomStation, converrtChaine(ptr));
        }
        else if (i == 3)
        {
          double heureDecimale = atof(ptr);
          int heure = (int)heureDecimale + 0.00000001; //on ajoute cette valeur pour arrondir
          int minute = (heureDecimale - heure) * 100.0;
          station->premierTrain.heure = heure;
          station->premierTrain.minute = minute;
        }
        else if (i == 4)
        {
          double heureDecimale = atof(ptr);
          int heure = (int)heureDecimale;
          int minute = (heureDecimale - heure) * 100.0;
          station->dernierTrain.heure = heure;
          station->dernierTrain.minute = minute;
        }
        i++;
        ptr = strtok(NULL, ":");
      }
      n++;
    }
  }
  return station;
}
/***
  Fonction qui supprime � partir de la t�te de la ligne
  **/
STATION *removeTailFromList(LIGNE *maLigne)
{
  //initialisation � 0
  STATION *stationToRemove = NULL;
  if (listeVide(maLigne))
    printf("Impossible de supprimer une liste vide !");
  else
  {
    stationToRemove = maLigne->derniereStation;
    maLigne->derniereStation = maLigne->derniereStation->prec;
    if (listeVide(maLigne))
    {
      maLigne->premiereStation = NULL;
      maLigne->derniereStation = NULL;
    }
    else
      maLigne->derniereStation->suiv = NULL;
  }
  return stationToRemove;
}
/***
  Fonction qui supprime � partir de la queue de la ligne
  **/
STATION *removeHeadFromList(LIGNE *maLigne)
{
  //initialisation � 0
  STATION *stationToRemove = NULL;
  if (listeVide(maLigne))
    printf("Impossible d'enlever au debut si la liste est vide ");
  else
  {
    stationToRemove = maLigne->premiereStation;
    maLigne->premiereStation = maLigne->premiereStation->suiv;
    maLigne->premiereStation->prec = NULL;
  }
  return stationToRemove;
}
/***
  Fonction ajoute une station � partir de la queue de la ligne
  **/
void addTailLigne(LIGNE *ligne, STATION nouvelleStation)
{
  STATION *station = (STATION *)calloc(1, sizeof(STATION));
  //on initialise les champs de station par les valeurs pass�es en param�tre
  station->nomStation = malloc(SIZE * sizeof(char));
  strcpy(station->nomStation, nouvelleStation.nomStation);
  station->premierTrain.heure = nouvelleStation.premierTrain.heure;
  station->premierTrain.minute = nouvelleStation.premierTrain.minute;
  station->dernierTrain.heure = nouvelleStation.dernierTrain.heure;
  station->dernierTrain.minute = nouvelleStation.dernierTrain.minute;
  station->tempsstationSuiv = nouvelleStation.tempsstationSuiv;
  station->poids = nouvelleStation.poids;
  station->parent = nouvelleStation.parent;
  station->numLigneStation = nouvelleStation.numLigneStation;
  if (listeVide(ligne))
  {
    ligne->premiereStation = station;
    ligne->derniereStation = station;
  }
  else
  {
    ligne->derniereStation->suiv = station;
    station->prec = ligne->derniereStation;
    ligne->derniereStation = station;
  }
}
/***
  Fonction ajoute une station � partir de la tete de ligne
  **/

void addHeadLigne(LIGNE *ligne, STATION *station)
{
  if (listeVide(ligne))
  {
    ligne->premiereStation = station;
    ligne->derniereStation = station;
  }
  else
  {
    ligne->premiereStation->prec = station;
    station->suiv = ligne->premiereStation;
    ligne->premiereStation = station;
  }
}
/***
  Fonction qui donne le nombre de saut � la ligne entre 2 lignes de m�tro en
  lisant le fichier et en utilisant les d�limiteurs qui identifient les lignes
  arguments: la ligne, le fichier, les delimiteurs du debut et de la fin de ligne
  **/
int nbjumbLigne(FILE *pFile, char *ligneStart, char *ligneEnd)
{
  int nbSizeOfLine = 0, startposition = 0;
  char *str = (char *)calloc(SIZE, sizeof(char));

  // On calcule le nombre de lignes entre ####i et ####j
  fgets(str, SIZE, pFile);
  startposition = ftell(pFile);
  char *ptr = strtok(str, ":");
  if (strcmp(ligneStart, ptr) != 0)
  {
    do
    {
      fgets(str, SIZE, pFile);
      ptr = strtok(str, ":");
    } while (strcmp(ligneStart, ptr) != 0);
    startposition = ftell(pFile); //on sauvegarde la position � la quelle on s'est deplace
  }
  if (strcmp(ligneStart, ptr) == 0)
    do
    {
      fgets(str, SIZE, pFile);
      ptr = strtok(str, ":");
      nbSizeOfLine++;
    } while (strcmp(ligneEnd, ptr) != 0);
  fseek(pFile, startposition, SEEK_SET); // on remet le curseur au niveau du ####i de d�part pour commencer � afficher � partir de cette position
  return nbSizeOfLine;
}
/***
  Fonction qui permet de remplir une ligne de m�tro avec les stations qui correspondent
  arguments : la ligne, le fichier, le debut et la fin de la ligne
  **/
void loadStationInLigne(LIGNE *pLigne, FILE *pFile, char *ligneStart, char *ligneEnd)
{
  int i = 0, nbSizeOfLine;
  STATION *stations = (STATION *)calloc(SIZE, sizeof(STATION));
  nbSizeOfLine = nbjumbLigne(pFile, ligneStart, ligneEnd);
  if (pFile == NULL)
    printf("Erreur d'ouverture du fichier");
  else
  {
    while (i < nbSizeOfLine - 1)
    {
      stations[i] = *getStation(pFile);
      addTailLigne(pLigne, stations[i]);
      i++;
    }
    // On renseigne le numero de la ligne
    if (ligneStart == "####1")
      pLigne->numLigne = 1;
    else if (ligneStart == "####2")
      pLigne->numLigne = 2;
    else if (ligneStart == "####3" )
      pLigne->numLigne = 3;
    else if (ligneStart == "####4")
      pLigne->numLigne = 4;
    else 
      pLigne->numLigne = 5;
  }
}
/***
  Fonction qui insert une station dans la ligne en fonction du poids
  **/
void insert(LIGNE *pLigne, STATION *station)
{
  STATION *vParcours = pLigne->premiereStation;
  vParcours = pLigne->premiereStation;
  if (pLigne->premiereStation == NULL)
  {
    station->suiv = NULL;
    pLigne->premiereStation = station;
    pLigne->derniereStation = station;
  }
  while (vParcours != NULL)
  {
    if (station->poids < vParcours->poids)
    {
      if (vParcours == pLigne->premiereStation)
      {
        vParcours->prec = station;
        pLigne->premiereStation = station;
        station->suiv = vParcours;
        break;
      }
      else
      {
        station->prec = vParcours->prec;
        vParcours->prec->suiv = station;
        vParcours->prec = station;
        station->suiv = vParcours;
        break;
      }
    }
    vParcours = vParcours->suiv;
  }
  vParcours = pLigne->derniereStation;
  if (station->poids >= vParcours->poids)
    addTailLigne(pLigne, *station);
}

/***
  Fonction qui supprime une station � partir de n'importe quelle position
  dans la ligne puis retourne cette station
  **/
STATION *removeStationX(LIGNE *ligne, char *nomStation)
{
  STATION *stationToRemove = NULL;
  if (listeVide(ligne))
    printf("On ne peux retirer d'une quelconque position si la ligne est vide !");
  else
  {
    // Liste Update
    if (!strcmp(ligne->premiereStation->nomStation, nomStation))
      stationToRemove = removeHeadFromList(ligne);
    else if (!strcmp(ligne->derniereStation->nomStation, nomStation))
      stationToRemove = removeTailFromList(ligne);
    else
    {
      stationToRemove = ligne->premiereStation->suiv;
      while (stationToRemove != ligne->derniereStation && strcmp(stationToRemove->nomStation, nomStation))
        stationToRemove = stationToRemove->suiv;

      stationToRemove->prec->suiv = stationToRemove->suiv;
      stationToRemove->suiv->prec = stationToRemove->prec;
    }
  }
  return stationToRemove;
}

/***
  Fonction qui permet de compter toutes les stations d'une ligne
  **/
int nombreDelement(LIGNE *ligne)
{
  int n = 0;
  if (listeVide(ligne))
    n = 0;
  else
  {
    STATION *station;
    station = ligne->premiereStation;
    while (station != NULL)
    {
      n++;
      station = station->suiv;
    }
  }
  return n;
}
/***
  Fonction qui permet de remplir un tableau de ligne qui correspond au
  graphe et d'initialiser les temps pour aller aux stations suivantes
  arguments : un tableau de ligne et un fichier
  **/
void loadGraphe(LIGNE metro[], FILE *pFile)
{
  //On charge toutes les stations
  loadStationInLigne(&metro[0], pFile, "####1", "####1");
  loadStationInLigne(&metro[1], pFile, "####2", "####2");
  loadStationInLigne(&metro[2], pFile, "####3", "####3");
  loadStationInLigne(&metro[3], pFile, "####4", "####4");
  loadStationInLigne(&metro[4], pFile, "####5", "####5");
  //on initialise pour chaque station le temps pour aller � la station suivante
  //et on initialise le num�ro de la ligne
  for (int j = 0; j < NombreLigne; j++)
    initialiseTemps(metro[j]);
}

/***
  Fonction qui permet de remplir une ligne contenant de toutes les stations possibles
  contenu dans le fichier metro.csv sans rep�tition (la reserve)
  **/
LIGNE reserve(FILE *file)
{
  file = fopen("tram.txt", "r");
  LIGNE reserve = {NULL, NULL, 0};
  //on charge toutes les lignes de metro sens a
  loadStationInLigne(&reserve, file, "####1", "####1");
  loadStationInLigne(&reserve, file, "####2", "####2");
  loadStationInLigne(&reserve, file, "####3", "####3");
  loadStationInLigne(&reserve, file, "####4", "####4");
  loadStationInLigne(&reserve, file, "####5", "####5");
  reserve.numLigne = 0;
  STATION *position = reserve.premiereStation;
  STATION *vparcours = reserve.derniereStation;
  //on verifie si une ligne apparait deux fois on la supprime
  while (position != NULL)
  {
    while (vparcours != position && vparcours != reserve.premiereStation)
    {
      if (!strcmp(position->nomStation, vparcours->nomStation))
        removeStationX(&reserve, vparcours->nomStation);
      vparcours = vparcours->prec;
    }
    vparcours = reserve.derniereStation;
    position = position->suiv;
  }
  return reserve;
}

/***
  Fonction qui permet de convertir des heures en minutes
  **/
int convertHeureToMin(temps time)
{
  return ((time.heure * 60) + time.minute);
}

/***
  Fonction qui permet d'initialiser le champs tempsstationSuiv qu'il
  faut pour aller � la station suivante lorsqu'on est sur une ligne
  **/
void initialiseTemps(LIGNE maLigne)
{
  STATION *vparcour = maLigne.premiereStation;
  temps time2, time1;
  while (vparcour != maLigne.derniereStation)
  {
    time2.heure = vparcour->suiv->dernierTrain.heure - vparcour->dernierTrain.heure;
    time2.minute = vparcour->suiv->dernierTrain.minute - vparcour->dernierTrain.minute;
    vparcour->tempsstationSuiv = convertHeureToMin(time2);
    if (vparcour->tempsstationSuiv == 0)
      vparcour->tempsstationSuiv = 1; //correction de l'erreur
    vparcour = vparcour->suiv;
  }

  vparcour = maLigne.derniereStation;
  while (vparcour != maLigne.premiereStation)
  {
    time1.heure = vparcour->dernierTrain.heure - vparcour->prec->dernierTrain.heure;
    time1.minute = vparcour->dernierTrain.minute - vparcour->prec->dernierTrain.minute;

    vparcour->tempsstationPrec = convertHeureToMin(time1);
    if (vparcour->tempsstationPrec == 0)
      vparcour->tempsstationPrec = 1; //correction de l'erreur
    vparcour = vparcour->prec;
  }

  maLigne.premiereStation->tempsstationPrec = 0;
  maLigne.derniereStation->tempsstationSuiv = 0;
}
/***
  Fonction qui permet de trouver une station dans une ligne
  return 1 si la station est presente, 0 sinon
  **/
int findStationInLigne(LIGNE ligne, char *nomStation)
{
  STATION *vparcour = ligne.premiereStation;
  while (vparcour->suiv != NULL)
  {
    if (strcmp(vparcour->nomStation, nomStation) == 0)
      return 1;
    vparcour = vparcour->suiv;
  }
  return 0;
}

/***
  Fonction qui convertit la chaine saisie en majuscule s'il y a des espaces, apostrophes, des 
  virgules ou des tirets de 8, on les laisse intacte, cela assure la reconnaissance de la station 
  **/
char *converrtChaine(char *string)
{
  int stringSize = strlen(string), i;
  char *newString = (char*)calloc(stringSize + 1, sizeof(char));
  for (i = 0; i < stringSize; i++)
  {
    // les noms des stations ne contiennent que ces 4 caracteres speciaux on les gère ici
    if (string[i] == ' ' || string[i] == '\'' || string[i] == ',' || string[i] == '-')
      newString[i] = string[i];

    //conversion des minuscules en majuscules
    if (string[i] <= 'z' && string[i] >= 'a')
      newString[i] = string[i] - 32;
    else
      newString[i] = string[i];
  }
  return newString;
}
