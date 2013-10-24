/***********************************************************************************

***********************************************************************************/

#ifndef ANIMATION_H
#define ANIMATION_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>
#include <vector>

//Container Struct
struct AnimBound {
    AnimBound() {}
    AnimBound(int lower, int upper)
    : first(lower), last(upper) {}
    int first;
    int last;
};

//Base Class
class CharacterAnim {
private:
    static const std::string PATH, MANIFEST, FORMAT;

    std::vector<AnimBound> textureBounds;
    std::vector<sf::Texture> loadedTextures;

protected:
    void loadTextures(const std::string& path, const std::string& manifest, const std::string& format);
    //Loads Textures In The Manifest File And Location Specified By The Parameters
    //Call This In Derived Class Constructors To Load Resources

public:
    CharacterAnim();
    virtual ~CharacterAnim() {}
    const std::vector<AnimBound>& getTextureBounds();
    //Returns textureBounds Which Is Synced With importedTextures
    const std::vector<sf::Texture>& getLoadedTextures();
    //Returns loadedTextures

    static const int SHOOT_LEFT = 0;
    static const int SHOOT_RIGHT = 1;
    static const int RUN_LEFT = 2;
    static const int RUN_RIGHT = 3;
    static const int STAND_LEFT = 4;
    static const int STAND_RIGHT = 5;
    //Make Sure New Animation Values Are Declared Consecutively With These
    //Eg: Start Next Animation At 6
};

//Derived Classes
class PlayerAnim : public CharacterAnim {

    static const std::string PATH, MANIFEST, FORMAT;

public:
    PlayerAnim();
    virtual ~PlayerAnim() {}

    static const int KNIFE_LEFT = 6;
    static const int KNIFE_RIGHT = 7;
    static const int PARACHUTE_LEFT = 8;
    static const int PARACHUTE_RIGHT = 9;
};

#endif // ANIMATION_H
