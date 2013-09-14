#include "Character.h"
#include <Level.h>

using namespace std;

Character::Character(TextureManager* tManager) : tInterface(tManager), currentProjectiles(), currentTracking(BOTTOM_RIGHT), lastTracking(BOTTOM_RIGHT),
                                                 shot(false), facingRight(true), stateLock(false), xCoord(0), yCoord(0), xSpeed(0), speedModifier(0),
                                                 currentSprite(), lastSprite()
{

}

Character::~Character()
{

}

void Character::runRight()
{
    xSpeed = getDefaultRunSpeed() + speedModifier;
    facingRight = true;
}

void Character::runLeft()
{
    xSpeed = -getDefaultRunSpeed() - speedModifier;
    facingRight = false;
}

void Character::shoot()
{
    if (!stateLock) {
        shot = true;
        stateLock = true;
    }
}

bool Character::updateProjectiles(Level& currentLevel)
{
    if (tInterface == nullptr) {
        return false;
    }

    if (shot && tInterface->getCurrentOffset() == GUNFIRE_OFFSET) {
        int startingX, startingY;
        bool shotRight;

        if (tInterface->getCurrentAnimation() == CharAnim::SHOOT_LEFT) {
            startingX = currentSprite.getGlobalBounds().left;
            shotRight = false;
        } else {
            startingX = currentSprite.getGlobalBounds().left + currentSprite.getGlobalBounds().width;
            shotRight = true;
        }
        startingY = currentSprite.getGlobalBounds().top + (currentSprite.getGlobalBounds().height * BULLET_SPAWN_OFFSET_FACTOR);

        currentProjectiles.addProjectile(sf::Vector2f(startingX, startingY), shotRight);
    }

    if (currentProjectiles.moveProjectiles(currentLevel, currentSprite.getGlobalBounds())) {
        return true;
    }

    return false;
}

bool Character::checkAnimation()
{
    if (tInterface == nullptr) {
        return false;
    }

    bool updatedAnimation = true;

    //Update Animation Immediately Or Wait
    if (shot) {
        //If Shooting
        updateAnimation();
        animationWait.restart();
    } else if (xSpeed > 0 && !facingRight) {
        //If Changing Direction From Left To Right
        updateAnimation();
        animationWait.restart();
    } else if (xSpeed < 0 && facingRight) {
        //If Changing Direction From Right To Left
        updateAnimation();
        animationWait.restart();
    } else {
        updatedAnimation = false;
    }

    //So That The Position Of A Sprite More Than One Iteration Old Is Not Used
    if (updatedAnimation) {
        translateCoords();
        updateState();
    }

    //Good Idea To Update Any Looped Animations If The Return Value Is False
    return updatedAnimation;
}

const std::vector<sf::Sprite>& Character::getProjectiles() const
{
    return currentProjectiles.getProjectiles();
}

const sf::Sprite& Character::getSprite() const
{
    return currentSprite;
}

const int Character::getCurrentRunSpeed() const
{
    return getDefaultRunSpeed() + speedModifier;
}

void Character::updateAnimation()
{
    int cAnim = tInterface->getCurrentAnimation(),
        cOff = tInterface->getCurrentOffset();

    if (shot) {
        if (cAnim != CharAnim::SHOOT_LEFT && cAnim != CharAnim::SHOOT_RIGHT) {
            //Transition To First Shoot Animation
            if (facingRight) {
                tInterface->setTexture(CharAnim::SHOOT_RIGHT);
            } else {
                tInterface->setTexture(CharAnim::SHOOT_LEFT);
            }
        } else {
            //Transition To First Animation Or Advance Animation
            if (tInterface->getBounds(cAnim).first + cOff == tInterface->getBounds(cAnim).last) {
                //From Last Animation To First Animation
                if (facingRight) {
                    tInterface->setTexture(CharAnim::SHOOT_RIGHT);
                } else {
                    tInterface->setTexture(CharAnim::SHOOT_LEFT);
                }
            } else {
                tInterface->setNextTexture();
            }
        }
    } else {
        if (xSpeed > 0) {
            //Running Right
            if (cAnim != CharAnim::RUN_RIGHT) {
                tInterface->setTexture(CharAnim::RUN_RIGHT);
            } else {
                //Transition To First Animation Or Advance Animation
                if (tInterface->getBounds(cAnim).first + cOff == tInterface->getBounds(cAnim).last) {
                    tInterface->setTexture(CharAnim::RUN_RIGHT);
                } else {
                    tInterface->setNextTexture();
                }
            }
        } else if (xSpeed < 0) {
            //Running Left
            if (cAnim != CharAnim::RUN_LEFT) {
                tInterface->setTexture(CharAnim::RUN_LEFT);
            } else {
                //Transition To First Animation Or Advance Animation
                if (tInterface->getBounds(cAnim).first + cOff == tInterface->getBounds(cAnim).last) {
                    tInterface->setTexture(CharAnim::RUN_LEFT);
                } else {
                    tInterface->setNextTexture();
                }
            }
        } else {
            //Transition To Standing Position
            if (facingRight) {
                tInterface->setTexture(CharAnim::STAND_RIGHT);
            } else {
                tInterface->setTexture(CharAnim::STAND_LEFT);
            }
        }
    }
}

void Character::updateState()
{
    int cAnimIndex = tInterface->getBounds(tInterface->getCurrentAnimation()).first + tInterface->getCurrentOffset();

    if (shot && cAnimIndex == tInterface->getBounds(tInterface->getCurrentAnimation()).last) {
        shot = false;
        stateLock = false;
    }
}

void Character::translateCoords()
{
    //Check If/Where To Track Sprite So It Does Not Have Placement Issues
    if (updateCoordTracking()) {
        if (currentTracking == BOTTOM_LEFT) {
            xCoord = (lastSprite.getGlobalBounds().left);
            yCoord = (lastSprite.getGlobalBounds().top + lastSprite.getGlobalBounds().height) - currentSprite.getGlobalBounds().height;
        } else if (currentTracking == CENTER) {
            xCoord = (lastSprite.getGlobalBounds().left + (lastSprite.getGlobalBounds().width / 2)) - currentSprite.getGlobalBounds().width / 2;
            yCoord = (lastSprite.getGlobalBounds().top + (lastSprite.getGlobalBounds().height / 2)) - currentSprite.getGlobalBounds().height / 2;
        } else {
            xCoord = (lastSprite.getGlobalBounds().left + lastSprite.getGlobalBounds().width) - currentSprite.getGlobalBounds().width;
            yCoord = (lastSprite.getGlobalBounds().top + lastSprite.getGlobalBounds().height) - currentSprite.getGlobalBounds().height;
        }
    }
}

bool Character::updateCoordTracking()
{
    int cAnimIndex = tInterface->getBounds(tInterface->getCurrentAnimation()).first + tInterface->getCurrentOffset();
    int lAnimIndex = tInterface->getBounds(tInterface->getLastAnimation()).first + tInterface->getLastOffset();

    if (cAnimIndex == lAnimIndex) {
        return false;
    }

    lastTracking = currentTracking;

    //Special Cases
    if (lAnimIndex == tInterface->getBounds(CharAnim::SHOOT_LEFT).last) {
        currentTracking = BOTTOM_RIGHT;
    } else if (lAnimIndex == tInterface->getBounds(CharAnim::SHOOT_RIGHT).last) {
        currentTracking = BOTTOM_LEFT;
    } else {
        //Other Cases
        if (shot) {
            if (facingRight) {
                currentTracking = BOTTOM_LEFT;
            } else {
                currentTracking = BOTTOM_RIGHT;
            }
        } else {
            if (facingRight) {
                currentTracking = BOTTOM_RIGHT;
            } else {
                currentTracking = BOTTOM_LEFT;
            }
        }
    }

    return true;
}
