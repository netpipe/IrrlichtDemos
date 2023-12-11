// Xterm-in'Hate

#ifndef __C_GUI_DYNAMIC_GRID_H_INCLUDED__
#define __C_GUI_DYNAMIC_GRID_H_INCLUDED__

#include "IrrCompileConfig.h"
//#ifdef _IRR_COMPILE_WITH_GUI_

#include "IGUIDynamicGrid.h"
#include "SColor.h"
#include "vector2d.h"

namespace irr
{
namespace core{

    template<class InputIt, class T>
    InputIt find(InputIt first, InputIt last, const T& value)
    {
        for (; first != last; ++first)
        {
            if (*first == value)
            {
                return first;
            }
        }
        return last;
    }
}
namespace gui
{

	class CGUIDynamicGrid : public IGUIDynamicGrid
	{
	public:
		//! constructor
		CGUIDynamicGrid(IGUIEnvironment* environment, IGUIElement* parent);

		//! destructor
		virtual ~CGUIDynamicGrid();

        //! show joints (DYNAMIC GRID DEBUG:To Be Deleted)
		void setJointVisible(bool visible);

        //! set the behavior of the Dynamic Grid
		void setBehavior(EGUI_DYNAMIC_GRID_BEHAVIOR behavior);

        //! get the behavior of the Dynamic Grid
		EGUI_DYNAMIC_GRID_BEHAVIOR getBehavior() const;

		//! draws the element and its children
		virtual void draw();

	private:
        //! Child GUI elements, which are sticked together, form a 'body'.

        //! Each child GUI element of a body is a 'bone'.
        struct bone; // forward decl.

        //! Orphan GUI elements are considered as bodies and bones too.

        //! Each 'bone' has got 8 joints (1 per axis (=2) x 1 per corner (=4)).
        struct joint
        {
            ///! enumaration of all types of joint
            enum EJOINT_TYPE
            {
                EJT_UPPER_RIGHT_TO_TOP=0,
                EJT_UPPER_RIGHT_TO_RIGHT,
                EJT_LOWER_RIGHT_TO_RIGHT,
                EJT_LOWER_RIGHT_TO_BOTTOM,
                EJT_LOWER_LEFT_TO_BOTTOM,
                EJT_LOWER_LEFT_TO_LEFT,
                EJT_UPPER_LEFT_TO_LEFT,
                EJT_UPPER_LEFT_TO_TOP,
                EJT_MAX
            };
            joint(EJOINT_TYPE type, bone* parent_bone, const core::rect<s32>& rectangle);
            ~joint();
            const core::position2di& getPosition() const;
            EJOINT_TYPE getType() const;
            bool isAttached() const;
            bool canAttachToJoint(const joint* j) const;
            void attachToJoint(joint* j);
            void dettach();
            bool isAttachedToBone(const bone* b) const;
            bone* getAttachedBone() const;
            void dragTo(u32 counter, const core::position2di& position);
            void update(u32 counter, const core::rect<s32>& rectangle);
        private:
            EJOINT_TYPE Type;
            bone* ParentBone;
            joint* AttachedJoint;
            core::position2di Position;
            u32 Counter;
        };

        struct bone
        {
            bone(IGUIElement* element);
            ~bone();
            joint* getJoint(size_t index) const;
            bool own(const IGUIElement* element) const;
            void update(u32 counter, const core::position2di& dist = core::position2di(0,0));
            bool canAttachToBone(const bone* b, size_t& j1, size_t& j2 ) const;
            void attachToBone(bone* b, size_t j1, size_t j2 );
            void dettach();
            bool isAttachedToBone(const bone* b) const;
            bool isRectCollidedBody(const bone* b) const;
            ///! Attach together every bones of the body identified by THIS bone, by as many joints as possible
            void consolidate();
            ///! Attach bones THIS and B with as many joint as possible
            void attachToBoneByAllPossibleJoints(bone* b);
            const IGUIElement* getElement() const;
        private:
            //! Recursive algorithm that detects overlapping bones => dont attached a bone to a body if a bone overlapping is detected
            static bool isRectCollidedBodyRecursive(const CGUIDynamicGrid::bone* b, const core::rect<s32>& rect, core::list<const bone*>& body_bones_processed);
            //! Recursive algorithm that lists the bones of a body, from one bone of this body
            static void listBonesRecursive(const bone* b,core::list<const bone*>& body_bones);
            static void listBonesRecursive(bone* b,core::list<bone*>& body_bones);
            joint* Joints[joint::EJT_MAX];
            IGUIElement* Element;
            u32 Counter;
        };

        //! bones database
        core::list<bone*> Bones;

        //! return the bone of a child element
        bone* getBone( IGUIElement* element );

        //! return true if this element is one of my managed child element (!= irr::isMyChild)
        bool hasBone(const IGUIElement* element, bone*& b) const;

        //! init dynamic grid algorithm
        void init();

        //! reset dynamic grid algorithm
        void reset();

        //! apply dynamic grid algorithm
        void snap();

        //! try to attach this bone to a body
        void tryToAttach(bone* b);

        //! try to form body based on bone proximity
        void tryToAttachAll();

        //! turn bodies into free bones
        void dettachAll();

        ///! appearance of the dynamic grid (DEBUG)
        bool isJointVisible;

        //! behavior of the dynamic grid
        EGUI_DYNAMIC_GRID_BEHAVIOR Behavior;
	};

} // end namespace gui
} // end namespace irr

//#endif // _IRR_COMPILE_WITH_GUI_

#endif // __C_GUI_DYNAMIC_GRID_H_INCLUDED__
