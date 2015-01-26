#include <sys/time.h>
#include <stdlib.h>
#include <math.h>

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
    gs->menu_state = MENU_STATE_PLAY;
    gettimeofday(&(gs->last_time), 0);
    gs->state = MODEL_STATE_MAINMENU;
}

static void model_mainmenu(struct game_state *gs)
{
    double dt;
    struct timeval last_time = gs->last_time;
    if (gs->controls & CONTROL_UP) {
        gs->controls &= ~CONTROL_UP;
        // voriger Menupunkt
        if (gs->menu_state) {
            gs->menu_state--;
        } else {
            gs->menu_state = 2;
        }
    } else if (gs->controls & CONTROL_DOWN) {
        gs->controls &= ~CONTROL_DOWN;
        // nächster Menupunkt
        gs->menu_state++;
        gs->menu_state %= 3;
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
                gs->menu_state = MENU_STATE_FULL;
                break;
            default:
                break;
        }
    } else if (gs->controls & CONTROL_ESCAPE) {
        gs->quit = 1;
        return;
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
    if (gs->controls & CONTROL_UP) {
        gs->controls &= ~CONTROL_UP;
        // voriger Menupunkt
        if (gs->menu_state) {
            gs->menu_state--;
        } else {
            gs->menu_state = 4;
        }
    } else if (gs->controls & CONTROL_DOWN) {
        gs->controls &= ~CONTROL_DOWN;
        // nächster Menupunkt
        gs->menu_state++;
        gs->menu_state %= 5;
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
        gs->state = MODEL_STATE_INTRO;
    } else if (gs->controls & CONTROL_ESCAPE) {
        gs->controls &= ~CONTROL_ESCAPE;
        gs->state = MODEL_STATE_INTRO;
    }
}

static void model_highscore(struct game_state *gs)
{
    if (gs->controls & CONTROL_ESCAPE) {
        gs->controls &= ~CONTROL_ESCAPE;
        gs->state = MODEL_STATE_INTRO;
    }
}

static void model_start(struct game_state *gs)
{
    struct brick *next;
    while (gs->brick_list) {
        next = gs->brick_list->next;
        free(gs->brick_list);
        gs->brick_list = next;
    }
    gs->position[0] = gs->position[1] = 0.5;
    gs->speed[0] = gs->speed[1] = 1.2;
    gs->score[0] = gs->score[1] = 0;
    // TODO: Namen einlesen
    gs->name[0] = "foo";
    gs->name[1] = "bar";
    gs->ball_position[0] = gs->ball_position[1] = 0.5;
    gs->ball_speed[0] = 1;
    gs->ball_speed[1] = ((rand() & 1) ? M_PI : 0) + M_PI * (double)(rand() % 1024) / (1023 * 4);
    gs->brick_list = 0;
    gs->brick_player = 0;
    gs->brick_active = 0;
    gs->state = MODEL_STATE_PLAYING;
    gettimeofday(&(gs->last_time), 0);
    gs->last_brick = gs->last_time;
    srand(gs->last_time.tv_sec);
}

static void model_end(struct game_state *gs)
{
    if (gs->controls & CONTROL_ENTER) {
        gs->controls &= ~CONTROL_ENTER;
        gs->state = MODEL_STATE_INTRO;
    }
}

static void model_playing(struct game_state *gs)
{
    void brick_add(struct game_state *gs)
    {
        struct brick *b;
        if (gs->last_time.tv_sec < (gs->last_brick.tv_sec + 5)) {
            return;
        }
        b = malloc(sizeof(struct brick));
        if (!b) {
            // Wenn kein Speicher mehr da ist, gibt es auch keinen weiteren Brick
            return;
        }
        b->position[0] = 4 * PADDLE_DISTANCE + (1 - 8 * PADDLE_DISTANCE) * (double)(rand() % 1024) / 1023;
        b->position[1] = (double)(rand() % 1024) / 1023;
        b->health = (rand() % 4) + 1;
        b->score = b->health * 10;
        b->type = IMAGE_BRICK;
        b->next = gs->brick_list;
        gs->brick_list = b;
        gs->last_brick = gs->last_time;
        gs->brick_active++;
    }
    void brick_collision(struct game_state *gs)
    {
        struct brick *b, *prev, *next;
        for (b = gs->brick_list, prev = 0; b; b = next) {
            next = b->next;
            if (((gs->ball_position[0] + BRICK_RADIUS) > b->position[0]) \
                && ((gs->ball_position[0] - BRICK_RADIUS) < b->position[0]) \
                && ((gs->ball_position[1] + BRICK_RADIUS) > b->position[1]) \
                && ((gs->ball_position[1] - BRICK_RADIUS) < b->position[1]))
            {
                gs->ball_speed[1] = 2 * M_PI - gs->ball_speed[1];
                if (!(--(b->health))) {
                    gs->score[gs->brick_player] += b->score;
                    if (prev) {
                        prev->next = next;
                    } else {
                        gs->brick_list = next;
                    }
                    free(b);
                    gs->brick_active--;
                }
            } else {
                prev = b;
            }
        }
    }
    double dt;
    struct timeval last_time = gs->last_time;
    gettimeofday(&(gs->last_time), 0);
    dt = (double)(gs->last_time.tv_sec - last_time.tv_sec) \
        + (double)(gs->last_time.tv_usec - last_time.tv_usec) / 1000000;
    // Wenn zu viele Bricks auf dem Feld sind, ist das Spiel zu Ende.
    if (gs->brick_active > BRICK_BORDER) {
        gs->state = MODEL_STATE_END;
    }
    // Ball in X-Richtung bewegen
    gs->ball_position[0] += gs->ball_speed[0] * cos(gs->ball_speed[1]) * dt;
    if ((gs->ball_position[0] <= PADDLE_DISTANCE) \
        && ((gs->position[0] - PADDLE_HEIGHT) < gs->ball_position[1]) \
        && ((gs->position[0] + PADDLE_HEIGHT) > gs->ball_position[1]))
    {
        gs->ball_speed[1] = -0.9 * M_PI * (gs->position[0] - gs->ball_position[1]) / (2 * PADDLE_HEIGHT);
        gs->ball_position[0] = PADDLE_DISTANCE;
        gs->brick_player = 0;
    } else if ((gs->ball_position[0] >= (1 - PADDLE_DISTANCE)) \
        && ((gs->position[1] - PADDLE_HEIGHT) < gs->ball_position[1]) \
        && ((gs->position[1] + PADDLE_HEIGHT) > gs->ball_position[1]))
    {
        gs->ball_speed[1] = M_PI * (1 + 0.9 * (gs->position[1] - gs->ball_position[1]) / (2 * PADDLE_HEIGHT));
        gs->ball_position[0] = 1 - PADDLE_DISTANCE;
        gs->brick_player = 1;
    }
    // Ball ist nach links oder rechts nicht aufgehalten worden - Punkte für den Gegner
    if ((gs->ball_position[0] >= 1) || (gs->ball_position[0] <= 0)) {
        if (gs->ball_position[0] >= 1) {
            gs->score[0] += 50;
        } else {
            gs->score[1] += 50;
        }
        // Ball in die Mitte setzen
        gs->ball_position[0] = 0.5;
        gs->ball_position[1] = 0.5;
        gs->ball_speed[0] = 1;
        gs->ball_speed[1] = ((rand() & 1) ? M_PI : 0) + M_PI * (double)(rand() % 1024) / (1023 * 4);
    }
    // Ball in Y-Richtung bewegen - dort kann er nur auf die Wand treffen
    gs->ball_position[1] += gs->ball_speed[0] * sin(gs->ball_speed[1]) * dt;
    if (gs->ball_position[1] > 1) {
        gs->ball_position[1] = 1;
        // Einfallswinkel = Ausfallswinkel
        gs->ball_speed[1] = 2 * M_PI - gs->ball_speed[1];
    } else if (gs->ball_position[1] < 0) {
        gs->ball_position[1] = 0;
        gs->ball_speed[1] = 2 * M_PI - gs->ball_speed[1];
    }
    // Benutzereingaben verwalten
    // linker Spieler
    if (gs->controls & CONTROL_W) {
        gs->position[0] -= gs->speed[0] * dt;
        if (gs->position[0] < 0) {
            gs->position[0] = 0;
        }
    } else if (gs->controls & CONTROL_S) {
        gs->position[0] += gs->speed[0] * dt;
        if (gs->position[0] > 1) {
            gs->position[0] = 1;
        }
    }
    // rechter Spieler
    if (gs->controls & CONTROL_UP) {
        gs->position[1] -= gs->speed[1] * dt;
        if (gs->position[1] < 0) {
            gs->position[1] = 0;
        }
    } else if (gs->controls & CONTROL_DOWN) {
        gs->position[1] += gs->speed[1] * dt;
        if (gs->position[1] > 1) {
            gs->position[1] = 1;
        }
    }
    if (gs->controls & CONTROL_ESCAPE) {
        gs->controls &= ~CONTROL_ESCAPE;
        gs->state = MODEL_STATE_INTRO;
    }
    // Bricks behandeln
    brick_collision(gs);
    brick_add(gs);
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
            model_end(gs);
            view_update_playing(gs);
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
    struct brick *next;
    while (gs->brick_list) {
        next = gs->brick_list->next;
        free(gs->brick_list);
        gs->brick_list = next;
    }
}
