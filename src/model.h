#ifndef _MODEL_H_
#define _MODEL_H_

#include "game.h"

struct game_state;

enum model_state {
    MODEL_STATE_MAINMENU,
    MODEL_STATE_OPTIONMENU,
    MODEL_STATE_HIGHSCORE,
    MODEL_STATE_START,
    MODEL_STATE_PLAYING,
    MODEL_STATE_END
};

int model_init(struct game_state *gs);

void model_cycle(struct game_state *gs);

void model_destroy(struct game_state *gs);

#endif
