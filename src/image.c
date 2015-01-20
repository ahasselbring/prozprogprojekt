#include <SDL2/SDL.h>
#include <stdio.h>

#include "game.h"

static const char *image_paths[IMAGE_NUMBER] = {
    "data/ball.bmp",
    "data/paddle.bmp",
    "data/brick1.bmp",
    "data/brick2.bmp",
    "data/brick3.bmp",
    "data/brick4.bmp"
};


int image_load(struct game_state *gs)
{
    unsigned int i,colorkey;
    for (i = 0; i < IMAGE_NUMBER; i++) {
        gs->images[i] = SDL_LoadBMP(image_paths[i]);
        if (gs->images[i] == NULL) {
            printf("Could not load image: %s\n", image_paths[i]);
            for (--i; i < IMAGE_NUMBER; i--) {
                SDL_FreeSurface(gs->images[i]);
            }
            return -1;
        }
        colorkey = SDL_MapRGB( gs->images[i]->format, 0,0,0);
        SDL_SetColorKey( gs->images[i], SDL_TRUE, colorkey);
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
