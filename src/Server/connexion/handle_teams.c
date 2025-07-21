/*
** EPITECH PROJECT, 2025
** B-YEP-400-RUN-4-1-zappy-julien.kbidi
** File description:
** handle_teams
*/

#include "../../../includes/Server/server.h"

static bool is_valid_team(global_t *global, char *name)
{
    char *token = strtok(name, "\r\n\t ");

    if (!token)
        return false;
    for (UI i = 0; FLAGS->teams_name[i]; i++) {
        if (strcmp(FLAGS->teams_name[i], token) == 0)
            return true;
    }
    return false;
}

static bool check_team_egg(global_t *global, int i, char *name)
{
    for (int i = 0; WORLD->eggs[i]; i++) {
        if ((strcmp(WORLD->eggs[i]->team_name, name) == 0) &&
            (WORLD->eggs[i]->hatched == false)) {
            WORLD->eggs[i]->hatched = true;
            return true;
        }
    }
    dprintf(SERVER->pfds[i].fd, "ko\n");
    return false;
}

static void set_gui_client(global_t *global, int i)
{
    SERVER->gui_client = i;
    SERVER->clients[i]->type = CLIENT_GUI;
    send_msz(global, i);
    send_mct(global, i);
    send_tna(global, i);
    LOG_INFO("Client [%d] [GRAPHIC] connected.", i);
}

static void set_ai_client(global_t *global, int i, const char *name)
{
    player_t *player = create_player(global, name);
    UI egg_nb = count_disponible_eggs(global, name);

    if (!player) {
        dprintf(SERVER->pfds[i].fd, "ko\n");
        LOG_ERROR("Failed to create player for team \"%s\"", name);
        return;
    }
    SERVER->clients[i]->type = CLIENT_IA;
    SERVER->clients[i]->id = player->id;
    LOG_INFO("Client [%d] has joined \"%s\"", i, name);
    dprintf(SERVER->pfds[i].fd, "%d\n%d %d\n",
        egg_nb, FLAGS->width, FLAGS->height);
    add_player_to_world(WORLD, player);
    add_player_to_tile(&WORLD->map[player->pos_y][player->pos_x], player);
    send_pnw(global, player, SERVER->gui_client);
}

void assign_team(global_t *global, char *name, int i)
{
    if (!name)
        return;
    if (strcmp(name, "GRAPHIC") == 0)
        return set_gui_client(global, i);
    if (!is_valid_team(global, name)) {
        LOG_WARN("Client [%d] tried to join invalid team \"%s\"", i, name);
        dprintf(SERVER->pfds[i].fd, "ko\n");
        return;
    }
    if (check_team_egg(global, i, name))
        set_ai_client(global, i, name);
}
