#include "CCompositeEventReceiver.h"

//Include all the possible windows
#include "TerraformWindow.h"

class GUIManager : public CCompositeEventReceiver
{
	irr::core::array<CCompositeEventReceiver*> Children;
	IGUIEnvironment* guienv;
	TerraformWindow* tw;
public:
	GUIManager(IGUIEnvironment *gui):guienv(gui)
	{
		tw = new TerraformWindow(gui);
		tw->setVisible(true);
	}

	bool OnEvent(const SEvent& event)
	{
		for (irr::u32 i=0; i<Children.size();++i)
		{
			if (Children[i]->OnEvent(event)) return true;
		}

		if (event.EventType == EET_GUI_EVENT)
		{
			if (event.GUIEvent.EventType == irr::gui::EGET_ELEMENT_CLOSED)
			{
				//tw->setVisible(false);
				return true;
			}
			else if (event.GUIEvent.Caller->getID() == tw->getTerraformSizeScrollbar()->getID())
			{
				core::stringw newText = L"Gridsize: ";
				newText += tw->getTerraformSizeScrollbar()->getPos();
				tw->getTerraformSizeText()->setText(newText.c_str());
				return true;
			}
		}


		if (event.EventType == irr::EET_KEY_INPUT_EVENT)
		{
			if (event.KeyInput.Key == KEY_F10)
			{
				//Hide GUI
			}
		}

		return false;
	}

	void add(CCompositeEventReceiver* newChild)
	{
		Children.push_back(newChild);
	}

	void remove(CCompositeEventReceiver* oldChild)
	{
		s32 idx = Children.binary_search(oldChild);
		if (idx != -1) Children.erase(idx);
		else std::runtime_error("EventReceiver: Not a child");
	}

	TerraformWindow* getTerraformWindow()
	{
		return tw;
	}
};
