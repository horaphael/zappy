/*
** EPITECH PROJECT, 2025
** B-YEP-400-RUN-4-1-zappy-julien.kbidi
** File description:
** init_world_struct
*/

#include "server.h"

static tile_t init_tile(UI id, UI i_h, UI i_w)
{
    tile_t tile;

    tile.id = id;
    tile.players = NULL;
    for (UI i = 0; i < NB_RESSOURCES; i++)
        tile.ressources[i] = 0;
    tile.x = i_w;
    tile.y = i_h;
    return tile;
}

static tile_t *init_line(UI width, UI *id, UI i_h)
{
    tile_t *line = malloc(sizeof(tile_t) * (width + 1));

    if (!line)
        return NULL;
    for (UI i_w = 0; i_w < width; i_w++) {
        line[i_w] = init_tile(*id, i_h, i_w);
        (*id)++;
    }
    return line;
}

tile_t **init_map(UI height, UI width)
{
    tile_t **map = malloc(sizeof(tile_t) * (height + 1));
    UI id = 0;

    if (!map)
        return NULL;
    for (UI i_h = 0; i_h < height; i_h++)
        map[i_h] = init_line(width, &id, i_h);
    return map;
}
