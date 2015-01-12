#ifndef _FONT_H_
#define _FONT_H_

#define FONT_NUMBER 1

#define FONT_PLAYING 0

#include "game.h"

struct game_state;

int font_load(struct game_state *gs);

void font_destroy(struct game_state *gs);

#endif
