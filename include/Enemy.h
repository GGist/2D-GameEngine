/***********************************************************************************

***********************************************************************************/

#ifndef ENEMY_H
#define ENEMY_H

#include <Character.h>

class Enemy : public Character
{
    public:
        Enemy();
        virtual ~Enemy();
        virtual bool checkAnimation();
        //Checks to see if the animation needs to be updated and calls updateAnimation()
        //Returns false if textureInterface is NULL or if animation was not updated
        virtual bool applySpeed(Level& currentLevel);
        //This function should be overloaded to updated both the location and
        //animation of the currentSprite
        //Returns whether or not the screen should be scrolled

    private:
        static const int DEFAULT_RUN_SPEED = 6;
        static constexpr float ANIMATION_DELAY = .06;
        static constexpr bool HORIZONTAL = false;
        static const sf::Vector2f SPRITE_SCALE;

        virtual int getDefaultRunSpeed() const;
        //Returns a default run speed constant
        virtual bool checkForDeath();
        //Checks if the character should be dead or not
};

#endif // ENEMY_H
