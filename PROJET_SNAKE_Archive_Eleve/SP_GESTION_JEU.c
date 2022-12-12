#include <stdio.h>
#include <time.h>
#include "myLib.h"
#include "mesTypes.h"
#include "SP_CONFIGURATION.h"
#include "SP_GESTION_JEU.h"


void SP_JEU(ST_JOUEUR* p_joueur,ST_PARAM_JEU* p_parametre,ST_SNAKE* p_snake){
    /*Sous-programme gerant le jeu*/
    cls();                                                      //Effacer le terminal
    hidecursor();                                               //Cacher le curseur

    SP_INIT_SERPENT(p_parametre,p_snake);                       //Initialisation des paramètres du sepent
    SP_AFFICHE_STADE(p_joueur,p_parametre);                     //Afficher le stade
    SP_AFFICHE_SCORE(p_joueur,p_parametre,p_snake);             //Afficher le score
    p_parametre->difficulte_game=p_parametre->difficulte;       //Associer la valeur de la difficulté de la partie à celle définie dans les options
    ST_POMME pomme;                                             //Création d'une structure contenant les informations d'une pomme
    pomme.compteur=0;                                           //Initialisation du compteur de pomme à 0
    SP_CREER_POMME(p_parametre,p_snake,&pomme);                 //Création d'une pomme
    PlaySound("Snake.wav", NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);

    //Boucle du jeu vérifiant si le joueur a perdu
    while(SP_GAME_OVER(p_joueur,p_parametre,p_snake)){

        msleep((4-p_parametre->difficulte_game)*60);            //Taux de rafraichissement du jeu

        /* Gestion pomme */

        if (SP_TEST_POMME_SNAKE(p_snake,&pomme)){               //Test si la pomme est mangée
            p_snake->taille++;                                  //Augmentation de la taille du serpent
            p_snake->pos[p_snake->taille-2].x=p_snake->tete.x;  //Positionnement de la premiere partie du corps
            p_snake->pos[p_snake->taille-2].y=p_snake->tete.y;
            if (pomme.type==1){                                 //Test si la pomme mangée est standard
                p_joueur->score=p_joueur->score+3;              //Augmentation du score de 3
            }
            else if(pomme.type==2){                             //Test si la pomme mangée est bonus
                p_joueur->score=p_joueur->score+9;              //Augmentation du score de 9
            }
            else if(pomme.type==3){                             //Test si la pomme mangée est empoisonnée
                p_parametre->difficulte_game=p_parametre->difficulte+1;//Augmentation de la difficulté de 1
            }

            SP_AFFICHE_SCORE(p_joueur,p_parametre,p_snake);     //Afficher le score
            SP_CREER_POMME(p_parametre,p_snake,&pomme);         //Création d'une nouvelle pomme
        }

        SP_EVOLUTION_SERPENT(p_joueur,p_parametre,p_snake);     //Evolution du serpent
        SP_AFFICHE_SERPENT(p_joueur,p_parametre,p_snake);       //Affichage du serpent

        //Remplacement du dernier élément du serpent par un élément composant le stade
        setColor(p_parametre->couleur_stade);
        gotoxy(p_snake->old_pos[0].x,p_snake->old_pos[0].y);
        printStadeElement();
    }
    PlaySound(NULL, NULL,0);
    PlaySound("GameOver.wav", NULL, SND_FILENAME | SND_ASYNC);
    SP_MENU_GAME_OVER(p_joueur,p_parametre,p_snake);            //Afficher le menu Game Over
}

void SP_EVOLUTION_SERPENT(ST_JOUEUR* p_joueur,ST_PARAM_JEU* p_parametre,ST_SNAKE* p_snake){
    /*Sous-programme gérant l'évolution de la position et de la direction du serpent au cour du jeu*/
    int i,taille;
    taille = p_snake->taille;

    /*Enregistrement tete*/
    p_snake->old_tete.x=p_snake->tete.x;
    p_snake->old_tete.y=p_snake->tete.y;

    /*Enregistrement position*/
    for(i=0;i<taille-1;i++){
        p_snake->old_pos[i].x=p_snake->pos[i].x;
        p_snake->old_pos[i].y=p_snake->pos[i].y;
    }

    /*Direction*/
    int direction=SP_Gestion_Clavier();

    if( !((direction==DROITE && p_snake->direction==GAUCHE) || (direction==GAUCHE && p_snake->direction==DROITE) || (direction==HAUT && p_snake->direction==BAS) || (direction==BAS && p_snake->direction==HAUT)) && direction!=-1){
        //Condition interdisant au serpent de revenir sur ses pas et donc de mourir
        p_snake->direction=direction;
    }

    /*Evolution tete*/
    if (p_snake->direction == HAUT){                //Tête vers le HAUT
        p_snake->tete.y = p_snake->tete.y - 1;
    }
    else if (p_snake->direction == BAS){            //Tête vers le BAS
        p_snake->tete.y = p_snake->tete.y + 1;
    }
    else if (p_snake->direction == GAUCHE){         //Tête vers la GAUCHE
        p_snake->tete.x = p_snake->tete.x - 1;
    }
    else if (p_snake->direction == DROITE){         //Tête vers la DROITE
        p_snake->tete.x = p_snake->tete.x + 1;
    }

    /*Evolution queue*/
    p_snake->pos[taille-2].x=p_snake->old_tete.x;
    p_snake->pos[taille-2].y=p_snake->old_tete.y;

    for(i=0;i<taille-2;i++){
        p_snake->pos[i].x=p_snake->old_pos[i+1].x;
        p_snake->pos[i].y=p_snake->old_pos[i+1].y;
    }
}

int SP_Gestion_Clavier(){
/*===================================================================================
/ Nom Sémantique : FONCTION SP_Gestion_Clavier

/ Sémantique : Gère la détection des évènements clavier sur les touches de direction

/ Paramètres :
/ direction (OUT) - entier : Contient la direction sollicitée par l'utilisateur
/ DROITE = 0 , GAUCHE = 1 , BAS = 2 , HAUT = 3 et -1 SINON
/ Pré-condition : AUCUNE
/ Post conditions : la direction vaut -1,0,1,2,3
/ ====================================================================================
/ Test : le chiffre renvoyé correspond à la direction appuyée
/ ====================================================================================*/
    char direction ;
    int dir;

    if ( kbhit()) {

    direction = getkey() ;

    switch (direction){

        case KEY_UP : dir = HAUT ; break;
        case KEY_DOWN : dir = BAS ; break;
        case KEY_LEFT : dir = GAUCHE ; break;
        case KEY_RIGHT: dir= DROITE ; break;
        }
    }
    else {dir=-1;}
    return dir;
}

void SP_AFFICHE_STADE(ST_JOUEUR* p_joueur,ST_PARAM_JEU* p_parametre){
    /*Sous-programme servant à afficher le stade*/
    int x,y;

    //Double boucle permettant d'afficher un rectangle (le stade)
    for(y=1;y<p_parametre->H_stade +3;y++){
        for (x=1;x<(p_parametre->L_stade +3 );x++ ){
            gotoxy(x,y);
            if (x==1 || x ==p_parametre->L_stade+2){        //Condition si emplacement de bordure
                setColor(p_parametre->couleur_bordure);     //Mise en couleur de la bordure
            }
            else if(y==1 ||y==p_parametre->H_stade+2){      //Condition si emplacement de bordure
                setColor(p_parametre->couleur_bordure);     //Mise en couleur de la bordure
            }
            else{                                           //Condition interieur des bordures
                setColor(p_parametre->couleur_stade);       //Mise en couleur du stade
            }
            setBackgroundColor(0);
            printStadeElement();
        }
    }

}

void SP_AFFICHE_SERPENT(ST_JOUEUR* p_joueur,ST_PARAM_JEU* p_parametre,ST_SNAKE* p_snake){
    /*Sous-programme servant à afficher le serpent*/
    setColor(p_parametre->couleur_snake);               //Mise en couleur du sepent

    gotoxy(p_snake->tete.x,p_snake->tete.y);            //Placement du curseur à la position de la tête
    printSnakeHead();                                   //Afficher la tête du serpent

    int i;
    for (i=0;i<p_snake->taille-1;i++){                  //Boucle permettant d'afficher le corps du serpent
        gotoxy(p_snake->pos[i].x,p_snake->pos[i].y);
        printf("=");


    }

}

void SP_CREER_POMME(ST_PARAM_JEU* p_parametre,ST_SNAKE* p_snake,ST_POMME* p_pomme){
    /*Sous-programme permettant de créer une pomme dans le stade*/
    float x,y;
    int Pomme=1;

    //Bloucle de génération de pomme
    while(Pomme){
        //Définition d'un emplacement aléatoire pour une pomme
        x=random()%p_parametre->L_stade;
        y=random()%p_parametre->H_stade;
        x=x+2;
        y=y+2;
        Pomme=0;
        if(((x==p_snake->tete.x) && (y==p_snake->tete.y)) || x==1 || x==p_parametre->L_stade+2 || y==1 || y==p_parametre->H_stade+2 )
        {   //Condition si l'emplacement de la futur pomme est sur une bordure ou sur la tête du serpent
            Pomme = 1;                  //Relancer la boucle
        }
        int i;
        for(i=0;i<p_snake->taille+1;i++)
        {
            if(x==p_snake->pos[i].x && y==p_snake->pos[i].y)
            {   //Condition si l'emplacement de la futur pomme est le corps du serpent
                Pomme= 1;               //Relancer la boucle
            }
        }
    }
    p_pomme->pos.x=x;
    p_pomme->pos.y=y;
    int num=random()%5;             //Création d'une variable aléatoire

    if (num<3){                     //Si la variable aléatoire est <3 alors créer une pomme normale
        setColor(GREEN);
        gotoxy(x,y);
        printPomme();
        p_pomme->type=1;

    }
    else if (num==4){               //Si la variable aléatoire est =4 alors créer une pomme bonus
        setColor(YELLOW);
        gotoxy(x,y);
        printPomme();
        p_pomme->type=2;
    }
    else if (num==3){               //Si la variable aléatoire est =3 alors créer une pomme empoisonnée
        setColor(RED);
        gotoxy(x,y);
        printPomme();
        p_pomme->type=3;
        p_pomme->compteur++;
    }
    if(p_pomme->compteur!=0){       //Condition permettant l'augmentation de la difficulté du jeu en cas de pomme empoisonnée
        if(p_pomme->compteur==5){
            p_pomme->compteur=0;
            p_parametre->difficulte_game=p_parametre->difficulte;
        }
        p_pomme->compteur++;
    }

}

int SP_TEST_POMME_SNAKE(ST_SNAKE* p_snake,ST_POMME* p_pomme){
    /*Fonction renvoyant 1 si le serpent mange une pomme 0 sinon*/
    int a=0;
    if ( p_snake->tete.x==p_pomme->pos.x && p_snake->tete.y==p_pomme->pos.y)
    {   //Condition si la tête du serpent est au même emplacement que la pomme
        a=1;
    }
    return a;

}

void SP_INIT_SERPENT(ST_PARAM_JEU* p_parametre,ST_SNAKE* p_snake){
    /*Sous-programme initialisant la position de départ du serpent*/
    p_snake->taille=5;                          //Serpent de taille = 5

    int x0,y0;
    //Placement d'un point au milieu du stade
    x0= floor((p_parametre->L_stade)/2);
    y0= floor((p_parametre->H_stade)/2);

    //Placement de la tête du serpent au milieu du stade
    p_snake->tete.x=x0;
    p_snake->tete.y=y0;

    int i,taille;
    taille = p_snake->taille;

    for(i=0;i<100;i++){
        p_snake->pos[i].x=2;
        p_snake->pos[i].y=2;

    }
    for(i=0;i<taille-1;i++){        //Boucle assignant les positions des parties du corps du sepent
        p_snake->pos[i].x=x0;
        p_snake->pos[i].y=y0-i-1+taille;

    }

    p_snake->direction= DROITE;     //Initialisation de la direction du serpent à DROITE
}

void SP_AFFICHE_SCORE(ST_JOUEUR* p_joueur,ST_PARAM_JEU* p_parametre,ST_SNAKE* p_snake){
    /*Sous-programme affichant le score actuel, la taille actuelle du serpent et les anciens scores pendant le jeu*/
    setColor(WHITE);
    //Afficher la taille et le score en dessous du stade
    gotoxy(1,p_parametre->H_stade+5);
    printf("Taille du serpent : %d\n",p_snake->taille );
    printf("Score : %d\n",p_joueur->score );

    //Afficher les anciens scores du fichier texte sur la droite du stade
    int caractereActuel = 0;
    FILE* HIGH_SCORE ;
    HIGH_SCORE = fopen ("highscores.txt" , "rt") ;    //Ouverture du fichier texte en rt (lecture seule)
    if (HIGH_SCORE != NULL)
    {
        gotoxy(p_parametre->L_stade+5,5);
        int h=0;
        do
        {   //Boucle affichant les caracteres un par un
            caractereActuel = fgetc(HIGH_SCORE);

            if(caractereActuel==10){
                    h=h+1; gotoxy(p_parametre->L_stade+5,5+h);
            }
            else{
               printf("%c", caractereActuel);
            }


        } while (caractereActuel != EOF);


    }
    fclose(HIGH_SCORE);     //Fermeture du fichier
}

int SP_GAME_OVER(ST_JOUEUR* p_joueur,ST_PARAM_JEU* p_parametre,ST_SNAKE* p_snake){
    /*Fonction renvoyant 0 si il y a collision entre la tête du serpent et le corps/les bordures 0 sinon*/
    int H,L;
    H=p_parametre->H_stade;
    L=p_parametre->L_stade;
    int a=1;

    if( p_snake->tete.x==1 || p_snake->tete.x==(L+2) || p_snake->tete.y==1 || p_snake->tete.y==(H+2))
    {   //Condition détectant la collision entre la tête du serpent et les bordures
        a= 0;
    }
    int i;
    for(i=0;i<p_snake->taille-2;i++){
        if (p_snake->tete.x==p_snake->pos[i].x && p_snake->tete.y==p_snake->pos[i].y)
        {   //Condition détectant la collision entre la tête du serpent et le corps
            a= 0;
        }
    }
    return a;

}

void SP_MENU_GAME_OVER(ST_JOUEUR* p_joueur,ST_PARAM_JEU* p_parametre,ST_SNAKE* p_snake){
    /*Sous-programme permettant d'afficher le menu GAME OVER*/
    cls();
    setColor(LIGHTCYAN);
    printf("\n******************************\n");
    printf("********** GAMEOVER **********\n\n\n");
    setColor(WHITE);
    printf("Ton score est %d\n", p_joueur->score);      //Affichage du score du joueur

    SP_SAUVEGARDE_SCORE(p_joueur);                      //Enregistrement du score du joueur

    //Menu permettant de rejouer ou de quitter
    printf("Que voulez vous faire %s?\n", p_joueur->nom);
    printf("1-REJOUER\n");
    printf("0-QUITTER\n");
    char choix_menu='\0';

    //Boucle permettant de redemander une valeur en cas d'erreur
    while(choix_menu=='\0'){
        fflush(stdin);                                          //Vider le buffer
        scanf("%c",&choix_menu);                                //Lecture du choix du joueur
        switch(choix_menu){
            case '1':
                p_joueur->score = 0;
                SP_ACCUEIL(p_joueur,p_parametre,p_snake);       //Retour au menu principal
                break;
            case '0':
                break;                                          //Fermer le jeu
            default:                                            //Cas d'erreur
                choix_menu='\0';
                printf("La valeur saise n'est pas bonne, veuillez recommencer\n");
                break;
        }
    }

}

