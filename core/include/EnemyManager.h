/***********************************************************************************
*This class is used as an interface to creating and controlling Enemy objects. This*
*class will accept state from the outside and remove enemies accordingly but the   *
*enemies should not be accessibly to outside code, all state checking will be      *
*handled through this object. Automating the enemies has been reduced down to a few*
*function calls. To see this class in use refer to the Test_Engine.cpp file on the *
*github page.                                                                      *
*Full code at https://github.com/GGist/2D-GameEngine                               *
*This code is licensed under the MIT License.                                      *
***********************************************************************************/

#ifndef ENEMYMANAGER_H
#define ENEMYMANAGER_H

#include <Enemy.h>
#include <fstream>
#include <sstream>

class EnemyManager
{
    public:
        EnemyManager(sf::Vector2i windowBounds);
        //Constructor
        virtual ~EnemyManager();
        //Destructor
        bool saveEnemies(std::string fileName);
        //Saves the enemy coordinates to the specified text file
        //Returns false if not in editing mode or enemyCoords is empty
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
        void removeEnemies(const sf::FloatRect& entityBounds);
        //Checks if the any enemies are within entityBounds and deletes them
        //This should be used for melee attacks
        bool checkShot(const sf::FloatRect& entityBounds, ProjectileManager& entityBullets);
        //Removes any enemys that intersects with entityBullets
        //Returns true if any of the enemy's bullets are within the entityBounds
        bool setSampleEnemy(float x, float y);
        //Sets the position of the SampleEnemy
        //Returns false if !editingMode
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
        bool editingMode;
        std::vector<Enemy> activeEnemies;
        std::vector<sf::Vector2f> enemyCoords;
        Enemy enemy;
        sf::Vector2i wBounds;
};

#endif // ENEMYMANAGER_H
