#include "Player.h"
#include <Level.h>

using namespace std;

const string Player::SPRITE_MANIFEST("~Manifest.txt"), Player::SPRITE_PATH("data/sprites/"), Player::SPRITE_FORMAT(".png");
const sf::Vector2f Player::SPRITE_SCALE(2.0, 2.0), Player::STARTING_POSITION(200, 200);

Player::Player() : currentAnimation(STAND_RIGHT), lastAnimation(currentAnimation), currentTracking(BOTTOM_RIGHT), lastTracking(currentTracking),
                   scrollOffset(DEFAULT_SCROLL), xSpeed(0), ySpeed(0), jumped(false), shot(false), knifed(false), falling(true), parachute(false),
                   retract(false)
{
    ifstream spriteManifest;
    string fileName;
    int animCount = 0;

    spriteManifest.open((SPRITE_PATH + SPRITE_MANIFEST).c_str());

    while (spriteManifest >> fileName) {
        playerTextures[animCount].loadFromFile(SPRITE_PATH + fileName + SPRITE_FORMAT);
        animCount++;
    }

    spriteManifest.close();

    currentSprite.setPosition(STARTING_POSITION);

    currentSprite.setTexture(playerTextures[currentAnimation], true);
    currentSprite.setScale(SPRITE_SCALE);
    lastSprite.setTexture(playerTextures[lastAnimation], true);
    lastSprite.setScale(SPRITE_SCALE);

    xCoord = currentSprite.getGlobalBounds().left;
    yCoord = currentSprite.getGlobalBounds().top;
}

Player::~Player()
{
    //dtor
}

void Player::runRight()
{
    xSpeed = RUN_SPEED;
}

void Player::runLeft()
{
    xSpeed = -RUN_SPEED;
}

void Player::jump()
{
    if (!jumped && !parachute) {
        ySpeed = JUMP_SPEED;
        jumped = true;
    }
}

void Player::deployParachute()
{
    if (jumped || falling) {
        parachute = true;
        retract = false;
    }
}

void Player::retractParachute()
{
    parachute = false;
    retract = true;
}

void Player::knife()
{
    if (!shot && !parachute) {
        knifed = true;
    }
}

void Player::shoot()
{
    if (!knifed && !parachute) {
        shot = true;
    }
}

bool Player::updatePlayer(Level& currentLevel)
{
    bool scroll = false, updatedAnimation = true;
    Level::BoundType vert, horiz;

    //Update Animation Immediately Or Wait
    if (knifed || shot || parachute || retract) {
        //If Knifing, Shooting, Or Parachuting
        updateAnimation();
        animationWait.restart();
    } else if (!jumped && currentAnimation >= PARACHUTE_LEFT && currentAnimation <= END_PARACHUTE_RIGHT) {
        //If Finished Jumping
        updateAnimation();
        animationWait.restart();
    } else if ((xSpeed > 0 && currentAnimation >= RUN_LEFT && currentAnimation < RUN_RIGHT) || currentAnimation == STAND_LEFT ) {
        //If Changing Direction From Left To Right
        updateAnimation();
        animationWait.restart();
    } else if ((xSpeed < 0 && currentAnimation >= RUN_RIGHT && currentAnimation < STAND_LEFT) || currentAnimation == STAND_RIGHT) {
        //If Changing Direction From Right To Left
        updateAnimation();
        animationWait.restart();
    } else if (animationWait.getElapsedTime().asSeconds() > ANIMATION_DELAY) {
        //After ANIMATION_DELAY Update Looped Animation
        updateAnimation();
        animationWait.restart();
    } else {
        updatedAnimation = false;
    }

    //So That The Position Of A Sprite More Than One Iteration Old Is Not Used
    if (updatedAnimation) {
        translateCoords();
        updateAttackStates();
    }

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
    //Horizontal Movement
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

bool Player::updateProjectiles(Level& currentLevel)
{
    if (currentAnimation == SHOOT_LEFT + GUNFIRE_FRAME || currentAnimation == SHOOT_RIGHT + GUNFIRE_FRAME) {
        int startingX, startingY;
        bool shotRight;

        if (currentAnimation == SHOOT_LEFT + GUNFIRE_FRAME) {
            startingX = currentSprite.getGlobalBounds().left;
            shotRight = false;
        } else {
            startingX = currentSprite.getGlobalBounds().left + currentSprite.getGlobalBounds().width;
            shotRight = true;
        }
        startingY = currentSprite.getGlobalBounds().top + (currentSprite.getGlobalBounds().height * BULLET_SPAWN_FACTOR);

        currentProjectiles.addProjectile(sf::Vector2f(startingX, startingY), shotRight);
    }

    if (currentProjectiles.moveProjectiles(currentLevel, currentSprite.getGlobalBounds())) {
        return true;
    }

    return false;
}

const std::vector<sf::Sprite>& Player::getProjectiles() const
{
    return currentProjectiles.getProjectiles();
}

const sf::Sprite& Player::getSprite() const
{
    return currentSprite;
}

int Player::getRunSpeed() const
{
    return RUN_SPEED;
}

void Player::updateAnimation()
{
    int currentIndex = static_cast<int>(currentAnimation);
    lastAnimation = currentAnimation;
    lastSprite = currentSprite;

    if (parachute || retract) {
        if (currentAnimation < PARACHUTE_LEFT || currentAnimation > END_PARACHUTE_RIGHT) {
            //Transition To First Parachute Animation
            if (xSpeed > 0 || currentAnimation == STAND_RIGHT || currentAnimation == END_SHOOT_RIGHT || currentAnimation == END_KNIFE_RIGHT) {
                currentAnimation = PARACHUTE_RIGHT;
            } else {
                currentAnimation = PARACHUTE_LEFT;
            }
        } else if (currentAnimation != END_PARACHUTE_LEFT && currentAnimation != END_PARACHUTE_RIGHT) {
            //Transition From Left To Right Or Vice Versa While Still Advancing Animation
            if (xSpeed > 0 && currentAnimation >= PARACHUTE_LEFT && currentAnimation <= END_PARACHUTE_LEFT) {
                currentIndex += PARACHUTE_LEFT_TO_RIGHT;
            } else if (xSpeed < 0 && currentAnimation >= PARACHUTE_RIGHT && currentAnimation <= END_PARACHUTE_RIGHT) {
                currentIndex += PARACHUTE_RIGHT_TO_LEFT;
            } else {
                (parachute) ? currentIndex++ : currentIndex--;
            }
            currentAnimation = static_cast<PlayerAnimation>(currentIndex);
        } else {
            if (retract) {
                //Start Retracting Parachute
                currentIndex--;
                currentAnimation = static_cast<PlayerAnimation>(currentIndex);
            } else {
                //Transition Between The Left And Right Last Animation While In The Air
                if (xSpeed > 0) {
                    currentAnimation = END_PARACHUTE_RIGHT;
                } else if (xSpeed < 0) {
                    currentAnimation = END_PARACHUTE_LEFT;
                } //Equal To Zero, Animation Stays The Same
            }
        }
    } else if (shot) {
        if (currentAnimation < SHOOT_LEFT || currentAnimation > END_SHOOT_RIGHT) {
            //Transition To First Shoot Animation
            if (xSpeed > 0 || currentAnimation == STAND_RIGHT || currentAnimation == END_KNIFE_RIGHT || currentAnimation == END_PARACHUTE_RIGHT || currentAnimation == PARACHUTE_RIGHT) {
                currentAnimation = SHOOT_RIGHT;
            } else {
                currentAnimation = SHOOT_LEFT;
            }
        } else {
            //Transition To First Animation Or Advance Animation
            if (lastAnimation == END_SHOOT_LEFT && xSpeed <= 0) {
                currentAnimation = SHOOT_LEFT;
            } else if (lastAnimation == END_SHOOT_RIGHT && xSpeed >= 0) {
                currentAnimation = SHOOT_RIGHT;
            } else {
                currentIndex++;
                currentAnimation = static_cast<PlayerAnimation>(currentIndex);
            }
        }
    } else if (knifed) {
        if (currentAnimation < KNIFE_LEFT || currentAnimation > END_KNIFE_RIGHT) {
            //Transition To First Knife Animation
            if (xSpeed > 0 || currentAnimation == STAND_RIGHT || currentAnimation == END_SHOOT_RIGHT || currentAnimation == END_PARACHUTE_RIGHT || currentAnimation == PARACHUTE_RIGHT) {
                currentAnimation = KNIFE_RIGHT;
            } else {
                currentAnimation = KNIFE_LEFT;
            }
        } else {
            //Transition To First Animation Or Advance Animation
            if (lastAnimation == END_KNIFE_LEFT && xSpeed <= 0) {
                currentAnimation = KNIFE_LEFT;
            } else if (lastAnimation == END_KNIFE_RIGHT && xSpeed >= 0) {
                currentAnimation = KNIFE_RIGHT;
            } else {
                currentIndex++;
                currentAnimation = static_cast<PlayerAnimation>(currentIndex);
            }
        }
    } else {
        if (xSpeed > 0) {
            //Running Right
            if (currentAnimation < RUN_RIGHT || currentAnimation > END_RUN_RIGHT) {
                currentAnimation = RUN_RIGHT;
            } else {
                //Transition To First Animation Or Advance Animation
                if (currentAnimation == END_RUN_RIGHT) {
                    currentAnimation = RUN_RIGHT;
                } else {
                    currentIndex++;
                    currentAnimation = static_cast<PlayerAnimation>(currentIndex);
                }
            }
        } else if (xSpeed < 0) {
            //Running Left
            if (currentAnimation < RUN_LEFT || currentAnimation > END_RUN_LEFT) {
                currentAnimation = RUN_LEFT;
            } else {
                //Transition To First Animation Or Advance Animation
                if (currentAnimation == END_RUN_LEFT) {
                    currentAnimation = RUN_LEFT;
                } else {
                    currentIndex++;
                    currentAnimation = static_cast<PlayerAnimation>(currentIndex);
                }
            }
        } else {
            //Transition To Standing Position
            if ((currentTracking == BOTTOM_RIGHT && currentAnimation != END_KNIFE_LEFT && currentAnimation != END_SHOOT_LEFT && currentAnimation != STAND_LEFT)
                || currentAnimation == END_PARACHUTE_RIGHT || currentAnimation == END_SHOOT_RIGHT || currentAnimation == END_KNIFE_RIGHT || currentAnimation == STAND_RIGHT) {
                currentAnimation = STAND_RIGHT;
            } else {
                currentAnimation = STAND_LEFT;
            }
        }
    }

    currentSprite.setTexture(playerTextures[currentAnimation], true);
}

void Player::updateAttackStates()
{
    if (currentAnimation == END_KNIFE_LEFT || currentAnimation == END_KNIFE_RIGHT) {
        knifed = false;
    } else if (currentAnimation == END_SHOOT_LEFT || currentAnimation == END_SHOOT_RIGHT) {
        shot = false;
    } else if ((currentAnimation == PARACHUTE_LEFT || currentAnimation == PARACHUTE_RIGHT) && retract) {
        retract = false;
    }
}

void Player::translateCoords()
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

bool Player::updateCoordTracking()
{
    if (lastAnimation == currentAnimation) {
        return false;
    }

    lastTracking = currentTracking;

    //Special Cases
    if (lastAnimation == END_SHOOT_LEFT || lastAnimation == END_KNIFE_LEFT) {
        currentTracking = BOTTOM_RIGHT;
    } else if (lastAnimation == END_SHOOT_RIGHT || lastAnimation == END_KNIFE_RIGHT) {
        currentTracking = BOTTOM_LEFT;
    } else {
        //Other Cases
        if (parachute || retract) {
            if (currentAnimation != END_PARACHUTE_RIGHT && currentAnimation != END_PARACHUTE_LEFT) {
                if (currentAnimation >= PARACHUTE_RIGHT) {
                    //Parachuting Right
                    currentTracking = BOTTOM_RIGHT;
                } else {
                    //Parachuting Left
                    currentTracking = BOTTOM_LEFT;
                }
            } else {
                //Last Parachute Animation Left Or Right
                currentTracking = CENTER;
            }
        } else if (shot || knifed) {
            if (currentAnimation >= KNIFE_RIGHT && currentAnimation <= END_KNIFE_RIGHT) {
                //Knifing Right
                currentTracking = BOTTOM_LEFT;
            } else if (currentAnimation >= SHOOT_RIGHT && currentAnimation <= END_SHOOT_RIGHT) {
                //Shooting Right
                currentTracking = BOTTOM_LEFT;
            } else {
                //Knifing Or Shooting Left
                currentTracking = BOTTOM_RIGHT;
            }
        } else {
            if ((currentAnimation >= RUN_RIGHT && currentAnimation <= END_RUN_RIGHT) || currentAnimation == STAND_RIGHT) {
                //Standing Or Running Right
                currentTracking = BOTTOM_RIGHT;
            } else {
                //Standing Or Running Left
                currentTracking = BOTTOM_LEFT;
            }
        }
    }

    return true;
}
