/***********************************************************************************
*This class is used to manipulate projectiles which are used in all Character      *
*objects. You can never explicitly delete a Projectile as they are deleted either  *
*if they hit a wall in the level, they intersect something in boundsCheck() or this*
*object is deleted (when a Character dies).                                        *
*Full code at https://github.com/GGist/2D-GameEngine                               *
*This code is licensed under the MIT License.                                      *
***********************************************************************************/

#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>
#include <AnimationManager.h>
#include <fstream>

class Level;

//Container For Individual Projectiles
struct Projectile {
    Projectile(sf::Sprite p, bool d) :
        proj(p), goingRight(d), offset(0) {}
    sf::Sprite proj;
    bool goingRight;
    int offset;
};

class ProjectileManager
{
    public:
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
        bool boundsCheck(const sf::FloatRect& entity);
        //Checks the sprite against the currentProjectiles
        //Returns false if the sprite is not hitting any projectiles

    private:
        //Constants
        static const int SPEED = 10, SCALE = 4, SCREEN_WIDTH = 1280;
        static const std::string SPRITE_MANIFEST, SPRITE_PATH, SPRITE_FORMAT;

        //Data
        int numProjectiles;
        AnimationManager aManager;
        std::vector<Projectile> currentProjectiles;
        sf::Sprite bullet;
};

#endif // PROJECTILE_H
