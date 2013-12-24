/***********************************************************************************
*This class is used as a base class to implement playable or computer controlled   *
*characters. To see how this class should be used see the Player and Enemy classes.*
*The most useful default behavior wrapped in this class is the character tracking  *
*which will make sure animations are repositioned when they are changing size.     *
*Projectile functionality is also provided which is fairly self explanatory.       *
*Full code at https://github.com/GGist/SpriteEngine                               *
*This code is licensed under the MIT License.                                      *
***********************************************************************************/

#ifndef CHARACTER_H
#define CHARACTER_H

#include <ProjectileManager.h>
#include <AnimationManager.h>

class Level;

class Character
{
    public:
        Character(const CharacterAnim* cAnim);
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
        virtual bool checkProjectiles(Level& currentLevel) = 0;
        //Returns true if a projectile should be spawn at spawnPoint and sets it
        //to go right if shootingRight
        virtual bool checkAnimation() = 0;
        //Returns false if animation should not be updated
        virtual bool applySpeed(Level& currentLevel) = 0;
        //Moves the sprite based on the current speed
        //See derived class for return value
        bool checkShot(const sf::FloatRect& bounds);
        //Returns true if any of the character's bullets are within the bounds
        void drawProjectiles(sf::RenderWindow& renderWindow) const;
        //Draws the projectiles stored in ProjectileManager
        const sf::Sprite& getSprite() const;
        //Returns the currentSprite object
        ProjectileManager& getProjManager();
        //Returns the sprite's projectile manager
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
        //Creates a projectile at the specified spawn point going right if shootingRight
        bool updateProjectiles(Level& currentLevel);
        //Moves the active projectiles based on their velocity and checks for collisions
        //Returns false if there are no active projectiles
        virtual bool updateAnimation();
        //Updates the animation if checkAnimation() returns true
        //Returns false if checkAnimation() returns false
        virtual int getDefaultRunSpeed() const = 0;
        //Returns a default run speed constant

        //Data
        AnimationManager aManager;
        CharTracking currentTracking, lastTracking;
        float xCoord, yCoord, xSpeed, ySpeed, speedModifier;
        bool shot, facingRight, stateLock;
        sf::Sprite currentSprite, lastSprite;

    private:
        ProjectileManager pManager;
};

#endif // CHARACTER_H
