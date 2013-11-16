#include "Character.h"
#include <Level.h>

using namespace std;

Character::Character(CharacterAnim* cAnim) : aManager(new AnimationManager(cAnim)),
    pManager(), currentTracking(BOTTOM_RIGHT), lastTracking(BOTTOM_RIGHT), shot(false),
    died(false), facingRight(true), stateLock(false), xCoord(0), yCoord(0), xSpeed(0),
    speedModifier(0), currentSprite(), lastSprite()
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

void Character::stopRun()
{
    xSpeed = 0;
}

void Character::shoot()
{
    if (!stateLock) {
        shot = true;
        stateLock = true;
    }
}

bool Character::isDead()
{
    return died;
}

void Character::drawProjectiles(sf::RenderWindow& renderWindow) const
{
    vector<Projectile> projectiles = pManager.getProjectiles();

    for (int i = 0; i < projectiles.size(); ++i) {
        renderWindow.draw(projectiles[i].proj);
    }
}

const sf::Sprite& Character::getSprite() const
{
    return currentSprite;
}

int Character::getCurrentRunSpeed() const
{
    return xSpeed;
}

void Character::updateState()
{
    int cAnimIndex = aManager->getBounds(aManager->getCurrentAnimation()).first + aManager->getCurrentOffset();

    if (shot && cAnimIndex == aManager->getBounds(aManager->getCurrentAnimation()).last) {
        shot = false;
        stateLock = false;
    }
}

bool Character::updateCoordTracking()
{
    int cAnimIndex = aManager->getBounds(aManager->getCurrentAnimation()).first + aManager->getCurrentOffset();
    int lAnimIndex = aManager->getBounds(aManager->getLastAnimation()).first + aManager->getLastOffset();

    if (cAnimIndex == lAnimIndex) {
        return false;
    }

    lastTracking = currentTracking;

    //Special Cases
    if (lAnimIndex == aManager->getBounds(CharacterAnim::SHOOT_LEFT).last) {
        currentTracking = BOTTOM_RIGHT;
    } else if (lAnimIndex == aManager->getBounds(CharacterAnim::SHOOT_RIGHT).last) {
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

void Character::createProjectile(sf::Vector2f spawnPoint, bool shootingRight)
{
    pManager.addProjectile(spawnPoint, shootingRight);
}

bool Character::updateProjectiles(Level& currentLevel)
{
    //Check for any active projectiles and move them
    if (pManager.moveProjectiles(currentLevel, getSprite().getGlobalBounds()))
        return true;

    return false;
}

bool Character::updateAnimation()
{
    int cAnim = aManager->getCurrentAnimation(),
        cOff = aManager->getCurrentOffset();

    if (shot) {
        if (cAnim != CharacterAnim::SHOOT_LEFT && cAnim != CharacterAnim::SHOOT_RIGHT) {
            //Transition To First Shoot Animation
            if (facingRight) {
                aManager->setTexture(CharacterAnim::SHOOT_RIGHT);
            } else {
                aManager->setTexture(CharacterAnim::SHOOT_LEFT);
            }
        } else {
            //Transition To First Animation Or Advance Animation
            if (aManager->getBounds(cAnim).first + cOff == aManager->getBounds(cAnim).last) {
                //From Last Animation To First Animation
                if (facingRight) {
                    aManager->setTexture(CharacterAnim::SHOOT_RIGHT);
                } else {
                    aManager->setTexture(CharacterAnim::SHOOT_LEFT);
                }
            } else {
                aManager->setNextTexture();

                //This only needs to be done for "blocking" animations that cannot go from left to right or vice versa mid animation
                if (aManager->getCurrentAnimation() == CharacterAnim::SHOOT_RIGHT)
                    facingRight = true;
                else
                    facingRight = false;
            }
        }
    } else {
        if (xSpeed > 0) {
            //Running Right
            if (cAnim != CharacterAnim::RUN_RIGHT) {
                aManager->setTexture(CharacterAnim::RUN_RIGHT);
            } else {
                //Transition To First Animation Or Advance Animation
                if (aManager->getBounds(cAnim).first + cOff == aManager->getBounds(cAnim).last) {
                    aManager->setTexture(CharacterAnim::RUN_RIGHT);
                } else {
                    aManager->setNextTexture();
                }
            }
        } else if (xSpeed < 0) {
            //Running Left
            if (cAnim != CharacterAnim::RUN_LEFT) {
                aManager->setTexture(CharacterAnim::RUN_LEFT);
            } else {
                //Transition To First Animation Or Advance Animation
                if (aManager->getBounds(cAnim).first + cOff == aManager->getBounds(cAnim).last) {
                    aManager->setTexture(CharacterAnim::RUN_LEFT);
                } else {
                    aManager->setNextTexture();
                }
            }
        } else {
            //Transition To Standing Position
            if (facingRight) {
                aManager->setTexture(CharacterAnim::STAND_RIGHT);
            } else {
                aManager->setTexture(CharacterAnim::STAND_LEFT);
            }
        }
    }

    currentSprite.setTexture(aManager->getCurrentTexture(), true);

    return true;
}
