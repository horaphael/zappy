/*
** EPITECH PROJECT, 2025
** new_zappy
** File description:
** init_struct
*/

#include "../../../includes/Server/server.h"

static flags_t *init_flag_struct(void)
{
    flags_t *flags = calloc(1, sizeof(flags_t));

    if (!flags)
        return NULL;
    return flags;
}

static server_t *init_server_struct(void)
{
    server_t *server = malloc(sizeof(server_t));

    if (!server)
        return NULL;
    server->listenfd = -1;
    server->nfds = 1;
    server->running = true;
    server->gui_client = 0;
    return server;
}

client_t *init_client(int index)
{
    client_t *client = malloc(sizeof(client_t));

    if (!client)
        return NULL;
    client->id = index;
    client->alive = true;
    client->type = CLIENT_UNKNOW;
    return client;
}

egg_t **init_eggs(global_t *global)
{
    UI nb_eggs = FLAGS->clientsNb * WORLD->nb_teams;
    egg_t **eggs = malloc(sizeof(egg_t *) * (nb_eggs + 1));
    UI index = 0;
    UI index_team = 0;

    if (!eggs)
        return NULL;
    eggs[nb_eggs] = NULL;
    for (UI i = 0; i < WORLD->nb_teams; i++) {
        for (UI j = 0; j != FLAGS->clientsNb; j++) {
            eggs[index] = malloc(sizeof(egg_t));
            eggs[index]->id = index;
            eggs[index]->hatched = false;
            eggs[index]->team_name = FLAGS->teams_name[index_team];
            index++;
        }
        index_team++;
    }
    return eggs;
}

team_t **init_teams(global_t *global)
{
    team_t **teams = malloc(sizeof(team_t *) * (WORLD->nb_teams + 1));

    if (!teams)
        return NULL;
    teams[WORLD->nb_teams] = NULL;
    for (UI i = 0; i < WORLD->nb_teams; i++) {
        teams[i] = malloc(sizeof(team_t));
        if (!teams[i])
            return NULL;
        teams[i]->team_name = FLAGS->teams_name[i];
        teams[i]->total_place = FLAGS->clientsNb;
        teams[i]->available_place = FLAGS->clientsNb;
    }
    return teams;
}

static world_t *init_world_struct(void)
{
    world_t *world = calloc(1, sizeof(world_t));

    if (!world)
        return NULL;
    return world;
}

global_t *init_global_struct(void)
{
    global_t *global = malloc(sizeof(global_t));

    if (!global)
        return NULL;
    FLAGS = init_flag_struct();
    if (!FLAGS) {
        free(global);
        return NULL;
    }
    SERVER = init_server_struct();
    if (!SERVER) {
        free_all(global);
        return NULL;
    }
    WORLD = init_world_struct();
    if (!WORLD) {
        free_all(global);
        return NULL;
    }
    return global;
}
