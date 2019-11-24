//#include "stdafx.h"
//#include "Nrp2DPictureFlow.h"
//#include "NrpRotatableImage.h"
#include "listFileSwitch.h"

#include <iostream>
using namespace std;
namespace irr
{

namespace gui
{

CNrp2DPictureFlow::CNrp2DPictureFlow( IrrlichtDevice *device2,IGUIEnvironment* env,
                          IGUIElement* parent,
                          core::recti rectangle,
                          core::recti pictureRect,
                          s32 id )
: IGUIListBox( env, parent, id, rectangle )
{
   device=device2;
   activeIndex_ = 0;
   pictureRect_ = pictureRect;
}

u32 CNrp2DPictureFlow::addItem( video::ITexture* texture, const wchar_t* text )
{
   CNrpImageDescription* descr = new CNrpImageDescription();
   images_.push_back( descr );
   descr->texture = texture;
   texture->grab();
   descr->rectangle = core::recti( 0, 0, 0, 0 );

   UpdateImages_();

   return images_.size() - 1;
}

irr::u32 CNrp2DPictureFlow::addItem( const wchar_t* text )
{
   return addItem( NULL, text );
}

void CNrp2DPictureFlow::UpdateImages_()
{
   core::recti tmpRect( AbsoluteRect.getCenter().X - pictureRect_.getWidth()/2,
                   AbsoluteRect.getCenter().Y - pictureRect_.getHeight()/2,
                   AbsoluteRect.getCenter().X + pictureRect_.getWidth()/2,
                   AbsoluteRect.getCenter().Y + pictureRect_.getHeight()/2 );

   if( activeIndex_ < (int)images_.size() )
      images_[ activeIndex_ ]->rectangle = tmpRect;

   s32 offsetx = 0;
   core::recti lRect = tmpRect;
   if( activeIndex_ - 1 >= 0 )
      for( int k=max( 0, activeIndex_-1); k >= 0; k-- )
      {
         offsetx += lRect.getWidth() * (0.7f - (activeIndex_-k)*0.1f);
         core::dimension2di sides( 0.7f * lRect.getWidth(), 0.7f * lRect.getHeight() );
         lRect = core::recti( AbsoluteRect.getCenter().X - sides.Width/2, AbsoluteRect.getCenter().Y - sides.Height/2,
            AbsoluteRect.getCenter().X + sides.Width/2, AbsoluteRect.getCenter().Y + sides.Height/2 );
         images_[ k ]->rectangle = lRect - core::position2di( offsetx, 0 );
      }

   offsetx = 0;
   core::recti rRect = tmpRect;
   for( int k = min(activeIndex_+1, (int)images_.size()); k < images_.size(); k++ )
   {
      offsetx += rRect.getWidth() * (0.7f - (k-activeIndex_)*0.1f);
      core::dimension2di sides( 0.7f * rRect.getWidth(), 0.7f * rRect.getHeight() );
      rRect = core::recti( AbsoluteRect.getCenter().X - sides.Width/2, AbsoluteRect.getCenter().Y - sides.Height/2,
                      AbsoluteRect.getCenter().X + sides.Width/2, AbsoluteRect.getCenter().Y + sides.Height/2 );
      images_[ k ]->rectangle = rRect + core::position2di( offsetx, 0 );
   }
}

void CNrp2DPictureFlow::draw()
{
   if( !IsVisible )
      return;

   UpdatePositions_();

   for( size_t pos=max( 0, activeIndex_-4 ); pos < activeIndex_; pos++ )
   {
      core::recti rectangle( images_[ pos ]->currentRect.UpperLeftCorner.X, images_[ pos ]->currentRect.UpperLeftCorner.Y,
                        images_[ pos ]->currentRect.LowerRightCorner.X, images_[ pos ]->currentRect.LowerRightCorner.Y );
      Environment->getVideoDriver()->draw2DImage( images_[ pos ]->texture, rectangle,
                                       core::recti( core::position2di( 0, 0), images_[ pos ]->texture->getSize() ));
   }


   for( size_t pos=min( activeIndex_ + 4, (int)images_.size()-1); pos > activeIndex_; pos-- )
   {
      core::recti rectangle( images_[ pos ]->currentRect.UpperLeftCorner.X, images_[ pos ]->currentRect.UpperLeftCorner.Y,
                        images_[ pos ]->currentRect.LowerRightCorner.X, images_[ pos ]->currentRect.LowerRightCorner.Y );
      Environment->getVideoDriver()->draw2DImage( images_[ pos ]->texture, rectangle,
                                                  core::recti( core::position2di( 0, 0), images_[ pos ]->texture->getSize() ));
   }

   if( images_.size() )
   {
      core::recti rectangle( images_[ activeIndex_ ]->currentRect.UpperLeftCorner.X, images_[ activeIndex_ ]->currentRect.UpperLeftCorner.Y,
         images_[ activeIndex_ ]->currentRect.LowerRightCorner.X, images_[ activeIndex_ ]->currentRect.LowerRightCorner.Y );
      Environment->getVideoDriver()->draw2DImage( images_[ activeIndex_ ]->texture, rectangle,
         core::recti( core::position2di( 0, 0), images_[ activeIndex_ ]->texture->getSize() ));
   }
   IGUIListBox::draw();
}

void CNrp2DPictureFlow::UpdatePositions_()
{
   for( size_t k=0; k < images_.size(); k++ )
   {
      if( images_[ k ]->rectangle.LowerRightCorner.X != images_[ k ]->currentRect.LowerRightCorner.X )
      {
         f32 offset = (images_[ k ]->rectangle.LowerRightCorner.X - images_[ k ]->currentRect.LowerRightCorner.X)
                                          /(float)Environment->getVideoDriver()->getFPS();

         images_[ k ]->currentRect.LowerRightCorner.X += offset;
      }

      if( images_[ k ]->rectangle.LowerRightCorner.Y != images_[ k ]->currentRect.LowerRightCorner.Y )
      {
         f32 offset = (images_[ k ]->rectangle.LowerRightCorner.Y - images_[ k ]->currentRect.LowerRightCorner.Y)
            /(float)Environment->getVideoDriver()->getFPS();

         images_[ k ]->currentRect.LowerRightCorner.Y += offset;
      }

      if( images_[ k ]->rectangle.UpperLeftCorner.X != images_[ k ]->currentRect.UpperLeftCorner.X )
      {
         f32 offset = (images_[ k ]->rectangle.UpperLeftCorner.X - images_[ k ]->currentRect.UpperLeftCorner.X)
            /(float)Environment->getVideoDriver()->getFPS();

         images_[ k ]->currentRect.UpperLeftCorner.X += offset;
      }

      if( images_[ k ]->rectangle.UpperLeftCorner.Y != images_[ k ]->currentRect.UpperLeftCorner.Y )
      {
         f32 offset = (images_[ k ]->rectangle.UpperLeftCorner.Y - images_[ k ]->currentRect.UpperLeftCorner.Y)
            /(float)Environment->getVideoDriver()->getFPS();

         images_[ k ]->currentRect.UpperLeftCorner.Y += offset;
      }
   }
}

s32 CNrp2DPictureFlow::getcurrent( )
{
return activeIndex_;
}

void CNrp2DPictureFlow::Prev( int offset )
{
   activeIndex_ -= offset;
   activeIndex_ = max( activeIndex_, 0 );

   UpdateImages_();
}

void CNrp2DPictureFlow::Next( int offset )
{
   activeIndex_ += offset;
   activeIndex_ = min( activeIndex_, (int)images_.size()-1 );

   UpdateImages_();
}

bool CNrp2DPictureFlow::OnEvent( const SEvent& event )
{
    GetTickCount =  device->getTimer()->getTime();
   s32 curTime = GetTickCount;
   switch( event.EventType )
   {
   case EET_KEY_INPUT_EVENT:
      //uncheck for double event of keyInput
      if (event.KeyInput.PressedDown && event.KeyInput.Key == KEY_RIGHT && (curTime - lastTimeKey_ > 20))
      {
         Next( +1 );
         lastTimeKey_ = GetTickCount;
         return true;
      }

      if (event.KeyInput.PressedDown && event.KeyInput.Key == KEY_LEFT && (curTime - lastTimeKey_ > 20))
      {
         Prev( 1 );
         lastTimeKey_ = GetTickCount;
         return true;
      }
   break;

   case EET_MOUSE_INPUT_EVENT:
      if( event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN )
      {
         int side = AbsoluteRect.getWidth() / 5;
         if( event.MouseInput.X > AbsoluteRect.getCenter().X / 2 )
         {
            int offset = (event.MouseInput.X-AbsoluteRect.getCenter().X / 2) / side - 1;
            Next( offset );
         }
         else
         {
            int offset = (AbsoluteRect.getCenter().X / 2 - event.MouseInput.X) / side + 1;
            Prev( offset );
         }
         return true;
      }
   break;
   }

   return IGUIElement::OnEvent(event);
}

CNrp2DPictureFlow::~CNrp2DPictureFlow()
{
   for( size_t pos=0; pos < images_.size(); pos++ )
   {
      images_[ pos ]->texture->drop();
      delete images_[ pos ];
   }
}

const wchar_t* CNrp2DPictureFlow::getListItem( u32 id ) const
{
   return images_[ id ]->name.c_str();
}

void CNrp2DPictureFlow::setSelected( const wchar_t *item )
{

}

void CNrp2DPictureFlow::setSelected( s32 index )
{
   if( index < (int)images_.size() && index >= 0 )
      activeIndex_ = index;
}

void CNrp2DPictureFlow::removeItem( u32 index )
{
   if( index < images_.size() )
   {
      images_[ index ]->texture->drop();
      delete images_[ index ];
      images_.erase( index );
   }
}
irr::s32 CNrp2DPictureFlow::getItemAt( s32 xpos, s32 ypos ) const
{
//    assert( false && "func not supported" );
    return -1;
}
#if (IRRLICHT_VERSION_MAJOR==1) && (IRRLICHT_VERSION_MINOR==7)
void CNrp2DPictureFlow::setItemOverrideColor( u32 index, const video::SColor& color )
#else
void CNrp2DPictureFlow::setItemOverrideColor( u32 index, video::SColor color )
#endif
{

}

void CNrp2DPictureFlow::clear()
{
   for( u32 k=0; k < images_.size(); k++ )
   {
      images_[ k ]->texture->drop();
      delete images_[ k ];
   }

   images_.clear();
   activeIndex_ = 0;
}
}//end namespace gui

}//end namespace irr
