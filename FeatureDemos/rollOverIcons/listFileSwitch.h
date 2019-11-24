
#pragma once
#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

namespace irr
{

namespace gui
{

class CNrp2DPictureFlow : public IGUIListBox
{
   class CNrpImageDescription
   {
   public:
      video::ITexture* texture;
      core::recti rectangle;
      core::rectf currentRect;
      core::stringw name;
   };

   CNrp2DPictureFlow();
public:
   ~CNrp2DPictureFlow();
   CNrp2DPictureFlow( IrrlichtDevice *device,IGUIEnvironment* env,
                  IGUIElement* parent,
                  core::recti rectangle,
                  core::recti pictureRect,
                  s32 id );
   u32 addItem( video::ITexture* texture, const wchar_t* text );
   u32 addItem(const wchar_t* text);
   void removeItem(u32 index);

virtual IGUIScrollBar* getVerticalScrollBar() const { return 0; }
  // IGUIScrollBar* getVerticalScrollBar() const = 0 ;
 //  IGUIScrollBar* getVerticalScrollBar() const = 0;
   bool OnEvent(const SEvent& event);

   void draw();
   void Next( int offset );
   void Prev( int offset );
   s32 getcurrent();

   void clear();
   s32 getSelected() const { return activeIndex_; }
   void setSelected(const wchar_t *item);
   void setSelected(s32 index);
   void setDrawBackground(bool draw) { drawBackground_ = draw; }

   u32 getItemCount() const { return images_.size(); }
   const wchar_t* getListItem(u32 id) const;

   float GetTickCount;
#if (IRRLICHT_VERSION_MAJOR==1) && (IRRLICHT_VERSION_MINOR==7)
    //! set all item colors at given index to color
    virtual void setItemOverrideColor(u32 index, const video::SColor &color);

    //! set all item colors of specified type at given index to color
    virtual void setItemOverrideColor(u32 index, EGUI_LISTBOX_COLOR colorType, const video::SColor &color) {};
#else
    //! set all item colors at given index to color
    virtual void setItemOverrideColor(u32 index, video::SColor color);

    //! set all item colors of specified type at given index to color
    virtual void setItemOverrideColor(u32 index, EGUI_LISTBOX_COLOR colorType, video::SColor color) {};
#endif
float scalem=0.7f;
float scaler=0.7f;
float scalel=0.7f;
void setscale(float,float,float);

private:

IrrlichtDevice *device;
   void UpdateImages_();
   void UpdatePositions_();
   void clearItemOverrideColor(u32 index, EGUI_LISTBOX_COLOR colorType) {}
   void clearItemOverrideColor(u32 index) {}
   bool hasItemOverrideColor(u32 index, EGUI_LISTBOX_COLOR colorType) const { return false; }
   video::SColor getItemOverrideColor(u32 index, EGUI_LISTBOX_COLOR colorType) const {return 0;}
   video::SColor getItemDefaultColor(EGUI_LISTBOX_COLOR colorType) const { return 0; }
   void setItem(u32 index, const wchar_t* text, s32 icon) {}
   void setItemOverrideColor(u32 index, const video::SColor &color) {}
   void setItemOverrideColor(u32 index, EGUI_LISTBOX_COLOR colorType, const video::SColor &color) {};
   u32 addItem(const wchar_t* text, s32 icon) { return 0; }
   s32 getIcon(u32 index) const { return 0; }
   void setSpriteBank(IGUISpriteBank* bank) {}
   void setAutoScrollEnabled(bool scroll) {}
   bool isAutoScrollEnabled() const { return false; }
   void setItemHeight( s32 height ) {};
   void swapItems(u32 index1, u32 index2) {}
   s32 insertItem(u32 index, const wchar_t* text, s32 icon) { return 0; }
    virtual s32 getItemAt(s32 xpos, s32 ypos) const;
   s32 activeIndex_;
   s32 lastTimeKey_;

   core::array< CNrpImageDescription* > images_;

   bool drawBackground_;
   core::recti pictureRect_;
};

}//end namespace gui

}//end namespace irr

