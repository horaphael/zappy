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

bool is_a_specified_id(int fd, int *fds, int length)
{
    for (int i = 0; i < length; i++){
        if (fd == fds[i])
            return true;
    }
    return false;
}

void net_send_to_specified_clients(net_server_t *server, group_t group_id,
        msg_packet_t packet, bool list_mode)
{
    if (group_id.group_size == 0){
        net_send_all(server, packet);
        return;
    }
    for (int i = 1; i < MAX_CLIENTS; i++){
        if (server->clients[i].active && is_a_specified_id(server->clients[i].id, group_id.clients_id, group_id.group_size) && list_mode){
            net_send(server->clients[i].fd, packet.message, packet.message_size);
            continue;
        }
        if (server->clients[i].active && !is_a_specified_id(server->clients[i].id, group_id.clients_id, group_id.group_size) && !list_mode){
            net_send(server->clients[i].fd, packet.message, packet.message_size);
            continue;
        }
    }
}

