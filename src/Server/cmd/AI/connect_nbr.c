/*
** EPITECH PROJECT, 2025
** Connect_nbr.c
** File description:
** Zappy
*/

#include "../../../../includes/Server/server.h"

void cmd_connect_nbr(global_t *global, int id, char *args)
{
    player_t *player = get_player_by_client_id(global, id);
    int team_index = -1;

    (void)args;
    if (!player) {
        LOG_WARN("Client ID [%d]: Connect_nbr failed, player not found.", id);
        dprintf(SERVER->pfds[id].fd, "ko\n");
        return;
    }
    team_index = get_team_index(global, player->team_name);
    if (team_index == -1) {
        LOG_WARN("Client ID [%d]: Connect_nbr failed, team not found.", id);
        dprintf(SERVER->pfds[id].fd, "ko\n");
        return;
    }
    dprintf(SERVER->pfds[id].fd, "%d\n",
        WORLD->teams[team_index]->available_place);
    LOG_INFO("Client ID [%d]: Connect_nbr => %d available slots",
        id, WORLD->teams[team_index]->available_place);
}
