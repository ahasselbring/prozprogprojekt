#include <string.h>

#include "game.h"

static struct game_state the_game;

int main(void)
{
    memset(&the_game, 0, sizeof(the_game));
    config_read(&the_game);
    if (model_init(&the_game) < 0) {
        return -1;
    }
    if (view_init(&the_game) < 0) {
        model_destroy(&the_game);
        return -1;
    }
    while (!(the_game.quit)) {
        controller_cycle(&the_game);
    }
    view_destroy(&the_game);
    model_destroy(&the_game);
    config_write(&the_game);
    return 0;
}
