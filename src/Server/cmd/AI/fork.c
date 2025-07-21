/*
** EPITECH PROJECT, 2025
** Player.c
** File description:
** Zappy
*/

#include "server.h"

void cmd_fork(global_t *global, int client_id, char *args)
{
    player_t *player = get_player_by_client_id(global, client_id);
    egg_t *egg;

    (void)args;
    if (!player) {
        LOG_WARN("Client ID [%d]: Fork failed, player not found.", client_id);
        dprintf(SERVER->pfds[client_id].fd, "ko\n");
        return;
    }
    egg = laid_egg(player);
    if (!egg) {
        LOG_ERROR("Client ID [%d]: Failed to create egg.", client_id);
        dprintf(SERVER->pfds[client_id].fd, "ko\n");
        return;
    }
    add_egg_to_world(WORLD, egg);
    dprintf(SERVER->pfds[client_id].fd, "ok\n");
    LOG_DEBUG("Fork: Egg ID=%d laid at (%d,%d) by player ID=%d (team=%s)",
        egg->id, egg->x, egg->y, player->id, player->team_name);
    send_pfk(global, WORLD->players, SERVER->gui_client);
    send_enw(global, player, egg, SERVER->gui_client);
}
