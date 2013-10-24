#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>

class Level;



class ProjectileManager
{
    public:
        //Container For Individual Projectiles
        struct Projectile {
            Projectile(sf::Sprite p, bool d) :
                proj(p), direction(d) {}
            sf::Sprite proj;
            bool direction;
        };

        ProjectileManager();
        //Constructor
        virtual ~ProjectileManager();
        //Destructor
        void addProjectile(sf::Vector2f starting, bool right);
        //Adds a projectile at the specified location and direction
        bool moveProjectiles(Level& currentLevel, sf::FloatRect playerBounds);
        //playerBounds NEEDS TO BE THE GLOBAL_BOUNDS FOR THE PLAYER
        //Moves the projectiles and checks for collision or off screen projectiles
        //Returns true if there are any active projectiles
        const std::vector<Projectile>& getProjectiles() const;
        //Returns the currentProjectile

        enum BoundType {
            TOP_BOUND,
            BOTTOM_BOUND,
            LEFT_BOUND,
            RIGHT_BOUND,
            NO_BOUND
        };
        BoundType boundsCheck(sf::Sprite& entity);
        //Checks the sprite against the currentProjectiles
        //Returns false if the sprite is not hitting any projectiles

    private:
        //Constants
        static const int NUM_ANIMATIONS = 4, SPEED = 10, SCALE = 4, SCREEN_WIDTH = 1280;
        static const std::string SPRITE_MANIFEST, SPRITE_PATH, SPRITE_FORMAT;

        //Data
        int numProjectiles;
        std::vector<Projectile> currentProjectiles;
        //std::vector<sf::Sprite> currentProjectiles;
        //std::vector<bool> currentDirections;
        //Utilize AnimationManager in the future
        sf::Texture projectileTextures[NUM_ANIMATIONS];
};

#endif // PROJECTILE_H
