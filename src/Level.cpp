#include "Level.h"

using namespace std;

const sf::Vector2f Level::SCALE(1.2, 1.2);
const string Level::TILE_NAME("Tile (14)"), Level::TILE_PATH("res/tiles/"), Level::TILE_FORMAT(".png"), Level::LEVEL_NAME("Coordinates.txt"), Level::LEVEL_PATH("res/tiles/");

Level::Level(sf::Vector2f windowRes) : SCREEN_WIDTH(windowRes.x), SCREEN_HEIGHT(windowRes.y), editingMode(false), tileCounter(0)
{
    tileTexture.loadFromFile(TILE_PATH + TILE_NAME + TILE_FORMAT);
    tile.setTexture(tileTexture);

    tile.scale(SCALE);
    tile.setPosition(sf::Vector2f(-tile.getGlobalBounds().width, -tile.getGlobalBounds().height));
}

Level::~Level()
{
    if (editingMode) {
        int numCoords;
        ofstream outputFile;

        sortTileQueue();
        removeDuplicates();

        numCoords = tileCoords.size();

        outputFile.open((LEVEL_PATH + LEVEL_NAME).c_str());

        for (int i = 0; i < numCoords - 1; i++) {
            outputFile << "(" << tileCoords.front().x << ", " << tileCoords.front().y << ")" << endl;
            tileCoords.pop();
        }
        outputFile << "(" << tileCoords.front().x << ", " << tileCoords.front().y << ")";
        tileCoords.pop();

        outputFile.close();
    }
}

bool Level::loadLevel(string fileName)
{
    int x, y;
    string temp;
    ifstream inputFile;

    inputFile.open(fileName.c_str());

    while (!inputFile.eof()) {
        while (inputFile.peek() != '(' && inputFile.peek() != ' ') {
            inputFile.ignore();
        }
        if (inputFile.peek() == '(') {
            inputFile.ignore();
            getline(inputFile, temp, ',');
            x = stringToType<int>(temp);
        } else if (inputFile.peek() == ' ') {
            inputFile.ignore();
            getline(inputFile, temp, ')');
            y = stringToType<int>(temp);
            tileCoords.push(sf::Vector2f(x, y));
            inputFile.ignore();
        }
    }

    inputFile.close();

    return true;
}

void Level::clearLevel()
{
    while (!tileCoords.empty()) {
        tileCoords.pop();
    }
    while (!activeTiles.empty()) {
        activeTiles.pop();
    }
}

bool Level::setEditorMode(bool mode)
{
    if (tileCoords.empty()) {
        editingMode = mode;
        return true;
    }

    return false;
}

bool Level::addTile(sf::Vector2f coord)
{
    if (!editingMode) {
        return false;
    }

    tile.setPosition(coord);
    activeTiles.push(tile);

    tileCoords.push(coord);

    return true;
}

void Level::updateLevel(sf::FloatRect playerBound)
{
    if (!editingMode) {
        //Delete Old Tiles
        while (!activeTiles.empty() && playerBound.left - activeTiles.front().getGlobalBounds().left > 512) {
            activeTiles.pop();
        }

        //Add New Tiles
        while (!tileCoords.empty() && tileCoords.front().x - playerBound.left <= SCREEN_WIDTH) {
            tile.setPosition(tileCoords.front());
            activeTiles.push(tile);
            tileCoords.pop();
        }
    }

    tileCounter = 0;
}

bool Level::nextCoord()
{
    if (activeTiles.empty()) {
        return false;
    }

    //Remove First Element, Place At Back
    tile = activeTiles.front();
    activeTiles.pop();
    activeTiles.push(tile);

    if (tileCounter == activeTiles.size()) {
        return false;
    }

    tileCounter++;

    return true;
}

const sf::Sprite& Level::getTile() const
{
    return tile;
}

bool Level::sortTileQueue()
{
    int index, queueSize = tileCoords.size();
    sf::Vector2f temp;
    sf::Vector2f* sortedCoords;

    if (tileCoords.empty() || queueSize == 1) {
        return true;
    }

    sortedCoords = new sf::Vector2f[tileCoords.size()];

    if (sortedCoords == NULL) {
        return false;
    }

    //Push Into Array
    for (int i = 0; i < queueSize; i++) {
        sortedCoords[i] = tileCoords.front();
        tileCoords.pop();
    }

    //Sort
    for (int i = 1; i < queueSize; i++) {
        index = i;
        while (index > 0 && sortedCoords[index].x <= sortedCoords[index - 1].x) {
            if (sortedCoords[index].x == sortedCoords[index - 1].x) {
                //Sort By Y
                while (index > 0 && sortedCoords[index].x == sortedCoords[index - 1].x && sortedCoords[index].y < sortedCoords[index - 1].y) {
                    temp = sortedCoords[index];
                    sortedCoords[index] = sortedCoords[index - 1];
                    sortedCoords[index - 1] = temp;
                    index--;
                }
                break;
            } else {
                //Sort By X
                temp = sortedCoords[index];
                sortedCoords[index] = sortedCoords[index - 1];
                sortedCoords[index - 1] = temp;
                index--;
            }
        }
    }

    //Push Into Queue
    for (int i = 0; i < queueSize; i++) {
        tileCoords.push(sortedCoords[i]);
    }

    delete sortedCoords;

    return true;
}

bool Level::removeDuplicates()
{
    int queueSize = tileCoords.size();
    sf::Vector2f tempOne;

    if (tileCoords.size() <= 1) {
        return false;
    }

    for (int i = 0; i < queueSize; i++) {
        tempOne = tileCoords.front();
        tileCoords.pop();
        if (tempOne != tileCoords.front()) {
            tileCoords.push(tempOne);
        }
    }

    return true;
}

template <typename T>
T Level::stringToType(string convert)
{
    T value;
    istringstream converter(convert);

    converter >> value;

    return value;
}

Level::BoundType Level::boundsCheck(sf::Sprite& entity, const bool vert)
{
    BoundType intersects = NO_BOUND;
    int queueSize = activeTiles.size(), difference = 0, loopCount;
    sf::Sprite tempSprite;
    sf::FloatRect firstBound, secondBound, tempBound;

    //Setup Entity Bounds
    if (vert) {
        //Top Bound
        firstBound.left = entity.getGlobalBounds().left + (entity.getGlobalBounds().width * .10);
        firstBound.top = entity.getGlobalBounds().top;
        firstBound.width = entity.getGlobalBounds().width - (entity.getGlobalBounds().width * .20);
        firstBound.height = 1;
        //Bottom Bound
        secondBound.left = entity.getGlobalBounds().left + (entity.getGlobalBounds().width * .10);
        secondBound.top = entity.getGlobalBounds().top + entity.getGlobalBounds().height;
        secondBound.width = entity.getGlobalBounds().width - (entity.getGlobalBounds().width * .20);
        secondBound.height = 1;
    } else {
        //Left Bound
        firstBound.left = entity.getGlobalBounds().left;
        firstBound.top = entity.getGlobalBounds().top + (entity.getGlobalBounds().height * 0.15);
        firstBound.width = 1;
        firstBound.height = entity.getGlobalBounds().height - (entity.getGlobalBounds().height * 0.30);
        //Right Bound
        secondBound.left = entity.getGlobalBounds().left + entity.getGlobalBounds().width;
        secondBound.top = entity.getGlobalBounds().top + (entity.getGlobalBounds().height * 0.15);
        secondBound.width = 1;
        secondBound.height = entity.getGlobalBounds().height - (entity.getGlobalBounds().height * 0.30);
    }

    //Pop And Push activeTiles Until Intersection Is Found Or None
    for (loopCount = 0; loopCount < queueSize && intersects == NO_BOUND; loopCount++) {
        tempSprite = activeTiles.front();
        tempBound = tempSprite.getGlobalBounds();

        if (firstBound.intersects(tempBound)) {
            if (vert) {
                difference = tempBound.top + tempBound.height;
                difference -= entity.getGlobalBounds().top;
                entity.move(0, difference);
                intersects = TOP_BOUND;
            } else {
                difference = tempBound.left + tempBound.width;
                difference -= entity.getGlobalBounds().left;
                entity.move(difference, 0);
                intersects = LEFT_BOUND;
            }
        } else if (secondBound.intersects(tempBound)) {
            if (vert) {
                difference = tempBound.top;
                difference -= entity.getGlobalBounds().top + entity.getGlobalBounds().height;
                entity.move(0, difference);
                intersects = BOTTOM_BOUND;
            } else {
                difference = tempBound.left;
                difference -= entity.getGlobalBounds().left + entity.getGlobalBounds().width;
                entity.move(difference, 0);
                intersects = RIGHT_BOUND;
            }
        } else {
            activeTiles.pop();
            activeTiles.push(tempSprite);
            tempSprite = activeTiles.front();
        }
    }

    //Preserve The Ordering Of activeTiles Before Checking Bounds
    for (int i = 0; i < queueSize - loopCount; i++) {
        tempSprite = activeTiles.front();
        activeTiles.pop();
        activeTiles.push(tempSprite);
    }

    return intersects;
}
