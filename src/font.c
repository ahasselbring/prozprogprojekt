#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

#include "game.h"

static const char *font_paths[FONT_NUMBER] = {
    "data/font1.ttf",
    "data/font2.ttf",
};

static int font_sizes[FONT_NUMBER] = {
    16,
    12
};


int font_load(struct game_state *gs)
{
    unsigned int i;
    if (TTF_Init() < 0) {
        printf("Could not initialize true type fonts.\n");
        return -1;
    }
    for (i = 0; i < FONT_NUMBER; i++) {
        gs->fonts[i] = TTF_OpenFont(font_paths[i], font_sizes[i]);
        if (gs->fonts[i] == NULL) {
            printf("Could not load font: %s\n", font_paths[i]);
            for (--i; i < IMAGE_NUMBER; i--) {
                TTF_CloseFont(gs->fonts[i]);
            }
            TTF_Quit();
            return -1;
        }
    }
    printf("Loaded fonts.\n");
    return 0;
}

void font_destroy(struct game_state *gs)
{
    unsigned int i;
    for (i = 0; i < FONT_NUMBER; i++) {
        TTF_CloseFont(gs->fonts[i]);
    }
    TTF_Quit();
}
