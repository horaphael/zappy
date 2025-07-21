/**
 * @file Network.hpp
 * @brief Déclaration de la classe Network pour la gestion réseau TCP.
 */

#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <string>
#include <iostream>
#include <sstream>
#include <memory>
#include <SFML/Network.hpp>
#include <array>

namespace Zappy
{
    /**
     * @class Network
     * @brief Classe gérant la connexion TCP, l'envoi et la réception de messages.
     */
    class Network
    {
    public:
        /** 
         * @brief Constructeur par défaut.
         */
        Network();

        /**
         * @brief Destructeur.
         */
        ~Network();

        /**
         * @brief Établit une connexion au serveur.
         * @param ip Adresse IP du serveur.
         * @param port Port du serveur.
         * @param sock Pointeur partagé vers le socket TCP.
         * @return true si la connexion a réussi, false sinon.
         */
        bool connectToServer(const std::string &ip, int port, std::shared_ptr<sf::TcpSocket> sock);

        /**
         * @brief Définit le mode bloquant ou non du socket.
         * @param blocking true pour bloquant, false pour non bloquant.
         */
        void setBlocking(bool blocking);

        /**
         * @brief Envoie un message au serveur via le socket TCP.
         * @param message Message à envoyer.
         * @return true si l'envoi a réussi, false sinon.
         */
        bool sendMessage(std::string message);

        /**
         * @brief Reçoit un message depuis le serveur.
         * @param buffer Buffer pour stocker les données reçues.
         * @param nbBytes Nombre d'octets effectivement reçus.
         * @return Statut de la réception (succès, échec, etc.).
         */
        sf::Socket::Status receiveMessage(std::array<char, 4096>& buffer, std::size_t& nbBytes);

        /**
         * @brief Récupère le socket TCP utilisé.
         * @return Pointeur partagé vers le socket.
         */
        std::shared_ptr<sf::TcpSocket> getSocket() const { return _sock; }

    private:
        std::shared_ptr<sf::TcpSocket> _sock; /**< Socket TCP partagé */
        std::string _readBuffer;              /**< Tampon pour les données reçues */
    };
}

#endif /* NETWORK_HPP */
