#ifndef _I_EXTENDED_TEXT_H_
#define _I_EXTENDED_TEXT_H_

#include <SColor.h>
#include <IGUIFont.h>

namespace irr
{
namespace gui
{

class ITextElement;

enum E_TEXT_ALIGN
{
    TEXT_ALIGN_LEFT = 0,
    TEXT_ALIGN_CENTER,
    TEXT_ALIGN_RIGHT
};

class IExtendedText
{
public:
    //! Changes the current text color. Following text will be dislayed in this color until another color is chosen.
    virtual void setCurrentColor( video::SColor color ) = 0;
    
    //! Returns the current text color.
    virtual video::SColor getCurrentColor() const = 0;
    
    //! Returns the default text color. The default text color is chosen by the IGUISkin.
    virtual video::SColor getDefaultColor() const = 0;
    
    //! Changes the current text font. Following text will be displayed in this font until another font is chosen.
    virtual void setCurrentFont( IGUIFont* font ) = 0;
    
    //! Returns the current text font.
    virtual IGUIFont* getCurrentFont() const = 0;
    
    //! Returns the default font. The default font is chosen by the IGUISkin.
    virtual IGUIFont* getDefaultFont() const = 0;
    
    //! Returns the total amount of pixels available on this line. Anchored objects will reduce this amount.
    virtual s32 getTotalLineSpace() const = 0;
    
    //! Returns the number of pixels currently remaining on this line.
    virtual s32 getLineSpaceRemaining() const = 0;
    
    //! Returns the height of this line so far. All lines are at least 8 pixels high.
    virtual s32 getLineHeight() const = 0;
    
    //! Inserts a new text element. A line-break is automatically inserted if there is insufficient space.
    virtual void insertTextElement( ITextElement* element ) = 0;
    
    //! Inserts an anchored text element. The element will be stuck on either side of the text, and text will wrap around it.
    virtual void insertAnchoredTextElement( ITextElement* element, bool leftAnchor ) = 0;
    
    //! Inserts a new line-break.
    virtual void insertLineBreak() = 0;
    
    //! Inserts text.
    virtual void appendText( const wchar_t* text ) = 0;
    
    //! Returns the alignment for this line.
    virtual E_TEXT_ALIGN getAlignment() = 0;
    
    //! Changes alignment. Unless the current line is empty, a new line will be inserted first.
    virtual void setAlignment( E_TEXT_ALIGN alignment ) = 0;
};

} // namespace gui
} // namespace irr

#endif
