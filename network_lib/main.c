/*
** EPITECH PROJECT, 2025
** network_lib
** File description:
** server
*/

#include "include/net.h"
#include "include/log.h"
#include <stdlib.h>

void handle_client_connect(net_client_t *client, net_server_t *server, void *args)
{
    (void)args;
    (void)server;
    LOG_INFO("New client connected: fd=%d", client->fd);
    client->active = true;
    if (client->buffer)
        free(client->buffer);
    client->buffer = calloc(1, sizeof(server->buffer_size));
    if (!client->buffer)
        return;
    net_send(client->fd, "Welcome to the server!\n");
}

void handle_client_disconnect(net_client_t *client, net_server_t *server, void *args)
{
    (void)args;
    (void)server;
    LOG_INFO("Client disconnected: fd=%d", client->fd);
    close(client->fd);
    client->fd = -1;
    client->active = false;
    free(client->buffer);
}

void handle_client_data(net_client_t *client, net_server_t *server, void *args)
{
    (void)server;
    (void)args;
    LOG_INFO("Data from fd=%d: %s", client->fd, client->buffer);
    net_send(client->fd, "Received your data\n");
}

int main(void)
{
    net_server_t *server = net_server_create("127.0.0.1", 4243, 2 * sizeof(char));
    int timeout = 0;

    if (!server)
        return 84;
    set_handle_connection(server, handle_client_connect);
    set_handle_disconnection(server, handle_client_disconnect);
    set_handle_data(server, handle_client_data);

    // set_handle_connection(server, NULL);
    // set_handle_disconnection(server, NULL);
    // set_handle_data(server, NULL);
    if (!net_server_start(server))
        return 84;
    while (server->running) {
        net_server_poll(server, timeout);
    }
    net_server_destroy(server);
    return 0;
}
