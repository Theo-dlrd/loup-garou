#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../head/role.h"
#include "../head/regles.h"

Regles * initRegles(){
    Regles * regles = malloc(sizeof(Regles));
    regles->explications = malloc(sizeof(char*)*3);
    regles->obj = malloc(sizeof(char*)*3);

    for(int i=0; i<3; i++){
        regles->explications[i] = malloc(sizeof(char)*200);
        switch(i){
            case 0 : strcpy(regles->explications[i], "Le loup-garou est un jeu qui se joue de 6 à 24 personnes, à tous âges."); break;
            case 1 : strcpy(regles->explications[i], "Au debut de chaque partie, chaque joueur recevra une carte qui déterminera son rôle."); break;
            case 2 : strcpy(regles->explications[i], "Mémorisez bien les caractéristiques de chaque rôle : il vous sera impossible de les revoir en jeu."); break;
            default : break;
        }

        regles->obj[i] = malloc(sizeof(char)*200);
        switch(i){
            case 0 : strcpy(regles->obj[i], "- Les VILLAGEOIS gagnent dès qu'ils réussissent a éliminer le dernier LOUP-GAROU."); break;
            case 1 : strcpy(regles->obj[i], "- Les LOUPS-GAROUS gagnent s'ils éliminent le dernier VILLAGEOIS."); break;
            case 2 : strcpy(regles->obj[i], "- Cas particulier : Si les Amoureux sont 1 LOUP-GAROU + 1 VILLAGEOIS, ils gagnent dès qu'ils ont élimine tous les autres."); break;
            default : break;
        }
    }
    return regles;
}

int destroyRegles(Regles * regles){
    for(int i=0; i<3; i++){
        free(regles->obj[i]);
        regles->obj[i] = NULL;

        free(regles->explications[i]);
        regles->explications[i] = NULL;
    }
    free(regles->explications);
    regles->explications = NULL;

    free(regles->obj);
    regles->obj = NULL;

    free(regles);
    regles = NULL;

    return 1;
}