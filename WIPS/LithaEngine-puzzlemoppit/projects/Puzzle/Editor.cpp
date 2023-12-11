
#include "Editor.h"
#include "Level.h"
#include "Map.h"
#include "Events.h"

core::vector3di Editor::GetTargetCoord()
{
	return level->GetCoord(targetCube->getPosition());
}

void Editor::DisplayEvent(core::vector3di coord)
{
	if (IMapEventOwner *event = map->GetEvent(coord))
	{
		IMesh *mesh = world->AddMesh((c8 *)event->GetIconMeshName().c_str());
		
		ASSERT(mesh);
		
		mesh->SetPosition(level->GetPosFromCoord(coord));
		mesh->ApplyTransformNow();
		
		DisplayedEvent de = {coord, mesh};
		displayedEvents.push_back(de);
	}
}

void Editor::HideEvent(core::vector3di coord)
{
	for (u32 i = 0; i < displayedEvents.size(); i ++)
	{
		if (displayedEvents[i].coord == coord)
		{
			world->RemoveTransformable(displayedEvents[i].entity);
			displayedEvents.erase(displayedEvents.begin()+i);
			return;
		}
	}
}

void Editor::RefreshEventDisplay()
{
	for (u32 i = 0; i < displayedEvents.size(); i ++)
		world->RemoveTransformable(displayedEvents[i].entity);
	
	displayedEvents.clear();
	
	// Find all events and make them visible...!?
	
	std::vector<core::vector3di> eventCoords = map->GetAllEvents();
	
	for (u32 i = 0; i < eventCoords.size(); i ++)
		DisplayEvent(eventCoords[i]);
}

Editor::Editor(Level *level)
{
	this->level = level;
	
	justExitedToolMode = false;
	
	// Level is grabbed so that it is not ever deleted before Editor.
	// (as Editor will want to call level->Save() when destructing)
	level->grab();
	
	this->map = level->GetMap();
	
	this->engine = GetEngine();
	this->world = GetEngine()->GetWorld();
	
	smgr = engine->GetIrrlichtDevice()->getSceneManager();
	
	// Disable logic for the level. Don't want movable things changing position while editing.
	level->SetLogicEnabled(false);
	
	// Remove third person camera and such
	level->GetPlayer()->SetController(NULL);
	world->SetCameraController(NULL);
	
	// Remove usual input system
	world->SetInputProfile(NULL);
	
	// Create "target cube", a 3D cursor.
	targetCube = smgr->addCubeSceneNode(1.0);
	targetCube->setMaterialFlag(video::EMF_WIREFRAME, true);
	
	// Controls
	
	IInputProfile *inputProfile = world->CreateInputProfile(4, 1);
	inputProfile->BindButton(0, KEY_LBUTTON);
	inputProfile->BindButton(1, KEY_RBUTTON);
	inputProfile->BindButton(2, KEY_KEY_S);
	inputProfile->BindButton(3, KEY_KEY_L);
	inputProfile->BindAxis(0, AXIS_MOUSE_WHEEL);
	world->SetInputProfile(inputProfile);
	inputProfile->drop();
	
	world->SubscribeToInput(this);
	
	// Want to receive GUI events (to know when brush icons have been clicked)
	engine->GetIrrlichtDevice()->getGUIEnvironment()->setUserEventReceiver(this);
	
	// want Litha events
	engine->RegisterEventInterest(this, "ButtonDown");
	
	// Find all events and make them visible...!?
	RefreshEventDisplay();
	
	// Add an Irrlicht FPS camera
	
	SKeyMap keyMapArray[] =
	{
		// Cursor keys
		{EKA_MOVE_FORWARD, KEY_UP},
		{EKA_MOVE_BACKWARD, KEY_DOWN},
		{EKA_STRAFE_LEFT, KEY_LEFT},
		{EKA_STRAFE_RIGHT, KEY_RIGHT},
		// WSAD
		{EKA_MOVE_FORWARD, KEY_KEY_W},
		{EKA_MOVE_BACKWARD, KEY_KEY_S},
		{EKA_STRAFE_LEFT, KEY_KEY_A},
		{EKA_STRAFE_RIGHT, KEY_KEY_D},
	};
	
	camera = smgr->addCameraSceneNodeFPS(NULL, 100.0, 0.005,
			-1, keyMapArray, 8);
	
	smgr->setActiveCamera( camera );
	
	// No brush initially selected.
	selectedBrush = 0;
	
	ASSERT(selectedBrush == EOT_UNKNOWN && selectedBrush == EET_UNKNOWN);
	
	// Hide player
	{
		//IMesh *mesh = (IMesh *)level->GetPlayer()->GetGraphic();
		
		//ASSERT(mesh);
		
		//mesh->GetIrrlichtNode()->setVisible(false);
		
		// Hack to get the player out of the way of editing.
		// "proper" solution would be to remove player completely.
		level->GetPlayer()->SetPosition( core::vector3df(0,-10000.0,0) );
	}
	
	inToolMode = false;
	SetMode(true);
}

Editor::~Editor()
{
	engine->UnregisterAllEventInterest(this);
	level->Save();
	level->drop();
}

void Editor::SetMode(bool toolMode)
{
	if (toolMode == inToolMode)
		return; // No change.
	
	if (toolMode)
	{
		// Entered tool select mode.
		camera->setInputReceiverEnabled(false);
		
		// Show cursor.
		engine->GetIrrlichtDevice()->getCursorControl()->setVisible(true);
		
		// Need to show GUI.
		CreateBrushSelector();
	}
	else
	{
		// Entered edit mode.
		camera->setInputReceiverEnabled(true);
		
		// Hide cursor.
		engine->GetIrrlichtDevice()->getCursorControl()->setVisible(false);
		
		// Hide GUI.
		DestroyBrushSelector();
	}
	
	inToolMode = toolMode;
}

void Editor::AddBrushIcon(u16 row, u16 column, const c8 *brushIconFileName, int id)
{
	gui::IGUIEnvironment *guienv = engine->GetIrrlichtDevice()->getGUIEnvironment();
	video::IVideoDriver *driver = engine->GetIrrlichtDevice()->getVideoDriver();
	
	const core::dimension2di iconDimensions = core::dimension2di(64,64);
	
	const int x = iconDimensions.Width;
	const int y = iconDimensions.Height;
	const int padding = 10;
	
	core::recti rect(
			x + (iconDimensions.Width + padding) * column, y + (iconDimensions.Height + padding) * row,
			x + (iconDimensions.Width + padding) * (column+1), y + (iconDimensions.Height + padding) * (row+1)
			);
	
	gui::IGUIImage *image = guienv->addImage(rect);
	image->setImage( driver->getTexture(brushIconFileName) );
	
	Brush brush = {image, row, column, id};
	brushes.push_back(brush);
}

void Editor::CreateBrushSelector()
{
	// For all object types
	// For all event types,
	// Add a graphic to *screen* (no zbuffer?) and store in a list...
	
	AddBrushIcon(0,0, "icon_groundtile.png",	EOT_GROUND_BLOCK);
	AddBrushIcon(1,0, "icon_fallground.png",	EOT_GROUND_BLOCK_FALL);
	AddBrushIcon(2,0, "icon_movable_block.png",	EOT_MOVABLE_BLOCK);
	AddBrushIcon(3,0, "icon_fan.png",			EOT_FAN);
	AddBrushIcon(4,0, "icon_lift.png",			EOT_LIFT);
	AddBrushIcon(5,0, "icon_slidingbox.png",	EOT_SLIDING_BLOCK);
	AddBrushIcon(6,0, "icon_balloon.png",		EOT_BALLOON);
	AddBrushIcon(7,0, "icon_exitportal.png",	EOT_EXIT_PORTAL);
	
	AddBrushIcon(0,1, "icon_event_player_start.png",	EOT_COUNT + EET_PLAYER_START_EVENT);
	AddBrushIcon(1,1, "icon_event_fan.png",		EOT_COUNT + EET_FAN_EVENT);
	AddBrushIcon(2,1, "icon_event_lift.png",	EOT_COUNT + EET_LIFT_EVENT);
}

void Editor::DestroyBrushSelector()
{
	for (u32 i = 0; i < brushes.size(); i ++)
		brushes[i].image->remove();
	
	brushes.clear();
}

bool Editor::OnEvent(const SEvent &event)
{
	// Clicking a GUI Image
	// Selecting a brush type by clicking its icon.
	if (event.EventType == EET_GUI_EVENT
			&& event.GUIEvent.EventType == gui::EGET_ELEMENT_FOCUSED)
	{
		selectedBrush = 0;
		
		// Need to match Caller to an object or event type...
		for (u32 i = 0; i < brushes.size(); i ++)
		{
			if (brushes[i].image == event.GUIEvent.Caller)
			{
				selectedBrush = brushes[i].id;
				break;
			}
		}
		
		justExitedToolMode = true;
		
		// Exit brush select mode!
		SetMode(false);
	}
	
	return false;
}

void Editor::OnEvent(const Event &event)
{
	if (event.IsType("ButtonDown") && event["button"] == KEY_ESCAPE)
		engine->Exit();
}

void Editor::OnAxisChange(int id, float delta)
{
	if (id != 0)
		return;
	
	if (delta < 0.0)
	{
		// Enter brush select mode.
		SetMode(true);
	}
	else
	{
		// Return to painting mode.
		SetMode(false);
	}
}

void Editor::OnButtonDown(int id)
{
	core::vector3di targetCoord = GetTargetCoord();
	
	if (id == 0 && !inToolMode && selectedBrush) // No adding or deletion when in tool mode.
	{
		// hack
		// since changing how events are handled, I now need to ensure
		// that this isn't the same mouse button down event that was generated
		// when selecting a tool.
		if (justExitedToolMode)
		{
			justExitedToolMode = false;
			return;
		}
		
		// Create an object or an event based on the currently selected brush.
		if (selectedBrush < EOT_COUNT)
		{
			// Add an object?
			if (!map->GetObject(targetCoord))
				level->CreateObject(targetCoord, (E_OBJECT_TYPE)selectedBrush);
		}
		else
		{
			// Add an event?
			if (!map->GetEvent(targetCoord))
			{
				level->CreateEvent(targetCoord, (E_EVENT_TYPE)(selectedBrush - EOT_COUNT));
				DisplayEvent(targetCoord);
			}
		}
	}
	else if (id == 1 && !inToolMode) // No adding or deletion when in tool mode.
	{
		// Remove object
		level->RemoveObject(targetCoord);
		
		// Remove event
		if (IMapEventOwner *event = map->GetEvent(targetCoord))
		{
			map->SetEvent(targetCoord, NULL);
			HideEvent(targetCoord);
		}
	}
	else if (id == 2)
	{
		// Save
		// Also auto saves on exit.
		level->Save();
	}
	else if (id == 3)
	{
		// Load
		// THIS IS NOW NOT DONE AT ALL. The only place a level is loaded for editing
		// is on start, when passed via command line.
		//level->Load();
		//RefreshEventDisplay();
	}
}

void Editor::OnButtonUp(int id)
{
}

void Editor::Update(f32 dt)
{
	// Need to update target cube's position... Keeping it tied to a grid coordinate.
	
	const f32 targetCubeDistance = 5.0;
	
	core::vector3df cameraVec = (camera->getTarget() - camera->getPosition()).normalize();
	
	core::vector3df worldPos = camera->getPosition()
			+ cameraVec
			* targetCubeDistance;
	
	targetCube->setPosition( level->GetPosFromCoord(level->GetCoord(worldPos)) );
	
	// don't want the mouse to be centred ever
	// despite World enabling it.
	engine->SetAutoMouseCentring(false,false);
}





