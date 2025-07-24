/*
** EPITECH PROJECT, 2025
** network_lib
** File description:
** net_handle
*/

#include "net.h"
#include "log.h"

void handle_data(int fd, char *data)
{
    char *token = strtok(data, "\n");

    if (!token)
        return;
    LOG_INFO("Client [%d] sent: \"%s\"", fd, token);
    net_send(fd, "OK\n");
}

void handle_connect(int fd)
{
    LOG_INFO("New client connected: [%d]", fd);
    net_send(fd, "Connected.\n");
}

void handle_disconnect(int fd)
{
    LOG_INFO("Client [%d] disconnected.", fd);
}
