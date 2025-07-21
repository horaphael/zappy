/*
** EPITECH PROJECT, 2025
** player
** File description:
** Player class implementation
*/

#include "Player.hpp"

/**
 * @brief Construct a new Player object
 *
 * @param id Player's unique identifier.
 * @param x Initial x coordinate.
 * @param y Initial y coordinate.
 * @param direction Initial facing direction.
 * @param team Player's team name.
 * @param mapWidth Width of the map.
 * @param mapHeight Height of the map.
 */
Zappy::Player::Player(int id, int x, int y, Direction direction, const std::string &team, int mapWidth, int mapHeight)
: id(id), _x(x), _y(y), _mapWidth(mapWidth), _mapHeight(mapHeight), _level(1), team(team), lifeUnits(1260), _direction(direction), _currentAction(IDLE)
{
    if (!_directionTextures[SOUTH].loadFromFile("./src/GUI/assets/Trisorien.png")
        || !_directionTextures[EAST].loadFromFile("./src/GUI/assets/Trisorien_profil.png")
        || !_directionTextures[NORTH].loadFromFile("./src/GUI/assets/Trisorien_dos.png"))
        std::cerr << "Failed to load player textures" << std::endl;

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
 * @brief Draw the player sprite on the window.
 *
 * @param window Render window where the sprite will be drawn.
 * @param tileSize Size of each tile (used to calculate position).
 * @param offsetX Horizontal offset for drawing.
 * @param offsetY Vertical offset for drawing.
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
 * @brief Get the team name of the player.
 *
 * @return std::string Team name.
 */
std::string Zappy::Player::getTeamName(void)
{
    return this->team;
}

/**
 * @brief Get the current level of the player.
 *
 * @return float Level value.
 */
float Zappy::Player::getLevel(void)
{
    return this->_level;
}

/**
 * @brief Get the bounding box of the player sprite for a given position.
 *
 * @param posX X coordinate for bounding box center.
 * @param posY Y coordinate for bounding box center.
 * @return sf::FloatRect Bounding rectangle.
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
 * @brief Set the player's inventory.
 *
 * Resets the inventory to zero then updates with provided resources.
 *
 * @param inventory Map of resource names and their quantities.
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
 * @brief Set the size of the map.
 *
 * @param width New map width.
 * @param height New map height.
 */
void Zappy::Player::setMapSize(int width, int height)
{
    _mapWidth = width;
    _mapHeight = height;
}

/**
 * @brief Update the player's sprite texture based on the direction.
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
 * @brief Get information string about the player for display on hover.
 *
 * @return std::string Multi-line info about player state.
 */
std::string Zappy::Player::getHoverInfo() const
{
    std::stringstream ss;

    ss << "Player #" << id << "\n";
    ss << "Team: " << team << "\n";
    ss << "Level: " << _level << "\n";
    ss << "Position: [" << _x << "; " << _y << "]\n";
    ss << "Direction: ";
    switch(_direction) {
        case NORTH: ss << "NORTH"; break;
        case EAST: ss << "EAST"; break;
        case SOUTH: ss << "SOUTH"; break;
        case WEST: ss << "WEST"; break;
    }
    ss << "\nInventory:\n";
    for (const auto& [item, quantity] : _inventory)
        ss << "  - " << item << ": " << quantity << "\n";
    return ss.str();
}

/**
 * @brief Trigger the broadcasting animation.
 */
void Zappy::Player::triggerBroadcast()
{
    _isBroadcasting = true;
}

/**
 * @brief Draw the broadcast waves effect.
 *
 * @param window Render window to draw on.
 * @param x X coordinate of the player on screen.
 * @param y Y coordinate of the player on screen.
 * @param tileSize Size of the tile.
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
