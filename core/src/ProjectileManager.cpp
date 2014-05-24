#include "ProjectileManager.h"
#include <Level.h>

using namespace std;

static const BulletAnim* getBulletAnim()
{
    static BulletAnim* bAnim = new BulletAnim;

    return bAnim;
}

ProjectileManager::ProjectileManager() : numProjectiles(0), aManager(getBulletAnim())
{
    aManager.setTexture(BulletAnim::BULLET);
    bullet.setTexture(aManager.getCurrentTexture());
}

ProjectileManager::~ProjectileManager()
{

}

void ProjectileManager::addProjectile(sf::Vector2f starting, bool right)
{
    bullet.setPosition(starting);
    currentProjectiles.push_back(Projectile(bullet, right));

    currentProjectiles[numProjectiles].proj.setScale(SCALE, SCALE);
    numProjectiles++;
}

bool ProjectileManager::moveProjectiles(Level& currentLevel, sf::FloatRect playerBounds)
{
    if (currentProjectiles.empty())
        return false;

    int index = 0;

    //Note: Access to member variables works inside lambdas because internally they are called through the *this pointer
    for_each(currentProjectiles.begin(), currentProjectiles.end(), [&currentLevel, &playerBounds, &index, this] (Projectile& projectile) {
        if (projectile.proj.getTexture() == &aManager.getTexture(BulletAnim::BULLET)) {
            //Still Going Through The Air
            if (projectile.goingRight) {
                //Going Right
                projectile.proj.move(SPEED, 0);
            } else {
                //Going Left
                projectile.proj.move(-(SPEED), 0);
            }

            //Check If Off Screen Or Collision
            if (abs(projectile.proj.getGlobalBounds().left - playerBounds.left) > SCREEN_WIDTH) {
                currentProjectiles.erase(currentProjectiles.begin() + index);
                numProjectiles--;
            } else if (currentLevel.boundsCheck(projectile.proj, false) != Level::NO_BOUND) {
                projectile.proj.setTexture(*(projectile.proj.getTexture() + 1));
            }

        } else if (projectile.offset != aManager.getBounds(BulletAnim::BULLET).last - 1) {
                //Currently In One Of The Destruction Animations
                ++projectile.offset;
                 projectile.proj.setTexture(aManager.getTexture(BulletAnim::BULLET, projectile.offset));
        } else {
                //On Last Animation, Delete The Sprite And Direction Bool
                currentProjectiles.erase(currentProjectiles.begin() + index);
                --numProjectiles;
        }
        index++;
    });

    return true;
}

const std::vector<Projectile>& ProjectileManager::getProjectiles() const
{
    return currentProjectiles;
}

bool ProjectileManager::boundsCheck(const sf::FloatRect& entity)
{
    bool collision = false;

    //Move projectiles that are to be deleted to the end
    auto new_end = remove_if(currentProjectiles.begin(), currentProjectiles.end(),
                             [&entity, &collision, this] (const Projectile& projectile) {
                             if (projectile.proj.getGlobalBounds().intersects(entity)) {
                                collision = true;
                                --numProjectiles;
                                return true;
                            }

                            return false;
                            });

    //Erase the projectiles
    currentProjectiles.erase(new_end, currentProjectiles.end());

    return collision;
}
