#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../head/jeu.h"

void aff_roles(jeu_t * jeu){
    for(int i=0; i<jeu->nbJoueur; i++){
        printf("Joueur n°%d : %s ", i+1, jeu->joueurs[i].nom);
        switch(jeu->joueurs[i].role){
            case LOUPGAROU: printf("[LOUP-GAROU]\n"); break;
            case VILLAGEOIS: printf("[VILLAGEOIS]\n"); break;
            case CHASSEUR: printf("[CHASSEUR]\n"); break;
            case VOYANTE: printf("[VOYANTE]\n"); break;
            case CUPIDON: printf("[CUPIDON]\n"); break;
            case SORCIERE: printf("[SORCIERE]\n"); break;
            case PETITEFILLE: printf("[PETITE-FILLE]\n"); break;
            case VOLEUR: printf("[VOLEUR]\n"); break;
            default: break;
        }
    }
}

int parmisJoueurVivant(jeu_t * jeu, Roles role){
    for(int i=0; i<jeu->nbJoueur; i++){
        if(jeu->joueurs[i].role == role && jeu->joueurs[i].etat == VIVANT){
            return 1;
        }
    }
    return 0;
}

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
            if(role == LOUPGAROU && delay>=15000 && delay<=17000){
                printf("%d\n", delay);
                Mix_PlayChannel(2, lg_speed_up, 0);
                printf("Dépèche\n");
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

    return bouton_selected;
}

int tourLoupGarou(jeu_t * jeu, SDL_Renderer * renderer, TTF_Font * font){
    return selectNom(jeu, renderer, font, LOUPGAROU);
}

int tourChasseur(jeu_t * jeu, SDL_Renderer * renderer, TTF_Font * font){
    return selectNom(jeu, renderer, font, CHASSEUR);
}

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

    int run=1;
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

int trouverVoleur(jeu_t * jeu){
    for(int i=0; i<jeu->nbJoueur; i++){
        if(jeu->joueurs[i].role == VOLEUR){
            return i;
        }
    }
    return -1;
}

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

int nuit(int tour, jeu_t * jeu, SDL_Renderer * renderer, TTF_Font *font, mort_t * mortNuit){
    int retour;

    Mix_Music * music_nuit = Mix_LoadMUS("../sound/music/music_nuit.mp3");
    Mix_VolumeMusic(MIX_MAX_VOLUME*1/8);
    Mix_PlayMusic(music_nuit, -1);

    Mix_AllocateChannels(3);
    Mix_Volume(2, MIX_MAX_VOLUME);
    Mix_Volume(1, MIX_MAX_VOLUME);
    Mix_Volume(0, MIX_MAX_VOLUME); 

    Mix_Chunk * info_nuit = Mix_LoadWAV("../sound/voice/debut_nuit.wav");

    SDL_Surface *title = TTF_RenderUTF8_Solid(font, "Le village s'endort", blackColor);
    SDL_Texture *txr_title = SDL_CreateTextureFromSurface(renderer, title);
    SDL_Rect rect_title = {LONG_WIN/2 - 125, LARG_WIN/2 - 37, 250, 75};
    SDL_FreeSurface(title);
    
    Mix_PlayChannel(0, info_nuit, 0);
    int tick = SDL_GetTicks();
    int labs;
    int r = 149, g = 56, b = 58;
    while((labs=SDL_GetTicks() - tick) < 5000){
        if(labs%103 >= 0 && labs%103 <= 20 && b<107){
            b++;
        }
        if(labs%98 >= 0 && labs%98 <= 20 && g>5){
            g--;
        }
        if(labs%37 >= 0 && labs%37 <= 20 && r>15){
            r--;
        }

        if(labs >= 2500 && labs <= 2600){
            SDL_DestroyTexture(txr_title);
            SDL_Surface *title = TTF_RenderUTF8_Solid(font, "Le village s'endort", whiteColor);
            txr_title = SDL_CreateTextureFromSurface(renderer, title);
            SDL_FreeSurface(title);
        }
        SDL_SetRenderDrawColor(renderer,r,g,b,255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, txr_title, NULL, &rect_title);
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyTexture(txr_title);

    while(Mix_Playing(0));
    Mix_FreeChunk(info_nuit);


    if(tour==1 && jeu->nbVoleur>0 && parmisJoueurVivant(jeu, VOLEUR)){
        Mix_Chunk * voleur_rev = Mix_LoadWAV("../sound/voice/voleur_rev.wav");
        Mix_VolumeChunk(voleur_rev, MIX_MAX_VOLUME/2); 
        Mix_PlayChannel(1, voleur_rev, 0);

        tourVoleur(jeu, renderer, font);

        Mix_Chunk * voleur_som = Mix_LoadWAV("../sound/voice/voleur_som.wav");
        Mix_VolumeChunk(voleur_som, MIX_MAX_VOLUME/2); 
        Mix_PlayChannel(1, voleur_som, 0);

        while(Mix_Playing(1));
        Mix_FreeChunk(voleur_rev);
        Mix_FreeChunk(voleur_som);

    }

    if(tour==1 && jeu->nbCupidon>0 && parmisJoueurVivant(jeu, CUPIDON)){
        tick = SDL_GetTicks();
        while(SDL_GetTicks() - tick < 2000);

        Mix_Chunk * cupidon_rev = Mix_LoadWAV("../sound/voice/cupidon_rev1.wav");
        Mix_VolumeChunk(cupidon_rev, MIX_MAX_VOLUME/2); 
        Mix_PlayChannel(1, cupidon_rev, 0);

        tourCupidon(jeu, renderer, font);

        Mix_Chunk * cupidon_som = Mix_LoadWAV("../sound/voice/cupidon_som.wav");
        Mix_VolumeChunk(cupidon_som, MIX_MAX_VOLUME/2); 
        Mix_PlayChannel(1, cupidon_som, 0);

        while(Mix_Playing(1));

        tick = SDL_GetTicks();
        while(SDL_GetTicks() - tick < 2000);

        Mix_Chunk * amour_rev = Mix_LoadWAV("../sound/voice/amoureux1.wav");
        Mix_VolumeChunk(amour_rev, MIX_MAX_VOLUME/2); 
        Mix_PlayChannel(1, amour_rev, 0);

        tourAmoureux(jeu, renderer, font);

        Mix_Chunk * amour_som = Mix_LoadWAV("../sound/voice/amoureux4.wav");
        Mix_VolumeChunk(amour_som, MIX_MAX_VOLUME/2); 
        Mix_PlayChannel(1, amour_som, 0);
        while(Mix_Playing(1));
        
        Mix_FreeChunk(cupidon_rev);
        Mix_FreeChunk(cupidon_som);
        Mix_FreeChunk(amour_rev);
        Mix_FreeChunk(amour_som);
    }

    if(jeu->nbVoyante>0 && parmisJoueurVivant(jeu, VOYANTE)){
        tick = SDL_GetTicks();
        while(SDL_GetTicks() - tick < 2000);

        Mix_Chunk * voyante_rev = Mix_LoadWAV("../sound/voice/voyante_rev.wav");
        Mix_VolumeChunk(voyante_rev, MIX_MAX_VOLUME/2); 
        Mix_PlayChannel(1, voyante_rev, 0);

        tourVoyante(jeu, renderer, font);

        Mix_Chunk * voyante_som = Mix_LoadWAV("../sound/voice/voyante_som.wav");
        Mix_VolumeChunk(voyante_som, MIX_MAX_VOLUME/2); 
        Mix_PlayChannel(1, voyante_som, 0);

        while(Mix_Playing(1));
        Mix_FreeChunk(voyante_rev);
        Mix_FreeChunk(voyante_som);
    }
    
    if(jeu->nbLoupGarou>0 && parmisJoueurVivant(jeu, LOUPGAROU)){
        tick = SDL_GetTicks();
        while(SDL_GetTicks() - tick < 2000);

        Mix_Chunk * lg_rev = Mix_LoadWAV("../sound/voice/lg_rev.wav");
        Mix_VolumeChunk(lg_rev, MIX_MAX_VOLUME/2); 
        Mix_PlayChannel(1, lg_rev, 0);

        retour = tourLoupGarou(jeu, renderer, font);
        if(retour != -1){
            mortNuit->tab_mort[0] = &jeu->joueurs[retour];
        }

        Mix_Chunk * lg_som = Mix_LoadWAV("../sound/voice/lg_som.wav");
        Mix_VolumeChunk(lg_som, MIX_MAX_VOLUME/2); 
        Mix_PlayChannel(1, lg_som, 0);

        while(Mix_Playing(1));
        Mix_FreeChunk(lg_rev);
        Mix_FreeChunk(lg_som);
    }

    if(jeu->nbSorciere>0 && parmisJoueurVivant(jeu, SORCIERE) && (jeu->potionsSorciere[0]>0 || jeu->potionsSorciere[1]>0)){
        tick = SDL_GetTicks();
        while(SDL_GetTicks() - tick < 2000);

        Mix_Chunk * sorciere_rev;
        if(jeu->potionsSorciere[0] > 0 && jeu->potionsSorciere[1] > 0){
            sorciere_rev = Mix_LoadWAV("../sound/voice/sorciere_rev1.wav");
        }
        else if(jeu->potionsSorciere[0] > 0){
            sorciere_rev = Mix_LoadWAV("../sound/voice/sorciere_rev2.wav");
        }
        else{
            sorciere_rev = Mix_LoadWAV("../sound/voice/sorciere_rev3.wav");
        }
        Mix_VolumeChunk(sorciere_rev, MIX_MAX_VOLUME/2); 
        Mix_PlayChannel(1, sorciere_rev, 0);

        retour = tourSorciere(jeu, renderer, font, mortNuit->tab_mort[0]);
        if(retour >= -1){
            mortNuit->tab_mort[mortNuit->iNbMort]->etat = MORT;
            mortNuit->role_tueur[mortNuit->iNbMort++] = LOUPGAROU;
        }
        if(retour >= 0){
            mortNuit->tab_mort[mortNuit->iNbMort] = &jeu->joueurs[retour];
            mortNuit->tab_mort[mortNuit->iNbMort]->etat = MORT;
            mortNuit->role_tueur[mortNuit->iNbMort++] = SORCIERE;
        }

        Mix_Chunk * sorciere_som = Mix_LoadWAV("../sound/voice/sorciere_som.wav");
        Mix_VolumeChunk(sorciere_som, MIX_MAX_VOLUME/2); 
        Mix_PlayChannel(1, sorciere_som, 0);

        while(Mix_Playing(1));
        Mix_FreeChunk(sorciere_rev);
        Mix_FreeChunk(sorciere_som);

    }
    else{
        mortNuit->tab_mort[mortNuit->iNbMort]->etat = MORT;
        mortNuit->role_tueur[mortNuit->iNbMort++] = LOUPGAROU;
    }

    tick = SDL_GetTicks();
    while(SDL_GetTicks() - tick < 2000);
    
    info_nuit = Mix_LoadWAV("../sound/voice/fin_nuit.wav");

    title = TTF_RenderUTF8_Solid(font, "Le village se reveille", whiteColor);
    txr_title = SDL_CreateTextureFromSurface(renderer, title);
    SDL_FreeSurface(title);

    Mix_PlayChannel(0, info_nuit, 0);

    tick = SDL_GetTicks();
    r = 15, g = 5, b = 107;
    while((labs=SDL_GetTicks() - tick) < 5000){
        if(labs%103 >= 0 && labs%103 <= 20 && b>58){
            b--;
        }
        if(labs%98 >= 0 && labs%98 <= 20 && g<56){
            g++;
        }
        if(labs%37 >= 0 && labs%37 <= 20 && r<149){
            r++;
        }

        if(labs >= 2500 && labs <= 2600){
            SDL_DestroyTexture(txr_title);
            SDL_Surface *title = TTF_RenderUTF8_Solid(font, "Le village se reveille", blackColor);
            txr_title = SDL_CreateTextureFromSurface(renderer, title);
            SDL_FreeSurface(title);
        }
        SDL_SetRenderDrawColor(renderer,r,g,b,255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, txr_title, NULL, &rect_title);
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyTexture(txr_title);

    while(Mix_Playing(0));
    Mix_FreeChunk(info_nuit);

    return 0;
}

int chercherAutreAmour(jeu_t * jeu, int numAmour1, mort_t * mortNuit){
    for(int i=0; i<jeu->nbJoueur; i++){
        if(jeu->joueurs[i].num != numAmour1 && jeu->joueurs[i].etat == VIVANT && (jeu->joueurs[i].statut[0] == AMOUREUX || jeu->joueurs[i].statut[1] == AMOUREUX)){
            jeu->joueurs[i].etat = MORT;
            mortNuit->tab_mort[mortNuit->iNbMort] = &jeu->joueurs[i];
            mortNuit->role_tueur[mortNuit->iNbMort++] = CUPIDON;
            return 1;
        }
    }

    return 0;
}

int verifAmour(mort_t * mortNuit){
    for(int i=0; i<mortNuit->iNbMort; i++){
        if(mortNuit->tab_mort[i]->statut[0] == AMOUREUX || mortNuit->tab_mort[i]->statut[1] == AMOUREUX){
            return mortNuit->tab_mort[i]->num;
        }
    }
    return -1;
}

SDL_Surface * LoadIMGFromRole(joueur_t * joueur){
    switch(joueur->role){
        case NONE : break;
        case LOUPGAROU : return IMG_Load("../img/loup-garou.png");
        case VILLAGEOIS : return IMG_Load("../img/villageois.png");
        case CHASSEUR : return IMG_Load("../img/chasseur.png");
        case VOYANTE : return IMG_Load("../img/voyante.png");
        case CUPIDON : return IMG_Load("../img/cupidon.png");
        case SORCIERE : return IMG_Load("../img/sorciere.png");
        case PETITEFILLE : return IMG_Load("../img/petite-fille.png");
        case VOLEUR : return IMG_Load("../img/voleur.png");
        default : break;
    }
    return NULL;
}

void affMorts(jeu_t * jeu, SDL_Renderer * renderer, TTF_Font *font, mort_t * mortNuit){

    SDL_Surface *info = NULL; 
    SDL_Texture *txr_info;
    SDL_Rect rect_info;

    SDL_Surface *img_mort1; 
    SDL_Texture *txr_img_mort1;
    SDL_Rect rect_img_mort1;

    SDL_Surface *mort1; 
    SDL_Texture *txr_mort1;
    SDL_Rect rect_mort1;
    

    SDL_Surface *img_mort2; 
    SDL_Texture *txr_img_mort2;
    SDL_Rect rect_img_mort2;

    SDL_Surface *mort2; 
    SDL_Texture *txr_mort2;
    SDL_Rect rect_mort2;

    SDL_Surface *img_mort3; 
    SDL_Texture *txr_img_mort3;
    SDL_Rect rect_img_mort3;

    SDL_Surface *mort3; 
    SDL_Texture *txr_mort3;
    SDL_Rect rect_mort3;

    Mix_Chunk * mort = NULL;

    int numAmour1;
    if((numAmour1 = verifAmour(mortNuit)) != -1){
        chercherAutreAmour(jeu, numAmour1, mortNuit);
    }

    SDL_Surface *tueur1 = NULL; 
    SDL_Texture * txr_tueur1;
    SDL_Rect rect_tueur1;
    rect_tueur1.x = rect_img_mort1.x;
    rect_tueur1.y = rect_mort1.y+rect_mort1.h;
    rect_tueur1.h = rect_mort1.h-10;
    if(mortNuit->iNbMort>0 && mortNuit->role_tueur[0] == LOUPGAROU){
        tueur1 = TTF_RenderUTF8_Solid(font, "Tué par les LOUPS-GAROUS", whiteColor);
        rect_tueur1.w = 300;
    }
    else if(mortNuit->iNbMort>0 && mortNuit->role_tueur[0] == SORCIERE){
        tueur1 = TTF_RenderUTF8_Solid(font, "Tué par la SORCIERE", whiteColor);
        rect_tueur1.w = 200;
    }
    else if(mortNuit->iNbMort>0 && mortNuit->role_tueur[0] == CUPIDON){
        tueur1 = TTF_RenderUTF8_Solid(font, "Tué par AMOUR", whiteColor);
        rect_tueur1.w = 150;
    }
    if(tueur1 != NULL){
        txr_tueur1 = SDL_CreateTextureFromSurface(renderer, tueur1);
        SDL_FreeSurface(tueur1);
    }

    SDL_Surface *tueur2 = NULL; 
    SDL_Texture * txr_tueur2;
    SDL_Rect rect_tueur2;
    rect_tueur2.x = rect_img_mort2.x;
    rect_tueur2.y = rect_mort2.y+rect_mort2.h;
    rect_tueur2.h = rect_mort2.h-10;
    if(mortNuit->iNbMort>1 && mortNuit->role_tueur[1] == LOUPGAROU){
        tueur2 = TTF_RenderUTF8_Solid(font, "Tué par les LOUPS-GAROUS", whiteColor);
        rect_tueur2.w = 300;
    }
    else if(mortNuit->iNbMort>1 && mortNuit->role_tueur[1] == SORCIERE){
        tueur2 = TTF_RenderUTF8_Solid(font, "Tué par la SORCIERE", whiteColor);
        rect_tueur2.w = 200;
    }
    else if(mortNuit->iNbMort>1 && mortNuit->role_tueur[1] == CUPIDON){
        tueur2 = TTF_RenderUTF8_Solid(font, "Tué par AMOUR", whiteColor);
        rect_tueur2.w = 150;
    }
    if(tueur2 != NULL){
        txr_tueur2 = SDL_CreateTextureFromSurface(renderer, tueur2);
        SDL_FreeSurface(tueur2);
    }

    SDL_Surface *tueur3 = NULL; 
    SDL_Texture * txr_tueur3;
    SDL_Rect rect_tueur3;
    rect_tueur3.x = rect_img_mort3.x;
    rect_tueur3.y = rect_mort3.y+rect_mort3.h;
    rect_tueur3.h = rect_mort3.h-10;
    if(mortNuit->iNbMort>2 && mortNuit->role_tueur[2] == LOUPGAROU){
        tueur3 = TTF_RenderUTF8_Solid(font, "Tué par les LOUPS-GAROUS", whiteColor);
        rect_tueur3.w = 300;
    }
    else if(mortNuit->iNbMort>2 && mortNuit->role_tueur[2] == SORCIERE){
        tueur3 = TTF_RenderUTF8_Solid(font, "Tué par la SORCIERE", whiteColor);
        rect_tueur3.w = 200;
    }
    else if(mortNuit->iNbMort>2 && mortNuit->role_tueur[2] == CUPIDON){
        tueur3 = TTF_RenderUTF8_Solid(font, "Tué par AMOUR", whiteColor);
        rect_tueur3.w = 150;
    }
    if(tueur1 != NULL){
        txr_tueur3 = SDL_CreateTextureFromSurface(renderer, tueur3);
        SDL_FreeSurface(tueur3);
    }

    if(mortNuit->iNbMort == 0){
        mort = Mix_LoadWAV("../sound/voice/0mort.wav");

        info = TTF_RenderUTF8_Solid(font, "Personne n'est mort cette nuit !", blackColor);
        txr_info = SDL_CreateTextureFromSurface(renderer, info);
        rect_info.x = LONG_WIN/2-150;
        rect_info.y = LARG_WIN/2-25;
        rect_info.w = 300;
        rect_info.h = 50;
    }
    else if(mortNuit->iNbMort == 1){
        mort = Mix_LoadWAV("../sound/voice/1mort.wav");

        info = TTF_RenderUTF8_Solid(font, "1 personne est morte cette nuit !", blackColor);
        txr_info = SDL_CreateTextureFromSurface(renderer, info);
        rect_info.x = LONG_WIN/2-150;
        rect_info.y = 25;
        rect_info.w = 300;
        rect_info.h = 50;
    
        img_mort1 = IMG_Load("../img/dos_carte.jpg");
        txr_img_mort1 = SDL_CreateTextureFromSurface(renderer, img_mort1);
        rect_img_mort1.x = LONG_WIN/2-150;
        rect_img_mort1.y = LARG_WIN/2-150;
        rect_img_mort1.w = 300;
        rect_img_mort1.h = 300;
        SDL_FreeSurface(img_mort1);

        mort1 = TTF_RenderUTF8_Solid(font, mortNuit->tab_mort[0]->nom, blackColor);
        txr_mort1 = SDL_CreateTextureFromSurface(renderer, mort1);
        rect_mort1.x = rect_img_mort1.x+rect_img_mort1.w/2+10*strlen(mortNuit->tab_mort[0]->nom);
        rect_mort1.y = rect_img_mort1.y+rect_img_mort1.h;
        rect_mort1.w = strlen(mortNuit->tab_mort[0]->nom)*20;
        rect_mort1.h = 40;
    }
    else if(mortNuit->iNbMort == 2){
        mort = Mix_LoadWAV("../sound/voice/1mort.wav");

        info = TTF_RenderUTF8_Solid(font, "2 personnes sont mortes cette nuit !", blackColor);
        txr_info = SDL_CreateTextureFromSurface(renderer, info);
        rect_info.x = LONG_WIN/2-150;
        rect_info.y = 25;
        rect_info.w = 300;
        rect_info.h = 50;

        img_mort1 = IMG_Load("../img/dos_carte.jpg");
        txr_img_mort1 = SDL_CreateTextureFromSurface(renderer, img_mort1);
        rect_img_mort1.x = LONG_WIN/3-150;
        rect_img_mort1.y = LARG_WIN/2-150;
        rect_img_mort1.w = 300;
        rect_img_mort1.h = 300;
        SDL_FreeSurface(img_mort1);

        mort1 = TTF_RenderUTF8_Solid(font, mortNuit->tab_mort[0]->nom, blackColor);
        txr_mort1 = SDL_CreateTextureFromSurface(renderer, mort1);
        rect_mort1.x = rect_img_mort1.x+rect_img_mort1.w/2+10*strlen(mortNuit->tab_mort[0]->nom);
        rect_mort1.y = rect_img_mort1.y+rect_img_mort1.h;
        rect_mort1.w = strlen(mortNuit->tab_mort[0]->nom)*20;
        rect_mort1.h = 40;

        img_mort2 = IMG_Load("../img/dos_carte.jpg");
        txr_img_mort2 = SDL_CreateTextureFromSurface(renderer, img_mort2);
        rect_img_mort2.x = LONG_WIN*2/3-150;
        rect_img_mort2.y = LARG_WIN/2-150;
        rect_img_mort2.w = 300;
        rect_img_mort2.h = 300;
        SDL_FreeSurface(img_mort2);

        mort2 = TTF_RenderUTF8_Solid(font, mortNuit->tab_mort[1]->nom, blackColor);
        txr_mort2 = SDL_CreateTextureFromSurface(renderer, mort2);
        rect_mort2.x = rect_img_mort2.x+rect_img_mort2.w/2+10*strlen(mortNuit->tab_mort[0]->nom);
        rect_mort2.y = rect_img_mort2.y+rect_img_mort2.h;
        rect_mort2.w = strlen(mortNuit->tab_mort[0]->nom)*20;
        rect_mort2.h = 40;
        
    }
    else if(mortNuit->iNbMort == 3){
        mort = Mix_LoadWAV("../sound/voice/1mort.wav");

        info = TTF_RenderUTF8_Solid(font, "3 personnes sont mortes cette nuit !", blackColor);
        txr_info = SDL_CreateTextureFromSurface(renderer, info);
        rect_info.x = LONG_WIN/2-150;
        rect_info.y = 25;
        rect_info.w = 300;
        rect_info.h = 50;

        img_mort1 = IMG_Load("../img/dos_carte.jpg");
        txr_img_mort1 = SDL_CreateTextureFromSurface(renderer, img_mort1);
        rect_img_mort1.x = LONG_WIN/4-150;
        rect_img_mort1.y = LARG_WIN/2-150;
        rect_img_mort1.w = 300;
        rect_img_mort1.h = 300;
        SDL_FreeSurface(img_mort1);

        mort1 = TTF_RenderUTF8_Solid(font, mortNuit->tab_mort[0]->nom, blackColor);
        txr_mort1 = SDL_CreateTextureFromSurface(renderer, mort1);
        rect_mort1.x = rect_img_mort1.x+rect_img_mort1.w/2+10*strlen(mortNuit->tab_mort[0]->nom);
        rect_mort1.y = rect_img_mort1.y+rect_img_mort1.h;
        rect_mort1.w = strlen(mortNuit->tab_mort[0]->nom)*20;
        rect_mort1.h = 40;
        SDL_FreeSurface(mort1);

        img_mort2 = IMG_Load("../img/dos_carte.jpg");
        txr_img_mort2 = SDL_CreateTextureFromSurface(renderer, img_mort2);
        rect_img_mort2.x = LONG_WIN*2/4-150;
        rect_img_mort2.y = LARG_WIN/2-150;
        rect_img_mort2.w = 300;
        rect_img_mort2.h = 300;
        SDL_FreeSurface(img_mort2);

        mort2 = TTF_RenderUTF8_Solid(font, mortNuit->tab_mort[1]->nom, blackColor);
        txr_mort2 = SDL_CreateTextureFromSurface(renderer, mort2);
        rect_mort2.x = rect_img_mort2.x+rect_img_mort2.w/2+10*strlen(mortNuit->tab_mort[0]->nom);
        rect_mort2.y = rect_img_mort2.y+rect_img_mort2.h;
        rect_mort2.w = strlen(mortNuit->tab_mort[0]->nom)*20;
        rect_mort2.h = 40;
        SDL_FreeSurface(mort2);

        img_mort3 = IMG_Load("../img/dos_carte.jpg");
        txr_img_mort3 = SDL_CreateTextureFromSurface(renderer, img_mort3);
        rect_img_mort3.x = LONG_WIN*3/4-150;
        rect_img_mort3.y = LARG_WIN/2-150;
        rect_img_mort3.w = 300;
        rect_img_mort3.h = 300;
        SDL_FreeSurface(img_mort3);

        mort3 = TTF_RenderUTF8_Solid(font, mortNuit->tab_mort[2]->nom, blackColor);
        txr_mort3 = SDL_CreateTextureFromSurface(renderer, mort3);
        rect_mort3.x = rect_img_mort3.x+rect_img_mort3.w/2+10*strlen(mortNuit->tab_mort[0]->nom);
        rect_mort3.y = rect_img_mort3.y+rect_img_mort3.h;
        rect_mort3.w = strlen(mortNuit->tab_mort[0]->nom)*20;
        rect_mort3.h = 40;
        SDL_FreeSurface(mort3);
    }
    SDL_FreeSurface(info);

    Mix_VolumeChunk(mort, MIX_MAX_VOLUME/2); 
    Mix_PlayChannel(1, mort, 0);

    int run = 1;
    int tick = SDL_GetTicks();
    int delay = SDL_GetTicks() - tick;
    while(delay<17000){
        delay = SDL_GetTicks() - tick;

        if(delay >= 7000 && delay <= 8000){
            if(mortNuit->iNbMort>=1){
                SDL_DestroyTexture(txr_img_mort1);
                img_mort1 = LoadIMGFromRole(mortNuit->tab_mort[0]);
                txr_img_mort1 = SDL_CreateTextureFromSurface(renderer, img_mort1);
                SDL_FreeSurface(img_mort1);
            }
            if(mortNuit->iNbMort>=2){
                SDL_DestroyTexture(txr_img_mort2);
                img_mort2 = LoadIMGFromRole(mortNuit->tab_mort[1]);
                txr_img_mort2 = SDL_CreateTextureFromSurface(renderer, img_mort2);
                SDL_FreeSurface(img_mort2);
            }
            if(mortNuit->iNbMort>=3){
                SDL_DestroyTexture(txr_img_mort3);
                img_mort3 = LoadIMGFromRole(mortNuit->tab_mort[2]);
                txr_img_mort3 = SDL_CreateTextureFromSurface(renderer, img_mort3);
                SDL_FreeSurface(img_mort3);
            }
        }

        
        SDL_SetRenderDrawColor(renderer,149,56,58,255);
        SDL_RenderClear(renderer);
        if(mortNuit->iNbMort>=1){
            if(delay>7000){
                SDL_RenderCopy(renderer, txr_mort1, NULL, &rect_mort1);
                SDL_RenderCopy(renderer, txr_tueur1, NULL, &rect_tueur1);
            }   
            SDL_RenderCopy(renderer, txr_img_mort1, NULL, &rect_img_mort1);
            
        }
        if(mortNuit->iNbMort>=2){
            if(delay>7000){
                SDL_RenderCopy(renderer, txr_mort2, NULL, &rect_mort2);
                SDL_RenderCopy(renderer, txr_tueur2, NULL, &rect_tueur2);
            }
            SDL_RenderCopy(renderer, txr_img_mort2, NULL, &rect_img_mort2);
        }
        if(mortNuit->iNbMort>=3){
            if(delay>7000){
                SDL_RenderCopy(renderer, txr_mort3, NULL, &rect_mort3);
                SDL_RenderCopy(renderer, txr_tueur3, NULL, &rect_tueur3);
            }
            SDL_RenderCopy(renderer, txr_img_mort3, NULL, &rect_img_mort3);
        }
    }
    SDL_DestroyTexture(txr_info);
    if(mortNuit->iNbMort>=1){
        SDL_DestroyTexture(txr_img_mort1);
        SDL_DestroyTexture(txr_mort1);
    }
    if(mortNuit->iNbMort>=2){
        SDL_DestroyTexture(txr_img_mort2);
        SDL_DestroyTexture(txr_mort2);
    }
    if(mortNuit->iNbMort>=3){
        SDL_DestroyTexture(txr_img_mort3);
        SDL_DestroyTexture(txr_mort3);
    }
}

void jour(jeu_t * jeu, SDL_Renderer * renderer, TTF_Font *font, mort_t * mortNuit){
    printf("début du jour\n");
    affMorts(jeu, renderer, font, mortNuit);
    printf("Fin du jour\n");
    /*
    if(mortNuit->tab_mort[0]->role == CHASSEUR || mortNuit->tab_mort[1]->role == CHASSEUR || mortNuit->tab_mort[2]->role == CHASSEUR){
        tourChasseur(jeu, renderer, font);
    }

    vote();
    */
}

void echanger(jeu_t * jeu, int i, int j){
    char * temp = malloc(sizeof(char)*MAX_NAME_SIZE);
    strcpy(temp, jeu->joueurs[i].nom);

    if(realloc(jeu->joueurs[i].nom, sizeof(char)*MAX_NAME_SIZE) != NULL){
        strcpy(jeu->joueurs[i].nom, jeu->joueurs[j].nom);
    }

    if(realloc(jeu->joueurs[j].nom, sizeof(char)*MAX_NAME_SIZE) != NULL){
        strcpy(jeu->joueurs[j].nom, temp);
    }

    free(temp);
    temp=NULL;
}

int initJeu(jeu_t * jeu, SDL_Renderer * renderer, TTF_Font * font){
    srand(time(NULL));
    int run;
    int ok_click;
    int x, y;

    if(jeu->nbVoleur>=1){
        jeu->nbVillageois += 2; /* Rajoute 2 villageois pour la pioche du voleur */
        jeu->piocheVoleur = malloc(sizeof(pioche_t)*2);
    }
    else{
        jeu->piocheVoleur = NULL;
    }

    int i, j, attribution;
    int iRole;
    int roles_attribue[NB_ROLES] = {0,0,0,0,0,0,0,0};

    /* Mélanger les nom */
    for(i=0; i<jeu->nbJoueur; i++){
        j = rand()%(jeu->nbJoueur);
        if(i!=j){
            echanger(jeu, i, j);
        }
    }

    /* Attribuer roles */
    char * roleName;
    for(i=0; i<jeu->nbJoueur; i++){
        if(i==0){
            roleName = malloc(sizeof(char)*20);
        }
        else{
            if(realloc(roleName, sizeof(char)*20)==NULL){
                printf("Erreur de réallocation du nom du rôle\n");
                return 0;
            }
        }

        jeu->joueurs[i].nomIMG = malloc(sizeof(char)*50);
        attribution=0;
        do{
            iRole = rand()%8;
            switch(iRole){
                case LOUPGAROU : 
                    if(roles_attribue[0]<jeu->nbLoupGarou){
                        jeu->joueurs[i].role = LOUPGAROU;
                        strcpy(jeu->joueurs[i].nomIMG,"../img/loup-garou.png");
                        strcpy(roleName, "Loup-Garou");
                        attribution = 1;
                    }
                    break;
                case VILLAGEOIS : 
                    if(roles_attribue[1]<jeu->nbVillageois){
                        jeu->joueurs[i].role = VILLAGEOIS;
                        strcpy(jeu->joueurs[i].nomIMG,"../img/villageois.png");
                        strcpy(roleName, "Villageois");
                        attribution = 1;
                    }
                    break;
                case CHASSEUR : 
                    if(roles_attribue[2]<jeu->nbChasseur){
                        jeu->joueurs[i].role = CHASSEUR;
                        strcpy(jeu->joueurs[i].nomIMG,"../img/chasseur.png");
                        strcpy(roleName, "Chasseur");
                        attribution = 1;
                    }
                    break;
                case VOYANTE : 
                    if(roles_attribue[3]<jeu->nbVoyante){
                        jeu->joueurs[i].role = VOYANTE;
                        strcpy(jeu->joueurs[i].nomIMG,"../img/voyante.png");
                        strcpy(roleName, "Voyante");
                        attribution = 1;
                    }
                    break;
                case CUPIDON : 
                    if(roles_attribue[4]<jeu->nbCupidon){
                        jeu->joueurs[i].role = CUPIDON;
                        strcpy(jeu->joueurs[i].nomIMG,"../img/cupidon.png");
                        strcpy(roleName, "Cupidon");
                        attribution = 1;
                    }
                    break;
                case SORCIERE : 
                    if(roles_attribue[5]<jeu->nbSorciere){
                        jeu->joueurs[i].role = SORCIERE;
                        strcpy(jeu->joueurs[i].nomIMG,"../img/sorciere.png");
                        strcpy(roleName, "Sorcière");
                        attribution = 1;
                    }
                    break;
                case PETITEFILLE : 
                    if(roles_attribue[6]<jeu->nbPetiteFille){
                        jeu->joueurs[i].role = PETITEFILLE;
                        strcpy(jeu->joueurs[i].nomIMG,"../img/petite-fille.png");
                        strcpy(roleName, "Petite-Fille");
                        attribution = 1;
                    }
                    break;
                case VOLEUR : 
                    if(roles_attribue[7]<jeu->nbVoleur){
                        jeu->joueurs[i].role = VOLEUR;
                        strcpy(jeu->joueurs[i].nomIMG,"../img/voleur.png");
                        strcpy(roleName, "Voleur");
                        attribution = 1;
                    }
                    break;
                
                default: break;
            }
        }while(!attribution);
        roles_attribue[iRole]++;

        SDL_Surface *title = TTF_RenderUTF8_Solid(font, "Votre carte", blackColor);
        SDL_Texture *txr_title = SDL_CreateTextureFromSurface(renderer, title);
        SDL_Rect rect_title = {LONG_WIN*1/2 - 100, 10, 200, 75};
        SDL_FreeSurface(title);

        SDL_Surface *name = TTF_RenderUTF8_Solid(font, jeu->joueurs[i].nom, blackColor);
        SDL_Texture *txr_name = SDL_CreateTextureFromSurface(renderer, name);
        SDL_Rect rect_name = {LONG_WIN*1/2 - 15 * strlen(jeu->joueurs[i].nom), rect_title.y + rect_title.h, 30 * strlen(jeu->joueurs[i].nom), 75};
        SDL_FreeSurface(name);

        SDL_Surface *info = TTF_RenderUTF8_Solid(font, "Donnez l'ordinateur à ce joueur", blackColor);
        SDL_Texture *txr_info = SDL_CreateTextureFromSurface(renderer, info);
        SDL_Rect rect_info = {LONG_WIN*1/2 - 150, rect_name.y + 70, 300, 40};
        SDL_FreeSurface(info);

        SDL_Surface *ok = TTF_RenderUTF8_Solid(font, "OK !", blackColor);
        SDL_Texture *txr_ok = SDL_CreateTextureFromSurface(renderer, ok);
        SDL_Rect rect_ok= {LONG_WIN*1/2-50, LARG_WIN-75, 100, 75};
        SDL_FreeSurface(ok);

        SDL_Surface *card = IMG_Load("../img/dos_carte.jpg");
        SDL_Texture *txr_card = SDL_CreateTextureFromSurface(renderer, card);
        SDL_Rect rect_card= {LONG_WIN/2-125, LARG_WIN/2-125, 250, 250};
        SDL_FreeSurface(card);

        SDL_Surface * role = TTF_RenderUTF8_Solid(font, roleName, blackColor);
        SDL_Texture *txr_role = SDL_CreateTextureFromSurface(renderer, role);
        SDL_Rect rect_role= {LONG_WIN*1/2-strlen(roleName)*10, rect_card.y + rect_card.h + 20, strlen(roleName)*20, 50};
        SDL_FreeSurface(role);

        run=1;
        ok_click = 0;
        while(run){
            SDL_Event event;
            while(SDL_PollEvent(&event)){
                switch(event.type){
                    case SDL_QUIT: 
                        run=0; 
                        break;

                    case SDL_MOUSEBUTTONUP:
                        SDL_GetMouseState(&x,&y);
                        if(x>=rect_ok.x-rect_ok.w/2 && x<=rect_ok.x-rect_ok.w/2+rect_ok.w && y>=rect_ok.y-rect_ok.h-10 && y<=rect_ok.y-rect_ok.w-10+rect_ok.h){
                            ok_click++;
                            if(ok_click == 1){
                                SDL_DestroyTexture(txr_card);
                                card = IMG_Load(jeu->joueurs[i].nomIMG);
                                txr_card = SDL_CreateTextureFromSurface(renderer, card);
                                SDL_FreeSurface(card);

                                SDL_DestroyTexture(txr_info);
                                rect_info.w = 400;
                                rect_info.x = LONG_WIN*1/2 - 200;
                                char * info_str = "Mémorisez bien votre rôle et votre numéro : ";
                                char num[2];
                                itoa(jeu->joueurs[i].num, num);
                                strcat(info_str, num);
                                info = TTF_RenderUTF8_Solid(font, info_str, blackColor);
                                txr_info = SDL_CreateTextureFromSurface(renderer, info);
                                SDL_FreeSurface(info);
                            }
                            else if(ok_click == 2){
                                run=0;
                            }
                        }
                    
                    case SDL_KEYDOWN:
                        if(event.key.keysym.sym == SDLK_RETURN){
                            ok_click++;
                            if(ok_click == 1){
                                SDL_DestroyTexture(txr_card);
                                card = IMG_Load(jeu->joueurs[i].nomIMG);
                                txr_card = SDL_CreateTextureFromSurface(renderer, card);

                                SDL_FreeSurface(card);

                                SDL_DestroyTexture(txr_info);
                                rect_info.w = 450;
                                rect_info.x = LONG_WIN*1/2 - 225;
                                char * info_str = malloc(sizeof(char)*50);
                                strcpy(info_str,"Mémorisez bien votre rôle et votre numéro : ");
                                char * numero = malloc(sizeof(char)*2);
                                itoa(jeu->joueurs[i].num, numero);
                                strcat(info_str, numero);
                                info = TTF_RenderUTF8_Solid(font, info_str, blackColor);
                                txr_info = SDL_CreateTextureFromSurface(renderer, info);

                                SDL_FreeSurface(info);
                                free(info_str);
                                info_str = NULL;
                                free(numero);
                                numero = NULL;
                            }
                            else if(ok_click == 2){
                                run=0;
                            }
                        }
                    default: break;
                }
            }
            SDL_SetRenderDrawColor(renderer,149,56,58,255);
            SDL_RenderClear(renderer);
            if(ok_click == 1){
                SDL_RenderCopy(renderer, txr_role, NULL, &rect_role);
            }
            SDL_RenderCopy(renderer, txr_card, NULL, &rect_card);
            SDL_RenderCopy(renderer, txr_title, NULL, &rect_title);
            SDL_RenderCopy(renderer, txr_ok, NULL, &rect_ok);
            SDL_RenderCopy(renderer, txr_info, NULL, &rect_info);
            SDL_RenderCopy(renderer, txr_name, NULL, &rect_name);
            SDL_RenderPresent(renderer);
        }
        SDL_DestroyTexture(txr_role);
        SDL_DestroyTexture(txr_card);
        SDL_DestroyTexture(txr_info);
        SDL_DestroyTexture(txr_ok);
        SDL_DestroyTexture(txr_name);
        SDL_DestroyTexture(txr_title);
    }
    free(roleName);
    roleName = NULL;

    if(jeu->nbVoleur>0){
        int iPioche = 0;
        int i;
        for(i=0; i<NB_ROLES && iPioche<2; i++){
            printf("ipioche = %d\n", iPioche);
            switch(i){
                case 0: 
                    printf("verif Loup\n");
                    while(roles_attribue[i] < jeu->nbLoupGarou){
                        jeu->piocheVoleur[iPioche].role = LOUPGAROU;
                        jeu->piocheVoleur[iPioche].nomIMG = malloc(sizeof(char)*50);
                        strcpy(jeu->piocheVoleur[iPioche++].nomIMG, "../img/loup-garou.png");
                        roles_attribue[i]++;
                    }
                    break;
                case 1: 
                    printf("verif vil\n");
                    while(roles_attribue[i] < jeu->nbVillageois){
                        jeu->piocheVoleur[iPioche].role = VILLAGEOIS;
                        jeu->piocheVoleur[iPioche].nomIMG = malloc(sizeof(char)*50);
                        strcpy(jeu->piocheVoleur[iPioche++].nomIMG , "../img/villageois.png");
                        roles_attribue[i]++;
                    }
                    break;
                case 2:
                    printf("verif cha\n");
                    while(roles_attribue[i] < jeu->nbChasseur){
                        jeu->piocheVoleur[iPioche].role = CHASSEUR;
                        jeu->piocheVoleur[iPioche].nomIMG  = malloc(sizeof(char)*50);
                        strcpy(jeu->piocheVoleur[iPioche++].nomIMG, "../img/chasseur.png");
                        roles_attribue[i]++;
                    }
                    break;
                case 3: 
                    printf("verif voy\n");
                    while(roles_attribue[i] < jeu->nbVoyante){
                        jeu->piocheVoleur[iPioche].role = VOYANTE;
                        jeu->piocheVoleur[iPioche].nomIMG = malloc(sizeof(char)*50);
                        strcpy(jeu->piocheVoleur[iPioche++].nomIMG, "../img/voyante.png");
                        roles_attribue[i]++;
                    }
                    break;
                case 4:
                    printf("verif cup\n");
                    while(roles_attribue[i] < jeu->nbCupidon){
                        jeu->piocheVoleur[iPioche].role = CUPIDON;
                        jeu->piocheVoleur[iPioche].nomIMG = malloc(sizeof(char)*50);
                        strcpy(jeu->piocheVoleur[iPioche++].nomIMG, "../img/cupidon.png");
                        roles_attribue[i]++;
                    }
                    break;
                case 5: 
                    printf("verif sor\n");
                    while(roles_attribue[i] < jeu->nbSorciere){
                        jeu->piocheVoleur[iPioche].role = SORCIERE;
                        jeu->piocheVoleur[iPioche].nomIMG = malloc(sizeof(char)*50);
                        strcpy(jeu->piocheVoleur[iPioche++].nomIMG, "../img/sorciere.png");
                        roles_attribue[i]++;
                    }
                    break;
                case 6: 
                    printf("verif pf\n");
                    while(roles_attribue[i] < jeu->nbPetiteFille){
                        jeu->piocheVoleur[iPioche].role = PETITEFILLE;
                        jeu->piocheVoleur[iPioche].nomIMG = malloc(sizeof(char)*50);
                        strcpy(jeu->piocheVoleur[iPioche++].nomIMG, "../img/petite-fille.png");
                        roles_attribue[i]++;
                    }
                    break;
                case 7: 
                    printf("verif vol\n");
                    while(roles_attribue[i] < jeu->nbVoleur){
                        jeu->piocheVoleur[iPioche].role = VOLEUR;
                        jeu->piocheVoleur[iPioche].nomIMG = malloc(sizeof(char)*50);
                        strcpy(jeu->piocheVoleur[iPioche].nomIMG, "../img/voleur.png");
                        roles_attribue[i]++;
                    }
                    break;
            default:
                break;
            }
        }
    }

    jeu->potionsSorciere[0] = 1;
    jeu->potionsSorciere[1] = 1;

    return 1;
}

int verif_vict_LG(jeu_t * jeu){
    for(int i=0; i<jeu->nbJoueur; i++){
        if(jeu->joueurs[i].role != LOUPGAROU && jeu->joueurs[i].etat == VIVANT && trouverVoleur(jeu)==-1){
            return 0;
        }
    }
    return 1;
}

int verif_vict_village(jeu_t * jeu){
    for(int i=0; i<jeu->nbJoueur; i++){
        if(jeu->joueurs[i].role == LOUPGAROU && jeu->joueurs[i].etat == VIVANT){
            return 0;
        }
    }
    return 1;
}


int verif_vict_amour(jeu_t * jeu){
    joueur_t ** amoureux = malloc(sizeof(joueur_t*)*2);
    int iAmour=0;
    for(int i=0; i<jeu->nbJoueur; i++){
        if((jeu->joueurs[i].statut[0] == AMOUREUX || jeu->joueurs[i].statut[1] == AMOUREUX) && jeu->joueurs[i].etat == VIVANT){
            amoureux[iAmour++] = &jeu->joueurs[i];
        }
    }
    if(iAmour==2){
        int nbVivant = 0;
        for(int i=0; i<jeu->nbJoueur; i++){
            if(jeu->joueurs[i].etat == VIVANT){
                nbVivant++;
            }
        }
        if(iAmour == nbVivant && ((amoureux[0]->role == LOUPGAROU && amoureux[1]->role != LOUPGAROU)||(amoureux[1]->role == LOUPGAROU && amoureux[0]->role != LOUPGAROU))){
            free(amoureux);
            amoureux = NULL;
            return 1;
        }
    }
    free(amoureux);
    amoureux = NULL;
    return 0;
}


int play(jeu_t * jeu, SDL_Renderer * renderer, TTF_Font * font){
    int victoire = 1;
    int tour = 0;

    while(!verif_vict_amour(jeu) && !verif_vict_LG(jeu) && !verif_vict_village(jeu)){
        tour++;
        mort_t * mortNuit = malloc(sizeof(mort_t));
        mortNuit->iNbMort = 0;
        mortNuit->tab_mort = malloc(sizeof(joueur_t*)*3);
        mortNuit->role_tueur = malloc(sizeof(Roles)*3);

        nuit(tour, jeu, renderer, font, mortNuit);
        if(!verif_vict_amour(jeu) && !verif_vict_LG(jeu) && !verif_vict_village(jeu)){
            jour(jeu, renderer, font, mortNuit);
        }

        free(mortNuit->role_tueur);
        mortNuit->role_tueur = NULL;
        free(mortNuit->tab_mort);
        mortNuit->tab_mort = NULL;
        free(mortNuit);
        mortNuit = NULL;
    }

    if(verif_vict_amour(jeu)){
        printf("Victoire des amoureux !\n");
    }
    else if(verif_vict_LG(jeu)){
        printf("Victoire des loups garous !\n");
    }
    else if(verif_vict_village(jeu)){
        printf("Victoire des Villageois !\n");
    }
    

    return 0;
}