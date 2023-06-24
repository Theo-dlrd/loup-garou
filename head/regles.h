#ifndef __REGLE__
#define __REGLE__

typedef struct regles_s{
    char ** explications;
    char ** obj;
} Regles;

Regles * initRegles();
int destroyRegles(Regles * regles);

#endif