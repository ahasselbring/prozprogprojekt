#ifndef _GAME_H_
#define _GAME_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <sys/time.h>

#include "image.h"
#include "font.h"
#include "model.h"
#include "view.h"
#include "controller.h"
#include "config.h"
#include "highscore.h"

#define PLAYER_LEFT 0
#define PLAYER_RIGHT 1

#define CONTROL_UP 0x01
#define CONTROL_DOWN 0x02
#define CONTROL_W 0x04
#define CONTROL_S 0x08
#define CONTROL_ENTER 0x10
#define CONTROL_ESCAPE 0x20

#define PADDLE_HEIGHT 0.2
#define PADDLE_DISTANCE 0.02

#define CYCLE_DELAY 2

#define BRICK_TIME 5
#define BRICK_RADIUS 0.05
#define BRICK_BORDER 5

enum model_state;
enum menu_state;
struct highscore_entry;

struct brick {
    double position[2];
    unsigned int score;
    unsigned int health;
    unsigned int type;
    struct brick *next;
};

struct game_state {
    SDL_Window *window; // nur vom View genutzt
    SDL_Surface *screen; // nur vom View genutzt
    SDL_Surface *images[IMAGE_NUMBER]; // nur vom View genutzt
    TTF_Font *fonts[FONT_NUMBER]; // nur vom View genutzt
    char name[2][HIGHSCORE_NAMELENGTH]; // lesen: Model und View, schreiben: Model
    unsigned int score[2]; // lesen: Model und View, schreiben: Model
    double position[2]; // lesen: Model und View, schreiben: Model
    double speed[2]; // nur vom Model genutzt
    struct brick *brick_list; // lesen: Model und View, schreiben: Model
    unsigned char brick_player; // nur vom Model genutzt
    unsigned char brick_active; // nur vom Model genutzt
    double ball_position[2]; // lesen: Model und View, schreiben: Model
    double ball_speed[2]; // nur vom Model genutzt - 0 <=> Betrag, 1 <=> Winkel
    enum model_state state; // nur vom Model genutzt
    struct timeval last_time; // nur vom Model genutzt
    struct timeval last_brick; // nur vom Model genutzt
    unsigned char quit; // lesen: Model, schreiben: Controller
    unsigned char controls; // lesen: Model, schreiben: Controller
    enum menu_state menu_state; // lesen: View und Model, schreiben: Model
    unsigned int resolution[2]; // lesen: View und Config, schreiben: Config und Model
    unsigned char fullscreen; // lesen: View und Config, schreiben: Config und Model
    struct highscore_entry highscore[HIGHSCORE_ENTRIES];
};

#endif
