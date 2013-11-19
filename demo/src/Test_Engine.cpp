#include "Test_Engine.h"

using namespace std;

const sf::Vector2i Test_Engine::SCREEN_RES(1280, 720), Test_Engine::ICON_RES(48, 63);
const bool Test_Engine::SHOW_PLAYER_BOUNDS(false);
const int Test_Engine::BOUNDS_THICKNESS(3), Test_Engine::FPS(60);
const string Test_Engine::WINDOW_TITLE("SFML-Test_Engine"), Test_Engine::WINDOW_ICON("res/icon.png"),
    Test_Engine::LEVEL_BACKGROUND("res/backgrounds/level_background.png"), Test_Engine::LEVEL_DATA("res/level_data/Level_Coords.txt"),
    Test_Engine::ENEMY_DATA("res/level_data/Enemy_Coords.txt");


Test_Engine::Test_Engine()
{

}

Test_Engine::~Test_Engine()
{

}

bool Test_Engine::playLevel(sf::RenderWindow& window, sf::View& windowScroll, sf::Sprite background)
{
    Player hero(SCREEN_RES);
    EnemyManager eManager(SCREEN_RES);
    Level level(SCREEN_RES);
    int totalOffset = 0;
    bool windowFocused = true;
    sf::Event mainEvent;
    sf::Clock timeStep;
    int test = 0;
    sf::Clock testC;

    //Hero Bounds Settings
    sf::RectangleShape heroBounds;
    heroBounds.setFillColor(sf::Color::Transparent);
    heroBounds.setOutlineColor(sf::Color::Black);
    heroBounds.setOutlineThickness(BOUNDS_THICKNESS);

    level.loadLevel(LEVEL_DATA);
    eManager.loadEnemies(ENEMY_DATA);

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
                    } else if (mainEvent.key.code == sf::Keyboard::D || mainEvent.key.code == sf::Keyboard::A) {
                        hero.stopRun();
                    }
                    break;
                case sf::Event::LostFocus:
                    windowFocused = false;
                    break;
                case sf::Event::GainedFocus:
                    windowFocused = true;
                    break;
                case sf::Event::MouseMoved:
                    //cout << "(" << mainEvent.mouseMove.x << ", " << mainEvent.mouseMove.y << ")" << endl;
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

        eManager.updateEnemies(hero.getSprite().getGlobalBounds(), level);

        hero.checkAnimation();
        if (hero.applySpeed(level)) {
            //Scroll
            windowScroll.move(hero.getCurrentRunSpeed(), 0);
            background.move(hero.getCurrentRunSpeed(), 0);
            totalOffset += hero.getCurrentRunSpeed();
        }

        if (eManager.checkShot(hero.getSprite().getGlobalBounds(), hero.getProjManager())) {
            return true;
        }
        if (hero.checkKnifed()){
            eManager.removeEnemies(hero.getSprite().getGlobalBounds());
        }
        level.updateLevel(hero.getSprite().getGlobalBounds());

        //Update Visual Representation Of Hero Bounds
        heroBounds.setSize(sf::Vector2f(hero.getSprite().getGlobalBounds().width, hero.getSprite().getGlobalBounds().height));
        heroBounds.setPosition(hero.getSprite().getGlobalBounds().left, hero.getSprite().getGlobalBounds().top);

        window.setView(windowScroll);
        //Start Drawing Window
        window.clear(sf::Color::White);
        window.draw(background);
        level.drawLevel(window);
        if (hero.checkProjectiles(level))
            hero.drawProjectiles(window);
        if (SHOW_PLAYER_BOUNDS)
            window.draw(heroBounds);
        eManager.drawEnemies(window);
        window.draw(hero.getSprite());
        //End Drawing Window
        window.display();
    }
}

bool Test_Engine::editLevel(sf::RenderWindow& window, sf::View& windowScroll, sf::Sprite background)
{
    sf::Event mainEvent;
    Level level(SCREEN_RES);
    EnemyManager eManager(SCREEN_RES);
    const int TILE_WIDTH = level.getSampleTile().getGlobalBounds().width, ENEMY_WIDTH = eManager.getSampleEnemy().getSprite().getGlobalBounds().width;
    bool held = false, windowFocused = true, tileActive = false;
    int x, y, lastX, lastY, scroll = 0;

    level.setEditorMode(true);
    eManager.setEditorMode(true);
    while (window.isOpen()) {
        while (window.pollEvent(mainEvent)) {
            switch (mainEvent.type) {
                //The exit button in upper right corner
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    if (mainEvent.key.code == sf::Keyboard::Escape)
                        window.close();
                    else if (mainEvent.key.code == sf::Keyboard::Space) {
                        tileActive = (tileActive) ? false : true;
                        if (tileActive)
                            level.setSampleTile(x, y);
                        else
                            eManager.setSampleEnemy(x, y);
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
                    if (tileActive) {
                        x = (mainEvent.mouseMove.x / TILE_WIDTH) * TILE_WIDTH + scroll;
                        y = (mainEvent.mouseMove.y / TILE_WIDTH) * TILE_WIDTH;
                    } else {
                        x = mainEvent.mouseMove.x / ENEMY_WIDTH * ENEMY_WIDTH + scroll;
                        y = mainEvent.mouseMove.y / ENEMY_WIDTH * ENEMY_WIDTH;
                    }
                    if (tileActive)
                        level.setSampleTile(x, y);
                    else
                        eManager.setSampleEnemy(x, y);
                    break;
                case sf::Event::MouseButtonPressed:
                    held = true;
                    if (tileActive)
                        level.addTile(sf::Vector2f(x, y));
                    else
                        eManager.addEnemy(sf::Vector2f(x, y));
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
                if (tileActive)
                    level.addTile(sf::Vector2f(x, y));
                else
                    eManager.addEnemy(sf::Vector2f(x, y));
            }
            level.updateLevel(sf::FloatRect());

            window.setView(windowScroll);
            //Start Drawing Window
            window.clear(sf::Color::White);
            window.draw(background);
            if (tileActive)
                window.draw(level.getSampleTile());
            else
                window.draw(eManager.getSampleEnemy().getSprite());
            level.drawLevel(window);
            eManager.drawEnemies(window);
            //End Drawing Window
            window.display();

        }
    }
}

/***********************************************************************************
*Using this function instead of the setFramerateLimit function in SFML because it  *
*uses sf::sleep which has a bug on Windows 8 that causes it to wait twice as long. *
*This function will work as long as the physics of the game can iterate once in    *
*under or at exactly 1.0 / FPS, otherwise, the game will update as fast as the     *
*computer can update on that specific iteration which may fluctuate greatly causing*
*inconsistent performance. This should not be a problem with modern hardware.      *
***********************************************************************************/
void Test_Engine::frameLimiter(sf::Time previousTime)
{
    sf::Clock currentWait;

    if (previousTime.asSeconds() < abs(1.0 / FPS)) {
        while (currentWait.getElapsedTime().asSeconds() < abs((1.0 / FPS) - previousTime.asSeconds())) {
        }
    }
}
