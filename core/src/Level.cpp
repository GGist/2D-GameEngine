#include "Level.h"

using namespace std;

const sf::Vector2f Level::SCALE(1.2, 1.2);
const string Level::TILE_NAME("Tile"), Level::TILE_PATH("res/tiles/"), Level::TILE_FORMAT(".png"),
    Level::LEVEL_NAME("Level_Coords.txt"), Level::LEVEL_PATH("res/level_data/");

Level::Level(sf::Vector2i windowRes) : SCREEN_WIDTH(windowRes.x), SCREEN_HEIGHT(windowRes.y),
    editingMode(false)
{
    tileTexture.loadFromFile(TILE_PATH + TILE_NAME + TILE_FORMAT);
    tile.setTexture(tileTexture);
    sampleTile.setTexture(tileTexture);

    tile.scale(SCALE);
    sampleTile.scale(SCALE);
    tile.setPosition(sf::Vector2f(-tile.getGlobalBounds().width, -tile.getGlobalBounds().height));
}

Level::~Level()
{

}

bool Level::saveLevel(string fileName)
{
    if (!editingMode || tileCoords.empty())
        return false;

    int i;
    ofstream outputFile;

    sortTileQueue();
    removeDuplicates();

    outputFile.open((LEVEL_PATH + LEVEL_NAME).c_str());

    if (outputFile.is_open()) {
        for (i = 0; i < tileCoords.size() - 1; ++i) {
            outputFile << "(" << tileCoords[i].x << ", "
                       << tileCoords[i].y << ")" << endl;
        }
        //So that there is no newline at end of file
        outputFile << "(" << tileCoords[i].x << ", "
                   << tileCoords[i].y << ")";

        outputFile.close();

        return true;
    }

    return false;
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
            tileCoords.push_front(sf::Vector2f(x, y));
            inputFile.ignore();
        }
    }

    inputFile.close();

    return true;
}

void Level::clearLevel()
{
    if (!tileCoords.empty())
        tileCoords.clear();

    if (!activeTiles.empty())
        activeTiles.clear();
}

bool Level::setEditorMode(bool mode)
{
    if (!tileCoords.empty() || !activeTiles.empty())
        return false;

    editingMode = mode;

    return true;
}

bool Level::addTile(sf::Vector2f coord)
{
    if (!editingMode) {
        return false;
    }

    tile.setPosition(coord);
    activeTiles.push_front(tile);

    tileCoords.push_front(coord);

    return true;
}

void Level::updateLevel(sf::FloatRect playerBound)
{
    if (!editingMode) {
        //Delete Old Tiles
        while (!activeTiles.empty() && playerBound.left - activeTiles.front().getGlobalBounds().left > 512) {
            activeTiles.pop_front();
        }

        //Add New Tiles
        while (!tileCoords.empty() && tileCoords.front().x - playerBound.left <= SCREEN_WIDTH) {
            tile.setPosition(tileCoords.front());
            activeTiles.push_front(tile);
            tileCoords.pop_front();
        }
    }

}

void Level::drawLevel(sf::RenderWindow& renderWindow)
{
    for (int i = 0; i < activeTiles.size(); ++i) {
        renderWindow.draw(activeTiles[i]);
    }
}

bool Level::setSampleTile(int x, int y)
{
    if (!editingMode)
        return false;

    sampleTile.setPosition(x, y);

    return true;
}

const sf::Sprite& Level::getSampleTile() const
{
    return sampleTile;
}

bool Level::sortTileQueue()
{
    int index, queueSize = tileCoords.size();
    sf::Vector2f temp;
    sf::Vector2f* sortedCoords;

    if (tileCoords.empty() || queueSize == 1)
        return true;

    sortedCoords = new sf::Vector2f[tileCoords.size()];

    if (sortedCoords == nullptr)
        return false;

    //Push Into Array
    for (int i = 0; i < queueSize; i++) {
        sortedCoords[i] = tileCoords.front();
        tileCoords.pop_front();
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
    for (int i = 0; i < queueSize; i++)
        tileCoords.push_front(sortedCoords[i]);

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
        tileCoords.pop_front();
        if (tempOne != tileCoords.front()) {
            tileCoords.push_front(tempOne);
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

bool Level::boundsCheck(sf::FloatRect& bounds)
{
    for (int i = 0; i < activeTiles.size(); ++i) {
        if (bounds.intersects(activeTiles[i].getGlobalBounds()))
            return true;
    }

    return false;
}

Level::BoundType Level::boundsCheck(sf::Sprite& entity, const bool vert)
{
    BoundType intersects = NO_BOUND;
    int difference = 0;
    sf::Sprite tempSprite;
    sf::FloatRect firstBound, secondBound, tempBound;

    //Setup Entity Bounds
    //The constant multiples are to give the bounds some buffer from the edges of the bounds so that,
    //for example, a BoundType::BOTTOM_BOUND doesnt trigger when running into a wall on the right
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

    //Search activeTiles Until Intersection Is Found Or None
    for (int i = 0; i < activeTiles.size() && intersects == NO_BOUND; ++i) {
        tempSprite = activeTiles[i];
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
        }
    }

    return intersects;
}
