/*
** EPITECH PROJECT, 2025
** B-YEP-400-RUN-4-1-zappy-julien.kbidi
** File description:
** free
*/

#include "server.h"

void free_all(global_t *global)
{
    if (FLAGS->teams_name) {
        for (int i = 0; FLAGS->teams_name[i]; i++)
            free(FLAGS->teams_name[i]);
        free(FLAGS->teams_name);
    }
    if (SERVER->listenfd)
        close(SERVER->listenfd);
    if (FLAGS)
        free(FLAGS);
    if (WORLD)
        free(WORLD);
    if (SERVER)
        free(SERVER);
    free(global);
}
