/*
** EPITECH PROJECT, 2025
** Egg
** File description:
** Classe Egg représentant un œuf dans la simulation Zappy.
*/

#ifndef EGG_HPP
#define EGG_HPP

#include <iostream>
#include <string.h>
#include <SFML/Graphics.hpp>
#include <unistd.h>
#include <sstream>
#include "Player.hpp"
#include <memory>

namespace Zappy
{
    /**
     * @class Egg
     * @brief Représente une entité œuf dans la simulation.
     *
     * La classe Egg gère les propriétés et comportements d’un œuf,
     * incluant sa position, son appartenance à une équipe, son état d’éclosion, et un minuteur.
     */
    class Egg
    {
        public:
            /**
             * @brief Construit un nouvel objet Egg.
             *
             * @param id Identifiant unique de l’œuf.
             * @param teamId Identifiant de l’équipe à laquelle appartient l’œuf.
             * @param x Coordonnée X initiale de l’œuf.
             * @param y Coordonnée Y initiale de l’œuf.
             */
            Egg(int id, int teamId, int x, int y);

            /**
             * @brief Détruit l’objet Egg.
             */
            ~Egg() = default;

            /**
             * @brief Met à jour le minuteur interne de l’œuf.
             *
             * @param deltaTime Temps écoulé depuis la dernière mise à jour.
             */
            void update(float deltaTime);

            /**
             * @brief Dessine le sprite de l’œuf sur la fenêtre donnée.
             *
             * @param window Fenêtre SFML sur laquelle dessiner.
             * @param posX Coordonnée X pour dessiner l’œuf.
             * @param posY Coordonnée Y pour dessiner l’œuf.
             */
            void draw(sf::RenderWindow &window, float posX, float posY) const;

            /**
             * @brief Obtient le rectangle englobant du sprite de l’œuf à la position spécifiée.
             *
             * @param posX Coordonnée X de l’œuf.
             * @param posY Coordonnée Y de l’œuf.
             * @return sf::FloatRect Rectangle englobant pour détection de collision ou survol.
             */
            sf::FloatRect getBounds(float posX, float posY) const;

            /** @name Accesseurs */
            ///@{
            inline int getID() const { return id; }
            inline int getTeamID() const { return teamid; }
            inline int getX() const { return x; }
            inline int getY() const { return y; }
            inline bool isHatched() const { return hatched; }
            inline float getTimer() const { return timer; }
            ///@}

            /** @name Mutateurs */
            ///@{
            /**
             * @brief Définit l’état d’éclosion de l’œuf.
             *
             * @param status Vrai si l’œuf est éclos, faux sinon.
             */
            void setHatched(bool status) { hatched = status; }

            /**
             * @brief Définit la position de l’œuf.
             *
             * @param newX Nouvelle coordonnée X.
             * @param newY Nouvelle coordonnée Y.
             */
            void setPosition(int newX, int newY) { x = newX; y = newY; }
            ///@}

        private:
            int id;               /**< Identifiant unique de l’œuf */
            int teamid;           /**< Identifiant de l’équipe propriétaire de l’œuf */
            int x;                /**< Position X sur la carte */
            int y;                /**< Position Y sur la carte */
            bool hatched = false; /**< État d’éclosion */
            float timer = 300.0f; /**< Temps restant avant éclosion */
            sf::Texture _texture; /**< Texture du sprite de l’œuf */
            sf::Sprite _sprite;   /**< Sprite à dessiner */
    };
}

#endif /* !EGG_HPP */
