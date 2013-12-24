/***********************************************************************************
*This class is used to represent playable characters and is currently skinned as a *
*PlayerAnim Animation object. This class provides extended functionality in that   *
*knifing and parachuting animations have been added so the relevant functions have *
*been modified to account for this. To see this class in action refer to the       *
*Test_Engine.cpp file on the project github page.                                  *
*Full code at https://github.com/GGist/SpriteEngine                               *
*This code is licensed under the MIT License.                                      *
***********************************************************************************/

#ifndef PLAYER_H
#define PLAYER_H

#include <Character.h>
#include <memory>

class Player : public Character
{
    public:
        Player(sf::Vector2i windowBounds);
        //Constructor
        virtual ~Player();
        //Destructor
        void knife();
        //Causes the player to perform a knife melee attack
        void jump();
        //Sets ySpeed equal to -JUMP_SPEED (making the sprite go up)
        void deployParachute();
        //Causes the player to deploy a parachute, slowing the falling speed
        void retractParachute();
        //Causes the player to retract the parachute, returning to normal falling speed
        virtual bool checkProjectiles(Level& currentLevel);
        //Returns true if a projectile should be spawn at spawnPoint and sets it
        //to go right if shootingRight
        virtual bool checkAnimation();
        //Returns false if animation should not be updated
        virtual bool applySpeed(Level& currentLevel);
        //Moves currentSprite based on xSpeed and ySpeed and updates it's animation
        //Returns true if the background needs to be scrolled
        bool checkKnifed();
        //Returns true if the character is on the LAST KNIFE animation

    private:
        //Constants
        static constexpr int DEFAULT_RUN_SPEED = 4, DEFAULT_SCROLL = 512, JUMP_SPEED = -18,
            PARACHUTE_LEFT_TO_RIGHT = 19, PARACHUTE_RIGHT_TO_LEFT = -17, GUNFIRE_OFFSET = 5;
        static constexpr float ANIMATION_DELAY = .06, SLOW_ANIMATION_DELAY = .02,
            ASCEND_GRAVITY = 0.8, DESCEND_GRAVITY = 0.2, VERT_COLLISION_GRAVITY = 1.5,
            BULLET_SPAWN_OFFSET_FACTOR = 0.10;
        static constexpr bool VERTICAL = true, HORIZONTAL = false;
        static const sf::Vector2f SPRITE_SCALE, STARTING_POSITION;

        //Functions
        virtual void updateState();
        //Update the bool states of the player
        virtual bool updateCoordTracking();
        //Updates the tracking bools used to determine where to track currentSprite
        //Returns true if the xCoord and yCoord need to be updated
        virtual bool updateAnimation();
        //Updates the animation if checkAnimation() returns true
        //Returns false if checkAnimation() returns false
        virtual int getDefaultRunSpeed() const;
        //Returns a default run speed constant

        //Data
        float scrollOffset;
        bool knifed, jumped, falling, parachute, retract;
        sf::IntRect wBounds;
        sf::Clock animTimer;
};

#endif // HERO_H
