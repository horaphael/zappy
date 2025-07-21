/*
** EPITECH PROJECT, 2025
** Egg
** File description:
** Egg class representing an egg in the Zappy game.
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
     * @brief Represents an egg entity in the game.
     *
     * The Egg class handles the properties and behaviors of an egg,
     * including its position, team affiliation, hatch status, and timer.
     */
    class Egg
    {
        public:
            /**
             * @brief Construct a new Egg object.
             *
             * @param id Unique identifier of the egg.
             * @param teamId Identifier of the team the egg belongs to.
             * @param x Initial X coordinate of the egg.
             * @param y Initial Y coordinate of the egg.
             */
            Egg(int id, int teamId, int x, int y);

            /**
             * @brief Destroy the Egg object.
             */
            ~Egg() = default;

            /**
             * @brief Update the egg's internal timer.
             *
             * @param deltaTime Time elapsed since last update.
             */
            void update(float deltaTime);

            /**
             * @brief Draw the egg sprite on the given render window.
             *
             * @param window SFML render window to draw on.
             * @param posX X coordinate to draw the egg at.
             * @param posY Y coordinate to draw the egg at.
             */
            void draw(sf::RenderWindow &window, float posX, float posY) const;

            /**
             * @brief Get the bounding rectangle of the egg sprite at the specified position.
             *
             * @param posX X coordinate of the egg.
             * @param posY Y coordinate of the egg.
             * @return sf::FloatRect Bounding rectangle for collision or hover detection.
             */
            sf::FloatRect getBounds(float posX, float posY) const;

            /** @name Getters */
            ///@{
            inline int getID() const { return id; }
            inline int getTeamID() const { return teamid; }
            inline int getX() const { return x; }
            inline int getY() const { return y; }
            inline bool isHatched() const { return hatched; }
            inline float getTimer() const { return timer; }
            ///@}

            /** @name Setters */
            ///@{
            /**
             * @brief Set the hatch status of the egg.
             *
             * @param status True if the egg is hatched, false otherwise.
             */
            void setHatched(bool status) { hatched = status; }

            /**
             * @brief Set the position of the egg.
             *
             * @param newX New X coordinate.
             * @param newY New Y coordinate.
             */
            void setPosition(int newX, int newY) { x = newX; y = newY; }
            ///@}

        private:
            int id;               /**< Unique egg ID */
            int teamid;           /**< Team ID that owns the egg */
            int x;                /**< X position on the map */
            int y;                /**< Y position on the map */
            bool hatched = false; /**< Hatch status */
            float timer = 300.0f; /**< Time until hatching */
            sf::Texture _texture; /**< Texture for the egg sprite */
            sf::Sprite _sprite;   /**< Sprite to draw */
    };
}

#endif /* !EGG_HPP */
