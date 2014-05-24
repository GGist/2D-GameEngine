#include "Animation.h"

const std::string BulletAnim::PATH("res/projectiles/"), BulletAnim::MANIFEST("~Manifest.txt"), BulletAnim::FORMAT(".png");
const std::string PlayerAnim::PATH("res/anims/player/"), PlayerAnim::MANIFEST("~Manifest.txt"), PlayerAnim::FORMAT(".png");
const std::string EnemyAnim::PATH("res/anims/enemy/"), EnemyAnim::MANIFEST("~Manifest.txt"), EnemyAnim::FORMAT(".png");

Animation::~Animation()
{

}

void Animation::loadTextures(const std::string& path, const std::string& manifest, const std::string& format)
{
    std::ifstream textureManifest;
    std::string currentFileName, lastFileName = " ";
    AnimBound currentBound;
    sf::Texture tempTexture;

    if (textureBounds.empty()) {
        currentBound.first = 0;
    } else {
        currentBound.first = textureBounds.back().last + 1;
    }

    textureManifest.open((path + manifest).c_str());

    while (!textureManifest.eof()) {
        std::getline(textureManifest, currentFileName);
        tempTexture.loadFromFile(path + currentFileName + format);
        loadedTextures.push_back(tempTexture);
        if (currentFileName != "Stand_Left" && currentFileName != "Stand_Right") {
            currentFileName.resize(currentFileName.find_last_of('_'));
        }
        if (lastFileName != currentFileName && lastFileName != " ") {
            currentBound.last = animationIndex;
            textureBounds.push_back(currentBound);
            currentBound.first = animationIndex + 1;
        }
        lastFileName = currentFileName;
        animationIndex++;
    }
    //Push Last Bounds Object In
    currentBound.last = animationIndex;
    textureBounds.push_back(currentBound);

    textureManifest.close();
}

const std::vector<AnimBound>& Animation::getTextureBounds() const
{
    return textureBounds;
}

const std::vector<sf::Texture>& Animation::getLoadedTextures() const
{
    return loadedTextures;
}

//Derived Classes
BulletAnim::BulletAnim()
{
    loadTextures(PATH, MANIFEST, FORMAT);
}

CharacterAnim::CharacterAnim(const std::string& path, const std::string& manifest, const std::string& format)
{
    loadTextures(path, manifest, format);
}

CharacterAnim::~CharacterAnim()
{

}

PlayerAnim::PlayerAnim() : CharacterAnim(PATH, MANIFEST, FORMAT)
{

}

EnemyAnim::EnemyAnim() : CharacterAnim(PATH, MANIFEST, FORMAT)
{

}
