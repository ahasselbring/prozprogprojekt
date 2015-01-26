#include "game.h"

void controller_cycle(struct game_state *gs)
{
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        if (ev.type == SDL_QUIT) {
            gs->quit = 1;
            return;
        } else if (ev.type == SDL_KEYDOWN) {
            switch (ev.key.keysym.sym) {
                case SDLK_UP:
                    gs->controls |= CONTROL_UP;
                    break;
                case SDLK_DOWN:
                    gs->controls |= CONTROL_DOWN;
                    break;
                case SDLK_w:
                    gs->controls |= CONTROL_W;
                    break;
                case SDLK_s:
                    gs->controls |= CONTROL_S;
                    break;
                case SDLK_RETURN:
                    gs->controls |= CONTROL_ENTER;
                    break;
                case SDLK_ESCAPE:
                    gs->controls |= CONTROL_ESCAPE;
                    break;
            }
        } else if (ev.type == SDL_KEYUP) {
            switch (ev.key.keysym.sym) {
                case SDLK_UP:
                    gs->controls &= ~CONTROL_UP;
                    break;
                case SDLK_DOWN:
                    gs->controls &= ~CONTROL_DOWN;
                    break;
                case SDLK_w:
                    gs->controls &= ~CONTROL_W;
                    break;
                case SDLK_s:
                    gs->controls &= ~CONTROL_S;
                    break;
                case SDLK_RETURN:
                    gs->controls &= ~CONTROL_ENTER;
                    break;
                case SDLK_ESCAPE:
                    gs->controls &= ~CONTROL_ESCAPE;
                    break;
            }
        }
    }
    model_cycle(gs);
    SDL_Delay(CYCLE_DELAY);
}
