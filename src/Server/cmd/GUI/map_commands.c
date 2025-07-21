/*
** EPITECH PROJECT, 2025
** B-YEP-400-RUN-4-1-zappy-julien.kbidi
** File description:
** map_commands
*/

#include "server.h"

void send_msz(global_t *global, UI index)
{
    if (!index)
        return;
    dprintf(SERVER->pfds[index].fd, "msz %d %d\n",
        FLAGS->height, FLAGS->width);
}

void send_bct(global_t *global, UI index, int i_h, int i_w)
{
    if (!index)
        return;
    dprintf(SERVER->pfds[index].fd, "bct %d %d %d %d %d %d %d %d %d\n",
        WORLD->map[i_h][i_w].y,
        WORLD->map[i_h][i_w].x,
        WORLD->map[i_h][i_w].ressources[0],
        WORLD->map[i_h][i_w].ressources[1],
        WORLD->map[i_h][i_w].ressources[2],
        WORLD->map[i_h][i_w].ressources[3],
        WORLD->map[i_h][i_w].ressources[4],
        WORLD->map[i_h][i_w].ressources[5],
        WORLD->map[i_h][i_w].ressources[6]);
}

void send_mct(global_t *global, UI index)
{
    if (!index)
        return;
    for (int i_h = 0; WORLD->map[i_h]; i_h++) {
        for (int i_w = 0; WORLD->map[i_w]; i_w++)
            send_bct(global, index, i_h, i_w);
    }
}
