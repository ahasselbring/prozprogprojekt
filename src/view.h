#ifndef _VIEW_H_
#define _VIEW_H_

#include "game.h"

struct game_state;

int view_init(struct game_state *gs);

void view_destroy(struct game_state *gs);

#endif
