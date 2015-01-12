#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "game.h"

struct game_state;

void controller_cycle(struct game_state *gs);

#endif
