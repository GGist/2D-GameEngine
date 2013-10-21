#include <iostream>
#include <SFML/Graphics.hpp>
#include <Player.h>
#include <Level.h>

using namespace std;

const int X_RES = 1280, Y_RES = 720, FPS = 60, ICON_X_SIZE = 48, ICON_Y_SIZE = 63, BOUNDS_THICKNESS = 3;
const string WINDOW_TITLE = "SFML-Shooter", WINDOW_ICON = "res/icon.png", LEVEL_BACKGROUND = "res/backgrounds/level_background.png", LEVEL_DATA = "res/tiles/Coordinates.txt";
const bool SHOW_PLAYER_BOUNDS = false;

void frameLimiter(sf::Time previousTime);
//This function will wait the appropriate time so that the framerate of the game
//will be approximately equal to the FPS that is set
bool editLevel(sf::RenderWindow& window, sf::View& windowScroll, sf::Sprite background);
//Switches to the level editor where the user can create their own level
//Returns false if the user wants to return to the main menu or true
//if the user wants to quit the game
bool playLevel(sf::RenderWindow& window, sf::View& windowScroll, sf::Sprite background);
//Switches to the game where the user plays through different levels
//Returns false if the user wants to return to the main menu or true
//if the user wants to quit the game

int main()
{
    char userResponse;

    //Window Settings
    sf::RenderWindow mainWindow(sf::VideoMode(X_RES, Y_RES), WINDOW_TITLE);
    sf::Image icon;
    icon.loadFromFile(WINDOW_ICON);
    mainWindow.setIcon(ICON_X_SIZE, ICON_Y_SIZE, icon.getPixelsPtr());
    mainWindow.setVisible(false);
    //Scroll Settings
    sf::View mainWindowScroll(sf::FloatRect(0, 0, X_RES, Y_RES));
    //Background Settings
    sf::Texture backgroundImage;
    sf::Sprite background;
    backgroundImage.loadFromFile(LEVEL_BACKGROUND);
    background.setTexture(backgroundImage);

    //Will put in a graphical menu in the future with fullscreen capability
    //Display Menu
    do {
        cout << "Enter 'G' to play the game or 'E' to open the level editor: ";
        cin >> userResponse;
        userResponse = toupper(userResponse);
    } while (userResponse != 'G' && userResponse != 'E');

    mainWindow.setVisible(true);

    if (userResponse == 'E') {
        editLevel(mainWindow, mainWindowScroll, background);
    } else {
        playLevel(mainWindow, mainWindowScroll, background);
    }

    return 0;
}

/***********************************************************************************
*Using this function instead of the setFramerateLimit function in SFML because it  *
*uses sf::sleep which has a bug on Windows 8 that causes it to wait twice as long. *
*This function will work as long as the physics of the game can iterate once in    *
*under or at exactly 1.0 / FPS, otherwise, the game will update as fast as the     *
*computer can update on that specific iteration which may fluctuate greatly causing*
*inconsistent performance. This should not be a problem with modern hardware.      *
***********************************************************************************/
void frameLimiter(sf::Time previousTime)
{
    sf::Clock currentWait;

    while (currentWait.getElapsedTime().asSeconds() < abs((1.0 / FPS) - previousTime.asSeconds())) {

    }
}

/***********************************************************************************
*Framerate is not set in this function in order to give users the smoothest        *
*possible placement of objects. Nothing is animated during level creation so this  *
*will not cause any undesired effects.                                             *
***********************************************************************************/
bool editLevel(sf::RenderWindow& window, sf::View& windowScroll, sf::Sprite background)
{
    Level level(sf::Vector2f(X_RES, Y_RES));
    const int TILE_WIDTH = level.getTile().getGlobalBounds().width;
    int x, y, lastX, lastY, scroll = 0;
    bool held = false, windowFocused = true;
    sf::Sprite displayTile;
    sf::Event mainEvent;

    displayTile = level.getTile();
    level.setEditorMode(true);
    while (window.isOpen()) {
        while (window.pollEvent(mainEvent)) {
            switch (mainEvent.type) {
                //The exit button in upper right corner
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    if (mainEvent.key.code == sf::Keyboard::Escape) {
                        window.close();
                    }
                    break;
                case sf::Event::MouseWheelMoved:
                    if (scroll + mainEvent.mouseWheel.delta * TILE_WIDTH >= 0) {
                        //Not Scrolling Behind Starting Point
                        scroll += mainEvent.mouseWheel.delta * TILE_WIDTH;
                        windowScroll.move(mainEvent.mouseWheel.delta * TILE_WIDTH, 0);
                        background.move(mainEvent.mouseWheel.delta * TILE_WIDTH, 0);
                    }
                    break;
                case sf::Event::MouseMoved:
                    lastX = x;
                    lastY = y;
                    x = (mainEvent.mouseMove.x / TILE_WIDTH) * TILE_WIDTH + scroll;
                    y = (mainEvent.mouseMove.y / TILE_WIDTH) * TILE_WIDTH;
                    displayTile.setPosition(x, y);
                    break;
                case sf::Event::MouseButtonPressed:
                    held = true;
                    level.addTile(sf::Vector2f(x, y));
                    break;
                case sf::Event::MouseButtonReleased:
                    held = false;
                    break;
                case sf::Event::LostFocus:
                    windowFocused = false;
                    break;
                case sf::Event::GainedFocus:
                    windowFocused = true;
                    break;
            }

            if (held && (lastX != x || lastY != y)) {
                level.addTile(sf::Vector2f(x, y));
            }
            level.updateLevel(sf::FloatRect());

            window.setView(windowScroll);
            //Start Drawing Window
            window.clear(sf::Color::White);
            window.draw(background);
            window.draw(displayTile);
            do {
                window.draw(level.getTile());
            } while (level.nextCoord());
            //End Drawing Window
            window.display();

        }
    }
}

bool playLevel(sf::RenderWindow& window, sf::View& windowScroll, sf::Sprite background)
{
    Player hero(sf::Vector2i(X_RES, Y_RES));
    Level level(sf::Vector2f(X_RES, Y_RES));
    int totalOffset = 0;
    int test = 0;
    bool windowFocused = true;
    sf::Event mainEvent;
    sf::Clock timeStep;
    sf::Clock testC;

    //Hero Bounds Settings
    sf::RectangleShape heroBounds;
    heroBounds.setFillColor(sf::Color::Transparent);
    heroBounds.setOutlineColor(sf::Color::Black);
    heroBounds.setOutlineThickness(BOUNDS_THICKNESS);

    level.loadLevel(LEVEL_DATA);

    while (window.isOpen()) {
        frameLimiter(timeStep.getElapsedTime());
        timeStep.restart();
        while (window.pollEvent(mainEvent)) {
            switch (mainEvent.type) {
                //The exit button in upper right corner
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    if (mainEvent.key.code == sf::Keyboard::Escape) {
                        window.close();
                    }
                    break;
                case sf::Event::KeyReleased:
                    if (mainEvent.key.code == sf::Keyboard::Space) {
                        hero.retractParachute();
                    }
                    break;
                case sf::Event::LostFocus:
                    windowFocused = false;
                    break;
                case sf::Event::GainedFocus:
                    windowFocused = true;
                    break;
                case sf::Event::MouseMoved:
                    cout << "(" << mainEvent.mouseMove.x << ", " << mainEvent.mouseMove.y << ")" << endl;
                    break;
            }
        }

        //Real-Time Input Checking
        if (windowFocused) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                hero.jump();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                hero.deployParachute();
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                hero.shoot();
            } else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                hero.knife();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
                hero.runRight();
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                hero.runLeft();
            }
        }

        //Update Limited To Refresh Rate
        if (testC.getElapsedTime().asSeconds() >= 1) {
            cout << test << endl;
            test = 0;
            testC.restart();
        }

        ++test;
        hero.checkAnimation();
        if (hero.applySpeed(level)) {
            //Scroll
            windowScroll.move(hero.getCurrentRunSpeed(), 0);
            background.move(hero.getCurrentRunSpeed(), 0);
            totalOffset += hero.getCurrentRunSpeed();
        }

        if (hero.isDead()) {
            //Restart At Beginning
            windowScroll.move(-totalOffset, 0);
            background.move(-totalOffset, 0);
            totalOffset = 0;
        }
        level.updateLevel(hero.getSprite().getGlobalBounds());

        //Update Visual Representation Of Hero Bounds
        heroBounds.setSize(sf::Vector2f(hero.getSprite().getGlobalBounds().width, hero.getSprite().getGlobalBounds().height));
        heroBounds.setPosition(hero.getSprite().getGlobalBounds().left, hero.getSprite().getGlobalBounds().top);

        window.setView(windowScroll);
        //Start Drawing Window
        window.clear(sf::Color::White);
        window.draw(background);
        do {
            window.draw(level.getTile());
        } while (level.nextCoord());
        if (hero.updateProjectiles(level)) {
            for_each(hero.getProjectiles().begin(), hero.getProjectiles().end(), [&window] (const sf::Sprite& p) {
                window.draw(p);
            });
        }
        if (SHOW_PLAYER_BOUNDS) {
            window.draw(heroBounds);
        }
        window.draw(hero.getSprite());
        //End Drawing Window
        window.display();
    }
}

//For Fullscreen Settings
/*
    std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
    for (std::size_t i = 0; i < modes.size(); ++i) {
        sf::VideoMode mode = modes[i];
        std::cout << "Mode #" << i << ": "
                << mode.width << "x" << mode.height << " - "
                << mode.bitsPerPixel << " bpp" << std::endl;
    }
*/

//To Hide The Console
/*
#if _WIN32_WINNT < 0x0500
  #undef  _WIN32_WINNT
  #define _WIN32_WINNT   0x0500
#endif
#include <windows.h>

ShowWindow(GetConsoleWindow(), SW_HIDE);
ShowWindow(GetConsoleWindow(), SW_RESTORE);
*/
