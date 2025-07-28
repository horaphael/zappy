/*
** EPITECH PROJECT, 2025
** network_lib
** File description:
** net_poll
*/

#include "net.h"

void init_poll_fds(net_server_t *server)
{
    for (int i = 1; i < MAX_CLIENTS; i++) {
        server->pfds[i].fd = -1;
        server->pfds[i].events = 0;
    }
}

static int search_free_slot(net_server_t *server)
{
    int free_slot = -1;

    for (int i = 1; i < MAX_CLIENTS; i++) {
        if (server->pfds[i].fd == -1) {
            free_slot = i;
            break;
        }
    }
    return free_slot;
}

static void new_connection(net_server_t *server, void (*on_connect)(net_client_t *client, net_server_t *server))
{
    int new_fd = accept(server->listen_fd, NULL, NULL);
    int free_slot = -1;

    if (new_fd < 0) {
        perror("accept");
        return;
    }
    free_slot = search_free_slot(server);
    if (free_slot != -1) {
        server->pfds[free_slot].fd = new_fd;
        server->pfds[free_slot].events = POLLIN;
        server->clients[free_slot].fd = new_fd;
        server->clients[free_slot].active = true;
        if (on_connect)
            on_connect(&server->clients[free_slot], server);
    } else
        close(new_fd);
}

static void handle_client_data(net_server_t *server, int i, void (*on_disconnect)(net_client_t *client, net_server_t *server), void (*on_data)(net_client_t *client, net_server_t *server))
{
    char buf[BUFFER_SIZE];
    ssize_t bytes_read = read(server->pfds[i].fd, buf, sizeof(buf) - 1);

    if (bytes_read <= 0) {
        if (on_disconnect)
            on_disconnect(&server->clients[i], server);
        net_close_client(server, server->pfds[i].fd);
    } else {
        buf[bytes_read] = '\0';
        if (on_data) {
            strncpy(server->clients[i].buffer, buf, sizeof(server->clients[i].buffer) - 1);
            server->clients[i].buffer[sizeof(server->clients[i].buffer) - 1] = '\0';
            on_data(&server->clients[i], server);
        }
    }
}

void net_server_init(net_server_t *server, int listen_fd, void (*on_connect)(net_client_t *, net_server_t *), void (*on_disconnect)(net_client_t *, net_server_t *), void (*on_data)(net_client_t *, net_server_t *))
{
    server->listen_fd = listen_fd;
    init_poll_fds(server);
    server->pfds[0].fd = listen_fd;
    server->pfds[0].events = POLLIN;
    server->on_connect = on_connect;
    server->on_disconnect = on_disconnect;
    server->on_data = on_data;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        server->clients[i].fd = -1;
        server->clients[i].active = false;
    }
}

void net_server_poll(net_server_t *server, int timeout)
{
    int poll_count = 0;

    if (!server)
        return;
    poll_count = poll(server->pfds, MAX_CLIENTS, timeout);
    if (poll_count < 0) {
        perror("poll");
        return;
    }
    if (server->pfds[0].revents & POLLIN)
        new_connection(server, server->on_connect);
    for (int i = 1; i < MAX_CLIENTS; i++) {
        if (server->pfds[i].fd != -1 && (server->pfds[i].revents & POLLIN))
            handle_client_data(server, i, server->on_disconnect, server->on_data);
    }
}
