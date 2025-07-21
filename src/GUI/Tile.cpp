/*
** EPITECH PROJECT, 2025
** tile
** Description du fichier :
** Implémentation de la classe Tile
*/

#include "Tile.hpp"

/**
 * @brief Carte statique contenant les textures de ressources partagées entre toutes les tuiles.
 */
std::map<std::string, sf::Texture> Zappy::Tile::_resourceTextures;

/**
 * @brief Construit un nouvel objet Tile.
 * 
 * @param x Coordonnée X de la tuile.
 * @param y Coordonnée Y de la tuile.
 * @param size Taille (largeur et hauteur) de la tuile.
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
 * @brief Charge les textures des ressources si elles ne sont pas déjà chargées.
 */
void Zappy::Tile::loadResourcesTextures()
{
    if (_resourceTextures.empty()) {
        if (!_resourceTextures["food"].loadFromFile("./src/GUI/assets/food.png"))
            std::cerr << "Échec du chargement de la texture food" << std::endl;
        if (!_resourceTextures["linemate"].loadFromFile("./src/GUI/assets/green.png"))
            std::cerr << "Échec du chargement de la texture linemate" << std::endl;
        if (!_resourceTextures["deraumere"].loadFromFile("./src/GUI/assets/blue.png"))
            std::cerr << "Échec du chargement de la texture deraumere" << std::endl;
        if (!_resourceTextures["sibur"].loadFromFile("./src/GUI/assets/black.png"))
            std::cerr << "Échec du chargement de la texture sibur" << std::endl;
        if (!_resourceTextures["mendiane"].loadFromFile("./src/GUI/assets/purple.png"))
            std::cerr << "Échec du chargement de la texture mendiane" << std::endl;
        if (!_resourceTextures["phiras"].loadFromFile("./src/GUI/assets/pink.png"))
            std::cerr << "Échec du chargement de la texture phiras" << std::endl;
        if (!_resourceTextures["thystame"].loadFromFile("./src/GUI/assets/red.png"))
            std::cerr << "Échec du chargement de la texture thystame" << std::endl;

        for (auto& [name, texture] : _resourceTextures) {
            sf::Sprite sprite;
            sprite.setTexture(texture);
            _resourceSprites[name] = sprite;
        }
    }
}

/**
 * @brief Ajoute une ressource à la tuile.
 * 
 * Si la ressource n'existe pas encore, l'initialise à 1.
 * Sinon, incrémente la quantité de 1.
 * 
 * @param resource Nom de la ressource à ajouter.
 */
void Zappy::Tile::addRessource(const std::string &resource)
{
    if (_resources.count(resource) == 0)
        _resources[resource] = 1;
    else
        _resources[resource] += 1;
}

/**
 * @brief Définit la position de la forme de la tuile.
 * 
 * @param x Nouvelle position en X.
 * @param y Nouvelle position en Y.
 */
void Zappy::Tile::setPosition(float x, float y)
{
    shape.setPosition(x, y);
}

/**
 * @brief Supprime une unité d'une ressource sur la tuile.
 * 
 * Si la quantité est supérieure à 1, décrémente de 1.
 * Sinon, supprime complètement l'entrée.
 * 
 * @param resource Nom de la ressource à supprimer.
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
 * @brief Dessine tous les sprites de ressources sur la tuile.
 * 
 * Les ressources sont redimensionnées et positionnées dans la tuile, retour à la ligne si nécessaire.
 * 
 * @param window Fenêtre de rendu sur laquelle dessiner.
 * @return int Nombre de sprites de ressources dessinés.
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
 * @brief Vérifie si la souris survole un joueur présent sur cette tuile.
 * 
 * @param mousePos Position du curseur de la souris.
 * @return std::tuple<bool, std::string, float> Tuple indiquant :
 *      - si un joueur est survolé (booléen),
 *      - le nom de l'équipe du joueur (chaîne),
 *      - le niveau du joueur (float).
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
 * @brief Ajoute un œuf à la tuile.
 * 
 * @param egg Pointeur partagé vers l'œuf à ajouter.
 */
void Zappy::Tile::addEgg(std::shared_ptr<Egg> egg)
{
    _eggs.push_back(egg);
}

/**
 * @brief Dessine tous les œufs présents sur la tuile.
 * 
 * @param window Fenêtre de rendu sur laquelle dessiner.
 */
void Zappy::Tile::drawEggs(sf::RenderWindow &window) const
{
    const float tileSize = shape.getSize().x;

    for (size_t i = 0; i < _eggs.size(); i++)
        _eggs[i]->draw(window, shape.getPosition().x + tileSize / 2.0f, shape.getPosition().y + 2);
}

/**
 * @brief Définit la carte des ressources pour cette tuile.
 * 
 * @param resources Carte contenant les noms de ressources et leurs quantités.
 */
void Zappy::Tile::setResources(const std::map<std::string, int>& resources)
{
    _resources = resources;
}

/**
 * @brief Définit l'état d'incantation et le niveau de la tuile.
 * 
 * Affiche un message indiquant le début ou la fin de l'incantation.
 * 
 * @param state Booléen indiquant si l'incantation est active.
 * @param level Niveau de l'incantation.
 */
void Zappy::Tile::setIncantation(bool state, int level)
{
    _isIncanting = state;
    _incantationLevel = level;

    if (state)
        std::cout << "Incantation niveau " << level << " commencée sur cette tuile" << std::endl;
    else
        std::cout << "Incantation terminée sur cette tuile" << std::endl;
}

/**
 * @brief Définit les joueurs actuellement présents sur cette tuile.
 * 
 * @param players Carte d'identifiants vers des pointeurs partagés vers les objets Player.
 */
void Zappy::Tile::addPlayer(std::map<int, std::shared_ptr<Player>> players)
{
    _players = players;
}

/**
 * @brief Retourne une chaîne d'information au survol de la souris sur la tuile.
 * 
 * Inclut les quantités de ressources présentes.
 * 
 * @param mousePos Position de la souris.
 * @return std::string Chaîne multi-ligne contenant les informations de ressources ou vide si non survolée.
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
 * @brief Dessine l'effet de particules d'incantation sur la tuile si elle est active.
 * 
 * Les particules montent progressivement et se renouvellent périodiquement.
 * 
 * @param window Fenêtre de rendu sur laquelle dessiner.
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
