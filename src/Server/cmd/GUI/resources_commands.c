/*
** EPITECH PROJECT, 2025
** B-YEP-400-RUN-4-1-zappy-julien.kbidi
** File description:
** resources_commands
*/

#include "server.h"

void send_pdr(global_t *global, player_t *player, UI index, int res_idx)
{
    if (!index)
        return;
    dprintf(SERVER->pfds[index].fd, "pdr #%d %d\n",
        player->id,
        player->ressources[res_idx]);
}

void send_pgt(global_t *global, player_t *player, UI index, int res_idx)
{
    if (!index)
        return;
    dprintf(SERVER->pfds[index].fd, "pgt #%d %d\n",
        player->id,
        player->ressources[res_idx]);
}
