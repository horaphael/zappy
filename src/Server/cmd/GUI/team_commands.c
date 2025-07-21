/*
** EPITECH PROJECT, 2025
** B-YEP-400-RUN-4-1-zappy-julien.kbidi
** File description:
** team_commands
*/

#include "server.h"

void send_tna(global_t *global, UI index)
{
    if (!index)
        return;
    for (int i = 0; FLAGS->teams_name[i]; i++)
        dprintf(SERVER->pfds[index].fd, "tna %s\n", FLAGS->teams_name[i]);
}
