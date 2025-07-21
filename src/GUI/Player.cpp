/*
** EPITECH PROJECT, 2025
** player
** File description:
** Implémentation de la classe Player
*/

#include "Player.hpp"

/**
 * @brief Construit un nouvel objet Player
 *
 * @param id Identifiant unique du joueur.
 * @param x Coordonnée x initiale.
 * @param y Coordonnée y initiale.
 * @param direction Direction initiale du regard.
 * @param team Nom de l’équipe du joueur.
 * @param mapWidth Largeur de la carte.
 * @param mapHeight Hauteur de la carte.
 */
Zappy::Player::Player(int id, int x, int y, Direction direction, const std::string &team, int mapWidth, int mapHeight)
: id(id), _x(x), _y(y), _mapWidth(mapWidth), _mapHeight(mapHeight), _level(1), team(team), lifeUnits(1260), _direction(direction), _currentAction(IDLE)
{
    if (!_directionTextures[SOUTH].loadFromFile("./src/GUI/assets/Trisorien.png")
        || !_directionTextures[EAST].loadFromFile("./src/GUI/assets/Trisorien_profil.png")
        || !_directionTextures[NORTH].loadFromFile("./src/GUI/assets/Trisorien_dos.png"))
        std::cerr << "Échec du chargement des textures du joueur" << std::endl;

    _sprite.setTexture(_directionTextures[direction]);
    _sprite.setScale(0.1f, 0.1f);
    _inventory = {
        {"food", 0},
        {"linemate", 0},
        {"deraumere", 0},
        {"sibur", 0},
        {"mendiane", 0},
        {"phiras", 0},
        {"thystame", 0}
    };
}

/**
 * @brief Dessine le sprite du joueur dans la fenêtre.
 *
 * @param window Fenêtre de rendu où dessiner le sprite.
 * @param tileSize Taille de chaque tuile (utilisée pour calculer la position).
 * @param offsetX Décalage horizontal pour le dessin.
 * @param offsetY Décalage vertical pour le dessin.
 */
void Zappy::Player::draw(sf::RenderWindow &window, float tileSize, float offsetX, float offsetY) const
{
    float posX = _x * tileSize + offsetX;
    float posY = _y * tileSize + offsetY;
    sf::Sprite spriteToDraw = _sprite;

    spriteToDraw.setPosition(posX + tileSize / 2, posY + tileSize / 2);
    spriteToDraw.setOrigin(_sprite.getTexture()->getSize().x / 2,  _sprite.getTexture()->getSize().y / 2);
    spriteToDraw.setRotation(static_cast<float>(_direction));
    window.draw(spriteToDraw);
}

/**
 * @brief Retourne le nom de l’équipe du joueur.
 *
 * @return std::string Nom de l’équipe.
 */
std::string Zappy::Player::getTeamName(void)
{
    return this->team;
}

/**
 * @brief Retourne le niveau actuel du joueur.
 *
 * @return float Valeur du niveau.
 */
float Zappy::Player::getLevel(void)
{
    return this->_level;
}

/**
 * @brief Retourne le rectangle englobant du sprite du joueur à une position donnée.
 *
 * @param posX Coordonnée X pour le centre du rectangle.
 * @param posY Coordonnée Y pour le centre du rectangle.
 * @return sf::FloatRect Rectangle englobant.
 */
sf::FloatRect Zappy::Player::getBounds(float posX, float posY) const
{
    sf::Sprite spriteCopy = _sprite;
    float width = _sprite.getGlobalBounds().width;
    float height = _sprite.getGlobalBounds().height;

    spriteCopy.setPosition(posX, posY);
    return sf::FloatRect(posX - width/2, posY - height/2, width, height);
}

/**
 * @brief Définit l’inventaire du joueur.
 *
 * Réinitialise l’inventaire à zéro puis met à jour avec les ressources fournies.
 *
 * @param inventory Dictionnaire de ressources et quantités associées.
 */
void Zappy::Player::setInventory(const std::map<std::string, int>& inventory)
{
    this->_inventory = {
        {"food", 0},
        {"linemate", 0},
        {"deraumere", 0},
        {"sibur", 0},
        {"mendiane", 0},
        {"phiras", 0},
        {"thystame", 0}
    };
    for (const auto& [resource, quantity] : inventory) {
        if (this->_inventory.find(resource) != this->_inventory.end())
            this->_inventory[resource] = quantity;
    }
}

/**
 * @brief Définit la taille de la carte.
 *
 * @param width Nouvelle largeur de la carte.
 * @param height Nouvelle hauteur de la carte.
 */
void Zappy::Player::setMapSize(int width, int height)
{
    _mapWidth = width;
    _mapHeight = height;
}

/**
 * @brief Met à jour la texture du sprite du joueur en fonction de sa direction.
 */
void Zappy::Player::updateSpriteTexture()
{
    switch(_direction) {
        case SOUTH:
            _sprite.setTexture(_directionTextures[SOUTH]);
            _sprite.setScale(0.1f, 0.1f);
            break;
        case EAST:
            _sprite.setTexture(_directionTextures[EAST]);
            _sprite.setScale(0.1f, 0.1f);
            break;
        case NORTH:
            _sprite.setTexture(_directionTextures[NORTH]);
            _sprite.setScale(-0.1f, 0.1f);
            break;
        case WEST:
            _sprite.setTexture(_directionTextures[EAST]);
            _sprite.setScale(-0.1f, 0.1f);
            break;
    }
}

/**
 * @brief Retourne une chaîne d’information sur le joueur pour l’affichage lors d’un survol.
 *
 * @return std::string Informations multi-lignes sur l’état du joueur.
 */
std::string Zappy::Player::getHoverInfo() const
{
    std::stringstream ss;

    ss << "Joueur #" << id << "\n";
    ss << "Équipe : " << team << "\n";
    ss << "Niveau : " << _level << "\n";
    ss << "Position : [" << _x << "; " << _y << "]\n";
    ss << "Direction : ";
    switch(_direction) {
        case NORTH: ss << "NORD"; break;
        case EAST: ss << "EST"; break;
        case SOUTH: ss << "SUD"; break;
        case WEST: ss << "OUEST"; break;
    }
    ss << "\nInventaire :\n";
    for (const auto& [item, quantity] : _inventory)
        ss << "  - " << item << " : " << quantity << "\n";
    return ss.str();
}

/**
 * @brief Déclenche l’animation de broadcast.
 */
void Zappy::Player::triggerBroadcast()
{
    _isBroadcasting = true;
}

/**
 * @brief Dessine l’effet d’ondes de broadcast.
 *
 * @param window Fenêtre de rendu sur laquelle dessiner.
 * @param x Coordonnée X du joueur à l’écran.
 * @param y Coordonnée Y du joueur à l’écran.
 * @param tileSize Taille de la tuile.
 */
void Zappy::Player::drawBroadcast(sf::RenderWindow& window, float x, float y, float tileSize)
{
    const float waveDuration = 3.f;
    const int waveCount = 3;
    const float maxRadius = 100.f;
    float totalElapsed = _broadcastClock.getElapsedTime().asSeconds();

    if (!_isBroadcasting)
        return;

    for (int i = 0; i < waveCount; i++) {
        float waveStartTime = i * (waveDuration / waveCount);
        float elapsedSinceWaveStart = totalElapsed - waveStartTime;
        if (elapsedSinceWaveStart < 0.f || elapsedSinceWaveStart > waveDuration)
            continue;
        float t = elapsedSinceWaveStart / waveDuration;
        float radius = tileSize / 2.f + t * (maxRadius - tileSize / 2.f);
        float alpha = 255 * (1.0f - t);
        sf::CircleShape circle(radius);
        circle.setOrigin(radius, radius);
        circle.setPosition(x + tileSize / 2.f, y + tileSize / 2.f);
        circle.setFillColor(sf::Color(0, 0, 255, static_cast<sf::Uint8>(alpha * 0.2f)));
        circle.setOutlineThickness(2.f);
        circle.setOutlineColor(sf::Color(0, 0, 255, static_cast<sf::Uint8>(alpha)));
        window.draw(circle);
    }
    float fullCycleDuration = waveDuration + (waveDuration / waveCount) * (waveCount - 1);
    if (totalElapsed > fullCycleDuration)
        _broadcastClock.restart();
}
