#include <stdio.h>
#include <time.h>
#include "myLib.h"
#include "mesTypes.h"
#include "SP_CONFIGURATION.h"
#include "SP_GESTION_JEU.h"
#include <mmsystem.h>
#include <windows.h>
#pragma comment(lib,"winmm.lib")


void SP_INIT_PARAMETRE_JEU(ST_PARAM_JEU* p_parametre, ST_SNAKE* p_snake){
    /*Sous-programme permettant d'initialiser les paramètres du jeu à des valeurs par défaut*/
    p_parametre->couleur_snake= RED;            //Initialisation de la couleur du serpent en rouge
    p_parametre->couleur_stade= WHITE;          //Initialisation de la couleur du stade en blanc
    p_parametre->couleur_bordure= GREEN;        //Initialisation de la couleur des bordures en vert

    p_parametre->difficulte=2;                  //Initialisation de la difficulté à 2 (Moyen)
    p_parametre->H_stade=20;                    //Initialisation de la hauteur du stade à 20
    p_parametre->L_stade=50;                    //Initialisation de la largeur du stade à 50

    p_snake->taille=5;                          //Initialisation de la taille du serpent à 5
    p_snake->direction=0;                       //Initialisation de la direction du serpent à 0 (DROITE)
}

void SP_INIT_FICHIER(){
    /*Sous-programme verifiant l'existance d'un fichier texte pour les high score*/
    FILE* High_Score ;
    High_Score = fopen ("highscores.txt" , "r+") ;
    if (High_Score==NULL){
        //Condition si le fichier n'existe pas
        fclose ( High_Score );
        High_Score = fopen ("highscores.txt" , "wt") ;  //Créer le fichier texte
        fprintf (High_Score, "\tNom\t Score\n\n") ;     //Afficher les titres des colonnes (Nom et Score)
        fclose ( High_Score );                          //Fermé le fichier
    }
}

void SP_ACCUEIL(ST_JOUEUR* p_joueur, ST_PARAM_JEU* p_parametre,ST_SNAKE* p_snake){
    /*Sous-programme servant d'accueil*/
    cls();                              //Efface le terminal
    setColor(LIGHTCYAN);
    printf("\n\n\n\n\n\n\t\t\t\t\t******************************\n\t\t\t\t\t* Bienvenue dans notre SNAKE *\n\t\t\t\t\t******************************\n");
    printf("\t\t\t\t\t      Quel est ton nom ?\n");
    gotoxy(47,11);
    int test=1;

    //Boucle permettant de redemander le nom du joueur lorsqu'il est trop grand
    while (test){
        fflush(stdin);
        scanf("%s", &(p_joueur->nom));
        test=0;
        if(strlen(p_joueur->nom) < 21){
            //Condition si le taille du nom du joueur est inférieur à 21
            SP_MENU(p_joueur, p_parametre,p_snake);         //Retour au menu principal
        }
        else{
            //Condition de cas d'erreur
            printf("Le nom saisi est trop long, veuillez recommencer\n");
            test=1;
        }
    }
}

void SP_MENU(ST_JOUEUR* p_joueur,ST_PARAM_JEU* p_parametre,ST_SNAKE* p_snake){
    /*Sous programme permettant d'interagir avec le menu principal avec les choix suivant
    1-Lancer le jeu
    2-Rentrer dans le menu des paramètres
    3-Afficher les listes des scores des parties précédentes
    0-Arreter le programme
    */
    cls();                                                                                             //Effacer le terminal
    setColor(LIGHTCYAN);                                                                               //Changer la couleur du futur texte
    printf("******************************\n        * Menu *\n******************************\n\n");
    setColor(YELLOW);
    char choix_menu='\0';
    //Affichage du menu                                                                                 //Création d'une variable permettant de choisir dans le menu
    printf("************ BONJOUR %s ! ************\n", p_joueur->nom);
    printf("1-JOUER\n");
    printf("2-OPTION\n");
    printf("3-HIGH SCORE\n");
    printf("0-QUITTER\n");

    //Boucle permettant de redemander une valeur en cas d'erreur
    while(choix_menu=='\0'){
        fflush(stdin);                                                      //Vider le buffer
        scanf("%c",&choix_menu);                                            //Lecture du choix du joueur
        switch(choix_menu){
            case '1':
                SP_JEU(p_joueur, p_parametre,p_snake);                      //Lancer le programme du jeu
                break;
            case '2':
                SP_MENU_OPTION(p_joueur, p_parametre,p_snake);              //Lancer le menu des options
                break;
            case '3':
                SP_HIGH_SCORE(p_joueur, p_parametre,p_snake);               //Afficher la liste des scores des parties précédentes
                break;
            case '0':
                break;
            default:                                                        //Cas où la valeur rentrée n'est pas bonne -> demande de rentrer une autre valeur
                choix_menu='\0';
                printf("La valeur saisie n'est pas bonne, veuillez recommencer\n");
                break;
        }
    }
}

void SP_MENU_OPTION(ST_JOUEUR* p_joueur, ST_PARAM_JEU* p_parametre,ST_SNAKE* p_snake){
    /*Sous programme permettant d'interagir avec le menu des options avec les choix suivant:
    1-Modifier la couleur du serpent
    2-Modifier la couleur du stade
    3-Modifier la couleur des bordures
    4-Modifier la difficulté
    5-Modifier la hauteur du stade
    6-Modifier la largeur du stade
    0-Retour au menu principal
    */
    char choix_menu_option='\0';                                            //Création d'une variable permettant de choisir dans le menu
    //Affichage du menu
    cls();
    setColor(LIGHTCYAN);
    printf("******************************\n        * Options *\n******************************\n\n");
    setColor(YELLOW);
    printf("1-Couleur du serpent : ");
    PrintColor(p_parametre->couleur_snake);
    printf("2-Couleur du stade : ");
    PrintColor(p_parametre->couleur_stade);
    printf("3-Couleur du bordure : ");
    PrintColor(p_parametre->couleur_bordure);
    printf("4-Difficulte : %d\n", p_parametre->difficulte);
    printf("5-Hauteur Stade : %d\n", p_parametre->H_stade);
    printf("6-Largeur Stade : %d\n", p_parametre->L_stade);
    printf("0-RETOUR\n");
    printf("Quelle option voulez-vous modifier ?\n");

    //Boucle permettant de redemander une valeur en cas d'erreur
    while(choix_menu_option=='\0'){
        fflush(stdin);                                                              //Vider le buffer
        scanf("%c",&choix_menu_option);                                             //Lecture du choix du joueur
        switch(choix_menu_option){
            case '1':
                SP_COULEUR_SERPENT(p_joueur, p_parametre,p_snake);                  //Lancer le sous-programme permettant de modifier la couleur du serpent
                break;
            case '2':
                SP_COULEUR_STADE(p_joueur, p_parametre,p_snake);                    //Lancer le sous-programme permettant de modifier la couleur du satde
                break;
            case '3':
                SP_COULEUR_BORDURE(p_joueur, p_parametre,p_snake);                  //Lancer le sous-programme permettant de modifier la couleur des bordures
                break;
            case '4':
                SP_DIFFICULTE(p_joueur, p_parametre,p_snake);                       //Lancer le sous-programme permettant de modifier la difficulté
                break;
            case '5':
                SP_HAUTEUR_STADE(p_joueur, p_parametre,p_snake);                    //Lancer le sous-programme permettant de modifier la hauteur de stade
                break;
            case '6':
                SP_LARGEUR_STADE(p_joueur, p_parametre,p_snake);                    //Lancer le sous-programme permettant de modifier la largeur du stade
            case '0':
                SP_MENU(p_joueur, p_parametre,p_snake);                             //Revenir au menu principal
                break;
            default:                                                                //Cas où la valeur rentrée n'est pas bonne -> demande de rentrer une autre valeur
                choix_menu_option='\0';
                printf("La valeur saisie n'est pas bonne, veuillez recommencer\n");
                break;
        }
    }
}

void SP_COULEUR_SERPENT(ST_JOUEUR* p_joueur, ST_PARAM_JEU* p_parametre,ST_SNAKE* p_snake){
    /*Sous-programme permettant de modifier la couleur du serpent en rentrant une valeur correspondant à une couleur*/

    char char_couleur;                                              //Création de la variable stockant la valeur correspondant à une couleur
    printf("Choisissez une couleur :\n");
    int i=1;
    //Boucle affichant la liste des couleur avec leur valeur associée
    while(i<=15){
        printf("%d-",i);
        PrintColor(i);
        i++;
    }

    int couleur;
    int test=1;

    //Boucle permettant de redemander une valeur en cas d'erreur
    while (test){
        fflush(stdin);
        scanf("%s", &char_couleur);
        couleur=atoi(&char_couleur);
        test=0;
        if (couleur!=1 && couleur!=2 && couleur!=3 && couleur!=4 && couleur!=5 && couleur!=6 && couleur!=7 && couleur!=8 && couleur!=9 && couleur!=10 && couleur!=11 && couleur!=12 && couleur!=13 && couleur!=14 && couleur!=15)
        {   //Condition vérifiant si la valeur rentrée correspond bien à une couleur
            printf("La valeur saisie n'est pas bonne, veuillez recommencer\n");
            test=1;
        }
        else if (couleur == p_parametre->couleur_stade)
        {   //Condition vérifiant si la valeur rentrée est la même que celle du stade
            printf("\nCouleur identique a celle du stade\n");
            test=1;
        }
        else if (couleur == p_parametre->couleur_bordure)
        {   //Condition vérifiant si la valeur rentrée est la même que celle de la bordure
            printf("\nCouleur identique a celle de la bordure\n");
            test=1;
        }
    }
    p_parametre->couleur_snake = couleur;           //Affectation de la couleur choisie au paramètre couleur du serpent
    SP_MENU_OPTION(p_joueur,p_parametre,p_snake); /********ON RETOURNE AU MENU PARAMETRE**********/
}

void SP_COULEUR_STADE(ST_JOUEUR* p_joueur,ST_PARAM_JEU* p_parametre,ST_SNAKE* p_snake){
    /*Sous-programme permettant de modifier la couleur du stade en rentrant une valeur correspondant à une couleur*/
    printf("Couleur stade\n");
    char char_couleur;
    printf("Choisissez une couleur :\n");
    //Boucle affichant la liste des couleur avec leur valeur associée
    int i=1;
    while(i<=15){
        printf("%d-",i);
        PrintColor(i);
        i++;
    }
    int couleur;
    int test=1;

    //Boucle permettant de redemander une valeur en cas d'erreur
    while (test){
        fflush(stdin);
        scanf("%s", &char_couleur);
        couleur=atoi(&char_couleur);
        test=0;
        if (couleur!=1 && couleur!=2 && couleur!=3 && couleur!=4 && couleur!=5 && couleur!=6 && couleur!=7 && couleur!=8 && couleur!=9 && couleur!=10 && couleur!=11 && couleur!=12 && couleur!=13 && couleur!=14 && couleur!=15)
        {   //Condition vérifiant si la valeur rentrée correspond bien à une couleur
            printf("La valeur saisie n'est pas bonne, veuillez recommencer\n");
            test=1;
        }
        else if (couleur == p_parametre->couleur_snake)
        {   //Condition vérifiant si la valeur rentrée est la même que celle du serpent
            printf("\nCouleur identique a celle du snake\n");
            test=1;
        }
        else if (couleur == p_parametre->couleur_bordure)
        {   //Condition vérifiant si la valeur rentrée est la même que celle de la bordure
            printf("\nCouleur identique a celle de la bordure\n");
            test=1;
        }

    }
    p_parametre->couleur_stade = couleur;           //Affectation de la couleur choisie au paramètre couleur du stade
    SP_MENU_OPTION(p_joueur, p_parametre,p_snake); /********ON RETOURNE AU MENU PARAMETRE**********/
}

void SP_COULEUR_BORDURE(ST_JOUEUR* p_joueur,ST_PARAM_JEU* p_parametre,ST_SNAKE* p_snake){
    /*Sous-programme permettant de modifier la couleur de la bordure en rentrant une valeur correspondant à une couleur*/
    printf("Couleur bordure\n");
    char char_couleur;
    printf("Choisissez une couleur :\n");
    //Boucle affichant la liste des couleur avec leur valeur associée
    int i=1;
    while(i<=15){
        printf("%d-",i);
        PrintColor(i);
        i++;
    }
    int couleur;
    int test=1;

    //Boucle permettant de redemander une valeur en cas d'erreur
    while (test){
        fflush(stdin);
        scanf("%s", &char_couleur);
        couleur=atoi(&char_couleur);
        test=0;
        if (couleur!=1 && couleur!=2 && couleur!=3 && couleur!=4 && couleur!=5 && couleur!=6 && couleur!=7 && couleur!=8 && couleur!=9 && couleur!=10 && couleur!=11 && couleur!=12 && couleur!=13 && couleur!=14 && couleur!=15)
        {   //Condition vérifiant si la valeur rentrée correspond bien à une couleur
            printf("La valeur saisie n'est pas bonne, veuillez recommencer\n");
            test=1;
        }
        else if (couleur == p_parametre->couleur_snake)
        {   //Condition vérifiant si la valeur rentrée est la même que celle du serpent
            printf("\nCouleur identique a celle du snake\n");
            test=1;
        }
        else if (couleur == p_parametre->couleur_stade)
        {   //Condition vérifiant si la valeur rentrée est la même que celle de la bordure
            printf("\nCouleur identique a celle du stade\n");
            test=1;
        }

    }
    p_parametre->couleur_bordure = couleur;           //Affectation de la couleur choisie au paramètre couleur de la bordure
    SP_MENU_OPTION(p_joueur, p_parametre,p_snake); /********ON RETOURNE AU MENU PARAMETRE**********/
}

void SP_DIFFICULTE(ST_JOUEUR* p_joueur,ST_PARAM_JEU* p_parametre,ST_SNAKE* p_snake){
    /*Sous-programme permettant de modifier la difficulté*/
    printf("Difficulte ?(1-Facile, 2-Moyen, 3-Difficile)\n");
    char difficulte;                //Création d'une variable stockant la difficulté
    int test=1;

    //Boucle permettant de redemander une valeur en cas d'erreur
    while (test){
        fflush(stdin);
        scanf("%c", &difficulte);
        p_parametre->difficulte=atoi(&difficulte); //Affectation de la valeur de la difficulté dans les paramètres du jeu
        test=0;
        if(p_parametre->difficulte == 1 ||p_parametre->difficulte == 2||p_parametre->difficulte == 3){
            //Condition vérifiant si la valeur rentrée est égale à 1 ou 2 ou 3
            printf("La difficulte est maintenant a %d\n", p_parametre->difficulte);
            SP_MENU_OPTION(p_joueur, p_parametre,p_snake); /********ON RETOURNE AU MENU PARAMETRE**********/
        }
        else{
            //Condition de cas d'erreur
            printf("La valeur saisie n'est pas bonne, veuillez recommencer\n");
            test=1;
        }
    }

}

void SP_HAUTEUR_STADE(ST_JOUEUR* p_joueur,ST_PARAM_JEU* p_parametre,ST_SNAKE* p_snake){
    /*Sous-programme permettant de modifier la hauteur du stade*/
    printf("Nouvelle hauteur du stade (entre 10 et 20)\n");
    char H_stade;
    int test=1;

    //Boucle permettant de redemander une valeur en cas d'erreur
    while(test){
        fflush(stdin);
        scanf("%s", &H_stade);
        p_parametre->H_stade=atoi(&H_stade); //Affectation de la valeur de la hauteur dans les paramètres du jeu
        test=0;
        if(p_parametre->H_stade >= 10 && p_parametre->H_stade <= 20){
            //Condition verifiant que la valeur selectionnée est comprise entre 10 et 20
            printf("La hauteur du stade est maintenant a %d\n", p_parametre->H_stade);
            SP_MENU_OPTION(p_joueur, p_parametre,p_snake); /********ON RETOURNE AU MENU PARAMETRE**********/
        }
        else{
            //Cas d'erreur
            printf("La valeur saisie n'est pas bonne, veuillez recommencer\n");
            test=1;
        }
    }
}

void SP_LARGEUR_STADE(ST_JOUEUR* p_joueur,ST_PARAM_JEU* p_parametre,ST_SNAKE* p_snake){
    /*Sous-programme permettant de modifier la largeur du stade*/
    printf("Nouvelle largeur du stade (entre 20 et 50)\n");
    char L_stade;
    int test=1;

    //Boucle permettant de redemander une valeur en cas d'erreur
    while(test){
        fflush(stdin);
        scanf("%s", &(L_stade));
        p_parametre->L_stade=atoi(&L_stade);
        test=0;
        if(p_parametre->L_stade >= 20 && p_parametre->L_stade <= 50){
            //Condition verifiant que la valeur selectionnée est comprise entre 20 et 50
            printf("La largeur du stade est maintenant a %d\n", p_parametre->L_stade);
            SP_MENU_OPTION(p_joueur, p_parametre,p_snake); /********ON RETOURNE AU MENU PARAMETRE**********/
        }
        else{
            //Cas d'erreur
            printf("La valeur saisie n'est pas bonne, veuillez recommencer\n");
            test=1;
        }

    }
}

void SP_HIGH_SCORE(ST_JOUEUR* p_joueur, ST_PARAM_JEU* p_parametre,ST_SNAKE* p_snake){
    /*Sous-programme permettant de d'afficher les scores précédents*/
    cls();
    setColor(LIGHTCYAN);
    int retour_menu;
    printf("******************************\n        * High Score *\n******************************\n\n");
    setColor(WHITE);

    int caractereActuel = 0;
    FILE* HIGH_SCORE ;
    HIGH_SCORE = fopen ("highscores.txt" , "rt") ;      //Ouverture du fichier texte en rt (lecture seule)
    if (HIGH_SCORE != NULL)
    {
        do
        {
            //Boucle affichant les caracteres un par un
            caractereActuel = fgetc(HIGH_SCORE);
            printf("%c", caractereActuel);
        } while (caractereActuel != EOF);               //Arret de la bloucle lorsque le dernier caractere est atteint

    }
    fclose(HIGH_SCORE);                                 //Fermeture du fichier
    printf("0-RETOUR\n");
    scanf("%d",&retour_menu);
    if(retour_menu == 0){
        SP_MENU(p_joueur,p_parametre,p_snake);          //Retour au menu principal
    }
}

void SP_SAUVEGARDE_SCORE(ST_JOUEUR* p_joueur){
    /*Sous-programme permettant de d'écrire les scores dans le fichier texte*/
    FILE * HIGH_SCORE;
    HIGH_SCORE = fopen ("highscores.txt" , "a+t") ;                         //Ouverture du fichier texte en a+t (ecriture à la fin)
    fprintf (HIGH_SCORE ,"\t%s\t %d\n",p_joueur->nom,p_joueur->score);      //Ecriture du nom du joueur suivi deson score
    fclose ( HIGH_SCORE );                                                  //Fermeture du fichier
}

char * SP_INT_TO_COLOR(int int_couleur){
    /*Fonction prennant en entrée un numero correspondant à une couleur et qui renvoie la la couleur correspondante*/
    switch(int_couleur){
        //Conditions qui assigne le numero à la couleur correspondante
    	case 0 : return "BLACK";
		case 1 : return "BLUE";
		case 2 : return "GREEN";
		case 3 : return "CYAN";
		case 4 : return "RED";
		case 5 : return "MAGENTA";
		case 6 : return "BROWN";
		case 7 : return "GREY";
		case 8 : return "DARKGREY";
		case 9 : return "LIGHTBLUE";
		case 10: return "LIGHTGREEN";
		case 11: return "LIGHTCYAN";
		case 12: return "LIGHTRED";
		case 13: return "LIGHTMAGENTA";
		case 14: return "YELLOW";
		case 15: return "WHITE";
		default: return "";         //Cas d'erreur
    }
}

void PrintColor(int color){
    /*Sous-programme permettant d'écrire la couleur (dans cette même couleur) correspondante au numero rentré*/
    setColor(color);
    printf("%s\n", SP_INT_TO_COLOR(color));     //Fonction renvoyant le texte de la couleur
    setColor(YELLOW);

}
