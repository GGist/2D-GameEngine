#include "Enemy.h"
#include <Level.h>

const sf::Vector2f Enemy::SPRITE_SCALE(1.5, 1.5);

Enemy::Enemy(sf::Vector2i windowBounds, sf::Vector2f startingPos) : Character(new EnemyAnim),
    wBounds(sf::Vector2i(0, 0), windowBounds)
{
    aManager->setTexture(EnemyAnim::STAND_RIGHT);

    currentSprite.setTexture(aManager->getCurrentTexture(), true);
    currentSprite.setPosition(startingPos);
    currentSprite.setScale(SPRITE_SCALE);

    xCoord = currentSprite.getGlobalBounds().left;
    yCoord = currentSprite.getGlobalBounds().top;
}

Enemy::~Enemy()
{

}

bool Enemy::checkProjectiles(Level& currentLevel)
{
    sf::Vector2f spawnPoint;
    bool shootingRight,
    lastAnimation = (aManager->getCurrentOffset() ==
                     aManager->getBounds(aManager->getCurrentAnimation()).last);

    if (shotTimer.getElapsedTime().asSeconds() > SHOOT_CD && shot && lastAnimation) {
        if (aManager->getCurrentAnimation() == CharacterAnim::SHOOT_LEFT) {
            spawnPoint.x = getSprite().getGlobalBounds().left;
            shootingRight = false;
        } else {
            spawnPoint.x = getSprite().getGlobalBounds().left + getSprite().getGlobalBounds().width;
            shootingRight = true;
        }
        spawnPoint.y = currentSprite.getGlobalBounds().top +
            (currentSprite.getGlobalBounds().height * BULLET_SPAWN_OFFSET_FACTOR);
        createProjectile(spawnPoint, shootingRight);

        shotTimer.restart();

        return true;
    }

    return updateProjectiles(currentLevel);
}

bool Enemy::checkAnimation()
{

}

bool Enemy::applySpeed(Level& currentLevel)
{
    if (aManager == nullptr)
        return false;

    Level::BoundType horiz;

    horiz = currentLevel.boundsCheck(currentSprite, HORIZONTAL);
    //Handle Horizontal Movement
    if (horiz == Level::LEFT_BOUND) {
        xSpeed = getDefaultRunSpeed();
    } else if (horiz == Level::RIGHT_BOUND) {
        xSpeed = -getDefaultRunSpeed();
    }
    currentSprite.setPosition(xCoord, currentSprite.getPosition().y);

    checkForDeath();

    return horiz != Level::NO_BOUND;
}

bool Enemy::updateAnimation()
{

}

int Enemy::getDefaultRunSpeed() const
{
    return DEFAULT_RUN_SPEED;
}

bool Enemy::checkForDeath()
{
    if (getSprite().getGlobalBounds().top > wBounds.top + wBounds.height) {
        died = true;
    } //else if {
        //Got Shot
    //}

    return died;
}
