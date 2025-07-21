/*
** EPITECH PROJECT, 2025
** Inventory.C
** File description:
** Zappy
*/

#include "../../../../includes/Server/server.h"

void cmd_inventory(global_t *global, int client_id, char *args)
{
    player_t *p = get_player_by_client_id(global, client_id);
    const char *r_names[] = {"food", "linemate", "deraumere",
        "sibur", "mendiane", "phiras", "thystame"};

    (void)args;
    if (!p) {
        dprintf(SERVER->pfds[client_id].fd, "ko\n");
        LOG_WARN("Client ID [%d]: Inventory failed.", client_id);
        return;
    }
    dprintf(SERVER->pfds[client_id].fd, "[");
    for (int i = 0; i < 7; i++){
        if (i == 6)
            dprintf(SERVER->pfds[client_id].fd, "%s %d]\n", r_names[i],
                    p->ressources[i]);
        else
            dprintf(SERVER->pfds[client_id].fd, "%s %d, ", r_names[i],
                    p->ressources[i]);
    }
    send_pin(global, WORLD->players, SERVER->gui_client);
}

