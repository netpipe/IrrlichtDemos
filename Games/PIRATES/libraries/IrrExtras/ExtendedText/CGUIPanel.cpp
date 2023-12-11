#include "CGUIPanel.h"

#include <IGUIEnvironment.h>
#include <IGUIScrollBar.h>
#include <IVideoDriver.h>

const int SCROLL_BAR_SIZE = 16; // Scrolls bars are 16 pixels wide
const int BORDER_WIDTH = 2;

namespace irr
{
namespace gui
{

CGUIPanel::CGUIPanel( IGUIEnvironment* environment, IGUIElement* parent, s32 id, const core::rect<s32>& rectangle,
            bool border,
            E_SCROLL_BAR_MODE vMode,
            E_SCROLL_BAR_MODE hMode )
    : IGUIElement( EGUIET_ELEMENT, environment, parent, id, rectangle )
{
    Border = border;
    VScrollBarMode = vMode;
    HScrollBarMode = hMode;
    
    s32 width = rectangle.getWidth();
    s32 height = rectangle.getHeight();
    
    core::rect<s32> rct = core::rect<s32>(
            width - SCROLL_BAR_SIZE,
            0,
            width,
            height );
    
    VScrollBar = environment->addScrollBar( false, rct, this, id );
    VScrollBar->grab();
    
    rct = core::rect<s32>(
        0,
        height - SCROLL_BAR_SIZE,
        width - SCROLL_BAR_SIZE, // this is to avoid both scrollbars fighting for the corner
        height );
    
    HScrollBar = environment->addScrollBar( true, rct, this, id );
    HScrollBar->grab();
    
    CurrentVScroll = 0;
    CurrentHScroll = 0;
    
    calculateClientArea();
}

CGUIPanel::~CGUIPanel()
{
    VScrollBar->drop();
    HScrollBar->drop();
}

void CGUIPanel::draw()
{
    video::IVideoDriver* driver = Environment->getVideoDriver();
    
    if ( Border )
    {
        IGUISkin* skin = Environment->getSkin();
        skin->draw3DSunkenPane( this, skin->getColor( EGDC_WINDOW), true,true, AbsoluteRect, &AbsoluteClippingRect );
    }
    
    s32 newHPos = HScrollBar->getPos();
    s32 newVPos = VScrollBar->getPos();
    
    s32 dv = newVPos - CurrentVScroll;
    s32 dh = newHPos - CurrentHScroll;
    
    CurrentVScroll = newVPos;
    CurrentHScroll = newHPos;
    
    core::position2d<s32> movement = core::position2d<s32>(-newHPos,-newVPos);
    core::position2d<s32> negmov = core::position2d<s32>(newHPos,newVPos);
    
    if ( Border )
    {
        movement.X += BORDER_WIDTH;
        negmov.X -= BORDER_WIDTH;
    }
    
    core::rect<s32> oldClip = AbsoluteClippingRect;
    
    AbsoluteClippingRect.clipAgainst( ClientArea );
    
    s32 maxY = 0;
    s32 maxX = 0;
    
    core::list<IGUIElement*>::Iterator it = Children.begin();
    while ( it != Children.end() )
    {
        if ( (*it) != HScrollBar && (*it) != VScrollBar )
        {
            (*it)->move( movement );
            
            (*it)->draw();
            
            (*it)->move( negmov );
            
            core::rect<s32> rct = (*it)->getRelativePosition();
            
            if ( rct.LowerRightCorner.X > maxX )
            {
                maxX = rct.LowerRightCorner.X;
            }
            if ( rct.LowerRightCorner.Y > maxY )
            {
                maxY = rct.LowerRightCorner.Y;
            }
        }
        
        it++;
    }
    
    AbsoluteClippingRect = oldClip;
    
    if ( (maxX > ClientArea.getWidth() || HScrollBarMode == ESBM_ALWAYS_VISIBLE) && HScrollBarMode != ESBM_ALWAYS_INVISIBLE )
    {
        HScrollBar->setVisible(true);
        HScrollBar->setMax( maxX - ClientArea.getWidth() );
    }
    else
        HScrollBar->setVisible(false);
    
    if ( (maxY > ClientArea.getHeight() || VScrollBarMode == ESBM_ALWAYS_VISIBLE) && VScrollBarMode != ESBM_ALWAYS_INVISIBLE )
    {
        VScrollBar->setVisible(true);
        VScrollBar->setMax( maxY - ClientArea.getHeight() );
    }
    else
        VScrollBar->setVisible(false);
    
    calculateClientArea();
    
    HScrollBar->draw();
    VScrollBar->draw();
}

bool CGUIPanel::hasBorder() const
{
    return Border;
}

void CGUIPanel::setBorder( bool enabled )
{
    Border = enabled;
}

IGUIScrollBar* CGUIPanel::getVScrollBar() const
{
    return VScrollBar;
}

void CGUIPanel::setVScrollBar( IGUIScrollBar* scrollBar )
{
    if ( scrollBar == NULL )
        return;
    
    VScrollBar->remove();
    VScrollBar->drop();
    
    VScrollBar = scrollBar;
    VScrollBar->grab();
    
    VScrollBar->remove();
    addChild( VScrollBar );
}

IGUIScrollBar* CGUIPanel::getHScrollBar() const
{
    return HScrollBar;
}

void CGUIPanel::setHScrollBar( IGUIScrollBar* scrollBar )
{
    if ( scrollBar == NULL )
        return;
    
    HScrollBar->remove();
    HScrollBar->drop();
    
    HScrollBar = scrollBar;
    HScrollBar->grab();
    
    HScrollBar->remove();
    addChild( HScrollBar );
}

E_SCROLL_BAR_MODE CGUIPanel::getVScrollBarMode() const
{
    return VScrollBarMode;
}

void CGUIPanel::setVScrollBarMode( E_SCROLL_BAR_MODE mode )
{
    VScrollBarMode = mode;
}

E_SCROLL_BAR_MODE CGUIPanel::getHScrollBarMode() const
{
    return HScrollBarMode;
}

void CGUIPanel::setHScrollBarMode( E_SCROLL_BAR_MODE mode )
{
    HScrollBarMode = mode;
}

void CGUIPanel::fitChildrenToPanel()
{
    fitChildrenToPanel( HScrollBarMode == ESBM_ALWAYS_INVISIBLE, VScrollBarMode == ESBM_ALWAYS_INVISIBLE );
}

void CGUIPanel::fitChildrenToPanel( bool horizontal, bool vertical )
{
    core::list<IGUIElement*>::Iterator it = Children.begin();
    while ( it != Children.end() )
    {
        if ( (*it) != VScrollBar && (*it) != HScrollBar )
        {
            core::rect<s32> rct = (*it)->getRelativePosition();
            
            if ( horizontal && rct.LowerRightCorner.X > ClientArea.getWidth() )
                rct.LowerRightCorner.X = ClientArea.getWidth();
            
            if ( vertical && rct.LowerRightCorner.Y > ClientArea.getHeight() )
                rct.LowerRightCorner.Y = ClientArea.getHeight();
            
            (*it)->setRelativePosition( rct );
        }
        it++;
    }
}

bool CGUIPanel::OnEvent( SEvent event )
{
    // Redirect mouse wheel to scrollbar
    if ( event.EventType == EET_MOUSE_INPUT_EVENT && event.MouseInput.Event == EMIE_MOUSE_WHEEL )
    {
        VScrollBar->OnEvent(event);
    }
    else
    {
        return IGUIElement::OnEvent( event );
    }
}

void CGUIPanel::calculateClientArea()
{
    ClientArea = AbsoluteRect;
    
    if ( VScrollBar->isVisible() )
        ClientArea.LowerRightCorner.X -= VScrollBar->getRelativePosition().getWidth();
    
    if ( HScrollBar->isVisible() )
        ClientArea.LowerRightCorner.Y -= HScrollBar->getRelativePosition().getHeight();
    
    if ( Border )
    {
        ClientArea.UpperLeftCorner += core::position2d<s32>( BORDER_WIDTH, BORDER_WIDTH );
        ClientArea.LowerRightCorner -= core::position2d<s32>( BORDER_WIDTH, BORDER_WIDTH );
    }
}

} // namespace gui
} // namespace irr
