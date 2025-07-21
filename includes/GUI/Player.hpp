/**
 * @file Player.hpp
 * @brief Déclaration de la classe Player pour l'interface graphique du projet Zappy.
 * 
 * Gère les informations, les actions et le rendu d’un joueur sur la carte.
 * Inclut les mécaniques de direction, d’inventaire, de vision et de broadcast.
 */

#ifndef PLAYER_HPP_
    #define PLAYER_HPP_

    #include <iostream>
    #include <sstream>
    #include <string>
    #include <map>
    #include <array>
    #include <SFML/Graphics.hpp>

namespace Zappy
{
    /**
     * @brief Direction du joueur sur la carte.
     */
    enum Direction {
        NORTH = 1, ///< Vers le haut de la carte
        EAST = 2,  ///< Vers la droite
        SOUTH = 3, ///< Vers le bas
        WEST = 4   ///< Vers la gauche
    };

    /**
     * @brief Action actuelle du joueur.
     */
    enum Action {
        IDLE,         ///< Inactif
        MOVING,       ///< Se déplace
        INCANTING,    ///< Effectue un rituel d'élévation
        EJECTING,     ///< Éjecte un autre joueur
        BROADCASTING  ///< Envoie un message
    };

    /**
     * @class Player
     * @brief Représente un joueur affiché sur la carte côté interface graphique.
     */
    class Player
    {
        public:
            /**
             * @brief Constructeur
             * @param id Identifiant du joueur
             * @param x Position X
             * @param y Position Y
             * @param direction Direction initiale
             * @param team Nom de l'équipe
             * @param mapWidth Largeur de la carte
             * @param mapHeight Hauteur de la carte
             */
            Player(int id, int x, int y, Direction direction, const std::string &team, int mapWidth, int mapHeight);

            /**
             * @brief Destructeur par défaut
             */
            ~Player() = default;

            //-----------POSITION-----------//

            /// Définit la position du joueur
            inline void setPosition(int x, int y) { this->_x = x; this->_y = y; }

            /// Retourne la position X
            inline int getX() const { return _x; }

            /// Retourne la position Y
            inline int getY() const { return _y; }

            //-----------LEVEL-----------//

            /// Définit le niveau du joueur
            inline void setLevel(int level) { _level = level; }

            /// Retourne le niveau du joueur
            inline int getLevel() const { return _level; }

            //-----------DIRECTION-----------//

            /// Retourne la direction actuelle
            inline Direction getDirection() const { return _direction; }

            /// Définit la direction du joueur (et fait pivoter le sprite)
            inline void setDirection(Direction dir) { _direction = dir; _sprite.setRotation(static_cast<float>(dir) * 90.f); }

            //-----------MAP-----------//

            /**
             * @brief Définit la taille de la carte (utile pour certains calculs)
             */
            void setMapSize(int width, int height);

            //-----------INVENTORY-----------//

            /**
             * @brief Met à jour l'inventaire du joueur
             */
            void setInventory(const std::map<std::string, int>& inventory);

            /**
             * @brief Retourne l'inventaire du joueur
             */
            inline const std::map<std::string, int>& getInventory() {return _inventory; }

            //-----------INFORMATIONS-----------//

            /**
             * @brief Retourne le nom de l'équipe
             */
            std::string getTeamName(void);

            /**
             * @brief Retourne le niveau (surcharge de getLevel ?)
             */
            float getLevel(void);

            /**
             * @brief Retourne la zone de collision du joueur pour la souris
             */
            sf::FloatRect getBounds(float posX, float posY) const;

            /**
             * @brief Retourne les informations affichées au survol
             */
            std::string getHoverInfo() const;

            /// Retourne la taille d’une tuile (sprite)
            inline float getTileSize() { return _tileSize; };

            /// Définit la fenêtre SFML à laquelle le joueur est rattaché
            inline void setWindow(sf::RenderWindow* window) { _window = window; }

            /// Retourne la fenêtre SFML utilisée
            inline sf::RenderWindow *getWindow() const { return _window; }

            //-----------RENDER-----------//

            /**
             * @brief Dessine le joueur à l’écran
             */
            void draw(sf::RenderWindow &window, float tileSize, float offsetX, float offsetY) const;

            /**
             * @brief Met à jour la texture du sprite en fonction de la direction
             */
            void updateSpriteTexture();

            //-----------BROADCAST-----------//

            /**
             * @brief Déclenche l’effet de broadcast (animation)
             */
            void triggerBroadcast();

            /**
             * @brief Dessine l’effet de broadcast autour du joueur
             */
            void drawBroadcast(sf::RenderWindow& window, float x, float y, float tileSize);

        private:
            int id; ///< ID unique du joueur
            int _x; ///< Position X
            int _y; ///< Position Y
            int _mapWidth; ///< Largeur de la carte
            int _mapHeight; ///< Hauteur de la carte
            int _level; ///< Niveau actuel
            std::string team; ///< Nom de l’équipe
            int lifeUnits; ///< Unités de vie (pour la faim)
            std::map<std::string, int> _inventory; ///< Inventaire du joueur
            Direction _direction; ///< Direction actuelle
            Action _currentAction; ///< Action actuelle
            std::array<sf::Texture, 4> _directionTextures; ///< Textures pour chaque direction
            sf::Sprite _sprite; ///< Sprite graphique du joueur
            float _tileSize = 32.f; ///< Taille de la tuile
            bool _isBroadcasting = false; ///< Si le joueur est en broadcast
            sf::Clock _broadcastClock; ///< Timer pour les effets de broadcast
            sf::RenderWindow* _window = nullptr; ///< Pointeur vers la fenêtre SFML
    };
}

#endif /* !PLAYER_HPP_ */
