/*
** EPITECH PROJECT, 2025
** network
** File description:
** network
*/

#include "Network.hpp"

/**
 * @brief Constructeur de la classe Network.
 * 
 * Initialise la socket TCP partagée.
 */
Zappy::Network::Network()
{
    _sock = std::make_shared<sf::TcpSocket>();
    std::cout << "[Network] Socket initialized" << std::endl;
}

/**
 * @brief Destructeur de la classe Network.
 * 
 * Déconnecte la socket si elle est toujours active.
 */
Zappy::Network::~Network()
{
    if (_sock) {
        _sock->disconnect();
        std::cout << "[Network] Socket disconnected" << std::endl;
    }
}

/**
 * @brief Définit si la socket doit être bloquante ou non.
 * 
 * @param blocking True pour bloquant, false pour non bloquant.
 */
void Zappy::Network::setBlocking(bool blocking)
{
    _sock->setBlocking(blocking);
}

/**
 * @brief Envoie un message via la socket TCP.
 * 
 * @param message Message à envoyer.
 * @return true Si l'envoi a réussi.
 * @return false Sinon.
 */
bool Zappy::Network::sendMessage(std::string message)
{
    if (_sock->send(message.c_str(), message.size()) == sf::Socket::Done) {
        return true;
    } else {
        std::cerr << "[GUI] Failed to send message: " << message << std::endl;
        return false;
    }
}

/**
 * @brief Reçoit un message depuis la socket TCP.
 * 
 * @param buffer Buffer où stocker les données reçues.
 * @param nbBytes Nombre d'octets reçus.
 * @return sf::Socket::Status Statut de la réception.
 */
sf::Socket::Status Zappy::Network::receiveMessage(std::array<char, 4096>& buffer, std::size_t& nbBytes)
{
    sf::Socket::Status status = _sock->receive(buffer.data(), buffer.size(), nbBytes);

    if (status == sf::Socket::NotReady)
        return sf::Socket::NotReady;
    return status;
}

/**
 * @brief Connecte la socket au serveur donné.
 * 
 * @param ip Adresse IP du serveur.
 * @param port Port du serveur.
 * @param sock Socket TCP partagée à utiliser.
 * @return true Si la connexion a réussi.
 * @return false Sinon.
 */
bool Zappy::Network::connectToServer(const std::string &ip, int port, std::shared_ptr<sf::TcpSocket> sock)
{
    _sock = sock;
    sf::IpAddress ipp(ip);
    if (_sock->connect(ipp, port) != sf::Socket::Done) {
        std::cerr << "Connection failed" << std::endl;
        return false;
    }
    _sock->setBlocking(false);
    return true;
}
