#include <string.h>

#include "game.h"

static struct game_state the_game;

int main(void)
{
    memset(&the_game, 0, sizeof(the_game));
    if (view_init(&the_game) < 0) {
        return -1;
    }
    view_destroy(&the_game);
    return 0;
}
