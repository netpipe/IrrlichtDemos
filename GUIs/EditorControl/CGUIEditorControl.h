#pragma once

#include "IGUITreeCtrl.h"
#include "IGUIElement.h"
#include "IGUIWindow.h"
#include "IGUISkin.h"
#include "irrTypes.h"

namespace irr
{
namespace gui
{
	class CGUIEditorControl : public IGUIWindow
	{		
	public:

		//! constructor
		CGUIEditorControl(IGUIEnvironment* environment, core::rect<s32> rectangle, IGUIElement* parent = 0, s32 id = -1);

		//! destructor
		~CGUIEditorControl();

		static CGUIEditorControl* GetSingleEditorControl(IGUIEnvironment* env, core::rect<s32> rc, IGUIElement* parent = 0, s32 id = -1)
		{
			if(!Singleton) Singleton = new CGUIEditorControl(env, rc, parent, id);	
			return Singleton;
		}

		bool OnEvent(const SEvent &event);
		void draw(void );
		void UseBuiltInSkin(bool useit = true);

		IGUIButton* getCloseButton() const		{ return CloseButton; }
		IGUIButton* getMinimizeButton() const	{ return MinButton; }
		IGUIButton* getMaximizeButton() const	{ return RestoreButton; }

	protected:

		core::position2d<s32> DragStart;
		bool Dragging;

		
		IGUISkin*					Skin;
		IGUIButton*					CloseButton;
		IGUIButton*					MinButton;
		IGUIButton*					RestoreButton;
		IGUITreeCtrl*				TreeView;
		static CGUIEditorControl*	Singleton;

		void ScalGUI(const core::dimension2d<s32> &oldresolution);
		void ScalElementsRecursive(f32 x_factor, f32 y_factor, IGUIElement*);
		void UpdateTreeView(void);
		void AddElementsRecursive(TreeNode*, IGUIElement*);
		void LoadToSelectedElement(void);
		void LoadFromSelectedElement(void);
		core::stringw GenNodeName(IGUIElement* element);
	}; 

} // end namespace gui
} // end namespace irr


