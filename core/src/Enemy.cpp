#include "Enemy.h"
#include <Level.h>

const sf::Vector2f Enemy::SPRITE_SCALE(1.4, 1.4);

using namespace std;

static const EnemyAnim* getEnemyAnim()
{
    static unique_ptr<EnemyAnim> eAnim(new EnemyAnim);

    return eAnim.get();
}

Enemy::Enemy(sf::Vector2f startingPos) : Character(getEnemyAnim()),
    shotTimer(), animTimer()
{
    aManager.setTexture(EnemyAnim::STAND_RIGHT);

    xSpeed = getDefaultRunSpeed();

    currentSprite.setTexture(aManager.getCurrentTexture(), true);
    currentSprite.setPosition(startingPos);
    currentSprite.setScale(SPRITE_SCALE);

    xCoord = currentSprite.getGlobalBounds().left;
    yCoord = currentSprite.getGlobalBounds().top;
}

Enemy::~Enemy()
{

}

bool Enemy::runOpposite()
{
    if (xSpeed > 0)
        runLeft();
    else if (xSpeed < 0)
        runRight();
    else
        return false;

    return true;
}

void Enemy::stopShoot()
{
    if (shot) {
        shot = false;
        stateLock = false;
    }
}

void Enemy::setPosition(float x, float y)
{
    xCoord = x;
    yCoord = y;
    lastSprite = currentSprite;
    currentSprite.setPosition(xCoord, yCoord);
}

bool Enemy::checkProjectiles(Level& currentLevel)
{
    sf::Vector2f spawnPoint;
    bool shootingRight,
    shootingAnimation = (aManager.getCurrentAnimation() == EnemyAnim::SHOOT_LEFT  || aManager.getCurrentAnimation() == EnemyAnim::SHOOT_RIGHT) &&
                            ((aManager.getBounds(aManager.getCurrentAnimation()).first + aManager.getCurrentOffset()) == aManager.getBounds(aManager.getCurrentAnimation()).last - 1);

    if (shot && shootingAnimation && shotTimer.getElapsedTime().asSeconds() > SHOOT_CD) {
        if (aManager.getCurrentAnimation() == EnemyAnim::SHOOT_LEFT) {
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
    }

    return updateProjectiles(currentLevel);
}

bool Enemy::checkAnimation()
{
    bool updateAnim = false;

    if (shot) {
        if (animTimer.getElapsedTime().asSeconds() > SLOW_ANIMATION_DELAY)
            updateAnim = true;
    } else if (xSpeed > 0 && !facingRight) {
        updateAnim = true;
    } else if (xSpeed < 0 && facingRight) {
        updateAnim = true;
    } else if (!updateAnim && animTimer.getElapsedTime().asSeconds() > ANIMATION_DELAY) {
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

bool Enemy::applySpeed(Level& currentLevel)
{
    //Enemies should not move if performing an action
    if (stateLock) {
        stopRun();
    } else {
        if (facingRight)
            runRight();
        else
            runLeft();
    }

    Level::BoundType horiz, vert;

    //Should Make Separate Functions For Checking Touched Bounds And Fixing Bounds In Future
    vert = currentLevel.boundsCheck(currentSprite, VERTICAL);
    //Handle Vertical Movement
    if (vert == Level::NO_BOUND || vert == Level::TOP_BOUND) {
        yCoord += ySpeed;
        ySpeed += DESCEND_GRAVITY;
    } else {
        ySpeed = 0;
    }

    horiz = currentLevel.boundsCheck(currentSprite, HORIZONTAL);
    //Handle Horizontal Movement
    if (horiz == Level::LEFT_BOUND) {
        runRight();
    } else if (horiz == Level::RIGHT_BOUND) {
        runLeft();
    }

    xCoord += xSpeed;
    yCoord += ySpeed;
    currentSprite.setPosition(xCoord, yCoord);
    currentLevel.boundsCheck(currentSprite, HORIZONTAL);
    currentLevel.boundsCheck(currentSprite, VERTICAL);

    return horiz != Level::NO_BOUND;
}

float Enemy::getBulletSpawnOffset()
{
    return BULLET_SPAWN_OFFSET_FACTOR;
}

void Enemy::updateState()
{
    //Nothing Requried
}

bool Enemy::updateAnimation()
{
    int cAnim = aManager.getCurrentAnimation(),
        cOff = aManager.getCurrentOffset();

    lastSprite = currentSprite;

    //Override Character's handling of a shot event
    if (shot) {
        if (cAnim != EnemyAnim::SHOOT_LEFT && cAnim != EnemyAnim::SHOOT_RIGHT) {
            //Transition To First Shoot Animation
            if (facingRight) {
                aManager.setTexture(EnemyAnim::SHOOT_RIGHT);
            } else {
                aManager.setTexture(EnemyAnim::SHOOT_LEFT);
            }
        } else if (aManager.getBounds(cAnim).first + cOff != aManager.getBounds(cAnim).last) {
            //Transition To Next Animation
            aManager.setNextTexture();

            //This only needs to be done for "blocking" animations that cannot go from left to right or vice versa mid animation
            if (aManager.getCurrentAnimation() == EnemyAnim::SHOOT_RIGHT)
                facingRight = true;
            else
                facingRight = false;
        } else {
            //Repeated Shooting
            aManager.setTexture(aManager.getCurrentAnimation(), aManager.getCurrentOffset() - 1);
        }
    } else {
        //Running Left Or Right
        Character::updateAnimation();
    }

    currentSprite.setTexture(aManager.getCurrentTexture(), true);

    return true;
}

int Enemy::getDefaultRunSpeed() const
{
    return DEFAULT_RUN_SPEED;
}

bool Enemy::getShotStatus()
{
    return shot;
}

bool Enemy::isFacingRight()
{
    return facingRight;
}
