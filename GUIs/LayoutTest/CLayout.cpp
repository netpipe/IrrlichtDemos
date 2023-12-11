//! ----------------------------------------------------------------------------------
//! CLayout class version 1.2
//! Helper class to ease positioning and sizeing the GUI elements
//! Created by Peter Szollosi, 2009.
//! ----------------------------------------------------------------------------------

#include <irrlicht.h>
using namespace irr;

#include "CLayout.h"


//! constructor
CLayout::CLayout(int ID, ELayoutDirection Direction, const core::dimension2di& Border, bool StretchH, bool StretchV, ELayoutAlignment Align)
{
    id = ID;
    direction = Direction;
    border = Border;
    curSize = minSize = border;
    stretchH = StretchH;
    stretchV = StretchV;
    align = Align;
    type = ELT_Container;
    enabled = true;
    changed = true;
    parent = first = last = prev = next = NULL;
    count = 0;
}

//! removeChildren
void CLayout::removeChildren()
{
    // for all the children, if there are
    CLayout* child = first, *childNext = NULL;

    first = last = NULL;

    while (child)
    {
        childNext = child->next;
        delete child;
        child = childNext;
    }

    count = 0;
}

//! removeChild
void CLayout::removeChild(int ID)
{
    CLayout* child = findChild(ID);
    if (!child) return; // can't find it

    CLayout* childParent = child->parent;
    if (!childParent) return;

    if (this != childParent) childParent->removeChild(ID);

    child->removeChildren(); // remove all the children of child

    CLayout* childPrev = child->prev;
    CLayout* childNext = child->next;

    if (!childPrev && !childNext)
    {
        first = last = NULL;
    }
    else
    if (!childPrev && childNext)
    {
        first = childNext;
        childNext->prev = NULL;
    }
    else
    if (childPrev && !childNext)
    {
        last = childPrev;
        childPrev->next = NULL;
    }
    else
    if (childPrev && childNext)
    {
        child->prev->next = childNext;
        child->next->prev = childPrev;
    }

    delete child;
    --count;
}

//! calcAbsPos
const core::position2di CLayout::calcAbsPos()
{
    // if not enabled, zero
    if (!enabled) return core::position2di();

    // start from relPos
    core::dimension2di b = (type == ELT_Slot) ? border : core::dimension2di();
    core::position2di absPos = relPos + b;

    // we need all the absolute positions
    if (parent) absPos += parent->calcAbsPos();

    return absPos;
}

//! calcAbsRect
const core::rect<s32> CLayout::calcAbsRect()
{
    // if not enabled, zero
    if (!enabled) return core::rect<s32>();

    core::position2di absPos = calcAbsPos();
    core::dimension2di b = (type == ELT_Slot) ? border : core::dimension2di();

    return core::rect<s32>(absPos.X, absPos.Y, absPos.X + curSize.Width - b.Width, absPos.Y + curSize.Height - b.Height);
}

//! changeEnabled
void CLayout::changeEnabled(bool Enabled)
{
    if (parent && !parent->enabled) enabled = false;
    else enabled = Enabled;

    // for all the children, if there are
    CLayout* child = first;

    while (child)
    {
        child->changeEnabled(Enabled);

        child = child->next;
    }
}

//! changeBorder
void CLayout::changeBorder(const core::dimension2di& Border)
{
    // only for containers
    if (type != ELT_Container) return;

    border = Border;
    curSize = minSize = border;

    // for all the slots of this container
    CLayout* child = first;

    while (child)
    {
        // for slots only
        if (child->type == ELT_Slot)
        {
            // delete previous border
            child->minSize.Width = child->minSize.Width - child->border.Width;
            child->minSize.Height = child->minSize.Height - child->border.Height;
            // add new border
            child->border = border;
            child->curSize = child->minSize = child->minSize + child->border;
        }

        child = child->next;
    }
}

//! addSlot
void CLayout::addSlot(int ID, const core::dimension2di& MinSize, bool StretchH, bool StretchV, ELayoutAlignment Align)
{
    // only containers can have children
    if (type != ELT_Container) return;

    // create new slot
    CLayout* child = new CLayout(ID, ELD_Unknown, border, StretchH, StretchV, Align);
    child->type = ELT_Slot;
    child->minSize = child->curSize = MinSize + border;
    addChild(child);
}

//! addSpace
void CLayout::addSpace(int MinSize, bool Stretch)
{
    // only containers can have children
    if (type != ELT_Container) return;

    // create new space
    CLayout* child = new CLayout(-1, ELD_Unknown, core::dimension2di(), Stretch, Stretch, ELA_UpperLeft);
    child->type = ELT_Space;
    child->minSize = child->curSize = core::dimension2di(MinSize, MinSize);
    addChild(child);
}

//! addChild
void CLayout::addChild(CLayout* Other)
{
    // only containers can have children
    if (type != ELT_Container) return;

    if (!Other) return;

    // add to this layout
    Other->parent = this;
    Other->changeEnabled(enabled);

    if (!first) // if there's no children yet
    {
        first = last = Other;
    }
    else // if there are children
    {
        Other->prev = last;
        last->next = Other;
        last = Other;
    }

    ++count;
}

//! findChild
CLayout* CLayout::findChild(int ID)
{
    // if ID is corrupt
    if (ID < 0) return NULL;

    // for all the children, if there are
    CLayout* child = first, *found = NULL;

    while (child)
    {
        // it's my child
        if (child->id == ID) return child;

        // it's my child's element
        if (found = child->findChild(ID)) return found;

        child = child->next;
    }

    return NULL;
}

//! update
void CLayout::update(IrrlichtDevice* Device, bool SetDesiredSize, const core::dimension2du& DesiredSize)
{
    // start from the root
    CLayout* root = findRoot();
    if (this != root) root->update(Device, SetDesiredSize, DesiredSize);

    core::dimension2di prevSize = curSize;

    // calculate minimum sizes
    setChanged();
    CLayout* child = NULL;
    while (child = findLowestChanged(false)) // if it's NULL, we're done
    {
        child->calcMinSize();
        child->changed = false;
    }

    // set desired size
    core::dimension2di maxSize = curSize;

    if (!SetDesiredSize)
    {
        maxSize.Width = prevSize.Width < curSize.Width ? curSize.Width : prevSize.Width;
        maxSize.Height = prevSize.Height < curSize.Height ? curSize.Height : prevSize.Height;
    }
    else
    {
        if (stretchH) maxSize.Width = DesiredSize.Width < maxSize.Width ? maxSize.Width : DesiredSize.Width;
        if (stretchV) maxSize.Height = DesiredSize.Height < maxSize.Height ? maxSize.Height : DesiredSize.Height;
    }

    // calculate maximum sizes
    calcChildrenMaxSize(maxSize);

    // calculate relative positions
    setChanged();
    while (child = findLowestChanged(true)) // if it's NULL, we're done
    {
        if (child->parent) child->parent->calcChildRelPos(child);
        child->changed = false;
    }

    // update binded GUIElements
    if (Device)
    {
        gui::IGUIElement* rootGUI = Device->getGUIEnvironment()->getRootGUIElement();
        if (rootGUI) updateBindedGUIElements(rootGUI);
    }
}

//! save
bool CLayout::save(IrrlichtDevice* Device, const core::stringc& FileName)
{
    // start from the root
    CLayout* root = findRoot();
    if (this != root) root->save(Device, FileName);

    if (!Device) return false;

    io::IFileSystem* fileSystem = Device->getFileSystem();
    if (!fileSystem) return false;

    io::IXMLWriter* XMLWriter = fileSystem->createXMLWriter(FileName.c_str());
    if (!XMLWriter) return false;

    // write out the system
    XMLWriter->writeXMLHeader();

    XMLWriter->writeElement(L"layout", false);
    XMLWriter->writeLineBreak();
    writeAttribs(XMLWriter);
    XMLWriter->writeClosingTag(L"layout");
    XMLWriter->writeLineBreak();

    // close the writer
    XMLWriter->drop();

    return true;
}

//! load
bool CLayout::load(IrrlichtDevice* Device, const core::stringc& FileName)
{
    // start from the root
    CLayout* root = findRoot();
    if (this != root) root->load(Device, FileName);

    if (!Device) return false;

    io::IFileSystem* fileSystem = Device->getFileSystem();
    if (!fileSystem) return false;

    io::IXMLReader* XMLReader = fileSystem->createXMLReader(FileName.c_str());
    if (!XMLReader) return false;

    // create the layout-system
    CLayout* curParent = NULL;
    bool canRead = false;

    while (XMLReader && XMLReader->read())
    {
        switch (XMLReader->getNodeType())
        {
            case io::EXN_ELEMENT:
            {
                // start reading if the header was found
                if (core::stringw("layout") == XMLReader->getNodeName()) canRead = true;

                // it's not the layout data yet
                if (!canRead) break;

                readAttribs(XMLReader, curParent);
            }
            break;
            case io::EXN_ELEMENT_END:
            {
                // it's not the layout data yet
                if (!canRead) break;

                // stop reading if the footer was found
                if (core::stringw("layout") == XMLReader->getNodeName()) { canRead = false; break; }

                if (core::stringw("container") == XMLReader->getNodeName())
                {
                    // step back a parent (if the current parent is not the root)
                    if (curParent && curParent->parent) curParent = curParent->parent;
                }
            }
            break;
            default: break;
        }
    }

    // close the reader
    XMLReader->drop();

    // update the system
    update(NULL, true);

    return true;
}

//! writeAttribs
void CLayout::writeAttribs(io::IXMLWriter* XMLWriter)
{
    if (!XMLWriter) return;

    core::stringw writeType;
    core::array<core::stringw> attribs, values;

    if (type == ELT_Unknown) return; // if type is corrupt, quit
    if (type == ELT_Slot) writeType = L"slot";
    if (type == ELT_Space ) writeType = L"space";
    if (type == ELT_Container) writeType = L"container";

    if (type != ELT_Space)
    {
        attribs.push_back(L"id"); values.push_back(core::stringw(id).c_str());
        attribs.push_back(L"stretchH"); values.push_back(core::stringw(stretchH));
        attribs.push_back(L"stretchV"); values.push_back(core::stringw(stretchV));
        attribs.push_back(L"align"); values.push_back(core::stringw(align));
    }
    if (type == ELT_Slot)
    {
        attribs.push_back(L"minSizeW"); values.push_back(core::stringw(minSize.Width - border.Width));
        attribs.push_back(L"minSizeH"); values.push_back(core::stringw(minSize.Height - border.Height));
    }
    if (type == ELT_Space)
    {
        attribs.push_back(L"stretch"); values.push_back(core::stringw(stretchH));
        attribs.push_back(L"size"); values.push_back(core::stringw(minSize.Width));
    }
    if (type == ELT_Container)
    {
        attribs.push_back(L"direction"); values.push_back(core::stringw(direction));
        attribs.push_back(L"borderW"); values.push_back(core::stringw(border.Width));
        attribs.push_back(L"borderH"); values.push_back(core::stringw(border.Height));
    }

    if (type != ELT_Container)
    {
        XMLWriter->writeElement(writeType.c_str(), true, attribs, values);
        XMLWriter->writeLineBreak();
        return; // if it's not a container, quit
    }

    // if it's a collector
    XMLWriter->writeElement(writeType.c_str(), false, attribs, values);
    XMLWriter->writeLineBreak();

    // for all the children, if there are
    CLayout* child = first;

    while (child)
    {
        child->writeAttribs(XMLWriter);

        child = child->next;
    }

    // close this one
    XMLWriter->writeClosingTag(writeType.c_str());
    XMLWriter->writeLineBreak();
}

//! readAttribs
void CLayout::readAttribs(io::IXMLReader* XMLReader, CLayout* &CurParent)
{
    if (!XMLReader) return;

    // default attribute values
    CLayout* child = NULL;
    ELayoutType readType = ELT_Unknown;
    ELayoutDirection readDirection = ELD_Unknown;
    int readID = -1;
    CLayout::ELayoutAlignment readAlign = ELA_UpperLeft;
    core::dimension2di readMinSize, readBorder;
    bool readStretchH = false, readStretchV = false, readStretch = false;
    int readSize = 0;

    // read the type
    if (core::stringw("slot") == XMLReader->getNodeName()) readType = ELT_Slot;
    else if (core::stringw("space") == XMLReader->getNodeName()) readType = ELT_Space;
    else if (core::stringw("container") == XMLReader->getNodeName()) readType = ELT_Container;

    // if the type is corrupt, quit
    if (readType == ELT_Unknown) return;

    // read the data
    readID = XMLReader->getAttributeValueAsInt(L"id");
    readAlign = (ELayoutAlignment)XMLReader->getAttributeValueAsInt(L"align");
    readMinSize = core::dimension2di(XMLReader->getAttributeValueAsInt(L"minSizeW"), XMLReader->getAttributeValueAsInt(L"minSizeH"));
    readSize = XMLReader->getAttributeValueAsInt(L"size");
    readStretchH = XMLReader->getAttributeValueAsInt(L"stretchH") ? true : false;
    readStretchV = XMLReader->getAttributeValueAsInt(L"stretchV") ? true : false;
    readStretch = XMLReader->getAttributeValueAsInt(L"stretch") ? true : false;
    readDirection = (ELayoutDirection)XMLReader->getAttributeValueAsInt(L"direction");
    readBorder = core::dimension2di(XMLReader->getAttributeValueAsInt(L"borderW"), XMLReader->getAttributeValueAsInt(L"borderH"));

    // create or set the element
    switch (readType)
    {
        case ELT_Slot: if (CurParent) CurParent->addSlot(readID, readMinSize, readStretchH, readStretchV, readAlign);
        break;
        case ELT_Space: if (CurParent) CurParent->addSpace(readSize, readStretch);
        break;
        case ELT_Container: // a container can be the root
        {
            if (!CurParent) // the first one is the root
            {
                id = readID;
                direction = readDirection;
                border = readBorder;
                stretchH = readStretchH;
                stretchV = readStretchV;
                align = readAlign;
                type = ELT_Container;
                changed = true;
                CurParent = this; // "this" is the new parent
            }
            else // create element
            {
                child = new CLayout(readID, readDirection, readBorder, readStretchH, readStretchV, readAlign);
                CurParent->addChild(child);
                CurParent = child; // "child" is the new parent
            }
        }
        break;
        default: break;
    }
}

//! setChanged
void CLayout::setChanged()
{
    changed = true;

    // for all the children, if there are
    CLayout* child = first;

    while (child)
    {
        child->setChanged();

        child = child->next;
    }
}

//! findRoot
CLayout* CLayout::findRoot()
{
    // no parent, this is the root
    if (!parent) return this;

    return parent->findRoot();
}

//! findLowestChanged
CLayout* CLayout::findLowestChanged(bool FindFirst)
{
    if (!changed) return NULL; // if it's done, NULL

    // for all the children, if there are
    CLayout* child = first, *lowest = this, *found = NULL;

    while (child)
    {
        // only if it's not NULL
        if (found = child->findLowestChanged(FindFirst))
        {
            lowest = found;

            if (FindFirst) return lowest;
        }

        child = child->next;
    }

    return lowest;
}

//! calcMinSize
void CLayout::calcMinSize()
{
    if (type == ELT_Slot)
    {
        curSize = core::dimension2di();

        if (!enabled) return;

        curSize = minSize;
    }
    else
    if (type == ELT_Space)
    {
        curSize = core::dimension2di();

        if (!enabled) return;

        // there should be a parent!
        if (parent && parent->type == ELT_Container)
        {
            if (parent->direction == ELD_Horizontal) curSize.Width = minSize.Width;
            if (parent->direction == ELD_Vertical) curSize.Height = minSize.Height;
        }
    }
    else
    if (type == ELT_Container)
    {
        curSize = minSize = core::dimension2di();

        if (!enabled || !first) return;

        curSize = minSize = border;

        // for all the children, if there are
        CLayout* child = first;

        while (child)
        {
            // expand with the child's minSize
            if (direction == ELD_Horizontal)
            {
                minSize.Width += child->minSize.Width;
                minSize.Height = (minSize.Height < child->minSize.Height + border.Height) ? child->minSize.Height + border.Height : minSize.Height;
            }
            else
            if (direction == ELD_Vertical)
            {
                minSize.Width = (minSize.Width < child->minSize.Width + border.Width) ? child->minSize.Width + border.Width : minSize.Width;
                minSize.Height += child->minSize.Height;
            }

            child = child->next;
        }

        curSize = minSize;
    }
}

//! calcChildrenMaxSize
void CLayout::calcChildrenMaxSize(const core::dimension2di& MaxSize)
{
    if (!enabled) return;

    curSize = MaxSize;

    // only containers can have children
    if (type != ELT_Container) return;

    // if there are no children
    if (!first) return;

    int diff = 0;
    if (direction == ELD_Horizontal) diff = curSize.Width - minSize.Width;
    if (direction == ELD_Vertical) diff = curSize.Height - minSize.Height;

    // among how many children do we have to share the difference?
    CLayout* child = first;
    int stretchers = 0;

    while (child)
    {
        if (child->enabled)
        {
            if (direction == ELD_Horizontal && child->stretchH) ++stretchers;
            if (direction == ELD_Vertical && child->stretchV) ++stretchers;
        }

        child = child->next;
    }

    int add = (stretchers > 0) ? (diff / stretchers) : 0;

    // for all the children
    child = first;

    while (child)
    {
        if (child->enabled)
        {
            core::dimension2di childMaxSize = child->curSize;

            // stretching
            if (direction == ELD_Horizontal)
            {
                if (child->stretchH) childMaxSize.Width = childMaxSize.Width + add;
                if (child->stretchV) childMaxSize.Height = curSize.Height - border.Height;
            }
            if (direction == ELD_Vertical)
            {
                if (child->stretchV) childMaxSize.Height = childMaxSize.Height + add;
                if (child->stretchH) childMaxSize.Width = curSize.Width - border.Width;
            }

            child->calcChildrenMaxSize(childMaxSize);
        }

        child = child->next;
    }
}

//! calcChildRelPos
void CLayout::calcChildRelPos(CLayout* Other)
{
    // only containers can have children
    if (type != ELT_Container) return;

    Other->relPos = core::position2di();

    if (!enabled) return;

    if (Other->prev)
    {
        if (direction == ELD_Horizontal) Other->relPos.X = Other->prev->relPos.X + Other->prev->curSize.Width;
        if (direction == ELD_Vertical) Other->relPos.Y = Other->prev->relPos.Y + Other->prev->curSize.Height;
    }

    // alignment
    if (direction == ELD_Horizontal && !Other->stretchV)
    {
        if (Other->align == ELA_LowerRight) Other->relPos.Y = curSize.Height - border.Height - Other->curSize.Height;
        else
        if (Other->align == ELA_Center) Other->relPos.Y = ((curSize.Height - border.Height) / 2) - (Other->curSize.Height / 2);
    }
    else
    if (direction == ELD_Vertical && !Other->stretchH)
    {
        if (Other->align == ELA_LowerRight) Other->relPos.X = curSize.Width - border.Width - Other->curSize.Width;
        else
        if (Other->align == ELA_Center) Other->relPos.X = ((curSize.Width - border.Width) / 2) - (Other->curSize.Width / 2);
    }
}

//! updateBindedGUIElements
void CLayout::updateBindedGUIElements(gui::IGUIElement* RootGUI)
{
    if (!RootGUI) return;

    // if it has a valid id
    CLayout* root = findRoot();
    if (id > -1)
    {
        gui::IGUIElement* e = RootGUI->getElementFromId(id, true);
        if (e)
        {
            if (this != root) e->setRelativePosition(calcAbsRect());
            else
            {
                core::rect<s32> er = e->getRelativePosition();
                core::rect<s32> ar = calcAbsRect();
                e->setRelativePosition(core::rect<s32>(er.UpperLeftCorner, er.UpperLeftCorner + ar.LowerRightCorner));
            }
        }
    }

    // for all the children, if there are
    CLayout* child = first;

    while (child)
    {
        child->updateBindedGUIElements(RootGUI);

        child = child->next;
    }
}

