/**
 * @file Map.hpp
 * @brief Déclaration de la classe Map pour gérer l'affichage et l'interaction avec la carte du simulateur.
 */

#ifndef MAP_HPP_
    #define MAP_HPP_

    #include <vector>
    #include <SFML/Graphics.hpp>
    #include <memory>
    #include "Tile.hpp"
    #include "Player.hpp"
    #include "Egg.hpp"
    #include "ParseServer.hpp"

namespace Zappy
{
    /**
     * @class Map
     * @brief Classe responsable de l'affichage, l'interaction et la gestion de la carte graphique.
     */
    class Map
    {
        public:
            /**
             * @brief Constructeur
             * @param width Largeur de la carte en tuiles
             * @param height Hauteur de la carte en tuiles
             * @param window Fenêtre de rendu SFML
             * @param parseServer Référence vers l’objet de parsing du serveur
             */
            Map(int width, int height, sf::RenderWindow &window, const ParseServer &parseServer);

            /**
             * @brief Dessine la carte complète
             */
            void draw(sf::RenderWindow& window) const;

            /**
             * @brief Vérifie si une tuile est survolée par la souris
             */
            void checkHoverTile(sf::RenderWindow &window) const;

            /**
             * @brief Met à jour une tuile spécifique avec les ressources, joueurs et œufs
             * @param x Coordonnée X
             * @param y Coordonnée Y
             * @param resources Ressources présentes sur la tuile
             * @param _players Joueurs présents sur la tuile
             * @param _eggs Œufs présents sur la tuile
             */
            void updateTile(int x, int y, const std::map<std::string, int>& resources, std::map<int, std::shared_ptr<Player>> _players, std::map<int, std::shared_ptr<Egg>> _eggs);

            /**
             * @brief Retourne la forme SFML de la carte
             */
            const sf::RectangleShape& getShape() const { return shape; };

            /**
             * @brief Retourne les infos d’un joueur sous la souris
             */
            std::string getPlayerHoverInfo(const sf::Vector2f& worldPos) const;

            /**
             * @brief Retourne les infos d’une tuile sous la souris
             */
            std::string getTileHoverInfo(const sf::Vector2f& worldPos) const;

            /**
             * @brief Retourne le contenu d’une tuile aux coordonnées données
             */
            std::string getTileAtPosition(int x, int y, const sf::Vector2f& worldPos) const;

            /**
             * @brief Affiche une infobulle contenant des infos à l’endroit de la souris
             */
            void drawHoverPopup(sf::RenderWindow& window, const std::string& info) const;

            /**
             * @brief Retourne les infos d’un œuf sous la souris
             */
            std::string getEggHoverInfo(const sf::Vector2f& worldPos) const;

            /**
             * @brief Retourne les infos de l’équipe sous la souris
             */
            std::string getTeamHoverInfo(const sf::Vector2f& worldPos) const;

        private:
            int _width;  ///< Largeur de la carte
            int _height; ///< Hauteur de la carte
            int _tileSize; ///< Taille d’une tuile en pixels
            int _offsetX = 0; ///< Décalage horizontal (scrolling)
            int _offsetY = 0; ///< Décalage vertical (scrolling)
            bool _isDragging; ///< Indique si la carte est en cours de glissement
            float _zoomLevel; ///< Niveau de zoom de la vue
            sf::RectangleShape shape; ///< Représentation visuelle de la carte
            sf::Texture _tileTexture; ///< Texture des tuiles
            sf::RenderWindow &_window; ///< Référence à la fenêtre d’affichage
            const Zappy::ParseServer& _parseServer; ///< Référence au parseur du serveur
            std::vector<std::vector<std::shared_ptr<Tile>>> _tiles; ///< Grille de tuiles
            std::map<std::string, int> teamCounts; ///< Comptage des joueurs par équipe
    };
}

#endif /* !MAP_HPP_ */
