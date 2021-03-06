#include <SDL2/SDL.h>

#include "game.h"

static SDL_Color text_color = { 255, 255, 255, 255 };
static SDL_Color text_color_highlighted = { 255, 0, 30, 255 };
static struct menu main_menu[] = {
    { "Spielen", MENU_STATE_PLAY },
    { "Highscore", MENU_STATE_HIGHSCORE },
    { "Optionen", MENU_STATE_OPTIONS }
};
static struct menu option_menu[] = {
    { "640x480", MENU_STATE_VGA },
    { "800x600", MENU_STATE_SVGA },
    { "1024x768", MENU_STATE_XGA },
    { "1280x1024", MENU_STATE_SXGA },
    { "Vollbild", MENU_STATE_FULL }
};

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

int view_reinit(struct game_state *gs)
{
    SDL_DestroyWindow(gs->window);
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
    printf("Reset display settings.\n");
    return 0;
}
    

void view_update_mainmenu(struct game_state *gs)
{
    unsigned int i;
    SDL_Rect pos;
    SDL_Surface *text;
    memset(&pos, 0, sizeof(pos));
    // Hintergrundszene malen
    view_update_playing(gs);
    for (i = 0; i < 3; i++) {
        text = TTF_RenderText_Solid(gs->fonts[FONT_MENU], main_menu[i].label, \
            ((gs->menu_state == main_menu[i].state) ? text_color_highlighted : text_color));
        if (!text) {
            continue;
        }
        pos.x = (gs->resolution[0] - text->w) / 2;
        pos.y = gs->resolution[1] * (i + 1) / 4 - text->h;
        SDL_BlitSurface(text, 0, gs->screen, &pos);
        SDL_FreeSurface(text);
    }
    // Buffer vertauschen
    SDL_UpdateWindowSurface(gs->window);
}

void view_update_optionmenu(struct game_state *gs)
{
    unsigned int i;
    SDL_Rect pos;
    SDL_Surface *text;
    memset(&pos, 0, sizeof(pos));
    // Hintergrund malen TODO: Bild
    SDL_FillRect(gs->screen, 0, SDL_MapRGB(gs->screen->format, 0, 0, 0));
    for (i = 0; i < 5; i++) {
        text = TTF_RenderText_Solid(gs->fonts[FONT_MENU], option_menu[i].label, \
            ((gs->menu_state == option_menu[i].state) ? text_color_highlighted : text_color));
        if (!text) {
            continue;
        }
        pos.x = (gs->resolution[0] - text->w) / 2;
        pos.y = gs->resolution[1] * (i + 1) / 6 - text->h;
        SDL_BlitSurface(text, 0, gs->screen, &pos);
        SDL_FreeSurface(text);
    }
    // Buffer vertauschen
    SDL_UpdateWindowSurface(gs->window);
}

void view_update_highscore(struct game_state *gs)
{
    unsigned int i;
    char str[11]; // 10 Stellen für Ziffern, 1 Nullbyte
    SDL_Rect pos;
    SDL_Surface *text;
    memset(&pos, 0, sizeof(pos));
    // Hintergrund malen TODO: Bild
    SDL_FillRect(gs->screen, 0, SDL_MapRGB(gs->screen->format, 0, 0, 0));
    for (i = 0; i < HIGHSCORE_ENTRIES; i++) {
        text = TTF_RenderText_Solid(gs->fonts[FONT_MENU], gs->highscore[i].name, text_color);
        if (!text) {
            continue;
        }
        pos.x = gs->resolution[0] / 2 - text->w - 10;
        pos.y = gs->resolution[1] * (i + 1) / (HIGHSCORE_ENTRIES + 1) - text->h;
        SDL_BlitSurface(text, 0, gs->screen, &pos);
        SDL_FreeSurface(text);
        sprintf(str, "%u", gs->highscore[i].score);
        text = TTF_RenderText_Solid(gs->fonts[FONT_MENU], str, text_color);
        if (!text) {
            continue;
        }
        pos.x = gs->resolution[0] / 2 + 10;
        // evtl. könnte man sich diese Berechnung sparen, aber ich weiß nicht, ob SDL_BlitSurface Nebenwirkungen hat.
        pos.y = gs->resolution[1] * (i + 1) / (HIGHSCORE_ENTRIES + 1) - text->h;
        SDL_BlitSurface(text, 0, gs->screen, &pos);
        SDL_FreeSurface(text);
    }
    // Buffer vertauschen
    SDL_UpdateWindowSurface(gs->window);
}

void view_update_playing(struct game_state *gs)
{
    struct brick *b;
    char str[11]; // 10 Stellen für Ziffern, 1 Nullbyte
    SDL_Rect pos;
    SDL_Surface *text;
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
    // Bricks malen
    for (b = gs->brick_list; b; b = b->next) {
        pos.x = b->position[0] * gs->resolution[0] - gs->images[b->type + b->health - 1]->w / 2;
        pos.y = b->position[1] * gs->resolution[1] - gs->images[b->type + b->health - 1]->h / 2;
        SDL_BlitSurface(gs->images[b->type + b->health - 1], 0, gs->screen, &pos);
    }
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
    // Buffer vertauschen FIXME: Hässlich, weil es auf gs->state zugreift, aber ansonsten flackert das Bild.
    (gs->state == MODEL_STATE_PLAYING) ? SDL_UpdateWindowSurface(gs->window) : 0;
}

void view_destroy(struct game_state *gs)
{
    SDL_DestroyWindow(gs->window);
    image_destroy(gs);
    font_destroy(gs);
    SDL_Quit();
}
