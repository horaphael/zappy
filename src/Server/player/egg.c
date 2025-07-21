/*
** EPITECH PROJECT, 2025
** egg.c
** File description:
** Zappy
*/

#include "server.h"

egg_t *laid_egg(player_t *player)
{
    static unsigned int next_egg_id = 0;
    egg_t *egg = malloc(sizeof(egg_t));

    if (!egg || !player)
        return NULL;
    egg->id = next_egg_id++;
    egg->x = player->pos_x;
    egg->y = player->pos_y;
    egg->team_name = strdup(player->team_name);
    egg->hatched = false;
    return egg;
}

void add_egg_to_world(world_t *world, egg_t *egg)
{
    static size_t next_index = 0;
    static size_t capacity = 0;
    size_t new_capacity = 0;
    egg_t **new_array;

    if (!world || !egg)
        return;
    if (next_index >= capacity) {
        new_capacity = (capacity == 0) ? 8 : capacity * 2;
        new_array = realloc(world->eggs, new_capacity * sizeof(egg_t *));
        if (!new_array)
            return;
        world->eggs = new_array;
        capacity = new_capacity;
    }
    world->eggs[next_index++] = egg;
}

unsigned int count_disponible_eggs(global_t *global, const char *name)
{
    UI count = 0;

    for (int i = 0; WORLD->eggs[i]; i++) {
        if ((strcmp(WORLD->eggs[i]->team_name, name) == 0) &&
            (WORLD->eggs[i]->hatched == false))
                count++;
    }
    return count;
}
