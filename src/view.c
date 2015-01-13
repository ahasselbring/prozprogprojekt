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
    gs->window = SDL_CreateWindow("PongING", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, \
        gs->resolution[0], gs->resolution[1], (gs->fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0));
    if (!(gs->window)) {
        font_destroy(gs);
        image_destroy(gs);
        SDL_Quit();
        return -1;
    }
    gs->screen = SDL_GetWindowSurface(gs->window);
    if (gs->fullscreen) {
        gs->resolution[0] = gs->screen->w;
        gs->resolution[1] = gs->screen->h;
    }
    printf("Created view datastructures.\n");
    return 0;
}

void view_update_mainmenu(struct game_state *gs)
{
    (void)gs;
}

void view_update_optionmenu(struct game_state *gs)
{
    (void)gs;
}

void view_update_highscore(struct game_state *gs)
{
    (void)gs;
}

void view_update_playing(struct game_state *gs)
{
    char str[11]; // 10 Stellen für Ziffern, 1 Nullbyte
    SDL_Rect pos;
    SDL_Surface *text;
    SDL_Color text_color = { 255, 255, 255, 255 };
    memset(&pos, 0, sizeof(pos));
    // Hintergrund malen TODO: Bild
    SDL_FillRect(gs->screen, 0, SDL_MapRGB(gs->screen->format, 0, 0, 0));
    // Paddle links malen
    pos.x = PADDLE_DISTANCE * gs->resolution[0] - gs->images[IMAGE_PADDLE]->w / 2;
    pos.y = gs->position[0] * gs->resolution[1] - gs->images[IMAGE_PADDLE]->h / 2;
    SDL_BlitSurface(gs->images[IMAGE_PADDLE], 0, gs->screen, &pos);
    // Paddle rechts malen
    pos.x = (1 - PADDLE_DISTANCE) * gs->resolution[0] - gs->images[IMAGE_PADDLE]->w / 2;
    pos.y = gs->position[1] * gs->resolution[1] - gs->images[IMAGE_PADDLE]->h / 2;
    SDL_BlitSurface(gs->images[IMAGE_PADDLE], 0, gs->screen, &pos);
    // Ball malen
    pos.x = gs->ball_position[0] * gs->resolution[0] - gs->images[IMAGE_BALL]->w / 2;
    pos.y = gs->ball_position[1] * gs->resolution[1] - gs->images[IMAGE_BALL]->h / 2;
    SDL_BlitSurface(gs->images[IMAGE_BALL], 0, gs->screen, &pos);
    // Score links malen
    sprintf(str, "%u", gs->score[0]);
    text = TTF_RenderText_Solid(gs->fonts[FONT_PLAYING], str, text_color);
    pos.x = gs->resolution[0] / 2 - text->w - 10;
    pos.y = 10;
    SDL_BlitSurface(text, 0, gs->screen, &pos);
    SDL_FreeSurface(text);
    text = TTF_RenderText_Solid(gs->fonts[FONT_PLAYING], gs->name[0], text_color);
    pos.x = 30;
    pos.y = 10;
    SDL_BlitSurface(text, 0, gs->screen, &pos);
    SDL_FreeSurface(text);
    // Score rechts malen
    sprintf(str, "%u", gs->score[1]);
    text = TTF_RenderText_Solid(gs->fonts[FONT_PLAYING], str, text_color);
    pos.x = gs->resolution[0] / 2 + 10;
    pos.y = 10;
    SDL_BlitSurface(text, 0, gs->screen, &pos);
    SDL_FreeSurface(text);
    text = TTF_RenderText_Solid(gs->fonts[FONT_PLAYING], gs->name[1], text_color);
    pos.x = gs->resolution[0] - text->w - 30;
    pos.y = 10;
    SDL_BlitSurface(text, 0, gs->screen, &pos);
    SDL_FreeSurface(text);
    // Buffer vertauschen
    SDL_UpdateWindowSurface(gs->window);
}

void view_destroy(struct game_state *gs)
{
    SDL_DestroyWindow(gs->window);
    image_destroy(gs);
    font_destroy(gs);
    SDL_Quit();
}
