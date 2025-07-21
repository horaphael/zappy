/*
** EPITECH PROJECT, 2025
** B-YEP-400-RUN-4-1-zappy-julien.kbidi
** File description:
** server
*/

#ifndef SERVER_H_
    #define SERVER_H_

    #include <unistd.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdbool.h>
    #include <poll.h>
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <signal.h>

    /* MACROS */
    #define RED     "\033[1;31m"
    #define GREEN   "\033[1;32m"
    #define YELLOW  "\033[1;33m"
    #define BLUE    "\033[1;34m"
    #define RESET   "\033[0m"

    #define LOG_INFO(fmt, ...)  fprintf(stdout, BLUE "[INFO]" RESET " " fmt "\n", ##__VA_ARGS__)
    #define LOG_DEBUG(fmt, ...) fprintf(stdout, GREEN "[DEBUG]" RESET " " fmt "\n", ##__VA_ARGS__)
    #define LOG_WARN(fmt, ...)  fprintf(stderr, YELLOW "[WARN]" RESET " " fmt "\n", ##__VA_ARGS__)
    #define LOG_ERROR(fmt, ...) fprintf(stderr, RED "[ERROR]" RESET " " fmt "\n", ##__VA_ARGS__)

    #define MAX_CLIENTS 100
    #define MAX_CLIENT_INDEX (MAX_CLIENTS + 1)
    #define MAX_MAP_SIZE 42
    #define NB_RESSOURCES 7
    #define SERVER global->server
    #define FLAGS global->flags
    #define WORLD global->world

    #define UI unsigned int

// --------- ENUM ---------
typedef enum direction_e {
    NORTH = 1,
    EAST = 2,
    SOUTH = 3,
    WEST = 4,
} direction_t;

typedef enum client_type_e {
    CLIENT_IA,
    CLIENT_GUI,
    CLIENT_UNKNOW,
} client_type_t;

// --------- STRUCT ---------
typedef struct client_s {
    int id;
    client_type_t type;
    bool alive;
} client_t;

typedef struct server_s {
    struct pollfd pfds[MAX_CLIENTS + 1];
    int listenfd;
    int nfds;
    bool running;
    client_t *clients[MAX_CLIENTS];
    int gui_client;
} server_t;

typedef struct egg_s {
    UI x;
    UI y;
    UI id;
    char *team_name;
    bool hatched;
} egg_t;

typedef struct elevation_s {
    UI level;
    UI nb_players;
    UI ressources[NB_RESSOURCES];
} elevation_t;

typedef struct flags_s {
    UI port;
    UI width;
    UI height;
    char **teams_name;
    UI clientsNb;
    UI freq;
} flags_t;

typedef struct player_s {
    UI id;
    UI pos_x;
    UI pos_y;
    direction_t direction;
    UI level;
    UI ressources[NB_RESSOURCES];
    char *team_name;
    struct player_s *next;
} player_t;

typedef struct team_s {
    char *team_name;
    UI total_place;
    UI available_place;
} team_t;

typedef struct tile_s {
    UI id;
    UI x;
    UI y;
    UI ressources[NB_RESSOURCES];
    player_t **players;
} tile_t;

typedef struct world_s {
    tile_t **map;
    UI nb_teams;
    player_t *players;
    egg_t **eggs;
    team_t **teams;
    UI food_density;
    UI linemate_density;
    UI deraumere_density;
    UI sibur_density;
    UI mendiane_density;
    UI phiras_density;
    UI thystame_density;
} world_t;

typedef struct global_s {
    server_t *server;
    flags_t *flags;
    world_t *world;
} global_t;

typedef struct ia_command_s {
    const char *name;
    void (*command)(global_t *global, int client_id, char *args);
} ia_command_t;

typedef struct pos_s {
    int x;
    int y;
} pos_t;

// --------- FUNCTIONS ---------
/* INIT STRUCT */
global_t *init_global_struct(void);
tile_t **init_map(UI height, UI width);
egg_t **init_eggs(global_t *global);
client_t *init_client(int index);
team_t **init_teams(global_t *global);

/* ZAPPY SERVER */
int zappy_server(global_t *global, char **av, int ac);

/* CHECK ARGS */
bool check_args(global_t *global, char **av, int ac);

/* PRINT SETTINGS */
void print_settings(global_t *global);
void display_map(tile_t **map, UI width, UI height);

/* CONNEXION */
int accept_connexion(global_t *global, int *nfds);
void poll_connexion(global_t *global, int *nfds);

/* PLAYERS */
void add_player_to_world(world_t *world, player_t *new_player);
player_t *get_player_by_client_id(global_t *global, int client_id);
player_t *create_player(global_t *global, const char *team_name);

/* EGGS */
void add_egg_to_world(world_t *world, egg_t *egg);
egg_t *laid_egg(player_t *players);
unsigned int count_disponible_eggs(global_t *global, const char *name);

/* HANDLE */
    // Clients
bool handle_client_data(global_t *global, int i);
void handle_ia_command(global_t *global, int client_id, char *input);
void disconnect_client(global_t *global, int i, int *nfds);
void broadcast_to_other_clients(global_t *global, const char *buff, int len,
    int sender_index);
    // Teams
int get_team_index(global_t *global, const char *team_name);
void assign_team(global_t *global, char *team_name, int i);

/* HANDLE RESOURCES */
void spawn_resources(world_t *world, UI height, UI width);
int get_ressource_id(char *input);
void reset_resources(global_t *global);

/* FREE */
void free_all(global_t *global);

/* UTILS */
void check_direction(player_t *players, flags_t *flags);
bool check_incantation(global_t *global, player_t *players);
bool has_required_players(tile_t *tile, UI level, UI req_count);
bool has_required_ressources(tile_t *tile, const UI required[NB_RESSOURCES]);
void apply_incantation(tile_t *tile, elevation_t elevation);
void remove_player_from_tile(tile_t *tile, player_t *player);
void add_player_to_tile(tile_t *tile, player_t *player);
char *get_tile_content(tile_t *tile);
const char *get_ressource_name(int i);

/* COMMANDS AI */
void cmd_connect_nbr(global_t *global, int client_id, char *args);
void cmd_fork(global_t *global, int client_id, char *args);
void cmd_incantation(global_t *global, int client_id, char *args);
void cmd_forward(global_t *global, int client_id, char *args);
void cmd_inventory(global_t *global, int client_id, char *args);
void cmd_turn(global_t *global, int client_id, char *args);
void cmd_inventory(global_t *global, int client_id, char *args);
void cmd_take(global_t *global, int client_id, char *args);
void cmd_set(global_t *global, int client_id, char *args);
void cmd_look(global_t *global, int client_id, char *args);
void cmd_broadcast(global_t *global, int client_id, char *args);

/* COMMANDS GUI */
void send_msz(global_t *global, UI index);
void send_bct(global_t *global, UI index, int i_h, int i_w);
void send_mct(global_t *global, UI index);
void send_tna(global_t *global, UI index);
void send_pnw(global_t *global, player_t *player, UI index);
void send_ppo(global_t *global, player_t *player, UI index);
void send_plv(global_t *global, player_t *player, UI index);
void send_pin(global_t *global, player_t *player, UI index);
void send_pex(global_t *global, player_t *player, UI index);
void send_pbc(global_t *global, player_t *player, UI index, const char *msg);


void send_pfk(global_t *global, player_t *player, UI index);
void send_pdr(global_t *global, player_t *player, UI index, int res_idx);
void send_pgt(global_t *global, player_t *player, UI index, int res_idx);
void send_pdi(global_t *global, player_t *player, UI index);
void send_enw(global_t *global, player_t *player, egg_t *eggs, UI index);




void send_seg(global_t *global, team_t *team, UI index);
void send_smg(global_t *global, UI index, const char *msg);
void send_suc(global_t *global, UI index);
void send_sbp(global_t *global, UI index);

#endif /* !SERVER_H_ */
