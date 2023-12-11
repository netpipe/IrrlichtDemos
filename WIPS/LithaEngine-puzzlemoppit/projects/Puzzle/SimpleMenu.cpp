
#include "SimpleMenu.h"
#include "Positioner.h"

// taken from mainstate
gui::IGUIStaticText *add_static_text2(const wchar_t *str);
gui::IGUIStaticText *add_static_text(const wchar_t *str);

// copied from mainstate
#define TEXT_COL			video::SColor(150, 255,255,255)
#define TEXT_COL_MOUSEOVER	video::SColor(100, 200,200,200)

SimpleMenu::SimpleMenu(s32 uniqueId)
{
	id = uniqueId;
	
	engine = GetEngine();
	device = engine->GetIrrlichtDevice();
	
	positioner = NULL;
	
	// sound
	menuSound = engine->GetSoundSystem()->CreateSound2D();
	menuSound->SetVolume(0.5);
	
	engine->RegisterEventInterest(this, "ButtonDown");
	engine->RegisterEventInterest(this, "AxisMoved");
}

SimpleMenu::~SimpleMenu()
{
	if (positioner)
	{
		positioner->Reset();
		delete positioner;
	}
	
	menuSound->drop();
	
	engine->UnregisterAllEventInterest(this);
}

// In future, shutdown should probably also queue the menu for complete removal.
void SimpleMenu::Shutdown()
{
	// No more clicks or mouseovers.
	engine->UnregisterAllEventInterest(this);
}

void SimpleMenu::AddItem(core::stringw name, s32 uniqueItemId, bool removeMenuOnClick)
{
	positioner->Add( add_static_text(name.c_str()), uniqueItemId );
}

void SimpleMenu::AddImageItem(io::path imageName, s32 uniqueItemId, bool removeMenuOnClick)
{
	gui::IGUIEnvironment *guienv = device->getGUIEnvironment();
	video::IVideoDriver *driver = device->getVideoDriver();
	
	video::ITexture *texture = driver->getTexture(imageName);
	//core::dimension2du size = texture->getOriginalSize();
	
	positioner->Add(
			guienv->addImage(texture, core::position2di(0,0)),
			uniqueItemId
			);
}

void SimpleMenu::SetItemGap(u32 gap)
{
	positioner->SetSpacing(gap);
}

void SimpleMenu::Finalise()
{
	positioner->Apply();
	
	// Init the menu mouseover stuff.
	Event event("AxisMoved");
	event["axis"] = AXIS_MOUSE_X;
	event["SimpleMenu::Finalise"] = 1;
	engine->PostEvent(event);
}

void SimpleMenu::SetMouseOverSound(const c8 *sound)
{
	mouseOverSound = sound;
}

s32 SimpleMenu::GetId()
{
	return id;
}

const std::vector<gui::IGUIElement *> &SimpleMenu::GetElements()
{
	ASSERT(positioner);
	return positioner->GetElements();
}

void SimpleMenu::OnEvent(const Event &event)
{
	if (event.IsType("ButtonDown"))
	{
		if (event["button"] == KEY_LBUTTON)
		{
			s32 mouseOverId = positioner->GetMouseOverId();
			
			if (mouseOverId != -1)
			{
				Event event("MenuButton");
				event["menu"] = id;
				event["button"] = mouseOverId;
				engine->QueueEvent(event);
				
				// And shutdown the menu.
				Shutdown();
			}
		}
	}
	else if (event.IsType("AxisMoved"))
	{
		if (event["axis"] == AXIS_MOUSE_X || event["axis"] == AXIS_MOUSE_Y)
		{
			gui::IGUIElement *mouseOverElement = positioner->GetMouseOverElement();
			
			// Update all colours
			
			const std::vector<gui::IGUIElement *> &elements = positioner->GetElements();
			
			for (u32 i = 0; i < elements.size(); i ++)
			{
				if (elements[i]->getType() == gui::EGUIET_STATIC_TEXT)
				{
					gui::IGUIStaticText *textElement = (gui::IGUIStaticText *)elements[i];
					
					if (elements[i] == mouseOverElement)
					{
						if (textElement->getOverrideColor() != TEXT_COL_MOUSEOVER)
						{
							if (mouseOverSound.size() && !event.HasKey("SimpleMenu::Finalise"))
								menuSound->Play(mouseOverSound.c_str());
						}
						
						textElement->setOverrideColor( TEXT_COL_MOUSEOVER );
					}
					else
					{
						textElement->setOverrideColor( TEXT_COL );
					}
				}
				else if (elements[i]->getType() == gui::EGUIET_IMAGE)
				{
					gui::IGUIImage *imageElement = (gui::IGUIImage *)elements[i];
					
					if (elements[i] == mouseOverElement)
					{
						// Play sound if state not detected before, or if it was not previously mouse-overed.
						if (!mouseOverStates.count(imageElement)
								|| !mouseOverStates[imageElement])
						{
							if (mouseOverSound.size() && !event.HasKey("SimpleMenu::Finalise"))
								menuSound->Play(mouseOverSound.c_str());
						}
						
						imageElement->setColor(video::SColor(100, 255,255,255));
						mouseOverStates[imageElement] = true;
					}
					else
					{
						imageElement->setColor(video::SColor(255, 255,255,255));
						mouseOverStates[imageElement] = false;
					}
				}
			}
		}
	}
}


// ***************** SimpleHorizontalMenu ****************


SimpleHorizontalMenu::SimpleHorizontalMenu(s32 uniqueId, s32 yPos, s32 spacing, bool vertCentred)
	: SimpleMenu(uniqueId)
{
	video::IVideoDriver *driver = device->getVideoDriver();
	u32 halfScreenHeight = driver->getScreenSize().Height / 2;
	
	positioner = new RowPositioner(device->getVideoDriver(), yPos == -1 ? halfScreenHeight : yPos, spacing, vertCentred);
}

void SimpleHorizontalMenu::SetHeading(core::stringw text)
{
	((RowPositioner *)positioner)->SetTitle( add_static_text(text.c_str()) );
}


// ***************** SimpleVerticalMenu ****************


SimpleVerticalMenu::SimpleVerticalMenu(s32 uniqueId, f32 marginBottom)
	: SimpleMenu(uniqueId)
{
	video::IVideoDriver *driver = device->getVideoDriver();
	u32 halfScreenHeight = driver->getScreenSize().Height / 2;
	
	//positioner = new ColumnPositioner(device->getVideoDriver(), 20);
	positioner = new ColumnPositionerCentred(device->getVideoDriver(), 10, marginBottom);
}




// ***************** SimpleEitherSideToggleMenu ****************


SimpleEitherSideToggleMenu::SimpleEitherSideToggleMenu(s32 uniqueId)
	: SimpleMenu(uniqueId)
{
	video::IVideoDriver *driver = device->getVideoDriver();
	
	positioner = new EitherSidePositioner(device->getVideoDriver());
}

void SimpleEitherSideToggleMenu::Enable(bool left, bool right)
{
	((EitherSidePositioner *)positioner)->Enable(left,right);
}




