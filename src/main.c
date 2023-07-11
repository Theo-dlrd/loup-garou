#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../head/main.h"
#include "../head/menu.h"
#include "../head/role.h"
#include "../head/jeu.h"

SDL_Color blackColor = {0,0,0,255};
SDL_Color grayColor = {32,32,32,255};
SDL_Color lightgrayColor = {211,211,211,255};
SDL_Color whiteColor = {255,255,255,255};

void aff_jeu(jeu_t* jeu){
    printf("\n\t\tJEU\n");
    printf("\t- Nb joueurs : %d \n", jeu->nbJoueur);
    printf("\t- Nb loups : %d \n", jeu->nbLoupGarou);
    printf("\t- Nb villageois : %d \n", jeu->nbVillageois);
    printf("\t- Nb chasseur : %d \n", jeu->nbChasseur);
    printf("\t- Nb voyante : %d \n", jeu->nbVoyante);
    printf("\t- Nb cupidon : %d \n", jeu->nbCupidon);
    printf("\t- Nb sorciere : %d \n", jeu->nbSorciere);
    printf("\t- Nb petite fille : %d \n", jeu->nbPetiteFille);
    printf("\t- Nb voleur : %d \n\n", jeu->nbVoleur);
    if(jeu->joueurs != NULL){
        int i;
        for(i=0; i<jeu->nbJoueur; i++){
            printf("\n\t* Joueur n°%d : %s", i+1, jeu->joueurs[i].nom);
        }
        printf("\n");
    }
}

int already_given(int numero, jeu_t * jeu, int indice){
    for(int j=0; j<indice; j++){
        if(jeu->joueurs[j].num == numero){
            return 1;
        }
    }
    return 0;
}

int saisieJoueur(jeu_t * jeu, SDL_Renderer* renderer, TTF_Font* font){
    int arret_saisie = 0;
    char * verifNULL;

    jeu->joueurs = malloc(sizeof(joueur_t) * jeu->nbJoueur); 
    for(int i=0; i<jeu->nbJoueur && arret_saisie == 0; i++){
        jeu->joueurs[i].nom = malloc(sizeof(char)*(MAX_NAME_SIZE+1));
        verifNULL = menu_nom(renderer, font, i, jeu);
        if(verifNULL == NULL){
            return 1;
        }
        else{
            strcpy(jeu->joueurs[i].nom, verifNULL);
            free(verifNULL);
            verifNULL = NULL;
        }
        jeu->joueurs[i].etat = VIVANT;
        for(int j=0; j<MAX_STATUT; j++){
            jeu->joueurs[i].statut[j] = AUCUN;
        }
        jeu->joueurs[i].role = NONE;
        do{
            jeu->joueurs[i].num = rand()%jeu->nbJoueur+1;
        }while(already_given(jeu->joueurs[i].num, jeu, i));
    }

    
    return 0;
}

int reInit(jeu_t * jeu){
    for(int i=0; i<jeu->nbJoueur; i++){
        if(jeu->joueurs[i].nomIMG != NULL){
            free(jeu->joueurs[i].nomIMG);
            jeu->joueurs[i].nomIMG = NULL;
        }
    }

    if(jeu->joueurs != NULL){
        free(jeu->joueurs);
        jeu->joueurs = NULL;
    }
    

    if(jeu->nbVoleur>1){
        free(jeu->piocheVoleur[0].nomIMG);
        jeu->piocheVoleur[0].nomIMG = NULL;
        free(jeu->piocheVoleur[1].nomIMG);
        jeu->piocheVoleur[1].nomIMG = NULL;
        free(jeu->piocheVoleur);
        jeu->piocheVoleur = NULL;
    }

    return 0;
}

int destroyJeu(jeu_t * jeu){
    for(int i=0; i<jeu->nbJoueur; i++){
        if(jeu->joueurs[i].nom != NULL){
            free(jeu->joueurs[i].nom);
            jeu->joueurs[i].nom = NULL;
        }
        if(jeu->joueurs[i].nomIMG != NULL){
            free(jeu->joueurs[i].nomIMG);
            jeu->joueurs[i].nomIMG = NULL;
        }
    }

    if(jeu->joueurs != NULL){
        free(jeu->joueurs);
        jeu->joueurs = NULL;
    }
    

    if(jeu->nbVoleur>1){
        free(jeu->piocheVoleur[0].nomIMG);
        jeu->piocheVoleur[0].nomIMG = NULL;
        free(jeu->piocheVoleur[1].nomIMG);
        jeu->piocheVoleur[1].nomIMG = NULL;
        free(jeu->piocheVoleur);
        jeu->piocheVoleur = NULL;
    }

    return 0;
}


int main(){
    
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow("Loup Garou", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, LONG_WIN, LARG_WIN, SDL_WINDOW_FULLSCREEN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_SOFTWARE);

    Mix_Init(MIX_INIT_MP3);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);
    
    Mix_Music* music_menu = Mix_LoadMUS("../sound/music/menu.mp3");
    

    TTF_Init();
    TTF_Font* font = NULL;
    font = TTF_OpenFont("../font/dum.ttf",100);
    if(font==NULL){
        printf("Font introuvable \n");
        return 1;
    }

    int exit = 0, rejouer = 0;
    jeu_t* jeu;

    while(!exit){
        Mix_VolumeMusic(MIX_MAX_VOLUME);
        Mix_PlayMusic(music_menu, -1);
        jeu = menu(renderer, font); 

        if(jeu!=NULL){
            if(!saisieJoueur(jeu, renderer, font)){
                do{
                    if(rejouer){
                        Mix_PlayMusic(music_menu, -1);
                    }

                    if(!initJeu(jeu, renderer, font)){
                        exit = 1;
                        break;
                    }

                    //Mix_HaltMusic();

                    rejouer = play(jeu, renderer, font);
                    printf("%d\n", rejouer);
                    if(rejouer){
                        reInit(jeu);
                    }
                    else{
                        destroyJeu(jeu);
                        free(jeu);
                        jeu = NULL;
                    }
                }while(rejouer);
            }
        }
        else{
            exit = 1;
        }
    }

    if(jeu != NULL){
        destroyJeu(jeu);
        free(jeu);
        jeu = NULL;
    }

    TTF_CloseFont(font);
    TTF_Quit();

    Mix_HaltMusic();
    Mix_FreeMusic(music_menu);
    Mix_CloseAudio();
    Mix_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    printf("Au revoir\n");
    return 0;
    
}
