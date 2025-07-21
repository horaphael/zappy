/*
** EPITECH PROJECT, 2025
** B-YEP-400-RUN-4-1-zappy-julien.kbidi
** File description:
** broadcast
*/

#include "server.h"

void cmd_broadcast(global_t *global, int client_id, char *args)
{
    char msg[1024];
    int sender_fd = SERVER->pfds[client_id].fd;
    int fd = 0;

    if (!args || strlen(args) == 0) {
        dprintf(sender_fd, "ko\n");
        return;
    }
    snprintf(msg, sizeof(msg), "%s\n", args);
    for (int j = 1; j <= MAX_CLIENTS; j++) {
        fd = SERVER->pfds[j].fd;
        if (j != client_id && fd != -1)
            write(fd, msg, strlen(msg));
    }
    dprintf(sender_fd, "ok\n");
}
