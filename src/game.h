#ifndef _GAME_H_
#define _GAME_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <sys/time.h>

#include "image.h"
#include "font.h"
#include "view.h"
#include "model.h"
#include "controller.h"
#include "config.h"

#define PLAYER_LEFT 0
#define PLAYER_RIGHT 1

#define CONTROL_UP 0x01
#define CONTROL_DOWN 0x02
#define CONTROL_LEFT 0x04
#define CONTROL_RIGHT 0x08
#define CONTROL_W 0x10
#define CONTROL_S 0x20
#define CONTROL_ENTER 0x40

#define PADDLE_DISTANCE 0.002

#define CYCLE_DELAY 2

enum model_state;
enum menu_state;

struct game_state {
    SDL_Window *window; // nur vom View genutzt
    SDL_Surface *screen; // nur vom View genutzt
    SDL_Surface *images[IMAGE_NUMBER]; // nur vom View genutzt
    TTF_Font *fonts[FONT_NUMBER]; // nur vom View genutzt
    const char *name[2]; // lesen: Model und View, schreiben: Model
    unsigned int score[2]; // lesen: Model und View, schreiben: Model
    double position[2]; // lesen: Model und View, schreiben: Model
    double speed[2]; // nur vom Model genutzt
    double ball_position[2]; // lesen: Model und View, schreiben: Model
    double ball_speed[2]; // nur vom Model genutzt
    enum model_state state; // nur vom Model genutzt
    struct timeval last_time; // nur vom Model genutzt
    unsigned char quit; // lesen: Model, schreiben: Controller
    unsigned char controls; // lesen: Model, schreiben: Controller
    enum menu_state menu_state; // lesen: View und Model, schreiben: Model
    unsigned int resolution[2]; // lesen: View und Config, schreiben: Config und Menu
    unsigned char fullscreen; // lesen: View und Config, schreiben: Config und Menu
};

#endif
