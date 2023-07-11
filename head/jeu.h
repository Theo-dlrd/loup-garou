#ifndef __JEU__
#define __JEU__

#include "../head/menu.h"
#include "../head/role.h"
#include "../head/main.h"
#include "../head/tourRole.h"

int play(jeu_t * jeu, SDL_Renderer * renderer, TTF_Font * font);
int initJeu(jeu_t * jeu, SDL_Renderer * renderer, TTF_Font * font);

#endif