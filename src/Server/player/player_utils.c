/*
** EPITECH PROJECT, 2025
** Player_utils.c
** File description:
** Zappy
*/

#include "server.h"

void add_player_to_world(world_t *world, player_t *new_player)
{
    if (!world || !new_player)
        return;
    new_player->next = world->players;
    world->players = new_player;
}

void add_player_to_tile(tile_t *tile, player_t *player)
{
    size_t count = 0;
    player_t **new_list;

    if (!tile->players) {
        tile->players = calloc(2, sizeof(player_t *));
        if (!tile->players)
            return;
        tile->players[0] = player;
        tile->players[1] = NULL;
        return;
    }
    while (tile->players[count])
        count++;
    new_list = realloc(tile->players, sizeof(player_t *) * (count + 2));
    if (!new_list)
        return;
    new_list[count] = player;
    new_list[count + 1] = NULL;
    tile->players = new_list;
}

char *get_tile_content(tile_t *tile)
{
    char *buffer = calloc(256, sizeof(char));

    if (!buffer)
        return NULL;
    for (UI i = 0; tile->players && tile->players[i]; i++)
        strcat(buffer, "player ");
    for (int i = 0; i < NB_RESSOURCES; i++) {
        for (UI j = 0; j < tile->ressources[i]; j++) {
            strcat(buffer, get_ressource_name(i));
            strcat(buffer, " ");
        }
    }
    buffer[strcspn(buffer, "\n")] = '\0';
    return buffer;
}

player_t *get_player_by_client_id(global_t *global, int client_id)
{
    int target_id;
    player_t *tmp;

    if (!global || !SERVER->clients[client_id])
        return NULL;
    target_id = SERVER->clients[client_id]->id;
    for (tmp = WORLD->players; tmp != NULL; tmp = tmp->next)
        if ((int)tmp->id == target_id)
            return tmp;
    return NULL;
}
