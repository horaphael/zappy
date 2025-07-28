/*
** EPITECH PROJECT, 2025
** network_lib
** File description:
** net_server
*/

#include "net.h"
#include "log.h"

static bool bind_and_listen(const char *ip, int port, int listenfd)
{
    struct sockaddr_in servaddr = {0};

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &servaddr.sin_addr) <= 0) {
        perror("inet_pton");
        return false;
    }
    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        close_socket("bind", listenfd);
        return false;
    }
    if (listen(listenfd, MAX_CLIENTS) < 0) {
        close_socket("listen", listenfd);
        return false;
    }
    return true;
}

static int create_server_socket(const char *ip, int port)
{
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;

    if (listenfd < 0) {
        perror("socket");
        return -1;
    }
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        close_socket("setsockopt", listenfd);
        return -1;
    }
    if (!bind_and_listen(ip, port, listenfd))
        return -1;
    return listenfd;
}

net_server_t *net_server_create(const char *ip, unsigned int port)
{
    net_server_t *server = calloc(1, sizeof(net_server_t));

    if (!server)
        return NULL;
    server->listen_fd = create_server_socket(ip, port);
    if (server->listen_fd < 0) {
        free(server);
        return NULL;
    }
    strncpy(server->ip, ip, INET_ADDRSTRLEN - 1);
    server->ip[INET_ADDRSTRLEN - 1] = '\0';
    server->port = port;
    server->running = false;
    server->pfds[0].fd = server->listen_fd;
    server->pfds[0].events = POLLIN;
    init_poll_fds(server);
    init_clients_array(server);
    LOG_DEBUG("Server created on %s:%d", server->ip, server->port);
    return server;
}

bool net_server_start(net_server_t *server)
{
    if (!server)
        return false;
    server->running = true;
    LOG_DEBUG("Server started.");
    return true;
}

void net_server_stop(net_server_t *server)
{
    if (!server)
        return;
    server->running = false;
    LOG_DEBUG("Server stopped.");
}

void net_server_destroy(net_server_t *server)
{
    if (!server)
        return;
    net_close_all_clients(server);
    if (server->listen_fd != -1) {
        close(server->listen_fd);
        server->listen_fd = -1;
    }
    free(server);
    LOG_DEBUG("Server destroyed.");
}
