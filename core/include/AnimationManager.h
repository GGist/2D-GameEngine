/***********************************************************************************
*This class is used as a animation manipulator class. It allows easy access to     *
*textures that were loaded in using the Animation class. Two ways I have used this *
*class have either been to keep track of a specific animation that an object is    *
*using or as an easy way to grab specific textures, ignoring the currentAnimation  *
*functionality. This class will not clean up any dynamic memory that is passed in  *
*and it is recommended to limit the creation of Animation classes you pass in as   *
*they are heavy objects. Full code at https://github.com/GGist/SpriteEngine       *
*This code is licensed under the MIT License.                                      *
***********************************************************************************/

#ifndef AnimationManager_H
#define AnimationManager_H

#include <Animation.h>

class AnimationManager
{
    public:
        AnimationManager(const Animation* anim);
        //Constructor
        AnimationManager(const AnimationManager& aMgr);
        //Copy Constructor (Deep Copy)
        AnimationManager& operator=(const AnimationManager& rhs);
        //Assignment Operator (Deep Copy)
        virtual ~AnimationManager();
        //Destructor
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
        //Functions
        bool outOfBounds(const int& anim, const int& offset);
        //Returns true if the parameters are out of bounds
        void swapMembers(AnimationManager& aMgr);
        //Performs member-wise swap of data members

        //Data
        const Animation* textures;
        const std::vector<AnimBound>* textureBounds;
        const std::vector<sf::Texture>* loadedTextures;
        int currentAnimation, lastAnimation;
        int currentOffset, lastOffset;
};

#endif // AnimationManager_H
