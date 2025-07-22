/**
 * @file ParseServer.hpp
 * @brief Déclaration de la classe ParseServer
 * 
 * ParseServer est responsable de la réception et du traitement des commandes envoyées par le serveur.
 * Elle met à jour les entités du simulateur (joueurs, œufs, tuiles...) côté graphique.
 */

#ifndef PARSESERVER_HPP
    #define PARSESERVER_HPP

    #include <string>
    #include <map>
    #include <vector>
    #include <memory>
    #include <functional>
    #include <utility>
    #include <iostream>
    #include <sstream>
    #include <algorithm>
    #include <unistd.h>
    #include <cstring>
    #include <SFML/Network.hpp>
    #include "Tile.hpp"
    #include "Player.hpp"
    #include "Network.hpp"
    #include "Egg.hpp"

namespace Zappy
{
    /**
     * @class ParseServer
     * @brief Gère la réception, l'interprétation et le traitement des commandes serveur.
     */
    class ParseServer
    {
        public:
            //----------------- CONSTRUCTOR -----------------//

            /**
             * @brief Constructeur
             * @param sock Socket TCP utilisée pour la communication
             * @param network Objet réseau partagé
             */
            ParseServer(std::shared_ptr<sf::TcpSocket> sock, std::shared_ptr<Zappy::Network> network);

            //----------------- GETTERS -----------------//

            /// Retourne la largeur de la carte
            inline int getMapWidth() const { return _mapWidth; }

            /// Retourne la hauteur de la carte
            inline int getMapHeight() const { return _mapHeight; }

            /// Indique si la carte est prête à être utilisée
            inline bool isMapReady() const { return _mapWidth > 0 && _mapHeight > 0 && !_tiles.empty(); }

            /// Retourne toutes les tuiles
            inline const std::map<std::pair<int, int>, Tile>& getTiles() const { return _tiles; }

            /// Retourne tous les joueurs connus
            inline const std::map<int, std::shared_ptr<Player>>& getPlayers() const { return _players; }

            /// Retourne tous les œufs en attente d’éclosion
            inline const std::map<int, std::shared_ptr<Egg>>& getEggs() const { return _eggs; }

            /// Retourne les noms des équipes enregistrées
            inline const std::vector<std::string>& getTeamNames() const { return _teamNames; }

            //----------------- CONTROL -----------------//

            /// Démarre l'exécution de la boucle principale
            inline void start() { _running = true; }

            /// Arrête l'exécution de la boucle principale
            inline void stop() { _running = false; }

            //----------------- MAIN LOOP -----------------//

            /**
             * @brief Boucle principale de lecture et traitement des commandes
             */
            void run();

            //----------------- NETWORK AND PARSING -----------------//

            /**
             * @brief Récupère et traite les commandes entrantes
             */
            void getCommand();

            /**
             * @brief Retourne le nom d'une ressource à partir de son ID
             */
            std::string getResourceName(int id);

            /**
             * @brief Découpe une ligne de commande en arguments
             */
            std::vector<std::string> split(const std::string &line);

            /**
             * @brief Traite une commande brute envoyée par le serveur
             */
            void processCommand(const std::string& rawCommand);

            /**
             * @brief Initialise la table de correspondance commande → handler
             */
            void initCommandMap();

            /**
             * @brief Met à jour une tuile spécifique
             */
            void updateTile(int x, int y);

            //----------------- COMMAND HANDLERS GRAPHIC -----------------//

            /// Gère la commande `msz` (taille de la carte)
            void handleMsz(const std::vector<std::string>& args);

            /// Gère la commande `bct` (contenu d'une tuile)
            void handleBct(const std::vector<std::string>& args);

            /// Gère la commande `mct` (contenu de toutes les tuiles)
            void handleMct(const std::vector<std::string>& args);

            /// Gère la commande `tna` (noms des équipes)
            void handleTna(const std::vector<std::string>& args);

            /// Gère l’arrivée d’un nouveau joueur
            void handlePnw(const std::vector<std::string>& args);

            void handlePpo(const std::vector<std::string>& args); ///< Position joueur
            void handlePlv(const std::vector<std::string>& args); ///< Niveau joueur
            void handlePin(const std::vector<std::string>& args); ///< Inventaire joueur
            void handlePex(const std::vector<std::string>& args); ///< Expulsion joueur
            void handlePbc(const std::vector<std::string>& args); ///< Broadcast
            void handlePic(const std::vector<std::string>& args); ///< Début incantation
            void handlePie(const std::vector<std::string>& args); ///< Fin incantation
            void handlePfk(const std::vector<std::string>& args); ///< Fork (œuf)
            void handlePdr(const std::vector<std::string>& args); ///< Drop ressource
            void handlePgt(const std::vector<std::string>& args); ///< Pickup ressource
            void handlePdi(const std::vector<std::string>& args); ///< Mort joueur
            void handleEnw(const std::vector<std::string>& args); ///< Nouvel œuf
            void handleEbo(const std::vector<std::string>& args); ///< Éclosion œuf
            void handleEdi(const std::vector<std::string>& args); ///< Mort œuf
            void handleSgt(const std::vector<std::string>& args); ///< Fréquence de temps
            void handleSst(const std::vector<std::string>& args); ///< Modif fréquence
            void handleSeg(const std::vector<std::string>& args); ///< Victoire
            void handleSmg(const std::vector<std::string>& args); ///< Message serveur
            void handleSuc(const std::vector<std::string>& args); ///< Commande invalide
            void handleSbp(const std::vector<std::string>& args); ///< Commande inconnue

        private:
            int _mapWidth = 0; ///< Largeur de la carte
            int _mapHeight = 0; ///< Hauteur de la carte
            bool _running = false; ///< Indique si la boucle tourne
            bool _mapReady = false; ///< Carte initialisée
            int _bctNb = 0; ///< Compteur de tuiles reçues
            bool _gameOver = false; ///< Partie terminée
            int _timeUnit = 100; ///< Unité de temps serveur
            std::string _winningTeam; ///< Équipe gagnante
            std::map<std::pair<int, int>, Tile> _tiles; ///< Carte des tuiles
            std::map<int, std::shared_ptr<Player>> _players; ///< Liste des joueurs
            std::map<int, std::shared_ptr<Egg>> _eggs; ///< Liste des œufs
            std::vector<std::string> _teamNames; ///< Noms des équipes
            std::map<std::string, std::function<void(const std::vector<std::string>&)>> _commands; ///< Commandes serveur
            std::shared_ptr<sf::TcpSocket> _sock; ///< Socket TCP
            std::shared_ptr<Zappy::Network> _network; ///< Réseau
            std::string _readBuffer; ///< Buffer de lecture
    };
}

#endif /* !PARSESERVER_HPP */
