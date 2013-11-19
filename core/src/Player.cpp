#include "Player.h"
#include <Level.h>

using namespace std;

const sf::Vector2f Player::SPRITE_SCALE(2.0, 2.0), Player::STARTING_POSITION(200, 200);

static const PlayerAnim* getPlayerAnim()
{
    static PlayerAnim* pAnim = new PlayerAnim;

    return pAnim;
}

Player::Player(sf::Vector2i windowBounds) : Character(getPlayerAnim()), wBounds(sf::Vector2i(0, 0),
    windowBounds), scrollOffset(DEFAULT_SCROLL), knifed(false), jumped(false), falling(false),
    parachute(false), retract(false), animTimer()
{
    aManager.setTexture(PlayerAnim::STAND_RIGHT);

    currentSprite.setTexture(aManager.getCurrentTexture(), true);
    currentSprite.setPosition(STARTING_POSITION);
    currentSprite.setScale(SPRITE_SCALE);

    xCoord = currentSprite.getGlobalBounds().left;
    yCoord = currentSprite.getGlobalBounds().top;
}

Player::~Player()
{

}

void Player::knife()
{
    if (!stateLock) {
        knifed = true;
        stateLock = true;
    }
}

void Player::jump()
{
    if (!jumped) {
        ySpeed = JUMP_SPEED;
        jumped = true;
    }
}

void Player::deployParachute()
{
    if (!stateLock && jumped) {
        parachute = true;
        stateLock = true;
        retract = false;
    }
}

void Player::retractParachute()
{
    if (parachute) {
        parachute = false;
        retract = true;
    }
}

bool Player::checkProjectiles(Level& currentLevel)
{
    //In case the GUNFIRE_OFFSET animation stays the same for > 1 checkProjectiles() call
    static bool shotOnce = false;

    sf::Vector2f spawnPoint;
    bool shootingRight;

    if (!shotOnce && shot && aManager.getCurrentOffset() == GUNFIRE_OFFSET) {
        if (aManager.getCurrentAnimation() == CharacterAnim::SHOOT_LEFT) {
            spawnPoint.x = getSprite().getGlobalBounds().left;
            shootingRight = false;
        } else {
            spawnPoint.x = getSprite().getGlobalBounds().left + getSprite().getGlobalBounds().width;
            shootingRight = true;
        }
        spawnPoint.y = currentSprite.getGlobalBounds().top +
            (currentSprite.getGlobalBounds().height * BULLET_SPAWN_OFFSET_FACTOR);
        createProjectile(spawnPoint, shootingRight);

        shotOnce = true;
    } else {
        shotOnce = false;
    }

    return updateProjectiles(currentLevel);
}

bool Player::checkAnimation()
{
    bool updateAnim = false;

    if (shot || knifed || parachute || retract) {
        //Fast Looped Animation
        if (animTimer.getElapsedTime().asSeconds() > SLOW_ANIMATION_DELAY)
            updateAnim = true;
    } else if (!jumped && (aManager.getCurrentAnimation() == PlayerAnim::PARACHUTE_LEFT ||
                           aManager.getCurrentAnimation() == PlayerAnim::PARACHUTE_RIGHT)) {
        //If Finished Jumping
        updateAnim = true;
    } else if (xSpeed > 0 && !facingRight) {
        //If Changing Direction From Left To Right
        updateAnim = true;
    } else if (xSpeed < 0 && facingRight) {
        //If Changing Direction From Right To Left
        updateAnim = true;
    } else if (!updateAnim && animTimer.getElapsedTime().asSeconds() > ANIMATION_DELAY) {
        //Normal Looped Animation Ex: Run
        updateAnim = true;
    }

    if (updateAnim) {
        updateAnimation();

        translateCoords();
        updateState();

        animTimer.restart();
    }

    return updateAnim;
}

bool Player::applySpeed(Level& currentLevel)
{
    bool scroll = false;
    Level::BoundType horiz, vert;

    //Should Make Separate Functions For Checking Touched Bounds And Fixing Bounds In Future
    vert = currentLevel.boundsCheck(currentSprite, VERTICAL);
    //Handle Vertical Movement
    if ((vert == Level::NO_BOUND) || (vert == Level::TOP_BOUND && ySpeed >= 0) || (vert == Level::BOTTOM_BOUND)) {
        yCoord += ySpeed;
        if (ySpeed < 0) {
            jumped = true;
            ySpeed += ASCEND_GRAVITY;
        } else {
            if (parachute) {
                ySpeed += DESCEND_GRAVITY / 5;
            } else {
                ySpeed += DESCEND_GRAVITY;
            }
            falling = true;
        }
    }

    horiz = currentLevel.boundsCheck(currentSprite, HORIZONTAL);
    //Handle Horizontal Movement
    if ((horiz == Level::NO_BOUND) || (horiz == Level::RIGHT_BOUND && xSpeed < 0) || (horiz == Level::LEFT_BOUND && xSpeed > 0)) {
        if (xSpeed <= 0 && xCoord <= scrollOffset - DEFAULT_SCROLL) {
            //Left Side Of Screen
            xCoord = scrollOffset - DEFAULT_SCROLL;
        } else if (xCoord + currentSprite.getGlobalBounds().width <= scrollOffset || xSpeed < 0) {
            //Between Left Side Of Screen And The Scroll Point
            xCoord += xSpeed;
        } else if (xCoord + currentSprite.getGlobalBounds().width >= scrollOffset && xSpeed > 0) {
            //Scroll Point
            scrollOffset += xSpeed;
            xCoord += xSpeed;
            scroll = true;
        }
    }

    currentSprite.setPosition(xCoord, yCoord);

    currentLevel.boundsCheck(currentSprite, HORIZONTAL);
    vert = currentLevel.boundsCheck(currentSprite, VERTICAL);
    //Update Movement States
    if (vert == Level::BOTTOM_BOUND) {
        if (parachute) {
            retract = true;
            parachute = false;
        }
        jumped = false;
        falling = false;
        ySpeed = 0;
    } else if (vert == Level::TOP_BOUND) {
        ySpeed = VERT_COLLISION_GRAVITY;
    }

    return scroll;
}

bool Player::checkKnifed()
{
    //Check For Mid Knife Animation
    if (knifed) {
        int lastOff = aManager.getBounds(aManager.getCurrentAnimation()).last -
                      aManager.getBounds(aManager.getCurrentAnimation()).first;

        if (aManager.getCurrentOffset() == lastOff)
            return true;
    }

    return false;
}

void Player::updateState()
{
    int cAnimIndex = aManager.getBounds(aManager.getCurrentAnimation()).first + aManager.getCurrentOffset();

    if (retract && cAnimIndex == aManager.getBounds(aManager.getCurrentAnimation()).first) {
        //Last Retract Animation (First Parachute)
        retract = false;
        stateLock = false;
    } else if (knifed && cAnimIndex == aManager.getBounds(aManager.getCurrentAnimation()).last) {
        //Last Knife Animation
        knifed = false;
        stateLock = false;
    } else {
        //Check Other Cases
        Character::updateState();
    }
}

bool Player::updateCoordTracking()
{
    int cAnimIndex = aManager.getBounds(aManager.getCurrentAnimation()).first + aManager.getCurrentOffset();
    int lAnimIndex = aManager.getBounds(aManager.getLastAnimation()).first + aManager.getLastOffset();

    if (cAnimIndex == lAnimIndex)
        return false;

    lastTracking = currentTracking;

    //Special Cases
    if (lAnimIndex == aManager.getBounds(PlayerAnim::KNIFE_LEFT).last) {
        currentTracking = BOTTOM_RIGHT;
    } else if (lAnimIndex == aManager.getBounds(PlayerAnim::KNIFE_RIGHT).last) {
        currentTracking = BOTTOM_LEFT;
    } else {
        //Other Cases
        if (parachute || retract) {
            if (cAnimIndex != aManager.getBounds(PlayerAnim::PARACHUTE_RIGHT).last && cAnimIndex != aManager.getBounds(PlayerAnim::PARACHUTE_LEFT).last) {
                if (facingRight) {
                    currentTracking = BOTTOM_RIGHT;
                } else {
                    currentTracking = BOTTOM_LEFT;
                }
            } else {
                currentTracking = CENTER;
            }
        } else if (knifed) {
            if (facingRight) {
                currentTracking = BOTTOM_LEFT;
            } else {
                currentTracking = BOTTOM_RIGHT;
            }
        } else {
            return Character::updateCoordTracking();
        }
    }

    return true;
}

bool Player::updateAnimation()
{
    int cAnim = aManager.getCurrentAnimation(),
        cOff = aManager.getCurrentOffset(),
        cAnimIndex = aManager.getBounds(cAnim).first + cOff;

    lastSprite = currentSprite;

    if (parachute || retract) {
            if (cAnim != PlayerAnim::PARACHUTE_LEFT && cAnim != PlayerAnim::PARACHUTE_RIGHT) {
                if (facingRight) {
                    aManager.setTexture(PlayerAnim::PARACHUTE_RIGHT);
                } else {
                    aManager.setTexture(PlayerAnim::PARACHUTE_LEFT);
                }
            } else if (cAnimIndex != aManager.getBounds(cAnim).last) {
                if (xSpeed > 0 && cAnim == PlayerAnim::PARACHUTE_LEFT) {
                    aManager.setTexture(PlayerAnim::PARACHUTE_RIGHT, ++cOff);
                } else if (xSpeed < 0 && cAnim == PlayerAnim::PARACHUTE_RIGHT) {
                    aManager.setTexture(PlayerAnim::PARACHUTE_LEFT, ++cOff);
                } else {
                    (parachute) ? aManager.setNextTexture() : aManager.setTexture(cAnim, --cOff);
                }
            } else {
                if (retract) {
                    aManager.setTexture(cAnim, --cOff);
                } else {
                    if (xSpeed > 0) {
                        int lastOffset = aManager.getBounds(PlayerAnim::PARACHUTE_RIGHT).last - aManager.getBounds(PlayerAnim::PARACHUTE_RIGHT).first;
                        aManager.setTexture(PlayerAnim::PARACHUTE_RIGHT, lastOffset);
                    } else if (xSpeed < 0) {
                        int lastOffset = aManager.getBounds(PlayerAnim::PARACHUTE_LEFT).last - aManager.getBounds(PlayerAnim::PARACHUTE_LEFT).first;
                        aManager.setTexture(PlayerAnim::PARACHUTE_LEFT, lastOffset);
                    }
                }
            }
    } else if (knifed) {
        if (cAnim != PlayerAnim::KNIFE_LEFT && cAnim != PlayerAnim::KNIFE_RIGHT) {
            if (facingRight) {
                aManager.setTexture(PlayerAnim::KNIFE_RIGHT);
            } else {
                aManager.setTexture(PlayerAnim::KNIFE_LEFT);
            }
        } else {
            if (aManager.getBounds(cAnim).first + cOff == aManager.getBounds(cAnim).last) {
                if (facingRight) {
                    aManager.setTexture(PlayerAnim::KNIFE_RIGHT);
                } else {
                    aManager.setTexture(PlayerAnim::KNIFE_LEFT);
                }
            } else {
                aManager.setNextTexture();

                //This only needs to be done for "blocking" animations that cannot go from left to right or vice versa mid animation
                if (aManager.getCurrentAnimation() == PlayerAnim::KNIFE_RIGHT)
                    facingRight = true;
                else
                    facingRight = false;
            }
        }
    } else {
        Character::updateAnimation();
    }

    currentSprite.setTexture(aManager.getCurrentTexture(), true);

    return true;
}

int Player::getDefaultRunSpeed() const
{
    return DEFAULT_RUN_SPEED;
}
