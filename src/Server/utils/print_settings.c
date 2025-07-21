/*
** EPITECH PROJECT, 2025
** B-YEP-400-RUN-4-1-zappy-julien.kbidi
** File description:
** print_settings
*/

#include "server.h"

void print_settings(global_t *global)
{
    printf("===================Zappy Server===================\n");
    printf("port = %d\n", global->flags->port);
    printf("width = %d\n", global->flags->width);
    printf("height = %d\n", global->flags->height);
    printf("clients_nb = %d\n", global->flags->clientsNb);
    printf("freq = %d\n", global->flags->freq);
    printf("Nb Teams: [%d]\n\n", global->world->nb_teams);
    for (UI i = 0; i < global->world->nb_teams; i++) {
        printf("Team name: \"%s\"\n", global->flags->teams_name[i]);
        printf("Nb player: [0]\n");
        printf("Nb eggs: [%d]\n\n", global->flags->clientsNb);
    }
    printf("verbose = 0\n");
    printf("match duration = 0.00 seconds\n");
    printf("==================================================\n");
}

void display_map(tile_t **map, UI width, UI height)
{
    for (UI j = 0; j < height; j++){
        for (UI i = 0; i < width; i++){
            printf("%d ", map[j][i].id);
        }
        printf("\n");
    }
    printf("\n");
}
