#include <sys/time.h>

#include "game.h"

static void model_mainmenu(struct game_state *gs)
{
    (void)gs;
    // TODO: bei KEY_ENTER aktuellen Menupunkt auswählen
    // TODO: bei KEY_DOWN einen Menupunkt weiter
    // TODO: bei KEY_UP einen Menupunkt zurück
    gs->state = MODEL_STATE_START;
}

static void model_optionmenu(struct game_state *gs)
{
    (void)gs;
}

static void model_highscore(struct game_state *gs)
{
    (void)gs;
}

static void model_start(struct game_state *gs)
{
    gs->position[0] = gs->position[1] = 0.5;
    gs->speed[0] = gs->speed[1] = 0.5;
    gs->score[0] = gs->score[1] = 0;
    gs->name[0] = "foo";
    gs->name[1] = "bar";
    gs->ball_position[0] = gs->ball_position[1] = 0.5;
    gs->ball_speed[0] = gs->ball_speed[1] = 0.2 * 1.414;
    gs->state = MODEL_STATE_PLAYING;
    gettimeofday(&(gs->last_time), 0);
}

static void model_playing(struct game_state *gs)
{
    double dt;
    struct timeval last_time = gs->last_time;
    gettimeofday(&(gs->last_time), 0);
    dt = (double)(gs->last_time.tv_sec - last_time.tv_sec) + (double)(gs->last_time.tv_usec - last_time.tv_usec) / 1000000;
    gs->ball_position[0] += gs->ball_speed[0] * dt;
    if ((gs->ball_position[0] >= 1) || (gs->ball_position[0] <= 0)) {
        if (gs->ball_position[0] >= 1) {
            gs->score[0]++;
        } else {
            gs->score[1]++;
        }
        gs->ball_position[0] = 0.5;
        gs->ball_position[1] = 0.5;
    }
    gs->ball_position[1] += gs->ball_speed[1] * dt;
    if ((gs->ball_position[1] >= 1) || (gs->ball_position[1] <= 0)) {
        gs->ball_speed[1] = -gs->ball_speed[1];
    }
    if (gs->controls & CONTROL_W) {
        gs->position[0] -= gs->speed[0] * dt;
    } else if (gs->controls & CONTROL_S) {
        gs->position[0] += gs->speed[0] * dt;
    }
    if (gs->controls & CONTROL_UP) {
        gs->position[1] -= gs->speed[1] * dt;
    } else if (gs->controls & CONTROL_DOWN) {
        gs->position[1] += gs->speed[1] * dt;
    }
    // TODO: Kollisionsprüfung Paddle - Ball
}

void model_cycle(struct game_state *gs)
{
    switch (gs->state) {
        case MODEL_STATE_MAINMENU:
            model_mainmenu(gs);
            view_update_mainmenu(gs);
            break;
        case MODEL_STATE_OPTIONMENU:
            model_optionmenu(gs);
            view_update_optionmenu(gs);
            break;
        case MODEL_STATE_HIGHSCORE:
            model_highscore(gs);
            view_update_highscore(gs);
            break;
        case MODEL_STATE_START:
            model_start(gs);
            break;
        case MODEL_STATE_PLAYING:
            model_playing(gs);
            view_update_playing(gs);
            break;
        case MODEL_STATE_END:
            break;
    }
}

int model_init(struct game_state *gs)
{
    gs->state = MODEL_STATE_MAINMENU;
    return 0;
}

void model_destroy(struct game_state *gs)
{
    (void)gs;
}
