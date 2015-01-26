#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <string.h>

#include "game.h"

void highscore_read(struct game_state *gs)
{
    unsigned int i;
    FILE *f;
    char *path;
    struct passwd *pw = getpwuid(getuid());
    path = malloc(strlen(pw->pw_dir) + 1 + strlen(HIGHSCORE_FILENAME) + 1);
    // Den Highscore auf einen definierten Zustand setzen, falls das Folgende fehlschlagen sollte.
    for (i = 0; i < HIGHSCORE_ENTRIES; i++) {
        strncpy(gs->highscore[i].name, "empty", HIGHSCORE_NAMELENGTH);
        gs->highscore[i].score = 0;
    }
    if (!path) {
        return;
    }
    memcpy(path, pw->pw_dir, strlen(pw->pw_dir));
    path[strlen(pw->pw_dir)] = '/';
    strcpy(path + strlen(pw->pw_dir) + 1, HIGHSCORE_FILENAME);
    f = fopen(path, "r");
    free(path);
    if (!f) {
        return;
    }
    for (i = 0; i < HIGHSCORE_ENTRIES; i++) {
        fscanf(f,  "%" HIGHSCORE_NAMELENGTH_STR "s %u\n", gs->highscore[i].name, &(gs->highscore[i].score));
    }
    fclose(f);
}

void highscore_add(struct game_state *gs, char *name, unsigned int score)
{
    unsigned int i;
    for (i = 0; (i < HIGHSCORE_ENTRIES) && (gs->highscore[i].score >= score); i++);
    if (i == HIGHSCORE_ENTRIES) {
        // Dann muss nichts eingetragen werden.
        return;
    }
    // vorige Einträge nach hinten verschieben.
    memmove(gs->highscore + i, gs->highscore + i + 1, (HIGHSCORE_ENTRIES - i - 1) * sizeof(struct highscore_entry));
    strncpy(gs->highscore[i].name, name, HIGHSCORE_NAMELENGTH);
    gs->highscore[i].score = score;
}

int highscore_write(struct game_state *gs)
{
    unsigned int i;
    FILE *f;
    char *path;
    struct passwd *pw = getpwuid(getuid());
    path = malloc(strlen(pw->pw_dir) + 1 + strlen(HIGHSCORE_FILENAME) + 1);
    if (!path) {
        return -1;
    }
    memcpy(path, pw->pw_dir, strlen(pw->pw_dir));
    path[strlen(pw->pw_dir)] = '/';
    strcpy(path + strlen(pw->pw_dir) + 1, HIGHSCORE_FILENAME);
    f = fopen(path, "w");
    free(path);
    if (!f) {
        return -1;
    }
    for (i = 0; i < HIGHSCORE_ENTRIES; i++) {
        fprintf(f, "%s %u\n", gs->highscore[i].name, gs->highscore[i].score);
    }
    fclose(f);
    return 0;
}
