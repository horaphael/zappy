/*
** EPITECH PROJECT, 2025
** network_lib
** File description:
** net
*/

#ifndef NET_H_
    #define NET_H_

    #define MAX_CLIENTS 128
    #define BUFFER_SIZE 1024

    #include <unistd.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <stdbool.h>
    #include <string.h>
    #include <errno.h>
    #include <poll.h>
    #include <sys/socket.h>
    #include <netinet/in.h>



            /* ================== STRUCT ================== */

/**
 * @brief Représente un client réseau connecté au serveur.
 */
typedef struct net_client_s {
    int fd;                         // Descripteur de socket du client
    bool active;                    // Indique si le client est actif
    char buffer[BUFFER_SIZE];       // Tampon de réception des données
} net_client_t;

/**
 * @brief Contient les informations et états du serveur.
 */
typedef struct net_server_s {
    int listen_fd; // Socket d'écoute
    unsigned int port; // Port TCP utilisé par le serveur
    struct pollfd pfds[MAX_CLIENTS]; // Tableau de pollfd pour la surveillance
    net_client_t clients[MAX_CLIENTS]; // Tableau des clients connectés
    bool running; // Indique si le serveur est en cours d’exécution
    void (*on_data)(net_client_t *client, struct net_server_s *server, void *args); // Function to handle incomming data = Null by default
    void (*on_disconnect)(net_client_t *client, struct net_server_s *server, void *args); // Function to handle disconnection of clients = Null by default
    void (*on_connect)(net_client_t *client, struct net_server_s *server, void *args); // Function to handle connection of clients = Null by default
} net_server_t;



            /* ================== CORE ================== */

/**
 * @brief Crée une instance de serveur réseau.
 *
 * @param port Le port TCP sur lequel le serveur doit écouter.
 * @return Un pointeur vers une structure net_server_t initialisée
 * , ou NULL en cas d’échec.
 */
net_server_t *net_server_create(const char *host, unsigned int port);

/**
 * @brief Démarre le serveur (bind + listen).
 *
 * @param server Un pointeur vers le serveur initialisé.
 * @return true si le démarrage a réussi, false sinon.
 */
bool net_server_start(net_server_t *server);

/**
 * @brief Lance la boucle de traitement des événements (poll).
 *
 * @param server Le serveur dont on veut traiter les événements.
 */
void net_server_poll(net_server_t *server, int poll_timeout);

/**
 * @brief Arrête proprement le serveur (arrête la boucle).
 *
 * @param server Le serveur à arrêter.
 */
void net_server_stop(net_server_t *server);

/**
 * @brief Libère les ressources allouées pour le serveur.
 *
 * @param server Le serveur à détruire.
 */
void net_server_destroy(net_server_t *server);



            /* ================== UTILS ================== */

/**
 * @brief Envoie un message via un socket.
 *
 * @param fd Descripteur de fichier du socket.
 * @param msg Chaîne de caractères à envoyer.
 */
void net_send(int fd, const char *msg);

/**
 * @brief Ferme proprement un socket avec message d’erreur.
 *
 * @param msg Message d’erreur à afficher.
 * @param socket Descripteur du socket à fermer.
 */
void close_socket(char *msg, int socket);



            /* ================== CLIENTS ================== */

/**
 * @brief Initialise tous les clients comme inactifs.
 *
 * @param server Le serveur dont on initialise les clients.
 */
void init_clients_array(net_server_t *server);

/**
 * @brief Ferme la connexion d’un client donné.
 *
 * @param server Le serveur auquel appartient le client.
 * @param fd Le descripteur de fichier du client à fermer.
 */
void net_close_client(net_server_t *server, int fd);

/**
 * @brief Ferme toutes les connexions clients actives.
 *
 * @param server Le serveur dont on veut fermer tous les clients.
 */
void net_close_all_clients(net_server_t *server);



            /* ================== POLL ================== */

/**
 * @brief Initialise le tableau de pollfd pour surveiller les sockets.
 *
 * @param server Le serveur dont on initialise les pollfds.
 */
void init_poll_fds(net_server_t *server);



            /* ================== HANDLERS ================== */

/**
 * @brief Gère la déconnexion d’un client.
 *
 * @param fd Le descripteur du client déconnecté.
 */
void handle_disconnect(int fd);

/**
 * @brief Gère une nouvelle connexion entrante.
 *
 * @param fd Le descripteur du client connecté.
 */
void handle_connect(int fd);

/**
 * @brief Gère les données reçues d’un client.
 *
 * @param fd Le descripteur du client.
 * @param data Les données reçues.
 */
void handle_data(int fd, char *data);

void set_handle_data(net_server_t *server, void (*on_data)
        (net_client_t *client, struct net_server_s *server, void *args));

void set_handle_connection(net_server_t *server, void (*on_connect)
        (net_client_t *client, struct net_server_s *server, void *args));

void set_handle_disconnection(net_server_t *server, void (*on_connect)
        (net_client_t *client, struct net_server_s *server, void *args));

#endif /* NET_H_ */
