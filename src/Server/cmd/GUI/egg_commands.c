/*
** EPITECH PROJECT, 2025
** B-YEP-400-RUN-4-1-zappy-julien.kbidi
** File description:
** egg_commands
*/

#include "server.h"

void send_pfk(global_t *global, player_t *player, UI index)
{
    if (!index)
        return;
    dprintf(SERVER->pfds[index].fd, "pfk #%d\n", player->id);
}

void send_enw(global_t *global, player_t *player, egg_t *eggs, UI index)
{
    if (!index)
        return;
    dprintf(SERVER->pfds[index].fd, "enw #%d #%d %d %d\n",
        eggs->id,
        player->id,
        player->pos_x,
        player->pos_y);
}

// void send_ebo(global_t *global, UI index)
// {}

// void send_edi(global_t *global, UI index)
// {}
