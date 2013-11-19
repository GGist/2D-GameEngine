/***********************************************************************************
*This program is used to demo my SFML 2D Game Engine which is made up of all of the*
*other classes included with the project on the github page.                       *
*Full code at https://github.com/GGist/Sprite_Utility                              *
*This code is licensed under the MIT License.                                      *
***********************************************************************************/

#include <iostream>
#include<Test_Engine.h>

using namespace std;

int main()
{
    //Window Settings
    sf::RenderWindow mainWindow(sf::VideoMode(Test_Engine::SCREEN_RES.x, Test_Engine::SCREEN_RES.y), Test_Engine::WINDOW_TITLE);
    sf::Image icon;
    icon.loadFromFile(Test_Engine::WINDOW_ICON);
    mainWindow.setIcon(Test_Engine::ICON_RES.x, Test_Engine::ICON_RES.y, icon.getPixelsPtr());
    mainWindow.setVisible(false);
    //Scroll Settings
    sf::FloatRect viewBounds(0, 0, Test_Engine::SCREEN_RES.x, Test_Engine::SCREEN_RES.y);
    sf::View mainWindowScroll(viewBounds);
    //Background Settings
    sf::Texture backgroundImage;
    sf::Sprite background;
    backgroundImage.loadFromFile(Test_Engine::LEVEL_BACKGROUND);
    background.setTexture(backgroundImage);

    char userResponse;
    //Display Menu
    do {
        cout << "Enter 'G' to play the game, 'E' to open the level editor, or 'Q' to quit: ";
        cin >> userResponse;
        userResponse = toupper(userResponse);

        if (userResponse == 'Q')
            return 0;

        mainWindow.setVisible(true);
        if (userResponse == 'E') {
            if (!Test_Engine::editLevel(mainWindow, mainWindowScroll, background))
                return 0;
        } else if (userResponse == 'G') {
            if (!Test_Engine::playLevel(mainWindow, mainWindowScroll, background))
                return 0;
        }
        mainWindow.setVisible(false);

        mainWindowScroll.reset(viewBounds);
        userResponse = ' ';
    } while (userResponse != 'G' && userResponse != 'E' && userResponse != 'Q');

    return 0;
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
