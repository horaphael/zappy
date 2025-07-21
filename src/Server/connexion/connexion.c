/*
** EPITECH PROJECT, 2025
** B-YEP-400-RUN-4-1-zappy-julien.kbidi
** File description:
** connexion
*/

#include "../../../includes/Server/server.h"

static bool fill_server_struct(global_t *global, int i, int connfd, int *nfds)
{
    if (SERVER->pfds[i].fd == -1) {
        SERVER->pfds[i].fd = connfd;
        SERVER->pfds[i].events = POLLIN | POLLHUP;
        SERVER->pfds[i].revents = 0;
        SERVER->clients[i] = init_client(i);
        if (i >= *nfds)
            *nfds = i + 1;
        write(connfd, "WELCOME\n", 8);
        return true;
    }
    return false;
}

int accept_connexion(global_t *global, int *nfds)
{
    int connfd = accept(SERVER->listenfd, NULL, NULL);

    if (connfd < 0) {
        perror("accept");
        return -1;
    }
    for (int i = 1; i < MAX_CLIENT_INDEX; i++)
        if (fill_server_struct(global, i, connfd, nfds))
            return connfd;
    close(connfd);
    return -1;
}

void poll_connexion(global_t *global, int *nfds)
{
    if (poll(SERVER->pfds, *nfds, 0) < 0)
        return perror("poll");
    if (SERVER->pfds[0].revents & POLLIN)
        accept_connexion(global, nfds);
    for (int i = 1; i < *nfds; i++) {
        if (SERVER->pfds[i].fd == -1)
            continue;
        if (SERVER->pfds[i].revents & (POLLHUP | POLLERR)) {
            disconnect_client(global, i, nfds);
            continue;
        }
        if (SERVER->pfds[i].revents & (POLLIN)
            && (!handle_client_data(global, i)))
            disconnect_client(global, i, nfds);
    }
}
