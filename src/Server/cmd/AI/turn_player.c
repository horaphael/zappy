/*
** EPITECH PROJECT, 2025
** Right.c
** File description:
** Zappy
*/

#include "../../../../includes/Server/server.h"

void cmd_turn(global_t *global, int client_id, char *args)
{
    player_t *p = get_player_by_client_id(global, client_id);

    printf("Client %d wants to turn %s\n", client_id, args);
    if (strcmp(args, "Left") == 0){
        p->direction -= 1;
        if (p->direction < 1)
            p->direction = 4;
        dprintf(SERVER->pfds[client_id].fd, "ok\n");
        send_ppo(global, WORLD->players, SERVER->gui_client);
        return;
    }
    if (strcmp(args, "Right") == 0){
        p->direction += 1;
        if (p->direction > 4)
            p->direction = 1;
        dprintf(SERVER->pfds[client_id].fd, "ok\n");
        send_ppo(global, WORLD->players, SERVER->gui_client);
        return;
    }
}
