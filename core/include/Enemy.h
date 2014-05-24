/***********************************************************************************
*This class is used to create and manipulate pseudo computer controlled characters.*
*Basic functionality is provided but it is recommended to not use this class       *
*directly and instead use EnemyManager which will handle most of the other computer*
*controlled functionality for you. This class is skinned with the EnemyAnim        *
*Animation subclass. Full code at https://github.com/GGist/SpriteEngine           *
*This code is licensed under the MIT License.                                      *
***********************************************************************************/

#ifndef ENEMY_H
#define ENEMY_H

#include <Character.h>
#include <memory>

class Enemy : public Character
{
    public:
        Enemy(sf::Vector2f startingPos);
        //Constructor
        virtual ~Enemy();
        //Destructor
        bool runOpposite();
        //Runs the opposite direction that the enemy is currently running
        //Returns false if the enemy was not moving
        void stopShoot();
        //Stops the shooting animation
        void setPosition(float x, float y);
        //Sets the position of the enemy sprite
        //Should only be used in certain cases
        virtual bool checkProjectiles(Level& currentLevel);
        //Returns true if a projectile should be spawn at spawnPoint and sets it
        //to go right if shootingRight
        virtual bool checkAnimation();
        //Returns false if animation should not be updated
        virtual bool applySpeed(Level& currentLevel);
        //Moves the sprite based on the current speed
        //Returns false if the sprite hit a wall
        float getBulletSpawnOffset();
        //Returns the BULLET_SPAWN_OFFSET_FACTOR
        bool getShotStatus();
        //Returns true if the enemy is shooting
        bool isFacingRight();
        //Returns true if the enemy is facing right

    private:
        //Constants
        static const int DEFAULT_RUN_SPEED = 2;
        static constexpr float ANIMATION_DELAY = 0.09, SLOW_ANIMATION_DELAY = 0.1,
            BULLET_SPAWN_OFFSET_FACTOR = 0.40, SHOOT_CD = 0.15, DESCEND_GRAVITY = 0.5;
        static constexpr bool VERTICAL = true, HORIZONTAL = false;
        static const sf::Vector2f SPRITE_SCALE;

        //Functions
        virtual void updateState();
        //Updates the bool states of the character
        virtual bool updateAnimation();
        //Updates the animation if checkAnimation() returns true
        //Returns false if checkAnimation() returns false
        virtual int getDefaultRunSpeed() const;
        //Returns a default run speed constant

        //Data
        sf::Clock shotTimer, animTimer;
};

#endif // ENEMY_H
