#ifndef __MENU__
#define __MENU__

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../head/regles.h"
#include "../head/role.h"
#include "../head/main.h"

#define MAX_INPUT 3

typedef struct button_s{
    int x, y, longueur, largeur;
} Button;

typedef struct elem_s{
    int x, y, longueur, largeur;
} Element;

jeu_t* menu(SDL_Renderer* renderer, TTF_Font* font);
char * menu_nom(SDL_Renderer* renderer, TTF_Font* font, int indice, jeu_t * jeu);
char* itoa(int value, char* buffer);

#endif