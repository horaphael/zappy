/*
** EPITECH PROJECT, 2025
** network_lib
** File description:
** server
*/

#include "../include/net.h"
#include "../include/log.h"
#include <stdlib.h>

void handle_client_connect(net_client_t *client, net_server_t *server, void *args)
{
    char *cmd = (char *)args;

    (void)server;
    LOG_INFO("New client connected: fd=%d", client->fd);
    LOG_INFO("CMD: %s", cmd);
}

void handle_client_disconnect(net_client_t *client, net_server_t *server, void *args)
{
    char *cmd = (char *)args;

    (void)server;
    LOG_INFO("Client disconnected: fd=%d", client->fd);
    LOG_INFO("CMD: %s", cmd);
}

void handle_client_data(net_client_t *client, net_server_t *server, void *args)
{
    char *cmd = (char *)args;
    int *client_id = malloc(sizeof(int) * 2);

    if (!client_id) {
        LOG_ERROR("Memory allocation failed for client_fd");
        return;
    }
    client_id[0] = 2;
    client_id[1] = 4;

    msg_packet_t packet = {
        .message = "a client send data\n",
        .message_size = strlen("a client send data\n"),
    };
    msg_packet_t special_packet = {
        .message = "a client send a special data for u\n",
        .message_size = strlen("a client send a special data for u\n"),
    };
    group_t group = {
        .group_size = 2,
        .clients_id = client_id,
    };
    net_send_all(server, packet);
    net_send_to_specified_clients(server, group, special_packet, true);
    LOG_INFO("Data from fd=%d: %s", client->fd, client->buffer);
    LOG_INFO("CMD: %s", cmd);
}

int main(void)
{
    net_server_t *server = net_server_create("127.0.0.1", 4243, 1024);
    int timeout = 0;
    char data_command[256] = "Commande reçu";
    char data_disconnect_command[256] = "Commande disconnection reçu";
    char data_connect_command[256] = "Commande connection reçu";

    if (!server)
        return 84;
    net_set_handle_connection(server, handle_client_connect, data_connect_command);
    net_set_handle_disconnection(server, handle_client_disconnect, data_disconnect_command);
    net_set_handle_data(server, handle_client_data, data_command);
    if (!net_server_start(server))
        return 84;
    net_server_poll(server, timeout);
    net_server_destroy(server);
    return 0;
}
