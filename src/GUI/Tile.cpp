/*
** EPITECH PROJECT, 2025
** tile
** File description:
** Tile class implementation
*/

#include "Tile.hpp"

/**
 * @brief Static map holding resource textures shared by all tiles.
 */
std::map<std::string, sf::Texture> Zappy::Tile::_resourceTextures;

/**
 * @brief Construct a new Tile object.
 * 
 * @param x X coordinate of the tile.
 * @param y Y coordinate of the tile.
 * @param size Size (width and height) of the tile.
 */
Zappy::Tile::Tile(int x, int y, int size) : _x(x), _y(y)
{
    shape.setSize(sf::Vector2f(size, size));
    shape.setPosition(0, 0);
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineThickness(1.f);
    shape.setOutlineColor(sf::Color(80, 80, 80));
    loadResourcesTextures();
}

/**
 * @brief Load textures for resources if not already loaded.
 */
void Zappy::Tile::loadResourcesTextures()
{
    if (_resourceTextures.empty()) {
        if (!_resourceTextures["food"].loadFromFile("./src/GUI/assets/food.png"))
            std::cerr << "Failed to load food texture" << std::endl;
        if (!_resourceTextures["linemate"].loadFromFile("./src/GUI/assets/green.png"))
            std::cerr << "Failed to load linemate texture" << std::endl;
        if (!_resourceTextures["deraumere"].loadFromFile("./src/GUI/assets/blue.png"))
            std::cerr << "Failed to load deraumere texture" << std::endl;
        if (!_resourceTextures["sibur"].loadFromFile("./src/GUI/assets/black.png"))
            std::cerr << "Failed to load sibur texture" << std::endl;
        if (!_resourceTextures["mendiane"].loadFromFile("./src/GUI/assets/purple.png"))
            std::cerr << "Failed to load mendiane texture" << std::endl;
        if (!_resourceTextures["phiras"].loadFromFile("./src/GUI/assets/pink.png"))
            std::cerr << "Failed to load phiras texture" << std::endl;
        if (!_resourceTextures["thystame"].loadFromFile("./src/GUI/assets/red.png"))
            std::cerr << "Failed to load thystame texture" << std::endl;

        for (auto& [name, texture] : _resourceTextures) {
            sf::Sprite sprite;
            sprite.setTexture(texture);
            _resourceSprites[name] = sprite;
        }
    }
}

/**
 * @brief Add a resource to the tile.
 * 
 * If resource doesn't exist yet, initializes with 1.
 * Otherwise increments the resource count by 1.
 * 
 * @param resource Name of the resource to add.
 */
void Zappy::Tile::addRessource(const std::string &resource)
{
    if (_resources.count(resource) == 0)
        _resources[resource] = 1;
    else
        _resources[resource] += 1;
}

/**
 * @brief Set the position of the tile's shape.
 * 
 * @param x New x position.
 * @param y New y position.
 */
void Zappy::Tile::setPosition(float x, float y)
{
    shape.setPosition(x, y);
}

/**
 * @brief Remove one unit of a resource from the tile.
 * 
 * If resource quantity is more than one, decrements by one.
 * Otherwise removes the resource entry entirely.
 * 
 * @param resource Name of the resource to remove.
 */
void Zappy::Tile::removeRessource(const std::string &resource)
{
    for (auto it = _resources.begin(); it != _resources.end(); ++it) {
        if (it->first == resource) {
            if (it->second > 1)
                it->second -= 1;
            else
                _resources.erase(it);
            break;
        }
    }
}

/**
 * @brief Draw all resource sprites on the tile.
 * 
 * Resources are scaled and positioned within the tile, wrapping lines if needed.
 * 
 * @param window Render window to draw on.
 * @return int Number of resource sprites drawn.
 */
int Zappy::Tile::drawResources(sf::RenderWindow &window) const
{
    const float tileSize = shape.getSize().x;
    const float resSize = tileSize / 4.0f;
    const float spacing = 1.0f;
    const int maxPerLine = static_cast<int>(tileSize / (resSize + spacing));
    int index = 0;
    float baseX = shape.getPosition().x + spacing;
    float baseY = shape.getPosition().y + spacing;

    for (const auto& [name, quantity] : _resources) {
        if (quantity <= 0 || _resourceTextures.find(name) == _resourceTextures.end())
            continue;

        sf::Sprite resourceSprite;
        resourceSprite.setTexture(_resourceTextures.at(name));
        sf::FloatRect spriteSize = resourceSprite.getLocalBounds();
        float scale = resSize / std::max(spriteSize.width, spriteSize.height);
        resourceSprite.setScale(scale, scale);

        int col = index % maxPerLine;
        int row = index / maxPerLine;
        float x = baseX + col * (resSize + spacing);
        float y = baseY + row * (resSize + spacing);
        resourceSprite.setPosition(x, y);

        window.draw(resourceSprite);
        index++;
    }
    return index;
}

/**
 * @brief Check if the mouse is hovering over any player on this tile.
 * 
 * @param mousePos Position of the mouse cursor.
 * @return std::tuple<bool, std::string, float> Tuple indicating
 *      - if hovering over player (bool),
 *      - player's team name (string),
 *      - player's level (float).
 */
std::tuple<bool, std::string, float> Zappy::Tile::hoverPlayer(const sf::Vector2f &mousePos) const
{
    float baseX = shape.getPosition().x;
    float baseY = shape.getPosition().y;

    for (const auto& player : _players) {
        sf::FloatRect bounds = player.second->getBounds(baseX, baseY);
        if (bounds.contains(mousePos))
            return std::make_tuple(true, player.second->getTeamName(), player.second->getLevel());
    }
    return std::make_tuple(false, "", 0.0f);
}

/**
 * @brief Add an egg to the tile.
 * 
 * @param egg Shared pointer to the Egg object to add.
 */
void Zappy::Tile::addEgg(std::shared_ptr<Egg> egg)
{
    _eggs.push_back(egg);
}

/**
 * @brief Draw all eggs on the tile.
 * 
 * @param window Render window to draw on.
 */
void Zappy::Tile::drawEggs(sf::RenderWindow &window) const
{
    const float tileSize = shape.getSize().x;

    for (size_t i = 0; i < _eggs.size(); i++)
        _eggs[i]->draw(window, shape.getPosition().x + tileSize / 2.0f, shape.getPosition().y + 2);
}

/**
 * @brief Set the resources map for the tile.
 * 
 * @param resources Map of resource names and their quantities.
 */
void Zappy::Tile::setResources(const std::map<std::string, int>& resources)
{
    _resources = resources;
}

/**
 * @brief Set the incantation state and level on the tile.
 * 
 * Logs start or end of incantation.
 * 
 * @param state Whether incantation is active or not.
 * @param level Incantation level.
 */
void Zappy::Tile::setIncantation(bool state, int level)
{
    _isIncanting = state;
    _incantationLevel = level;

    if (state)
        std::cout << "Incantation level " << level << " started on this tile" << std::endl;
    else
        std::cout << "Incantation ended on this tile" << std::endl;
}

/**
 * @brief Set the players currently on this tile.
 * 
 * @param players Map of player IDs to shared pointers to Player objects.
 */
void Zappy::Tile::addPlayer(std::map<int, std::shared_ptr<Player>> players)
{
    _players = players;
}

/**
 * @brief Get a hover info string about this tile at a given mouse position.
 * 
 * Includes resource quantities.
 * 
 * @param mousePos Position of the mouse.
 * @return std::string Multi-line string with resource info or empty if not hovering on tile.
 */
std::string Zappy::Tile::getHoverInfo(sf::Vector2f mousePos)
{
    const float tileSize = shape.getSize().x;
    sf::FloatRect bounds(shape.getPosition().x, shape.getPosition().y, tileSize, tileSize);
    std::stringstream ss;

    if (!bounds.contains(mousePos))
        return "";
    if (!_resources.empty()) {
        ss << "Ressources:\n";
        for (auto &r : _resources)
            ss << " - " << r.first << " x" << r.second << "\n";
    }
    return ss.str();
}

/**
 * @brief Draw incantation particle effect on the tile if active.
 * 
 * Particles float upwards and refresh periodically.
 * 
 * @param window Render window to draw on.
 */
void Zappy::Tile::drawIncantation(sf::RenderWindow &window)
{
    const float tileSize = shape.getSize().x;
    const int particleCount = 50;
    const float particleSize = tileSize / 10.0f;
    const float lifetime = 0.35f;

    if (!_isIncanting)
        return;

    if (_particles.empty() || _particleClock.getElapsedTime().asSeconds() > lifetime) {
        _particles.clear();
        _particleClock.restart();

        for (int i = 0; i < particleCount; i++) {
            sf::RectangleShape particle(sf::Vector2f(particleSize, particleSize));
            particle.setFillColor(sf::Color(0, 255, 0, 77));
            float px = shape.getPosition().x + static_cast<float>(std::rand() % static_cast<int>(tileSize - particleSize));
            float py = shape.getPosition().y + static_cast<float>(std::rand() % static_cast<int>(tileSize - particleSize));
            particle.setPosition(px, py);
            _particles.push_back(particle);
        }
    }

    float elapsed = _particleClock.getElapsedTime().asSeconds();
    float offset = 2.f * (elapsed / lifetime);
    for (auto& particle : _particles) {
        sf::Vector2f pos = particle.getPosition();
        particle.setPosition(pos.x, pos.y - offset);
        window.draw(particle);
    }
}
