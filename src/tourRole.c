#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <string.h>

#include "../head/tourRole.h"

/**
 * @file tourRole.c
 * @brief Fonctions liées au tour de chaque rôle lors de la partie
 * @author Théo Duluard
 * @version 1.0
 * @date 11/07/2023
 */


/**
 * @fn selectNom
 * @brief Permet de selectionner graphiquement le nom des personnage à éléminer en fonction du rôle du selectionneur.
 * @param jeu 
 * @param renderer 
 * @param font 
 * @param role 
 * @return int 
 */
int selectNom(jeu_t * jeu, SDL_Renderer * renderer,TTF_Font * font, Roles role){
    int iLigne=1, iCol=0, ecart;
    if(jeu->nbJoueur > 4 && jeu->nbJoueur <= 8){
        ecart=5;
    }
    else if(jeu->nbJoueur > 8 && jeu->nbJoueur <= 12){
        ecart=6;
    }
    else if(jeu->nbJoueur > 12 && jeu->nbJoueur <= 16){
        ecart=7;
    }
    else if(jeu->nbJoueur > 16 && jeu->nbJoueur <= 20){
        ecart=8;
    }
    else if(jeu->nbJoueur > 20 && jeu->nbJoueur <= 24){
        ecart=10;
    }

    SDL_Surface * title = NULL;
    SDL_Rect rect_title;
    Mix_Chunk * lg_speed_up;
    SDL_Color couleur = whiteColor, gray = lightgrayColor;
    Mix_Chunk *tir;
    if(role == LOUPGAROU){
        lg_speed_up = Mix_LoadWAV("../sound/voice/lg_design.wav");
        Mix_VolumeChunk(lg_speed_up, MIX_MAX_VOLUME/2); 
        title = TTF_RenderUTF8_Solid(font, "Tour des Loups-Garous", couleur);
        rect_title.x = LONG_WIN/2-150;
        rect_title.w = 300;
    }
    else if(role == SORCIERE){
        title = TTF_RenderUTF8_Solid(font, "Tour de la Sorcière", couleur);
        rect_title.x = LONG_WIN/2-125;
        rect_title.w = 250;
    }
    else if(role == CHASSEUR){
        tir = Mix_LoadWAV("../sound/effects/tir.wav");
        couleur = blackColor;
        gray = grayColor;
        title = TTF_RenderUTF8_Solid(font, "Tour du Chasseur", couleur);
        rect_title.x = LONG_WIN/2-100;
        rect_title.w = 200;
    }
    rect_title.y = 10;
    rect_title.h = 75;
    SDL_Texture *txr_title = SDL_CreateTextureFromSurface(renderer, title);
    
    SDL_FreeSurface(title);

    SDL_Surface * info = NULL;
    SDL_Rect rect_info;
    if(role == CHASSEUR){
        info = TTF_RenderUTF8_Solid(font, "Qui emportez-vous dans la mort ?", couleur);
        rect_info.x = LONG_WIN/2 - 275;
        rect_info.y = rect_title.y+rect_title.h;
        rect_info.w = 550;
        rect_info.h = 50;
    }
    else{
        info = TTF_RenderUTF8_Solid(font, "Sélectionner votre victime", couleur);
        rect_info.x = LONG_WIN/2 - 250;
        rect_info.y = rect_title.y+rect_title.h;
        rect_info.w = 500;
        rect_info.h = 50;
    }
    SDL_Texture *txr_info = SDL_CreateTextureFromSurface(renderer, info);
    SDL_FreeSurface(info);

    SDL_Surface *confirmer = TTF_RenderUTF8_Solid(font, "Confirmer", gray);
    SDL_Texture *txr_confirmer = SDL_CreateTextureFromSurface(renderer, confirmer);
    SDL_Rect rect_confirmer;
    if(role == SORCIERE){
        rect_confirmer.x = LONG_WIN/2+75;
        rect_confirmer.y = LARG_WIN-75-30;
        rect_confirmer.w = 150;
        rect_confirmer.h = 75;
    }
    else{
        rect_confirmer.x = LONG_WIN/2-75;
        rect_confirmer.y = LARG_WIN-75-30;
        rect_confirmer.w = 150;
        rect_confirmer.h = 75;
    }
    SDL_FreeSurface(confirmer);

    SDL_Surface * back = NULL;
    SDL_Texture * txr_back = NULL;
    SDL_Rect rect_back;
    if(role == SORCIERE){
        back = TTF_RenderUTF8_Solid(font, "Retour", lightgrayColor);
        txr_back = SDL_CreateTextureFromSurface(renderer, back);
        rect_back.x = LONG_WIN/2-225;
        rect_back.y = LARG_WIN-75-30;
        rect_back.w = 150;
        rect_back.h = 75;
        SDL_FreeSurface(back);
    }

    SDL_Surface *tab_noms[jeu->nbJoueur];
    SDL_Texture *tab_txr_noms[jeu->nbJoueur];
    SDL_Rect tab_rect_nom[jeu->nbJoueur];

    int etat_boutons[jeu->nbJoueur];
    for(int i=0; i<jeu->nbJoueur; i++){
        etat_boutons[i] = 0;
    }
    int bouton_selected = -1;

    for(int i=0; i<jeu->nbJoueur; i++){
        tab_noms[i] = TTF_RenderUTF8_Solid(font, jeu->joueurs[i].nom, couleur);
        tab_txr_noms[i] = SDL_CreateTextureFromSurface(renderer, tab_noms[i]);
        tab_rect_nom[i].x = LONG_WIN*(4+4*iCol++)/20-strlen(jeu->joueurs[i].nom)*10;
        tab_rect_nom[i].y = LARG_WIN*(iLigne+1)/ecart+20;
        tab_rect_nom[i].w = strlen(jeu->joueurs[i].nom)*20;
        tab_rect_nom[i].h = 40;
        SDL_FreeSurface(tab_noms[i]);
        if(i%4==3){
            iLigne++;
            iCol=0;
        }
    }

    int run=1, delay;
    int tick = SDL_GetTicks();
    int x, y;
    while(run){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            delay = SDL_GetTicks()-tick;
            if(role == LOUPGAROU && delay>=20000 && delay<=21000){
                printf("%d\n", delay);
                Mix_PlayChannel(2, lg_speed_up, 0);
            }
            switch(event.type){
                case SDL_QUIT: 
                    run=0; 
                    break;

                case SDL_MOUSEBUTTONUP:
                    SDL_GetMouseState(&x,&y);
                    for(int i=0; i<jeu->nbJoueur; i++){
                        if(jeu->joueurs[i].etat == VIVANT && x>=tab_rect_nom[i].x && x<=tab_rect_nom[i].x+tab_rect_nom[i].w && y>=tab_rect_nom[i].y && y<=tab_rect_nom[i].y+tab_rect_nom[i].h){
                            if(bouton_selected == -1 && etat_boutons[i] == 0){
                                etat_boutons[i] = 1;
                                bouton_selected = i;
                            }
                            else if(etat_boutons[i] == 1){
                                etat_boutons[i] = 0;
                                bouton_selected = -1;
                            }
                        }
                    }
                    if(bouton_selected != -1  && x>=rect_confirmer.x && x<=rect_confirmer.x+rect_confirmer.w && y>=rect_confirmer.y && y<=rect_confirmer.y+rect_confirmer.h){
                        if(role == CHASSEUR){
                            Mix_PlayChannel(1,tir,0);
                            while(Mix_Playing(1));
                        }
                        run = 0;
                    }
                    if(x>=rect_back.x && x<=rect_back.x+rect_back.w && y>=rect_back.y && y<=rect_back.y+rect_back.h){
                        bouton_selected = -1;
                        run = 0;
                    }
            }
        }
        if(role == CHASSEUR){
            SDL_SetRenderDrawColor(renderer,149,56,58,255);
        }
        else{
            SDL_SetRenderDrawColor(renderer,15,5,107,255);
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, txr_title, NULL, &rect_title);
        SDL_RenderCopy(renderer, txr_info, NULL, &rect_info);
        for(int i=0; i<jeu->nbJoueur; i++){
            if(jeu->joueurs[i].etat == VIVANT){
                if(etat_boutons[i] == 1){
                    SDL_SetRenderDrawColor(renderer, 139, 0, 0, 255);
                    SDL_RenderFillRect(renderer, &tab_rect_nom[i]);
                }
                else{
                    if(role == CHASSEUR){
                        SDL_SetRenderDrawColor(renderer,149,56,58,255);
                    }
                    else{
                        SDL_SetRenderDrawColor(renderer,15,5,107,255);
                    }
                    SDL_RenderFillRect(renderer, &tab_rect_nom[i]);
                }
                if(role == CHASSEUR){
                    SDL_SetRenderDrawColor(renderer,149,56,58,255);
                }
                else{
                    SDL_SetRenderDrawColor(renderer,15,5,107,255);
                }
                SDL_RenderCopy(renderer, tab_txr_noms[i], NULL, &tab_rect_nom[i]);
            }
        }
        if(bouton_selected != -1){
            SDL_RenderCopy(renderer, txr_confirmer, NULL, &rect_confirmer);
        }
        if(role == SORCIERE){
            SDL_RenderCopy(renderer, txr_back, NULL, &rect_back);
        }
        SDL_RenderPresent(renderer);
    }

    for(int i=0; i<jeu->nbJoueur; i++){
        SDL_DestroyTexture(tab_txr_noms[i]);
    }
    SDL_DestroyTexture(txr_confirmer);
    SDL_DestroyTexture(txr_title);
    SDL_DestroyTexture(txr_info);
    if(role == CHASSEUR){
        Mix_FreeChunk(tir);
    }

    return bouton_selected;
}

/**
 * @brief Réalise le tour de jeu des Loups garous grâce à l'appel de la fonction "selectNom"
 * @fn tourLoupGarou
 * @param jeu 
 * @param renderer 
 * @param font 
 * @return int 
 */
int tourLoupGarou(jeu_t * jeu, SDL_Renderer * renderer, TTF_Font * font){
    return selectNom(jeu, renderer, font, LOUPGAROU);
}

/**
 * @brief Réalise le tour de jeu du Chasseur à sa mort grâce à l'appel de la fonction "selectNom"
 * @fn tourChasseur
 * @param jeu 
 * @param renderer 
 * @param font 
 * @return int 
 */
int tourChasseur(jeu_t * jeu, SDL_Renderer * renderer, TTF_Font * font){
    return selectNom(jeu, renderer, font, CHASSEUR);
}

/**
 * @brief Réalise le tour de jeu de la voyante. Elle selectionne le nom d'un joueur pour voir son rôle.
 * @fn tourVoyante
 * @param jeu 
 * @param renderer 
 * @param font 
 */
void tourVoyante(jeu_t * jeu, SDL_Renderer * renderer, TTF_Font * font){
    int iLigne=1, iCol=0, ecart;
    if(jeu->nbJoueur > 4 && jeu->nbJoueur <= 8){
        ecart=5;
    }
    else if(jeu->nbJoueur > 8 && jeu->nbJoueur <= 12){
        ecart=6;
    }
    else if(jeu->nbJoueur > 12 && jeu->nbJoueur <= 16){
        ecart=7;
    }
    else if(jeu->nbJoueur > 16 && jeu->nbJoueur <= 20){
        ecart=8;
    }
    else if(jeu->nbJoueur > 20 && jeu->nbJoueur <= 24){
        ecart=10;
    }

    SDL_Surface *title = TTF_RenderUTF8_Solid(font, "Tour de la voyante", whiteColor);
    SDL_Texture *txr_title = SDL_CreateTextureFromSurface(renderer, title);
    SDL_Rect rect_title = {LONG_WIN/2 - 100, 10, 200, 75};
    SDL_FreeSurface(title);

    SDL_Surface *info = TTF_RenderUTF8_Solid(font, "Sélectionner le prénom d'une personne pour voir sa carte", whiteColor);
    SDL_Texture *txr_info = SDL_CreateTextureFromSurface(renderer, info);
    SDL_Rect rect_info = {LONG_WIN/2 - 250, rect_title.y+rect_title.h, 500, 50};
    SDL_FreeSurface(info);

    SDL_Surface *confirmer = TTF_RenderUTF8_Solid(font, "Confirmer", lightgrayColor);
    SDL_Texture *txr_confirmer = SDL_CreateTextureFromSurface(renderer, confirmer);
    SDL_Rect rect_confirmer = {LONG_WIN/2-75, LARG_WIN-75-30, 150, 75};
    SDL_FreeSurface(confirmer);

    SDL_Surface *tab_noms[jeu->nbJoueur];
    SDL_Texture *tab_txr_noms[jeu->nbJoueur];
    SDL_Rect tab_rect_nom[jeu->nbJoueur];

    int etat_boutons[jeu->nbJoueur];
    for(int i=0; i<jeu->nbJoueur; i++){
        etat_boutons[i] = 0;
    }
    int bouton_selected = -1;

    for(int i=0; i<jeu->nbJoueur; i++){
        tab_noms[i] = TTF_RenderUTF8_Solid(font, jeu->joueurs[i].nom, whiteColor);
        tab_txr_noms[i] = SDL_CreateTextureFromSurface(renderer, tab_noms[i]);
        tab_rect_nom[i].x = LONG_WIN*(4+4*iCol++)/20-strlen(jeu->joueurs[i].nom)*10;
        tab_rect_nom[i].y = LARG_WIN*(iLigne+1)/ecart+20;
        tab_rect_nom[i].w = strlen(jeu->joueurs[i].nom)*20;
        tab_rect_nom[i].h = 40;
        SDL_FreeSurface(tab_noms[i]);
        if(i%4==3){
            iLigne++;
            iCol=0;
        }
    }

    int run=1;
    int x, y;
    while(run){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT: 
                    run=0; 
                    break;

                case SDL_MOUSEBUTTONUP:
                    SDL_GetMouseState(&x,&y);
                    for(int i=0; i<jeu->nbJoueur; i++){
                        if(x>=tab_rect_nom[i].x && x<=tab_rect_nom[i].x+tab_rect_nom[i].w && y>=tab_rect_nom[i].y && y<=tab_rect_nom[i].y+tab_rect_nom[i].h){
                            if(bouton_selected == -1 && etat_boutons[i] == 0){
                                etat_boutons[i] = 1;
                                bouton_selected = i;
                            }
                            else if(etat_boutons[i] == 1){
                                etat_boutons[i] = 0;
                                bouton_selected = -1;
                            }
                        }
                    }
                    if(bouton_selected != -1  && x>=rect_confirmer.x && x<=rect_confirmer.x+rect_confirmer.w && y>=rect_confirmer.y && y<=rect_confirmer.y+rect_confirmer.h){
                        run = 0;
                    }
            }
        }
        SDL_SetRenderDrawColor(renderer,15,5,107,255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, txr_title, NULL, &rect_title);
        SDL_RenderCopy(renderer, txr_info, NULL, &rect_info);
        for(int i=0; i<jeu->nbJoueur; i++){
            if(jeu->joueurs[i].etat == VIVANT){
                if(etat_boutons[i] == 1){
                    SDL_SetRenderDrawColor(renderer, 139, 0, 0, 255);
                    SDL_RenderFillRect(renderer, &tab_rect_nom[i]);
                }
                else{
                    SDL_SetRenderDrawColor(renderer,15,5,107,255);
                    SDL_RenderFillRect(renderer, &tab_rect_nom[i]);
                }
                SDL_SetRenderDrawColor(renderer,15,5,107,255);
                SDL_RenderCopy(renderer, tab_txr_noms[i], NULL, &tab_rect_nom[i]);
            }
        }
        if(bouton_selected != -1){
            SDL_RenderCopy(renderer, txr_confirmer, NULL, &rect_confirmer);
        }
        SDL_RenderPresent(renderer);
    }

    SDL_Surface *carte = IMG_Load(jeu->joueurs[bouton_selected].nomIMG);
    SDL_Texture *txr_carte = SDL_CreateTextureFromSurface(renderer, carte);
    SDL_Rect rect_carte = {LONG_WIN/2-200, LARG_WIN/2-200, 400, 400};
    SDL_FreeSurface(carte);

    SDL_Surface *nom = TTF_RenderUTF8_Solid(font, jeu->joueurs[bouton_selected].nom, whiteColor);
    SDL_Texture *txr_nom = SDL_CreateTextureFromSurface(renderer, nom);
    SDL_Rect rect_nom = {LONG_WIN/2 - strlen(jeu->joueurs[bouton_selected].nom)*10, rect_carte.y - 60, strlen(jeu->joueurs[bouton_selected].nom)*20, 50};
    SDL_FreeSurface(nom);
    
    SDL_SetRenderDrawColor(renderer,15,5,107,255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, txr_title, NULL, &rect_title);
    SDL_RenderCopy(renderer, txr_nom, NULL, &rect_nom);
    SDL_RenderCopy(renderer, txr_carte, NULL, &rect_carte);
    SDL_RenderPresent(renderer);

    int tick = SDL_GetTicks();
    while(SDL_GetTicks()-tick<5000);

    for(int i=0; i<jeu->nbJoueur; i++){
        SDL_DestroyTexture(tab_txr_noms[i]);
    }
    SDL_DestroyTexture(txr_confirmer);
    SDL_DestroyTexture(txr_nom);
    SDL_DestroyTexture(txr_title);
    SDL_DestroyTexture(txr_carte);
    SDL_DestroyTexture(txr_info);
}

/**
 * @brief Réalise le tour de jeu des amoureux, permet de s'entre-identifier.
 * @fn tourAmoureux
 * @param jeu 
 * @param renderer 
 * @param font 
 */
void tourAmoureux(jeu_t * jeu, SDL_Renderer * renderer, TTF_Font * font){
    SDL_Surface *title = TTF_RenderUTF8_Solid(font, "Tour de Cupidon", whiteColor);
    SDL_Texture *txr_title = SDL_CreateTextureFromSurface(renderer, title);
    SDL_Rect rect_title = {LONG_WIN/2 - 100, 10, 200, 75};
    SDL_FreeSurface(title);

    SDL_Surface *info = TTF_RenderUTF8_Solid(font, "Voici le numéro des deux amoureux", whiteColor);
    SDL_Texture *txr_info = SDL_CreateTextureFromSurface(renderer, info);
    SDL_Rect rect_info = {LONG_WIN/2 - 200, rect_title.y+rect_title.h, 400, 50};
    SDL_FreeSurface(info);

    int i;
    for(i=0; jeu->joueurs[i].statut[0]!=AMOUREUX && jeu->joueurs[i].statut[1]!=AMOUREUX; i++);

    SDL_Surface *carte1 = IMG_Load("../img/dos_carte.jpg");
    SDL_Texture *txr_carte1 = SDL_CreateTextureFromSurface(renderer, carte1);
    SDL_Rect rect_carte1 = {LONG_WIN/3-150, LARG_WIN/2-150, 300, 300};
    SDL_FreeSurface(carte1);

    SDL_Surface *carte2 = IMG_Load("../img/dos_carte.jpg");
    SDL_Texture *txr_carte2 = SDL_CreateTextureFromSurface(renderer, carte2);
    SDL_Rect rect_carte2 = {LONG_WIN*2/3-150, LARG_WIN/2-150, 300, 300};
    SDL_FreeSurface(carte2);

    char * strNumAmour1 = malloc(sizeof(char)*2);
    itoa( jeu->joueurs[i].num, strNumAmour1);
    SDL_Surface *NumAmour1 = TTF_RenderUTF8_Solid(font, strNumAmour1, whiteColor);
    SDL_Texture *txr_NumAmour1 = SDL_CreateTextureFromSurface(renderer, NumAmour1);
    SDL_Rect rect_NumAmour1 = {rect_carte1.x+rect_carte1.w/2-60, rect_carte1.y+rect_carte1.h/2-100, 100, 200};
    SDL_FreeSurface(NumAmour1);

    for(i=i+1; jeu->joueurs[i].statut[0]!=AMOUREUX && jeu->joueurs[i].statut[1]!=AMOUREUX; i++);

    char * strNumAmour2 = malloc(sizeof(char)*2);
    itoa( jeu->joueurs[i].num, strNumAmour2);
    SDL_Surface *NumAmour2 = TTF_RenderUTF8_Solid(font, strNumAmour2, whiteColor);
    SDL_Texture *txr_NumAmour2 = SDL_CreateTextureFromSurface(renderer, NumAmour2);
    SDL_Rect rect_NumAmour2 = {rect_carte2.x+rect_carte2.w/2-60, rect_carte2.y+rect_carte2.h/2-100, 100, 200};
    SDL_FreeSurface(NumAmour2);

    SDL_SetRenderDrawColor(renderer,15,5,107,255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, txr_title, NULL, &rect_title);
    SDL_RenderCopy(renderer, txr_info, NULL, &rect_info);
    SDL_RenderCopy(renderer, txr_carte1, NULL, &rect_carte1);
    SDL_RenderCopy(renderer, txr_NumAmour1, NULL, &rect_NumAmour1);
    SDL_RenderCopy(renderer, txr_carte2, NULL, &rect_carte2);
    SDL_RenderCopy(renderer, txr_NumAmour2, NULL, &rect_NumAmour2);
    SDL_RenderPresent(renderer);

    int tick = SDL_GetTicks();
    while(SDL_GetTicks()-tick<15000);

    Mix_Chunk * amour_rev = Mix_LoadWAV("../sound/voice/amoureux2.wav");
    Mix_VolumeChunk(amour_rev, MIX_MAX_VOLUME/2); 
    Mix_PlayChannel(1, amour_rev, 0);

    tick = SDL_GetTicks();
    while(SDL_GetTicks()-tick<5000);

    Mix_Chunk * amour_vue = Mix_LoadWAV("../sound/voice/amoureux3.wav");
    Mix_VolumeChunk(amour_vue, MIX_MAX_VOLUME/2); 
    Mix_PlayChannel(1, amour_vue, 0);

    tick = SDL_GetTicks();
    while(SDL_GetTicks()-tick<10000);

    SDL_DestroyTexture(txr_title);
    SDL_DestroyTexture(txr_info);
    SDL_DestroyTexture(txr_carte1);
    SDL_DestroyTexture(txr_carte2);
    SDL_DestroyTexture(txr_NumAmour1);
    SDL_DestroyTexture(txr_NumAmour2);
}

/**
 * @brief Réalise le tour de jeu de Cupidon. Il selectionne deux joueurs qui seront les amoureux.
 * @fn tourCupidon
 * @param jeu 
 * @param renderer 
 * @param font 
 */
void tourCupidon(jeu_t * jeu, SDL_Renderer * renderer, TTF_Font * font){
    int iLigne=1, iCol=0, ecart;
    if(jeu->nbJoueur > 4 && jeu->nbJoueur <= 8){
        ecart=5;
    }
    else if(jeu->nbJoueur > 8 && jeu->nbJoueur <= 12){
        ecart=6;
    }
    else if(jeu->nbJoueur > 12 && jeu->nbJoueur <= 16){
        ecart=7;
    }
    else if(jeu->nbJoueur > 16 && jeu->nbJoueur <= 20){
        ecart=8;
    }
    else if(jeu->nbJoueur > 20 && jeu->nbJoueur <= 24){
        ecart=10;
    }
    
    SDL_Surface *title = TTF_RenderUTF8_Solid(font, "Tour de Cupidon", whiteColor);
    SDL_Texture *txr_title = SDL_CreateTextureFromSurface(renderer, title);
    SDL_Rect rect_title = {LONG_WIN/2 - 100, 10, 200, 75};
    SDL_FreeSurface(title);

    SDL_Surface *info = TTF_RenderUTF8_Solid(font, "Sélectionner le prenom des deux joueurs à marier", whiteColor);
    SDL_Texture *txr_info = SDL_CreateTextureFromSurface(renderer, info);
    SDL_Rect rect_info = {LONG_WIN/2 - 200, rect_title.y+rect_title.h, 400, 50};
    SDL_FreeSurface(info);

    SDL_Surface *confirmer = TTF_RenderUTF8_Solid(font, "Confirmer", lightgrayColor);
    SDL_Texture *txr_confirmer = SDL_CreateTextureFromSurface(renderer, confirmer);
    SDL_Rect rect_confirmer = {LONG_WIN/2-75, LARG_WIN-75-30, 150, 75};
    SDL_FreeSurface(confirmer);

    SDL_Surface *tab_noms[jeu->nbJoueur];
    SDL_Texture *tab_txr_noms[jeu->nbJoueur];
    SDL_Rect tab_rect_nom[jeu->nbJoueur];

    int etat_boutons[jeu->nbJoueur];
    for(int i=0; i<jeu->nbJoueur; i++){
        etat_boutons[i] = 0;
    }
    int bouton_selected[2] = {-1,-1};

    for(int i=0; i<jeu->nbJoueur; i++){
        if(jeu->joueurs[i].etat == VIVANT){
            tab_noms[i] = TTF_RenderUTF8_Solid(font, jeu->joueurs[i].nom, whiteColor);
            tab_txr_noms[i] = SDL_CreateTextureFromSurface(renderer, tab_noms[i]);
            tab_rect_nom[i].x = LONG_WIN*(4+4*iCol++)/20-strlen(jeu->joueurs[i].nom)*10;
            tab_rect_nom[i].y = LARG_WIN*(iLigne+1)/ecart+20;
            tab_rect_nom[i].w = strlen(jeu->joueurs[i].nom)*20;
            tab_rect_nom[i].h = 40;
            SDL_FreeSurface(tab_noms[i]);
            if(i%4==3){
                iLigne++;
                iCol=0;
            }
        }
    }

    int run=1;
    int x, y;
    while(run){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT: 
                    run=0; 
                    break;

                case SDL_MOUSEBUTTONUP:
                    SDL_GetMouseState(&x,&y);
                    for(int i=0; i<jeu->nbJoueur; i++){
                        if(jeu->joueurs[i].etat == VIVANT && x>=tab_rect_nom[i].x && x<=tab_rect_nom[i].x+tab_rect_nom[i].w && y>=tab_rect_nom[i].y && y<=tab_rect_nom[i].y+tab_rect_nom[i].h){
                            if((bouton_selected[0] == -1 || bouton_selected[1] == -1) && etat_boutons[i] == 0){
                                etat_boutons[i] = 1;
                                if(bouton_selected[0] == -1){
                                    bouton_selected[0] = i;
                                }
                                else{
                                    bouton_selected[1] = i;
                                }
                            }
                            else if(etat_boutons[i] == 1){
                                etat_boutons[i] = 0;
                                if(bouton_selected[0] == i){
                                    bouton_selected[0] = -1;
                                }
                                else{
                                    bouton_selected[1] = -1;
                                }
                            }
                        }
                    }

                    if(bouton_selected[0] != -1 && bouton_selected[1] != -1 && x>=rect_confirmer.x && x<=rect_confirmer.x+rect_confirmer.w && y>=rect_confirmer.y && y<=rect_confirmer.y+rect_confirmer.h){
                        run = 0;
                        if(jeu->joueurs[bouton_selected[0]].statut[0] == AUCUN){
                            jeu->joueurs[bouton_selected[0]].statut[0] = AMOUREUX;
                        }
                        else{
                            jeu->joueurs[bouton_selected[0]].statut[1] = AMOUREUX;
                        }

                        if(jeu->joueurs[bouton_selected[1]].statut[0] == AUCUN){
                            jeu->joueurs[bouton_selected[1]].statut[0] = AMOUREUX;
                        }
                        else{
                            jeu->joueurs[bouton_selected[1]].statut[1] = AMOUREUX;
                        }
                    }
            }
        }
        SDL_SetRenderDrawColor(renderer,15,5,107,255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, txr_title, NULL, &rect_title);
        SDL_RenderCopy(renderer, txr_info, NULL, &rect_info);
        for(int i=0; i<jeu->nbJoueur; i++){
            if(jeu->joueurs[i].etat == VIVANT){
                if(etat_boutons[i] == 1){
                    SDL_SetRenderDrawColor(renderer, 139, 0, 0, 255);
                    SDL_RenderFillRect(renderer, &tab_rect_nom[i]);
                }
                else{
                    SDL_SetRenderDrawColor(renderer,15,5,107,255);
                    SDL_RenderFillRect(renderer, &tab_rect_nom[i]);
                }
                SDL_SetRenderDrawColor(renderer,15,5,107,255);
                SDL_RenderCopy(renderer, tab_txr_noms[i], NULL, &tab_rect_nom[i]);
            }
        }
        if(bouton_selected[0] != -1 && bouton_selected[1] != -1){
            SDL_RenderCopy(renderer, txr_confirmer, NULL, &rect_confirmer);
        }
        SDL_RenderPresent(renderer);
    }

    for(int i=0; i<jeu->nbJoueur; i++){
        if(jeu->joueurs[i].etat == VIVANT){
            SDL_DestroyTexture(tab_txr_noms[i]);
        }
    }
    SDL_DestroyTexture(txr_title);
    SDL_DestroyTexture(txr_confirmer);
    SDL_DestroyTexture(txr_info);
}

/**
 * @brief Réalise le tour de jeu de la Sorcière. Si elle clique sur le bouton resusciter, le joueur décédé precedement revient à la vie. Si elle clique sur tuer, appel de la fonction "selectNom". Sinon passage du tour.
 * @fn tourSorciere
 * @param jeu 
 * @param renderer 
 * @param font 
 * @param joueurMort 
 * @return int 
 */
int tourSorciere(jeu_t * jeu, SDL_Renderer * renderer, TTF_Font * font, joueur_t * joueurMort){
    SDL_Surface *title = TTF_RenderUTF8_Solid(font, "Tour de la Sorcière", whiteColor);
    SDL_Texture *txr_title = SDL_CreateTextureFromSurface(renderer, title);
    SDL_Rect rect_title = {LONG_WIN/2 - 100, 10, 250, 75};
    SDL_FreeSurface(title);

    SDL_Surface *info = TTF_RenderUTF8_Solid(font, "Cette personne a été tuée par les Loups-Garous :", whiteColor);
    SDL_Texture *txr_info = SDL_CreateTextureFromSurface(renderer, info);
    SDL_Rect rect_info = {LONG_WIN/2 - 250, rect_title.y+rect_title.h, 500, 40};
    SDL_FreeSurface(info);

    SDL_Surface *nom = TTF_RenderUTF8_Solid(font, joueurMort->nom, whiteColor);
    SDL_Texture *txr_nom = SDL_CreateTextureFromSurface(renderer, nom);
    SDL_Rect rect_nom = {LONG_WIN/2 - strlen(joueurMort->nom)*10, rect_info.y+rect_info.h, strlen(joueurMort->nom)*20, 50};
    SDL_FreeSurface(nom);

    SDL_Surface *img_vie;
    if(jeu->potionsSorciere[0] > 0){
        img_vie = IMG_Load("../img/potion_soin.png");
    }
    else{
        img_vie = IMG_Load("../img/potion_soin_black.png");
    }
    SDL_Texture *txr_img_vie = SDL_CreateTextureFromSurface(renderer, img_vie);
    SDL_Rect rect_img_vie = {LONG_WIN/3-100, LARG_WIN/2-100, 200, 200};
    SDL_FreeSurface(img_vie);

    SDL_Surface *vie = TTF_RenderUTF8_Solid(font, "Ressusciter", whiteColor);
    SDL_Texture *txr_vie = SDL_CreateTextureFromSurface(renderer, vie);
    SDL_Rect rect_vie = {LONG_WIN/3-strlen("Ressusciter")*7, rect_img_vie.y+rect_img_vie.h, strlen("Ressusciter")*14, 40};
    SDL_FreeSurface(vie);

    SDL_Surface *img_mort;
    if(jeu->potionsSorciere[1] > 0){
        img_mort = IMG_Load("../img/potion_mort.png");
    }
    else{
        img_mort = IMG_Load("../img/potion_mort_black.png");
    }
    SDL_Texture *txr_img_mort = SDL_CreateTextureFromSurface(renderer, img_mort);
    SDL_Rect rect_img_mort = {LONG_WIN*2/3-100, LARG_WIN/2-100, 200, 200};
    SDL_FreeSurface(img_mort);

    SDL_Surface *mort = TTF_RenderUTF8_Solid(font, "Tuer un autre joueur", whiteColor);
    SDL_Texture *txr_mort = SDL_CreateTextureFromSurface(renderer, mort);
    SDL_Rect rect_mort = {LONG_WIN*2/3-strlen("Tuer un autre joueur")*7, rect_img_mort.y+rect_img_mort.h, strlen("Tuer un autre joueur")*14, 40};
    SDL_FreeSurface(mort);

    SDL_Surface *rf = TTF_RenderUTF8_Solid(font, "Ne rien faire", whiteColor);
    SDL_Texture *txr_rf = SDL_CreateTextureFromSurface(renderer, rf);
    SDL_Rect rect_rf = {LONG_WIN/2-strlen("Ne rien faire")*7, LARG_WIN*3/4, strlen("Ne rien faire")*14, 40};
    SDL_FreeSurface(rf);

    int retour=-2;
    int run=1;
    int x, y;
    while(run){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT: 
                    run=0; 
                    break;

                case SDL_MOUSEBUTTONUP:
                    SDL_GetMouseState(&x,&y);
                    if(x>=rect_rf.x && x<=rect_rf.x+rect_rf.w && y>=rect_rf.y && y<=rect_rf.y+rect_rf.h){
                        retour = -1;
                        run = 0;
                    }
                    else if(jeu->potionsSorciere[1]>0 && ((x>=rect_mort.x && x<=rect_mort.x+rect_mort.w && y>=rect_mort.y && y<=rect_mort.y+rect_mort.h)||(x>=rect_img_mort.x && x<=rect_img_mort.x+rect_img_mort.w && y>=rect_img_mort.y && y<=rect_img_mort.y+rect_img_mort.h))){
                        jeu->potionsSorciere[1] = 0;
                        retour = selectNom(jeu, renderer, font, SORCIERE);
                        if(retour>-1){
                            run = 0;
                        }
                    }
                    else if(jeu->potionsSorciere[0]>0 && ((x>=rect_vie.x && x<=rect_vie.x+rect_vie.w && y>=rect_vie.y && y<=rect_vie.y+rect_vie.h)||(x>=rect_img_vie.x && x<=rect_img_vie.x+rect_img_vie.w && y>=rect_img_vie.y && y<=rect_img_vie.y+rect_img_vie.h))){
                        jeu->potionsSorciere[0] = 0;
                        joueurMort->etat = VIVANT;
                        retour = -2;
                        run = 0;
                    }
            }
        }

        SDL_SetRenderDrawColor(renderer,15,5,107,255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, txr_title, NULL, &rect_title);
        SDL_RenderCopy(renderer, txr_info, NULL, &rect_info);
        SDL_RenderCopy(renderer, txr_nom, NULL, &rect_nom);
        SDL_RenderCopy(renderer, txr_img_vie, NULL, &rect_img_vie);
        SDL_RenderCopy(renderer, txr_img_mort, NULL, &rect_img_mort);
        if(jeu->potionsSorciere[0] > 0){
            SDL_RenderCopy(renderer, txr_vie, NULL, &rect_vie);
        }
        if(jeu->potionsSorciere[1] > 0){
            SDL_RenderCopy(renderer, txr_mort, NULL, &rect_mort);
        }
        SDL_RenderCopy(renderer, txr_rf, NULL, &rect_rf);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(txr_title);
    SDL_DestroyTexture(txr_info);
    SDL_DestroyTexture(txr_img_mort);
    SDL_DestroyTexture(txr_img_vie);
    SDL_DestroyTexture(txr_rf);
    SDL_DestroyTexture(txr_mort);
    SDL_DestroyTexture(txr_vie);
    SDL_DestroyTexture(txr_nom);

    return retour;
}

/**
 * @brief trouverVoleur
 * @fn Fonction permettant de trouver le voleur parmis les joueur et vérifie ainsi si il existe.
 * @param jeu 
 * @return int 
 */
int trouverVoleur(jeu_t * jeu){
    for(int i=0; i<jeu->nbJoueur; i++){
        if(jeu->joueurs[i].role == VOLEUR){
            return i;
        }
    }
    return -1;
}

/**
 * @brief Réalise le tour de jeu du Voleur. Selectionne l'une des deux cartes en face de lui, qui deviendra son futur role jusqu'à la fin de la partie.
 * @fn tourVoleur
 * @param jeu 
 * @param renderer 
 * @param font 
 */
void tourVoleur(jeu_t * jeu, SDL_Renderer * renderer, TTF_Font * font){
    char nomCarte1[50], nomCarte2[50];
    int run=1;

    switch(jeu->piocheVoleur[0].role){
        case NONE : printf("Erreur : carte sans role\n"); run = 0; break;
        case LOUPGAROU : strcpy(nomCarte1,"Loup-Garou"); break;
        case VILLAGEOIS : strcpy(nomCarte1,"Villageois"); break;
        case CHASSEUR : strcpy(nomCarte1,"Chasseur"); break;
        case VOYANTE : strcpy(nomCarte1,"Voyante"); break;
        case CUPIDON : strcpy(nomCarte1,"Cupidon"); break;
        case SORCIERE : strcpy(nomCarte1,"Sorcière"); break;
        case PETITEFILLE : strcpy(nomCarte1,"Petite-Fille"); break;
        case VOLEUR : strcpy(nomCarte1,"Voleur"); break;
    }
    switch(jeu->piocheVoleur[1].role){
        case NONE : printf("Erreur : carte sans role\n"); run = 0; break;
        case LOUPGAROU : strcpy(nomCarte2,"Loup-Garou"); break;
        case VILLAGEOIS : strcpy(nomCarte2,"Villageois"); break;
        case CHASSEUR : strcpy(nomCarte2,"Chasseur"); break;
        case VOYANTE : strcpy(nomCarte2,"Voyante"); break;
        case CUPIDON : strcpy(nomCarte2,"Cupidon"); break;
        case SORCIERE : strcpy(nomCarte2,"Sorcière"); break;
        case PETITEFILLE : strcpy(nomCarte2,"Petite-Fille"); break;
        case VOLEUR : strcpy(nomCarte2,"Voleur"); break;
    }

    SDL_Surface *title = TTF_RenderUTF8_Solid(font, "Tour du Voleur", whiteColor);
    SDL_Texture *txr_title = SDL_CreateTextureFromSurface(renderer, title);
    SDL_Rect rect_title = {LONG_WIN/2 - 100, 10, 200, 75};
    SDL_FreeSurface(title);

    SDL_Surface *info = TTF_RenderUTF8_Solid(font, "Choisissez l'un de ces deux cartes pour découvrir votre nouveau rôle !", whiteColor);
    SDL_Texture *txr_info = SDL_CreateTextureFromSurface(renderer, info);
    SDL_Rect rect_info = {LONG_WIN/2-225, rect_title.y+rect_title.h+30, 500, 40};
    SDL_FreeSurface(info);
    
    SDL_Surface *carte1 = IMG_Load("../img/dos_carte.jpg");
    SDL_Texture *txr_carte1 = SDL_CreateTextureFromSurface(renderer, carte1);
    SDL_Rect rect_carte1 = {LONG_WIN/3-150, LARG_WIN/2-150, 300, 300};
    SDL_FreeSurface(carte1);

    SDL_Surface *txt_carte1 = TTF_RenderUTF8_Solid(font, nomCarte1, whiteColor);
    SDL_Texture *txr_txt_carte1 = SDL_CreateTextureFromSurface(renderer, txt_carte1);
    SDL_Rect rect_txt_carte1 = {LONG_WIN/3 - strlen(nomCarte1)*10, rect_carte1.y+rect_carte1.h+30, strlen(nomCarte1)*20, 75};
    SDL_FreeSurface(txt_carte1);

    SDL_Surface *carte2 = IMG_Load("../img/dos_carte.jpg");
    SDL_Texture *txr_carte2 = SDL_CreateTextureFromSurface(renderer, carte2);
    SDL_Rect rect_carte2 = {LONG_WIN*2/3-150, LARG_WIN/2-150, 300, 300};
    SDL_FreeSurface(carte2);

    SDL_Surface *txt_carte2 = TTF_RenderUTF8_Solid(font, nomCarte2, whiteColor);
    SDL_Texture *txr_txt_carte2 = SDL_CreateTextureFromSurface(renderer, txt_carte2);
    SDL_Rect rect_txt_carte2 = {LONG_WIN*2/3 - strlen(nomCarte2)*10, rect_carte2.y+rect_carte2.h+30, strlen(nomCarte2)*20, 75};
    SDL_FreeSurface(txt_carte2);

    int tick;
    int card_click=0;
    int x, y;
    int iVoleur = trouverVoleur(jeu);
    while(run){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT: 
                    run=0; 
                    break;

                case SDL_MOUSEBUTTONUP:
                    SDL_GetMouseState(&x,&y);
                    if(x>=rect_carte1.x && x<=rect_carte1.x+rect_carte1.w && y>=rect_carte1.y && y<=rect_carte1.y+rect_carte1.h && card_click==0){
                        card_click=1;
                        SDL_DestroyTexture(txr_carte1);
                        carte1 = IMG_Load(jeu->piocheVoleur[0].nomIMG);
                        txr_carte1 = SDL_CreateTextureFromSurface(renderer, carte1);
                        SDL_FreeSurface(carte1);

                        jeu->joueurs[iVoleur].role = jeu->piocheVoleur[0].role;
                        if(realloc(jeu->joueurs[iVoleur].nomIMG, sizeof(char)*50) != NULL){
                            strcpy(jeu->joueurs[iVoleur].nomIMG, jeu->piocheVoleur[0].nomIMG);
                        }
                        tick = SDL_GetTicks();
                    }
                    else if(x>=rect_carte2.x && x<=rect_carte2.x+rect_carte2.w && y>=rect_carte2.y && y<=rect_carte2.y+rect_carte2.h && card_click==0){
                        card_click=2;
                        SDL_DestroyTexture(txr_carte2);
                        carte2 = IMG_Load(jeu->piocheVoleur[1].nomIMG);
                        txr_carte2 = SDL_CreateTextureFromSurface(renderer, carte2);
                        SDL_FreeSurface(carte2);

                        jeu->joueurs[iVoleur].role = jeu->piocheVoleur[1].role;
                        if(realloc(jeu->joueurs[iVoleur].nomIMG, sizeof(char)*50) != NULL){
                            strcpy(jeu->joueurs[iVoleur].nomIMG, jeu->piocheVoleur[1].nomIMG);
                        }

                        tick = SDL_GetTicks();
                    }
                default: break;
            }
            if(card_click!=0 && SDL_GetTicks()-tick>1000){
                run = 0;
            }
        }
        SDL_SetRenderDrawColor(renderer,15,5,107,255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, txr_title, NULL, &rect_title);
        SDL_RenderCopy(renderer, txr_info, NULL, &rect_info);
        SDL_RenderCopy(renderer, txr_carte1, NULL, &rect_carte1);
        SDL_RenderCopy(renderer, txr_carte2, NULL, &rect_carte2);
        if(card_click==1){
            SDL_RenderCopy(renderer, txr_txt_carte1, NULL, &rect_txt_carte1);
        }
        else if(card_click==2){
            SDL_RenderCopy(renderer, txr_txt_carte2, NULL, &rect_txt_carte2);
        }
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyTexture(txr_carte1);
    SDL_DestroyTexture(txr_carte2);
    SDL_DestroyTexture(txr_info);
    SDL_DestroyTexture(txr_title);
    SDL_DestroyTexture(txr_txt_carte1);
    SDL_DestroyTexture(txr_txt_carte2);   
}