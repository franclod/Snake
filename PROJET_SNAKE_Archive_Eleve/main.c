#include <stdio.h>
#include "myLib.h"
#include "mesTypes.h"
#include "SP_GESTION_JEU.h"
#include "SP_CONFIGURATION.h"




int main()
{
    ST_PARAM_JEU parametre;                                 //Cr�ation des param�tres du jeu
    ST_SNAKE snake;                                         //Cr�ation des param�tres du serpent
    ST_JOUEUR joueur;                                       //Cr�ation des param�tres du joueur

    SP_INIT_PARAMETRE_JEU(&parametre,&snake);               //Initialisation des param�tres
    SP_INIT_FICHIER();                                      //Initialisation du fichier contenant les scores

    SP_ACCUEIL(&joueur,&parametre,&snake);                  //Lancement du programme permettant de choisir le nom du joueur
    return 0;

}
