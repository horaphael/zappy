/*
** EPITECH PROJECT, 2025
** netlib
** File description:
** function to set the handling of data, connection and disconnectino
*/

#include "../include/net.h"
#include "../include/log.h"

void net_set_handle_data(net_server_t *server, void (*on_data)
        (net_client_t *client, struct net_server_s *server, void *args), void *args)
{
    server->on_data = on_data;
    server->data_args = args;
}

void net_set_handle_connection(net_server_t *server, void (*on_connect)
        (net_client_t *client, struct net_server_s *server, void *args), void *args)
{
    server->on_connect = on_connect;
    server->data_connection = args;
}

void net_set_handle_disconnection(net_server_t *server, void (*on_disconnect)
        (net_client_t *client, struct net_server_s *server, void *args), void *args)
{
    server->on_disconnect = on_disconnect;
    server->data_disconnection = args;
}
