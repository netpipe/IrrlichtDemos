#include "CGUIExtendedText.h"

#include "CPlainTextElement.h"

#include <IGUIEnvironment.h>
#include <IGUIFont.h>
#include <irrTypes.h>

#include "ITextTag.h"

namespace irr
{
namespace gui
{

CGUIExtendedText::CGUIExtendedText( IGUIEnvironment* environment, IGUIElement* parent, s32 id, const core::rect<s32>& rectangle, const wchar_t* text )
    : IGUIElement( EGUIET_ELEMENT, environment, parent, id, rectangle )
{
    #ifdef _DEBUG
    setDebugName( "CGUIExtendedText" );
    #endif
    
    if ( text != 0 )
        setText( text );
    
    LastPlainTextElement = NULL;
}

CGUIExtendedText::~CGUIExtendedText()
{
    // Free the text tags
    core::list<ITextTag*>::Iterator it = TextTags.begin();
    while ( it != TextTags.end() )
    {
        (*it)->drop();
        it = TextTags.erase(it);
    }
    
    clearLines();
}

void CGUIExtendedText::setText( const wchar_t* text )
{
    Text = text;
    parseText( text );
}

void CGUIExtendedText::draw()
{
    if ( Text.size() == 0 )
        return;
    
    if ( LastRelativeRect.getSize() != RelativeRect.getSize() )
        parseText( Text.c_str() );
    
    video::IVideoDriver* driver = Environment->getVideoDriver();
    
    s32 y = AbsoluteRect.UpperLeftCorner.Y;
    
    // Render all lines
    for ( s32 i=0; i<Lines.size(); i++ )
    {
        // Is this line within the clipping rect?
        if ( y <= AbsoluteClippingRect.LowerRightCorner.Y && y + Lines[i]->Height >= AbsoluteClippingRect.UpperLeftCorner.Y )
        {
            E_TEXT_ALIGN align = Lines[i]->Alignment;
            s32 width = Lines[i]->Width;
            
            s32 leftX = AbsoluteRect.UpperLeftCorner.X + Lines[i]->LeftMargin;
            s32 rightX = AbsoluteRect.LowerRightCorner.X - Lines[i]->RightMargin;
            s32 xWidth = rightX - leftX;
            
            // Find the starting X value
            s32 x;
            switch(align)
            {
                case TEXT_ALIGN_LEFT:
                    x = leftX;
                    break;
                    
                case TEXT_ALIGN_CENTER:
                    x = leftX + (xWidth - width) / 2;
                    break;
                
                case TEXT_ALIGN_RIGHT:
                    x = leftX + xWidth - width;
                    break;
            }
            
            core::list<ITextElement*>::Iterator it = Lines[i]->Elements.begin();
            while ( it != Lines[i]->Elements.end() )
            {
                ITextElement* element = (*it);
                
                core::rect<s32> dstRect = core::rect<s32>( core::position2d<s32>( x, y ), element->getDimension() );
                
                element->draw( dstRect, AbsoluteClippingRect, driver );
                
                x += element->getDimension().Width;
                
                it++;
            }
        }
        y += Lines[i]->Height;
    }
    
    // Render all anchored elements
    core::list<SAnchoredElement>::Iterator ait = AnchoredElements.begin();
    while ( ait != AnchoredElements.end() )
    {
        ITextElement* elem = (*ait).Element;
        
        core::dimension2d<s32> dim = elem->getDimension();
        
        s32 y = AbsoluteRect.UpperLeftCorner.Y + (*ait).BottomY - dim.Height;
        
        s32 x;
        if ( (*ait).IsLeft )
            x = AbsoluteRect.UpperLeftCorner.X;
        else
            x = AbsoluteRect.LowerRightCorner.X - dim.Width;
        
        elem->draw( core::rect<s32>( core::position2d<s32>(x,y), dim ), AbsoluteClippingRect, driver );
        
        ait++;
    }
    
    IGUIElement::draw();
}

void CGUIExtendedText::addTextTag( ITextTag* textTag )
{
    if ( textTag != NULL )
    {
        textTag->grab();
        TextTags.push_back( textTag );
    }
}

ITextTag* CGUIExtendedText::getTextTagMatching( const core::stringw& tag )
{
    core::list<ITextTag*>::Iterator it = TextTags.begin();
    
    while ( it != TextTags.end() )
    {
        ITextTag* textTag = (*it);
        
        if ( textTag->match(tag) )
            return textTag;
        
        it++;
    }
    
    return NULL;
}

void CGUIExtendedText::setCurrentColor( video::SColor color )
{
    CurrentColor = color;
    LastPlainTextElement = NULL;
}

video::SColor CGUIExtendedText::getCurrentColor() const
{
    return CurrentColor;
}

video::SColor CGUIExtendedText::getDefaultColor() const
{
    return DefaultColor;
}

void CGUIExtendedText::setCurrentFont( IGUIFont* font )
{
    CurrentFont = font;
    LastPlainTextElement = NULL;
}

IGUIFont* CGUIExtendedText::getCurrentFont() const
{
    return CurrentFont;
}

IGUIFont* CGUIExtendedText::getDefaultFont() const
{
    return DefaultFont;
}

s32 CGUIExtendedText::getTotalLineSpace() const
{
    return LineSpace;
}

s32 CGUIExtendedText::getLineSpaceRemaining() const
{
    return LineSpaceRemaining;
}

s32 CGUIExtendedText::getLineHeight() const
{
    return Lines[CurrentLine]->Height;
}

//! Picks up a word from the string, and advances 'c' past the word.
inline core::stringw parseWord( const wchar_t* &c )
{
    core::stringw str;
    while ( *c != 0 && *c != L' ' && *c != L'\n' && *c != L'\r' && *c != L'#' )
    {
        str += *c;
        c++;
    }
    return str;
}

//! Picks up all the concecutive whitespace, and advances 'c' past the whitespace.
inline core::stringw parseWhitespace( const wchar_t* &c )
{
    core::stringw str;
    while ( *c == ' ' )
    {
        str += *c;
        c++;
    }
    return str;
}

//! Picks up all linebreak characters, and returns the number of linebreaks parsed.
//! 'c' is advanced past the last linebreak.
inline int parseLineBreak( const wchar_t* &c )
{
    s32 numN = 0;
    s32 numR = 0;
    while ( *c == L'\n' || *c == L'\r' )
    {
        if ( *c == L'\n' )
            numN++;
        else
            numR++;
        
        c++;
    }
    
    if ( numR > numN )
        return numR;
        
    return numN;
}

//! Parses a token. Returns true if a token was found. 'c' is advanced past the token (past the last '#').
//! The token's name is stored in 'tokenName', and the parameters added to 'params'.
inline bool parseToken( const wchar_t* &c, core::stringw* tokenName, core::array<core::stringw>* params )
{
    if ( *c != L'#' )
        return false;
    
    c++;
    
    bool tokenNameFound = false;
    
    core::stringw str;
    
    while ( *c != 0 && *c != L'#' )
    {
        if ( *c == L':' )
        {
            if ( !tokenNameFound )
            {
                tokenNameFound = true;
                *tokenName = str;
            }
            else
                params->push_back( str );
            
            str = L"";
        }
        else
        {
            str += *c;
        }
        c++;
    }
    
    if ( !tokenNameFound )
        *tokenName = str;
    else
        params->push_back( str );
    
    if ( *c == L'#' )
    {
        c++;
        return true;
    }
    
    return false;
}

void CGUIExtendedText::insertTextElement( ITextElement* element )
{
    core::dimension2d<s32> dim = element->getDimension();
    
    if ( dim.Width > LineSpaceRemaining )
        insertLineBreak();
    
    LineSpaceRemaining -= dim.Width;
    
    if ( dim.Height > Lines[CurrentLine]->Height )
        Lines[CurrentLine]->Height = dim.Height;
    
    Lines[CurrentLine]->Elements.push_back( element );
    Lines[CurrentLine]->Width += dim.Width;
    
    LastPlainTextElement = NULL;
}

void CGUIExtendedText::insertAnchoredTextElement( ITextElement* element, bool leftAnchor )
{
    core::dimension2d<s32> dim = element->getDimension();
    
    if ( FullLineSpace < dim.Width ) // There simply isn't room for this element
        return;
    
    // Insert line-breaks until there is room for this element
    while ( dim.Width > LineSpaceRemaining )
        insertLineBreak();
    
    SAnchoredElement anch;
    anch.FarX = dim.Width + (leftAnchor? CurrentLeftMargin : CurrentRightMargin);
    anch.BottomY = TotalHeight + dim.Height;
    anch.IsLeft = leftAnchor;
    anch.Element = element;
    
    AnchoredElements.push_back( anch );
    
    updateMargins();
    
    Lines[CurrentLine]->LeftMargin = CurrentLeftMargin;
    Lines[CurrentLine]->RightMargin = CurrentRightMargin;
    
    LineSpace -= dim.Width;
    LineSpaceRemaining -= dim.Width;
}

void CGUIExtendedText::insertLineBreak()
{
    Lines.push_back( new SLine() );
    
    CurrentLine++;
    
    Lines[CurrentLine]->Alignment = Lines[CurrentLine-1]->Alignment;
    
    TotalHeight += Lines[CurrentLine-1]->Height;
    
    updateMargins();
    
    Lines[CurrentLine]->LeftMargin = CurrentLeftMargin;
    Lines[CurrentLine]->RightMargin = CurrentRightMargin;
    
    LineSpace = FullLineSpace - CurrentLeftMargin - CurrentRightMargin;
    
    LineSpaceRemaining = LineSpace;
    
    LastPlainTextElement = NULL;
}

void CGUIExtendedText::appendText( const wchar_t* text )
{
    appendText( text, CurrentFont->getDimension(text) );
}

E_TEXT_ALIGN CGUIExtendedText::getAlignment()
{
    return Lines[CurrentLine]->Alignment;
}

void CGUIExtendedText::setAlignment( E_TEXT_ALIGN alignment )
{
    if ( Lines[CurrentLine]->Alignment != alignment )
    {
        if ( Lines[CurrentLine]->Width > 0 )
        {
            insertLineBreak();
        }
        Lines[CurrentLine]->Alignment = alignment;
    }
}

void CGUIExtendedText::parseText( const wchar_t* text )
{
    DefaultFont = Environment->getSkin()->getFont();
    DefaultColor = Environment->getSkin()->getColor( IsEnabled? EGDC_BUTTON_TEXT : EGDC_GRAY_TEXT );
    
    CurrentFont = DefaultFont;
    CurrentColor = DefaultColor;
    
    if ( CurrentFont == NULL )
        return;
    
    CurrentLeftMargin = 0;
    CurrentRightMargin = 0;
    TotalHeight = 0;
    
    FullLineSpace = RelativeRect.getWidth();
    LineSpace = FullLineSpace;
    LineSpaceRemaining = LineSpace;
    
    CurrentLine = 0;
    
    clearLines();
    Lines.push_back( new SLine() );
    
    s32 whiteSpaceSize = CurrentFont->getDimension( L" " ).Width;
    core::dimension2d<s32> squareDim = CurrentFont->getDimension( L"#" );
    
    LastPlainTextElement = NULL;
    
    core::array<core::stringw> tokenParams;
    
    const wchar_t* c = text;
    while ( *c != 0 )
    {
        // Insert word
        core::stringw str = parseWord(c);
        
        if ( str.size() > 0 )
        {
            appendText( str.c_str(), CurrentFont->getDimension(str.c_str()) );
        }
        
        
        // Insert whitespace
        str = parseWhitespace(c);
        
        if ( str.size() > 0 )
        {
            s32 size = whiteSpaceSize * str.size();
            
            if ( LineSpaceRemaining < size )
            {
                insertLineBreak();
            }
            else
            {
                appendText( str.c_str(), core::dimension2d<s32>( size, 1 ) );
            }
        }
        
        // Insert linebreaks
        int numLineBreaks = parseLineBreak(c);
        
        for ( s32 i=0; i<numLineBreaks; i++ )
        {
            insertLineBreak();
        }
        
        // Parse token
        tokenParams.set_used(0);
        if ( parseToken( c, &str, &tokenParams ) )
        {
            // Is it an empty token? ("##")
            if ( str == L"" )
            {
                appendText( L"#", squareDim );
            }
            // Now this is a real token
            else
            {
                ITextTag* textTag = getTextTagMatching( str );
                
                if ( textTag != NULL )
                {
                    textTag->create( this, str, tokenParams );
                }
            }
        }
    }
    
    RelativeRect.LowerRightCorner.Y = RelativeRect.UpperLeftCorner.Y + TotalHeight;
    LastRelativeRect = RelativeRect;
}

void CGUIExtendedText::appendText( const wchar_t* text, const core::dimension2d<s32>& dim )
{
    if ( dim.Width > LineSpaceRemaining )
    {
        insertLineBreak();
    }
    
    if ( LastPlainTextElement != NULL )
    {
        LastPlainTextElement->append( text, dim );
                
        LineSpaceRemaining -= dim.Width;
        Lines[CurrentLine]->Width += dim.Width;
        
        if ( Lines[CurrentLine]->Height < dim.Height )
            Lines[CurrentLine]->Height = dim.Height;
    }
    else
    {
        LastPlainTextElement = new CPlainTextElement( text, dim, CurrentColor, CurrentFont );
        
        insertTextElement( LastPlainTextElement );
    }
}

void CGUIExtendedText::clearLines()
{
    // Free the lines
    for ( s32 i=0; i<Lines.size(); i++ )
    {
        core::list<ITextElement*>::Iterator it = Lines[i]->Elements.begin();
        while ( it != Lines[i]->Elements.end() )
        {
            delete *it;
            it = Lines[i]->Elements.erase(it);
        }
        delete Lines[i];
    }
    Lines.set_used(0);
    
    core::list<SAnchoredElement>::Iterator it = AnchoredElements.begin();
    while ( it != AnchoredElements.end() )
    {
        delete (*it).Element;
        it = AnchoredElements.erase(it);
    }
}

void CGUIExtendedText::updateMargins()
{
    CurrentLeftMargin = 0;
    CurrentRightMargin = 0;
    
    core::list<SAnchoredElement>::Iterator it = AnchoredElements.begin();
    while ( it != AnchoredElements.end() )
    {
        if ( (*it).BottomY >= TotalHeight )
        {
            if ( (*it).IsLeft && (*it).FarX > CurrentLeftMargin )
                CurrentLeftMargin = (*it).FarX;
            else
            if ( (!(*it).IsLeft) && (*it).FarX > CurrentRightMargin )
                CurrentRightMargin = (*it).FarX;
        }
        it++;
    }
}

} // namespace gui
} // namespace irr
