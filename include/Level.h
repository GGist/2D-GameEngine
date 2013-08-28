#ifndef LEVEL_H
#define LEVEL_H

#include <Player.h>
#include <queue>
#include <sstream>

class Level
{
    public:
        Level(sf::Vector2f windowRes);
        //Constructor
        virtual ~Level();
        //Destructor
        bool loadLevel(std::string fileName);
        //Loads the level from a file
        //Returns false if file could not be found
        void clearLevel();
        //Clears the current level
        bool setEditorMode(bool mode = false);
        //THIS FUNCTION WIL ONLY WORK IF THE LEVEL IS CLEARED
        //Puts the object in level editing mode to accept tiles through addTile
        //Returns false if the level has not been cleared
        bool addTile(sf::Vector2f coord);
        //THIS FUNCTION WILL ONLY WORK IF editingMode IS TRUE
        //Adds a vector to tileCoords
        //Returns false if the vector could not be added due to memory issues or editingMode
        void updateLevel(sf::FloatRect playerBound);
        //Updates the coordinates in tileCoords and sets the first coordinate in the queue
        bool nextCoord();
        //Moves the tile to the next coordinate so it can be drawn to the screen
        //Returns false when it reaches the first coordinate in the queue since calling updateLevel()
        const sf::Sprite& getTile() const;
        //Returns the Sprite tile

    protected:
    private:
        //Constants
        const int SCREEN_WIDTH, SCREEN_HEIGHT;
        static const sf::Vector2f SCALE;
        static const std::string TILE_NAME, TILE_PATH, TILE_FORMAT, LEVEL_PATH, LEVEL_NAME;
        enum BoundType {
            TOP_BOUND,
            BOTTOM_BOUND,
            LEFT_BOUND,
            RIGHT_BOUND,
            NO_BOUND
        };

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

        //Friends
        friend bool Player::updatePlayer(Level&);
        friend bool Projectile::moveProjectiles(Level&, sf::FloatRect);

        Level::BoundType boundsCheck(sf::Sprite& entity, const bool vert);
        //USED BY FRIEND FUNCTIONS ONLY
        //Checks the hero against the tileCoords
        //Returns false if the player is not hitting any tiles

        //Data
        bool editingMode;
        size_t tileCounter;
        std::queue<sf::Sprite> activeTiles;
        std::queue<sf::Vector2f> tileCoords;
        sf::Texture tileTexture;
        sf::Sprite tile;
        sf::Vector2f firstVec;
};

#endif // LEVEL_H
