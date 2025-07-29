/*
** EPITECH PROJECT, 2025
** network_lib
** File description:
** net_server
*/
#include "../include/net.h"
#include "../include/log.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


static void init_server_address(struct sockaddr_in *addr, int port, const char *host)
{
    memset(addr, 0, sizeof(struct sockaddr_in));
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    inet_aton(host, &addr->sin_addr);
}

static bool bind_and_listen(struct sockaddr_in *servaddr, int listenfd)
{
    if (bind(listenfd, (struct sockaddr *)servaddr, sizeof(*servaddr)) < 0) {
        close_socket("bind", listenfd);
        return false;
    }
    if (listen(listenfd, MAX_CLIENTS) < 0) {
        close_socket("listen", listenfd);
        return false;
    }
    return true;
}

static int create_server_socket(const char *host, unsigned int port)
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
    init_server_address(&servaddr, port, host);
    if (!bind_and_listen(&servaddr, listenfd))
        return -1;
    return listenfd;
}

net_server_t *net_server_create(const char *host, unsigned int port, size_t buffer_size)
{
    net_server_t *server = calloc(1, sizeof(net_server_t));

    if (!server)
        return NULL;
    server->listen_fd = create_server_socket(host, port);
    if (!server->listen_fd) {
        free(server);
        return NULL;
    }
    server->port = port;
    server->running = false;
    server->pfds[0].fd = server->listen_fd;
    server->pfds[0].events = POLLIN;
    server->on_connect = NULL;
    server->on_disconnect = NULL;
    server->on_data = NULL;
    server->buffer_size = buffer_size;
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
    return true;
}

void net_server_stop(net_server_t *server)
{
    if (!server)
        return;
    server->running = false;
    LOG_DEBUG("Server stoped.");
}

static void free_server(net_server_t *server)
{
    for (int i = 0; i < MAX_CLIENTS; i++){
        if (server->clients[i].buffer)
            free(server->clients[i].buffer);
    }
    free(server);
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
    free_server(server);
    LOG_DEBUG("Server destroyed.");
}

