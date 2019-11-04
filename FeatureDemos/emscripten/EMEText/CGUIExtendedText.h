#ifndef _C_GUI_EXTENDED_TEXT_H_
#define _C_GUI_EXTENDED_TEXT_H_

#include <IGUIElement.h>
#include <SColor.h>
#include <irrArray.h>
#include <dimension2d.h>
#include "IExtendedText.h"

namespace irr
{
namespace gui
{

class CPlainTextElement;
class ITextElement;
class IGUIFont;
class ITextTag;

class CGUIExtendedText : public IGUIElement, private IExtendedText
{
public:
    CGUIExtendedText( IGUIEnvironment* environment, IGUIElement* parent, s32 id, const core::rect<s32>& rectangle, const wchar_t* text=0 );
    virtual ~CGUIExtendedText();

    virtual void setText( const wchar_t* text );

    virtual void draw();

    void addTextTag( ITextTag* textTag );

    ITextTag* getTextTagMatching( const core::stringw& tag );

private:
    virtual void setCurrentColor( video::SColor color );
    virtual video::SColor getCurrentColor() const;
    virtual video::SColor getDefaultColor() const;

    virtual void setCurrentFont( IGUIFont* font );
    virtual IGUIFont* getCurrentFont() const;
    virtual IGUIFont* getDefaultFont() const;

    virtual s32 getTotalLineSpace() const;
    virtual s32 getLineSpaceRemaining() const;
    virtual s32 getLineHeight() const;

    virtual void insertTextElement( ITextElement* element );
    virtual void insertAnchoredTextElement( ITextElement* element, bool leftAnchor );
    virtual void insertLineBreak();

    virtual void appendText( const wchar_t* text );

    virtual E_TEXT_ALIGN getAlignment();
    virtual void setAlignment( E_TEXT_ALIGN alignment );

private:
    void parseText( const wchar_t* text );

    void appendText( const wchar_t* text, const core::dimension2d<s32>& dim );

    void clearLines();

    void updateMargins();

    video::SColor CurrentColor;
    IGUIFont* CurrentFont;

    video::SColor DefaultColor;
    IGUIFont* DefaultFont;

    s32 LineSpaceRemaining;
    s32 FullLineSpace;
    s32 LineSpace;
    s32 CurrentLine;

    s32 CurrentLeftMargin;
    s32 CurrentRightMargin;

    s32 TotalHeight;

    core::rect<s32> LastRelativeRect;

    struct SAnchoredElement
    {
        s32 FarX;
        s32 BottomY;
        bool IsLeft;
        ITextElement* Element;
    };

    core::list<SAnchoredElement> AnchoredElements;

    CPlainTextElement* LastPlainTextElement;

    core::list<ITextTag*> TextTags;

    struct SLine
    {
        s32 Height;
        s32 Width;
        s32 LeftMargin;
        s32 RightMargin;
        E_TEXT_ALIGN Alignment;
        core::list<ITextElement*> Elements;

        SLine() : Height(8), Width(0), LeftMargin(0), RightMargin(0), Alignment(TEXT_ALIGN_LEFT) {}
    };

    core::array<SLine*> Lines;


};

} // namespace gui
} // namespace irr

#endif
