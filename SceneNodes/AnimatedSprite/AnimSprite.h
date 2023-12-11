/*-----------------------------------------------------------------------------*
| headerfile AnimSprite.h                                                      |
|                                                                              |
| version 1.1                                                                  |
| date: (04.09.2007)                                                           |
|                                                                              |
| author:  Michal Švantner                                                     |
|                                                                              |
| for Irrlicht engine                                                          |
| 2d animated sprite class                                                     |
*-----------------------------------------------------------------------------*/

#ifndef ANIMSPRITE_H
#define ANIMSPRITE_H

#include <irrlicht.h>
using namespace irr;

class AnimSprite
{
    struct Animation
    {
        s32 start;
        s32 end;
        u32 speed;
        bool loop;
        bool reverse;
    };
    
    video::ITexture *texture;
    core::array< core::rect<s32> > frame;
    s32 currentFrame;
    core::rect<s32> position;
    
    core::array<Animation> animation;
    u32 currentAnim;
    
    u32 oldTime;
    
    IrrlichtDevice *Device;
    
public:
    
    // constructor
    // \param idevice -pointer to Irrlicht device
    // \param itexture -pointer to texture used, must be power of two size
    // \param divX - number of frames along X axis
    // \param divY - number of frames along Y axis
    AnimSprite(IrrlichtDevice *idevice, video::ITexture *itexture, s32 divX = 1, s32 divY = 1)
    {
        Device = idevice;
        
        texture = itexture;
        
        core::dimension2d<s32> spriteSize(texture->getSize());
        spriteSize.Width = spriteSize.Width / divX;
        spriteSize.Height = spriteSize.Height / divY;
        
        frame.set_used(divX * divY);
        
        u32 n = 0;
        for(s32 j=0; j<divY; j++)
            for(s32 i=0; i<divX; i++)
            {
                frame[n] = core::rect<s32>(
                    i*spriteSize.Width, j*spriteSize.Height, 
                    (i+1)*spriteSize.Width, (j+1)*spriteSize.Height);
                n++;
            }
        
        currentFrame = 0;
        
        position.UpperLeftCorner = core::position2d<s32>(0,0);
        position.LowerRightCorner = core::position2d<s32>(spriteSize.Width,spriteSize.Height);
        
        currentAnim = 0;
    }
    
    // copy constructor
    // \param other -sprite to coppy
    AnimSprite(AnimSprite &other)
    {
        texture = other.texture;
        
        frame.set_used(other.frame.size());
        for(u32 i=0; i<frame.size(); i++) frame[i] = other.frame[i];
        
        currentFrame = 0;
        
        position.UpperLeftCorner = core::position2d<s32>(0,0);
        position.LowerRightCorner = other.position.LowerRightCorner - other.position.UpperLeftCorner;
        
        animation.set_used(other.animation.size());
        for(u32 i=0; i<animation.size(); i++)
        {
            animation[i].start = other.animation[i].start;
            animation[i].end = other.animation[i].end;
            animation[i].speed = other.animation[i].speed;
            animation[i].loop = other.animation[i].loop;
            animation[i].reverse = other.animation[i].reverse;
        }
        
        currentAnim = 0;
        
        Device = other.Device;
    }
    
    // destructor
    ~AnimSprite(){}
    
    // draw frame
    // \param num -frame id number based on 0 based index
    virtual void draw(u32 num = 0)
    {
        Device->getVideoDriver()->draw2DImage(texture, position, frame[num], 0, 0, true);
    }
    
    // play sprite
    virtual void play()
    {
        if(animation.empty())
        {
            draw(0);
            return;
        }
        
        u32 time = Device->getTimer()->getTime();
        
        if(oldTime + animation[currentAnim].speed <= time)
        {
            if(animation[currentAnim].reverse)
            {
                currentFrame--;
                if(currentFrame < animation[currentAnim].start)
                {
                    if(animation[currentAnim].loop)currentFrame = animation[currentAnim].end;
                    else currentFrame++;
                }
            }
            else
            {
                currentFrame++;
            
                if(currentFrame > animation[currentAnim].end)
                {
                    if(animation[currentAnim].loop)currentFrame = animation[currentAnim].start;
                    else currentFrame--;
                }
            }
            
            oldTime = time;
        }
        
        draw(currentFrame);
    }
    
    // add new animation
    // returns id number of animation
    // \param start -start frame number
    // \param end - end frame number
    // \param speed - animation speed in miliseconds
    // \param loop - true if animation should loop, false if not
    // \param reverse - true if animation should play reversed, false if not
    virtual u32 addAnimation(u32 start, u32 end, u32 speed, bool loop = false, bool reverse = false)
    {
        Animation tmp;
        tmp.start = start;
        tmp.end = end;
        tmp.speed = speed;
        tmp.loop = loop;
        tmp.reverse = reverse;
        
        animation.push_back(tmp);
        
        return animation.size()-1;
    }
    
    // remove animation
    // \param num -animation id number
    virtual void removeAnimation(u32 num)
    {
        animation.erase(num);
        
        if(currentAnim == num)
        {
            currentAnim = 0;
            currentFrame = 0;
        }
        
        if(currentAnim > num) currentAnim--;
    }
    
    // remove all animations
    virtual void clearAnimations()
    {
        animation.clear();
        currentAnim = 0;
        currentFrame = 0;
    }
    
    // set animation to play
    // \param num -animation id number
    virtual void setAnimation(u32 num)
    {
        currentAnim = num;
        if(animation[currentAnim].reverse)
            currentFrame = animation[currentAnim].end;
        else
            currentFrame = animation[currentAnim].start;
        oldTime = Device->getTimer()->getTime();
    }
    
    // return id number of animation played
    virtual u32 getCurrentAnimation()
    {
        return currentAnim;
    }
    
    // set speed of animation
    // \param num -animation id number
    // \param newspeed -new animation speed in miliseconds
    virtual void setAnimationSpeed(u32 num, u32 newspeed)
    {
        if(animation.empty()) return;
        animation[num].speed = newspeed;
    }
    
    // return speed of animation
    // \param num -animation id number
    virtual u32 getAnimationSpeed(u32 num)
    {
        if(animation.empty()) return 0;
        return animation[num].speed;
    }
    
    // return id number of animation start frame
    // \param num -animation id number
    virtual u32 getAnimationStart(u32 num)
    {
        return animation[num].start;
    }
    
    // return id number of animation end frame
    // \param num -animation id number
    virtual u32 getAnimationEnd(u32 num)
    {
        return animation[num].end;
    }
    
    // set animation loop state
    // \param num -animation id number
    // \param loop -true if animation should loop, false if not
    virtual void setAnimationLoopState(u32 num, bool loop)
    {
        animation[num].loop = loop;
    }
    
    // return true if animation loopes, false if not
    // \param num -animation id number
    virtual bool getAnimationLoopState(u32 num)
    {
        return animation[num].loop;
    }
    
    // set animation reverse state
    // \param num -animation id number
    // \param reverse -true if animation should play reversed, false if not
    virtual void reverseAnimation(u32 num, bool reverse)
    {
        animation[num].reverse = reverse;
    }
    
    // return true if animation play reverse, false if not
    // \param num -animation id number
    virtual bool isAnimationReversed(u32 num)
    {
        return animation[num].reverse;
    }
    
    // set position of sprite on screen
    // \param x - screen X coordinate of new position
    // \param y - screen Y coordinate of new position
    virtual void setPosition(s32 x, s32 y)
    {
        position.LowerRightCorner = position.LowerRightCorner - position.UpperLeftCorner + core::position2d<s32>(x,y);
        position.UpperLeftCorner = core::position2d<s32>(x,y);
    }
    
    // return position of sprite on screen
    virtual core::position2d<s32> getPosition()
    {
        return position.UpperLeftCorner;
    }
    
    // return number of frames in sprite
    virtual u32 getFrameCount()
    {
        return frame.size();
    }
    
    // return number of animations in sprite
    virtual u32 getAnimationCount()
    {
        return animation.size();
    }
    
    // set new texture sprite should use
    // \param newtexture - pointer to texture sprite should use
    //                     must be the same size as original texture
    virtual void setTexture(video::ITexture *newtexture)
    {
        texture = newtexture;
    }
    
    // return pointer to texture sprite is using
    virtual video::ITexture* getTexture()
    {
        return texture;
    }
    
    // return size of sprite in pixels
    virtual core::dimension2d<s32> getSize()
    {
        core::dimension2d<s32> size;
        
        size.Width = position.LowerRightCorner.X - position.UpperLeftCorner.X;
        size.Height = position.LowerRightCorner.Y - position.UpperLeftCorner.Y;
        
        return size;
    }
};

#endif
