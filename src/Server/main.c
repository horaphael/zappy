/*
** EPITECH PROJECT, 2025
** B-YEP-400-RUN-4-1-zappy-julien.kbidi
** File description:
** main
*/

#include "server.h"

static int disp_usage(void)
{
    printf("USAGE: ./zappy_server -p port -x width -y height -n name1 name2");
    printf(" ... -c clientsNb -f freq\n");
    printf("\tport\t\tport number\n");
    printf("\twidth\t\twidth of the world (max %d)\n", MAX_MAP_SIZE);
    printf("\theight\t\theight of the world (max %d)\n", MAX_MAP_SIZE);
    printf("\tname\t\tname of the team\n");
    printf("\tclientsNb\tnumber of authorized clients per team\n");
    printf("\tfreq\t\treciprocal of time unit for execution of actions\n");
    return 84;
}

int main(int ac, char **av)
{
    global_t *global_struct = NULL;

    if ((ac == 2) && (strcmp(av[1], "-help") == 0))
        return disp_usage();
    global_struct = init_global_struct();
    if (!global_struct)
        return 84;
    return zappy_server(global_struct, av, ac);
}
