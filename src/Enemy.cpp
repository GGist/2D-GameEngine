#include "Enemy.h"

Enemy::Enemy() : Character(new AnimationManager(new CharacterAnim))
{
    //ctor
}

Enemy::~Enemy()
{
    //dtor
}

bool Enemy::checkAnimation()
{
    return true;
}

bool Enemy::applySpeed(Level& currentLevel)
{
    return true;
}

int Enemy::getDefaultRunSpeed() const
{
    return 1;
}

bool Enemy::checkForDeath()
{
    return true;
}
