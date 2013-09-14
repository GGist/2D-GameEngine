#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>

class Level;

class Projectile
{
    public:
        Projectile();
        //Constructor
        virtual ~Projectile();
        //Destructor
        void addProjectile(sf::Vector2f starting, bool right);
        //Adds a projectile at the specified location and direction
        bool moveProjectiles(Level& currentLevel, sf::FloatRect playerBounds);
        //playerBounds NEEDS TO BE THE GLOBAL_BOUNDS FOR THE PLAYER
        //Moves the projectiles and checks for collision or off screen projectiles
        //Returns true if there are any active projectiles
        const std::vector<sf::Sprite>& getProjectiles() const;
        //Returns the currentProjectile

    private:
        //Constants
        static const int NUM_ANIMATIONS = 4, SPEED = 10, SCALE = 4, SCREEN_WIDTH = 1280;
        static const std::string SPRITE_MANIFEST, SPRITE_PATH, SPRITE_FORMAT;

        //Functions

        //Data
        int numProjectiles, numDirections;
        std::vector<sf::Sprite> currentProjectiles;
        std::vector<bool> currentDirections;
        sf::Texture projectileTextures[NUM_ANIMATIONS];
};

#endif // PROJECTILE_H
