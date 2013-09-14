#include "Player.h"
#include <Level.h>

using namespace std;

const sf::Vector2f Player::SPRITE_SCALE(2.0, 2.0), Player::STARTING_POSITION(200, 200);

//Could Add A Factory For TextureManager/AnimCollection Classes Later
Player::Player() : Character(new TextureManager(new PlayerAnim)), ySpeed(0), scrollOffset(DEFAULT_SCROLL),
                   knifed(false), jumped(false), falling(false), parachute(false), retract(false)
{
    tInterface->setTexture(PlayerAnim::STAND_RIGHT);

    currentSprite.setTexture(tInterface->getCurrentTexture(), true);
    currentSprite.setPosition(STARTING_POSITION);
    currentSprite.setScale(SPRITE_SCALE);

    xCoord = currentSprite.getGlobalBounds().left;
    yCoord = currentSprite.getGlobalBounds().top;
}

Player::~Player()
{
    if (tInterface != nullptr) {
        delete tInterface;
        tInterface = nullptr;
    }
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
        stateLock = false;
        retract = true;
    }
}

bool Player::checkAnimation()
{
    if (tInterface == nullptr) {
        return false;
    }

    int pAnim = tInterface->getCurrentAnimation();
    bool updated;

    //Update Animation Immediately Or Wait
    if (parachute || retract || knifed) {
        updateAnimation();
        animationWait.restart();

        translateCoords();
        updateState();
    }  else if (!jumped && (pAnim == PlayerAnim::PARACHUTE_LEFT || pAnim == PlayerAnim::PARACHUTE_RIGHT)) {
        //If Finished Jumping
        Character::updateAnimation();
        animationWait.restart();

        translateCoords();
        updateState();
    } else {
        //Other Cases
        updated = Character::checkAnimation();
    }

    if (!updated && animationWait.getElapsedTime().asSeconds() > ANIMATION_DELAY) {
        //After ANIMATION_DELAY Update Looped Animation
        updateAnimation();
        animationWait.restart();

        translateCoords();
        updateState();
    }

    return true;
}

bool Player::applySpeed(Level& currentLevel)
{
    if (tInterface == nullptr) {
        return false;
    }

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
    xSpeed = 0;

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

int Player::getDefaultRunSpeed() const
{
    return DEFAULT_RUN_SPEED;
}

void Player::updateAnimation()
{
    int cAnim = tInterface->getCurrentAnimation(),
        cOff = tInterface->getCurrentOffset(),
        cAnimIndex = tInterface->getBounds(cAnim).first + cOff;

        lastSprite = currentSprite;

    if (parachute || retract) {
            if (cAnim != PlayerAnim::PARACHUTE_LEFT && cAnim != PlayerAnim::PARACHUTE_RIGHT) {
                if (facingRight) {
                    tInterface->setTexture(PlayerAnim::PARACHUTE_RIGHT);
                } else {
                    tInterface->setTexture(PlayerAnim::PARACHUTE_LEFT);
                }
            } else if (cAnimIndex != tInterface->getBounds(cAnim).last) {
                if (xSpeed > 0 && cAnim == PlayerAnim::PARACHUTE_LEFT) {
                    tInterface->setTexture(PlayerAnim::PARACHUTE_RIGHT, ++cOff);
                } else if (xSpeed < 0 && cAnim == PlayerAnim::PARACHUTE_RIGHT) {
                    tInterface->setTexture(PlayerAnim::PARACHUTE_LEFT, ++cOff);
                } else {
                    (parachute) ? tInterface->setNextTexture() : tInterface->setTexture(cAnim, --cOff);
                }
            } else {
                if (retract) {
                    tInterface->setTexture(cAnim, --cOff);
                } else {
                    if (xSpeed > 0) {
                        int lastOffset = tInterface->getBounds(PlayerAnim::PARACHUTE_RIGHT).last - tInterface->getBounds(PlayerAnim::PARACHUTE_RIGHT).first;
                        tInterface->setTexture(PlayerAnim::PARACHUTE_RIGHT, lastOffset);
                    } else if (xSpeed < 0) {
                        int lastOffset = tInterface->getBounds(PlayerAnim::PARACHUTE_LEFT).last - tInterface->getBounds(PlayerAnim::PARACHUTE_LEFT).first;
                        tInterface->setTexture(PlayerAnim::PARACHUTE_LEFT, lastOffset);
                    }
                }
            }
    } else if (knifed) {
        if (cAnim != PlayerAnim::KNIFE_LEFT && cAnim != PlayerAnim::KNIFE_RIGHT) {
            if (facingRight) {
                tInterface->setTexture(PlayerAnim::KNIFE_RIGHT);
            } else {
                tInterface->setTexture(PlayerAnim::KNIFE_LEFT);
            }
        } else {
            if (tInterface->getBounds(cAnim).first + cOff == tInterface->getBounds(cAnim).last) {
                if (facingRight) {
                    tInterface->setTexture(PlayerAnim::KNIFE_RIGHT);
                } else {
                    tInterface->setTexture(PlayerAnim::KNIFE_LEFT);
                }
            } else {
                tInterface->setNextTexture();
            }
        }
    } else {
        Character::updateAnimation();
    }

    currentSprite.setTexture(tInterface->getCurrentTexture(), true);
}

void Player::updateState()
{
    int cAnimIndex = tInterface->getBounds(tInterface->getCurrentAnimation()).first + tInterface->getCurrentOffset();

    if (retract && cAnimIndex == tInterface->getBounds(tInterface->getCurrentAnimation()).first) {
        retract = false;
        stateLock = false;
    } else if (knifed && cAnimIndex == tInterface->getBounds(tInterface->getCurrentAnimation()).last) {
        knifed = false;
        stateLock = false;
    } else {
        //Check Other Cases
        Character::updateState();
    }
}

bool Player::updateCoordTracking()
{
    int cAnimIndex = tInterface->getBounds(tInterface->getCurrentAnimation()).first + tInterface->getCurrentOffset();
    int lAnimIndex = tInterface->getBounds(tInterface->getLastAnimation()).first + tInterface->getLastOffset();

    if (cAnimIndex == lAnimIndex) {
        return false;
    }

    lastTracking = currentTracking;

    //Special Cases
    if (lAnimIndex == tInterface->getBounds(PlayerAnim::KNIFE_LEFT).last) {
        currentTracking = BOTTOM_RIGHT;
    } else if (lAnimIndex == tInterface->getBounds(PlayerAnim::KNIFE_RIGHT).last) {
        currentTracking = BOTTOM_LEFT;
    } else {
        //Other Cases
        if (parachute || retract) {
            if (cAnimIndex != tInterface->getBounds(PlayerAnim::PARACHUTE_RIGHT).last && cAnimIndex != tInterface->getBounds(PlayerAnim::PARACHUTE_LEFT).last) {
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
