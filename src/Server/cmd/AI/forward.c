/*
** EPITECH PROJECT, 2025
** Forward.c
** File description:
** Zappy
*/

#include "../../../../includes/Server/server.h"

void check_direction(player_t *p, flags_t *flags)
{
    switch (p->direction) {
        case NORTH:
            p->pos_y = (p->pos_y == 0) ? flags->height - 1 : p->pos_y - 1;
            break;
        case SOUTH:
            p->pos_y = (p->pos_y + 1) % flags->height;
            break;
        case EAST:
            p->pos_x = (p->pos_x + 1) % flags->width;
            break;
        case WEST:
            p->pos_x = (p->pos_x == 0) ? flags->width - 1 : p->pos_x - 1;
            break;
    }
}

void remove_player_from_tile(tile_t *tile, player_t *player)
{
    for (size_t i = 0; tile->players[i]
        && (tile->players[i] == player); i++) {
        for (; tile->players[i]; i++)
            tile->players[i] = tile->players[i + 1];
        return;
    }
}

void cmd_forward(global_t *global, int client_id, char *args)
{
    player_t *p = get_player_by_client_id(global, client_id);
    tile_t *old_tile;
    tile_t *new_tile;

    (void)args;
    if (!p) {
        dprintf(SERVER->pfds[client_id].fd, "ko\n");
        LOG_WARN("Client ID [%d]: Forward failed.", client_id);
        return;
    }
    old_tile = &WORLD->map[p->pos_y][p->pos_x];
    remove_player_from_tile(old_tile, p);
    check_direction(p, FLAGS);
    new_tile = &WORLD->map[p->pos_y][p->pos_x];
    add_player_to_tile(new_tile, p);
    dprintf(SERVER->pfds[client_id].fd, "ok\n");
    LOG_INFO("Client ID [%d] moved to [%d;%d]", client_id, p->pos_x, p->pos_y);
    send_ppo(global, WORLD->players, SERVER->gui_client);
}
