#ifndef __ROLES__
#define __ROLES__

typedef enum{NONE=-1, LOUPGAROU, VILLAGEOIS, CHASSEUR, VOYANTE, CUPIDON, SORCIERE, PETITEFILLE, VOLEUR} Roles;
typedef enum{VIVANT, MORT} Etat;
typedef enum{AUCUN, AMOUREUX, MAIRE} Statut;


#define MAX_STATUT 2
#define MAX_NAME_SIZE 11
#define NB_ROLES 8

typedef struct pioche_s{
    Roles role;
    char * nomIMG;
} pioche_t;

typedef struct joueur_s{
    char *nom;
    Roles role;
    char * nomIMG;
    Etat etat;
    Statut statut[MAX_STATUT];
    int num;    /* 1-nbJoueur */
} joueur_t;

typedef struct mort_s{
    int iNbMort;
    joueur_t ** tab_mort;
}mort_t;

typedef struct jeu_s{
    int nbJoueur;       /* 6-24 */
    int nbLoupGarou;    /* 1-nbJoueur/2 */
    int nbVillageois;   /* 1-nbJoueur/2 */
    int nbChasseur;     /* 0-1 */
    int nbVoyante;      /* 0-1 */
    int nbCupidon;      /* 0-1 */
    int nbSorciere;     /* 0-1 */
    int nbPetiteFille;  /* 0-1 */
    int nbVoleur;       /* 0-1 */
    joueur_t * joueurs;
    pioche_t * piocheVoleur;    /* carte 1 : piocheVoleur[0], carte 2 : piocheVoleur[1] */
    int potionsSorciere[2];     /* potionSorciere[0] : vie, potionSorciere[1] : mort */
}jeu_t;

#endif