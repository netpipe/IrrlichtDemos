// Xterm-in'Hate

//#ifdef _IRR_COMPILE_WITH_GUI_

#include "CGUIDynamicGrid.h"
#include "IVideoDriver.h"

namespace irr
{
namespace gui
{

//! constructor
CGUIDynamicGrid::CGUIDynamicGrid(IGUIEnvironment* environment, IGUIElement* parent)
: IGUIDynamicGrid( environment, parent ), isJointVisible( false ), Behavior(EGDGB_RESET)
{
	#ifdef _DEBUG
	setDebugName("CGUIDynamicGrid");
	#endif
}

//! destructor
CGUIDynamicGrid::~CGUIDynamicGrid()
{
    reset();
}

void CGUIDynamicGrid::setJointVisible(bool visible)
{
    isJointVisible = visible;
}

void CGUIDynamicGrid::setBehavior(EGUI_DYNAMIC_GRID_BEHAVIOR behavior)
{
    switch(Behavior)
    {
    case EGDGB_RESET:
        {
            if( behavior != EGDGB_RESET )
            {
                // init bones and jonts based on child GUI elements
                init();
                // then change behavior
                Behavior = behavior;
            }
        }
        break;
    case EGDGB_ATTACH_RUN_ONCE:
        {
            if( behavior == EGDGB_RESET )
                reset();
            // change behavior
            tryToAttachAll();
            Behavior = behavior;
        }
        break;
    case EGDGB_ATTACH_FOCUSED:
        {
            if( behavior == EGDGB_RESET )
                reset();
            // change behavior
            Behavior = behavior;
        }
        break;
    case EGDGB_RUN:
        {
            if( behavior == EGDGB_RESET )
                reset();
            // change behavior
            Behavior = behavior;
        }
        break;
    case EGDGB_DETACH_FOCUSED:
        {
            if( behavior == EGDGB_RESET )
                reset();
            // change behavior
            Behavior = behavior;
        }
        break;
    case EGDGB_DETACH_ALL:
        {
            if( behavior == EGDGB_RESET )
                reset();
            // change behavior
            Behavior = behavior;
        }
        break;
    }
}

EGUI_DYNAMIC_GRID_BEHAVIOR CGUIDynamicGrid::getBehavior() const
{
        return Behavior;
}

void CGUIDynamicGrid::draw()
{
    // update the size of the grid element (all screen)
    const core::rect<s32>& rect = Parent->getAbsolutePosition();
    DesiredRect = core::rect<s32>(0,0,rect.getWidth(),rect.getHeight());
    recalculateAbsolutePosition(true);

    // apply dynamic grid algorithm
    snap(); // find a better place not to decrease FPS

	if (!IsVisible)
        return;

    if (isJointVisible)
    {
        video::IVideoDriver * driver = Environment->getVideoDriver();
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

	IGUIElement::draw();
}

void CGUIDynamicGrid::init()
{
    reset();

    // create a bone per child element
    core::list<IGUIElement*>::ConstIterator ite = Children.begin();
    for (; ite != Children.end(); ++ite)
    {
          Bones.push_back( new bone( (*ite) ) );
    }
}

void CGUIDynamicGrid::reset()
{
    // delete all bones
    core::list<bone*>::Iterator itb = Bones.begin();
    for (; itb != Bones.end(); ++itb)
    {
        delete (*itb);
    }
    Bones.clear();
}

void CGUIDynamicGrid::snap()
{
    static u32 counter = 1; ///< 1 : because every bones and joints are initialzed with a null counter.
    switch(Behavior)
    {
    case EGDGB_RESET:
        {

        }
        break;
    case EGDGB_ATTACH_RUN_ONCE:
        {
            tryToAttachAll();
            Behavior = EGDGB_RUN;
        }
        break;
    case EGDGB_ATTACH_FOCUSED:
        {
            IGUIElement* focused = Environment->getFocus();
            if ( focused )
            {
                bone* b;
                if ( hasBone(focused,b) )
                {
                    b->update(counter);
                    tryToAttach( b );
                    ++counter;
                }
            }
        }
        break;
    case EGDGB_RUN:
        {
            IGUIElement* focused = Environment->getFocus();
            if ( focused )
            {
                bone* b;
                if ( hasBone(focused,b) )
                {
                    b->update(counter);
                    ++counter;
                }
            }
        }
        break;
    case EGDGB_DETACH_FOCUSED:
        {
            IGUIElement* focused = Environment->getFocus();
            if ( focused )
            {
                bone* b;
                if ( hasBone(focused,b) )
                {
                    b->update(counter);
                    b->dettach();
                    ++counter;
                }
            }
        }
        break;
    case EGDGB_DETACH_ALL:
        {
            dettachAll();
            Behavior = EGDGB_RUN;
        }
        break;
    }
}

void CGUIDynamicGrid::tryToAttach(bone* b)
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

void CGUIDynamicGrid::tryToAttachAll()
{
    core::list<bone*>::ConstIterator itb = Bones.begin();
    for (; itb != Bones.end(); ++itb)
    {
        tryToAttach( (*itb) );
    }
}

void CGUIDynamicGrid::dettachAll()
{
    core::list<bone*>::ConstIterator itb = Bones.begin();
    for (; itb != Bones.end(); ++itb)
    {
        (*itb)->dettach();
    }
}

CGUIDynamicGrid::bone* CGUIDynamicGrid::getBone(IGUIElement* element)
{
    core::list<bone*>::ConstIterator itb = Bones.begin();
    for (; itb != Bones.end(); ++itb)
    {
        if ( (*itb)->own(element) )
            return (*itb);
    }
    return 0;
}

bool CGUIDynamicGrid::hasBone(const IGUIElement* element, bone*& b) const
{
    core::list<bone*>::ConstIterator itb = Bones.begin();
    for (; itb != Bones.end(); ++itb)
    {
        if ( (*itb)->own(element) )
        {
            b = (*itb);
            return true;
        }
    }
    return false;
}

/// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CGUIDynamicGrid::bone::bone(IGUIElement* element) :
Element(element), Counter(0)
{
    // create 8 joints per element (two at each corner)
    const core::rect<s32>& rect = Element->getRelativePosition();
    for (size_t index = 0; index != joint::EJT_MAX; ++index)
    {
        Joints[index] = new joint( (joint::EJOINT_TYPE)index, this, rect );
    }
}

CGUIDynamicGrid::bone::~bone()
{
    // delete all joints
    for (size_t index = 0; index != joint::EJT_MAX; ++index)
    {
        delete Joints[index];
    }
}

CGUIDynamicGrid::joint* CGUIDynamicGrid::bone::getJoint(size_t index) const
{
    return Joints[index];
}

bool CGUIDynamicGrid::bone::own(const IGUIElement* element) const
{
    return Element == element;
}

bool CGUIDynamicGrid::bone::canAttachToBone(const bone* b, size_t& j1, size_t& j2) const
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

void CGUIDynamicGrid::bone::attachToBone(bone* b, size_t j1, size_t j2)
{
    Joints[j1]->attachToJoint(b->getJoint(j2));
}


void CGUIDynamicGrid::bone::dettach()
{
    for (size_t index = 0; index != joint::EJT_MAX; ++index)
    {
        Joints[index]->dettach();
    }
}

bool CGUIDynamicGrid::bone::isAttachedToBone(const bone* b) const
{
    for (size_t index = 0; index != joint::EJT_MAX; ++index)
    {
        if (Joints[index]->isAttachedToBone(b))
            return true;
    }
    return false;
}

// I have to do it recursively
bool CGUIDynamicGrid::bone::isRectCollidedBodyRecursive(const CGUIDynamicGrid::bone* b, const core::rect<s32>& rect, core::list<const bone*>& body_bones_processed)
{
    // check current bone
    if ( b->getElement()->getRelativePosition().isRectCollided( rect ) || b->getElement()->getRelativePosition() == rect )
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

void CGUIDynamicGrid::bone::listBonesRecursive(const bone* b,core::list<const bone*>& body_bones)
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

void CGUIDynamicGrid::bone::listBonesRecursive(bone* b,core::list<bone*>& body_bones)
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

bool CGUIDynamicGrid::bone::isRectCollidedBody(const bone* b) const
{
    core::list<const bone*> body_bones;
    listBonesRecursive(this,body_bones);
    core::list<const bone*>::ConstIterator itb = body_bones.begin();
    for (; itb != body_bones.end(); ++itb)
    {
        core::rect<s32> rect = (*itb)->getElement()->getRelativePosition();
        core::list<const bone*> body_bones_processed;
        if( (*itb)->isRectCollidedBodyRecursive(b,rect,body_bones_processed) )
            return true; // exit at once if collision detected
    }
    return false; // no collision
}

///! Attach together every bones of the body identified by THIS bone, by as many joints as possible
void CGUIDynamicGrid::bone::consolidate()
{
/// \todo try to attach together each bone of one body, starting from this bone
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
void CGUIDynamicGrid::bone::attachToBoneByAllPossibleJoints(bone* b)
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

const IGUIElement* CGUIDynamicGrid::bone::getElement() const
{
    return Element;
}

void CGUIDynamicGrid::bone::update(u32 counter, const core::position2di& dist)
{
    if (Counter != counter)
    {
        Counter = counter;
        Element->move( dist );
        const core::rect<s32>& rect = Element->getRelativePosition();
        for (size_t index = 0; index != joint::EJT_MAX; ++index )
        {
            Joints[index]->update(counter,rect);
        };
    }
}

/// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CGUIDynamicGrid::joint::joint(EJOINT_TYPE type, bone* parent_bone, const core::rect<s32>& rectangle) :
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
    }
}

CGUIDynamicGrid::joint::~joint()
{
    // detach joint
    if ( AttachedJoint != 0 )
        AttachedJoint->AttachedJoint = 0;
}

const core::position2di & CGUIDynamicGrid::joint::getPosition() const
{
    return Position;
}

CGUIDynamicGrid::joint::EJOINT_TYPE CGUIDynamicGrid::joint::getType() const
{
    return Type;
}

bool CGUIDynamicGrid::joint::isAttached() const
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

bool CGUIDynamicGrid::joint::canAttachToJoint(const joint* j) const
{
    return joint_compatibility[Type][j->getType()] && ( Position.getDistanceFrom( j->getPosition() ) < 4 ); ///< Distance between two 'attachable' joint
}

void CGUIDynamicGrid::joint::attachToJoint(joint* j)
{
    AttachedJoint = j;
    j->AttachedJoint = this;
}

void CGUIDynamicGrid::joint::dettach()
{
    if( isAttached() )
    {
        // mutually detach joints
        AttachedJoint->AttachedJoint = 0;
        AttachedJoint = 0;
    }
}

bool CGUIDynamicGrid::joint::isAttachedToBone(const bone* b) const
{
    if( !isAttached() )
        return false;

    return  AttachedJoint->ParentBone == b;
}

CGUIDynamicGrid::bone* CGUIDynamicGrid::joint::getAttachedBone() const
{
    return AttachedJoint->ParentBone;
}

void CGUIDynamicGrid::joint::update(u32 counter, const core::rect<s32>& rectangle)
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
        }
        if ( AttachedJoint != 0 )
        {
            AttachedJoint->dragTo(counter,Position);
        }
    }
}

void CGUIDynamicGrid::joint::dragTo(u32 counter, const core::position2di& position)
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
