#ifndef _GAME_H_
#define _GAME_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "image.h"
#include "font.h"
#include "view.h"

#define PLAYER_LEFT 0
#define PLAYER_RIGHT 1

struct game_state {
    SDL_Window *window;
    SDL_Surface *screen;
    SDL_Surface *images[IMAGE_NUMBER];
    TTF_Font *fonts[FONT_NUMBER];
    unsigned int position[2];
    unsigned int score[2];
    const char *name[2];
    unsigned int ball_position[2];
    int ball_speed[2];
    unsigned char quit;
};

#endif
