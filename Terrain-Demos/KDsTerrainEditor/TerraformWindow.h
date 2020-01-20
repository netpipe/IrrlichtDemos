#include <irrlicht.h>

class TerraformWindow
{
	IGUIEnvironment* guienv;
	IGUIWindow* window;
	IGUIScrollBar* gridsizeScrollbar;
	IGUIStaticText* gridsizeText;
public:

	TerraformWindow(IGUIEnvironment* gui) :guienv(gui)
	{
		window = guienv->addWindow(rect<s32>(10,10, 310, 510), false, L"Terraformer");
		gridsizeScrollbar = guienv->addScrollBar(true, recti(5,30, 205, 45), window);
		gridsizeScrollbar->setMin(3);
		gridsizeScrollbar->setMax(30);
		gridsizeScrollbar->setText(L"TestText");
		gridsizeScrollbar->setSmallStep(2);
		gridsizeText = guienv->addStaticText(L"Gridsize: 3", recti(210, 32, 300, 60), false, false, window);

		IGUIStaticText* controls = guienv->addStaticText(L"Left click to raise\nRight click to lower\n\nMove using arrow keys\nGo up/down using pg up/pg dwn\nTilt camera up/down using home/end",recti(5,50,300,300), false, true, window);
	}

	void setVisible(bool state)
	{
		window->setVisible(state);
	}

	IGUIScrollBar* getTerraformSizeScrollbar()
	{
		return gridsizeScrollbar;
	}

	IGUIStaticText* getTerraformSizeText()
	{
		return gridsizeText;
	}
};
