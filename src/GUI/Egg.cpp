/*
** EPITECH PROJECT, 2025
** egg
** File description:
** egg
*/

#include "Egg.hpp"

/// @brief Constructeur de la classe Egg.
/// @param id Identifiant unique de l'œuf.
/// @param teamId Identifiant de l'équipe à laquelle appartient l'œuf.
/// @param x Position X de l'œuf sur la carte.
/// @param y Position Y de l'œuf sur la carte.
Zappy::Egg::Egg(int id, int teamId, int x, int y) : id(id), teamid(teamId), x(x), y(y)
{
    if (!_texture.loadFromFile("./src/GUI/assets/Egg_triso.png")) {
        std::cerr << "Failed to load egg texture\n";
    } else {
        _sprite.setTexture(_texture);
        _sprite.setScale(0.05f, 0.05f);  ///< Mise à l’échelle du sprite.
    }
}

/// @brief Met à jour l'état de l'œuf (vérifie s’il a éclos).
/// @param deltaTime Temps écoulé depuis la dernière mise à jour.
void Zappy::Egg::update(float deltaTime)
{
    if (!hatched) {
        timer -= deltaTime;
        if (timer <= 0) {
            hatched = true;
        }
    }
}

/// @brief Dessine l'œuf sur la fenêtre.
/// @param window Fenêtre SFML dans laquelle dessiner.
/// @param posX Position X où dessiner l'œuf.
/// @param posY Position Y où dessiner l'œuf.
void Zappy::Egg::draw(sf::RenderWindow &window, float posX, float posY) const
{
    sf::Sprite sprite = _sprite;
    sprite.setPosition(posX, posY);
    window.draw(sprite);
}

/// @brief Retourne la bounding box (zone de collision) de l'œuf à une position donnée.
/// @param posX Position X.
/// @param posY Position Y.
/// @return Rectangle flottant représentant la zone de collision.
sf::FloatRect Zappy::Egg::getBounds(float posX, float posY) const
{
    sf::Sprite spriteCopy = _sprite;
    spriteCopy.setScale(0.05f, 0.05f);
    spriteCopy.setPosition(posX + 2.f, posY + 2.f);
    return spriteCopy.getGlobalBounds();
}
