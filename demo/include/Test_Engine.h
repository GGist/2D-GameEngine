/***********************************************************************************
*This class is used to demo my SFML 2D Game Engine which is made up of all of the  *
*other classes included with the project on the github page.                       *
*Full code at https://github.com/GGist/SpriteEngine                               *
*This code is licensed under the MIT License.                                      *
***********************************************************************************/

#ifndef TEST_ENGINE_H
#define TEST_ENGINE_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <Level.h>
#include <Player.h>
#include <EnemyManager.h>

class Test_Engine
{
    public:
        Test_Engine();
        //Constructor
        ~Test_Engine();
        //Destructor
        static bool playLevel(sf::RenderWindow& window, sf::View& windowScroll, sf::Sprite background);
        //Switches to the game where the user plays through different levels
        //Returns false if the user wants to return to the main menu or true
        //if the user wants to quit the game
        static bool editLevel(sf::RenderWindow& window, sf::View& windowScroll, sf::Sprite background);
        //Switches to the level editor where the user can create their own level
        //Returns false if the user wants to return to the main menu or true
        //if the user wants to quit the game

        //Constants
        static const sf::Vector2i SCREEN_RES, ICON_RES;
        static const std::string WINDOW_TITLE, WINDOW_ICON, LEVEL_BACKGROUND;
    private:
        //Constants
        static const bool SHOW_PLAYER_BOUNDS;
        static const int BOUNDS_THICKNESS, FPS;
        static const std::string LEVEL_DATA, ENEMY_DATA;

        //Functions
        static void frameLimiter(sf::Time previousTime);
        //This function will wait the appropriate time so that the framerate of the game
        //will be approximately equal to the FPS that is set
};

#endif // TEST_ENGINE_H
