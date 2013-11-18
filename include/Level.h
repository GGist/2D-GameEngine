/***********************************************************************************
Redo the class functions to make use of the new deque data structure being using to
hold the tiles. (random access is available)
***********************************************************************************/

#ifndef LEVEL_H
#define LEVEL_H

#include <Player.h>
#include <queue>
#include <sstream>

class Level
{
    public:
        Level(sf::Vector2i windowRes);
        //Constructor
        virtual ~Level();
        //Destructor
        //If editing mode was enabled it will save out any coordinate
        //data for each tile to a text file that can be loaded in
        bool loadLevel(std::string fileName);
        //Loads the level from a file
        //Returns false if file could not be found
        void clearLevel();
        //Clears the current level
        bool setEditorMode(bool mode = false);
        //THIS FUNCTION REQUIRES THE LEVEL TO HAVE BEEN CLEARED
        //Puts the object in level editing mode to accept tiles through addTile
        //Returns false if the level has not been cleared
        bool addTile(sf::Vector2f coord);
        //THIS FUNCTION WILL ONLY WORK IF editingMode IS TRUE
        //Adds a vector to tileCoords
        //Returns false if the vector could not be added due to memory issues or editingMode
        void updateLevel(sf::FloatRect playerBound);
        //Updates the coordinates in tileCoords and sets the first coordinate in the queue
        void drawLevel(sf::RenderWindow& renderWindow);
        //Draws the current tiles to the renderWindow
        const sf::Sprite& getSampleTile() const;
        //Returns the Sprite tile

        enum BoundType {
            TOP_BOUND,
            BOTTOM_BOUND,
            LEFT_BOUND,
            RIGHT_BOUND,
            NO_BOUND
        };
        Level::BoundType boundsCheck(sf::Sprite& entity, const bool vert);
        //Checks the sprite against the activeTiles and corrects its position
        //Returns NO_BOUND if the sprite is not hitting any tiles
        bool boundsCheck(sf::FloatRect& bounds);
        //Checks the bounds against the activeTiles
        //Returns false if it does not intersect

    private:
        //Constants
        const int SCREEN_WIDTH, SCREEN_HEIGHT;
        static const sf::Vector2f SCALE;
        static const std::string TILE_NAME, TILE_PATH, TILE_FORMAT, LEVEL_PATH, LEVEL_NAME;

        //Functions
        bool sortTileQueue();
        //Sorts tileCoords in ascending order
        //Returns false if the memory for the array used to sort could not be allocated
        bool removeDuplicates();
        //THIS FUNCTION WIL ONLY WORK IF tileCoords IS SORTED
        //Checks for duplicate vectors in tileCoords and removes them
        //Returns false if size is less than or equal to one
        template <typename T>
        T stringToType(std::string convert);
        //Converts a string to the type specified
        //Returns the converted value

        //Data
        bool editingMode;
        std::deque<sf::Sprite> activeTiles;
        std::deque<sf::Vector2f> tileCoords;
        sf::Texture tileTexture;
        sf::Sprite tile;
        sf::Vector2f firstVec;
};

#endif // LEVEL_H
