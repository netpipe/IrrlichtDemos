
#ifndef __World_h
#define __World_h

#include <irrlicht.h>
#include <irrKlang.h>
#include "Object.h"
#include "Player.h"
#include "ThirdPersonCamera.h"
#include "ODEPhysics.h"
#include "SoundAnimator.h"
#include "Editor.h"
#include "Shader.h"
#include "Weather.h"

using namespace irr;

struct PlayerMovement
{
    PlayerMovement()
    {
        reset();
    }
    
    bool forward,back,left,right;
    
    void reset()
    {
        forward = false;
        back = false;
        left = false;
        right = false;
    }
    
    bool moved()
    {
        return forward||back||left||right;
    }
};

class World : public IEventReceiver, public ODEPhysics, public Editor
{
public:
    World(IrrlichtDevice *, audio::ISoundEngine *);
    ~World();
    
    void update(float dt);
    
    bool OnEvent(const SEvent &);
    
private:
    SoundAnimator *cameraSoundAnim;
    
    Shader *shader;
    Weather *weather;
    
    bool bloomEnabled;
    video::ITexture *bloomRTT, *bloomRTT2;
    video::SMaterial bloomMat;
	video::S3DVertex bloomVertices[4];
	u16 bloomIndices[6];
    
    // Player, movement etc
    PlayerMovement playerMovement;
    
    // Editing
    
    enum OBJECT_TYPE
    {
        OBJECT_PLATFORM_1 = 0
    };
};

#endif
