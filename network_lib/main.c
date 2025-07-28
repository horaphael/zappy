/*
** EPITECH PROJECT, 2025
** network_lib
** File description:
** server
*/

#include "include/net.h"
#include "include/log.h"

void handle_client_connect(net_client_t *client, net_server_t *server)
{
    (void)server;
    LOG_INFO("New client connected: fd=%d", client->fd);
    client->active = true;
    memset(client->buffer, 0, sizeof(client->buffer));
    net_send(client->fd, "Welcome to the server!\n");
}

void handle_client_disconnect(net_client_t *client, net_server_t *server)
{
    (void)server;
    LOG_INFO("Client disconnected: fd=%d", client->fd);
    close(client->fd);
    client->fd = -1;
    client->active = false;
    memset(client->buffer, 0, sizeof(client->buffer));
}

void handle_client_data(net_client_t *client, net_server_t *server)
{
    (void)server;
    LOG_INFO("Data from fd=%d: %s", client->fd, client->buffer);
    net_send(client->fd, "Received your data\n");
}

int main(void)
{
    net_server_t *server = net_server_create("127.0.0.1", 4242);

    if (!server)
        return 84;
    if (!net_server_start(server))
        return 84;
    net_server_init(server, server->listen_fd, handle_client_connect, handle_client_disconnect, handle_client_data);
    while (server->running) {
        net_server_poll(server, 1000);
    }
    net_server_destroy(server);
    return 0;
}
