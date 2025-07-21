/*
** EPITECH PROJECT, 2025
** handle_commands.c
** File description:
** Zappy
*/

#include "server.h"

void send_pex(global_t *global, player_t *player, UI index)
{
    if (!index)
        return;
    dprintf(SERVER->pfds[index].fd, "pex #%d\n", player->id);
}

void send_pbc(global_t *global, player_t *player, UI index, const char *msg)
{
    if (!index)
        return;
    dprintf(SERVER->pfds[index].fd, "pbc #%d %s\n", player->id, msg);
}

// void send_pic(global_t *global, player_t *player, UI index)
// {
//     dprintf(SERVER->pfds[index].fd, "pic #%d %s ...\n",
//         player->pos_x,
//         player->pos_y,
//         player->id);
// }

// void send_pie(global_t *global, UI index)
// {}
