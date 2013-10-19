#include "TextureManager.h"

TextureManager::TextureManager(CharAnim* animations) : textures(animations), textureBounds(textures->getTextureBounds()), loadedTextures(textures->getLoadedTextures()),
                                                       currentAnimation(0), lastAnimation(0), currentOffset(0), lastOffset(0)
{

}

TextureManager::~TextureManager()
{
    if (textures != NULL) {
        delete textures;
    }
}

bool TextureManager::setTexture(const int anim, const int offset)
{
    if (outOfBounds(anim, offset)) {
        throw "Bad Bounds: TextureManager::setTexture";
    }

    lastAnimation = currentAnimation;
    lastOffset = currentOffset;
    currentAnimation = anim;
    currentOffset = offset;

    return true;
}

bool TextureManager::setNextTexture()
{
    int textureIndex = getBounds(currentAnimation).first + currentOffset;

    if (textureIndex + 1 > getBounds(currentAnimation).last) {
        return false;
    }

    lastOffset = currentOffset;
    ++currentOffset;

    return true;
}

const sf::Texture& TextureManager::getTexture(const int anim, const int offset)
{
    if (outOfBounds(anim, offset)) {
        throw "Bad Bounds: TextureManager::getTexture";
    }

    return loadedTextures[getBounds(getCurrentAnimation()).first + getCurrentOffset()];
}

const sf::Texture& TextureManager::getCurrentTexture()
{
    return loadedTextures[getBounds(getCurrentAnimation()).first + getCurrentOffset()];
}

int TextureManager::getCurrentAnimation()
{
    return currentAnimation;
}

int TextureManager::getLastAnimation()
{
    return lastAnimation;
}

int TextureManager::getCurrentOffset()
{
    return currentOffset;
}

int TextureManager::getLastOffset()
{
    return lastOffset;
}

AnimBound TextureManager::getBounds(const int anim)
{
    return textureBounds[anim];
}

 bool TextureManager::outOfBounds(const int& anim, const int& offset)
 {
    int newTextureIndex = getBounds(anim).first + offset;

    if (anim < 0 || anim > textureBounds.size() || newTextureIndex > textureBounds.back().last) {
        return true;
    }

    return false;
 }
