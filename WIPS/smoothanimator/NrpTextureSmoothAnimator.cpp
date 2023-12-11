//CGuiImage* img;//created somewhere in code
//new CNrpTextureSmoothAnimator( GuiEnvironment, img, number_of_repeat, scale_increased_in_step );


#include "StdAfx.h"
#include "NrpTextureSmoothAnimator.h"
#include "nrpButton.h"
#include "nrpScript.h"

#include <irrlicht.h>

namespace irr
{

namespace gui
{

CNrpTextureSmoothAnimator::~CNrpTextureSmoothAnimator(void)
{
}

CNrpTextureSmoothAnimator::CNrpTextureSmoothAnimator( IGUIEnvironment* environment, IGUIElement* parent, u32 times, f32 scale )
	: IGUIAnimator( environment, parent ), _texture( NULL ), _blurScale( scale ), _blurTimes( times ), _angle( 0 )
{
}

void CNrpTextureSmoothAnimator::_DrawBlurRotate( u32 angle )
{
    if( angle < 2 )
    {
        _DrawBlur();
        return;
    }

    if( !_texture || !AlphaBlend )
        return;

    core::vector2df scale;
    video::IVideoDriver* driver = Environment->getVideoDriver();

    float spost = 0.0f;    // Начальное смещение координат
    float alpha = AlphaBlend / 255.f;    // Начальное значение прозрачности
    float alphainc = alpha / _blurTimes;  // Скорость уменьшения прозрачности

    core::recti txsRect = core::recti(core::position2d<s32>(0,0), core::dimension2di(_texture->getOriginalSize()));

    for (int num = 0;num < _blurTimes;num++)// Количество походов = times  
    {
        core::recti currentRect;
        currentRect.UpperLeftCorner.X = txsRect.UpperLeftCorner.X + txsRect.getWidth() * spost;
        currentRect.UpperLeftCorner.Y = txsRect.UpperLeftCorner.Y + txsRect.getHeight() * spost;
        currentRect.LowerRightCorner.X = txsRect.LowerRightCorner.X - txsRect.getWidth() * spost;
        currentRect.LowerRightCorner.Y = txsRect.LowerRightCorner.Y - txsRect.getHeight() * spost;

        scale = core::vector2df( AbsoluteRect.getWidth() / f32( currentRect.getWidth() ), 
            AbsoluteRect.getHeight() / f32( currentRect.getHeight() ) );

        video::SColor tmpColor( AlphaBlend, 0xff, 0xff, 0xff );
        tmpColor.setAlpha( alpha * 255 );
        driver->draw2DImage( _texture, AbsoluteRect.getCenter(), txsRect,
                             angle, true, scale, tmpColor, true );
        // Установить значение alpha (начальное = 0.2)
        // Увеличение spost (Приближение к центру текстуры)
        spost += _blurScale;
        // Уменьшение alpha (постепенное затухание рисунка)
        alpha = alpha - alphainc;
    }
}

void CNrpTextureSmoothAnimator::_DrawBlur()
{
    if( !_texture )
        return;

    core::vector2df scale;
    video::IVideoDriver* driver = Environment->getVideoDriver();

    core::recti txsRect( core::position2d<s32>(0,0), core::dimension2di( _texture->getOriginalSize()));
    float spost = 0.0f;    // Начальное смещение координат
    float alpha = AlphaBlend / 255.f;    // Начальное значение прозрачности
    float alphainc = alpha / _blurTimes;  // Скорость уменьшения прозрачности

    for (int num = 0;num < _blurTimes;num++)// Количество походов = times  
    {
        core::recti currentAbs;
        currentAbs.UpperLeftCorner.X = AbsoluteRect.UpperLeftCorner.X - AbsoluteRect.getWidth() * spost;
        currentAbs.UpperLeftCorner.Y = AbsoluteRect.UpperLeftCorner.Y - AbsoluteRect.getHeight() * spost;
        currentAbs.LowerRightCorner.X = AbsoluteRect.LowerRightCorner.X + AbsoluteRect.getWidth() * spost;
        currentAbs.LowerRightCorner.Y = AbsoluteRect.LowerRightCorner.Y + AbsoluteRect.getHeight() * spost;

        video::SColor tmpColor( AlphaBlend, 0xff, 0xff, 0xff );
        tmpColor.setAlpha( alpha * 255 );
        const video::SColor Colors[] = {tmpColor,tmpColor,tmpColor,tmpColor};
        driver->draw2DImage( _texture, currentAbs, txsRect, NULL, Colors, true );
        // Установить значение alpha (начальное = 0.2)
        // Увеличение spost (Приближение к центру текстуры)
        spost += _blurScale;
        // Уменьшение alpha (постепенное затухание рисунка)
        alpha = alpha - alphainc;
    }
}

void CNrpTextureSmoothAnimator::draw()
{
    if( _blurTimes == 0 )
        return;

    if( IGUIImage* img = static_cast< IGUIImage* >( Parent ) )
    {
        _texture = img->getImage();
        AbsoluteRect = img->getAbsolutePosition();
        _DrawBlurRotate( img->getRotate() );
    }
    else if( CNrpButton* btn = static_cast< CNrpButton* >( Parent ) )
    {
        if( !btn->isEnabled() )
            _texture = btn->getImage( EGBS_BUTTON_DISABLED );
        else
        {
            _texture = Environment->isHovered( btn ) 
                                ? btn->getImage( EGBS_BUTTON_MOUSE_OVER )
                                : btn->getImage( EGBS_BUTTON_UP );
        }
       
        AbsoluteRect = btn->getAbsolutePosition();
        _DrawBlurRotate( btn->getRotate() );
    }
    else
    {
        AbsoluteRect = core::recti( core::position2d<s32>(0,0), core::dimension2di( _texture->getOriginalSize()));
        _DrawBlurRotate( _angle );
    }
}

void CNrpTextureSmoothAnimator::setRepeatNumber( int times )
{
    _blurTimes = times;
}

void CNrpTextureSmoothAnimator::setBlurScale( float blurScale )
{
    _blurScale = blurScale;
}

void CNrpTextureSmoothAnimator::setRotate( u32 angle )
{
    _angle = angle;
}

}//namespace gui

}//namespace irr
