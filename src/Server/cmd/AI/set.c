/*
** EPITECH PROJECT, 2025
** set.c
** File description:
** Zappy
*/

#include "../../../../includes/Server/server.h"

void cmd_set(global_t *global, int client_id, char *args)
{
    player_t *p = get_player_by_client_id(global, client_id);
    char *token = strtok(NULL, "\n\t\r");
    int ind = 0;

    (void)args;
    if (!p) {
        dprintf(SERVER->pfds[client_id].fd, "ko\n");
        LOG_WARN("Client ID [%d]: Take failed.", client_id);
        return;
    }
    if (!token){
        for (int i = 0; i < 7; i++){
            WORLD->map[p->pos_y][p->pos_x].ressources[i] += p->ressources[i];
            p->ressources[i] = 0;
        }
    } else {
        ind = get_ressource_id(token);
        WORLD->map[p->pos_y][p->pos_x].ressources[ind] += p->ressources[ind];
        p->ressources[ind] = 0;
    }
    dprintf(SERVER->pfds[client_id].fd, "ok\n");
    send_pdr(global, WORLD->players, SERVER->gui_client, ind);
}
