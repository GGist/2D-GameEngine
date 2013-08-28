#include "Projectile.h"
#include <Level.h>

using namespace std;

const string Projectile::SPRITE_MANIFEST("~Manifest.txt"), Projectile::SPRITE_PATH("data/projectiles/"), Projectile::SPRITE_FORMAT(".png");

Projectile::Projectile() : numProjectiles(0), numDirections(0)
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

Projectile::~Projectile()
{

}

void Projectile::addProjectile(sf::Vector2f starting, bool right)
{
    currentProjectiles.push_back(sf::Sprite(projectileTextures[0]));
    currentDirections.push_back(right);

    currentProjectiles[numProjectiles].setPosition(starting);
    currentProjectiles[numProjectiles].setScale(SCALE, SCALE);
    numProjectiles++;
    numDirections++;
}

bool Projectile::moveProjectiles(Level& currentLevel, sf::FloatRect playerBounds)
{
    if (currentProjectiles.empty()) {
        return false;
    }

    int index = 0;

    //Was Familiarizing Myself With Lambdas, Maybe Not The Best Application
        //Note: Access to member variables works inside lambdas because internally they are called through the *this pointer
    for_each(currentProjectiles.begin(), currentProjectiles.end(), [&currentLevel, &playerBounds, &index, this] (sf::Sprite &proj) {
        if (proj.getTexture() == &projectileTextures[0]) {
            //Still Going Through The Air
            if (currentDirections[index]) {
                //Going Right
                proj.move(SPEED, 0);
            } else {
                //Going Left
                proj.move(-(SPEED), 0);
            }

            //Check If Off Screen Or Collision
            if (abs(proj.getGlobalBounds().left - playerBounds.left) > SCREEN_WIDTH) {
                currentProjectiles.erase(currentProjectiles.begin() + index);
                currentDirections.erase(currentDirections.begin() + index);
                numProjectiles--;
                numDirections--;
            } else if (currentLevel.boundsCheck(proj, false) != Level::NO_BOUND) {
                proj.setTexture(*(proj.getTexture() + 1));
            }

        } else if (proj.getTexture() != &projectileTextures[NUM_ANIMATIONS - 1]) {
                //Currently In One Of The Destruction Animations
                 proj.setTexture(*(proj.getTexture() + 1));
        } else {
                //On Last Animation, Delete The Sprite And Direction Bool
                currentProjectiles.erase(currentProjectiles.begin() + index);
                currentDirections.erase(currentDirections.begin() + index);
                numProjectiles--;
                numDirections--;
        }
        index++;
    } );

    return true;
}

const std::vector<sf::Sprite>& Projectile::getProjectiles() const
{
    return currentProjectiles;
}
