/*
** EPITECH PROJECT, 2025
** handle_ia_command.c
** File description:
** Zappy
*/

#include "../../../../includes/Server/server.h"

static const ia_command_t IA_COMMANDS[] = {
    {"Forward", cmd_forward},
    {"Connect_nbr", cmd_connect_nbr},
    {"Incantation", cmd_incantation},
    {"Fork", cmd_fork},
    {"Right", cmd_turn},
    {"Left", cmd_turn},
    {"Inventory", cmd_inventory},
    {"Take", cmd_take},
    {"Set", cmd_set},
    {"Look", cmd_look},
    {"Broadcast", cmd_broadcast},
    {NULL, NULL},
};

void handle_ia_command(global_t *global, int client_id, char *input)
{
    char *command = strtok(input, " \r\n\t");

    if (!command)
        return;
    for (int i = 0; IA_COMMANDS[i].name; i++)
        if (strcmp(command, IA_COMMANDS[i].name) == 0) {
            IA_COMMANDS[i].command(global, client_id, command);
            return;
        }
    dprintf(SERVER->pfds[client_id].fd, "ko\n");
}
