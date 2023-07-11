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
        rect_info.y = LARG_WIN/5-25;
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
        mort = Mix_LoadWAV("../sound/voice/multi_mort.wav");

        info = TTF_RenderUTF8_Solid(font, "2 personnes sont mortes cette nuit !", blackColor);
        txr_info = SDL_CreateTextureFromSurface(renderer, info);
        rect_info.x = LONG_WIN/2-150;
        rect_info.y = LARG_WIN/5-25;
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
        mort = Mix_LoadWAV("../sound/voice/multi_mort.wav");

        info = TTF_RenderUTF8_Solid(font, "3 personnes sont mortes cette nuit !", blackColor);
        txr_info = SDL_CreateTextureFromSurface(renderer, info);
        rect_info.x = LONG_WIN/2-150;
        rect_info.y = LARG_WIN/5-25;
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
    SDL_Surface *tueur1 = NULL; 
    SDL_Texture * txr_tueur1;
    SDL_Rect rect_tueur1;
    rect_tueur1.y = rect_mort1.y+rect_mort1.h;
    rect_tueur1.h = rect_mort1.h-10;
    printf("%d\n", mortNuit->iNbMort);
    if(mortNuit->iNbMort>0 && mortNuit->role_tueur[0] == LOUPGAROU){
        tueur1 = TTF_RenderUTF8_Solid(font, "Tué par les LOUPS-GAROUS", whiteColor);
        rect_tueur1.x = rect_mort1.x-150;
        rect_tueur1.w = 300;
    }
    else if(mortNuit->iNbMort>0 && mortNuit->role_tueur[0] == SORCIERE){
        tueur1 = TTF_RenderUTF8_Solid(font, "Tué par la SORCIERE", whiteColor);
        rect_tueur1.x = rect_mort1.x-125;
        rect_tueur1.w = 250;
    }
    else if(mortNuit->iNbMort>0 && mortNuit->role_tueur[0] == CUPIDON){
        tueur1 = TTF_RenderUTF8_Solid(font, "Tué par AMOUR", whiteColor);
        rect_tueur1.x = rect_mort1.x-100;
        rect_tueur1.w = 200;
    }
    if(tueur1 != NULL){
        txr_tueur1 = SDL_CreateTextureFromSurface(renderer, tueur1);
        SDL_FreeSurface(tueur1);
    }

    SDL_Surface *tueur2 = NULL; 
    SDL_Texture * txr_tueur2;
    SDL_Rect rect_tueur2;
    rect_tueur2.y = rect_mort2.y+rect_mort2.h;
    rect_tueur2.h = rect_mort2.h-10;
    if(mortNuit->iNbMort>1 && mortNuit->role_tueur[1] == LOUPGAROU){
        tueur2 = TTF_RenderUTF8_Solid(font, "Tué par les LOUPS-GAROUS", whiteColor);
        rect_tueur2.x = rect_mort2.x-150;
        rect_tueur2.w = 300;
    }
    else if(mortNuit->iNbMort>1 && mortNuit->role_tueur[1] == SORCIERE){
        tueur2 = TTF_RenderUTF8_Solid(font, "Tué par la SORCIERE", whiteColor);
        rect_tueur2.x = rect_mort2.x-125;
        rect_tueur2.w = 250;
    }
    else if(mortNuit->iNbMort>1 && mortNuit->role_tueur[1] == CUPIDON){
        tueur2 = TTF_RenderUTF8_Solid(font, "Tué par AMOUR", whiteColor);
        rect_tueur2.x = rect_mort2.x-100;
        rect_tueur2.w = 200;
    }
    if(tueur2 != NULL){
        txr_tueur2 = SDL_CreateTextureFromSurface(renderer, tueur2);
        SDL_FreeSurface(tueur2);
    }

    SDL_Surface *tueur3 = NULL; 
    SDL_Texture * txr_tueur3;
    SDL_Rect rect_tueur3;
    rect_tueur3.x = rect_mort3.x;
    rect_tueur3.y = rect_mort3.y+rect_mort3.h;
    rect_tueur3.h = rect_mort3.h-10;
    if(mortNuit->iNbMort>2 && mortNuit->role_tueur[2] == LOUPGAROU){
        tueur3 = TTF_RenderUTF8_Solid(font, "Tué par les LOUPS-GAROUS", whiteColor);
        rect_tueur3.x = rect_mort3.x-150;
        rect_tueur3.w = 300;
    }
    else if(mortNuit->iNbMort>2 && mortNuit->role_tueur[2] == SORCIERE){
        tueur3 = TTF_RenderUTF8_Solid(font, "Tué par la SORCIERE", whiteColor);
        rect_tueur3.x = rect_mort3.x-125;
        rect_tueur3.w = 250;
    }
    else if(mortNuit->iNbMort>2 && mortNuit->role_tueur[2] == CUPIDON){
        tueur3 = TTF_RenderUTF8_Solid(font, "Tué par AMOUR", whiteColor);
        rect_tueur3.x = rect_mort3.x-100;
        rect_tueur3.w = 200;
    }
    if(tueur1 != NULL){
        txr_tueur3 = SDL_CreateTextureFromSurface(renderer, tueur3);
        SDL_FreeSurface(tueur3);
    }

    SDL_FreeSurface(info);

    Mix_VolumeChunk(mort, MIX_MAX_VOLUME/2); 
    Mix_PlayChannel(1, mort, 0);

    int tick = SDL_GetTicks();
    int delay = SDL_GetTicks() - tick;
    while(delay<12000){
        delay = SDL_GetTicks() - tick;

        if(delay >= 5000 && delay <= 6000){
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
        SDL_RenderCopy(renderer, txr_info, NULL, &rect_info);
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
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyTexture(txr_info);
    if(mortNuit->iNbMort>=1){
        SDL_DestroyTexture(txr_img_mort1);
        SDL_DestroyTexture(txr_mort1);
        SDL_DestroyTexture(txr_tueur1);
    }
    if(mortNuit->iNbMort>=2){
        SDL_DestroyTexture(txr_img_mort2);
        SDL_DestroyTexture(txr_mort2);
        SDL_DestroyTexture(txr_tueur2);
    }
    if(mortNuit->iNbMort>=3){
        SDL_DestroyTexture(txr_img_mort3);
        SDL_DestroyTexture(txr_mort3);
        SDL_DestroyTexture(txr_tueur3);
    }
}
/*
void jour(jeu_t * jeu, SDL_Renderer * renderer, TTF_Font *font, mort_t * mortNuit){
    
    if(mortNuit->tab_mort[0]->role == CHASSEUR || mortNuit->tab_mort[1]->role == CHASSEUR || mortNuit->tab_mort[2]->role == CHASSEUR){
        tourChasseur(jeu, renderer, font);
    }

    vote();
}
*/

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

    affAllPlayer(renderer, font, jeu);

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

int victoire_amour(jeu_t * jeu, SDL_Renderer * renderer,TTF_Font * font){
    Mix_Music *music_amour = Mix_LoadMUS("../sound/music/vict_amour.mp3");
    Mix_FadeInMusic(music_amour, 0, 3000);

    SDL_Surface *title = TTF_RenderUTF8_Solid(font, "Victoire des Amoureux", whiteColor);
    SDL_Texture *txr_title = SDL_CreateTextureFromSurface(renderer, title);
    SDL_Rect rect_title = {LONG_WIN/2 - 150, 10, 300, 100};
    SDL_FreeSurface(title);

    SDL_Surface *rejouer = TTF_RenderUTF8_Solid(font, "Rejouer", whiteColor);
    SDL_Texture *txr_rejouer = SDL_CreateTextureFromSurface(renderer, rejouer);
    SDL_Rect rect_rejouer = {LONG_WIN/2 + 37, LARG_WIN-75, 150, 75};
    SDL_FreeSurface(rejouer);

    SDL_Surface *menu = TTF_RenderUTF8_Solid(font, "Menu", whiteColor);
    SDL_Texture *txr_menu = SDL_CreateTextureFromSurface(renderer, menu);
    SDL_Rect rect_menu = {LONG_WIN/2 - 150, LARG_WIN-75, 100, 75};
    SDL_FreeSurface(menu);

    SDL_Surface *tab_noms_amour[2];
    SDL_Texture *tab_txr_noms_amour[2];
    SDL_Rect tab_rect_nom_amour[2];

    SDL_Surface *img_amour1;
    SDL_Texture *txr_img_amour1;
    SDL_Rect rect_img_amour1;

    SDL_Surface *img_amour2;
    SDL_Texture *txr_img_amour2;
    SDL_Rect rect_img_amour2;

    int j=0;
    for(int i=0; i<jeu->nbJoueur; i++){
        if(jeu->joueurs[i].statut[0] == AMOUREUX || jeu->joueurs[i].statut[1] == AMOUREUX){
            tab_noms_amour[j] = TTF_RenderUTF8_Solid(font, jeu->joueurs[i].nom, whiteColor);
            tab_txr_noms_amour[j] = SDL_CreateTextureFromSurface(renderer, tab_noms_amour[j]);
            SDL_FreeSurface(tab_noms_amour[j]);
            if(j==0){
                img_amour1 = LoadIMGFromRole(&jeu->joueurs[i]);
                txr_img_amour1 = SDL_CreateTextureFromSurface(renderer, img_amour1);
                SDL_FreeSurface(img_amour1);
                rect_img_amour1.x = LONG_WIN/3-150;
                rect_img_amour1.w = 300;
                rect_img_amour1.y = LARG_WIN/2-200;
                rect_img_amour1.h = 300;

                tab_rect_nom_amour[j].x = rect_img_amour1.x+150-strlen(jeu->joueurs[i].nom)*10;
                tab_rect_nom_amour[j].w = strlen(jeu->joueurs[i].nom)*20;
                tab_rect_nom_amour[j].y = rect_img_amour1.y+rect_img_amour1.h;
                tab_rect_nom_amour[j].h = 40;
            }
            if(j==1){
                img_amour2 = LoadIMGFromRole(&jeu->joueurs[i]);
                txr_img_amour2 = SDL_CreateTextureFromSurface(renderer, img_amour2);
                SDL_FreeSurface(img_amour2);
                rect_img_amour2.x = LONG_WIN*2/3-150;
                rect_img_amour2.w = 300;
                rect_img_amour2.y = LARG_WIN/2-200;
                rect_img_amour2.h = 300;

                tab_rect_nom_amour[j].x = rect_img_amour2.x+150-strlen(jeu->joueurs[i].nom)*10;
                tab_rect_nom_amour[j].w = strlen(jeu->joueurs[i].nom)*20;
                tab_rect_nom_amour[j].y = rect_img_amour2.y+rect_img_amour2.h;
                tab_rect_nom_amour[j].h = 40;
            }
            j++;
        }
    }

    int x,y;
    int retour = 0;
    int run = 1;
    while(run){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    run = 0;
                    break;
                case SDL_MOUSEBUTTONUP:
                    SDL_GetMouseState(&x, &y);
                    if(x>=rect_rejouer.x && x<=rect_rejouer.x+rect_rejouer.w && y>=rect_rejouer.y && y<=rect_rejouer.y+rect_rejouer.h){
                        retour = 1;
                        run = 0;
                    }
                    else if(x>=rect_menu.x && x<=rect_menu.x+rect_menu.w && y>=rect_menu.y && y<=rect_menu.y+rect_menu.h){
                        retour = 0;
                        run = 0;
                    }
            }
        }
        SDL_SetRenderDrawColor(renderer,149,56,58,255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, txr_title, NULL, &rect_title);
        SDL_RenderCopy(renderer, txr_menu, NULL, &rect_menu);
        SDL_RenderCopy(renderer, txr_rejouer, NULL, &rect_rejouer);
        SDL_RenderCopy(renderer, txr_img_amour1, NULL, &rect_img_amour1);
        SDL_RenderCopy(renderer, txr_img_amour2, NULL, &rect_img_amour2);
        SDL_RenderCopy(renderer, tab_txr_noms_amour[0], NULL, &tab_rect_nom_amour[0]);
        SDL_RenderCopy(renderer, tab_txr_noms_amour[1], NULL, &tab_rect_nom_amour[1]);
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyTexture(txr_title);
    SDL_DestroyTexture(txr_menu);
    SDL_DestroyTexture(txr_rejouer);
    SDL_DestroyTexture(txr_img_amour1);
    SDL_DestroyTexture(txr_img_amour2);
    SDL_DestroyTexture(tab_txr_noms_amour[0]);
    SDL_DestroyTexture(tab_txr_noms_amour[1]);

    Mix_FadeOutMusic(2000);
    while(Mix_PlayingMusic());

    Mix_FreeMusic(music_amour);

    return retour;
}

int victoire_lg(jeu_t * jeu, SDL_Renderer * renderer,TTF_Font * font){
    Mix_Music *music_lg = Mix_LoadMUS("../sound/music/vict_lg.mp3");
    Mix_FadeInMusic(music_lg, 0, 3000);

    int ligne=1, nbLigne, iCol=0, ecart;
    if(jeu->nbLoupGarou >= 1 && jeu->nbLoupGarou <= 6){
        nbLigne = 1;
        ecart = 0;
    }
    if(jeu->nbLoupGarou > 6 && jeu->nbLoupGarou <= 12){
        nbLigne = 2;
        ecart = 200;
    }
    else if(jeu->nbLoupGarou > 12 && jeu->nbLoupGarou <= 18){
        nbLigne = 3;
        ecart = 200;
    }
    else if(jeu->nbLoupGarou > 18 && jeu->nbLoupGarou <= 24){
        nbLigne = 4;
        ecart = 175;
    }
    

    SDL_Surface *title = TTF_RenderUTF8_Solid(font, "Victoire des Loups-Garous", whiteColor);
    SDL_Texture *txr_title = SDL_CreateTextureFromSurface(renderer, title);
    SDL_Rect rect_title = {LONG_WIN/2 - 150, 10, 300, 100};
    SDL_FreeSurface(title);

    SDL_Surface *rejouer = TTF_RenderUTF8_Solid(font, "Rejouer", whiteColor);
    SDL_Texture *txr_rejouer = SDL_CreateTextureFromSurface(renderer, rejouer);
    SDL_Rect rect_rejouer = {LONG_WIN/2 + 37, LARG_WIN-75, 150, 75};
    SDL_FreeSurface(rejouer);

    SDL_Surface *menu = TTF_RenderUTF8_Solid(font, "Menu", whiteColor);
    SDL_Texture *txr_menu = SDL_CreateTextureFromSurface(renderer, menu);
    SDL_Rect rect_menu = {LONG_WIN/2 - 150, LARG_WIN-75, 100, 75};
    SDL_FreeSurface(menu);

    SDL_Surface *tab_noms[jeu->nbLoupGarou];
    SDL_Surface *tab_role[jeu->nbLoupGarou];
    SDL_Texture *tab_txr_noms[jeu->nbLoupGarou];
    SDL_Texture *tab_txr_role[jeu->nbLoupGarou];
    SDL_Rect tab_rect_nom[jeu->nbLoupGarou];
    SDL_Rect tab_rect_role[jeu->nbLoupGarou];

    int j=0;
    for(int i=0; i<jeu->nbJoueur; i++){
        if(jeu->joueurs[i].role == LOUPGAROU){
            tab_noms[j] = TTF_RenderUTF8_Solid(font, jeu->joueurs[i].nom, whiteColor);
            tab_txr_noms[j] = SDL_CreateTextureFromSurface(renderer, tab_noms[j]);
            tab_rect_nom[j].x = LONG_WIN*(2+3*iCol++)/20-strlen(jeu->joueurs[i].nom)*10+20;
            if(ligne>1){
                tab_rect_nom[j].y = LARG_WIN/(nbLigne+1)+(ligne-1)*ecart-50;
            }
            else{
                tab_rect_nom[j].y = LARG_WIN/(nbLigne+1)-50;
            }
            tab_rect_nom[j].w = strlen(jeu->joueurs[i].nom)*20;
            tab_rect_nom[j].h = 40;
            SDL_FreeSurface(tab_noms[j]);

            tab_role[j] = LoadIMGFromRole(&jeu->joueurs[i]);
            tab_txr_role[j] = SDL_CreateTextureFromSurface(renderer, tab_role[j]);
            tab_rect_role[j].w = 100;
            tab_rect_role[j].h = 100;
            tab_rect_role[j].x = tab_rect_nom[j].x + tab_rect_nom[j].w/2 - 50;
            tab_rect_role[j].y = tab_rect_nom[j].y + tab_rect_nom[j].h+5;
            SDL_FreeSurface(tab_role[j]);

            if(j%6==5){
                ligne++;
                iCol=0;
            }
            j++;
        }
    }

    int x,y;
    int retour = 0;
    int run = 1;
    while(run){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    run = 0;
                    break;
                case SDL_MOUSEBUTTONUP:
                    SDL_GetMouseState(&x, &y);
                    if(x>=rect_rejouer.x && x<=rect_rejouer.x+rect_rejouer.w && y>=rect_rejouer.y && y<=rect_rejouer.y+rect_rejouer.h){
                        retour = 1;
                        run = 0;
                    }
                    else if(x>=rect_menu.x && x<=rect_menu.x+rect_menu.w && y>=rect_menu.y && y<=rect_menu.y+rect_menu.h){
                        retour = 0;
                        run = 0;
                    }
            }
        }
        SDL_SetRenderDrawColor(renderer,149,56,58,255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, txr_title, NULL, &rect_title);
        SDL_RenderCopy(renderer, txr_menu, NULL, &rect_menu);
        SDL_RenderCopy(renderer, txr_rejouer, NULL, &rect_rejouer);
        for(int i=0; i<jeu->nbLoupGarou; i++){
            SDL_RenderCopy(renderer, tab_txr_noms[i], NULL, &tab_rect_nom[i]);
            SDL_RenderCopy(renderer, tab_txr_role[i], NULL, &tab_rect_role[i]);
        }
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyTexture(txr_title);
    SDL_DestroyTexture(txr_menu);
    SDL_DestroyTexture(txr_rejouer);
    for(int i=0; i<jeu->nbLoupGarou; i++){
        SDL_DestroyTexture(tab_txr_noms[i]);
        SDL_DestroyTexture(tab_txr_role[i]);
    }

    Mix_FadeOutMusic(2000);
    while(Mix_PlayingMusic());

    Mix_FreeMusic(music_lg);

    return retour;
}

int victoire_village(jeu_t * jeu, SDL_Renderer * renderer,TTF_Font * font){
    Mix_Music *music_lg = Mix_LoadMUS("../sound/music/vict_vil.mp3");
    Mix_FadeInMusic(music_lg, 0, 3000);

    int ligne=1, nbLigne, iCol=0, ecart;
    int nbNonLoup = jeu->nbJoueur-jeu->nbLoupGarou;
    if(nbNonLoup >= 1 && nbNonLoup <= 6){
        nbLigne = 1;
        ecart = 0;
    }
    if(nbNonLoup > 6 && nbNonLoup <= 12){
        nbLigne = 2;
        ecart = 200;
    }
    else if(nbNonLoup > 12 && nbNonLoup <= 18){
        nbLigne = 3;
        ecart = 200;
    }
    else if(nbNonLoup > 18 && nbNonLoup <= 24){
        nbLigne = 4;
        ecart = 175;
    }
    

    SDL_Surface *title = TTF_RenderUTF8_Solid(font, "Victoire du Village", whiteColor);
    SDL_Texture *txr_title = SDL_CreateTextureFromSurface(renderer, title);
    SDL_Rect rect_title = {LONG_WIN/2 - 150, 10, 300, 100};
    SDL_FreeSurface(title);

    SDL_Surface *rejouer = TTF_RenderUTF8_Solid(font, "Rejouer", whiteColor);
    SDL_Texture *txr_rejouer = SDL_CreateTextureFromSurface(renderer, rejouer);
    SDL_Rect rect_rejouer = {LONG_WIN/2 + 37, LARG_WIN-75, 150, 75};
    SDL_FreeSurface(rejouer);

    SDL_Surface *menu = TTF_RenderUTF8_Solid(font, "Menu", whiteColor);
    SDL_Texture *txr_menu = SDL_CreateTextureFromSurface(renderer, menu);
    SDL_Rect rect_menu = {LONG_WIN/2 - 150, LARG_WIN-75, 100, 75};
    SDL_FreeSurface(menu);

    SDL_Surface *tab_noms[nbNonLoup];
    SDL_Surface *tab_role[nbNonLoup];
    SDL_Texture *tab_txr_noms[nbNonLoup];
    SDL_Texture *tab_txr_role[nbNonLoup];
    SDL_Rect tab_rect_nom[nbNonLoup];
    SDL_Rect tab_rect_role[nbNonLoup];

    int j=0;
    for(int i=0; i<jeu->nbJoueur; i++){
        if(jeu->joueurs[i].role != LOUPGAROU){
            tab_noms[j] = TTF_RenderUTF8_Solid(font, jeu->joueurs[i].nom, whiteColor);
            tab_txr_noms[j] = SDL_CreateTextureFromSurface(renderer, tab_noms[j]);
            tab_rect_nom[j].x = LONG_WIN*(2+3*iCol++)/20-strlen(jeu->joueurs[i].nom)*10+20;
            if(ligne>1){
                tab_rect_nom[j].y = LARG_WIN/(nbLigne+1)+(ligne-1)*ecart-50;
            }
            else{
                tab_rect_nom[j].y = LARG_WIN/(nbLigne+1)-50;
            }
            tab_rect_nom[j].w = strlen(jeu->joueurs[i].nom)*20;
            tab_rect_nom[j].h = 40;
            SDL_FreeSurface(tab_noms[j]);

            tab_role[j] = LoadIMGFromRole(&jeu->joueurs[i]);
            tab_txr_role[j] = SDL_CreateTextureFromSurface(renderer, tab_role[j]);
            tab_rect_role[j].w = 100;
            tab_rect_role[j].h = 100;
            tab_rect_role[j].x = tab_rect_nom[j].x + tab_rect_nom[j].w/2 - 50;
            tab_rect_role[j].y = tab_rect_nom[j].y + tab_rect_nom[j].h+5;
            SDL_FreeSurface(tab_role[j]);

            if(j%6==5){
                ligne++;
                iCol=0;
            }
            j++;
        }
    }

    int x,y;
    int retour = 0;
    int run = 1;
    while(run){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    run = 0;
                    break;
                case SDL_MOUSEBUTTONUP:
                    SDL_GetMouseState(&x, &y);
                    if(x>=rect_rejouer.x && x<=rect_rejouer.x+rect_rejouer.w && y>=rect_rejouer.y && y<=rect_rejouer.y+rect_rejouer.h){
                        retour = 1;
                        run = 0;
                    }
                    else if(x>=rect_menu.x && x<=rect_menu.x+rect_menu.w && y>=rect_menu.y && y<=rect_menu.y+rect_menu.h){
                        retour = 0;
                        run = 0;
                    }
            }
        }
        SDL_SetRenderDrawColor(renderer,149,56,58,255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, txr_title, NULL, &rect_title);
        SDL_RenderCopy(renderer, txr_menu, NULL, &rect_menu);
        SDL_RenderCopy(renderer, txr_rejouer, NULL, &rect_rejouer);
        for(int i=0; i<nbNonLoup; i++){
            SDL_RenderCopy(renderer, tab_txr_noms[i], NULL, &tab_rect_nom[i]);
            SDL_RenderCopy(renderer, tab_txr_role[i], NULL, &tab_rect_role[i]);
        }
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyTexture(txr_title);
    SDL_DestroyTexture(txr_menu);
    SDL_DestroyTexture(txr_rejouer);
    for(int i=0; i<nbNonLoup; i++){
        SDL_DestroyTexture(tab_txr_noms[i]);
        SDL_DestroyTexture(tab_txr_role[i]);
    }

    Mix_FadeOutMusic(2000);
    while(Mix_PlayingMusic());

    Mix_FreeMusic(music_lg);

    return retour;
}


int play(jeu_t * jeu, SDL_Renderer * renderer, TTF_Font * font){
    int tour = 0;
    int rejouer = 0;

    while(!verif_vict_amour(jeu) && !verif_vict_LG(jeu) && !verif_vict_village(jeu)){
        tour++;
        mort_t * mortNuit = malloc(sizeof(mort_t));
        mortNuit->iNbMort = 0;
        mortNuit->tab_mort = malloc(sizeof(joueur_t*)*3);
        mortNuit->role_tueur = malloc(sizeof(Roles)*3);

        nuit(tour, jeu, renderer, font, mortNuit);
        affMorts(jeu, renderer, font, mortNuit);
        if(!verif_vict_amour(jeu) && !verif_vict_LG(jeu) && !verif_vict_village(jeu)){
            //jour(jeu, renderer, font, mortNuit);
        }

        free(mortNuit->role_tueur);
        mortNuit->role_tueur = NULL;
        free(mortNuit->tab_mort);
        mortNuit->tab_mort = NULL;
        free(mortNuit);
        mortNuit = NULL;
    }

    Mix_FadeOutMusic(3000);
    while(Mix_PlayingMusic());

    if(verif_vict_amour(jeu)){
        printf("Victoire des amoureux !\n");
        rejouer = victoire_amour(jeu, renderer, font);
    }
    else if(verif_vict_LG(jeu)){
        printf("Victoire des loups garous !\n");
        rejouer = victoire_lg(jeu, renderer, font);
    }
    else if(verif_vict_village(jeu)){
        printf("Victoire des Villageois !\n");
        rejouer = victoire_village(jeu, renderer, font);
    }

    return rejouer;
}