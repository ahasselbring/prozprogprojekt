#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>

#include "game.h"

void config_read(struct game_state *gs)
{
    FILE *f;
    char *path;
    struct passwd *pw = getpwuid(getuid());
    path = malloc(strlen(pw->pw_dir) + 1 + strlen(CONFIG_FILENAME) + 1);
    gs->resolution[0] = 640;
    gs->resolution[1] = 480;
    gs->fullscreen = 0;
    if (!path) {
        return;
    }
    memcpy(path, pw->pw_dir, strlen(pw->pw_dir));
    path[strlen(pw->pw_dir)] = '/';
    strcpy(path + strlen(pw->pw_dir) + 1, CONFIG_FILENAME);
    f = fopen(path, "r");
    free(path);
    if (!f) {
        return;
    }
    fscanf(f, "w %u\n", &(gs->resolution[0]));
    fscanf(f, "h %u\n", &(gs->resolution[1]));
    fscanf(f, "f %c\n", &(gs->fullscreen));
    gs->fullscreen -= '0';
    fclose(f);
}

int config_write(struct game_state *gs)
{
    FILE *f;
    char *path;
    struct passwd *pw = getpwuid(getuid());
    path = malloc(strlen(pw->pw_dir) + 1 + strlen(CONFIG_FILENAME) + 1);
    if (!path) {
        return -1;
    }
    memcpy(path, pw->pw_dir, strlen(pw->pw_dir));
    path[strlen(pw->pw_dir)] = '/';
    strcpy(path + strlen(pw->pw_dir) + 1, CONFIG_FILENAME);
    f = fopen(path, "w");
    free(path);
    if (!f) {
        return -1;
    }
    fprintf(f, "w %u\n", gs->resolution[0]);
    fprintf(f, "h %u\n", gs->resolution[1]);
    fprintf(f, "f %c\n", gs->fullscreen + '0');
    fclose(f);
    return 0;
}
