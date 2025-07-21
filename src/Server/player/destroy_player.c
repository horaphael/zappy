/*
** EPITECH PROJECT, 2025
** Destroy_player.c
** File description:
** Zappy
*/

#include "../../../includes/Server/server.h"

static void delete_player_from_list(global_t *global,
    player_t *prev, player_t *curr, int player_id)
{
    if (prev)
        prev->next = curr->next;
    else
        WORLD->players = curr->next;
    LOG_DEBUG("Deleted player ID=%d from world", player_id);
    free(curr->team_name);
    free(curr);
}

static void remove_player_from_world(global_t *global, int player_id)
{
    player_t *prev = NULL;
    player_t *curr = WORLD->players;

    while (curr) {
        if ((int)curr->id == player_id) {
            delete_player_from_list(global, prev, curr, player_id);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void disconnect_client(global_t *global, int i, int *nfds)
{
    int fd = SERVER->pfds[i].fd;
    int player_id = SERVER->clients[i] ? SERVER->clients[i]->id : -1;

    if (SERVER->gui_client == i) {
        SERVER->gui_client = 0;
    } else {
        send_pdi(global, WORLD->players, SERVER->gui_client);
    }
    LOG_INFO("Client ID [%d] disconnected.", i);
    close(fd);
    remove_player_from_world(global, player_id);
    SERVER->pfds[i].fd = -1;
    SERVER->pfds[i].events = 0;
    SERVER->clients[i]->type = CLIENT_UNKNOW;
    (*nfds)--;
}
