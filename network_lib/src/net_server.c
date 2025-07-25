/*
** EPITECH PROJECT, 2025
** network_lib
** File description:
** net_server
*/

#include "net.h"
#include "log.h"

static void init_server_address(struct sockaddr_in *addr, int port)
{
    memset(addr, 0, sizeof(struct sockaddr_in));
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    addr->sin_addr.s_addr = INADDR_ANY;
}

static bool bind_and_listen(struct sockaddr_in *servaddr, int listenfd)
{
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

static int create_server_socket(int port)
{
    struct sockaddr_in servaddr;
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;

    if (listenfd < 0) {
        perror("socket");
        return -1;
    }
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
        &opt, sizeof(opt)) < 0) {
        close_socket("setsockopt", listenfd);
        return -1;
    }
    init_server_address(&servaddr, port);
    if (!bind_and_listen(&servaddr, listenfd))
        return -1;
    return listenfd;
}

net_server_t *net_server_create(unsigned int port)
{
    net_server_t *server = calloc(1, sizeof(net_server_t));

    if (!server)
        return NULL;
    server->listen_fd = create_server_socket(port);
    if (!server->listen_fd) {
        free(server);
        return NULL;
    }
    server->port = port;
    server->running = false;
    server->pfds[0].fd = server->listen_fd;
    server->pfds[0].events = POLLIN;
    init_poll_fds(server);
    init_clients_array(server);
    LOG_DEBUG("Server successfully created.");
    return server;
}

bool net_server_start(net_server_t *server)
{
    if (!server)
        return false;
    server->running = true;
    LOG_DEBUG("Server launched.");
    while (server->running) {
        net_server_poll(server);
    }
    return true;
}

void net_server_stop(net_server_t *server)
{
    if (!server)
        return;
    server->running = false;
    LOG_DEBUG("Server stoped.");
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
