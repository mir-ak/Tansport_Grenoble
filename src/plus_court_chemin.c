#include <stdio.h>              // Pour printf, scanf, getchar, 
#include <string.h>            // Pour strlen, strcmp,
#include <stdlib.h>           // Pour EXIT_SUCCESS, EXIT_FAILLURE, 
#include <time.h>            // Pour localtime, time_t, struct tm;
#include "../include/include.h"
/*** 
Fonction qui permet de trouver le plus court chemin en utilisant l'algorithme de Djikstra
arguments: le graphe, la reserve, le nom de la station de d�part et le nom de la station d'arriv�e
**/
LIGNE *shortestPath(LIGNE metro[], LIGNE Reserve, char *stationDepart, char *stationDarrivee)
{
    LIGNE itineraire = { NULL, NULL, 0 }, *itineraireOrdonne = (LIGNE*)calloc(1, sizeof(LIGNE));
    STATION *varParcour = Reserve.premiereStation;
    while (varParcour != NULL)
    {
        varParcour->parent = NULL;
        varParcour->poids = INFINI;
        varParcour = varParcour->suiv;
    }

    STATION *tmp = removeStationX(&Reserve, stationDepart);
    tmp->poids = 0.0;
    insert(&Reserve, tmp);

    STATION *varParcour1Reserve = (STATION*)calloc(1, sizeof(STATION));
    int arrive = 1;
    while ((listeVide(&Reserve) != 1) && arrive)
    { 
        varParcour1Reserve = removeHeadFromList(&Reserve);
        insert(&itineraire, varParcour1Reserve);
        if (!strcmp(stationDarrivee, varParcour1Reserve->nomStation))
        {
            STATION *varParcourAffichage = itineraire.derniereStation;
            while (varParcourAffichage != NULL)
            {
                addHeadLigne(itineraireOrdonne, varParcourAffichage);
                varParcourAffichage = varParcourAffichage->parent;
            }
            arrive = 0; //condition de sorti lorsque le chemin le plus court sera trouv�
        }
        //recherche de la station dans le graphe
        double tempsParcours = 0;
        for (int i = 0; i < NombreLigne; i++)
        {
            STATION *varParcour1metro = metro[i].premiereStation;
            STATION *varParcour2Reserve = NULL;
            STATION *toRemove2 = NULL;
            while (varParcour1metro->suiv != NULL)
            {
                if (!strcmp(varParcour1metro->nomStation, varParcour1Reserve->nomStation))
                {
                    tempsParcours = (double)varParcour1metro->tempsstationSuiv;
                    double nouveauPoids = varParcour1Reserve->poids + tempsParcours;

                    // Recherche de la station suivante dans la reserve
                    varParcour2Reserve = Reserve.premiereStation;
                    while (varParcour2Reserve != NULL)
                    {
                        if (!strcmp(varParcour2Reserve->nomStation, varParcour1metro->suiv->nomStation))
                            if (nouveauPoids < varParcour2Reserve->poids)
                            {
                                toRemove2 = removeStationX(&Reserve, varParcour2Reserve->nomStation);
                                toRemove2->poids = nouveauPoids;
                                toRemove2->parent = varParcour1Reserve;  //on donne a son parent l'adresse de la station ????
                                insert(&Reserve, toRemove2);
                            }
                        varParcour2Reserve = varParcour2Reserve->suiv;
                    }
                    // Dans le sens b
                    if (varParcour1metro->prec != NULL)
                    {
                        tempsParcours = (double)varParcour1metro->tempsstationPrec;
                        double nouveauPoids = varParcour1Reserve->poids + tempsParcours;
                        // Recherche de la station suivante dans la reserve
                        varParcour2Reserve = Reserve.premiereStation;
                        while (varParcour2Reserve != NULL)
                        {
                            if (!strcmp(varParcour2Reserve->nomStation, varParcour1metro->prec->nomStation))
                                if (nouveauPoids < varParcour2Reserve->poids)
                                {
                                    toRemove2 = removeStationX(&Reserve, varParcour2Reserve->nomStation);
                                    toRemove2->poids = nouveauPoids;
                                    toRemove2->parent = varParcour1Reserve;  //on donne a son parent l'adresse de la station ????
                                    insert(&Reserve, toRemove2);
                                }
                            varParcour2Reserve = varParcour2Reserve->suiv;
                        }
                    }
                }
                varParcour1metro = varParcour1metro->suiv;
            }
        }
    }
  return itineraireOrdonne;
}

