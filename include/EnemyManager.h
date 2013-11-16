/***********************************************************************************

***********************************************************************************/

#ifndef ENEMYMANAGER_H
#define ENEMYMANAGER_H

#include <Enemy.h>
#include <sstream>

class EnemyManager
{
    public:
        EnemyManager(sf::Vector2i windowBounds);
        //Constructor
        virtual ~EnemyManager();
        //Destructor
        //If editing mode was enabled it will save out any coordinate
        //data for each enemy to a text file that can be loaded in
        bool loadEnemies(std::string fileName);
        //Loads the enemy coordinates located in fileName
        //Returns false if the file could not be found
        bool setEditorMode(bool mode = false);
        //THIS FUNCTION REQUIRES THE ENEMIES TO HAVE BEEN CLEARED
        //Puts the object in enemy editing mode to accept new enemies
        //Returns false if the enemies have not been cleared
        void clearEnemies();
        //Removes all of the enemies
        bool addEnemy(sf::Vector2f coord);
        //Returns false if an enemy can not be added
        void updateEnemies(sf::FloatRect playerBounds, Level& level);
        //Updates the enemies that are currently on the screen
        void drawEnemies(sf::RenderWindow& renderWindow);
        //Draws the enemies and their projectiles to the renderWindow
        Enemy& getSampleEnemy();
        //Returns a sample enemy textured sprite
    private:
        //Constants
        static const std::string ENEMY_DATA_PATH, ENEMY_DATA_NAME;

        //Functions
        void sortEnemyCoords();
        //Sorts enemyCoords in ascending order
        template <typename T>
        T stringToType(std::string convert);
        //Converts a string to the type specified
        //Returns the converted value

        //Data
        std::vector<sf::RectangleShape> enemyBounds;

        bool editingMode;
        std::vector<Enemy> activeEnemies;
        std::vector<sf::Vector2f> enemyCoords;
        Enemy enemy;
        sf::Vector2i wBounds;
};

#endif // ENEMYMANAGER_H
