// Xterm-in'Hate

//#ifdef _IRR_COMPILE_WITH_GUI_

#include "CGUIWindowManager.h"
#include "IVideoDriver.h"
#include "IGUIEnvironment.h"

namespace irr
{
namespace gui
{

//! constructor
CGUIWindowManager::CGUIWindowManager(IGUIEnvironment* environment, IGUIElement* parent, ICursorControl* cursor)
: IGUIElement(EGUIET_ELEMENT, environment, parent, -1,
    core::rect<s32>(0,0,parent->getAbsolutePosition().getWidth(),parent->getAbsolutePosition().getHeight()))
, isJointVisible( false ), Behavior(EGWMB_RESET), Cursor(cursor),
MinimizedPosition(EGWMMP_BOTTOM), MinimizedDirection(EGWMMD_FROM_TOP_LEFT), MinimizedSize(120),
Skin(0)
{
	#ifdef _DEBUG
	setDebugName("CGUIWindowManager");
	#endif
}

//! destructor
CGUIWindowManager::~CGUIWindowManager()
{
    reset();

	if (Skin)
	{
		Skin->drop();
		Skin = 0;
	}
}

void CGUIWindowManager::setJointVisible(bool visible)
{
    isJointVisible = visible;
}

void CGUIWindowManager::setBehavior(EGUI_WINDOW_MANAGER_BEHAVIOR behavior)
{
    switch(Behavior)
    {
    case EGWMB_RESET:
        {
            if( behavior != EGWMB_RESET )
            {
                // init bones and jonts based on child GUI elements
                init();
                // then change behavior
                Behavior = behavior;
            }
        }
        break;
    case EGWMB_ATTACH_RUN_ONCE:
        {
            if( behavior == EGWMB_RESET )
                reset();
            // change behavior
            tryToAttachAll();
            Behavior = behavior;
        }
        break;
    case EGWMB_ATTACH_FOCUSED:
        {
            if( behavior == EGWMB_RESET )
                reset();
            // change behavior
            Behavior = behavior;
        }
        break;
    case EGWMB_RUN:
        {
            if( behavior == EGWMB_RESET )
                reset();
            // change behavior
            Behavior = behavior;
        }
        break;
    case EGWMB_DETACH_FOCUSED:
        {
            if( behavior == EGWMB_RESET )
                reset();
            // change behavior
            Behavior = behavior;
        }
        break;
    case EGWMB_DETACH_ALL:
        {
            if( behavior == EGWMB_RESET )
                reset();
            // change behavior
            Behavior = behavior;
        }
        break;
    }
}

EGUI_WINDOW_MANAGER_BEHAVIOR CGUIWindowManager::getBehavior() const
{
        return Behavior;
}

void CGUIWindowManager::draw()
{
    // update the size of the grid element (all screen)
    const core::rect<s32>& rect = Parent->getAbsolutePosition();
    DesiredRect = core::rect<s32>(0,0,rect.getWidth(),rect.getHeight());
    recalculateAbsolutePosition(true);
    recalculateMinimizedWindow();

    // apply dynamic grid algorithm
    snap(); // find a better place not to decrease FPS

	if (!IsVisible)
        return;

    video::IVideoDriver * driver = Environment->getVideoDriver();

    if (isJointVisible)
    {
        IGUISkin * skin = Environment->getSkin();
        core::list<bone*>::ConstIterator itb = Bones.begin();
        for (; itb != Bones.end(); ++itb)
        {
            for (size_t index = 0; index != joint::EJT_MAX; ++index)
            {
                const video::SColor color(255,0,255,0);
                const core::rect<s32> drawrect( rect.UpperLeftCorner + (*itb)->getJoint(index)->getPosition() - core::position2di(4,4),
                                                rect.UpperLeftCorner + (*itb)->getJoint(index)->getPosition() + core::position2di(4,4) );
                if ( (*itb)->getJoint(index)->isAttached() )
                {
                    driver->draw2DRectangle(color, drawrect);
                }
                else
                {
                    driver->draw2DRectangleOutline(drawrect,skin->getColor(EGDC_3D_SHADOW));
                }
            }
        }
    }

    driver->draw2DRectangle(Skin->getColor((EGUI_DEFAULT_COLOR)EGAWC_WINDOW_SHADOW), MinimizedBar);

    core::list<CGUIAdvancedWindow*>::ConstIterator itw = Windows.begin();
    for (; itw != Windows.end(); ++itw)
    {
        (*itw)->draw();
    }

	IGUIElement::draw();
}

//! called if an event happened.
bool CGUIWindowManager::OnEvent(const SEvent& event)
{


	return IGUIElement::OnEvent(event);
}

void CGUIWindowManager::OnPostRender(u32 timeMs)
{
    core::list<CGUIAdvancedWindow*>::ConstIterator itw = Windows.begin();
    for (; itw != Windows.end(); ++itw)
    {
        (*itw)->OnPostRender(timeMs);
    }
	IGUIElement::OnPostRender(timeMs);
}

void CGUIWindowManager::setSkin(IGUISkin* skin)
{
    if (Skin==skin)
        return;

    if( Skin )
        Skin->drop();

    Skin = skin;

	if (Skin)
		Skin->grab();

    core::list<CGUIAdvancedWindow*>::ConstIterator itw = Windows.begin();
    for (; itw != Windows.end(); ++itw)
    {
        (*itw)->setSkin(skin);
    }
}

void CGUIWindowManager::setMinimizedPosition(EGUI_WINDOW_MANAGER_MINIMIZED_POSITION position)
{
    MinimizedPosition = position;
    recalculateMinimizedWindow();
}

void CGUIWindowManager::setMinimizedDirection(EGUI_WINDOW_MANAGER_MINIMIZED_DIRECTION direction)
{
    MinimizedDirection = direction;
    recalculateMinimizedWindow();
}

void CGUIWindowManager::setMinimizedSize(u32 size)
{
    MinimizedSize = size;
    recalculateMinimizedWindow();
}

void CGUIWindowManager::init()
{
    reset();

    // create a bone per child element
    core::list<CGUIAdvancedWindow*>::ConstIterator itw = Windows.begin();
    for (; itw != Windows.end(); ++itw)
    {
          Bones.push_back( new bone( (*itw) ) );
    }
}

void CGUIWindowManager::reset()
{
    // delete all bones
    core::list<bone*>::Iterator itb = Bones.begin();
    for (; itb != Bones.end(); ++itb)
    {
        delete (*itb);
    }
    Bones.clear();
}

void CGUIWindowManager::snap()
{
    static u32 counter = 1; ///< 1 : because every bones and joints are initialzed with a null counter.
    switch(Behavior)
    {
    case EGWMB_RESET:
        {

        }
        break;
    case EGWMB_ATTACH_RUN_ONCE:
        {
            tryToAttachAll();
            Behavior = EGWMB_RUN;
        }
        break;
    case EGWMB_ATTACH_FOCUSED:
        {
            IGUIElement* focused = Environment->getFocus();
            if ( focused )
            {
                if (isChildWindow(focused))
                {
                    bone* b;
                    if ( hasBone((CGUIAdvancedWindow*)focused,b) )
                    {
                        b->update(counter);
                        tryToAttach( b );
                        ++counter;
                    }
                }
            }
        }
        break;
    case EGWMB_RUN:
        {
            IGUIElement* focused = Environment->getFocus();
            if ( focused )
            {
                if (isChildWindow(focused))
                {
                    bone* b;
                    if ( hasBone((CGUIAdvancedWindow*)focused,b) )
                    {
                        b->update(counter);
                        ++counter;
                    }
                }
            }
        }
        break;
    case EGWMB_DETACH_FOCUSED:
        {
            IGUIElement* focused = Environment->getFocus();
            if ( focused )
            {
                if (isChildWindow(focused))
                {
                    bone* b;
                    if ( hasBone((CGUIAdvancedWindow*)focused,b) )
                    {
                        b->update(counter);
                        b->dettach();
                        ++counter;
                    }
                }
            }
        }
        break;
    case EGWMB_DETACH_ALL:
        {
            dettachAll();
            Behavior = EGWMB_RUN;
        }
        break;
    }
}

void CGUIWindowManager::tryToAttach(bone* b)
{
    core::list<bone*>::ConstIterator itb = Bones.begin();
    for (; itb != Bones.end(); ++itb)
    {
        // don't attach to itself
        if ( (*itb) == b || (*itb)->isAttachedToBone(b) )
            continue;

        // a pair of joints...
        size_t j1, j2;
        if (b->canAttachToBone((*itb),j1,j2 ))
        {
            if( ! b->isRectCollidedBody( (*itb)))
            {
                b->attachToBone((*itb),j1,j2);
                b->consolidate();
            }
        }
    }
}

void CGUIWindowManager::tryToAttachAll()
{
    core::list<bone*>::ConstIterator itb = Bones.begin();
    for (; itb != Bones.end(); ++itb)
    {
        tryToAttach( (*itb) );
    }
}

void CGUIWindowManager::dettachAll()
{
    core::list<bone*>::ConstIterator itb = Bones.begin();
    for (; itb != Bones.end(); ++itb)
    {
        (*itb)->dettach();
    }
}

CGUIWindowManager::bone* CGUIWindowManager::getBone(CGUIAdvancedWindow* window)
{
    core::list<bone*>::ConstIterator itb = Bones.begin();
    for (; itb != Bones.end(); ++itb)
    {
        if ( (*itb)->own(window) )
            return (*itb);
    }
    return 0;
}

bool CGUIWindowManager::hasBone(const CGUIAdvancedWindow* window, bone*& b) const
{
    core::list<bone*>::ConstIterator itb = Bones.begin();
    for (; itb != Bones.end(); ++itb)
    {
        if ( (*itb)->own(window) )
        {
            b = (*itb);
            return true;
        }
    }
    return false;
}

CGUIAdvancedWindow* CGUIWindowManager::addWindow( core::rect<s32> rectangle, const wchar_t *text, const core::array<CGUIAdvancedWindow::buttoninfo>& buttons, EWINDOW_CLOSE close, video::ITexture* texture)
{
    CGUIAdvancedWindow* aw = new CGUIAdvancedWindow( Environment, this, -1, rectangle, text, buttons, close, core::position2di(0,0), Cursor, texture);
    Windows.push_back( aw );
    Bones.push_back( new bone(aw) );
    recalculateMinimizedWindow();
    if( Skin )
        aw->setSkin(Skin);
    return aw;
}

void CGUIWindowManager::recalculateMinimizedWindow()
{
    u32 counter = 0;
    core::position2di origin;
    core::position2di move;
    switch(MinimizedPosition)
    {
    case EGWMMP_TOP:
        origin = core::position2di(MinimizedDirection==EGWMMD_FROM_TOP_LEFT?0:Parent->getAbsolutePosition().getWidth()-MinimizedSize,0);
        move=core::position2di(MinimizedDirection==EGWMMD_FROM_TOP_LEFT?MinimizedSize:-MinimizedSize,0);
        MinimizedBar = core::rect<s32>(0,0,Parent->getAbsolutePosition().getWidth(),21);
        break;
    case EGWMMP_BOTTOM:
        origin = core::position2di(MinimizedDirection==EGWMMD_FROM_TOP_LEFT?0:Parent->getAbsolutePosition().getWidth()-MinimizedSize,Parent->getAbsolutePosition().getHeight()-21);
        move=core::position2di(MinimizedDirection==EGWMMD_FROM_TOP_LEFT?MinimizedSize:-MinimizedSize,0);
        MinimizedBar = core::rect<s32>(0,Parent->getAbsolutePosition().getHeight()-21,Parent->getAbsolutePosition().getWidth(),Parent->getAbsolutePosition().getHeight());
        break;
    case EGWMMP_RIGHT:
        origin = core::position2di(Parent->getAbsolutePosition().getWidth()-MinimizedSize,MinimizedDirection==EGWMMD_FROM_TOP_LEFT?0:Parent->getAbsolutePosition().getHeight()-21);
        move=core::position2di(0,MinimizedDirection==EGWMMD_FROM_TOP_LEFT?21:-21);
        MinimizedBar = core::rect<s32>(Parent->getAbsolutePosition().getWidth()-MinimizedSize,0,Parent->getAbsolutePosition().getWidth(),Parent->getAbsolutePosition().getHeight());
        break;
    case EGWMMP_LEFT:
        origin = core::position2di(0,MinimizedDirection==EGWMMD_FROM_TOP_LEFT?0:Parent->getAbsolutePosition().getHeight()-21);
        move=core::position2di(0,MinimizedDirection==EGWMMD_FROM_TOP_LEFT?0:-21);
        MinimizedBar = core::rect<s32>(0,0,MinimizedSize,Parent->getAbsolutePosition().getHeight());
        break;
    }
    core::list<CGUIAdvancedWindow*>::ConstIterator itw = Windows.begin();
    for (; itw != Windows.end(); ++itw)
    {
        (*itw)->setMinimizedPosition(origin+move*counter);
        (*itw)->setMinimizedWidth(MinimizedSize);
        if( (*itw)->isMinimized() )
            (*itw)->setRelativePosition(origin+move*counter);
        ++counter;
    }
}

bool CGUIWindowManager::isChildWindow( IGUIElement* element) const
{
    core::list<CGUIAdvancedWindow*>::ConstIterator itw = Windows.begin();
    for (; itw != Windows.end(); ++itw)
    {
        if (element == (*itw))
            return true;
    }
    return false;
}

void CGUIWindowManager::minimizeAll()
{
    core::list<CGUIAdvancedWindow*>::Iterator itw = Windows.begin();
    for (; itw != Windows.end(); ++itw)
    {
        (*itw)->setMinimized(true);
    }
}

void CGUIWindowManager::restoreAll()
{
    core::list<CGUIAdvancedWindow*>::Iterator itw = Windows.begin();
    for (; itw != Windows.end(); ++itw)
    {
        (*itw)->setMinimized(false);
    }
}

/// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CGUIWindowManager::bone::bone(CGUIAdvancedWindow* window) :
Window(window), Counter(0)
{
    // create 8 joints per element (two at each corner)
    const core::rect<s32>& rect = Window->getRelativePosition();
    for (size_t index = 0; index != joint::EJT_MAX; ++index)
    {
        Joints[index] = new joint( (joint::EJOINT_TYPE)index, this, rect );
    }
}

CGUIWindowManager::bone::~bone()
{
    // delete all joints
    for (size_t index = 0; index != joint::EJT_MAX; ++index)
    {
        delete Joints[index];
    }
}

CGUIWindowManager::joint* CGUIWindowManager::bone::getJoint(size_t index) const
{
    return Joints[index];
}

bool CGUIWindowManager::bone::own(const CGUIAdvancedWindow* window) const
{
    return Window == window;
}

bool CGUIWindowManager::bone::canAttachToBone(const bone* b, size_t& j1, size_t& j2) const
{
    for (size_t index1=0; index1 != joint::EJT_MAX; ++index1)
    {
        if ( ! Joints[index1]->isAttached() )
        {
            for (size_t index2 = 0; index2 != joint::EJT_MAX; ++index2)
            {
                if (Joints[index1]->canAttachToJoint(b->getJoint(index2)))
                {
                    j1 = index1;
                    j2 = index2;
                    return true;
                }
            }
        }
    };
    return false;
}

void CGUIWindowManager::bone::attachToBone(bone* b, size_t j1, size_t j2)
{
    Joints[j1]->attachToJoint(b->getJoint(j2));
    Window->setMinimizable(false);
}


void CGUIWindowManager::bone::dettach()
{
    for (size_t index = 0; index != joint::EJT_MAX; ++index)
    {
        Joints[index]->dettach();
    }
    Window->setMinimizable(true);
}

void CGUIWindowManager::bone::updateMinimizable()
{
    Window->setMinimizable(true);
    for (size_t index = 0; index != joint::EJT_MAX; ++index)
    {
        if ( Joints[index]->isAttached() )
        {
            Window->setMinimizable(false);
            break;
        }
    }
}

bool CGUIWindowManager::bone::isAttachedToBone(const bone* b) const
{
    for (size_t index = 0; index != joint::EJT_MAX; ++index)
    {
        if (Joints[index]->isAttachedToBone(b))
            return true;
    }
    return false;
}

// I have to do it recursively
bool CGUIWindowManager::bone::isRectCollidedBodyRecursive(const CGUIWindowManager::bone* b, const core::rect<s32>& rect, core::list<const bone*>& body_bones_processed)
{
    // check current bone
    if ( b->getWindow()->getRelativePosition().isRectCollided( rect ) || b->getWindow()->getRelativePosition() == rect )
        return true;
    // dont process twice this bone
    body_bones_processed.push_back(b);
    // process every attached bone, except those already processed
    for (size_t index = 0; index != joint::EJT_MAX; ++index)
    {
        if( b->getJoint(index)->isAttached() )
        {
            const bone* next = b->getJoint(index)->getAttachedBone();
            if( core::find( body_bones_processed.begin(), body_bones_processed.end(), next) == body_bones_processed.end() )
            {
                if ( isRectCollidedBodyRecursive( next, rect, body_bones_processed  ) )
                {
                    return true;
                }
            }

        }
    }
    return false;
}

void CGUIWindowManager::bone::listBonesRecursive(const bone* b,core::list<const bone*>& body_bones)
{
    if( core::find( body_bones.begin(), body_bones.end(), b) == body_bones.end() )
    {
        body_bones.push_back(b);
        for (size_t index = 0; index != joint::EJT_MAX; ++index)
        {
            if( b->getJoint(index)->isAttached() )
            {
                const bone* next = b->getJoint(index)->getAttachedBone();
                listBonesRecursive(next,body_bones);
            }
        }
    }
}

void CGUIWindowManager::bone::listBonesRecursive(bone* b,core::list<bone*>& body_bones)
{
    if( core::find( body_bones.begin(), body_bones.end(), b) == body_bones.end() )
    {
        body_bones.push_back(b);
        for (size_t index = 0; index != joint::EJT_MAX; ++index)
        {
            if( b->getJoint(index)->isAttached() )
            {
                bone* next = b->getJoint(index)->getAttachedBone();
                listBonesRecursive(next,body_bones);
            }
        }
    }
}

bool CGUIWindowManager::bone::isRectCollidedBody(const bone* b) const
{
    core::list<const bone*> body_bones;
    listBonesRecursive(this,body_bones);
    core::list<const bone*>::ConstIterator itb = body_bones.begin();
    for (; itb != body_bones.end(); ++itb)
    {
        core::rect<s32> rect = (*itb)->getWindow()->getRelativePosition();
        core::list<const bone*> body_bones_processed;
        if( (*itb)->isRectCollidedBodyRecursive(b,rect,body_bones_processed) )
            return true; // exit at once if collision detected
    }
    return false; // no collision
}

///! Attach together every bones of the body identified by THIS bone, by as many joints as possible
void CGUIWindowManager::bone::consolidate()
{
    core::list<bone*> body_bones;
    listBonesRecursive(this,body_bones);
    core::list<bone*>::Iterator itb1 = body_bones.begin();
    for (; itb1 != body_bones.end(); ++itb1)
    {
        core::list<bone*>::Iterator itb2 = itb1+1;
        for (; itb2 != body_bones.end(); ++itb2)
        {
            // don't attach to itself
            if ( (*itb1) == (*itb2) )
                continue;
            // a pair of joints...
            (*itb2)->attachToBoneByAllPossibleJoints((*itb1));
        }
    }
}

///! Attach bones THIS and B with as many joint as possible
void CGUIWindowManager::bone::attachToBoneByAllPossibleJoints(bone* b)
{
    for (size_t index1 = 0; index1 != joint::EJT_MAX; ++index1)
    {
        if ( ! Joints[index1]->isAttached() )
        {
            for (size_t index2 = 0; index2 != joint::EJT_MAX; ++index2)
            {
                if( Joints[index1]->canAttachToJoint(b->getJoint(index2) ) )
                {
                    Joints[index1]->attachToJoint(b->getJoint(index2));
                }
            }
        }
    }
}

const CGUIAdvancedWindow* CGUIWindowManager::bone::getWindow() const
{
    return Window;
}

void CGUIWindowManager::bone::update(u32 counter, const core::position2di& dist)
{
    if (Counter != counter)
    {
        Counter = counter;
        Window->move( dist );
        const core::rect<s32>& rect = Window->getRelativePosition();
        for (size_t index = 0; index != joint::EJT_MAX; ++index )
        {
            Joints[index]->update(counter,rect);
        };
    }
}

/// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CGUIWindowManager::joint::joint(EJOINT_TYPE type, bone* parent_bone, const core::rect<s32>& rectangle) :
Type( type ), ParentBone(parent_bone), AttachedJoint(0), Counter(0)
{
    switch(Type)
    {
    case EJT_UPPER_RIGHT_TO_TOP:
    case EJT_UPPER_RIGHT_TO_RIGHT:
        Position = core::position2di( rectangle.LowerRightCorner.X, rectangle.UpperLeftCorner.Y );
        break;
    case EJT_LOWER_RIGHT_TO_RIGHT:
    case EJT_LOWER_RIGHT_TO_BOTTOM:
        Position = rectangle.LowerRightCorner;
        break;
    case EJT_LOWER_LEFT_TO_BOTTOM:
    case EJT_LOWER_LEFT_TO_LEFT:
        Position = core::position2di( rectangle.UpperLeftCorner.X, rectangle.LowerRightCorner.Y );
        break;
    case EJT_UPPER_LEFT_TO_LEFT:
    case EJT_UPPER_LEFT_TO_TOP:
        Position = rectangle.UpperLeftCorner;
        break;
    default:
        break;
    }
}

CGUIWindowManager::joint::~joint()
{
    // detach joint
    if ( AttachedJoint != 0 )
        AttachedJoint->AttachedJoint = 0;
}

const core::position2di & CGUIWindowManager::joint::getPosition() const
{
    return Position;
}

CGUIWindowManager::joint::EJOINT_TYPE CGUIWindowManager::joint::getType() const
{
    return Type;
}

bool CGUIWindowManager::joint::isAttached() const
{
    return AttachedJoint != 0;
}

const bool joint_compatibility[8][8] =
{
    { false, false, false,  true, false, false, false, false }, // EJT_UPPER_RIGHT_TO_TOP
    { false, false, false, false, false, false,  true, false }, // EJT_UPPER_RIGHT_TO_RIGHT
    { false, false, false, false, false,  true, false, false }, // EJT_LOWER_RIGHT_TO_RIGHT
    {  true, false, false, false, false, false, false, false }, // EJT_LOWER_RIGHT_TO_BOTTOM
    { false, false, false, false, false, false, false,  true }, // EJT_LOWER_LEFT_TO_BOTTOM
    { false, false,  true, false, false, false, false, false }, // EJT_LOWER_LEFT_TO_LEFT
    { false,  true, false, false, false, false, false, false }, // EJT_UPPER_LEFT_TO_LEFT
    { false, false, false, false,  true, false, false, false }  // EJT_UPPER_LEFT_TO_TOP
};

bool CGUIWindowManager::joint::canAttachToJoint(const joint* j) const
{
    return joint_compatibility[Type][j->getType()] && ( Position.getDistanceFrom( j->getPosition() ) < 4 ); ///< Distance between two 'attachable' joint
}

void CGUIWindowManager::joint::attachToJoint(joint* j)
{
    AttachedJoint = j;
    j->AttachedJoint = this;
    j->ParentBone->updateMinimizable();
    ParentBone->updateMinimizable();
}

void CGUIWindowManager::joint::dettach()
{
    if( isAttached() )
    {
        // mutually detach joints
        AttachedJoint->AttachedJoint = 0;
        AttachedJoint->ParentBone->updateMinimizable();
        AttachedJoint = 0;
        ParentBone->updateMinimizable();
    }
}

bool CGUIWindowManager::joint::isAttachedToBone(const bone* b) const
{
    if( !isAttached() )
        return false;

    return  AttachedJoint->ParentBone == b;
}

CGUIWindowManager::bone* CGUIWindowManager::joint::getAttachedBone() const
{
    return AttachedJoint->ParentBone;
}

void CGUIWindowManager::joint::update(u32 counter, const core::rect<s32>& rectangle)
{
    if ( Counter != counter )
    {
        Counter = counter;
        switch(Type)
        {
        case EJT_UPPER_RIGHT_TO_TOP:
        case EJT_UPPER_RIGHT_TO_RIGHT:
            Position = core::position2di( rectangle.LowerRightCorner.X, rectangle.UpperLeftCorner.Y );
            break;
        case EJT_LOWER_RIGHT_TO_RIGHT:
        case EJT_LOWER_RIGHT_TO_BOTTOM:
            Position = rectangle.LowerRightCorner;
            break;
        case EJT_LOWER_LEFT_TO_BOTTOM:
        case EJT_LOWER_LEFT_TO_LEFT:
            Position = core::position2di( rectangle.UpperLeftCorner.X, rectangle.LowerRightCorner.Y );
            break;
        case EJT_UPPER_LEFT_TO_LEFT:
        case EJT_UPPER_LEFT_TO_TOP:
            Position = rectangle.UpperLeftCorner;
            break;
        default:
            break;
        }
        if ( AttachedJoint != 0 )
        {
            AttachedJoint->dragTo(counter,Position);
        }
    }
}

void CGUIWindowManager::joint::dragTo(u32 counter, const core::position2di& position)
{
    if (Counter != counter)
    {
        Counter = counter;
        const core::vector2di dist(position-Position);
        Position = position;
        ParentBone->update(counter,dist);
    }
}

// xterminahte > if you have understood this code until this point, please, phone me and tell me how it is possible this is working not too bad ? ;-)

} // end namespace gui
} // end namespace irr

//#endif // _IRR_COMPILE_WITH_GUI_

