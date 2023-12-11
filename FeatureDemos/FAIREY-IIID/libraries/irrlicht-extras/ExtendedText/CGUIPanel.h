#ifndef _C_GUI_PANEL_H_
#define _C_GUI_PANEL_H_

#include <IGUIElement.h>

namespace irr
{
namespace gui
{
    
class IGUIScrollBar;

enum E_SCROLL_BAR_MODE
{
    //! The scrollbar will only show up when needed.
    ESBM_AUTOMATIC = 0,     
    
    //! The scrollbar will never be visible. Use fitChildrenToPanel() to force the children to fit the size of the panel.
    ESBM_ALWAYS_INVISIBLE,
    
    //! The scrollbar will always the visible.
    ESBM_ALWAYS_VISIBLE
};

class CGUIPanel : public IGUIElement
{
public:
    CGUIPanel( IGUIEnvironment* environment, IGUIElement* parent, s32 id, const core::rect<s32>& rectangle,
            bool border=false,
            E_SCROLL_BAR_MODE vMode=ESBM_AUTOMATIC,
            E_SCROLL_BAR_MODE hMode=ESBM_ALWAYS_INVISIBLE );
    
    virtual ~CGUIPanel();
    
    virtual void draw();
    
    bool hasBorder() const;
    void setBorder( bool enabled );
    
    IGUIScrollBar* getVScrollBar() const;
    void setVScrollBar( IGUIScrollBar* scrollBar );
    
    IGUIScrollBar* getHScrollBar() const;
    void setHScrollBar( IGUIScrollBar* scrollBar );
    
    E_SCROLL_BAR_MODE getVScrollBarMode() const;
    void setVScrollBarMode( E_SCROLL_BAR_MODE mode );
    
    E_SCROLL_BAR_MODE getHScrollBarMode() const;
    void setHScrollBarMode( E_SCROLL_BAR_MODE mode );
    
    void fitChildrenToPanel();
    void fitChildrenToPanel( bool horizontal, bool vertical );
    
    virtual bool OnEvent( SEvent event );
    
private:    
    void calculateClientArea();
    
    IGUIScrollBar* VScrollBar;
    IGUIScrollBar* HScrollBar;
    
    E_SCROLL_BAR_MODE VScrollBarMode;
    E_SCROLL_BAR_MODE HScrollBarMode;
    
    s32 CurrentVScroll;
    s32 CurrentHScroll;
    
    core::rect<s32> ClientArea;
    
    bool Border;
};

} // namespace gui
} // namespace irr

#endif
