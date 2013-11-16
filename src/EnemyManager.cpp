#include "EnemyManager.h"
#include <Level.h>

const std::string EnemyManager::ENEMY_DATA_PATH("/res/level_data/"), EnemyManager::ENEMY_DATA_NAME("Enemy_Coords.txt");

using namespace std;

EnemyManager::EnemyManager(sf::Vector2i windowBounds) : editingMode(false),
    activeEnemies(), enemyCoords(), enemy(sf::Vector2f(0, 0)), wBounds(windowBounds)
{
    enemyBounds.resize(3);
    for (int i = 0; i < enemyBounds.size(); ++i) {
        enemyBounds[i].setFillColor(sf::Color::Transparent);
        enemyBounds[i].setOutlineColor(sf::Color::Black);
        enemyBounds[i].setOutlineThickness(3);
    }
}

EnemyManager::~EnemyManager()
{
    //Write to file
    if (editingMode) {
        ofstream outputFile;

        sortEnemyCoords();

        outputFile.open((ENEMY_DATA_PATH + ENEMY_DATA_NAME).c_str());

        //Insert Coordinates In Descending Order
        for (int i = 0; i < activeEnemies.size(); ++i) {
            outputFile << "(" << enemyCoords[i].x << ", "
                       << enemyCoords[i].y << ")" << endl;
        }

        outputFile.close();
    }
}

bool EnemyManager::loadEnemies(std::string fileName)
{
    ifstream inputFile;
    string temp;
    int x, y;

    inputFile.open(fileName.c_str());

    while (!inputFile.eof()) {
        while (inputFile.peek() != '(' && inputFile.peek() != ' ')
            inputFile.ignore();

        if (inputFile.peek() == '(') {
            inputFile.ignore();
            getline(inputFile, temp, ',');
            x = stringToType<int>(temp);
        } else {
            inputFile.ignore();
            getline(inputFile, temp, ')');
            y = stringToType<int>(temp);
            enemyCoords.push_back(sf::Vector2f(x, x));
            inputFile.ignore();
        }
    }

    inputFile.close();

    return true;
}

bool EnemyManager::setEditorMode(bool mode)
{
    if (!enemyCoords.empty() || !activeEnemies.empty())
        return false;

    editingMode = mode;

    return true;
}

void EnemyManager::clearEnemies()
{
    if (!enemyCoords.empty())
        enemyCoords.clear();

    if (!activeEnemies.empty())
        activeEnemies.clear();
}

bool EnemyManager::addEnemy(sf::Vector2f coord)
{
    //if (!editingMode)
    //    return false;

    //activeEnemies.push_back(Enemy(coord));
    enemyCoords.push_back(coord);

    return true;

}

void EnemyManager::updateEnemies(sf::FloatRect playerBounds, Level& level)
{
    //Push new enemies on activeEnemies
    while (!enemyCoords.empty() && enemyCoords.back().x - playerBounds.left <= wBounds.x) {
        activeEnemies.push_back(Enemy(enemyCoords.back()));
        enemyCoords.pop_back();
    }

    //Check if enemy should shoot at player
    for (int i = 0; i < activeEnemies.size(); ++i) {
        float left = 0, top = 0, width = 0, height = 0;
        sf::FloatRect currBound = activeEnemies[i].getSprite().getGlobalBounds();

        if (activeEnemies[i].isFacingRight()) {
            if (playerBounds.left < currBound.left)
                continue;
            left = (currBound.left + currBound.width);
            width = playerBounds.left - (currBound.left + currBound.width) + 0.1;
        } else {
            if (playerBounds.left > currBound.left)
                continue;
            left = (playerBounds.left + playerBounds.width) - 0.1;
            width = (currBound.left) - (playerBounds.left + playerBounds.width);
        }
        top = currBound.top + (currBound.height * activeEnemies[i].getBulletSpawnOffset());
        height = 5;

        sf::FloatRect shotBound(left, top, width, height);

        enemyBounds[i].setSize(sf::Vector2f(width, height));
        enemyBounds[i].setPosition(left, top);

        if (!level.boundsCheck(shotBound) && playerBounds.intersects(shotBound)) {
            activeEnemies[i].shoot();
        } else {
            activeEnemies[i].stopShoot();

        }
    }

    //Update each enemy
    for (int i = 0; i < activeEnemies.size(); ++i) {
        activeEnemies[i].checkAnimation();
        activeEnemies[i].applySpeed(level);
        activeEnemies[i].checkProjectiles(level);
    }
}

void EnemyManager::drawEnemies(sf::RenderWindow& renderWindow)
{
    for (int i = 0; i < enemyBounds.size(); ++i)
        renderWindow.draw(enemyBounds[i]);

    for (int i = 0; i < activeEnemies.size(); ++i) {
        renderWindow.draw(activeEnemies[i].getSprite());
        activeEnemies[i].drawProjectiles(renderWindow);
    }
}

Enemy& EnemyManager::getSampleEnemy()
{
    return enemy;
}

void EnemyManager::sortEnemyCoords()
{
    sf::Vector2f temp;
    int index;

    //Insertion Sort
    for (int i = 1; i < enemyCoords.size(); ++i) {
        index = i;
        for (int j = 0; j < i; ++j) {
            if (enemyCoords[j].x <= enemyCoords[i].x) {
                if (enemyCoords[j].x != enemyCoords[i].x) {
                    index = j;
                } else {
                    if (enemyCoords[j].y < enemyCoords[i].y)
                        index = j;
                }
            }
        }
        if (index != i) {
            temp = enemyCoords[i];
            enemyCoords[i] = enemyCoords[index];
            enemyCoords[index] = enemyCoords[i];
        }
    }
}

template <typename T>
T EnemyManager::stringToType(std::string convert)
{
    T value;
    istringstream converter(convert);

    converter >> value;

    return value;
}
