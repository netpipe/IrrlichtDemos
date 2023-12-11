// Mostly copied from ListBox Source

#pragma once

#include "IGUIElement.h"
#include "irrTypes.h"

namespace irr
{
namespace gui
{
	const u32 EGUIET_TREECTRL = 0x45455254;	// TREE in Hex

	const u32 MAX_NODES = 1000000;	// A million nodes is enough. Right?
	const u32 EGET_TREEVIEW_CHANGED_SELECTION = 5000;
	const u32 EGET_TREEVIEW_OPENED_NODE = EGET_TREEVIEW_CHANGED_SELECTION + MAX_NODES;
	const u32 EGET_TREEVIEW_CLOSED_NODE = EGET_TREEVIEW_OPENED_NODE + MAX_NODES;

	class IGUIFont;
	

	
	struct TreeNode
	{
		core::stringw Text;
		wchar_t	IconSingle;	// Icon with no children
		wchar_t IconOpen;	// Icon with children showing
		wchar_t IconClosed;	// Icon with children not showing
		TreeNode *Parent;
		TreeNode *Child;
		TreeNode *Sibling;
		u32 ID;
		void *Data;
		// Display
		bool Open;
	};


	class IGUITreeCtrl : public IGUIElement
	{	
		friend IGUITreeCtrl *AddGUITreeCtrl(
		IGUIEnvironment *env,
		const core::rect<s32> Rectangle,
		IGUIElement *parent,
		s32
			);
		public:


			//! destructor
			~IGUITreeCtrl() {};

			virtual u32 GetNumItems() = 0;
			
			// Create and Delete
			virtual TreeNode *InsertNode(core::stringw Text, TreeNode *Parent = NULL, u32 ID = 0, void *Data = NULL, wchar_t IconSingle = L'\0', wchar_t IconOpen = L'-', wchar_t IconClosed = L'+') = 0;
			virtual bool DeleteNode(TreeNode *node) = 0;
			virtual void Clear() = 0;
			// Find and Navigate
			virtual TreeNode *FindNode(u32 ID) = 0;
			virtual TreeNode *GetRootNode() = 0;
			virtual TreeNode *GetNextNode(TreeNode *node) = 0;
			virtual TreeNode *GetPrevNode(TreeNode *node) = 0;
			
			// Visuals
			virtual TreeNode *GetNextVisibleNode(TreeNode *node) = 0;
			virtual TreeNode *GetPrevVisibleNode(TreeNode *node) = 0;

			virtual bool ChangeParent(TreeNode *node, TreeNode *newparent) = 0;
			
			virtual bool SetSelected(TreeNode *node) = 0;
			virtual TreeNode *GetSelected() = 0;
			
			virtual void OpenNode(TreeNode *node) = 0;
			virtual void CloseNode(TreeNode *node) = 0;
			inline void ToggleOpenNode(TreeNode *node)
			{
				if(node->Open)
					CloseNode(node);
				else
					OpenNode(node);
			};

			virtual bool SetIconFont(IGUIFont *font) = 0;
			virtual IGUIFont *GetIconFont() = 0;
			
			virtual bool SetOverrideFont(IGUIFont *font) = 0;
			virtual IGUIFont *GetOverrideFont() = 0;

			virtual void draw() = 0;
			
			virtual inline gui::EGUI_ELEMENT_TYPE getType() {return (EGUI_ELEMENT_TYPE)EGUIET_TREECTRL;};
			
			virtual bool OnEvent(const SEvent &event) = 0;
		protected:
			//! constructor
			IGUITreeCtrl(IGUIEnvironment* environment, IGUIElement* parent, s32 id, core::rect<s32> rectangle)
			: IGUIElement((EGUI_ELEMENT_TYPE)EGUIET_TREECTRL, environment, parent, id, rectangle) {}

	};
	
	IGUITreeCtrl *AddGUITreeCtrl(
		IGUIEnvironment *env,
		const core::rect<s32> Rectangle,
		IGUIElement *parent = NULL,
		s32 id = -1
	); 

} // end namespace gui
} // end namespace irr


