#include <SDL2/SDL.h>

#include "game.h"

int view_init(struct game_state *gs)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Could not initialize SDL.\n");
        return -1;
    }
    if (image_load(gs) < 0) {
        SDL_Quit();
        return -1;
    }
    if (font_load(gs) < 0) {
        image_destroy(gs);
        SDL_Quit();
        return -1;
    }
    printf("Created view datastructures.\n");
    return 0;
}

void view_destroy(struct game_state *gs)
{
    image_destroy(gs);
    font_destroy(gs);
    SDL_Quit();
}
