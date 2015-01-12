#ifndef _CONFIG_H_
#define _CONFIG_H_

#define CONFIG_FILENAME ".ponging"

#include "game.h"

struct game_state;

void config_read(struct game_state *gs);

int config_write(struct game_state *gs);

#endif
