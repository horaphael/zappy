/**
 * @file Tile.hpp
 * @brief Fichier d'en-tête pour la classe Tile
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
     * @brief Représente une tuile dans le monde de la simulation
     */
    class Tile
    {
        public:
            //-----------CONSTRUCTEUR-----------//
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

            //-----------ŒUFS-----------//
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
            int _x; ///< Coordonnée X de la tuile
            int _y; ///< Coordonnée Y de la tuile
            sf::RectangleShape shape; ///< Forme SFML représentant la tuile
            std::map<std::string, int> _resources; ///< Ressources présentes sur la tuile
            std::map<int, std::shared_ptr<Player>> _players; ///< Joueurs présents sur la tuile
            std::vector<std::shared_ptr<Egg>> _eggs; ///< Œufs présents sur la tuile
            bool _isIncanting = false; ///< Indique si une incantation est en cours
            int _incantationLevel = 0; ///< Niveau de l’incantation
            int nb_ressources = 0; ///< Nombre de ressources
            std::map<std::string, sf::Sprite> _resourceSprites; ///< Sprites des ressources
            static std::map<std::string, sf::Texture> _resourceTextures; ///< Textures des ressources
            std::vector<sf::RectangleShape> _particles; ///< Particules pour les effets
            sf::Clock _particleClock; ///< Horloge pour les effets de particules
    };
}

#endif /* !TILE_HPP_ */
