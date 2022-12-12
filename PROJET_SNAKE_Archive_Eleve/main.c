#include <stdio.h>
#include "myLib.h"
#include "mesTypes.h"
#include "SP_GESTION_JEU.h"
#include "SP_CONFIGURATION.h"




int main()
{
    ST_PARAM_JEU parametre;                                 //Création des paramètres du jeu
    ST_SNAKE snake;                                         //Création des paramètres du serpent
    ST_JOUEUR joueur;                                       //Création des paramètres du joueur

    SP_INIT_PARAMETRE_JEU(&parametre,&snake);               //Initialisation des paramètres
    SP_INIT_FICHIER();                                      //Initialisation du fichier contenant les scores

    SP_ACCUEIL(&joueur,&parametre,&snake);                  //Lancement du programme permettant de choisir le nom du joueur
    return 0;

}
