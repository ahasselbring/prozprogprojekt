#include <SDL2/SDL.h>
#include <stdio.h>

#include "game.h"

static const char *image_paths[IMAGE_NUMBER] = {
    "data/image1.bmp",
    "data/image2.bmp",
};


int image_load(struct game_state *gs)
{
    unsigned int i;
    for (i = 0; i < IMAGE_NUMBER; i++) {
        gs->images[i] = SDL_LoadBMP(image_paths[i]);
        if (gs->images[i] == NULL) {
            printf("Could not load image: %s\n", image_paths[i]);
            for (--i; i < IMAGE_NUMBER; i--) {
                SDL_FreeSurface(gs->images[i]);
            }
            return -1;
        }
    }
    printf("Loaded images.\n");
    return 0;
}

void image_destroy(struct game_state *gs)
{
    unsigned int i;
    for (i = 0; i < IMAGE_NUMBER; i++) {
        SDL_FreeSurface(gs->images[i]);
    }
}
