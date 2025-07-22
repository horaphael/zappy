/*
** EPITECH PROJECT, 2025
** gui
** File description:
** gui
*/

#include "Gui.hpp"

/**
 * @brief Constructeur de l'interface graphique.
 * Initialise la fenêtre SFML, charge le fond et la police,
 * prépare l'affichage du timer.
 */
Zappy::GUI::GUI() : _window(sf::VideoMode(1500, 1000), "Zappy GUI")
{
    _window.setFramerateLimit(60);
    _network = std::make_unique<Zappy::Network>();
    _sock = std::make_shared<sf::TcpSocket>();

    if (!_backgroundTexture.loadFromFile("./src/GUI/assets/ciel.jpg"))
        std::cerr << "Failed to load GUI background" << std::endl;
    else {
        _backgroundSprite.setTexture(_backgroundTexture);
        float scaleX = static_cast<float>(_window.getSize().x) / _backgroundTexture.getSize().x;
        float scaleY = static_cast<float>(_window.getSize().y) / _backgroundTexture.getSize().y;
        _backgroundSprite.setScale(scaleX, scaleY);
    }
    if (!_font.loadFromFile("./src/GUI/assets/font.ttf"))
        std::cerr << "Failed to load font for timer" << std::endl;
    else {
        _timerText.setFont(_font);
        _timerText.setCharacterSize(18);
        _timerText.setFillColor(sf::Color::White);
        _timerText.setPosition(1240, 110);
    }
}

/**
 * @brief Envoie un message au serveur via le socket.
 * 
 * @param message Message à envoyer.
 * @return true si l'envoi a réussi.
 * @return false si le socket n'existe pas ou en cas d'échec.
 */
bool Zappy::GUI::sendMessage(const std::string& message)
{
    if (!_sock)
        return false;
    return _network->sendMessage(message);
}

/**
 * @brief Connexion au serveur avec IP et port, initialisation du réseau.
 * Envoie le message "GRAPHIC" pour s'identifier comme client GUI.
 * 
 * @param port Port du serveur.
 * @param ip Adresse IP du serveur.
 */
void Zappy::GUI::run_server(int port, std::string ip)
{
    std::cout << "[GUI] Starting run()" << std::endl;
    _network = std::make_unique<Zappy::Network>();
    std::cout << "[GUI] Network instance created" << std::endl;
    _sock = std::make_shared<sf::TcpSocket>();
    std::cout << "[GUI] Socket instance created" << std::endl;
    if (!_network->connectToServer(ip, port, _sock)) {
        std::cerr << "[GUI] Failed to connect to server" << std::endl;
        exit(84);
    }
    std::cout << "[GUI] Connected to server" << std::endl;
    if (sendMessage("GRAPHIC"))
        std::cout << "[GUI] GRAPHIC sent" << std::endl;
    else
        std::cerr << "[GUI] Failed to send GRAPHIC" << std::endl;
}

/**
 * @brief Traite les événements de la fenêtre (ex: fermeture).
 */
void Zappy::GUI::processEvents()
{
    sf::Event event;

    while (_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            _window.close();
            _running = false;
        }
    }
}

/**
 * @brief Met à jour le texte du timer affiché selon le temps écoulé.
 */
void Zappy::GUI::render()
{
    sf::Time gameTime = _gameClock.getElapsedTime();
    std::stringstream ss;
    int seconds = static_cast<int>(gameTime.asSeconds());
    int minutes = seconds / 60;

    seconds %= 60;
    ss << "Time: " << std::setw(2) << std::setfill('0') << minutes << ":" << std::setw(2) << std::setfill('0') << seconds;
    _timerText.setString(ss.str());
}

/**
 * @brief Boucle principale du simulateur.
 * Traite les événements, met à jour la carte, dessine la scène et le timer.
 */
void Zappy::GUI::run_game()
{
    sf::Clock frameClock;
    sf::Time time;

    time += frameClock.restart();
    while (_window.isOpen()) {
        processEvents();
        render();
        for (const auto& [coords, tile] : _parser->getTiles())
            _map->updateTile(coords.first, coords.second, tile.getResources(), _parser->getPlayers(), _parser->getEggs());
        _window.clear(sf::Color::Black);
        _window.draw(_backgroundSprite);
        if (_map)
            _map->draw(_window);
        _window.draw(_timerText);
        _window.display();
    }
}

/**
 * @brief Point d'entrée principal de l'interface graphique.
 * Lance la connexion au serveur, le thread de parsing et la boucle de jeu.
 *
 * @param port Port du serveur.
 * @param ip Adresse IP du serveur.
 */
void Zappy::GUI::run(int port, std::string ip)
{
    run_server(port, ip);
    _parser = std::make_unique<ParseServer>(_sock, _network);
    _parser->start();
    _running = true;
    _parserThread = std::thread([this]() {
        while (_running)
            _parser->run();
    });
    while (!_parser->isMapReady())
        sf::sleep(sf::milliseconds(100));
    int width = _parser->getMapWidth();
    int height = _parser->getMapHeight();
    _map = std::make_unique<Map>(width, height, _window, *_parser);
    run_game();
    _running = false;
    _parser->stop();
    if (_parserThread.joinable())
        _parserThread.join();
}
