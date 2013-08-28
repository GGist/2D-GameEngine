#ifndef PLAYER_H
#define PLAYER_H

#include <Projectile.h>

class Player
{
    public:
        Player();
        //Constructor
        virtual ~Player();
        //Destructor
        void runRight();
        //Sets xSpeed equal to RUN_SPEED
        void runLeft();
        //Sets xSpeed equal to -RUN_SPEED
        void jump();
        //Sets ySpeed equal to -JUMP_SPEED (making the sprite go up)
        void deployParachute();
        //Causes the player to deploy a parachute, slowing the falling speed
        void retractParachute();
        //Causes the player to retract the parachute, returning to normal falling speed
        void knife();
        //Causes the player to perform a knife melee attack
        void shoot();
        //Causes the player to shoot a projectile from the active gun
        bool updatePlayer(Level& currentLevel);
        //Moves currentSprite based on xSpeed and ySpeed and updates it's animation
        //Returns true if the background needs to be scrolled
        bool updateProjectiles(Level& currentLevel);
        //Moves the active projectiles based on their velocity and checks for collisions
        //Returns true if there is at least one active projectile
        const std::vector<sf::Sprite>& getProjectiles() const;
        //Returns the projectiles vector object
        const sf::Sprite& getSprite() const;
        //Returns the currentSprite object
        int getRunSpeed() const;
        //Returns RUN_SPEED

    protected:
    private:
        //Constants
        static const int NUM_ANIMATIONS = 108, DEFAULT_SCROLL = 512, WIDTH_CLIP = 25, RUN_SPEED = 6,
                         JUMP_SPEED = -18, GUNFIRE_FRAME = 5, PARACHUTE_LEFT_TO_RIGHT = 19,
                         PARACHUTE_RIGHT_TO_LEFT = -17;
        static constexpr float ANIMATION_DELAY = .06, ASCEND_GRAVITY = 0.5, DESCEND_GRAVITY = 0.18, VERT_COLLISION_GRAVITY = 1.5,
                                BULLET_SPAWN_FACTOR = 0.20;
        static constexpr bool VERTICAL = true, HORIZONTAL = false;
        static const sf::Vector2f SPRITE_SCALE, STARTING_POSITION;
        static const std::string SPRITE_MANIFEST, SPRITE_PATH, SPRITE_FORMAT;
        //~Manifest FileNames Must Be Ordered The Same As PlayerAnimation
        enum PlayerAnimation {
            KNIFE_LEFT = 0,
            END_KNIFE_LEFT = 11,
            KNIFE_RIGHT = 12,
            END_KNIFE_RIGHT = 23,
            PARACHUTE_LEFT = 24,
            END_PARACHUTE_LEFT = 41,
            PARACHUTE_RIGHT = 42,
            END_PARACHUTE_RIGHT = 59,
            SHOOT_LEFT = 60,
            END_SHOOT_LEFT = 70,
            SHOOT_RIGHT = 71,
            END_SHOOT_RIGHT = 81,
            RUN_LEFT = 82,
            END_RUN_LEFT = 93,
            RUN_RIGHT = 94,
            END_RUN_RIGHT = 105,
            STAND_LEFT = 106,
            STAND_RIGHT = 107
        };
        enum PlayerTracking {
            CENTER,
            BOTTOM_LEFT,
            BOTTOM_RIGHT
        };

        //Functions
        void updateAnimation();
        //Updates the currentSprite animation
        void updateAttackStates();
        //Update the shot and knifed states of the Player
        void translateCoords();
        //Translates lastSprite position to currentSprite position based on currentTracking
        bool updateCoordTracking();
        //Updates the tracking bools used to determine where to track currentSprite
        //Returns true if the xCoord and yCoord need to be updated

        //Data
        Projectile currentProjectiles;
        PlayerAnimation currentAnimation, lastAnimation;
        PlayerTracking currentTracking, lastTracking;
        float xCoord, yCoord, scrollOffset;
        float xSpeed, ySpeed;
        bool jumped, shot, knifed, falling, parachute, retract;
        sf::Texture playerTextures[NUM_ANIMATIONS];
        sf::Sprite currentSprite, lastSprite;
        sf::Clock animationWait;
};

#endif // HERO_H
