#include "ProjectileManager.h"
#include <Level.h>

using namespace std;

const string ProjectileManager::SPRITE_MANIFEST("~Manifest.txt"), ProjectileManager::SPRITE_PATH("res/projectiles/"), ProjectileManager::SPRITE_FORMAT(".png");

ProjectileManager::ProjectileManager() : numProjectiles(0)
{
    ifstream projectileManifest;
    string fileName;
    int animCount = 0;

    projectileManifest.open((SPRITE_PATH + SPRITE_MANIFEST).c_str());

    while (projectileManifest >> fileName) {
        projectileTextures[animCount].loadFromFile(SPRITE_PATH + fileName + SPRITE_FORMAT);
        animCount++;
    }

    projectileManifest.close();
}

ProjectileManager::~ProjectileManager()
{

}

void ProjectileManager::addProjectile(sf::Vector2f starting, bool right)
{
    currentProjectiles.push_back(Projectile(sf::Sprite(projectileTextures[0]), right));

    currentProjectiles[numProjectiles].proj.setPosition(starting);
    currentProjectiles[numProjectiles].proj.setScale(SCALE, SCALE);
    numProjectiles++;
}

bool ProjectileManager::moveProjectiles(Level& currentLevel, sf::FloatRect playerBounds)
{
    if (currentProjectiles.empty()) {
        return false;
    }

    int index = 0;

    //Was Familiarizing Myself With Lambdas, Maybe Not The Best Application
        //Note: Access to member variables works inside lambdas because internally they are called through the *this pointer
    for_each(currentProjectiles.begin(), currentProjectiles.end(), [&currentLevel, &playerBounds, &index, this] (Projectile& projectile) {
        if (projectile.proj.getTexture() == &projectileTextures[0]) {
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

        } else if (projectile.proj.getTexture() != &projectileTextures[NUM_ANIMATIONS - 1]) {
                //Currently In One Of The Destruction Animations
                 projectile.proj.setTexture(*(projectile.proj.getTexture() + 1));
        } else {
                //On Last Animation, Delete The Sprite And Direction Bool
                currentProjectiles.erase(currentProjectiles.begin() + index);
                numProjectiles--;
        }
        index++;
    });

    return true;
}

const std::vector<Projectile>& ProjectileManager::getProjectiles() const
{
    return currentProjectiles;
}

bool ProjectileManager::boundsCheck(sf::Sprite& entity)
{
    bool collision = false;

    remove_if(currentProjectiles.begin(), currentProjectiles.end(), [&entity, &collision, this] (const Projectile& projectile) {
        if (projectile.proj.getGlobalBounds().intersects(entity.getGlobalBounds()))
            collision = true;

        return collision;
    });

    return collision;
}
