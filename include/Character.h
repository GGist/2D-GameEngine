/***********************************************************************************

***********************************************************************************/

#ifndef CHARACTER_H
#define CHARACTER_H

#include <ProjectileManager.h>
#include <AnimationManager.h>

class Level;

class Character
{
    public:
        Character(CharacterAnim* aMgr);
        //Constructor
        virtual ~Character();
        //Destructor
        void runRight();
        //Sets xSpeed equal to RUN_SPEED
        void runLeft();
        //Sets xSpeed equal to -RUN_SPEED
        void stopRun();
        //Sets xSpeed equal to 0
        void shoot();
        //Causes the player to shoot a projectile from the active gun
        bool isDead();
        //Returns true if the character is dead
        virtual bool checkProjectiles(Level& currentLevel) = 0;
        //Returns true if a projectile should be spawn at spawnPoint and sets it
        //to go right if shootingRight
        virtual bool checkAnimation() = 0;
        //Returns false if animation should not be updated
        virtual bool applySpeed(Level& currentLevel) = 0;
        //Moves the sprite based on the current speed
        //
        void drawProjectiles(sf::RenderWindow& renderWindow) const;
        //
        const sf::Sprite& getSprite() const;
        //Returns the currentSprite object
        int getCurrentRunSpeed() const;
        //Returns current run speed

    protected:
        //Constants
        enum CharTracking {
            CENTER,
            BOTTOM_LEFT,
            BOTTOM_RIGHT
        };

        //Functions
        virtual void updateState();
        //Updates the bool states of the character
        virtual bool updateCoordTracking();
        //Updates the tracking bools used to determine where to track currentSprite
        //Returns true if the xCoord and yCoord need to be updated
        void translateCoords();
        //Translates lastSprite position to currentSprite position based on currentTracking
        void createProjectile(sf::Vector2f spawnPoint, bool shootingRight);
        //
        bool updateProjectiles(Level& currentLevel);
        //Moves the active projectiles based on their velocity and checks for collisions
        //Returns false if there are no active projectiles
        virtual bool updateAnimation();
        //Updates the animation if checkAnimation() returns true
        //Returns false if checkAnimation() returns false
        virtual int getDefaultRunSpeed() const = 0;
        //Returns a default run speed constant
        virtual bool checkForDeath() = 0;
        //Returns true if the character is dead

        //Data
        AnimationManager* aManager;
        CharTracking currentTracking, lastTracking;
        float xCoord, yCoord, xSpeed, ySpeed, speedModifier;
        bool shot, died, facingRight, stateLock;
        sf::Sprite currentSprite, lastSprite;

    private:
        ProjectileManager pManager;
};

#endif // CHARACTER_H
