/*
** EPITECH PROJECT, 2025
** B-YEP-400-RUN-4-1-zappy-julien.kbidi
** File description:
** check_args
*/

#include "server.h"

static bool check_port(flags_t *flags, char **av)
{
    int port = -1;

    if (strcmp(av[1], "-p") == 0) {
        if (!av[2]) {
            fprintf(stderr, "Error: Invalid port.\n");
            return false;
        }
        port = atoi(av[2]);
        if (port >= 0 && port <= 65535) {
            flags->port = port;
            return true;
        } else {
            fprintf(stderr, "Error: Invalid port.\n");
            return false;
        }
    }
    return false;
}

static bool check_width(flags_t *flags, char **av)
{
    int width = -1;

    if (strcmp(av[3], "-x") == 0) {
        if (!av[4]) {
            fprintf(stderr, "Error: Invalid width.\n");
            return false;
        }
        width = atoi(av[4]);
        if (width >= 10 && width <= MAX_MAP_SIZE) {
            flags->width = width;
            return true;
        } else {
            fprintf(stderr, "Error: Invalid map width.\n");
            return false;
        }
    }
    return false;
}

static bool check_height(flags_t *flags, char **av)
{
    int height = -1;

    if (strcmp(av[5], "-y") == 0) {
        if (!av[6]) {
            fprintf(stderr, "Error: Invalid height.\n");
            return false;
        }
        height = atoi(av[6]);
        if (height >= 10 && height <= MAX_MAP_SIZE) {
            flags->height = height;
            return true;
        } else {
            fprintf(stderr, "Error: Invalid map height.\n");
            return false;
        }
    }
    return false;
}

static int get_nb_team(global_t *global, char **av)
{
    int nb_team_name = 0;

    for (int i = 1; av[7 + i]; i++) {
        if (strcmp(av[7 + i], "-c") == 0)
            break;
        nb_team_name++;
    }
    global->world->nb_teams = nb_team_name;
    return nb_team_name;
}

bool check_team_name(char **team_name)
{
    int len_tab = 0;
    int j = 0;

    for (int i = 0; team_name[i]; i++)
        len_tab++;
    for (int i = 0; team_name[i] && team_name[j]; i++) {
        for (; team_name[j]; j++) {
            if (i == len_tab && j == len_tab)
                break;
            if (i == j)
                continue;
            if (strcmp(team_name[i], team_name[j]) == 0) {
                fprintf(stderr, "Error: Bad team name\n");
                return false;
            }
        }
        j = 0;
    }
    return true;
}

static bool fill_teams_name(flags_t *flags, char **av)
{
    int j = 0;

    for (int i = 1; av[7 + i]; i++) {
        if (strcmp(av[7 + i], "-c") == 0)
            break;
        flags->teams_name[j] = strdup(av[7 + i]);
        if (!flags->teams_name[j])
            return false;
        j++;
    }
    if (!check_team_name(flags->teams_name))
        return false;
    return true;
}

static bool check_name(global_t *global, char **av)
{
    int nbName = get_nb_team(global, av);

    if (strcmp(av[7], "-n") == 0) {
        global->flags->teams_name = malloc(sizeof(char *) * (nbName + 1));
        global->flags->teams_name[nbName] = NULL;
        if (!global->flags->teams_name)
            return false;
        if (!fill_teams_name(global->flags, av))
            return false;
        global->flags->teams_name[nbName] = NULL;
        return true;
    }
    return false;
}

static bool check_clients_nb(flags_t *flags, char **av, int i)
{
    int clientsNb = -1;

    if (strcmp(av[i], "-c") == 0) {
        if (!av[i + 1]) {
            fprintf(stderr, "Error: Invalid clientsNb.\n");
            return false;
        }
        clientsNb = atoi(av[i + 1]);
        if (clientsNb > 0) {
            flags->clientsNb = clientsNb;
            return true;
        }
    }
    return true;
}

static bool check_freq(flags_t *flags, char **av, int i)
{
    int freq = 100;

    if (strcmp(av[i], "-f") == 0) {
        if (!av[i + 1]) {
            fprintf(stderr, "Error: Invalid freq.\n");
            return false;
        }
        freq = atoi(av[i + 1]);
        if (freq > 0) {
            flags->freq = freq;
            return true;
        } else {
            fprintf(stderr, "Error: Invalid freq.\n");
            return false;
        }
    }
    return true;
}

bool check_args(global_t *global, char **av, int ac)
{
    if (ac < 13) {
        fprintf(stderr, "Error: invalid args\nTry \"./zappy_server -help\"\n");
        return false;
    }
    if (!check_port(global->flags, av))
        return false;
    if (!check_width(global->flags, av))
        return false;
    if (!check_height(global->flags, av))
        return false;
    if (!check_name(global, av))
        return false;
    for (int i = 7; i < ac; i++) {
        if (!check_clients_nb(global->flags, av, i))
            return false;
        if (!check_freq(global->flags, av, i))
            return false;
    }
    return true;
}
