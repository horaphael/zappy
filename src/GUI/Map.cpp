/*
** EPITECH PROJECT, 2025
** map
** File description:
** map
*/

#include "Map.hpp"
#include "Player.hpp"
#include "Egg.hpp"

/**
 * @brief Constructeur de la map.
 * 
 * Initialise la taille de la map, la fenêtre SFML,
 * charge la texture des tiles, calcule la taille et position des tiles,
 * et crée la grille des tiles.
 * 
 * @param width Largeur de la map (nombre de tiles).
 * @param height Hauteur de la map (nombre de tiles).
 * @param window Référence à la fenêtre SFML où dessiner.
 * @param parseServer Référence au parseur serveur contenant les données.
 */
Zappy::Map::Map(int width, int height, sf::RenderWindow &window, const Zappy::ParseServer &parseServer) : _width(width), _height(height), _window(window), _parseServer(parseServer)
{
    sf::Vector2u windowSize = window.getSize();
    int popupMargin = 400;
    int tileWidth = (windowSize.x - popupMargin) / width;
    int tileHeight = windowSize.y / height;
    std::shared_ptr<Tile> tile;

    if (!_tileTexture.loadFromFile("./src/GUI/assets/tile_triso.png"))
        std::cerr << "Failed to load tile texture" << std::endl;
    _tileSize = std::min(tileWidth, tileHeight);
    _offsetX = 30;
    _offsetY = (windowSize.y - (_tileSize * _height)) / 2;
    _tiles.reserve(height);
    for (int y = 0; y < height; y++) {
        std::vector<std::shared_ptr<Tile>> row;
        row.reserve(width);
        for (int x = 0; x < width; x++) {
            tile = std::make_shared<Tile>(x, y, _tileSize);
            tile->setPosition(x * _tileSize + _offsetX, y * _tileSize + _offsetY);
            row.emplace_back(tile);
        }
        _tiles.push_back(row);
    }
}

/**
 * @brief Dessine la map, les tiles, les ressources, les joueurs, les oeufs,
 *        ainsi qu'une fenêtre d'information sur la map.
 * 
 * @param window Référence à la fenêtre SFML où dessiner.
 */
void Zappy::Map::draw(sf::RenderWindow &window) const
{
    sf::Sprite tileSprite;
    tileSprite.setTexture(_tileTexture);
    tileSprite.setScale(static_cast<float>(_tileSize) / _tileTexture.getSize().x, static_cast<float>(_tileSize) / _tileTexture.getSize().y);
    int total = 0;
    std::stringstream infoText;

    for (auto row : _tiles) {
        for (auto tile : row) {
            tileSprite.setPosition(tile->getShape().getPosition());
            window.draw(tileSprite);
            window.draw(tile->getShape());
            total += tile->drawResources(window);
        }
    }
    infoText << "    INFORMATION OF MAP:\n";
    infoText << " Width: " << _width << "               Height: " << _height << "\n";
    infoText << "     Total Resources: " << total << "\n\n";
    infoText << "\n\t\t\t  --- TEAMS ---\n";
    const auto& teamNames = _parseServer.getTeamNames();
    for (const auto& team : teamNames) {
        int teamPlayerCount = 0;
        for (const auto& [id, player] : _parseServer.getPlayers()) {
            if (player->getTeamName() == team)
                teamPlayerCount++;
        }
        infoText << team << ": " << teamPlayerCount << " players\n";
    }
    static sf::Font font;
    font.loadFromFile("./src/GUI/assets/font.ttf");
    sf::RectangleShape popupWindow(sf::Vector2f(350, 200));
    popupWindow.setFillColor(sf::Color(0, 0, 0, 180));
    popupWindow.setOutlineColor(sf::Color::White);
    popupWindow.setOutlineThickness(1);
    popupWindow.setPosition(window.getSize().x - 380, 40);
    sf::Text popup(infoText.str(), font, 18);
    popup.setFillColor(sf::Color::White);
    popup.setPosition(window.getSize().x - 320, 40);
    popup.setOutlineColor(sf::Color::Black);
    popup.setOutlineThickness(1.f);
    window.draw(popupWindow);
    window.draw(popup);
    if (!_parseServer.getPlayers().empty())
        _parseServer.getPlayers().begin()->second->triggerBroadcast();
    for (const auto &[id, player] : _parseServer.getPlayers())
        player->draw(window, _tileSize, _offsetX, _offsetY);
    for (const auto& [id, egg] : _parseServer.getEggs()) {
        int x = egg->getX();
        int y = egg->getY();
        if (x >= 0 && x < _width && y >= 0 && y < _height) {
            float posX = x * _tileSize + _offsetX + _tileSize / 2 - 10;
            float posY = y * _tileSize + _offsetY + _tileSize / 2 - 10;
            egg->draw(window, posX, posY);
        }
    }
    checkHoverTile(window);
}

/**
 * @brief Met à jour les ressources, joueurs et oeufs présents sur une tile donnée.
 * 
 * @param x Coordonnée x de la tile.
 * @param y Coordonnée y de la tile.
 * @param resources Map des ressources présentes sur la tile.
 * @param _players Map des joueurs présents.
 * @param _eggs Map des oeufs présents.
 */
void Zappy::Map::updateTile(int x, int y, const std::map<std::string, int> &resources, std::map<int, std::shared_ptr<Player>> _players, std::map<int, std::shared_ptr<Egg>> _eggs)
{
    if (x >= 0 && x < _width && y >= 0 && y < _height)
        _tiles[y][x]->setResources(resources);
    else
        std::cerr << "Invalid tile coordinates: (" << x << "," << y << ")" << std::endl;
    for (const auto& [id, player] : _players) {
        if (x >= 0 && x < _width && y >= 0 && y < _height && x == player->getX() && y == player->getY())
            _tiles[y][x]->addPlayer(_players);
    }
    for (const auto& [id, egg] : _eggs) {
        if (egg->getX() == x && egg->getY() == y)
            _tiles[y][x]->addEgg(egg);
    }
}

/**
 * @brief Récupère les informations à afficher au survol d'un joueur.
 * 
 * @param worldPos Position dans le monde (coordonnées flottantes).
 * @return std::string Informations à afficher.
 */
std::string Zappy::Map::getPlayerHoverInfo(const sf::Vector2f& worldPos) const
{
    for (const auto& [id, player] : _parseServer.getPlayers()) {
        float posX = player->getX() * _tileSize + _offsetX + _tileSize / 2;
        float posY = player->getY() * _tileSize + _offsetY + _tileSize / 2;
        sf::FloatRect bounds = player->getBounds(posX, posY);
        if (bounds.contains(worldPos)) {
            std::string playerInfo = player->getHoverInfo();
            std::string tileInfo = getTileAtPosition(player->getX(), player->getY(), worldPos);
            std::string combined;
            if (!playerInfo.empty())
                combined += "--- PLAYER ---\n" + playerInfo + "\n\n";
            if (!tileInfo.empty())
                combined += "--- TILE ---\n" + tileInfo;
            return combined;
        }
    }
    return "";
}

/**
 * @brief Récupère les informations à afficher au survol d'une tile donnée.
 * 
 * @param x Coordonnée x de la tile.
 * @param y Coordonnée y de la tile.
 * @param worldPos Position dans le monde (coordonnées flottantes).
 * @return std::string Informations à afficher.
 */
std::string Zappy::Map::getTileAtPosition(int x, int y, const sf::Vector2f& worldPos) const
{
    if (x >= 0 && x < _width && y >= 0 && y < _height)
        return _tiles[y][x]->getHoverInfo(worldPos);
    return "";
}

/**
 * @brief Récupère les informations à afficher au survol d'une tile (parcours complet).
 * 
 * @param worldPos Position dans le monde (coordonnées flottantes).
 * @return std::string Informations à afficher.
 */
std::string Zappy::Map::getTileHoverInfo(const sf::Vector2f& worldPos) const
{
    for (const auto& row : _tiles) {
        for (const auto& tile : row) {
            std::string tileInfo = tile->getHoverInfo(worldPos);
            if (!tileInfo.empty())
                return "--- TILE ---\n" + tileInfo;
        }
    }
    return "";
}

/**
 * @brief Récupère les informations à afficher au survol d'un œuf.
 * 
 * @param worldPos Position dans le monde (coordonnées flottantes).
 * @return std::string Informations à afficher.
 */
std::string Zappy::Map::getEggHoverInfo(const sf::Vector2f& worldPos) const
{
    std::stringstream ss;

    for (const auto& [id, egg] : _parseServer.getEggs()) {
        float posX = egg->getX() * _tileSize + _offsetX + _tileSize / 2;
        float posY = egg->getY() * _tileSize + _offsetY + _tileSize / 2;
        sf::FloatRect bounds = egg->getBounds(posX, posY);
        if (bounds.contains(worldPos)) {
            ss << "Egg ID: " << egg->getID() << "\n"
               << "Team: " << egg->getTeamID() << "\n"
               << "Position: (" << egg->getX() << ", " << egg->getY() << ")\n"
               << "Status: " << (egg->isHatched() ? "Hatched" : "Not hatched");
            return ss.str();
        }
    }
    return "";
}

/**
 * @brief Vérifie si la souris survole un joueur, œuf ou tile et affiche une popup d'info.
 * 
 * @param window Référence à la fenêtre SFML.
 */
void Zappy::Map::checkHoverTile(sf::RenderWindow& window) const
{
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
    std::string hoverInfo = getPlayerHoverInfo(worldPos);

    if (hoverInfo.empty())
        hoverInfo = getEggHoverInfo(worldPos);
    if (hoverInfo.empty())
        hoverInfo = getTileHoverInfo(worldPos);
    if (!hoverInfo.empty())
        drawHoverPopup(window, hoverInfo);
}

/**
 * @brief Dessine une popup affichant les informations données.
 * 
 * @param window Référence à la fenêtre SFML.
 * @param info Texte à afficher dans la popup.
 */
void Zappy::Map::drawHoverPopup(sf::RenderWindow& window, const std::string& info) const
{
    static sf::Font font;
    static bool fontLoaded = false;
    int lineCount = std::count(info.begin(), info.end(), '\n') + 1;
    int popupHeight = 40 + lineCount * 22;

    if (!fontLoaded) {
        font.loadFromFile("./src/GUI/assets/font.ttf");
        fontLoaded = true;
    }
    sf::RectangleShape popupWindow(sf::Vector2f(350, popupHeight));
    popupWindow.setPosition(window.getSize().x - 380, 380);
    popupWindow.setFillColor(sf::Color(0, 0, 0, 180));
    popupWindow.setOutlineColor(sf::Color::White);
    popupWindow.setOutlineThickness(1);
    sf::Text popupText(info, font, 18);
    popupText.setFillColor(sf::Color::White);
    popupText.setOutlineColor(sf::Color::Black);
    popupText.setOutlineThickness(0.5f);
    popupText.setPosition(window.getSize().x - 370, 400);
    window.draw(popupWindow);
    window.draw(popupText);
}
