#include "AnimationManager.h"

AnimationManager::AnimationManager(Animation* animations) : textures(animations),
    textureBounds(textures->getTextureBounds()), loadedTextures(textures->getLoadedTextures()),
    currentAnimation(0), lastAnimation(0), currentOffset(0), lastOffset(0)
{

}

AnimationManager::~AnimationManager()
{
    if (textures != nullptr)
        delete textures;

    textures = nullptr;
}

bool AnimationManager::setTexture(const int anim, const int offset)
{
    if (outOfBounds(anim, offset))
        throw "Bad Bounds: AnimationManager::setTexture";

    lastAnimation = currentAnimation;
    lastOffset = currentOffset;
    currentAnimation = anim;
    currentOffset = offset;

    return true;
}

bool AnimationManager::setNextTexture()
{
    int textureIndex = getBounds(currentAnimation).first + currentOffset;

    if (textureIndex + 1 > getBounds(currentAnimation).last)
        return false;

    lastOffset = currentOffset;
    ++currentOffset;

    return true;
}

const sf::Texture& AnimationManager::getTexture(const int anim, const int offset)
{
    if (outOfBounds(anim, offset))
        throw "Bad Bounds: AnimationManager::getTexture";

    return loadedTextures[getBounds(getCurrentAnimation()).first + getCurrentOffset()];
}

const sf::Texture& AnimationManager::getCurrentTexture()
{
    return loadedTextures[getBounds(getCurrentAnimation()).first + getCurrentOffset()];
}

int AnimationManager::getCurrentAnimation()
{
    return currentAnimation;
}

int AnimationManager::getLastAnimation()
{
    return lastAnimation;
}

int AnimationManager::getCurrentOffset()
{
    return currentOffset;
}

int AnimationManager::getLastOffset()
{
    return lastOffset;
}

AnimBound AnimationManager::getBounds(const int anim)
{
    return textureBounds[anim];
}

 bool AnimationManager::outOfBounds(const int& anim, const int& offset)
 {
    int newTextureIndex = getBounds(anim).first + offset;

    if (anim < 0 || anim > textureBounds.size() || newTextureIndex > textureBounds.back().last)
        return true;

    return false;
 }
