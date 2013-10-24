/***********************************************************************************

***********************************************************************************/

#ifndef AnimationManager_H
#define AnimationManager_H

#include <Animation.h>

class AnimationManager
{
    public:
        AnimationManager(CharacterAnim* animations);
        virtual ~AnimationManager();
        bool setTexture(const int anim, const int offset = 0);
        //Sets the current texture to the texture of the specified animation
        //with an optional offset that is past the first texture of the animation
        //Returns false if the specified animation or offset is out of bounds
        bool setNextTexture();
        //Sets the current texture to the next texture in the animation
        //Returns false if the current texture is the last one in the animation
        //THIS FUNCTION WILL NOT ALLOW YOU TO CROSS BETWEEN TWO ANIMATIONS, USE
        //setTexture() TO GO TO A NEW ANIMATION
        const sf::Texture& getTexture(const int anim, const int offset = 0);
        //Returns a reference to the texture of the specified animation with an
        //optional offset to get a texture that is past the first for the animation
        const sf::Texture& getCurrentTexture();
        //Returns a reference to the current texture based on currentAnimation
        //and currentOffset
        int getCurrentAnimation();
        //Returns the current animation which may or may not be the first texture
        //for the animation
        int getLastAnimation();
        //Returns the last animation which may or may not be the first texture
        //for the animation
        int getCurrentOffset();
        //Returns the offset of the current animation to the exact texture
        int getLastOffset();
        //Returns the offset of the last animation to the exact texture
        AnimBound getBounds(const int anim);
        //Returns the AnimBound for the specified animation

    private:
        bool outOfBounds(const int& anim, const int& offset);
        //Returns true if the parameters are out of bounds

        CharacterAnim* textures;
        const std::vector<AnimBound>& textureBounds;
        const std::vector<sf::Texture>& loadedTextures;
        int currentAnimation, lastAnimation;
        int currentOffset, lastOffset;
};

#endif // AnimationManager_H
