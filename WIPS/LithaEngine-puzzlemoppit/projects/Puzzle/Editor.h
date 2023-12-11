
#include "Litha.h"
#include "Enums.h"

class Level;
class Map;

struct Brush
{
	gui::IGUIImage *image;
	u16 row,column;
	int id;
};

class Editor : public IUpdatable, public IWantInput, public IEventReceiver, public IWantEvents
{
	IEngine *engine;
	IWorld *world;
	Level *level;
	Map *map;
	scene::ISceneManager *smgr;
	
	scene::ICameraSceneNode *camera;
	scene::ISceneNode *targetCube;
	
	bool inToolMode;
	
	bool justExitedToolMode;
	
	std::vector<Brush> brushes;
	int selectedBrush;
	
	core::vector3di GetTargetCoord();
	
	struct DisplayedEvent
	{
		core::vector3di coord;
		ITransformable *entity;
	};
	
	std::vector<DisplayedEvent> displayedEvents;
	
	// Display or hide an event graphical representation.
	void DisplayEvent(core::vector3di coord);
	void HideEvent(core::vector3di coord);
	void RefreshEventDisplay();
	
	void SetMode(bool toolMode);
	
	void AddBrushIcon(u16 row, u16 column, const c8 *brushIconFileName, int id);
	
	// Creates all icons representing available objects and events.
	void CreateBrushSelector();
	void DestroyBrushSelector();

public:
	Editor(Level *level);
	~Editor();
	
	// irr events
	bool OnEvent(const SEvent &event);
	// litha engine events
	void OnEvent(const Event &event);
	
	void OnAxisChange(int id, float delta);
	void OnButtonDown(int id);
	void OnButtonUp(int id);
	
	void Update(f32 dt);
};



