/***********************************************************************************

***********************************************************************************/

#ifndef ANIMATION_H
#define ANIMATION_H

#include <iostream>
#include <memory>
#include <string>
#include <fstream>
#include <vector>
#include <SFML/Graphics.hpp>

//Container Struct
struct AnimBound {
    AnimBound() {}
    AnimBound(int lower, int upper)
    : first(lower), last(upper) {}
    int first;
    int last;
};

//Base Class
class Animation {
public:
    Animation() : animationIndex(-1) { std::cout << "anim" << std::endl;}
    Animation(const Animation& anim) : animationIndex(anim.animationIndex),
        textureBounds(anim.textureBounds), loadedTextures(anim.loadedTextures)  { }
    virtual ~Animation();
    const std::vector<AnimBound>& getTextureBounds() const;
    //Returns textureBounds Which Is Synced With importedTextures
    const std::vector<sf::Texture>& getLoadedTextures() const;
    //Returns loadedTextures

protected:
    void loadTextures(const std::string& path, const std::string& manifest, const std::string& format);
    //Loads Textures In The Manifest File And Location Specified By The Parameters
    //Call This In Derived Class Constructors To Load Resources

private:
    int animationIndex;

    std::vector<AnimBound> textureBounds;
    std::vector<sf::Texture> loadedTextures;
};

//Default Animations
class BulletAnim : public Animation {
private:
    static const std::string PATH, MANIFEST, FORMAT;

public:
    BulletAnim();
    virtual ~BulletAnim() {}

    static const int BULLET = 0;
};

class CharacterAnim : public Animation {
public:
    CharacterAnim(const std::string& path, const std::string& manifest, const std::string& format);
    virtual ~CharacterAnim() = 0;

    static const int SHOOT_LEFT = 0;
    static const int SHOOT_RIGHT = 1;
    static const int RUN_LEFT = 2;
    static const int RUN_RIGHT = 3;
    static const int STAND_LEFT = 4;
    static const int STAND_RIGHT = 5;
    //Make Sure New Animation Values Are Declared Consecutively With These
    //Eg: Start Next Animation At 6
};

//Extended Animations
class PlayerAnim : public CharacterAnim {
private:
    static const std::string PATH, MANIFEST, FORMAT;

public:
    PlayerAnim();
    virtual ~PlayerAnim() {}

    static const int KNIFE_LEFT = 6;
    static const int KNIFE_RIGHT = 7;
    static const int PARACHUTE_LEFT = 8;
    static const int PARACHUTE_RIGHT = 9;
    //Make Sure New Animation Values Are Declared Consecutively With These
    //Eg: Start Next Animation At 10
};

class EnemyAnim : public CharacterAnim {
private:
    static const std::string PATH, MANIFEST, FORMAT;

public:
    EnemyAnim();
    virtual ~EnemyAnim() {}
};

#endif // ANIMATION_H
