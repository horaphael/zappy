/*
** EPITECH PROJECT, 2025
** network_lib
** File description:
** net_client
*/

#include "../include/net.h"
#include <stdlib.h>
#include <string.h>

void init_clients_array(net_server_t *server)
{
    for (int i = 0; i < MAX_CLIENTS; i++) {
        server->clients[i].fd = -1;
        server->clients[i].active = false;
        server->clients[i].buffer = calloc(1, sizeof(server->buffer_size));
    }
}

void net_close_client(net_server_t *server, int fd)
{
    if (!server || fd < 0)
        return;
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (server->clients[i].fd == fd && server->clients[i].active) {
            close(server->clients[i].fd);
            server->clients[i].fd = -1;
            server->clients[i].active = false;
            server->pfds[i + 1].fd = -1;
            server->pfds[i + 1].events = 0;
            server->pfds[i + 1].revents = 0;
            break;
        }
    }
}

void net_close_all_clients(net_server_t *server)
{
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (server->clients[i].active && server->clients[i].fd != -1) {
            close(server->clients[i].fd);
            server->clients[i].fd = -1;
            server->clients[i].active = false;
        }
    }
}
