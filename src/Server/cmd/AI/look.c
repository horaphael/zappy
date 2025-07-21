/*
** EPITECH PROJECT, 2025
** Look.c
** File description:
** Zappy
*/

#include "server.h"

pos_t get_tile_position(player_t *p, int depth, int offset, flags_t *f)
{
    pos_t pos = {p->pos_x, p->pos_y};

    switch (p->direction) {
        case NORTH:
            pos.x = (p->pos_x + offset + f->width) % f->width;
            pos.y = (p->pos_y - depth + f->height) % f->height;
            break;
        case SOUTH:
            pos.x = (p->pos_x - offset + f->width) % f->width;
            pos.y = (p->pos_y + depth) % f->height;
            break;
        case EAST:
            pos.x = (p->pos_x + depth) % f->width;
            pos.y = (p->pos_y + offset + f->height) % f->height;
            break;
        case WEST:
            pos.x = (p->pos_x - depth + f->width) % f->width;
            pos.y = (p->pos_y - offset + f->height) % f->height;
            break;
    }
    return pos;
}

const char *get_ressource_name(int i)
{
    static const char *names[NB_RESSOURCES] = {
        " food", " linemate", " deraumere", " sibur",
        " mendiane", " phiras", " thystame"
    };

    if (i < 0 || i >= NB_RESSOURCES)
        return "";
    return names[i];
}

static void append_resources(tile_t *tile, char *buffer, size_t size)
{
    for (int i = 0; i < NB_RESSOURCES; i++) {
        for (UI j = 0; j < tile->ressources[i]; j++) {
            strncat(buffer, get_ressource_name(i),
                size - strlen(buffer) - 1);
            strncat(buffer, " ", size - strlen(buffer) - 1);
        }
    }
}

static void append_players(tile_t *tile, char *buffer, size_t size)
{
    for (int i = 0; tile->players && tile->players[i]; i++)
        strncat(buffer, "player ", size - strlen(buffer) - 1);
}

void append_tile_contents(tile_t *tile, char *buffer, size_t size)
{
    size_t len = 0;

    append_players(tile, buffer, size);
    append_resources(tile, buffer, size);
    len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == ' ')
        buffer[len - 1] = '\0';
}

void look_level(global_t *global, player_t *player, int d, char *result)
{
    pos_t pos;
    tile_t *tile;

    for (int i = -d; i <= d; i++) {
        pos = get_tile_position(player, d, i, FLAGS);
        tile = &WORLD->map[pos.y][pos.x];
        if (d != 0 || i != -d)
            strcat(result, ",");
        append_tile_contents(tile, result, sizeof(result));
    }
}

void cmd_look(global_t *global, int client_id, char *args)
{
    player_t *p = get_player_by_client_id(global, client_id);
    char result[1024] = {'\0'};

    (void)args;
    if (!p) {
        dprintf(SERVER->pfds[client_id].fd, "ko\n");
        LOG_WARN("Client ID [%d]: Look failed.", client_id);
        return;
    }
    strcat(result, "[");
    for (int d = 0; d <= (int)p->level; d++)
        look_level(global, p, d, result);
    strcat(result, "]\n");
    dprintf(SERVER->pfds[client_id].fd, "%s", result);
    LOG_INFO("Client [%d]: Look at [%d;%d] facing %d",
            client_id, p->pos_x, p->pos_y, p->direction);
}
