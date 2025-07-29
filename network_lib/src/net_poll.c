/*
** EPITECH PROJECT, 2025
** network_lib
** File description:
** net_poll
*/

#include "../include/net.h"

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

static void new_connection(net_server_t *server)
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
        server->clients[free_slot].buffer = malloc(server->buffer_size);
        if (server->on_connect)
            server->on_connect(&server->clients[free_slot], server, server->data_connection);
    } else
        close(new_fd);
}

static void handle_client_data(net_server_t *server, int i)
{
    char buf[server->buffer_size];
    ssize_t bytes_read = read(server->pfds[i].fd, buf, sizeof(buf) - 1);

    if (bytes_read <= 0) {
        if (server->on_disconnect)
            server->on_disconnect(&server->clients[i], server, server->data_disconnection);
        net_close_client(server, server->pfds[i].fd);
    } else {
        buf[bytes_read] = '\0';
        if (server->on_data) {
            strncpy(server->clients[i].buffer, buf, sizeof(server->clients[i].buffer) - 1);
            server->clients[i].buffer[sizeof(server->clients[i].buffer) - 1] = '\0';
            server->on_data(&server->clients[i], server, server->data_args);
        }
    }
}

bool net_server_poll(net_server_t *server, int poll_timeout)
{
    int poll_count = 0;

    if (!server)
        return false;
    poll_count = poll(server->pfds, MAX_CLIENTS, poll_timeout);
    if (poll_count < 0) {
        perror("poll");
        return false;
    }
    if (server->pfds[0].revents & POLLIN)
        new_connection(server);
    for (int i = 1; i < MAX_CLIENTS; i++) {
        if (server->pfds[i].fd != -1 && (server->pfds[i].revents & POLLIN))
            handle_client_data(server, i);
    }
    return true;
}
