//! ----------------------------------------------------------------------------------
//! CLayout class version 1.2
//! Helper class to ease positioning and sizeing the GUI elements
//! Created by Peter Szollosi, 2009.
//! ----------------------------------------------------------------------------------

#ifndef CLAYOUT_H_INCLUDED
#define CLAYOUT_H_INCLUDED

#include <irrlicht.h>
using namespace irr;


//! CLayout class
//! helper class to ease positioning and sizeing the GUI elements
class CLayout
{
    public:
        //! ELayoutDirection enum
        enum ELayoutDirection { ELD_Unknown = 0, ELD_Horizontal, ELD_Vertical };

        //! ELayoutAlignment enum
        enum ELayoutAlignment { ELA_Unknown = 0, ELA_UpperLeft, ELA_Center, ELA_LowerRight };

        //! ELayoutType enum
        enum ELayoutType { ELT_Unknown = 0, ELT_Slot, ELT_Space, ELT_Container };


        /*! constructor
            \note you can easily bind GUIElements to layout elements by giving the same ID to them
            \param ID: identifier of the container layout
            \param Direction: direction of growth
            \param Border: border size to set for the container and for it's own slots
            \param StretchH: determines if the container should stretch horizontally
            \param StretchV: determines if the container should stretch vertically
            \param Align: sets the alignment of the container inside it's parent layout */
        CLayout(int ID = -1, ELayoutDirection Direction = ELD_Unknown, const core::dimension2di& Border = core::dimension2di(),
                bool StretchH = false, bool StretchV = false, ELayoutAlignment Align = ELA_UpperLeft);

        //! destructor
        ~CLayout() { removeChildren(); }


        //! returns the identifier of this element
        int getID() const { return id; }

        //! returns the number of children
        int getCount() const { return count; }

        //! returns the type of this element
        ELayoutType getType() const { return type; }

        //! returns the direction of this element
        ELayoutDirection getDirection() const { return direction; }

        //! returns if this element is enabled
        bool getEnabled() const { return enabled; }

        //! returns the border size of this element
        const core::dimension2di& getBorder() const { return border; }

        //! returns the alignment of this element inside it's parent
        ELayoutAlignment getAlign() const { return align; }

        //! returns if this element is stretched horizontally
        bool getStretchH() const { return stretchH; }

        //! returns if this element is stretched vertically
        bool getStretchV() const { return stretchV; }


        /*! removes all the children of this element to the bottom (top down)
            \note removes the elements from the list, and deletes them permanently!
            \note this will not remove binded GUIElements! */
        void removeChildren();

        /*! removes a child with the given ID and all it's children to the bottom (top down)
            \note removes the element from the list, and deletes it permanently!
            \note this will not remove binded GUIElements!
            \param ID: identifier of the child element to remove */
        void removeChild(int ID);


        /*! calculates and returns the absolute position of this element
            \return position in absolute (screen) coordinates */
        const core::position2di calcAbsPos();

        /*! calculates and returns the absolute rectangle of this element
            \return rectangle in absolute (screen) coordinates */
        const core::rect<s32> calcAbsRect();


        /*! changes the identifier of this element
            \note you can easily bind GUIElements to layout elements by giving the same ID to them */
        void changeID(int ID) { id = ID; }

        /*! changes the direction of growth of this container
            \note only for containers! */
        void changeDirection(ELayoutDirection Direction)
        { direction = (type == ELT_Container) ? Direction : direction; }

        /*! changes the enabled flag from this element to the bottom (top down)
            \param Enabled: sets if this element is enabled or not */
        void changeEnabled(bool Enabled);

        /*! changes the border of this element and for all it's own slots
            \param Border: size of the border to set */
        void changeBorder(const core::dimension2di& Border);

        /*! changes the alignment of this element inside it's parent layout
            \param Align: alignment type to set */
        void changeAlign(ELayoutAlignment Align) { align = Align; }

        /*! changes the stretch flags of this element
            \param StretchH: determines if the element should stretch horizontally
            \param StretchV: determines if the element should stretch vertically */
        void changeStretch(bool StretchH, bool StretchV)
        { stretchH = StretchH; stretchV = StretchV; }


        /*! creates and adds a new slot to the layout
            \note slot elements cannot have children!
            \note you can easily bind GUIElements to layout elements by giving the same ID to them
            \param ID: identifier of the slot
            \param MinSize: minimum size of of the slot
            \param StretchH: determines if the slot should stretch horizontally
            \param StretchV: determines if the slot should stretch vertically
            \param Align: sets the alignment of the slot inside it's parent layout */
        void addSlot(int ID, const core::dimension2di& MinSize, bool StretchH = false,
                     bool StretchV = false, ELayoutAlignment Align = ELA_UpperLeft);

        /*! creates and adds a new space to the layout
            \note space elements cannot have children!
            \param MinSize: minimum size of of the space
            \param Stretch: determines if the space should stretch */
        void addSpace(int MinSize, bool Stretch = false);

        /*! adds an existing element to the layout
            \note use this function to add other containers.
            \note after adding it, this layout will be the owner!
            \note you shouldn't delete the pointer of the added element!
            \param Other: the other layout that should be added to the list */
        void addChild(CLayout* Other);


        /*! finds a child element by it's ID (top down)
            \param ID: identifier of the child element to find
            \return pointer to the found element, or NULL, if it couln't be found. */
        CLayout* findChild(int ID);


        /*! updates the whole layout-system from the root
            \note binded GUIElements will only update if the Device parameter is specified here!
            \param Device: pointer to the Irrlicht device (only needed if you want the binded elements to update)
            \param SetDesiredSize: true to set the desired size, false to leave the previous size
            \param DesiredSize: the desired size of the layout (zero if you want the minimum size) */
        void update(IrrlichtDevice* Device, bool SetDesiredSize, const core::dimension2du& DesiredSize = core::dimension2du());


        /*! saves the whole layout-system from the root (top down)
            \note the layout-system will only be saved if the Device parameter is specified here!
            \param Device: pointer to the Irrlicht device (needed to use XML functions)
            \param FileName: file to save the layout into
            \return true if succeeded, false if there was an error */
        bool save(IrrlichtDevice* Device, const core::stringc& FileName);

        /*! loads the whole layout-system into the root
            \note it won't delete the previous layout-system!
            \note the layout-system will only be loaded if the Device parameter is specified here!
            \param Device: pointer to the Irrlicht device (needed to use XML functions)
            \param FileName: file to load the layout from
            \return true if succeeded, false if there was an error */
        bool load(IrrlichtDevice* Device, const core::stringc& FileName);


    private:
        //! writes the attributes out from this element to the bottom (top down)
        void writeAttribs(io::IXMLWriter* XMLWriter);

        //! reads the attributes for this element and creates it if needed
        void readAttribs(io::IXMLReader* XMLReader, CLayout* &CurParent);

        //! sets the changed flag from this element to the bottom (top down)
        void setChanged();

        //! finds the root (bottom to top)
        CLayout* findRoot();

        //! finds the lowest element that changed (top down)
        CLayout* findLowestChanged(bool FindFirst);

        //! calculates the minimum size for the layout
        void calcMinSize();

        //! calculates the maximum size for all the child elements (top down)
        void calcChildrenMaxSize(const core::dimension2di& MaxSize);

        //! calculates the relative position for a child element
        void calcChildRelPos(CLayout* Other);

        //! updates the binded GUIElements
        void updateBindedGUIElements(gui::IGUIElement* RootGUI);


        //! variables
        CLayout* parent, *first, *last, *prev, *next;
        ELayoutDirection direction;
        bool changed, enabled, stretchH, stretchV;
        core::position2di relPos;
        core::dimension2di minSize, curSize, border;
        ELayoutAlignment align;
        ELayoutType type;
        int id, count;

}; // end class CLayout


#endif // CLAYOUT_H_INCLUDED
