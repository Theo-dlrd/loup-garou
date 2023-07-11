#ifndef __TOUR_ROLE__
#define __TOUR_ROLE__

#include "../head/menu.h"
#include "../head/role.h"
#include "../head/main.h"


int tourLoupGarou(jeu_t * jeu, SDL_Renderer * renderer, TTF_Font * font);
int tourChasseur(jeu_t * jeu, SDL_Renderer * renderer, TTF_Font * font);
void tourVoyante(jeu_t * jeu, SDL_Renderer * renderer, TTF_Font * font);
void tourAmoureux(jeu_t * jeu, SDL_Renderer * renderer, TTF_Font * font);
void tourCupidon(jeu_t * jeu, SDL_Renderer * renderer, TTF_Font * font);
int tourSorciere(jeu_t * jeu, SDL_Renderer * renderer, TTF_Font * font, joueur_t * joueurMort);
int trouverVoleur(jeu_t * jeu);
void tourVoleur(jeu_t * jeu, SDL_Renderer * renderer, TTF_Font * font);



#endif