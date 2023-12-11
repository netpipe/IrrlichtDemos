
#include "CGUITreeCtrl.h"
#include "IGUIEnvironment.h"
#include "IGUIFont.h"
#include "IVideoDriver.h"

using namespace irr;
using namespace gui;
using namespace core;

IGUITreeCtrl *irr::gui::AddGUITreeCtrl(
		IGUIEnvironment *env,
		const core::rect<s32> Rectangle,
		IGUIElement *parent,
		s32 id
)
{
	if(parent == NULL)
		parent = env->getRootGUIElement();
	CGUITreeCtrl *tree = new CGUITreeCtrl(env, parent, id, Rectangle);
	return tree;
}
CGUITreeCtrl::~CGUITreeCtrl()
{
	ScrollBarH->drop();
	ScrollBarV->drop();
	if(Font)
		Font->drop();
	Clear();
	delete RootNode;
	IDList.clear();
}

CGUITreeCtrl::CGUITreeCtrl(IGUIEnvironment* environment, IGUIElement* parent, s32 id, core::rect<s32> rectangle):
IGUITreeCtrl(environment, parent, id, rectangle),
NumItems(0),
Selected(NULL),
Font(NULL),
IconFont(NULL),
OverrideFont(NULL),
video(environment->getVideoDriver())
{
	RootNode = new TreeNode;
	RootNode->Parent = NULL;
	RootNode->Child = NULL;
	RootNode->Sibling = NULL;
	RootNode->ID = 0;
	RootNode->Text = stringw("Root Node");
	RootNode->Data = 0;
	RootNode->Open = true;
	
	IDList.set_sorted(true);
	IDList.push_back(0);
	
	ScrollBarV = Environment->addScrollBar(
		false,
		core::rect<s32>(RelativeRect.getWidth() - Environment->getSkin()->getSize(EGDS_SCROLLBAR_SIZE), 0, RelativeRect.getWidth(), RelativeRect.getHeight() - Environment->getSkin()->getSize(EGDS_SCROLLBAR_SIZE)),
		this
	);
	ScrollBarH = Environment->addScrollBar(true,
		core::rect<s32>(0,RelativeRect.getHeight() - Environment->getSkin()->getSize(EGDS_SCROLLBAR_SIZE), RelativeRect.getWidth()-Environment->getSkin()->getSize(EGDS_SCROLLBAR_SIZE), RelativeRect.getHeight()),
		this
	);
	ScrollBarH->setPos(0);
	ScrollBarV->setPos(0);
	recalculateItemHeight();
//	Font = Environment->getSkin()->getFont();
}

TreeNode *CGUITreeCtrl::FindNode(u32 ID)
{
	if(ID == 0)
		return NULL;	// Don't find root node!
	bool FromChild = false;
	for(TreeNode *CurNode = RootNode; CurNode; CurNode = GetNextNode(CurNode))
		if(CurNode->ID == ID)
			return CurNode;
	return NULL;
}
TreeNode *CGUITreeCtrl::GetNextNode(TreeNode *node)
{
	// Try Child
	if(node->Child)
		return node->Child;
	// Try Sibling
	else if(node->Sibling)
		return node->Sibling;
	// Go down ancestor tree until we find a sibling
	while(node->Parent && !node->Sibling)
		node = node->Parent;
	if(node->Sibling)
		return node->Sibling;
	else
		return NULL;			// Bust
}
TreeNode *CGUITreeCtrl::GetNextVisibleNode(TreeNode *node)
{
	// Try Child
	if(node->Child && node->Open)
		return node->Child;
	// Try Sibling
	else if(node->Sibling)
		return node->Sibling;
	// Go down ancestor tree until we find a sibling
	while(node->Parent && !node->Sibling)
		node = node->Parent;
	if(node->Sibling)
		return node->Sibling;
	else
		return NULL;			// Bust
}
TreeNode *CGUITreeCtrl::GetPrevNode(TreeNode *node)
{
	if(!node->Parent)
		return NULL;
	if(node->Parent->Child != node)	// Later Sibling. Return previous
	{
		TreeNode *prev = node->Parent->Child;
		while(prev->Sibling != node)
			prev = prev->Sibling;
		if(prev->Child)	// Get last child of previous
		{
			prev = prev->Child;
			bool Last = false;
			while(prev->Sibling || prev->Child)
			{
				if(prev->Sibling)
					prev = prev->Sibling;
				else if(prev->Child)
					prev = prev->Child;
			}
			return prev;
		}
		else	// Previous sibling has no children. Return it
		{
			return prev;
		}
	}
	else if(node->Parent->Child == node)	// Only Child. Return parent
		return node->Parent;
	else
		return NULL;	// No previous
/*	Horrible Inefficient Old Way
	TreeNode *prev = RootNode;
	TreeNode *next;
	next = GetNextNode(prev);
	while(next && next != node)
	{
		prev = next;
		next = GetNextNode(prev);
	}
	if(next)
		return prev;
	else
		return NULL;
*/
}
TreeNode *CGUITreeCtrl::GetPrevVisibleNode(TreeNode *node)
{
	if(node->Parent->Child != node)	// Later Sibling. Return previous
	{
		TreeNode *prev = node->Parent->Child;
		while(prev->Sibling != node)
			prev = prev->Sibling;
		if(prev->Child && prev->Open)	// Get last child of previous
		{
			prev = prev->Child;
			bool Last = false;
			while(prev->Sibling || (prev->Child && prev->Open))
			{
				if(prev->Sibling)
					prev = prev->Sibling;
				else if(prev->Child && prev->Open)
					prev = prev->Child;
			}
		}
		return prev;
	}
	else if(node->Parent->Child == node)	// Only Child. Return parent
	{
		if(node->Parent == RootNode)
			return NULL;
		return node->Parent;
	}
	else
		return NULL;	// No previous
/*	Inefficient Old Way
	if(node == RootNode)	// Only one that doesn't have a previous
		return false;
	TreeNode *prev = RootNode;
	TreeNode *next;
	next = GetNextNode(prev);
	while(next && next != node)
	{
		prev = next;
		next = GetNextVisibleNode(prev);
	}
	if(next)
		return prev;
	else
		return NULL;
*/
}
TreeNode *CGUITreeCtrl::InsertNode(core::stringw Text, TreeNode *parent, u32 id, void *Data, wchar_t IconSingle , wchar_t IconOpen , wchar_t IconClosed)
{
	// Root node is parent, if none other
	if(!parent)
		parent = RootNode;
	// Make ID if none
	if(id == 0)
	{
		u32 i = IDList.size();
		while(IDList.binary_search(i) != -1)
		{
			i++;
			if(i>=MAX_NODES)
			{
				i -= MAX_NODES;
			}
		}
		id = i;
	}
	// Duplicate ID, quit
	if(IDList.binary_search(id) != -1)
		return NULL;
	TreeNode *newnode = new TreeNode;
	newnode->Child = NULL;
	newnode->Parent = parent;
	newnode->Sibling = NULL;
	newnode->ID = id;
	newnode->Data = Data;
	newnode->Text = Text;
	newnode->Open = false;
	newnode->IconSingle = IconSingle;
	newnode->IconOpen = IconOpen;
	newnode->IconClosed = IconClosed;
	if(!parent->Child)	// Childless parent is simple
	{
		parent->Child = newnode;
	}
	else
	{
		TreeNode *curnode = parent->Child;
		while(curnode->Sibling)
			curnode = curnode->Sibling;
		curnode->Sibling = newnode;
		// Find last sibling of parent and include new sibling
	}
	// Tidy up
	NumItems++;
	IDList.push_back(id);
	recalculateScrollBars();
	return newnode;
}
bool CGUITreeCtrl::ChangeParent(TreeNode *node, TreeNode *newparent)
{
	if(!node)				// We have to move SOMETHING
		return false;
	if(node == RootNode)	// Heavens No!
		return false;
	if(newparent == node->Parent)	// What would be the point?
		return true;	// There! Done.
	if(!newparent)			// No go.
		return false;
	// Sever old ties
	CutTies(node);
	// Insert self into new situation
	node->Parent = newparent;	// We have a new parent, no matter what
	node->Sibling = NULL;	// We're the last, no matter what
	// Is the parent childless?
	if(!newparent->Child)
	{
		newparent->Child = node;
	}
	// Otherwise, put ourselves at the end of the list
	else
	{
		TreeNode *lastnode = newparent->Child;
		while(lastnode->Sibling)
			lastnode = lastnode->Sibling;
		lastnode->Sibling = node;
	}
	// OK, all good
	return true;
}

bool CGUITreeCtrl::DeleteNode(TreeNode *node)
{
	if(!node)				// No can do
		return false;
	if(node == RootNode)	// Noooo!!!!!
		return false;
	if(node == Selected)
		Selected = NULL;
	// Deal with child nodes
	while(node->Child)
		DeleteNode(node->Child);
	CutTies(node);
	// Kill our ID from the list
	s32 Offset = IDList.binary_search(node->ID);
	if(Offset != -1)
		IDList.erase(Offset);
	delete node;
	NumItems--;
	recalculateScrollBars();
	return true;
}

void CGUITreeCtrl::Clear()
{
	while(RootNode->Child)
		DeleteNode(RootNode->Child);
}
void CGUITreeCtrl::draw()
{
	if(!IsVisible)
		return;

	recalculateItemHeight(); // if the font changed

	IGUISkin* skin = Environment->getSkin();
	irr::video::IVideoDriver* driver = Environment->getVideoDriver();

	core::rect<s32>* clipRect = 0;
	clipRect = &AbsoluteClippingRect;

	// draw background
	core::rect<s32> frameRect(AbsoluteRect);

	skin->draw3DSunkenPane(this, skin->getColor(EGDC_3D_HIGH_LIGHT), true,
		true, frameRect, clipRect);
	
	// draw items

	core::rect<s32> clientClip(AbsoluteRect);
	clientClip.UpperLeftCorner.Y += 1;
	clientClip.UpperLeftCorner.X += 1;
	clientClip.LowerRightCorner.X -= skin->getSize(EGDS_SCROLLBAR_SIZE)+1;
	clientClip.LowerRightCorner.Y -= skin->getSize(EGDS_SCROLLBAR_SIZE)+1;

	if (clipRect)
		clientClip.clipAgainst(*clipRect);
	
	// Our frame
/*
	frameRect = AbsoluteRect;
	frameRect.UpperLeftCorner.X += 1;
	frameRect.LowerRightCorner.X = AbsoluteRect.LowerRightCorner.X - skin->getSize(EGDS_SCROLLBAR_SIZE);
	frameRect.LowerRightCorner.Y = AbsoluteRect.UpperLeftCorner.Y + ItemHeight;

	frameRect.UpperLeftCorner.Y -= ScrollBar->getPos();
	frameRect.LowerRightCorner.Y -= ScrollBar->getPos();
*/
	
	if(RootNode->Child)	// No point in drawing anything otherwise
	{
		core::rect<s32> *MyClip = NULL;		
		core::array<bool> DistStack;
		u32 Indent = 1-ScrollBarH->getPos()/10;
		s32 Count = 0;
		video::SColor colblk = skin->getColor(gui::EGDC_BUTTON_TEXT);
		TreeNode *node = RootNode->Child;
		bool FromChild = false;
		while(node)	// If we're at the root node, then we're done
		{
			s32 DistCount = Count - ScrollBarV->getPos()/10;
			if(DistCount >= 0)
			{
				// Stop drawing
				if((DistCount+1) * ItemHeight > RelativeRect.getHeight())
				{
					break;
				}
				else if((DistCount+2) * ItemHeight > RelativeRect.getHeight())
				{
					MyClip = &clientClip;
				}
				// this seems to fix the horizontal clipping problem 
				MyClip = &clientClip; 
				
				s32 IconWidth = 0;
				// Get Origin
				// = ItemWidth to the left of the text
				core::position2d<s32> Origin = AbsoluteRect.UpperLeftCorner + core::position2d<s32>((Indent-1)*ItemHeight, DistCount * ItemHeight);
				video::SColor col;
				if(node == Selected)
				{
					col = skin->getColor(gui::EGDC_HIGH_LIGHT_TEXT);
				}
				else
				{
					col = skin->getColor(gui::EGDC_BUTTON_TEXT);
				}
				// Draw Lines
				if(node->Parent != RootNode)
				{
					// Horizontal
					if(node->Child)	// Accomodate Square
					{
						DrawHorizontalLine(
							Origin + core::position2d<s32>(-Frac12, Frac12),
							Frac12 + Frac18,
							colblk, MyClip);
					}
					else	// No Need
					{
						DrawHorizontalLine(
							Origin + core::position2d<s32>(-Frac12, Frac12),
							Frac12,
							colblk, &clientClip);
					}
					// Vertical
					if(node->Sibling)
					{
						DrawVerticalLine(
							Origin + core::position2d<s32>(-Frac12, 0),
							ItemHeight,
							colblk, MyClip);
					}
					else
					{
						DrawVerticalLine(
							Origin + core::position2d<s32>(-Frac12, 0),
							Frac12,
							colblk, MyClip);
					}
					// Vertical Far left lines
					for(u32 l = 0; l < DistStack.size()-1; l++)
					{
						if(DistStack[DistStack.size() - l - 1])
						{
							DrawVerticalLine(
								Origin - core::position2d<s32>((l+1)*ItemHeight + Frac12,0),
								ItemHeight,
								colblk, &clientClip);
						}
						/*	Red lines for debugging
						else
						{
							Draw2DLine(
								Origin - core::position2d<s32>((l+1)*ItemHeight + Frac12+1, 0),
								Origin - core::position2d<s32>((l+1)*ItemHeight + Frac12+1, -ItemHeight),
								video::SColor(0xFFFF0000), clientClip);
						}
						*/
					}	
				}
				// Square, Plus sign if closed
				if(node->Child && Indent > 0)
				{
					if(IconFont)
					{
						wchar_t Txt[2] = {node->Open ? node->IconOpen : node->IconClosed, L'\0'};
						IconWidth = IconFont->getDimension(Txt).Width+1;
						// Use EGDC_BUTTON_TEXT color (colblk) for everything except selected text, otherwise it doesn't show properly against certain background colors 
						IconFont->draw(Txt, core::rect<s32>(Origin, Origin + core::position2d<s32>(IconWidth, ItemHeight)), colblk, false, true, MyClip); 
					}
					else
					{
						core::position2d<s32> UpperLeft = Origin + core::position2d<s32>(Frac18, Frac18);
						core::position2d<s32> UpperRight = Origin + core::position2d<s32>(Frac78, Frac18);
						core::position2d<s32> LowerLeft = Origin + core::position2d<s32>(Frac18, Frac78);
						if(node->Open)	// Draw bottom line to match up
						{
							DrawVerticalLine(
								Origin + core::position2d<s32>(Frac12, Frac78),
								Frac18, colblk, MyClip);
						}
						else
						{
							// Draw Plus Sign
							// Horizontal
							DrawHorizontalLine(
								Origin + core::position2d<s32>(Frac18, Frac12),
								SqSize, colblk, MyClip);
							// Vertical
							DrawVerticalLine(
								Origin + core::position2d<s32>(Frac12, Frac18),
								SqSize, colblk, MyClip);
						}
						// Draw Square (After, so it covers up and looks nice
						DrawVerticalLine(UpperLeft, SqSize, colblk, MyClip);
						DrawHorizontalLine(UpperLeft, SqSize, colblk, MyClip);
						DrawVerticalLine(UpperRight, SqSize, colblk, MyClip);
						DrawHorizontalLine(LowerLeft, SqSize, colblk, MyClip);
						IconWidth = ItemHeight;
					}				
				}
				// Draw Text
				if(!node->Child && IconFont)
				{
					wchar_t Txt[2] = {node->IconSingle, L'\0'};
					IconFont->draw(Txt,
						core::rect<s32>
						(	AbsoluteRect.UpperLeftCorner.X + (Indent-1)*ItemHeight + 1,
							AbsoluteRect.UpperLeftCorner.Y + DistCount * ItemHeight + 1,
							AbsoluteRect.LowerRightCorner.X - skin->getSize(EGDS_SCROLLBAR_SIZE),
							AbsoluteRect.UpperLeftCorner.Y + (DistCount+1) * ItemHeight
						), col, false, true, MyClip);
					IconWidth = IconFont->getDimension(Txt).Width+1;
				}
				if(node == Selected)
					video->draw2DRectangle(skin->getColor(gui::EGDC_HIGH_LIGHT),core::rect<s32>(Origin + core::position2d<s32>(IconWidth,0), Origin + core::position2d<s32>(ItemHeight + Font->getDimension(node->Text.c_str()).Width+IconWidth-ItemHeight+1,ItemHeight)), &clientClip); 
				Font->draw(node->Text.c_str(),
					core::rect<s32>
					(	AbsoluteRect.UpperLeftCorner.X + (Indent-1)*ItemHeight + IconWidth + 2,
						AbsoluteRect.UpperLeftCorner.Y + DistCount * ItemHeight,
						AbsoluteRect.LowerRightCorner.X - skin->getSize(EGDS_SCROLLBAR_SIZE),
						AbsoluteRect.UpperLeftCorner.Y + (DistCount+1) * ItemHeight
					), col, false, false, MyClip);
			}
			// And we get the next node
			// Try Child
			if(node->Child && node->Open)
			{
				if(node->Sibling)
					DistStack.push_back(true);
				else
					DistStack.push_back(false);
				Indent++;
				node = node->Child;
			}
			// Try Sibling
			else if(node->Sibling)
			{
				node = node->Sibling;
			}
			// Go down ancestor tree until we find a sibling
			else
			{
				while(node->Parent && !node->Sibling)
				{
					node = node->Parent;
					Indent--;
					if(node != RootNode)
						DistStack.erase(DistStack.size()-1);
				}
				if(node->Sibling)
					node = node->Sibling;
				else
					break;
			}
			Count++;
		}
	}
	IGUIElement::draw();
}
void CGUITreeCtrl::DrawHorizontalLine(core::position2d<s32> start, s32 length, video::SColor &color, const core::rect<s32> *ClipRect)
{
	// Line can't be draw here
	if(ClipRect)
	{
		// this fixes the 'missing vert lines in top row' prob 
		rect<s32> ExpClipRect(	ClipRect->UpperLeftCorner - position2d<s32>(1,1), 
								ClipRect->LowerRightCorner + position2d<s32>(1,1)); 
		if(!ExpClipRect.isPointInside(start)) 
			return;
		if(start.X + length > ClipRect->LowerRightCorner.X)
			length = ClipRect->LowerRightCorner.X - start.X;
	}
	video->draw2DLine(start, start + core::position2d<s32>(length,0), color);
}
void CGUITreeCtrl::DrawVerticalLine(core::position2d<s32> start, s32 length, video::SColor &color, const core::rect<s32> *ClipRect)
{
	// Line can't be draw here
	if(ClipRect)
	{
		if(!ClipRect->isPointInside(start))
			return;
		if(start.Y + length > ClipRect->LowerRightCorner.Y)
			length = ClipRect->LowerRightCorner.Y - start.Y;
	}
	video->draw2DLine(start, start + core::position2d<s32>(0,length), color);
}
void CGUITreeCtrl::recalculateItemHeight()
{
	IGUISkin* skin = Environment->getSkin();
	if(
		(OverrideFont && Font != OverrideFont)
		||
		(!OverrideFont && Font != skin->getFont())
	)
	{
		if (Font)
			Font->drop();

		if(OverrideFont)
			Font = OverrideFont;
		else
			Font = skin->getFont();
		ItemHeight = 0;

		if (Font)
		{
			ItemHeight = Font->getDimension(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789").Height;
			// The squares look better
			// When itemheight is divisible by 4 or 8
			if(ItemHeight <= 12)	// Div by 4
			{				
				if(ItemHeight % 4)
					ItemHeight += 4-(ItemHeight % 4);
				Frac18 = ItemHeight/4;
				Frac12 = ItemHeight/2;
				Frac78 = ItemHeight*3/4;
			}
			else	// Div by 8
			{
				if(ItemHeight % 8)
					ItemHeight += 8-(ItemHeight % 8);
				Frac18 = ItemHeight/8;
				Frac12 = ItemHeight/2;
				Frac78 = ItemHeight*7/8;
			}
			SqSize = ItemHeight - Frac18 * 2;
			Font->grab();
			recalculateScrollBars();
		}
	}

//	TotalItemHeight = ItemHeight * Items.size();
//	ScrollBar->setMax(TotalItemHeight - AbsoluteRect.getHeight());
}

void CGUITreeCtrl::recalculateScrollBars()
{
	s32 MaxItems =  NumItems - RelativeRect.getHeight() / ItemHeight + 1;
	if(MaxItems < 0)
		MaxItems = 0;
	ScrollBarV->setMax(10 * MaxItems);
}

void CGUITreeCtrl::CutTies(TreeNode *node)
{
	// Only Child
	if(node->Parent->Child == node && !node->Sibling)
	{
		node->Parent->Child = NULL;
	}
	// First Child
	else if(node->Parent->Child == node && node->Sibling)
	{
		node->Parent->Child = node->Sibling;
	}
	// Last Child
	else if(node->Parent->Child != node && !node->Sibling)
	{
		TreeNode *NewLast = node->Parent->Child;
		while(NewLast->Sibling != node)
			NewLast = NewLast->Sibling;
		NewLast->Sibling = NULL;
	}
	// Middle Child
	else
	{
		TreeNode *NewPlace = node->Parent->Child;
		while(NewPlace->Sibling != node)
			NewPlace = NewPlace->Sibling;
		NewPlace->Sibling = node->Sibling;
	}
	node->Parent = NULL;
	node->Sibling = NULL;
}
//! called if an event happened.
bool CGUITreeCtrl::SetSelected(TreeNode *node)
{
	if(node == RootNode)	// Can't Select Root Node
		node = NULL;
	if(node && this->FindNode(node->ID) != node)	// Make sure this node exists on this tree
		return false;
	Selected = node;
	if(Parent)
	{
		SEvent NewEvent;
		NewEvent.EventType = irr::EET_GUI_EVENT;
		NewEvent.GUIEvent.Caller = this;
		if(node)
			NewEvent.GUIEvent.EventType = (irr::gui::EGUI_EVENT_TYPE)(EGET_TREEVIEW_CHANGED_SELECTION + node->ID);
		else
			NewEvent.GUIEvent.EventType = (irr::gui::EGUI_EVENT_TYPE)(EGET_TREEVIEW_CHANGED_SELECTION);
		Parent->OnEvent(NewEvent);
	}
	return true;
}
TreeNode *CGUITreeCtrl::GetSelected()
{
	return Selected;
}
void CGUITreeCtrl::OpenNode(TreeNode *node)
{
	if(!node || node->Open)
		return;
	if(Parent)
	{
		SEvent NewEvent;
		NewEvent.GUIEvent.Caller = this;
		NewEvent.EventType = irr::EET_GUI_EVENT;
		NewEvent.GUIEvent.Caller = this;
		NewEvent.GUIEvent.EventType = (irr::gui::EGUI_EVENT_TYPE)(EGET_TREEVIEW_OPENED_NODE + node->ID);
		node->Open = true;
		Parent->OnEvent(NewEvent);
	}
}
void CGUITreeCtrl::CloseNode(TreeNode *node)
{
	if(!node || !node->Open)
		return;
	if(Parent)
	{
		SEvent NewEvent;
		NewEvent.GUIEvent.Caller = this;
		NewEvent.EventType = irr::EET_GUI_EVENT;
		NewEvent.GUIEvent.EventType = (irr::gui::EGUI_EVENT_TYPE)(EGET_TREEVIEW_CLOSED_NODE + node->ID);
		node->Open = false;
		Parent->OnEvent(NewEvent);
	}
}

bool CGUITreeCtrl::OnEvent(const SEvent &event)
{

	switch(event.EventType)
	{
	case irr::EET_KEY_INPUT_EVENT:
	{
		if(Selected && event.KeyInput.PressedDown)
		{
			TreeNode *NewSelected = NULL;
			switch(event.KeyInput.Key)
			{
				case KEY_LEFT:
					if(Selected->Child && Selected->Open)
					{
						CloseNode(Selected);
						return true;
					}
				case KEY_UP:
					NewSelected = GetPrevVisibleNode(Selected);
					break;
				case KEY_RIGHT:
					if(Selected->Child && !Selected->Open)
					{
						OpenNode(Selected);
						return true;
					}
				case KEY_DOWN:
					NewSelected = GetNextVisibleNode(Selected);
					break;
			}
			if(NewSelected != NULL)
				SetSelected(NewSelected);
		}
		return true;		
	}
	case EET_GUI_EVENT:
		switch(event.GUIEvent.EventType)
		{
		case gui::EGET_SCROLL_BAR_CHANGED:
			if (event.GUIEvent.Caller == ScrollBarV || event.GUIEvent.Caller == ScrollBarH)
			{
				return true;
			}
			break;
		}
		break;
	case EET_MOUSE_INPUT_EVENT:
		{
			core::position2d<s32> p(event.MouseInput.X, event.MouseInput.Y);

			switch(event.MouseInput.Event)
			{
				case EMIE_MOUSE_WHEEL:
					if(Selected)
					{
						TreeNode *NewSel; 
						if(event.MouseInput.Wheel < 0.0f)
							NewSel = GetNextVisibleNode(Selected);
						else if(event.MouseInput.Wheel > 0.0f)
							NewSel = GetPrevVisibleNode(Selected);
						else
							{}
						if(NewSel != NULL)
							SetSelected(NewSel);
					}
					return true;

				case EMIE_LMOUSE_PRESSED_DOWN:
				{
					if(RootNode->Child)
					{
						core::rect<s32> AreaRect = AbsoluteRect;
						AreaRect.LowerRightCorner.X -= Environment->getSkin()->getSize(gui::EGDS_SCROLLBAR_SIZE);
						AreaRect.LowerRightCorner.Y -= Environment->getSkin()->getSize(gui::EGDS_SCROLLBAR_SIZE);
						if(AreaRect.isPointInside(p))
							Environment->setFocus(this);
						else
						{
							Environment->removeFocus(this);
							return false;
						}
						p -= AbsoluteRect.UpperLeftCorner;
						int CY = -ScrollBarV->getPos() / 10 * ItemHeight;
						int Indent = 0;
						bool FromChild = false;
						TreeNode *node = RootNode;
						do
						{
							// And we get the next node
							if(node->Open && node->Child)
							{
								node = node->Child;
								Indent++;
							}
							else if(node->Sibling)
							{
								node = node->Sibling;
								FromChild = false;
							}
							else
							{
								TreeNode *next = node;
								while(!node->Sibling && node->Parent)
								{
									node = node->Parent;
									Indent--;
								}
								if(node->Sibling)
									node = node->Sibling;
								else
								{
									SetSelected(NULL);
									return false;
								}
							}
							CY += ItemHeight;
						}
						while(CY < p.Y);
						
						if(p.X > (Indent-1) * ItemHeight)
						{
							if(IconFont)
							{
								wchar_t txt[2] = { node->Open ? node->IconOpen : node->IconClosed, L'\0' };
								if(p.X < (Indent-1) * ItemHeight + IconFont->getDimension(txt).Width)
									ToggleOpenNode(node);
								else
									SetSelected(node);
									
							}
							else
							{
								if(p.X < Indent * ItemHeight && node->Child)
									ToggleOpenNode(node);
								else
									SetSelected(node);
							}
						}
					}
					return true;
				}
				case EMIE_LMOUSE_LEFT_UP:		
					return true;

				case EMIE_MOUSE_MOVED:
					return true;
			}
		}
		break;
	}

	return Parent ? Parent->OnEvent(event) : false;
}


bool CGUITreeCtrl::SetIconFont(IGUIFont *font)
{
	if(font == IconFont)
		return false;
	if(IconFont)
		IconFont->drop();
	IconFont = font;
	if(IconFont)
		IconFont->grab();
	return true;
}

IGUIFont *CGUITreeCtrl::GetIconFont()
{
	return IconFont;
}

bool CGUITreeCtrl::SetOverrideFont(IGUIFont *font)
{
	if(font == OverrideFont)
		return false;
	if(OverrideFont)
		OverrideFont->drop();
	OverrideFont = font;
	if(OverrideFont)
		OverrideFont->grab();	
	recalculateItemHeight();
	return true;
}

IGUIFont *CGUITreeCtrl::GetOverrideFont()
{
	return OverrideFont;
}