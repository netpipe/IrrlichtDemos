
#include "Litha.h"
#include "level_stats.h"

class MainState;
class Level;

class EndLevelScreen : public IUpdatable, public IWantEvents
{
	IEngine *engine;
	IRenderSystem *renderSystem;
	
	MainState *mainState;
	Level *level;
	
	LevelStats stats;
	
	E_SCORE_RESULT scoreResult;
	
	std::vector<gui::IGUIElement *> guiElements;
	
	IEventQueue *eventQueue;
	
	ISound *sound;
	
public:
	EndLevelScreen(MainState *mainState, Level *level);
	~EndLevelScreen();
	
	void OnEvent(const Event &event);
};

