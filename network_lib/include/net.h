/*
** EPITECH PROJECT, 2025
** network_lib
** File description:
** net
*/

#ifndef NET_H_
    #define NET_H_

    #define MAX_CLIENTS 128
    #define BUFFER_SIZE 1024

    #include <unistd.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <stdbool.h>
    #include <string.h>
    #include <errno.h>
    #include <poll.h>
    #include <sys/socket.h>
    #include <netinet/in.h>

typedef struct net_client_s {
    int fd;
    bool active;
    char buffer[BUFFER_SIZE];
} net_client_t;

typedef struct net_server_s {
    int listen_fd;
    unsigned int port;
    struct pollfd pfds[MAX_CLIENTS];
    net_client_t clients[MAX_CLIENTS];
    bool running;
} net_server_t;

    /* Core */
net_server_t *net_server_create(unsigned int port);
bool net_server_start(net_server_t *server);
void net_server_poll(net_server_t *server);
void net_server_stop(net_server_t *server);
void net_server_destroy(net_server_t *server);

    /* Utils */
void net_send(int fd, const char *msg);
void close_socket(char *msg, int socket);

    /* Clients */
void init_clients_array(net_server_t *server);
void net_close_client(net_server_t *server, int fd);
void net_close_all_clients(net_server_t *server);

    /* Poll */
void init_poll_fds(net_server_t *server);

    /* Handle client */
void handle_disconnect(int fd);
void handle_connect(int fd);
void handle_data(int fd, char *data);

#endif /* NET_H_ */
