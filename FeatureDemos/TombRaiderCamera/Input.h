/*

Input

*/

class cInput {
public:
	
	// constructor
	cInput(cEventReceiver *eventReceiver);
	
	// destructor
	~cInput();
	
	// check mouse and keyboard input
	void handleInput(cEventReceiver *eventReceiver, cSkeleton *skeleton);
	
private:

	
};

// constructor
cInput::cInput(cEventReceiver *eventReceiver) {
};

// destructor
cInput::~cInput() {
};

// check mouse and keyboard input
void cInput::handleInput(cEventReceiver *eventReceiver, cSkeleton *skeleton) {

	if (eventReceiver->Keys[KEY_KEY_W])
		if (eventReceiver->Keys[KEY_SHIFT])
			skeleton->setAnimType(CSK_ANIM_WALK);
		else
			skeleton->setAnimType(CSK_ANIM_RUN);
	else
		skeleton->setAnimType(CSK_ANIM_STAND);

	if (eventReceiver->Keys[KEY_SPACE])
		skeleton->setAnimType(CSK_ANIM_JUMP);

};
