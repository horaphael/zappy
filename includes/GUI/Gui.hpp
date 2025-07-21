/**
 * @file GUI.hpp
 * @brief Déclaration de la classe GUI qui gère l'interface graphique principale du client Zappy.
 */

#ifndef GUI_HPP_
    #define GUI_HPP_

    #include <iostream>
    #include <string.h>
    #include <SFML/Graphics.hpp>
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <sstream>
    #include <thread>
    #include <memory>
    #include <SFML/Network.hpp>
    #include "Map.hpp"
    #include "Player.hpp"
    #include "Egg.hpp"
    #include "Network.hpp"
    #include <iomanip>

namespace Zappy
{
    /**
     * @class GUI
     * @brief Classe principale pour gérer l'interface graphique de la simulation Zappy.
     */
    class GUI
    {
        public:
            /**
             * @brief Constructeur par défaut.
             */
            GUI();

            /**
             * @brief Destructeur par défaut.
             */
            ~GUI() = default;

            /**
             * @brief Lance la boucle principale avec la connexion au serveur.
             * @param port Port du serveur
             * @param ip Adresse IP du serveur
             */
            void run(int port, std::string ip);

            /**
             * @brief Établit la connexion réseau avec le serveur.
             * @param port Port du serveur
             * @param ip Adresse IP du serveur
             */
            void run_server(int port, std::string ip);

            /**
             * @brief Lance la boucle de la simulation et le rendu de la carte.
             */
            void run_game();

            /**
             * @brief Gère l'affichage graphique à chaque frame.
             */
            void render();

            /**
             * @brief Traite les événements utilisateur (clavier, souris...).
             */
            void processEvents();

            /**
             * @brief Envoie un message au serveur via la socket TCP.
             * @param message Message à envoyer
             * @return true si l'envoi a réussi, false sinon
             */
            bool sendMessage(const std::string& message);

        private:
            std::shared_ptr<sf::TcpSocket> _sock;             ///< Socket TCP pour la communication réseau
            std::shared_ptr<Network> _network;                ///< Objet réseau pour la communication
            bool _connected = false;                          ///< Indique si la connexion est établie
            int _mapWidth = 10;                               ///< Largeur de la carte (valeur par défaut)
            int _mapHeight = 10;                              ///< Hauteur de la carte (valeur par défaut)
            std::unique_ptr<Map> _map;                        ///< Carte de la simulation affichée
            std::unique_ptr<ParseServer> _parser;             ///< Parseur pour traiter les messages du serveur
            std::thread _parserThread;                        ///< Thread séparé pour le parsing
            bool _running;                                    ///< Indique si la boucle principale est en cours
            sf::RenderWindow _window;                         ///< Fenêtre de rendu SFML
            sf::Texture _backgroundTexture;                   ///< Texture de fond
            sf::Sprite _backgroundSprite;                     ///< Sprite de fond
            sf::Font _font;                                   ///< Police pour l'affichage de texte
            sf::Text _timerText;                              ///< Texte affichant un minuteur
            sf::Clock _gameClock;                             ///< Horloge pour gérer le temps écoulé
    };
}

#endif /* !GUI_HPP_ */
