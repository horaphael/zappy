/**
 * @file Tile.hpp
 * @brief Header file for the Tile class
 */

/*
** EPITECH PROJECT, 2025
** tile
** File description:
** tile
*/

#ifndef TILE_HPP_
    #define TILE_HPP_

    #include <map>
    #include <memory>
    #include <string>
    #include <SFML/Graphics.hpp>
    #include "Player.hpp"
    #include "Egg.hpp"

namespace Zappy
{
    /**
     * @class Tile
     * @brief Represents a tile in the game world
     */
    class Tile
    {
        public:
            //-----------CONSTRUCTOR-----------//
            Tile() = default;
            Tile(int x, int y, int size);
            ~Tile() = default;

            //-----------RESSOURCES-----------//
            void addPlayer(std::map<int, std::shared_ptr<Player>> _players);
            void addRessource(const std::string  &resource);
            int drawResources(sf::RenderWindow &window) const;
            void loadResourcesTextures();
            void removeRessource(const std::string &resource);
            const std::map<std::string, int>& getResources() const { return _resources; }
            void setResources(const std::map<std::string, int>& resources);

            //-----------EGG-----------//
            void addEgg(std::shared_ptr<Egg> egg);
            void drawEggs(sf::RenderWindow &window) const;
            void removeEgg(const std::string &egg);
            std::tuple<bool, int, int, float> hoverEgg(const sf::Vector2f &mousePos) const;

            //-----------INFORMATIONS-----------//
            void setPosition(float x, float y);
            std::string getHoverInfo(sf::Vector2f mousePos);
            std::tuple<bool, std::string, float> hoverPlayer(const sf::Vector2f &mousePos) const;
            void setIncantation(bool state, int level);
            const sf::RectangleShape &getShape() const { return shape; };
            void drawIncantation(sf::RenderWindow &window);

        private:
            int _x; ///< X coordinate of the tile
            int _y; ///< Y coordinate of the tile
            sf::RectangleShape shape; ///< SFML shape representing the tile
            std::map<std::string, int> _resources; ///< Map of resources on the tile
            std::map<int, std::shared_ptr<Player>> _players; ///< Players on the tile
            std::vector<std::shared_ptr<Egg>> _eggs; ///< Eggs on the tile
            bool _isIncanting = false; ///< Incantation status flag
            int _incantationLevel = 0; ///< Incantation level
            int nb_ressources = 0; ///< Number of resources
            std::map<std::string, sf::Sprite> _resourceSprites; ///< Resource sprites
            static std::map<std::string, sf::Texture> _resourceTextures; ///< Resource textures
            std::vector<sf::RectangleShape> _particles; ///< Particles for effects
            sf::Clock _particleClock; ///< Clock for particle effects
    };
}

#endif /* !TILE_HPP_ */