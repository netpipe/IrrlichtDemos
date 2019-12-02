/***********
TEXT CONTROL
***********/
class GUIText
{
public:
	GUIText(int idIn, stringc textIn, position2d<s32> upperLeftIn, position2d<s32> lowerRightIn);
	int getId();
	void changeText(stringc textIn);
private:
	IGUIStaticText* text;
	stringc textString;
	int id;
	position2d<s32> upperLeft, lowerRight;
};

void GUIText::changeText(stringc textIn)
{
	textString=textIn;
	stringw textW;
	convertString(textString, textW,0);
	text->setText(textW.c_str());
}

int GUIText::getId()
{
	return id;
}

GUIText::GUIText(int idIn, irr::core::stringc textIn, irr::core::position2d<s32> upperLeftIn, irr::core::position2d<s32> lowerRightIn)
{
	textString=textIn;
	id=idIn;
	upperLeft=upperLeftIn;
	lowerRight=lowerRightIn;
	stringw textW;
	convertString(textString, textW,0);
	text=env->addStaticText(textW.c_str(), rect<s32>(upperLeft, lowerRight), false, true, 0, -1, false);
}
/**********
BAR CONTROL
**********/
class GUIBar
{
public:
	GUIBar(int idIn, position2d<s32> upperLeftIn, position2d<s32> lowerRightIn, stringc imageIn, bool verticalIn, stringc textIn, bool displayInfoIn);
	void update(float percent);
	int getId();
	~GUIBar();
private:
	IGUIImage* image;
	int id;
	bool vertical, displayInfo;
	position2d<s32> upperLeft, lowerRight, maxes;
	stringc text;
};

int GUIBar::getId()
{
	return id;
}

GUIBar::GUIBar(int idIn, irr::core::position2d<s32> upperLeftIn, irr::core::position2d<s32> lowerRightIn, irr::core::stringc imageIn, bool verticalIn, irr::core::stringc textIn, bool displayInfoIn)
{
	vertical=verticalIn;
	displayInfo=displayInfoIn;
	upperLeft=upperLeftIn;
	lowerRight=lowerRightIn;
	id=idIn;
	text=textIn;
	maxes.X=lowerRight.X-upperLeft.X;
	maxes.Y=lowerRight.Y-upperLeft.Y;
	image=env->addImage(driver->getTexture(imageIn.c_str()),upperLeftIn,true,0,-1);
	image->setMaxSize(dimension2du(maxes.X,maxes.Y));
}

void GUIBar::update(float percent)
{
	if(vertical)
	{
		image->setMaxSize(dimension2du(maxes.X, maxes.Y*percent));
	}else{
		image->setMaxSize(dimension2du(maxes.X*percent, maxes.Y));
	}
}

GUIBar::~GUIBar()
{
	id=-1;
}

/*************
BUTTON CONTROL
*************/
class GUIButton
{
public:
	GUIButton(int idIn, position2d<s32> upperLeftIn, position2d<s32> lowerRightIn, bool changeableIn, bool swapIn, IGUIImage* background, int typeIn);
	void changeButton(int idIn, stringc imageIn);
	bool getChangeable();
	bool getSwap();
	int getId();
	bool isPointInside(position2d<s32> thePoint);
	int getType();
	~GUIButton();
private:
	IGUIButton* button;
	position2d<s32> upperLeft, lowerRight;
	int id, targetId, type;
	bool changeable, swap;
};
int GUIButton::getType()
{
	return type;
}
bool GUIButton::isPointInside(irr::core::position2d<s32> thePoint)
{
	return button->isPointInside(thePoint);
}

int GUIButton::getId()
{
	return id;
}

bool GUIButton::getSwap()
{
	return swap;
}

bool GUIButton::getChangeable()
{
	return changeable;
}

GUIButton::~GUIButton()
{
	id=-1;
}

GUIButton::GUIButton(int idIn, irr::core::position2d<s32> upperLeftIn, irr::core::position2d<s32> lowerRightIn, bool changeableIn, bool swapIn, IGUIImage* background, int typeIn)
{
	//bring variables in
	type=typeIn;
	id=idIn;
	upperLeft=upperLeftIn;
	lowerRight=lowerRightIn;
	changeable=changeableIn;
	swap=swapIn;
	//add button
	button=env->addButton(rect<s32>( upperLeft, lowerRight), background, id);
	button->setImage(driver->getTexture("images/emptyslot.png"));
}

void GUIButton::changeButton(int idIn, stringc imageIn)
{
	id=idIn;
	button->setImage(driver->getTexture(imageIn.c_str()));
}


/*****************************************************
WINDOW THAT HAS UNIQUE BACKGROUND AND ITS OWN ELEMENTS
*****************************************************/
class GUIWindow
{
public:
	void setBackgroundImage(stringc image, int idIn);
	void addButton(int idIn, irr::core::position2d<s32> upperLeftIn, irr::core::position2d<s32> lowerRightIn, bool changeableIn, bool swapIn, int type=0);
	void changeButton(int idIn, stringc image, int idReplace);
	void removeButton(int idIn);
	void addBar(int idIn, irr::core::position2d<s32> upperLeftIn, irr::core::position2d<s32> lowerRightIn, irr::core::stringc imageIn, bool verticalIn, irr::core::stringc textIn, bool displayInfoIn);
	void removeBar(int idIn);
	void updateBar(int idIn, float percentIn);
	void addText(int idIn, stringc textIn, position2d<s32> upperLeftIn, position2d<s32> lowerRightIn);
	void changeText(int idIn, stringc textIn);
	void setId(int idIn);
	int getId();
	bool isPointInside(position2d<s32> thePoint);
	int findButtonFromPoint(position2d<s32> thePoint, bool &changeableOut, bool &swapOut);
	void remove();
	int getButtonTypeFromId(int idIn);
private:
	IGUIImage* background;
	position2d<s32> upperLeft, lowerRight;
	int id;
	vector<GUIButton> buttons;
	vector<GUIBar> bars;
	vector<GUIText> texts;
};
int GUIWindow::getButtonTypeFromId(int idIn)
{
	for(int x=0;x<buttons.size();x++)
	{
		if(buttons[x].getId()==idIn)
		{
			return buttons[x].getType();
		}
	}
	return -1;
}
void GUIWindow::remove()
{
	//remove everything
	background->remove();
	id=-1;
}
int GUIWindow::findButtonFromPoint(position2d<s32> thePoint, bool &changeableOut, bool &swapOut)
{
	int buttonId=-1;
	for(int x=0;x<buttons.size();x++)
	{
		if(buttons[x].isPointInside(thePoint))
		{
			buttonId=buttons[x].getId();
			changeableOut=buttons[x].getChangeable();
			swapOut=buttons[x].getSwap();
			x=buttons.size();
		}
	}
	return buttonId;
}

bool GUIWindow::isPointInside(position2d<s32> thePoint)
{
	return background->isPointInside(thePoint);
}

void GUIWindow::changeText(int idIn, irr::core::stringc textIn)
{
	for(int x=0;x<texts.size();x++)
	{
		if(texts[x].getId()==idIn)
		{
			texts[x].changeText(textIn);
		}
	}
}

void GUIWindow::addText(int idIn, stringc textIn, position2d<s32> upperLeftIn, position2d<s32> lowerRightIn)
{
	GUIText newText(idIn, textIn, upperLeftIn, lowerRightIn);
	texts.push_back(newText);
}

int GUIWindow::getId()
{
	return id;
}

void GUIWindow::setId(int idIn)
{
	id=idIn;
}
void GUIWindow::updateBar(int idIn, float percentIn)
{
	for(int x=0;x<bars.size();x++)
	{
		if(bars[x].getId()==idIn)
		{
			bars[x].update(percentIn);
		}
	}
}

void GUIWindow::addBar(int idIn, irr::core::position2d<s32> upperLeftIn, irr::core::position2d<s32> lowerRightIn, irr::core::stringc imageIn, bool verticalIn, irr::core::stringc textIn, bool displayInfoIn)
{
	GUIBar newBar(idIn, upperLeftIn, lowerRightIn, imageIn, verticalIn, textIn, displayInfoIn);
	bars.push_back(newBar);
}

void GUIWindow::removeBar(int idIn)
{
	for(int x=0;x<bars.size();x++)
	{
		if(bars[x].getId()==idIn)
		{
			bars[x].~GUIBar();
			vector<GUIBar> newBar;
			for(int y=0;y<bars.size()-1;y++)
			{
				if(bars[y].getId()>-1)
				{
					newBar.push_back(bars[y]);
				}
			}
		}
	}
}

void GUIWindow::changeButton(int idIn, stringc image, int idReplace)
{
	for(int x=0;x<buttons.size();x++)
	{
		if(buttons[x].getId()==idReplace&&buttons[x].getChangeable())
		{
			buttons[x].changeButton(idIn, image);
		}
	}
}

void GUIWindow::removeButton(int idIn)
{
	for(int x=0;x<buttons.size();x++)
	{
		if(buttons[x].getId()==idIn)
		{
			buttons[x].~GUIButton();
			vector<GUIButton> newButtons;
			for(int y=0;y<buttons.size()-1;y++)
			{
				if(buttons[y].getId()>-1)
				{
					newButtons.push_back(buttons[y]);
				}
			}
		}
	}
}

void GUIWindow::setBackgroundImage(stringc image, int idIn)
{
	background=env->addImage(driver->getTexture(image.c_str()),upperLeft,true, 0,idIn);
	id=idIn;
}

void GUIWindow::addButton(int idIn, irr::core::position2d<s32> upperLeftIn, irr::core::position2d<s32> lowerRightIn, bool changeableIn, bool swapIn, int type)
{
	GUIButton button(idIn,upperLeftIn,lowerRightIn,changeableIn, swapIn, background, type);
	buttons.push_back(button);
}

/***************************
ENVIRONMENT CONTROLS WINDOWS
***************************/
/*
button types
0 default nothing
1 inventory
2 quickslot
3 skill list
4 equipment
*/
class GUIEnvironment
{
public:
	void buildInventory();
	void buildInterface();
	void buildChatBox();
	void buildMapEditor();
	int findButtonFromPoint(position2d<s32> thePoint, bool &changeableOut, bool &swapOut);
	void changeButton(int idIn, stringc image, int idReplace);
	int getButtonTypeFromId(int idIn);
	bool isInventoryBuilt();
private:
	bool interfaceBuilt;
	vector<GUIWindow> openWindows;
};
void GUIEnvironment::changeButton(int idIn, stringc image, int idReplace)
{
	for(int x=0;x<openWindows.size();x++)
	{
		openWindows[x].changeButton(idIn, image, idReplace);
	}
}

int GUIEnvironment::getButtonTypeFromId(int idIn)
{
	for(int x=0;x<openWindows.size();x++)
	{
		int type=openWindows[x].getButtonTypeFromId(idIn);
		if(type!=-1)
		{
			return type;
		}
	}
	return -1;
}
void GUIEnvironment::buildMapEditor()
{
	GUIWindow guiInterface;
	//id 0 means interface
	guiInterface.setId(0);
	guiInterface.setBackgroundImage("images/guibg.png",100);
	guiInterface.addButton(301,position2d<s32>(990,50), position2d<s32>(990+27,50+27),true, false);
	guiInterface.addButton(302,position2d<s32>(990,110), position2d<s32>(990+27,110+27),true, false);
	guiInterface.addButton(303,position2d<s32>(990,80), position2d<s32>(990+27,80+27),true, false);
	guiInterface.addButton(307,position2d<s32>(990,475), position2d<s32>(990+27,475+27),true, false);
	guiInterface.changeButton(301,"images/heightup.png",301);
	guiInterface.changeButton(302,"images/color.png",302);
	guiInterface.changeButton(303,"images/heightdown.png",303);
	guiInterface.changeButton(307,"images/color.png",307);
	guiInterface.addText(-1,"blue",position2d<s32>(990,140),position2d<s32>(990+30,140+10));
	blueEditBox=env->addEditBox(L"0",rect<s32>(990,155,1020,155+27));
	guiInterface.addText(-1,"red",position2d<s32>(990,190),position2d<s32>(990+30,190+10));
	redEditBox=env->addEditBox(L"0",rect<s32>(990,205,1020,205+27));
	guiInterface.addText(-1,"green",position2d<s32>(990,240),position2d<s32>(990+30,240+10));
	greenEditBox=env->addEditBox(L"0",rect<s32>(990,255,1020,255+27));
	guiInterface.addText(-1,"radius",position2d<s32>(990,290),position2d<s32>(990+30,290+10));
	sphereRadiusEditBox=env->addEditBox(L"5",rect<s32>(990,305,1020,305+27));
	guiInterface.addText(-1,"rate",position2d<s32>(990,340),position2d<s32>(990+30,340+10));
	heightRateEditBox=env->addEditBox(L"50",rect<s32>(990,355,1020,355+27));
	guiInterface.addButton(304,position2d<s32>(990,385), position2d<s32>(990+27,385+27),true, false);
	guiInterface.changeButton(304,"images/setvars.png",304);
	guiInterface.addButton(305,position2d<s32>(990,415), position2d<s32>(990+27,415+27),true, false);
	guiInterface.changeButton(305,"images/save.png",305);
	guiInterface.addButton(306,position2d<s32>(990,445), position2d<s32>(990+27,445+27),true, false);
	guiInterface.changeButton(306,"images/quit.png",306);
	guiInterface.addButton(308,position2d<s32>(990,505), position2d<s32>(990+27,505+27),true, false);
	guiInterface.changeButton(308,"images/tex.png",308);
	//guiInterface.addButton(303,position2d<s32>(990,110), position2d<s32>(990+27,110+27),true, false);
	//guiInterface.addButton(304,position2d<s32>(990,140), position2d<s32>(990+27,140+27),true, false);
	//add to environment
	openWindows.push_back(guiInterface);
}

int GUIEnvironment::findButtonFromPoint(position2d<s32> thePoint, bool &changeableOut, bool &swapOut)
{
	int buttonId=-1;
	for(int x=0;x<openWindows.size();x++)
	{
		if(openWindows[x].isPointInside(thePoint))
		{
			buttonId=openWindows[x].findButtonFromPoint(thePoint, changeableOut, swapOut);
			if(buttonId>-1)
			{
				x=openWindows.size();
			}
		}
	}
	return buttonId;
}
