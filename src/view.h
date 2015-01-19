#ifndef _VIEW_H_
#define _VIEW_H_

#include "game.h"

struct game_state;

int view_init(struct game_state *gs);

int view_reinit(struct game_state *gs);

void view_update_mainmenu(struct game_state *gs);

void view_update_optionmenu(struct game_state *gs);

void view_update_highscore(struct game_state *gs);

void view_update_playing(struct game_state *gs);

void view_destroy(struct game_state *gs);

#endif
