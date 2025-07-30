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

static const char *color_codes[] = {
    "\033[0;31m", // Red
    "\033[0;32m", // Green
    "\033[0;33m", // Yellow
    "\033[0;34m", // Blue
    "\033[0;35m", // Magenta
    "\033[0;36m", // Cyan
    "\033[0;37m"  // White
};
void handle_client_data(net_client_t *client, net_server_t *server, void *args)
{
    (void)args;
    char formatted_msg[1024];

    LOG_INFO("Commande reçue de %d: %s", client->fd, client->buffer);
    snprintf(formatted_msg, sizeof(formatted_msg), "%sUser %d send you:\n%s\033[0m\n", color_codes[client->fd % 7], client->fd, client->buffer);
    msg_packet_t packet = {
        .message = formatted_msg,
        .message_size = strlen(formatted_msg) + 1
    };
    net_send_all(server, packet);
}

int main(void)
{
    net_server_t *server = net_server_create("127.0.0.1", 4242, 1024);
    int timeout = 0;
    char data_disconnect_command[256] = "Commande disconnection reçu";
    char data_connect_command[256] = "Commande connection reçu";

    if (!server)
        return 84;
    net_set_handle_connection(server, handle_client_connect, data_connect_command);
    net_set_handle_disconnection(server, handle_client_disconnect, data_disconnect_command);
    net_set_handle_data(server, handle_client_data, NULL);
    if (!net_server_start(server))
        return 84;
    while (server->running) {
        net_server_poll(server, timeout);
    }
    net_server_destroy(server);
    return 0;
}
