/*
** EPITECH PROJECT, 2025
** B-YEP-400-RUN-4-1-zappy-julien.kbidi
** File description:
** zappy_server
*/

#include "../../includes/Server/server.h"

static void close_socket(const char *message, int fd)
{
    if (message)
        perror(message);
    close(fd);
}

static int create_server_socket(int port)
{
    struct sockaddr_in servaddr;
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);

    if ((listenfd) < 0) {
        perror("socket");
        return -1;
    }
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        close_socket("bind", listenfd);
        return -1;
    }
    if (listen(listenfd, 10) < 0) {
        close_socket("listen", listenfd);
        return -1;
    }
    return listenfd;
}

static bool init_server(global_t *global)
{
    SERVER->listenfd = create_server_socket(global->flags->port);
    if (SERVER->listenfd < 0)
        return false;
    SERVER->pfds[0].fd = SERVER->listenfd;
    SERVER->pfds[0].events = POLLIN;
    for (int i = 1; i < MAX_CLIENTS + 1; i++)
        SERVER->pfds[i].fd = -1;
    return true;
}

static void handler_sig(int sig)
{
    signal(sig, SIG_IGN);
    printf("Server shutting down.\n");
    exit(0);
}

static void handler_sig_pipe(__attribute_maybe_unused__ int sig)
{
    LOG_DEBUG("Deleted player from world.");
}

static void run_server(global_t *global)
{
    signal(SIGINT, handler_sig);
    signal(SIGPIPE, handler_sig_pipe);
    while (SERVER->running) {
        poll_connexion(global, &SERVER->nfds);
    }
}

static bool fill_world_struct(global_t *global)
{
    UI width = FLAGS->width;
    UI height = FLAGS->height;

    WORLD->teams = init_teams(global);
    if (!WORLD->teams)
        return false;
    WORLD->map = init_map(height, width);
    if (!WORLD->map)
        return false;
    WORLD->eggs = init_eggs(global);
    if (!WORLD->eggs)
        return false;
    WORLD->food_density = (UI)(width * height * 0.5);
    WORLD->linemate_density = (UI)(width * height * 0.3);
    WORLD->deraumere_density = (UI)(width * height * 0.15);
    WORLD->sibur_density = (UI)(width * height * 0.1);
    WORLD->mendiane_density = (UI)(width * height * 0.1);
    WORLD->phiras_density = (UI)(width * height * 0.08);
    WORLD->thystame_density = (UI)(width * height * 0.05);
    return true;
}

int zappy_server(global_t *global, char **av, int ac)
{
    if (!check_args(global, av, ac)) {
        free_all(global);
        return 84;
    }
    if (!fill_world_struct(global)) {
        free_all(global);
        return 84;
    }
    if (!init_server(global)) {
        free_all(global);
        return 84;
    }
    spawn_resources(WORLD, FLAGS->height, FLAGS->width);
    print_settings(global);
    run_server(global);
    free_all(global);
    return 0;
}
