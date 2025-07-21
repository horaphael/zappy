/*
** EPITECH PROJECT, 2025
** Incantation.c
** File description:
** Zappy
*/

#include "server.h"

static const elevation_t incantation_levels[] = {
    {1, 1, {1, 0, 0, 0, 0, 0, 0}},
    {2, 2, {1, 1, 1, 0, 0, 0, 0}},
    {3, 2, {2, 0, 1, 0, 2, 0, 0}},
    {4, 4, {1, 1, 2, 0, 1, 0, 0}},
    {5, 4, {1, 2, 1, 3, 0, 0, 0}},
    {6, 6, {1, 2, 3, 0, 1, 0, 0}},
    {7, 6, {2, 2, 2, 2, 2, 1, 0}},
};

bool check_incantation(global_t *global, player_t *players)
{
    tile_t *tile;
    elevation_t required;
    UI level;
    UI x;
    UI y;

    if (players->level >= 8)
        return false;
    level = players->level;
    x = players->pos_x;
    y = players->pos_y;
    tile = &WORLD->map[y][x];
    required = incantation_levels[level - 1];
    if (!has_required_players(tile, level, required.nb_players))
        return false;
    if (!has_required_ressources(tile, required.ressources))
        return false;
    return true;
}

bool has_required_players(tile_t *tile, UI level, UI req_count)
{
    UI count = 0;
    UI res = 0;

    for (UI i = 0; tile->players[i]; i++) {
        LOG_DEBUG("Found player ID=%d level=%d",
            tile->players[i]->id, tile->players[i]->level);
        if (tile->players[i]->level == level)
            count++;
    }
    LOG_DEBUG("Total matching players: %u, required: %u", count, req_count);
    res = count >= req_count;
    return res;
}

bool has_required_ressources(tile_t *tile, const UI required[NB_RESSOURCES])
{
    if (!required)
        return false;
    for (UI i = 0; i < NB_RESSOURCES; i++)
        if (tile->ressources[i] < required[i])
            return false;
    LOG_DEBUG("Checking tile ressources: linemate = %u", tile->ressources[0]);
    return true;
}

void apply_incantation(tile_t *tile, elevation_t elevation)
{
    for (UI i = 0; i < NB_RESSOURCES; i++)
        tile->ressources[i] -= elevation.ressources[i];
    for (UI i = 0; tile->players && tile->players[i]; i++)
        if (tile->players[i]->level == elevation.level)
            tile->players[i]->level++;
    LOG_INFO("Successful incantation at (%u;%u), next level-> %u",
        tile->x, tile->y, elevation.level + 1);
}

static bool check_player_level(global_t *global, player_t *players, int id)
{
    if (!players) {
        LOG_WARN("Client ID [%d]: Incantion failed, player not found.",
            id);
        dprintf(SERVER->pfds[id].fd, "ko\n");
        return false;
    }
    if (players->level >= 8) {
        LOG_INFO("Client ID [%d]: Already at max level.", id);
        dprintf(SERVER->pfds[id].fd, "ko\n");
        return false;
    }
    return true;
}

void cmd_incantation(global_t *global, int client_id, char *args)
{
    player_t *players = get_player_by_client_id(global, client_id);
    tile_t *tile;
    elevation_t required;

    (void)args;
    if (!check_player_level(global, players, client_id))
        return;
    tile = &WORLD->map[players->pos_y][players->pos_x];
    required = incantation_levels[players->level - 1];
    if (!has_required_players(tile, players->level, required.nb_players) ||
        !has_required_ressources(tile, required.ressources)) {
        LOG_WARN("Client ID [%d]: Incantation failed on (%u;%u)", client_id,
        tile->x, tile->y);
        dprintf(SERVER->pfds[client_id].fd, "ko\n");
        return;
    }
    apply_incantation(tile, required);
    dprintf(SERVER->pfds[client_id].fd, "Elevation underway\n");
    // send_pic();
}
