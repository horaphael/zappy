/*
** EPITECH PROJECT, 2025
** B-YEP-400-RUN-4-1-zappy-julien.kbidi
** File description:
** server_command
*/

#include "server.h"

void send_seg(global_t *global, team_t *team, UI index)
{
    if (!index)
        return;
    dprintf(SERVER->pfds[index].fd, "seg %s\n", team->team_name);
}

void send_smg(global_t *global, UI index, const char *msg)
{
    if (!index)
        return;
    dprintf(SERVER->pfds[index].fd, "smg %s\n", msg);
}

void send_suc(global_t *global, UI index)
{
    if (!index)
        return;
    dprintf(SERVER->pfds[index].fd, "suc\n");
}

void send_sbp(global_t *global, UI index)
{
    if (!index)
        return;
    dprintf(SERVER->pfds[index].fd, "sbp\n");
}
