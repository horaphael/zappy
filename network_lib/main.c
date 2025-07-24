/*
** EPITECH PROJECT, 2025
** network_lib
** File description:
** server
*/

#include "include/net.h"

int main(void)
{
    net_server_t *server = net_server_create(4242);

    if (!server)
        return 84;
    if (!net_server_start(server))
        return 84;
    net_server_poll(server);
    net_server_destroy(server);
    return 0;
}
