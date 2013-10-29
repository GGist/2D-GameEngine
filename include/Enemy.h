/***********************************************************************************

***********************************************************************************/

#ifndef ENEMY_H
#define ENEMY_H

#include <Character.h>

class Enemy : public Character
{
    public:
        Enemy(sf::Vector2i windowBounds, sf::Vector2f startingPos);
        virtual ~Enemy();
        virtual bool checkProjectiles(Level& currentLevel);
        //Returns true if a projectile should be spawn at spawnPoint and sets it
        //to go right if shootingRight
        virtual bool checkAnimation();
        //Returns false if animation should not be updated
        virtual bool applySpeed(Level& currentLevel);
        //Moves the sprite based on the current speed
        //Returns false if the sprite hit a wall

    private:
        //Constants
        static const int DEFAULT_RUN_SPEED = 6;
        static constexpr float ANIMATION_DELAY = .06, BULLET_SPAWN_OFFSET_FACTOR = 0.10, SHOOT_CD = 0.5;
        static constexpr bool HORIZONTAL = false;
        static const sf::Vector2f SPRITE_SCALE;

        //Functions
        virtual bool updateAnimation();
        //Updates the animation if checkAnimation() returns true
        //Returns false if checkAnimation() returns false
        virtual int getDefaultRunSpeed() const;
        //Returns a default run speed constant
        virtual bool checkForDeath();
        //Checks if the character should be dead or not

        //Data
        sf::Clock shotTimer;
        sf::IntRect wBounds;
};

#endif // ENEMY_H
