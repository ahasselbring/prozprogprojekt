#ifndef _MODEL_H_
#define _MODEL_H_

#define MENU_DELAY 20

#include "game.h"

struct game_state;

enum model_state {
    MODEL_STATE_INTRO,
    MODEL_STATE_MAINMENU,
    MODEL_STATE_OPTIONMENU,
    MODEL_STATE_HIGHSCORE,
    MODEL_STATE_START,
    MODEL_STATE_PLAYING,
    MODEL_STATE_END
};

enum menu_state {
    MENU_STATE_PLAY = 0,
    MENU_STATE_HIGHSCORE = 1,
    MENU_STATE_OPTIONS = 2,
    MENU_STATE_VGA = 0,
    MENU_STATE_SVGA = 1,
    MENU_STATE_XGA = 2,
    MENU_STATE_SXGA = 3,
    MENU_STATE_FULL = 4
};

int model_init(struct game_state *gs);

void model_cycle(struct game_state *gs);

void model_destroy(struct game_state *gs);

#endif
