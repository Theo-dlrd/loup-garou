#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <string.h>

#include "../head/menu.h"

int name_available(char * nom, jeu_t * jeu, int i){
    for(int j=0; j<i; j++){
        if(!strcmp(nom, jeu->joueurs[j].nom)){
            return 0;
        }
    }
    return 1;
}

void * affAllPlayer(SDL_Renderer* renderer, TTF_Font* font, jeu_t * jeu){
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
    

    SDL_Surface *title = TTF_RenderUTF8_Solid(font, "Les joueurs", whiteColor);
    SDL_Texture *txr_title = SDL_CreateTextureFromSurface(renderer, title);
    SDL_Rect rect_title = {LONG_WIN/2 - 150, 10, 200, 100};
    SDL_FreeSurface(title);

    SDL_Surface *begin = TTF_RenderUTF8_Solid(font, "Commencer !", whiteColor);
    SDL_Texture *txr_begin = SDL_CreateTextureFromSurface(renderer, begin);
    SDL_Rect rect_begin = {LONG_WIN/2 + 37, LARG_WIN-75, 150, 75};
    SDL_FreeSurface(begin);

    SDL_Surface *quit = TTF_RenderUTF8_Solid(font, "Quitter", whiteColor);
    SDL_Texture *txr_quit = SDL_CreateTextureFromSurface(renderer, quit);
    SDL_Rect rect_quit = {LONG_WIN/2 - 150, LARG_WIN-75, 100, 75};
    SDL_FreeSurface(menu);

    SDL_Surface *tab_noms[jeu->nbJoueur];
    SDL_Surface *tab_role[jeu->nbJoueur];
    SDL_Texture *tab_txr_noms[jeu->nbJoueur];
    SDL_Texture *tab_txr_role[jeu->nbJoueur];
    SDL_Rect tab_rect_nom[jeu->nbJoueur];
    SDL_Rect tab_rect_role[jeu->nbJoueur];

    int j=0;
    for(int i=0; i<jeu->nbJoueur; i++){
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

        tab_role[j] = IMG_Load(jeu->joueurs[i].nomIMG);
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
                    if(x>=rect_begin.x && x<=rect_begin.x+rect_begin.w && y>=rect_rejouer.y && y<=rect_rejouer.y+rect_rejouer.h){
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
        SDL_RenderCopy(renderer, txr_begin, NULL, &rect_begin);
        SDL_RenderCopy(renderer, txr_quit, NULL, &rect_quit);
        for(int i=0; i<jeu->nbJoueur; i++){
            SDL_RenderCopy(renderer, tab_txr_noms[i], NULL, &tab_rect_nom[i]);
            SDL_RenderCopy(renderer, tab_txr_role[i], NULL, &tab_rect_role[i]);
        }
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyTexture(txr_title);
    SDL_DestroyTexture(txr_begin);
    SDL_DestroyTexture(txr_quit);
    for(int i=0; i<jeu->nbJoueur; i++){
        SDL_DestroyTexture(tab_txr_noms[i]);
        SDL_DestroyTexture(tab_txr_role[i]);
    }

    Mix_FadeOutMusic(2000);
    while(Mix_PlayingMusic());
}

char * menu_nom(SDL_Renderer* renderer, TTF_Font* font, int indice, jeu_t * jeu){
    char * strNumJoueur = malloc(sizeof(char)*20);
    sprintf(strNumJoueur, "Joueur n°%d : ", indice+1);

    SDL_Surface *title = TTF_RenderUTF8_Solid(font, "Saisie le pseudo des joueurs", blackColor);
    SDL_Texture *txr_title = SDL_CreateTextureFromSurface(renderer, title);
    Element title_place = {LONG_WIN*1/2, 0, 500, 75};
    SDL_Rect rect_title = {title_place.x-title_place.longueur/2, title_place.y+10, title_place.longueur, title_place.largeur};
    SDL_FreeSurface(title);

    SDL_Surface *continuer = TTF_RenderUTF8_Solid(font, "Continuer", grayColor);
    SDL_Texture *txr_continuer = SDL_CreateTextureFromSurface(renderer, continuer);
    Button continuer_button = {LONG_WIN*1/2, LARG_WIN, 150, 75};
    SDL_Rect rect_continuer = {continuer_button.x+continuer_button.longueur*0.5, continuer_button.y-continuer_button.largeur, continuer_button.longueur, continuer_button.largeur};
    SDL_FreeSurface(continuer);

    SDL_Surface *back = TTF_RenderUTF8_Solid(font, "Retour", grayColor);
    SDL_Texture *txr_back = SDL_CreateTextureFromSurface(renderer, back);
    Button back_button = {LONG_WIN*1/2, LARG_WIN, 150, 75};
    SDL_Rect rect_back = {back_button.x-back_button.longueur*1.5, back_button.y-back_button.largeur, back_button.longueur, back_button.largeur};
    SDL_FreeSurface(back);

    SDL_Surface *cartes = IMG_Load("../img/all_card.jpg");
    SDL_Texture *txr_cartes = SDL_CreateTextureFromSurface(renderer, cartes);
    SDL_Rect rect_cartes = {LONG_WIN/2 - 250, LARG_WIN/3+100, 500, 250};
    SDL_FreeSurface(cartes);

    SDL_Surface * demandeNom = TTF_RenderUTF8_Solid(font,  strNumJoueur, blackColor);
    SDL_Texture * txr_demandeNom = SDL_CreateTextureFromSurface(renderer, demandeNom);
    SDL_Rect rect_demandeNom = {rect_cartes.x, LARG_WIN/4, 250, 75};
    SDL_FreeSurface(demandeNom);
    free(strNumJoueur);
    strNumJoueur = NULL;

    SDL_Surface * saisieNom = TTF_RenderUTF8_Solid(font, "votre pseudo", blackColor);
    SDL_Texture * txr_saisieNom = SDL_CreateTextureFromSurface(renderer, saisieNom);
    SDL_Rect rect_saisieNom = {rect_demandeNom.x+rect_demandeNom.w,  rect_demandeNom.y, 250, rect_demandeNom.h};
    SDL_FreeSurface(saisieNom);

    SDL_Surface * NoName = TTF_RenderUTF8_Solid(font, "Pseudo indisponible : choisissez-en un autre !", whiteColor);
    SDL_Texture * txr_NoName = SDL_CreateTextureFromSurface(renderer, NoName);
    SDL_Rect rect_NoName = {LONG_WIN/2 - 250, LARG_WIN/3, 500, 75};
    SDL_FreeSurface(NoName);

    SDL_StartTextInput();
    char * textInput = malloc(sizeof(char)*MAX_NAME_SIZE);
    int textInputSize = 0;
    
    int run = 1;
    int x, y;
    while(run){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    run = 0;
                    break;
                case SDL_MOUSEBUTTONUP:
                    SDL_GetMouseState(&x, &y);
                    if (x >= rect_back.x && x <= rect_back.x + rect_back.w && y >= rect_back.y && y <= rect_back.y + rect_back.h){
                        run = 0;
                        textInputSize = 0;
                        free(textInput);
                        textInput = NULL;
                    }
                    else if (x >= rect_continuer.x && x <= rect_continuer.x + rect_continuer.w && y >= rect_continuer.y && y <= rect_continuer.y + rect_continuer.h && textInputSize>0 && strcmp(textInput,"")){
                        if(name_available(textInput, jeu, indice)){
                            textInputSize++;
                            textInput[textInputSize] = '\0';
                            run = 0;
                        }
                    } 
                case SDL_TEXTINPUT:
                    if(textInputSize < MAX_NAME_SIZE-1 && event.text.text[0] != ' ' && ((event.text.text[0] >= 'a' && event.text.text[0] <= 'z') || (event.text.text[0] >= 'A' && event.text.text[0] <= 'Z'))){
                        textInput[textInputSize] = event.text.text[0];
                        textInputSize++;
                        textInput[textInputSize] = '\0';
                        if(txr_saisieNom){
                            SDL_DestroyTexture(txr_saisieNom);
                        }
                        saisieNom = TTF_RenderUTF8_Solid(font, textInput, blackColor);
                        if(saisieNom){
                            txr_saisieNom = SDL_CreateTextureFromSurface(renderer, saisieNom);
                            rect_saisieNom.w = 30 * textInputSize;
                            SDL_FreeSurface(saisieNom);
                        }
                    }
                    break;
                case SDL_KEYDOWN:
                    if(event.key.keysym.sym == SDLK_BACKSPACE && textInputSize>0){
                        textInputSize--;
                        textInput[textInputSize] = '\0';

                        saisieNom = TTF_RenderUTF8_Solid(font, textInput, blackColor);
                        if(saisieNom){
                            txr_saisieNom = SDL_CreateTextureFromSurface(renderer, saisieNom);
                            rect_saisieNom.w = 30 * textInputSize;
                            SDL_FreeSurface(saisieNom);
                        }
                        else{
                            saisieNom = TTF_RenderUTF8_Solid(font, "votre pseudo", blackColor);
                            txr_saisieNom = SDL_CreateTextureFromSurface(renderer, saisieNom);
                            rect_saisieNom.w = 250;
                            textInputSize = 0;
                            SDL_FreeSurface(saisieNom);
                        }
                    }
                    else if(event.key.keysym.sym == SDLK_RETURN && textInputSize>0 && strcmp(textInput,"") && name_available(textInput, jeu, indice)){
                        textInputSize++;
                        textInput[textInputSize] = '\0';
                        run = 0;
                    }
                default: break;
            }
        }
        SDL_SetRenderDrawColor(renderer,149,56,58,255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, txr_back, NULL, &rect_back);
        SDL_RenderCopy(renderer, txr_cartes, NULL, &rect_cartes);
        SDL_RenderCopy(renderer, txr_continuer, NULL, &rect_continuer);
        SDL_RenderCopy(renderer, txr_demandeNom, NULL, &rect_demandeNom);
        SDL_RenderCopy(renderer, txr_saisieNom, NULL, &rect_saisieNom);
        SDL_RenderCopy(renderer, txr_title, NULL, &rect_title);
        if(!name_available(textInput, jeu, indice)){
            SDL_RenderCopy(renderer, txr_NoName, NULL, &rect_NoName);
        }
        SDL_RenderPresent(renderer);
    }
    SDL_StopTextInput();
    SDL_DestroyTexture(txr_cartes);
    SDL_DestroyTexture(txr_back);
    SDL_DestroyTexture(txr_continuer);
    SDL_DestroyTexture(txr_demandeNom);
    SDL_DestroyTexture(txr_saisieNom);
    SDL_DestroyTexture(txr_title);
    SDL_DestroyTexture(txr_NoName);
    return textInput;
}


int rules_menu(SDL_Renderer* renderer, TTF_Font* font){
    Regles * regles = initRegles();
    SDL_Surface *title = TTF_RenderUTF8_Solid(font, "Règles du jeu", blackColor);
    SDL_Texture *txr_title = SDL_CreateTextureFromSurface(renderer, title);
    Element title_place = {LONG_WIN*1/2, 0, 200, 75};
    SDL_Rect rect_title = {title_place.x-title_place.longueur/2, title_place.y+10, title_place.longueur, title_place.largeur};
    SDL_FreeSurface(title);

    SDL_Surface *back = TTF_RenderUTF8_Solid(font, "Retour", grayColor);
    SDL_Texture *txr_back = SDL_CreateTextureFromSurface(renderer, back);
    Button back_button = {LONG_WIN*1/2, LARG_WIN, 150, 75};
    SDL_Rect rect_back = {back_button.x-back_button.longueur/2, back_button.y-back_button.largeur, back_button.longueur, back_button.largeur};
    SDL_FreeSurface(back);

    SDL_Surface *expl1 = TTF_RenderUTF8_Solid(font, regles->explications[0], blackColor);
    SDL_Texture *txr_expl1 = SDL_CreateTextureFromSurface(renderer, expl1);
    SDL_Rect rect_expl1 = {100, LARG_WIN*3/20-20, 703, 40};
    SDL_FreeSurface(expl1);

    SDL_Surface *expl2 = TTF_RenderUTF8_Solid(font, regles->explications[1], blackColor);
    SDL_Texture *txr_expl2 = SDL_CreateTextureFromSurface(renderer, expl2);
    SDL_Rect rect_expl2 = {100, rect_expl1.y+rect_expl1.h, 827, 40};
    SDL_FreeSurface(expl2);

    SDL_Surface *expl3 = TTF_RenderUTF8_Solid(font, regles->explications[2], blackColor);
    SDL_Texture *txr_expl3 = SDL_CreateTextureFromSurface(renderer, expl3);
    SDL_Rect rect_expl3 = {100, rect_expl2.y+rect_expl2.h, 1000, 40};
    SDL_FreeSurface(expl3);

    SDL_Surface *but1 = TTF_RenderUTF8_Solid(font, regles->obj[0], blackColor);
    SDL_Texture *txr_but1 = SDL_CreateTextureFromSurface(renderer, but1);
    SDL_Rect rect_but1 = {50, LARG_WIN*14/20, 900, 40};
    SDL_FreeSurface(but1);

    SDL_Surface *but2 = TTF_RenderUTF8_Solid(font, regles->obj[1], blackColor);
    SDL_Texture *txr_but2 = SDL_CreateTextureFromSurface(renderer, but2);
    SDL_Rect rect_but2 = {50, rect_but1.y+rect_but1.h, 800, 40};
    SDL_FreeSurface(but2);

    SDL_Surface *but3 = TTF_RenderUTF8_Solid(font, regles->obj[2], blackColor);
    SDL_Texture *txr_but3 = SDL_CreateTextureFromSurface(renderer, but3);
    SDL_Rect rect_but3 = {50, rect_but2.y+rect_but2.h, 1100, 40};
    SDL_FreeSurface(but3);


    /*----------------------------------*/
    /*          Loup Garou              */
    /*----------------------------------*/
    SDL_Surface *lg = IMG_Load("../img/loup-garou.png");
    SDL_Texture *txr_lg = SDL_CreateTextureFromSurface(renderer, lg);
    SDL_Rect rect_lg = {20, LARG_WIN*6/20, 60, 60};
    SDL_FreeSurface(lg);

    SDL_Surface *desc_lg = TTF_RenderUTF8_Solid(font, "Loup-Garou : Tue un villageois chaque nuit", blackColor);
    SDL_Texture *txr_desc_lg = SDL_CreateTextureFromSurface(renderer, desc_lg);
    SDL_Rect rect_desc_lg = {10+rect_lg.x+rect_lg.w, rect_lg.y+10, 400, 40};
    SDL_FreeSurface(desc_lg);

    /*----------------------------------*/
    /*          Villageois              */
    /*----------------------------------*/
    SDL_Surface *vil = IMG_Load("../img/villageois.png");
    SDL_Texture *txr_vil = SDL_CreateTextureFromSurface(renderer, vil);
    SDL_Rect rect_vil = {20, LARG_WIN*8/20, 60, 60};
    SDL_FreeSurface(vil);

    SDL_Surface *desc_vil = TTF_RenderUTF8_Solid(font, "Villageois : Aucun", blackColor);
    SDL_Texture *txr_desc_vil = SDL_CreateTextureFromSurface(renderer, desc_vil);
    SDL_Rect rect_desc_vil = {10+rect_vil.x+rect_vil.w, rect_vil.y+10, 200, 40};
    SDL_FreeSurface(desc_vil);
    
    /*----------------------------------*/
    /*           Le Chasseur            */
    /*----------------------------------*/
    SDL_Surface *ch = IMG_Load("../img/chasseur.png");
    SDL_Texture *txr_ch = SDL_CreateTextureFromSurface(renderer, ch);
    SDL_Rect rect_ch = {20, LARG_WIN*10/20, 60, 60};
    SDL_FreeSurface(ch);

    SDL_Surface *desc_ch = TTF_RenderUTF8_Solid(font, "Chasseur : Il peut choisir d'éliminer un autre joueur à sa mort.", blackColor);
    SDL_Texture *txr_desc_ch = SDL_CreateTextureFromSurface(renderer, desc_ch);
    SDL_Rect rect_desc_ch = {10+rect_ch.x+rect_ch.w, rect_ch.y+10, 500, 40};
    SDL_FreeSurface(desc_ch);

    /*----------------------------------*/
    /*          La Voyante              */
    /*----------------------------------*/
    SDL_Surface *voy = IMG_Load("../img/voyante.png");
    SDL_Texture *txr_voy = SDL_CreateTextureFromSurface(renderer, voy);
    SDL_Rect rect_voy = {20, LARG_WIN*12/20, 60, 60};
    SDL_FreeSurface(voy);

    SDL_Surface *desc_voy = TTF_RenderUTF8_Solid(font, "Voyante : Peut observer la carte d'un joueur chaque nuit", blackColor);
    SDL_Texture *txr_desc_voy = SDL_CreateTextureFromSurface(renderer, desc_voy);
    SDL_Rect rect_desc_voy = {10+rect_voy.x+rect_voy.w, rect_voy.y+10, 450, 40};
    SDL_FreeSurface(desc_voy);

    /*----------------------------------*/
    /*              Cupidon             */
    /*----------------------------------*/
    SDL_Surface *cu = IMG_Load("../img/cupidon.png");
    SDL_Texture *txr_cu = SDL_CreateTextureFromSurface(renderer, cu);
    SDL_Rect rect_cu = {LONG_WIN/2, LARG_WIN*6/20, 60, 60};
    SDL_FreeSurface(cu);

    SDL_Surface *desc_cu = TTF_RenderUTF8_Solid(font, "Cupidon : Désigne deux amoureux (si l'un meurt l'autre se suicide)", blackColor);
    SDL_Texture *txr_desc_cu = SDL_CreateTextureFromSurface(renderer, desc_cu);
    SDL_Rect rect_desc_cu = {10+rect_cu.x+rect_cu.w, rect_cu.y+10, 500, 40};
    SDL_FreeSurface(desc_cu);

    /*----------------------------------*/
    /*          La Sorcière             */
    /*----------------------------------*/
    SDL_Surface *so = IMG_Load("../img/sorciere.png");
    SDL_Texture *txr_so = SDL_CreateTextureFromSurface(renderer, so);
    SDL_Rect rect_so = {LONG_WIN/2, LARG_WIN*8/20, 60, 60};
    SDL_FreeSurface(so);

    SDL_Surface *desc_so = TTF_RenderUTF8_Solid(font, "Sorcière : Dispose d'une potion de vie et d'une potion de mort", blackColor);
    SDL_Texture *txr_desc_so = SDL_CreateTextureFromSurface(renderer, desc_so);
    SDL_Rect rect_desc_so = {10+rect_so.x+rect_so.w, rect_so.y+10, 500, 40};
    SDL_FreeSurface(desc_so);

    /*----------------------------------*/
    /*        La petite fille           */
    /*----------------------------------*/
    SDL_Surface *pf = IMG_Load("../img/petite-fille.png");
    SDL_Texture *txr_pf = SDL_CreateTextureFromSurface(renderer, pf);
    SDL_Rect rect_pf = {LONG_WIN/2, LARG_WIN*10/20, 60, 60};
    SDL_FreeSurface(pf);

    SDL_Surface *desc_pf = TTF_RenderUTF8_Solid(font, "Petite Fille : Peut espionner les loups durant la nuit", blackColor);
    SDL_Texture *txr_desc_pf = SDL_CreateTextureFromSurface(renderer, desc_pf);
    SDL_Rect rect_desc_pf = {10+rect_pf.x+rect_pf.w, rect_pf.y+10, 450, 40};
    SDL_FreeSurface(desc_pf);

    /*----------------------------------*/
    /*          Le Voleur               */
    /*----------------------------------*/
    SDL_Surface *vo = IMG_Load("../img/voleur.png");
    SDL_Texture *txr_vo = SDL_CreateTextureFromSurface(renderer, vo);
    SDL_Rect rect_vo = {LONG_WIN/2, LARG_WIN*12/20, 60, 60};
    SDL_FreeSurface(vo);

    SDL_Surface *desc_vo = TTF_RenderUTF8_Solid(font, "Voleur : Peut choisir une carte parmi deux en début de partie", blackColor);
    SDL_Texture *txr_desc_vo = SDL_CreateTextureFromSurface(renderer, desc_vo);
    SDL_Rect rect_desc_vo = {10+rect_vo.x+rect_vo.w, rect_vo.y+10, 500, 40};
    SDL_FreeSurface(desc_vo);
    
    SDL_SetRenderDrawColor(renderer,149,56,58,255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, txr_title, NULL, &rect_title);
    SDL_RenderCopy(renderer, txr_back, NULL, &rect_back);

    SDL_RenderCopy(renderer, txr_expl1, NULL, &rect_expl1);
    SDL_RenderCopy(renderer, txr_expl2, NULL, &rect_expl2);
    SDL_RenderCopy(renderer, txr_expl3, NULL, &rect_expl3);

    SDL_RenderCopy(renderer, txr_but1, NULL, &rect_but1);
    SDL_RenderCopy(renderer, txr_but2, NULL, &rect_but2);
    SDL_RenderCopy(renderer, txr_but3, NULL, &rect_but3);

    SDL_RenderCopy(renderer, txr_lg, NULL, &rect_lg);
    SDL_RenderCopy(renderer, txr_desc_lg, NULL, &rect_desc_lg);
    SDL_RenderCopy(renderer, txr_vil, NULL, &rect_vil);
    SDL_RenderCopy(renderer, txr_desc_vil, NULL, &rect_desc_vil);
    SDL_RenderCopy(renderer, txr_ch, NULL, &rect_ch);
    SDL_RenderCopy(renderer, txr_desc_ch, NULL, &rect_desc_ch);
    SDL_RenderCopy(renderer, txr_voy, NULL, &rect_voy);
    SDL_RenderCopy(renderer, txr_desc_voy, NULL, &rect_desc_voy);
    SDL_RenderCopy(renderer, txr_cu, NULL, &rect_cu);
    SDL_RenderCopy(renderer, txr_desc_cu, NULL, &rect_desc_cu);
    SDL_RenderCopy(renderer, txr_so, NULL, &rect_so);
    SDL_RenderCopy(renderer, txr_desc_so, NULL, &rect_desc_so);
    SDL_RenderCopy(renderer, txr_pf, NULL, &rect_pf);
    SDL_RenderCopy(renderer, txr_desc_pf, NULL, &rect_desc_pf);
    SDL_RenderCopy(renderer, txr_vo, NULL, &rect_vo);
    SDL_RenderCopy(renderer, txr_desc_vo, NULL, &rect_desc_vo);
    
    SDL_RenderPresent(renderer);

    int x, y;
    int run=1;
    while(run){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
            case SDL_QUIT: 
                run=0; 
                break;

            case SDL_MOUSEBUTTONUP:
                SDL_GetMouseState(&x,&y);
                if(x>=back_button.x-back_button.longueur/2 && x<=back_button.x-back_button.longueur/2+back_button.longueur && y>=back_button.y-back_button.largeur-10 && y<=back_button.y-back_button.largeur-10+back_button.largeur){
                    run=0;
                }
            
            default:
                break;
            }
        }
    }
    SDL_DestroyTexture(txr_back);
    SDL_DestroyTexture(txr_title);

    SDL_DestroyTexture(txr_expl1);
    SDL_DestroyTexture(txr_expl2);
    SDL_DestroyTexture(txr_expl3);

    SDL_DestroyTexture(txr_but1);
    SDL_DestroyTexture(txr_but2);
    SDL_DestroyTexture(txr_but3);

    SDL_DestroyTexture(txr_lg);
    SDL_DestroyTexture(txr_desc_lg);
    SDL_DestroyTexture(txr_vil);
    SDL_DestroyTexture(txr_desc_vil);
    SDL_DestroyTexture(txr_ch);
    SDL_DestroyTexture(txr_desc_ch);
    SDL_DestroyTexture(txr_voy);
    SDL_DestroyTexture(txr_desc_voy);
    SDL_DestroyTexture(txr_cu);
    SDL_DestroyTexture(txr_desc_cu);
    SDL_DestroyTexture(txr_so);
    SDL_DestroyTexture(txr_desc_so);
    SDL_DestroyTexture(txr_pf);
    SDL_DestroyTexture(txr_desc_pf);
    SDL_DestroyTexture(txr_vo);
    SDL_DestroyTexture(txr_desc_vo);
    destroyRegles(regles);
    
    return 0;
}

jeu_t* online_menu(SDL_Renderer* renderer, TTF_Font* font){
    SDL_Surface *coming = TTF_RenderText_Solid(font, "Coming soon...", blackColor);
    SDL_Texture *txr_coming = SDL_CreateTextureFromSurface(renderer, coming);
    Element coming_place = {LONG_WIN*1/2, 0, 200, 75};
    SDL_Rect rect_coming = {coming_place.x-coming_place.longueur/2, coming_place.y+10, coming_place.longueur, coming_place.largeur};
    SDL_FreeSurface(coming);

    SDL_Surface *back = TTF_RenderText_Solid(font, "Retour", grayColor);
    SDL_Texture *txr_back = SDL_CreateTextureFromSurface(renderer, back);
    Button back_button = {LONG_WIN*1/2, LARG_WIN, 150, 75};
    SDL_Rect rect_back = {back_button.x-back_button.longueur/2, back_button.y-back_button.largeur, back_button.longueur, back_button.largeur};
    SDL_FreeSurface(back);

    SDL_SetRenderDrawColor(renderer,149,56,58,255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, txr_coming, NULL, &rect_coming);
    SDL_RenderCopy(renderer, txr_back, NULL, &rect_back);
    SDL_RenderPresent(renderer);

    int x, y;
    int run=1;
    while(run){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
            case SDL_QUIT: 
                run=0; 
                break;

            case SDL_MOUSEBUTTONUP:
                SDL_GetMouseState(&x,&y);
                if(x>=back_button.x-back_button.longueur/2 && x<=back_button.x-back_button.longueur/2+back_button.longueur && y>=back_button.y-back_button.largeur-10 && y<=back_button.y-back_button.largeur-10+back_button.largeur){
                    run=0;
                    printf("Retour au menu général\n");
                }
            
            default:
                break;
            }
        }
    }

    SDL_DestroyTexture(txr_back);
    SDL_DestroyTexture(txr_coming);

    return NULL;
}

// Fonction pour échanger deux nombres
void swap(char *x, char *y) {
    char t = *x; *x = *y; *y = t;
}
 
// Fonction pour inverser `buffer[i…j]`
char* reverse(char *buffer, int i, int j)
{
    while (i < j) {
        swap(&buffer[i++], &buffer[j--]);
    }
 
    return buffer;
}
 
// Fonction itérative pour implémenter la fonction `itoa()` en C
char* itoa(int value, char* buffer){
    // considère la valeur absolue du nombre
    int n = abs(value);
    int i = 0;
    while (n){
        int r = n % 10;
        if (r >= 10) {
            buffer[i++] = 65 + (r - 10);
        }
        else {
            buffer[i++] = 48 + r;
        }
 
        n = n / 10;
    }
 
    // si le nombre est 0
    if (i == 0) {
        buffer[i++] = '0';
    }
 
    // Si la base est 10 et la valeur est négative, la string résultante
    // est précédé d'un signe moins (-)
    // Avec toute autre base, la valeur est toujours considérée comme non signée
    if (value < 0) {
        buffer[i++] = '-';
    }
 
    buffer[i] = '\0'; // string de fin nulle
 
    // inverse la string et la renvoie
    return reverse(buffer, 0, i - 1);
}

int joueursEgalRoles(jeu_t* jeu){
    int somme = jeu->nbLoupGarou+jeu->nbVillageois+jeu->nbChasseur+jeu->nbVoyante+jeu->nbCupidon+jeu->nbSorciere+jeu->nbPetiteFille+jeu->nbVoleur;
    return (somme==jeu->nbJoueur);
}

int nbCartes(jeu_t * jeu){
    return jeu->nbLoupGarou+jeu->nbVillageois+jeu->nbChasseur+jeu->nbVoyante+jeu->nbCupidon+jeu->nbSorciere+jeu->nbPetiteFille+jeu->nbVoleur;
}


jeu_t* local_menu(SDL_Renderer* renderer, TTF_Font* font){
    jeu_t* jeu = malloc(sizeof(jeu_t));
    jeu->nbJoueur = 6;
    jeu->nbLoupGarou = 2;
    jeu->nbVillageois = 1;
    jeu->nbChasseur = 1;
    jeu->nbVoyante = 1;
    jeu->nbCupidon = 0;
    jeu->nbSorciere = 1;
    jeu->nbPetiteFille = 0;
    jeu->nbVoleur = 0;
    jeu->joueurs = NULL;

    char charJoueur[3];
    char charCarte[3];
    char charLoupGarou[3];    
    char charVillageois[3];   
    char charChasseur[3];     
    char charVoyante[3];      
    char charCupidon[3];      
    char charSorciere[3];     
    char charPetiteFille[3];  
    char charVoleur[3];       

    itoa(jeu->nbJoueur, charJoueur);
    itoa(jeu->nbLoupGarou, charLoupGarou);
    itoa(jeu->nbVillageois, charVillageois);
    itoa(jeu->nbChasseur, charChasseur);
    itoa(jeu->nbVoyante, charVoyante);
    itoa(jeu->nbCupidon, charCupidon);
    itoa(jeu->nbSorciere, charSorciere);
    itoa(jeu->nbPetiteFille, charPetiteFille);
    itoa(jeu->nbVoleur, charVoleur);
    itoa(nbCartes(jeu), charCarte);

    SDL_Surface *title = TTF_RenderUTF8_Solid(font, "Partie locale", blackColor);
    SDL_Texture *txr_title = SDL_CreateTextureFromSurface(renderer, title);
    Element title_place = {LONG_WIN*1/2, 0, 200, 75};
    SDL_Rect rect_title = {title_place.x-title_place.longueur/2, title_place.y+10, title_place.longueur, title_place.largeur};
    SDL_FreeSurface(title);

    SDL_Surface *back = TTF_RenderUTF8_Solid(font, "Retour", grayColor);
    SDL_Texture *txr_back = SDL_CreateTextureFromSurface(renderer, back);
    Button back_button = {LONG_WIN*1/2, LARG_WIN, 150, 75};
    SDL_Rect rect_back = {back_button.x-back_button.longueur*1.5, back_button.y-back_button.largeur, back_button.longueur, back_button.largeur};
    SDL_FreeSurface(back);
    
    SDL_Surface *continuer = TTF_RenderUTF8_Solid(font, "Continuer", grayColor);
    SDL_Texture *txr_continuer = SDL_CreateTextureFromSurface(renderer, continuer);
    Button continuer_button = {LONG_WIN*1/2, LARG_WIN, 150, 75};
    SDL_Rect rect_continuer = {continuer_button.x+continuer_button.longueur*0.5, continuer_button.y-continuer_button.largeur, continuer_button.longueur, continuer_button.largeur};
    SDL_FreeSurface(continuer);

    SDL_Surface *textnbJoueur = TTF_RenderUTF8_Solid(font, "Nombre de joueurs : ", blackColor);
    SDL_Texture *txr_textnbJoueur = SDL_CreateTextureFromSurface(renderer, textnbJoueur);
    SDL_Rect rect_textnbJoueur = {LONG_WIN/4, LARG_WIN*3/20, 200, 30};
    SDL_FreeSurface(textnbJoueur);

    SDL_Surface * temp_joueur = TTF_RenderUTF8_Solid(font, charJoueur, blackColor);
    SDL_Texture * txr_temp_joueur = SDL_CreateTextureFromSurface(renderer, temp_joueur);
    SDL_Rect rect_temp_joueur = {rect_textnbJoueur.x+rect_textnbJoueur.w, LARG_WIN*3/20, 20, 30};
    SDL_FreeSurface(temp_joueur);

    SDL_Surface * moins_joueur = TTF_RenderUTF8_Solid(font, " - ", blackColor);
    SDL_Texture *txr_moins_joueur = SDL_CreateTextureFromSurface(renderer, moins_joueur);
    SDL_Rect rect_moins_joueur = {rect_temp_joueur.x+rect_temp_joueur.w+10, rect_temp_joueur.y, 20, 30};
    SDL_FreeSurface(moins_joueur);

    SDL_Surface * plus_joueur = TTF_RenderUTF8_Solid(font, " + ", blackColor);
    SDL_Texture *txr_plus_joueur = SDL_CreateTextureFromSurface(renderer, plus_joueur);
    SDL_Rect rect_plus_joueur = {rect_moins_joueur.x+rect_moins_joueur.w, rect_moins_joueur.y, 20, 30};
    SDL_FreeSurface(plus_joueur);

    SDL_Surface *textnbCarte = TTF_RenderUTF8_Solid(font, "Nombre de cartes attribuées : ", blackColor);
    SDL_Texture *txr_textnbCarte = SDL_CreateTextureFromSurface(renderer, textnbCarte);
    SDL_Rect rect_textnbCarte = {LONG_WIN/2+75, LARG_WIN*3/20, 300, 30};
    SDL_FreeSurface(textnbCarte);

    SDL_Surface * temp_carte = TTF_RenderUTF8_Solid(font, charCarte, blackColor);
    SDL_Texture * txr_temp_carte = SDL_CreateTextureFromSurface(renderer, temp_carte);
    SDL_Rect rect_temp_carte = {rect_textnbCarte.x+rect_textnbCarte.w, LARG_WIN*3/20, 20, 30};
    SDL_FreeSurface(temp_carte);

    /*----------------------------------*/
    /*          Loup Garou              */
    /*----------------------------------*/
    SDL_Surface *lg = IMG_Load("../img/loup-garou.png");
    SDL_Texture *txr_lg = SDL_CreateTextureFromSurface(renderer, lg);
    SDL_Rect rect_lg = {LONG_WIN/3-100, LARG_WIN*5/20, 100, 100};
    SDL_FreeSurface(lg);

    SDL_Surface *desc_lg = TTF_RenderUTF8_Solid(font, "X ", blackColor);
    SDL_Texture *txr_desc_lg = SDL_CreateTextureFromSurface(renderer, desc_lg);
    SDL_Rect rect_desc_lg = {10+rect_lg.x+rect_lg.w, rect_lg.y+30, 40, 40};
    SDL_FreeSurface(desc_lg);

    SDL_Surface * temp_lg = TTF_RenderUTF8_Solid(font, charLoupGarou, blackColor);
    SDL_Texture * txr_temp_lg = SDL_CreateTextureFromSurface(renderer, temp_lg);
    SDL_Rect rect_temp_lg = {rect_desc_lg.x+rect_desc_lg.w, rect_desc_lg.y, 30, 40};
    SDL_FreeSurface(temp_lg);

    SDL_Surface * moins_lg = TTF_RenderUTF8_Solid(font, " - ", blackColor);
    SDL_Texture *txr_moins_lg = SDL_CreateTextureFromSurface(renderer, moins_lg);
    SDL_Rect rect_moins_lg = {rect_temp_lg.x+rect_temp_lg.w+10, rect_temp_lg.y, 20, 30};
    SDL_FreeSurface(moins_lg);

    SDL_Surface * plus_lg = TTF_RenderUTF8_Solid(font, " + ", blackColor);
    SDL_Texture *txr_plus_lg = SDL_CreateTextureFromSurface(renderer, plus_lg);
    SDL_Rect rect_plus_lg = {rect_moins_lg.x+rect_moins_lg.w, rect_moins_lg.y, 20, 30};
    SDL_FreeSurface(plus_lg);

    /*----------------------------------*/
    /*          Villageois              */
    /*----------------------------------*/
    SDL_Surface *vil = IMG_Load("../img/villageois.png");
    SDL_Texture *txr_vil = SDL_CreateTextureFromSurface(renderer, vil);
    SDL_Rect rect_vil = {LONG_WIN/3-100, LARG_WIN*8/20, 100, 100};
    SDL_FreeSurface(vil);

    SDL_Surface *desc_vil = TTF_RenderUTF8_Solid(font, "X ", blackColor);
    SDL_Texture *txr_desc_vil = SDL_CreateTextureFromSurface(renderer, desc_vil);
    SDL_Rect rect_desc_vil = {10+rect_vil.x+rect_vil.w, rect_vil.y+30, 40, 40};
    SDL_FreeSurface(desc_vil);

    SDL_Surface * temp_vil = TTF_RenderUTF8_Solid(font, charVillageois, blackColor);
    SDL_Texture * txr_temp_vil = SDL_CreateTextureFromSurface(renderer, temp_vil);
    SDL_Rect rect_temp_vil = {rect_desc_vil.x+rect_desc_vil.w, rect_desc_vil.y, 30, 40};
    SDL_FreeSurface(temp_vil);

    SDL_Surface * moins_vil = TTF_RenderUTF8_Solid(font, " - ", blackColor);
    SDL_Texture *txr_moins_vil = SDL_CreateTextureFromSurface(renderer, moins_vil);
    SDL_Rect rect_moins_vil = {rect_temp_vil.x+rect_temp_vil.w+10, rect_temp_vil.y, 20, 30};
    SDL_FreeSurface(moins_vil);

    SDL_Surface * plus_vil = TTF_RenderUTF8_Solid(font, " + ", blackColor);
    SDL_Texture *txr_plus_vil = SDL_CreateTextureFromSurface(renderer, plus_vil);
    SDL_Rect rect_plus_vil = {rect_moins_vil.x+rect_moins_vil.w, rect_moins_vil.y, 20, 30};
    SDL_FreeSurface(plus_vil);
    
    /*----------------------------------*/
    /*           Le Chasseur            */
    /*----------------------------------*/
    SDL_Surface *ch = IMG_Load("../img/chasseur.png");
    SDL_Texture *txr_ch = SDL_CreateTextureFromSurface(renderer, ch);
    SDL_Rect rect_ch = {LONG_WIN/3-100, LARG_WIN*11/20, 100, 100};
    SDL_FreeSurface(ch);

    SDL_Surface *desc_ch = TTF_RenderUTF8_Solid(font, "X ", blackColor);
    SDL_Texture *txr_desc_ch = SDL_CreateTextureFromSurface(renderer, desc_ch);
    SDL_Rect rect_desc_ch = {10+rect_ch.x+rect_ch.w, rect_ch.y+30, 40, 40};
    SDL_FreeSurface(desc_ch);

    SDL_Surface * temp_ch = TTF_RenderUTF8_Solid(font, charChasseur, blackColor);
    SDL_Texture * txr_temp_ch = SDL_CreateTextureFromSurface(renderer, temp_ch);
    SDL_Rect rect_temp_ch = {rect_desc_ch.x+rect_desc_ch.w, rect_desc_ch.y, 30, 40};
    SDL_FreeSurface(temp_ch);

    SDL_Surface * moins_ch = TTF_RenderUTF8_Solid(font, " - ", blackColor);
    SDL_Texture *txr_moins_ch = SDL_CreateTextureFromSurface(renderer, moins_ch);
    SDL_Rect rect_moins_ch = {rect_temp_ch.x+rect_temp_ch.w+10, rect_temp_ch.y, 20, 30};
    SDL_FreeSurface(moins_ch);

    SDL_Surface * plus_ch = TTF_RenderUTF8_Solid(font, " + ", blackColor);
    SDL_Texture *txr_plus_ch = SDL_CreateTextureFromSurface(renderer, plus_ch);
    SDL_Rect rect_plus_ch = {rect_moins_ch.x+rect_moins_ch.w, rect_moins_ch.y, 20, 30};
    SDL_FreeSurface(plus_ch);

    /*----------------------------------*/
    /*          La Voyante              */
    /*----------------------------------*/
    SDL_Surface *voy = IMG_Load("../img/voyante.png");
    SDL_Texture *txr_voy = SDL_CreateTextureFromSurface(renderer, voy);
    SDL_Rect rect_voy = {LONG_WIN/3-100, LARG_WIN*14/20, 100, 100};
    SDL_FreeSurface(voy);

    SDL_Surface *desc_voy = TTF_RenderUTF8_Solid(font, "X ", blackColor);
    SDL_Texture *txr_desc_voy = SDL_CreateTextureFromSurface(renderer, desc_voy);
    SDL_Rect rect_desc_voy = {10+rect_voy.x+rect_voy.w, rect_voy.y+30, 40, 40};
    SDL_FreeSurface(desc_voy);

    SDL_Surface * temp_voy = TTF_RenderUTF8_Solid(font, charVoyante, blackColor);
    SDL_Texture * txr_temp_voy = SDL_CreateTextureFromSurface(renderer, temp_voy);
    SDL_Rect rect_temp_voy = {rect_desc_voy.x+rect_desc_voy.w, rect_desc_voy.y, 30, 40};
    SDL_FreeSurface(temp_voy);

    SDL_Surface * moins_voy = TTF_RenderUTF8_Solid(font, " - ", blackColor);
    SDL_Texture *txr_moins_voy = SDL_CreateTextureFromSurface(renderer, moins_voy);
    SDL_Rect rect_moins_voy = {rect_temp_voy.x+rect_temp_voy.w+10, rect_temp_voy.y, 20, 30};
    SDL_FreeSurface(moins_voy);

    SDL_Surface * plus_voy = TTF_RenderUTF8_Solid(font, " + ", blackColor);
    SDL_Texture *txr_plus_voy = SDL_CreateTextureFromSurface(renderer, plus_voy);
    SDL_Rect rect_plus_voy = {rect_moins_voy.x+rect_moins_voy.w, rect_moins_voy.y, 20, 30};
    SDL_FreeSurface(plus_voy);

    /*----------------------------------*/
    /*              Cupidon             */
    /*----------------------------------*/
    SDL_Surface *cu = IMG_Load("../img/cupidon.png");
    SDL_Texture *txr_cu = SDL_CreateTextureFromSurface(renderer, cu);
    SDL_Rect rect_cu = {LONG_WIN*2/3-100, LARG_WIN*5/20, 100, 100};
    SDL_FreeSurface(cu);

    SDL_Surface *desc_cu = TTF_RenderUTF8_Solid(font, "X ", blackColor);
    SDL_Texture *txr_desc_cu = SDL_CreateTextureFromSurface(renderer, desc_cu);
    SDL_Rect rect_desc_cu = {10+rect_cu.x+rect_cu.w, rect_cu.y+30, 40, 40};
    SDL_FreeSurface(desc_cu);

    SDL_Surface * temp_cu = TTF_RenderUTF8_Solid(font, charCupidon, blackColor);
    SDL_Texture * txr_temp_cu = SDL_CreateTextureFromSurface(renderer, temp_cu);
    SDL_Rect rect_temp_cu = {rect_desc_cu.x+rect_desc_cu.w, rect_desc_cu.y, 30, 40};
    SDL_FreeSurface(temp_cu);

    SDL_Surface * moins_cu = TTF_RenderUTF8_Solid(font, " - ", blackColor);
    SDL_Texture *txr_moins_cu = SDL_CreateTextureFromSurface(renderer, moins_cu);
    SDL_Rect rect_moins_cu = {rect_temp_cu.x+rect_temp_cu.w+10, rect_temp_cu.y, 20, 30};
    SDL_FreeSurface(moins_cu);

    SDL_Surface * plus_cu = TTF_RenderUTF8_Solid(font, " + ", blackColor);
    SDL_Texture *txr_plus_cu = SDL_CreateTextureFromSurface(renderer, plus_cu);
    SDL_Rect rect_plus_cu = {rect_moins_cu.x+rect_moins_cu.w, rect_moins_cu.y, 20, 30};
    SDL_FreeSurface(plus_cu);
    
    /*----------------------------------*/
    /*          La Sorcière             */
    /*----------------------------------*/
    SDL_Surface *so = IMG_Load("../img/sorciere.png");
    SDL_Texture *txr_so = SDL_CreateTextureFromSurface(renderer, so);
    SDL_Rect rect_so = {LONG_WIN*2/3-100, LARG_WIN*8/20, 100, 100};
    SDL_FreeSurface(so);

    SDL_Surface *desc_so = TTF_RenderUTF8_Solid(font, "X ", blackColor);
    SDL_Texture *txr_desc_so = SDL_CreateTextureFromSurface(renderer, desc_so);
    SDL_Rect rect_desc_so = {10+rect_so.x+rect_so.w, rect_so.y+30, 40, 40};
    SDL_FreeSurface(desc_so);

    SDL_Surface * temp_so = TTF_RenderUTF8_Solid(font, charSorciere, blackColor);
    SDL_Texture * txr_temp_so = SDL_CreateTextureFromSurface(renderer, temp_so);
    SDL_Rect rect_temp_so = {rect_desc_so.x+rect_desc_so.w, rect_desc_so.y, 30, 40};
    SDL_FreeSurface(temp_so);

    SDL_Surface * moins_so = TTF_RenderUTF8_Solid(font, " - ", blackColor);
    SDL_Texture *txr_moins_so = SDL_CreateTextureFromSurface(renderer, moins_so);
    SDL_Rect rect_moins_so = {rect_temp_so.x+rect_temp_so.w+10, rect_temp_so.y, 20, 30};
    SDL_FreeSurface(moins_so);

    SDL_Surface * plus_so = TTF_RenderUTF8_Solid(font, " + ", blackColor);
    SDL_Texture *txr_plus_so = SDL_CreateTextureFromSurface(renderer, plus_so);
    SDL_Rect rect_plus_so = {rect_moins_so.x+rect_moins_so.w, rect_moins_so.y, 20, 30};
    SDL_FreeSurface(plus_so);

    /*----------------------------------*/
    /*        La petite fille           */
    /*----------------------------------*/
    SDL_Surface *pf = IMG_Load("../img/petite-fille.png");
    SDL_Texture *txr_pf = SDL_CreateTextureFromSurface(renderer, pf);
    SDL_Rect rect_pf = {LONG_WIN*2/3-100, LARG_WIN*11/20, 100, 100};
    SDL_FreeSurface(pf);

    SDL_Surface *desc_pf = TTF_RenderUTF8_Solid(font, "X ", blackColor);
    SDL_Texture *txr_desc_pf = SDL_CreateTextureFromSurface(renderer, desc_pf);
    SDL_Rect rect_desc_pf = {10+rect_pf.x+rect_pf.w, rect_pf.y+30, 40, 40};
    SDL_FreeSurface(desc_pf);

    SDL_Surface * temp_pf = TTF_RenderUTF8_Solid(font, charPetiteFille, blackColor);
    SDL_Texture * txr_temp_pf = SDL_CreateTextureFromSurface(renderer, temp_pf);
    SDL_Rect rect_temp_pf = {rect_desc_pf.x+rect_desc_pf.w, rect_desc_pf.y, 30, 40};
    SDL_FreeSurface(temp_pf);

    SDL_Surface * moins_pf = TTF_RenderUTF8_Solid(font, " - ", blackColor);
    SDL_Texture *txr_moins_pf = SDL_CreateTextureFromSurface(renderer, moins_pf);
    SDL_Rect rect_moins_pf = {rect_temp_pf.x+rect_temp_pf.w+10, rect_temp_pf.y, 20, 30};
    SDL_FreeSurface(moins_pf);

    SDL_Surface * plus_pf = TTF_RenderUTF8_Solid(font, " + ", blackColor);
    SDL_Texture *txr_plus_pf = SDL_CreateTextureFromSurface(renderer, plus_pf);
    SDL_Rect rect_plus_pf = {rect_moins_pf.x+rect_moins_pf.w, rect_moins_pf.y, 20, 30};
    SDL_FreeSurface(plus_pf);

    /*----------------------------------*/
    /*          Le Voleur               */
    /*----------------------------------*/
    SDL_Surface *vo = IMG_Load("../img/voleur.png");
    SDL_Texture *txr_vo = SDL_CreateTextureFromSurface(renderer, vo);
    SDL_Rect rect_vo = {LONG_WIN*2/3-100, LARG_WIN*14/20, 100, 100};
    SDL_FreeSurface(vo);

    SDL_Surface *desc_vo = TTF_RenderUTF8_Solid(font, "X ", blackColor);
    SDL_Texture *txr_desc_vo = SDL_CreateTextureFromSurface(renderer, desc_vo);
    SDL_Rect rect_desc_vo = {10+rect_vo.x+rect_vo.w, rect_vo.y+30, 40, 40};
    SDL_FreeSurface(desc_vo);

    SDL_Surface * temp_vo = TTF_RenderUTF8_Solid(font, charVoleur, blackColor);
    SDL_Texture * txr_temp_vo = SDL_CreateTextureFromSurface(renderer, temp_vo);
    SDL_Rect rect_temp_vo = {rect_desc_vo.x+rect_desc_vo.w, rect_desc_vo.y, 30, 40};
    SDL_FreeSurface(temp_vo);

    SDL_Surface * moins_vo = TTF_RenderUTF8_Solid(font, " - ", blackColor);
    SDL_Texture *txr_moins_vo = SDL_CreateTextureFromSurface(renderer, moins_vo);
    SDL_Rect rect_moins_vo = {rect_temp_vo.x+rect_temp_vo.w+10, rect_temp_vo.y, 20, 30};
    SDL_FreeSurface(moins_vo);

    SDL_Surface * plus_vo = TTF_RenderUTF8_Solid(font, " + ", blackColor);
    SDL_Texture *txr_plus_vo = SDL_CreateTextureFromSurface(renderer, plus_vo);
    SDL_Rect rect_plus_vo = {rect_moins_vo.x+rect_moins_vo.w, rect_moins_vo.y, 20, 30};
    SDL_FreeSurface(plus_vo);
    

    SDL_SetRenderDrawColor(renderer,149,56,58,255);
    int run=1;
    int maj=1;
    int x, y;
    while(run){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    run = 0;
                    maj=0;
                    break;
                case SDL_MOUSEBUTTONUP:
                    SDL_GetMouseState(&x, &y);
                    if (x >= rect_back.x && x <= rect_back.x + rect_back.w && y >= rect_back.y && y <= rect_back.y + rect_back.h){
                        run = 0;
                        maj=0;
                        if(jeu!=NULL){
                            free(jeu);
                            jeu = NULL;
                        }
                    }
                    else if(x >= rect_continuer.x && x <= rect_continuer.x + rect_continuer.w && y >= rect_continuer.y && y <= rect_continuer.y + rect_continuer.h){
                        if(joueursEgalRoles(jeu)){
                            run=0;
                            maj=0;
                        }
                        else{
                            printf("Validation impossible : nbJoueur != nbCartes.\n");
                        }
                    }
                    /* boutons nbJoueur */
                    else if (x >= rect_moins_joueur.x && x <= rect_moins_joueur.x + rect_moins_joueur.w && y >= rect_moins_joueur.y && y <= rect_moins_joueur.y + rect_moins_joueur.h){
                        if(jeu->nbJoueur>6){
                            jeu->nbJoueur--;
                            if(jeu->nbJoueur/2 < jeu->nbLoupGarou){
                                jeu->nbLoupGarou--;
                                itoa(jeu->nbLoupGarou,charLoupGarou);
                                SDL_DestroyTexture(txr_temp_lg);
                                SDL_Surface * temp_lg = TTF_RenderUTF8_Solid(font, charLoupGarou, blackColor);
                                txr_temp_lg = SDL_CreateTextureFromSurface(renderer, temp_lg);
                                SDL_FreeSurface(temp_lg);
                            }
                            if(jeu->nbJoueur < jeu->nbVillageois){
                                jeu->nbVillageois--;
                                itoa(jeu->nbVillageois,charVillageois);
                                SDL_DestroyTexture(txr_temp_vil);
                                SDL_Surface * temp_vil = TTF_RenderUTF8_Solid(font, charVillageois, blackColor);
                                txr_temp_vil = SDL_CreateTextureFromSurface(renderer, temp_vil);
                                SDL_FreeSurface(temp_vil);
                            }
                        }
                        itoa(jeu->nbJoueur,charJoueur);
                        SDL_DestroyTexture(txr_temp_joueur);
                        SDL_Surface * temp_joueur = TTF_RenderUTF8_Solid(font, charJoueur, blackColor);
                        txr_temp_joueur = SDL_CreateTextureFromSurface(renderer, temp_joueur);
                        SDL_FreeSurface(temp_joueur);
                    } 
                    else if (x >= rect_plus_joueur.x && x <= rect_plus_joueur.x + rect_plus_joueur.w && y >= rect_plus_joueur.y && y <= rect_plus_joueur.y + rect_plus_joueur.h){
                        if(jeu->nbJoueur<24){
                            jeu->nbJoueur++;
                        }
                        itoa(jeu->nbJoueur,charJoueur);
                        SDL_DestroyTexture(txr_temp_joueur);
                        SDL_Surface * temp_joueur = TTF_RenderText_Solid(font, charJoueur, blackColor);
                        txr_temp_joueur = SDL_CreateTextureFromSurface(renderer, temp_joueur);
                        SDL_FreeSurface(temp_joueur);
                    } 
                    /* boutons nbLoupGarou */
                    else if (x >= rect_moins_lg.x && x <= rect_moins_lg.x + rect_moins_lg.w && y >= rect_moins_lg.y && y <= rect_moins_lg.y + rect_moins_lg.h){
                        if(jeu->nbLoupGarou>1){
                            jeu->nbLoupGarou--;
                        }
                        itoa(jeu->nbLoupGarou,charLoupGarou);
                        SDL_DestroyTexture(txr_temp_lg);
                        SDL_Surface * temp_lg = TTF_RenderUTF8_Solid(font, charLoupGarou, blackColor);
                        txr_temp_lg = SDL_CreateTextureFromSurface(renderer, temp_lg);
                        SDL_FreeSurface(temp_lg);
                    } 
                    else if (x >= rect_plus_lg.x && x <= rect_plus_lg.x + rect_plus_lg.w && y >= rect_plus_lg.y && y <= rect_plus_lg.y + rect_plus_lg.h){
                        jeu->nbLoupGarou=atoi(charLoupGarou);
                        if(jeu->nbLoupGarou<jeu->nbJoueur){///2){
                            jeu->nbLoupGarou++;
                        }
                        itoa(jeu->nbLoupGarou,charLoupGarou);
                        SDL_DestroyTexture(txr_temp_lg);
                        SDL_Surface * temp_lg = TTF_RenderText_Solid(font, charLoupGarou, blackColor);
                        txr_temp_lg = SDL_CreateTextureFromSurface(renderer, temp_lg);
                        SDL_FreeSurface(temp_lg);
                    } 
                    /* boutons nbVillageois */
                    else if (x >= rect_moins_vil.x && x <= rect_moins_vil.x + rect_moins_vil.w && y >= rect_moins_vil.y && y <= rect_moins_vil.y + rect_moins_vil.h){
                        if(jeu->nbVillageois>1){
                            jeu->nbVillageois--;
                        }
                        itoa(jeu->nbVillageois,charVillageois);
                        SDL_DestroyTexture(txr_temp_vil);
                        SDL_Surface * temp_vil = TTF_RenderUTF8_Solid(font, charVillageois, blackColor);
                        txr_temp_vil = SDL_CreateTextureFromSurface(renderer, temp_vil);
                        SDL_FreeSurface(temp_vil);
                    } 
                    else if (x >= rect_plus_vil.x && x <= rect_plus_vil.x + rect_plus_vil.w && y >= rect_plus_vil.y && y <= rect_plus_vil.y + rect_plus_vil.h){
                        jeu->nbVillageois=atoi(charVillageois);
                        if(jeu->nbVillageois<jeu->nbJoueur){
                            jeu->nbVillageois++;
                        }
                        itoa(jeu->nbVillageois,charVillageois);
                        SDL_DestroyTexture(txr_temp_vil);
                        SDL_Surface * temp_vil = TTF_RenderText_Solid(font, charVillageois, blackColor);
                        txr_temp_vil = SDL_CreateTextureFromSurface(renderer, temp_vil);
                        SDL_FreeSurface(temp_vil);
                    } 
                    /* boutons nbChasseur */
                    else if (x >= rect_moins_ch.x && x <= rect_moins_ch.x + rect_moins_ch.w && y >= rect_moins_ch.y && y <= rect_moins_ch.y + rect_moins_ch.h){
                        if(jeu->nbChasseur>0){
                            jeu->nbChasseur--;
                        }
                        itoa(jeu->nbChasseur,charChasseur);
                        SDL_DestroyTexture(txr_temp_ch);
                        SDL_Surface * temp_ch = TTF_RenderUTF8_Solid(font, charChasseur, blackColor);
                        txr_temp_ch = SDL_CreateTextureFromSurface(renderer, temp_ch);
                        SDL_FreeSurface(temp_ch);
                    } 
                    else if (x >= rect_plus_ch.x && x <= rect_plus_ch.x + rect_plus_ch.w && y >= rect_plus_ch.y && y <= rect_plus_ch.y + rect_plus_ch.h){
                        jeu->nbChasseur=atoi(charChasseur);
                        if(jeu->nbChasseur<1){
                            jeu->nbChasseur++;
                        }
                        itoa(jeu->nbChasseur,charChasseur);
                        SDL_DestroyTexture(txr_temp_ch);
                        SDL_Surface * temp_ch = TTF_RenderText_Solid(font, charChasseur, blackColor);
                        txr_temp_ch = SDL_CreateTextureFromSurface(renderer, temp_ch);
                        SDL_FreeSurface(temp_ch);
                    } 
                    /* boutons nbVoyante */
                    else if (x >= rect_moins_voy.x && x <= rect_moins_voy.x + rect_moins_voy.w && y >= rect_moins_voy.y && y <= rect_moins_voy.y + rect_moins_voy.h){
                        if(jeu->nbVoyante>0){
                            jeu->nbVoyante--;
                        }
                        itoa(jeu->nbVoyante,charVoyante);
                        SDL_DestroyTexture(txr_temp_voy);
                        SDL_Surface * temp_voy = TTF_RenderUTF8_Solid(font, charVoyante, blackColor);
                        txr_temp_voy = SDL_CreateTextureFromSurface(renderer, temp_voy);
                        SDL_FreeSurface(temp_voy);
                    } 
                    else if (x >= rect_plus_voy.x && x <= rect_plus_voy.x + rect_plus_voy.w && y >= rect_plus_voy.y && y <= rect_plus_voy.y + rect_plus_voy.h){
                        jeu->nbVoyante=atoi(charVoyante);
                        if(jeu->nbVoyante<1){
                            jeu->nbVoyante++;
                        }
                        itoa(jeu->nbVoyante,charVoyante);
                        SDL_DestroyTexture(txr_temp_voy);
                        SDL_Surface * temp_voy = TTF_RenderText_Solid(font, charVoyante, blackColor);
                        txr_temp_voy = SDL_CreateTextureFromSurface(renderer, temp_voy);
                        SDL_FreeSurface(temp_voy);
                    } 
                    /* boutons nbCupidon */
                    else if (x >= rect_moins_cu.x && x <= rect_moins_cu.x + rect_moins_cu.w && y >= rect_moins_cu.y && y <= rect_moins_cu.y + rect_moins_cu.h){
                        if(jeu->nbCupidon>0){
                            jeu->nbCupidon--;
                        }
                        itoa(jeu->nbCupidon,charCupidon);
                        SDL_DestroyTexture(txr_temp_cu);
                        SDL_Surface * temp_cu = TTF_RenderUTF8_Solid(font, charCupidon, blackColor);
                        txr_temp_cu = SDL_CreateTextureFromSurface(renderer, temp_cu);
                        SDL_FreeSurface(temp_cu);
                    } 
                    else if (x >= rect_plus_cu.x && x <= rect_plus_cu.x + rect_plus_cu.w && y >= rect_plus_cu.y && y <= rect_plus_cu.y + rect_plus_cu.h){
                        jeu->nbCupidon=atoi(charCupidon);
                        if(jeu->nbCupidon<1){
                            jeu->nbCupidon++;
                        }
                        itoa(jeu->nbCupidon,charCupidon);
                        SDL_DestroyTexture(txr_temp_cu);
                        SDL_Surface * temp_cu = TTF_RenderText_Solid(font, charCupidon, blackColor);
                        txr_temp_cu = SDL_CreateTextureFromSurface(renderer, temp_cu);
                        SDL_FreeSurface(temp_cu);
                    } 
                    /* boutons nbSorciere */
                    else if (x >= rect_moins_so.x && x <= rect_moins_so.x + rect_moins_so.w && y >= rect_moins_so.y && y <= rect_moins_so.y + rect_moins_so.h){
                        if(jeu->nbSorciere>0){
                            jeu->nbSorciere--;
                        }
                        itoa(jeu->nbSorciere,charSorciere);
                        SDL_DestroyTexture(txr_temp_so);
                        SDL_Surface * temp_so = TTF_RenderUTF8_Solid(font, charSorciere, blackColor);
                        txr_temp_so = SDL_CreateTextureFromSurface(renderer, temp_so);
                        SDL_FreeSurface(temp_so);
                    } 
                    else if (x >= rect_plus_so.x && x <= rect_plus_so.x + rect_plus_so.w && y >= rect_plus_so.y && y <= rect_plus_so.y + rect_plus_so.h){
                        jeu->nbSorciere=atoi(charSorciere);
                        if(jeu->nbSorciere<1){
                            jeu->nbSorciere++;
                        }
                        itoa(jeu->nbSorciere,charSorciere);
                        SDL_DestroyTexture(txr_temp_so);
                        SDL_Surface * temp_so = TTF_RenderText_Solid(font, charSorciere, blackColor);
                        txr_temp_so = SDL_CreateTextureFromSurface(renderer, temp_so);
                        SDL_FreeSurface(temp_so);
                    } 
                    /* boutons nbPetiteFille */
                    else if (x >= rect_moins_pf.x && x <= rect_moins_pf.x + rect_moins_pf.w && y >= rect_moins_pf.y && y <= rect_moins_pf.y + rect_moins_pf.h){
                        if(jeu->nbPetiteFille>0){
                            jeu->nbPetiteFille--;
                        }
                        itoa(jeu->nbPetiteFille,charPetiteFille);
                        SDL_DestroyTexture(txr_temp_pf);
                        SDL_Surface * temp_pf = TTF_RenderUTF8_Solid(font, charPetiteFille, blackColor);
                        txr_temp_pf = SDL_CreateTextureFromSurface(renderer, temp_pf);
                        SDL_FreeSurface(temp_pf);
                    } 
                    else if (x >= rect_plus_pf.x && x <= rect_plus_pf.x + rect_plus_pf.w && y >= rect_plus_pf.y && y <= rect_plus_pf.y + rect_plus_pf.h){
                        jeu->nbPetiteFille=atoi(charPetiteFille);
                        if(jeu->nbPetiteFille<1){
                            jeu->nbPetiteFille++;
                        }
                        itoa(jeu->nbPetiteFille,charPetiteFille);
                        SDL_DestroyTexture(txr_temp_pf);
                        SDL_Surface * temp_pf = TTF_RenderText_Solid(font, charPetiteFille, blackColor);
                        txr_temp_pf = SDL_CreateTextureFromSurface(renderer, temp_pf);
                        SDL_FreeSurface(temp_pf);
                    } 
                    /* boutons nbVoleur */
                    else if (x >= rect_moins_vo.x && x <= rect_moins_vo.x + rect_moins_vo.w && y >= rect_moins_vo.y && y <= rect_moins_vo.y + rect_moins_vo.h){
                        if(jeu->nbVoleur>0){
                            jeu->nbVoleur--;
                        }
                        itoa(jeu->nbVoleur,charVoleur);
                        SDL_DestroyTexture(txr_temp_vo);
                        SDL_Surface * temp_vo = TTF_RenderUTF8_Solid(font, charVoleur, blackColor);
                        txr_temp_vo = SDL_CreateTextureFromSurface(renderer, temp_vo);
                        SDL_FreeSurface(temp_vo);
                    } 
                    else if (x >= rect_plus_vo.x && x <= rect_plus_vo.x + rect_plus_vo.w && y >= rect_plus_vo.y && y <= rect_plus_vo.y + rect_plus_vo.h){
                        jeu->nbVoleur=atoi(charVoleur);
                        if(jeu->nbVoleur<1){
                            jeu->nbVoleur++;
                        }
                        itoa(jeu->nbVoleur, charVoleur);
                        SDL_DestroyTexture(txr_temp_vo);
                        SDL_Surface * temp_vo = TTF_RenderText_Solid(font, charVoleur, blackColor);
                        txr_temp_vo = SDL_CreateTextureFromSurface(renderer, temp_vo);
                        SDL_FreeSurface(temp_vo);
                    }
                    if(maj){
                        itoa(nbCartes(jeu),charCarte);
                        SDL_DestroyTexture(txr_temp_carte);
                        SDL_Surface * temp_carte = TTF_RenderUTF8_Solid(font, charCarte, blackColor);
                        txr_temp_carte = SDL_CreateTextureFromSurface(renderer, temp_carte);
                        SDL_FreeSurface(temp_carte);
                    }
            }
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, txr_back, NULL, &rect_back);
        SDL_RenderCopy(renderer, txr_continuer, NULL, &rect_continuer);
        SDL_RenderCopy(renderer, txr_title, NULL, &rect_title);
        SDL_RenderCopy(renderer, txr_textnbJoueur, NULL, &rect_textnbJoueur);
        SDL_RenderCopy(renderer, txr_temp_joueur, NULL, &rect_temp_joueur);
        SDL_RenderCopy(renderer, txr_textnbCarte, NULL, &rect_textnbCarte);
        SDL_RenderCopy(renderer, txr_temp_carte, NULL, &rect_temp_carte);

        SDL_RenderCopy(renderer, txr_lg, NULL, &rect_lg);
        SDL_RenderCopy(renderer, txr_desc_lg, NULL, &rect_desc_lg);
        SDL_RenderCopy(renderer, txr_temp_lg, NULL, &rect_temp_lg);

        SDL_RenderCopy(renderer, txr_vil, NULL, &rect_vil);
        SDL_RenderCopy(renderer, txr_desc_vil, NULL, &rect_desc_vil);
        SDL_RenderCopy(renderer, txr_temp_vil, NULL, &rect_temp_vil);

        SDL_RenderCopy(renderer, txr_ch, NULL, &rect_ch);
        SDL_RenderCopy(renderer, txr_desc_ch, NULL, &rect_desc_ch);
        SDL_RenderCopy(renderer, txr_temp_ch, NULL, &rect_temp_ch);

        SDL_RenderCopy(renderer, txr_voy, NULL, &rect_voy);
        SDL_RenderCopy(renderer, txr_desc_voy, NULL, &rect_desc_voy);
        SDL_RenderCopy(renderer, txr_temp_voy, NULL, &rect_temp_voy);

        SDL_RenderCopy(renderer, txr_cu, NULL, &rect_cu);
        SDL_RenderCopy(renderer, txr_desc_cu, NULL, &rect_desc_cu);
        SDL_RenderCopy(renderer, txr_temp_cu, NULL, &rect_temp_cu);

        SDL_RenderCopy(renderer, txr_so, NULL, &rect_so);
        SDL_RenderCopy(renderer, txr_desc_so, NULL, &rect_desc_so);
        SDL_RenderCopy(renderer, txr_temp_so, NULL, &rect_temp_so);

        SDL_RenderCopy(renderer, txr_pf, NULL, &rect_pf);
        SDL_RenderCopy(renderer, txr_desc_pf, NULL, &rect_desc_pf);
        SDL_RenderCopy(renderer, txr_temp_pf, NULL, &rect_temp_pf);

        SDL_RenderCopy(renderer, txr_vo, NULL, &rect_vo);
        SDL_RenderCopy(renderer, txr_desc_vo, NULL, &rect_desc_vo);
        SDL_RenderCopy(renderer, txr_temp_vo, NULL, &rect_temp_vo);

        SDL_SetRenderDrawColor(renderer, 139, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect_plus_joueur);
        SDL_RenderFillRect(renderer, &rect_moins_joueur);
        SDL_RenderFillRect(renderer, &rect_plus_lg);
        SDL_RenderFillRect(renderer, &rect_moins_lg);
        SDL_RenderFillRect(renderer, &rect_plus_vil);
        SDL_RenderFillRect(renderer, &rect_moins_vil);
        SDL_RenderFillRect(renderer, &rect_plus_ch);
        SDL_RenderFillRect(renderer, &rect_moins_ch);
        SDL_RenderFillRect(renderer, &rect_plus_voy);
        SDL_RenderFillRect(renderer, &rect_moins_voy);
        SDL_RenderFillRect(renderer, &rect_plus_cu);
        SDL_RenderFillRect(renderer, &rect_moins_cu);
        SDL_RenderFillRect(renderer, &rect_plus_so);
        SDL_RenderFillRect(renderer, &rect_moins_so);
        SDL_RenderFillRect(renderer, &rect_plus_pf);
        SDL_RenderFillRect(renderer, &rect_moins_pf);
        SDL_RenderFillRect(renderer, &rect_plus_vo);
        SDL_RenderFillRect(renderer, &rect_moins_vo);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &rect_plus_joueur);
        SDL_RenderDrawRect(renderer, &rect_moins_joueur);
        SDL_RenderDrawRect(renderer, &rect_plus_lg);
        SDL_RenderDrawRect(renderer, &rect_moins_lg);
        SDL_RenderDrawRect(renderer, &rect_plus_vil);
        SDL_RenderDrawRect(renderer, &rect_moins_vil);
        SDL_RenderDrawRect(renderer, &rect_plus_ch);
        SDL_RenderDrawRect(renderer, &rect_moins_ch);
        SDL_RenderDrawRect(renderer, &rect_plus_voy);
        SDL_RenderDrawRect(renderer, &rect_moins_voy);
        SDL_RenderDrawRect(renderer, &rect_plus_cu);
        SDL_RenderDrawRect(renderer, &rect_moins_cu);
        SDL_RenderDrawRect(renderer, &rect_plus_so);
        SDL_RenderDrawRect(renderer, &rect_moins_so);
        SDL_RenderDrawRect(renderer, &rect_plus_pf);
        SDL_RenderDrawRect(renderer, &rect_moins_pf);
        SDL_RenderDrawRect(renderer, &rect_plus_vo);
        SDL_RenderDrawRect(renderer, &rect_moins_vo);
        SDL_SetRenderDrawColor(renderer, 149, 56, 58, 255);
        SDL_RenderCopy(renderer, txr_plus_joueur, NULL, &rect_plus_joueur);
        SDL_RenderCopy(renderer, txr_moins_joueur, NULL, &rect_moins_joueur);
        SDL_RenderCopy(renderer, txr_plus_lg, NULL, &rect_plus_lg);
        SDL_RenderCopy(renderer, txr_moins_lg, NULL, &rect_moins_lg);
        SDL_RenderCopy(renderer, txr_plus_lg, NULL, &rect_plus_vil);
        SDL_RenderCopy(renderer, txr_moins_lg, NULL, &rect_moins_vil);
        SDL_RenderCopy(renderer, txr_plus_lg, NULL, &rect_plus_ch);
        SDL_RenderCopy(renderer, txr_moins_lg, NULL, &rect_moins_ch);
        SDL_RenderCopy(renderer, txr_plus_lg, NULL, &rect_plus_voy);
        SDL_RenderCopy(renderer, txr_moins_lg, NULL, &rect_moins_voy);
        SDL_RenderCopy(renderer, txr_plus_lg, NULL, &rect_plus_cu);
        SDL_RenderCopy(renderer, txr_moins_lg, NULL, &rect_moins_cu);
        SDL_RenderCopy(renderer, txr_plus_lg, NULL, &rect_plus_so);
        SDL_RenderCopy(renderer, txr_moins_lg, NULL, &rect_moins_so);
        SDL_RenderCopy(renderer, txr_plus_lg, NULL, &rect_plus_pf);
        SDL_RenderCopy(renderer, txr_moins_lg, NULL, &rect_moins_pf);
        SDL_RenderCopy(renderer, txr_plus_lg, NULL, &rect_plus_vo);
        SDL_RenderCopy(renderer, txr_moins_lg, NULL, &rect_moins_vo);
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyTexture(txr_back);
    SDL_DestroyTexture(txr_textnbJoueur);
    SDL_DestroyTexture(txr_temp_joueur);
    SDL_DestroyTexture(txr_textnbCarte);
    SDL_DestroyTexture(txr_temp_carte);
    SDL_DestroyTexture(txr_continuer);
    SDL_DestroyTexture(txr_title);
    SDL_DestroyTexture(txr_moins_joueur);
    SDL_DestroyTexture(txr_plus_joueur);

    SDL_DestroyTexture(txr_lg);
    SDL_DestroyTexture(txr_desc_lg);
    SDL_DestroyTexture(txr_temp_lg);
    SDL_DestroyTexture(txr_moins_lg);
    SDL_DestroyTexture(txr_plus_lg);

    SDL_DestroyTexture(txr_vil);
    SDL_DestroyTexture(txr_desc_vil);
    SDL_DestroyTexture(txr_temp_vil);
    SDL_DestroyTexture(txr_moins_vil);
    SDL_DestroyTexture(txr_plus_vil);

    SDL_DestroyTexture(txr_ch);
    SDL_DestroyTexture(txr_desc_ch);
    SDL_DestroyTexture(txr_temp_ch);
    SDL_DestroyTexture(txr_moins_ch);
    SDL_DestroyTexture(txr_plus_ch);

    SDL_DestroyTexture(txr_voy);
    SDL_DestroyTexture(txr_desc_voy);
    SDL_DestroyTexture(txr_temp_voy);
    SDL_DestroyTexture(txr_moins_voy);
    SDL_DestroyTexture(txr_plus_voy);

    SDL_DestroyTexture(txr_cu);
    SDL_DestroyTexture(txr_desc_cu);
    SDL_DestroyTexture(txr_temp_cu);
    SDL_DestroyTexture(txr_moins_cu);
    SDL_DestroyTexture(txr_plus_cu);

    SDL_DestroyTexture(txr_so);
    SDL_DestroyTexture(txr_desc_so);
    SDL_DestroyTexture(txr_temp_so);
    SDL_DestroyTexture(txr_moins_so);
    SDL_DestroyTexture(txr_plus_so);

    SDL_DestroyTexture(txr_pf);
    SDL_DestroyTexture(txr_desc_pf);
    SDL_DestroyTexture(txr_temp_pf);
    SDL_DestroyTexture(txr_moins_pf);
    SDL_DestroyTexture(txr_plus_pf);

    SDL_DestroyTexture(txr_vo);
    SDL_DestroyTexture(txr_desc_vo);
    SDL_DestroyTexture(txr_temp_vo);
    SDL_DestroyTexture(txr_moins_vo);
    SDL_DestroyTexture(txr_plus_vo);

    return jeu;
}

jeu_t* menu(SDL_Renderer* renderer, TTF_Font* font){
    jeu_t* retour = NULL;

    SDL_Surface *bg = IMG_Load("../img/bg.jpg");
    SDL_Texture *txr_bg = SDL_CreateTextureFromSurface(renderer, bg);
    SDL_FreeSurface(bg);

    SDL_Surface *local = TTF_RenderUTF8_Solid(font, "Local", grayColor);
    SDL_Texture *txr_local = SDL_CreateTextureFromSurface(renderer, local);
    Button local_button = {LONG_WIN*3/7, LARG_WIN*3/5, 100, 75};
    SDL_Rect rect_local = {local_button.x, local_button.y, local_button.longueur, local_button.largeur};
    SDL_FreeSurface(local);

    SDL_Surface *online = TTF_RenderUTF8_Solid(font, "En ligne", grayColor);
    SDL_Texture *txr_online = SDL_CreateTextureFromSurface(renderer, online);
    Button online_button = {LONG_WIN*4/7, LARG_WIN*3/5, 150, 75};
    SDL_Rect rect_online = {online_button.x-online_button.longueur/6, online_button.y, online_button.longueur, online_button.largeur};
    SDL_FreeSurface(online);

    SDL_Surface *rules = TTF_RenderUTF8_Solid(font, "Règles", grayColor);
    SDL_Texture *txr_rules = SDL_CreateTextureFromSurface(renderer, rules);
    Button rules_button = {LONG_WIN*5/7, LARG_WIN*3/5, 100, 75};
    SDL_Rect rect_rules = {rules_button.x, rules_button.y, rules_button.longueur, rules_button.largeur};
    SDL_FreeSurface(rules);
    
    SDL_Surface *quit = TTF_RenderUTF8_Solid(font, "Quitter", grayColor);
    SDL_Texture *txr_quit = SDL_CreateTextureFromSurface(renderer, quit);
    Button quit_button = {LONG_WIN*4/7, LARG_WIN*4/5, 125, 75};
    SDL_Rect rect_quit = {quit_button.x, quit_button.y, quit_button.longueur, quit_button.largeur};
    SDL_FreeSurface(quit);

    int x,y;
    int run=1;
    while(run){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch (event.type){
            case SDL_QUIT:
                run=0;
                break;
            
            case SDL_MOUSEBUTTONUP:
                SDL_GetMouseState(&x,&y);
                if(x>=quit_button.x && x<=quit_button.x+quit_button.longueur && y>=quit_button.y && y<=quit_button.y+quit_button.largeur){
                    run=0;
                }
                else if(x>=local_button.x && x<=local_button.x+local_button.longueur && y>=local_button.y && y<=local_button.y+local_button.largeur){
                    retour = local_menu(renderer, font);
                    if(retour != NULL){
                        run=0;
                    }
                }
                else if(x>=online_button.x && x<=online_button.x+online_button.longueur && y>=online_button.y && y<=online_button.y+online_button.largeur){
                    online_menu(renderer, font);
                }
                else if(x>=rules_button.x && x<=rules_button.x+rules_button.longueur && y>=rules_button.y && y<=rules_button.y+rules_button.largeur){
                    rules_menu(renderer, font);
                }
                break;
            
            default: break;
            }
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, txr_bg, NULL, NULL);
        SDL_RenderCopy(renderer, txr_local, NULL, &rect_local);
        SDL_RenderCopy(renderer, txr_online, NULL, &rect_online);
        SDL_RenderCopy(renderer, txr_rules, NULL, &rect_rules);
        SDL_RenderCopy(renderer, txr_quit, NULL, &rect_quit);
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyTexture(txr_bg);
    SDL_DestroyTexture(txr_local);
    SDL_DestroyTexture(txr_online);
    SDL_DestroyTexture(txr_rules);
    SDL_DestroyTexture(txr_quit);
    return retour;
}