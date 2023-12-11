// Copyright (C) 2010 randomMesh
// This file is part of the Sierpinksi demo.
// For conditions of distribution and use, see copyright notice in main.cpp

#ifndef DEMO_H_
#define DEMO_H_

#include <IEventReceiver.h>

namespace irr
{
	class IrrlichtDevice;

	namespace scene
	{
		class ISierpinskiSceneNode;
		class ISceneNodeAnimator;
	}

	namespace gui
	{
		class IGUIStaticText;
	}
}

class Demo : public irr::IEventReceiver
{

public:

	Demo();

	~Demo();

	bool init(int argc, char* argv[]);

	void run() const;

	virtual bool OnEvent(const irr::SEvent& event);

private:

	void takeScreenshot() const;

	irr::IrrlichtDevice* device;

	irr::scene::ISierpinskiSceneNode* node;

	irr::scene::ISceneNodeAnimator* anim;

	irr::gui::IGUIStaticText* infoText;
};

#endif /* DEMO_H_ */
