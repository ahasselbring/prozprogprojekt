#include <sys/time.h>

#include "game.h"

static void model_intro(struct game_state *gs)
{
    // Mit diesen Einstellungen bewegt sich der Ball nur in der Horizontalen.
    gs->position[0] = gs->position[1] = 0.5;
    gs->speed[0] = gs->speed[1] = 0;
    gs->score[0] = gs->score[1] = 0;
    gs->name[0] = "42";
    gs->name[1] = "e^(i*pi)+1";
    gs->ball_position[0] = gs->ball_position[1] = 0.5;
    gs->ball_speed[0] = 0.5;
    gs->ball_speed[1] = 0;
    gs->state = MODEL_STATE_PLAYING;
    gettimeofday(&(gs->last_time), 0);
    gs->state = MODEL_STATE_MAINMENU;
}

static void model_mainmenu(struct game_state *gs)
{
    // Das Delay ist dazu da, dass bei gedrückter Taste nicht jeden Cycle die Aktion wiederholt wird.
    double dt;
    struct timeval last_time = gs->last_time;
    static unsigned int delay = 0;
    if (delay) {
        delay--;
    }
    if (gs->controls & CONTROL_UP) {
        if (!delay) {
            // voriger Menupunkt
            if (gs->menu_state) {
                gs->menu_state--;
            } else {
                gs->menu_state = 2;
            }
            delay = MENU_DELAY;
        }
    } else if (gs->controls & CONTROL_DOWN) {
        if (!delay) {
            // nächster Menupunkt
            gs->menu_state++;
            gs->menu_state %= 3;
            delay = MENU_DELAY;
        }
    } else if (gs->controls & CONTROL_ENTER) {
        // aktuellen Menupunkt auswählen
        // Enter löschen, da gedrückt gehaltenes Enter keine Auswirkungen haben soll.
        gs->controls &= ~CONTROL_ENTER;
        switch (gs->menu_state) {
            case MENU_STATE_PLAY:
                gs->state = MODEL_STATE_START;
                break;
            case MENU_STATE_HIGHSCORE:
                gs->state = MODEL_STATE_HIGHSCORE;
                break;
            case MENU_STATE_OPTIONS:
                gs->state = MODEL_STATE_OPTIONMENU;
                break;
            default:
                break;
        }
        gs->menu_state = 0;
    }
    gettimeofday(&(gs->last_time), 0);
    dt = (double)(gs->last_time.tv_sec - last_time.tv_sec) \
        + (double)(gs->last_time.tv_usec - last_time.tv_usec) / 1000000;
    gs->ball_position[0] += gs->ball_speed[0] * dt;
    if ((gs->ball_position[0] <= PADDLE_DISTANCE) \
        && ((gs->position[0] - 0.2) < gs->ball_position[1]) \
        && ((gs->position[0] + 0.2) > gs->ball_position[1]))
    {
        gs->ball_speed[0] = -gs->ball_speed[0];
        gs->ball_position[0] += gs->ball_speed[0] * dt;
    } else if ((gs->ball_position[0] >= (1 - PADDLE_DISTANCE)) \
        && ((gs->position[1] - 0.2) < gs->ball_position[1]) \
        && ((gs->position[1] + 0.2) > gs->ball_position[1]))
    {
        gs->ball_speed[0] = -gs->ball_speed[0];
        gs->ball_position[0] += gs->ball_speed[0] * dt;
    }
}

static void model_optionmenu(struct game_state *gs)
{
    // Das Delay ist dazu da, dass bei gedrückter Taste nicht jeden Cycle die Aktion wiederholt wird.
    static unsigned int delay = 0;
    if (delay) {
        delay--;
    }
    if (gs->controls & CONTROL_UP) {
        if (!delay) {
            // voriger Menupunkt
            if (gs->menu_state) {
                gs->menu_state--;
            } else {
                gs->menu_state = 4;
            }
            delay = MENU_DELAY;
        }
    } else if (gs->controls & CONTROL_DOWN) {
        if (!delay) {
            // nächster Menupunkt
            gs->menu_state++;
            gs->menu_state %= 5;
            delay = MENU_DELAY;
        }
    } else if (gs->controls & CONTROL_ENTER) {
        // aktuellen Menupunkt auswählen
        // Enter löschen, da gedrückt gehaltenes Enter keine Auswirkungen haben soll.
        gs->controls &= ~CONTROL_ENTER;
        switch (gs->menu_state) {
            case MENU_STATE_VGA:
                gs->resolution[0] = 640;
                gs->resolution[1] = 480;
                gs->fullscreen = 0;
                break;
            case MENU_STATE_SVGA:
                gs->resolution[0] = 800;
                gs->resolution[1] = 600;
                gs->fullscreen = 0;
                break;
            case MENU_STATE_XGA:
                gs->resolution[0] = 1024;
                gs->resolution[1] = 768;
                gs->fullscreen = 0;
                break;
            case MENU_STATE_SXGA:
                gs->resolution[0] = 1280;
                gs->resolution[1] = 1024;
                gs->fullscreen = 0;
                break;
            case MENU_STATE_FULL:
                gs->resolution[0] = gs->resolution[1] = 0;
                gs->fullscreen = 1;
                break;
            default:
                break;
        }
        view_reinit(gs);
        gs->state = MODEL_STATE_MAINMENU;
        gs->menu_state = MENU_STATE_PLAY;
    }
}

static void model_highscore(struct game_state *gs)
{
    (void)gs;
}

static void model_start(struct game_state *gs)
{
    gs->position[0] = gs->position[1] = 0.5;
    gs->speed[0] = gs->speed[1] = 0.8;
    gs->score[0] = gs->score[1] = 0;
    // TODO: Namen einlesen
    gs->name[0] = "foo";
    gs->name[1] = "bar";
    gs->ball_position[0] = gs->ball_position[1] = 0.5;
    gs->ball_speed[0] = 0.5;
    gs->ball_speed[1] = 0.7;
    gs->state = MODEL_STATE_PLAYING;
    gettimeofday(&(gs->last_time), 0);
}

static void model_playing(struct game_state *gs)
{
    double dt;
    struct timeval last_time = gs->last_time;
    gettimeofday(&(gs->last_time), 0);
    dt = (double)(gs->last_time.tv_sec - last_time.tv_sec) \
        + (double)(gs->last_time.tv_usec - last_time.tv_usec) / 1000000;
    gs->ball_position[0] += gs->ball_speed[0] * dt;
    if ((gs->ball_position[0] <= PADDLE_DISTANCE) \
        && ((gs->position[0] - 0.2) < gs->ball_position[1]) \
        && ((gs->position[0] + 0.2) > gs->ball_position[1]))
    {
        gs->ball_speed[0] = -gs->ball_speed[0];
        gs->ball_position[0] += gs->ball_speed[0] * dt;
    } else if ((gs->ball_position[0] >= (1 - PADDLE_DISTANCE)) \
        && ((gs->position[1] - 0.2) < gs->ball_position[1]) \
        && ((gs->position[1] + 0.2) > gs->ball_position[1]))
    {
        gs->ball_speed[0] = -gs->ball_speed[0];
        gs->ball_position[0] += gs->ball_speed[0] * dt;
    }
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
        gs->ball_position[1] += gs->ball_speed[1] * dt;
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
}

void model_cycle(struct game_state *gs)
{
    switch (gs->state) {
        case MODEL_STATE_INTRO:
            model_intro(gs);
            break;
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
    gs->state = MODEL_STATE_INTRO;
    return 0;
}

void model_destroy(struct game_state *gs)
{
    (void)gs;
}
