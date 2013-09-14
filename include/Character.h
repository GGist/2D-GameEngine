/***********************************************************************************

***********************************************************************************/

#ifndef CHARACTER_H
#define CHARACTER_H

#include <Projectile.h>
#include <TextureManager.h>

class Level;

class Character
{
    public:
        Character(TextureManager* tManager);
        //Constructor
        virtual ~Character();
        //Destructor
        void runRight();
        //Sets xSpeed equal to RUN_SPEED
        void runLeft();
        //Sets xSpeed equal to -RUN_SPEED
        void shoot();
        //Causes the player to shoot a projectile from the active gun
        bool updateProjectiles(Level& currentLevel);
        //Moves the active projectiles based on their velocity and checks for collisions
        //Returns true if there is at least one active projectile
        virtual bool checkAnimation();
        //Checks to see if the animation needs to be updated and calls updateAnimation()
        //Returns false if textureInterface is NULL or if animation was not updated
        virtual bool applySpeed(Level& currentLevel) = 0;
        //This function should be overloaded to updated both the location and
        //animation of the currentSprite
        //Returns whether or not the screen should be scrolled
        const std::vector<sf::Sprite>& getProjectiles() const;
        //Returns the projectiles vector object
        const sf::Sprite& getSprite() const;
        //Returns the currentSprite object
        const int getCurrentRunSpeed() const;
        //Returns current run speed

    protected:
        //Constants
        enum CharTracking {
            CENTER,
            BOTTOM_LEFT,
            BOTTOM_RIGHT
        };


        //Functions
        virtual void updateAnimation();
        //Updates the currentSprite animation
        virtual void updateState();
        //Update the bool states of the player
        void translateCoords();
        //Translates lastSprite position to currentSprite position based on currentTracking
        virtual bool updateCoordTracking();
        //Updates the tracking bools used to determine where to track currentSprite
        //Returns true if the xCoord and yCoord need to be updated

        //Data
        TextureManager* tInterface;
        CharTracking currentTracking, lastTracking;
        float xCoord, yCoord, xSpeed, speedModifier;
        bool shot, facingRight, stateLock;
        sf::Sprite currentSprite, lastSprite;
        sf::Clock animationWait;

    private:
        const int GUNFIRE_OFFSET = 5, BULLET_SPAWN_OFFSET_FACTOR = 0.20;

        virtual int getDefaultRunSpeed() const = 0;
        //Returns a default run speed constant

        Projectile currentProjectiles;
};

#endif // CHARACTER_H
