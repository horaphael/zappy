/*
** EPITECH PROJECT, 2025
** network_lib
** File description:
** server
*/

#include "../include/net.h"
#include "../include/log.h"
#include <signal.h>

static bool is_running = true;

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
    msg_packet_t packet = {
        .message = "a client send data\n",
        .message_size = strlen("a client send data\n"),
    };
    net_send_all(server, packet);
    LOG_INFO("Data from fd=%d: %s", client->fd, client->buffer);
    LOG_INFO("CMD: %s", cmd);
}

void sighandler(int sig)
{
    signal(sig, SIG_IGN);
    printf("Server has been shut down.\n");
    is_running = false;
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
    // set_handle_connection(server, NULL);
    // set_handle_disconnection(server, NULL);
    // set_handle_data(server, NULL);
    if (!net_server_start(server))
        return 84;
    while (server->running) {
        signal(SIGINT, sighandler);
        if (!is_running)
            server->running = false;
        net_server_poll(server, timeout);
    }
    net_server_destroy(server);
    return 0;
}
