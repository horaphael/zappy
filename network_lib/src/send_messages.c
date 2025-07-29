/*
** EPITECH PROJECT, 2025
** network_lib
** File description:
** function to send message
*/
#include "../include/net.h"
#include <unistd.h>

void net_send_all(net_server_t *server, msg_packet_t packet)
{
    for (int i = 1; i < MAX_CLIENTS; i++){
        if (server->clients[i].active)
            write(server->clients[i].fd, packet.message, packet.message_size);
    }
}

void net_send_to_specified_clients(net_server_t *server, void *fd,
        msg_packet_t packet, bool list_mode)
{
    int *client_list = NULL;

    if (!fd){
        net_send_all(server, packet);
        return;
    }
    client_list = (int *)fd;
    for (int i = 1; i < MAX_CLIENTS; i++){
        if (server->clients[i].active && server->clients[i].fd == client_list[i]){
            if (!list_mode)
                continue;
            net_send(client_list[i], packet.message, packet.message_size);
        }
    }
}

