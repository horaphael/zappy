# 📡 network_lib

Une bibliothèque réseau simple en C pour créer des serveurs TCP multi-clients avec `poll()`.  
Elle permet de gérer les connexions, les déconnexions et la réception de données, avec un système de logs colorés intégré.

---

## 🔧 Fonctionnalités

- Création de serveurs TCP avec adresse IP, port et taille de buffer personnalisables
- Support multi-clients (jusqu’à `MAX_CLIENTS`)
- Callbacks utilisateur pour :
  - Connexion client
  - Réception de données
  - Déconnexion client
- Logs colorés (`INFO`, `DEBUG`, `WARN`, `ERROR`)
- Utilise `poll()` pour la gestion des événements

---

## 📦 Structure du projet
network_lib/
├── include/
│ ├── net.h # Déclarations principales
│ └── log.h # Système de log
├── src/
│ ├── net_server.c # Création et gestion du serveur
│ ├── net_client.c # Gestion des clients
│ ├── net_poll.c # Système de poll() pour les sockets
│ ├── net_utils.c # Fonctions utilitaires (send, log, close)
│ └── handlers.c # Définition des callbacks utilisateurs
├── main.c # Exemple de serveur


---

## 🛠️ Compilation

Compilez avec :

<!-- ```bash -->
# gcc -o server main.c src/*.c -Iinclude/

| Fonction                                     | Description                      |
| -------------------------------------------- | -------------------------------- |
| `net_server_create(ip, port, bufsize)`       | Crée un serveur TCP              |
| `net_server_start(server)`                   | Lance le serveur                 |
| `net_server_poll(server, timeout)`           | Gère les I/O avec poll()         |
| `net_server_destroy(server)`                 | Libère toutes les ressources     |
| `set_handle_connection(server, cb, args)`    | Callback de connexion client     |
| `set_handle_disconnection(server, cb, args)` | Callback de déconnexion client   |
| `set_handle_data(server, cb, args)`          | Callback de réception de données |
| `net_send(fd, msg)`                          | Envoie un message à un client    |
| `LOG_INFO(...)`, `LOG_ERROR(...)`, etc.      | Macros de logs colorés           |


🚀 Exemple d'utilisation

#include "include/net.h"
#include "include/log.h"

void handle_client_connect(net_client_t *client, net_server_t *server, void *args)
{
    char *cmd = (char *)args;

    (void)server;
    LOG_INFO("New client connected: fd=%d", client->fd);
    LOG_INFO("CMD: %s", cmd);
}

void handle_client_disconnect(net_client_t *client, net_server_t *server, void *args)
{
    char *cmd = (char *)args;

    (void)server;
    LOG_INFO("Client disconnected: fd=%d", client->fd);
    LOG_INFO("CMD: %s", cmd);
}

void handle_client_data(net_client_t *client, net_server_t *server, void *args)
{
    char *cmd = (char *)args;

    (void)server;
    LOG_INFO("Data from fd=%d: %s", client->fd, client->buffer);
    LOG_INFO("CMD: %s", cmd);
}

int main(void)
{
    net_server_t *server = net_server_create("127.0.0.1", 4243, 1024);
    int timeout = 0;
    char data_command[256] = "Commande reçu";
    char data_disconnect_command[256] = "Commande disconnection reçu";
    char data_connect_command[256] = "Commande connection reçu";

    if (!server)
        return 84;
    set_handle_connection(server, handle_client_connect, data_connect_command);
    set_handle_disconnection(server, handle_client_disconnect, data_disconnect_command);
    set_handle_data(server, handle_client_data, data_command);

    // set_handle_connection(server, NULL);
    // set_handle_disconnection(server, NULL);
    // set_handle_data(server, NULL);
    if (!net_server_start(server))
        return 84;
    while (server->running) {
        net_server_poll(server, timeout);
    }
    net_server_destroy(server);
    return 0;
}

# Démarrer le serveur
./server

# Depuis un autre terminal
nc 127.0.0.1 4243
# Tapez du texte, vous verrez les logs côté serveur
