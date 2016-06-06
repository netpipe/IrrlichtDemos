#ifndef _IrrConsole_TestApp_h_
#define _IrrConsole_TestApp_h_
#include "console.h"
#include "nodeViewer.h"
//=====================================================================================
//! a simple test app to demonstrate the console capabilities
class IC_TestApp : public irr::IEventReceiver, public NodeViewer
{
public:
	IC_TestApp(const String str);
	virtual ~IC_TestApp();
	bool OnEvent(irr::SEvent event);
	void main(array<String>& args);
	const String getName() const{ return name; }

	//
	//	node viewer implementation
	//
	void showModel();
	void showMap();
	void showTestNode();
	void hideNode();

protected:
	void init(array<String>& args);
	bool isRunning();
	void setRunning(bool bVal);
	bool run();
	void drawCaptions();
private:
	bool bRunning;
	irr::IrrlichtDevice* device;
	irr::scene::ISceneNode* sceneNode;
	irr::scene::IAnimatedMesh* levelMesh;
	irr::scene::IAnimatedMesh* modelMesh;
	irr::video::ITexture* modelTexture;
	irr::video::ITexture* testTexture;
	irr::gui::IGUIFont* captionFont;

};
//=====================================================================================
#endif
