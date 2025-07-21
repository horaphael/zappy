/*
** EPITECH PROJECT, 2025
** B-YEP-400-RUN-4-1-zappy-julien.kbidi
** File description:
** player_commands
*/

#include "server.h"

void send_pnw(global_t *global, player_t *player, UI index)
{
    if (!index)
        return;
    dprintf(SERVER->pfds[index].fd, "pnw #%d %d %d %d %d %s\n",
        player->id,
        player->pos_x,
        player->pos_y,
        player->direction,
        player->level,
        player->team_name);
}

void send_ppo(global_t *global, player_t *player, UI index)
{
    if (!index)
        return;
    dprintf(SERVER->pfds[index].fd, "ppo #%d %d %d %d\n",
        player->id, player->pos_x, player->pos_y, player->direction);
}

void send_plv(global_t *global, player_t *player, UI index)
{
    if (!index)
        return;
    dprintf(SERVER->pfds[index].fd, "plv #%d %d\n", player->id, player->level);
}

void send_pin(global_t *global, player_t *player, UI index)
{
    if (!index)
        return;
    dprintf(SERVER->pfds[index].fd, "pin #%d %d %d %d %d %d %d %d %d %d\n",
        player->id,
        player->pos_x,
        player->pos_y,
        player->ressources[0],
        player->ressources[1],
        player->ressources[2],
        player->ressources[3],
        player->ressources[4],
        player->ressources[5],
        player->ressources[6]);
}

void send_pdi(global_t *global, player_t *player, UI index)
{
    if (!index)
        return;
    dprintf(SERVER->pfds[index].fd, "pdi #%d\n", player->id);
}
