#ifndef _IMAGE_H_
#define _IMAGE_H_

#define IMAGE_NUMBER 6

#define IMAGE_BALL 0
#define IMAGE_PADDLE 1
#define IMAGE_BRICK 2

#include "game.h"

struct game_state;

int image_load(struct game_state *gs);

void image_destroy(struct game_state *gs);

#endif
