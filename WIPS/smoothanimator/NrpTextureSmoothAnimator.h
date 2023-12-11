#ifndef _INCLUDE_TEXTURESMOOTH_ANIMATOR_
#define _INCLUDE_TEXTURESMOOTH_ANIMATOR_
#include "IGuiAnimator.h"

namespace irr
{

namespace gui
{

class CNrpTextureSmoothAnimator : public IGUIAnimator
{
public:
	CNrpTextureSmoothAnimator( IGUIEnvironment* environment, IGUIElement* node, u32 times, f32 scale );
    CNrpTextureSmoothAnimator( IGUIEnvironment* environment, video::ITexture* texture, u32 times, f32 scale, f32 startAlpha );

    ~CNrpTextureSmoothAnimator(void);

    virtual void setRepeatNumber( int times );
    virtual void setBlurScale( float blurScale );
    virtual void setRotate( u32 angle );

    virtual void draw();
   
private:
    u32 _blurTimes, _angle;
    f32 _blurScale;
    video::ITexture* _texture;

    void _DrawBlurRotate( u32 angle );
    void _DrawBlur();
};

}//namspace gui

}//namespace irr

#endif