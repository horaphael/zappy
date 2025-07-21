/*
** EPITECH PROJECT, 2025
** Create_player.c
** File description:
** Zappy
*/

#include "../../../includes/Server/server.h"

static direction_t random_direction(void)
{
    return (direction_t)((rand() % 4) + 1);
}

int get_team_index(global_t *global, const char *team_name)
{
    if (!team_name)
        return -1;
    for (UI i = 0; i < WORLD->nb_teams; ++i)
        if (strcmp(WORLD->teams[i]->team_name, team_name) == 0)
            return i;
    return -1;
}

player_t *create_player(global_t *global, const char *team_name)
{
    int team_index = get_team_index(global, team_name);
    static UI next_id = 1;
    player_t *player = calloc(1, sizeof(player_t));

    if (team_index == -1)
        return NULL;
    if (!player)
        return NULL;
    player->id = next_id++;
    player->level = 1;
    player->direction = random_direction();
    player->pos_x = rand() % FLAGS->width;
    player->pos_y = rand() % FLAGS->height;
    player->team_name = strdup(team_name);
    LOG_DEBUG("Player creation: ID = %d, Team = %s, Pos[%d;%d], Dir = %d",
        player->id, player->team_name, player->pos_x, player->pos_y,
        player->direction);
    return player;
}
