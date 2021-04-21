#define SIZE 1700         // Nombre de ligne du fichier metro.csv
#define NombreLigne 5  // Nombre de ligne de metro
#define INFINI 100000     // Valeur tr�s grande utilis� dans shortestPath

typedef struct temps
{
  int heure;
  int minute;
} temps;

/* structure representant une station ie 
un arret donn� contenu dans une ligne*/
typedef struct _STATION
{
  char *nomStation;
  struct _STATION *suiv;
  struct _STATION *prec;
  temps premierTrain;
  temps dernierTrain;

  /* champ  sup pour mesurer le temps 
  entre deux station avec DJIKSTRA */
  double poids;
  struct _STATION *parent;
  int tempsstationSuiv;    // le temps qu'il faut pour aller � la prochaine station
  int tempsstationPrec;   // le temps qu'il faut pour aller � la station precedente
  int numLigneStation;   // la ligne sur laquelle se trouve la station

} STATION;

/*structure representant une ligne */
typedef struct _LIGNE
{
  STATION *premiereStation;
  STATION *derniereStation;
  int numLigne;   
} LIGNE;

/*Declaration des fonctionnamlit�es suppl�mentaires*/

void printStation(STATION* station);
void printLigne(LIGNE * maLigne);
void initialiseTemps(LIGNE maLigne);
void addHeadLigne(LIGNE *ligne, STATION *station);
void addTailLigne(LIGNE *ligne, STATION nouvelleStation);
void loadStationInLigne(LIGNE *pLigne, FILE *pFile, char *ligneStart, char *ligneEnd);
void insert(LIGNE *pLigne, STATION *station);
void loadGraphe(LIGNE metro[], FILE *pFile);
void printPath(LIGNE *pLigne);
void handleCorrespondance(LIGNE *shortestPath);
void keyBordInteraction(FILE *pFile);  // Menu d'interaction avec l'utilisateur
int nbjumbLigne(FILE *pFile, char *ligneStart, char *ligneEnd);
int nombreDelement(LIGNE *ligne);
int listeVide(LIGNE* maLigne);
int convertHeureToMin(temps time);
int findStationInLigne(LIGNE ligne, char *nomStation);
char *converrtChaine(char *string);
STATION *removeTailFromList(LIGNE *maLigne);
STATION *removeHeadFromList(LIGNE *maLigne);
STATION *removeStationX(LIGNE *ligne, char* nomStation);
STATION *getStation(FILE *file);
LIGNE reserve(FILE *file);
LIGNE *shortestPath(LIGNE metro[], LIGNE reserve, char *stationDepart, char *stationDarrivee);

