Irrlicht Tree Control

By: Veylon (veylon_ii@yahoo.com)
based heavily on the listbox code by Nikolaus Gebhardt

This is a fairly basic tree control,
as seen in many guis, but not irrlicht.

Just put these files in your code:
	IGUITreeCtrl.h
	CGUITreeCtrl.cpp
	CGUITreeCtrl.h

And create a tree control thus:
	gui::IGUITreeCtrl *tree = gui::IGUITreeCtrl *tree = gui::AddGUITreeCtrl(gui, core::rect<s32>(20,20,220,420));

Add a node like this:
	gui::TreeNode *node = tree->InsertNode(L"A Node")

Iterate Through all the nodes like this:
	for(gui::TreeNode *node = tree->GetRootNode(); node; node = tree->GetNextNode(node))
		wprintf(L"Name: %s, ID: %i\n", node->Text, node->ID);

Change the Text like this:
	gui::TreeNode *node = tree->FindNode(25);
	if(node)
		node->Text = stringw(L"Node ID 25");

To navigate the tree control, you can use the mouse and keyboard.
	Clicking on a square shows or hides the child nodes.
	Clicking on a node selects it.
	Pressing right or left shows or hides the child nodes.
	Pressing up or down moves to the next visible node.
	Rolling the mouse wheel does the same.
 	You can scroll the scrollbar to see more items.

Most of the functions are simple and self-explanatory, but note the following:
	There are no sort functions. You can, however, sort them yourself.
	The GetPrevious... Functions are much less efficient than the GetNext... ones.
	The Get...Visible Functions do not take into accout where the scrollbar is, only whether
		or not their parent is "Open" or "Closed"
	Also, please do not change the nodes' IDs or delete them outside of the control.

Here are the currently existing functions:
	bool ChangeParent(TreeNode* node, TreeNode* newparent)	Moves a node to a new parent
	Clear()							Deletes all the nodes
	TreeNode *FindNode(u32 ID)				Finds a node with a particular ID
	TreeNode *GetNextNode(TreeNode*)			Get the next node, iteratively
	TreeNode *GetPrevNode(TreeNode*)			Get the previous node, iteratively
	TreeNode *GetNextVisibleNode(TreeNode*)			Get the next visible node, iteratively
	TreeNode *GetPrevVisibleNode(TreeNode*)			Get the previous visible node, iteratively
	u32 GetNumItems()					Get the number of nodes
	TreeNode *GetRootNode()					Get the root (but don't edit)
	TreeNode *InsertNode(stringw Text, int ID, TreeNode *parent)	Adds a new node
	bool DeleteNode(TreeNode*)				Delete a node

Here are the TreeNode variables:
	TreeNode *Child		FIRST Child Node of this node
	TreeNode *Parent	Parent of node
	TreeNode *Sibling	Next sibling if current node
	bool Open		True if child nodes should be displayed, false otherwise
	stringw Text		Displayed Text
	u32 ID			ID of node. Used only for searching. RootNode is 0
	void *Data		A pointer to data. User defined. Not deleted when Node is