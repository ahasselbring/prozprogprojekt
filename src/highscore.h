#ifndef _HIGHSCORE_H_
#define _HIGHSCORE_H_

#define HIGHSCORE_FILENAME ".pongscore"
#define HIGHSCORE_ENTRIES 10
#define HIGHSCORE_NAMELENGTH 50
#define HIGHSCORE_NAMELENGTH_STR "50"

#include "game.h"

struct highscore_entry {
    char name[HIGHSCORE_NAMELENGTH];
    unsigned int score;
};

struct game_state;

void highscore_read(struct game_state *gs);

void highscore_add(struct game_state *gs, char *name, unsigned int score);

int highscore_write(struct game_state *gs);


#endif
