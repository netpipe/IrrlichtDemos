#ifndef _GLOBALS_H
#define _GLOBALS_H

#include "main.h"

#define LIGHTING 1          // Enables Lights
#define DEBUG_CONSOLE 0     // Verbose Console Output
#define DEBUG_PHYSICS 0     // Physics Debug Drawing
#define DEBUG_ZONES 0       // Zone Drawing
#define CC_PHYSICS 1        // Character Controlled by Physics
#define LIGHT_BB 0          // Light BillBoards
#define SOUNDS 1            // Enable Sounds

#define PI 3.14159265

#define ID_IsMap (1 << 0)
#define ID_IsPlayer (1 << 1)
#define ID_IsEnemyShot (1 << 2)
#define ID_IsPlayerShot (1 << 3)
#define ID_IsNotPickable (1 << 4)

enum DebugDrawModes
{
    DBG_NoDebug=0,
    DBG_DrawWireframe = 1,
    DBG_DrawAabb=2,
    DBG_DrawFeaturesText=4,
    DBG_DrawContactPoints=8,
    DBG_NoDeactivation=16,
    DBG_NoHelpText = 32,
    DBG_DrawText=64,
    DBG_ProfileTimings = 128,
    DBG_EnableSatComparison = 256,
    DBG_DisableBulletLCP = 512,
    DBG_EnableCCD = 1024,
    DBG_MAX_DEBUG_DRAW_MODE
};

enum TriggerTypes
{
    TR_SOUND = 1,
    TR_AMMO = 2,
    TR_CONTROL = 3,
    TR_SHADER = 4
};

enum ShaderTypes
{
    SHT_BLOOM = 1,
    SHT_INVERT = 2
};

enum EOSATypes
{
    EOSA_PCTRL = 1
};

enum WeaponTypes
{
    WT_NORMAL = 0,
    WT_ATTRACTIVE = 1,
    WT_EXPLOSIVE = 2
};

enum FiringMode
{
    FM_SINGLE = 0,
    FM_TRIPLE = 1,
    FM_LIMITED = 2
};

enum Powerups
{
    PU_HEALTH = 0,
    PU_A_NORMAL = 1,
    PU_A_ATTRACTIVE = 2,
    PU_A_EXPLOSIVE = 3
};

enum CollisionFlags
{
    CF_STATIC_OBJECT= 1,
    CF_KINEMATIC_OBJECT= 2,
    CF_NO_CONTACT_RESPONSE = 4,
    CF_CUSTOM_MATERIAL_CALLBACK = 8
};

enum ZoneColors
{
    Z_RED = 1,
    Z_BLUE = 2,
    Z_GREEN = 3,
    Z_BLACK = 4
};

enum ZoneTypes
{
    Z_NORMAL = 1,
    Z_DEATH = 2,
    Z_END = 3
};

class CEventReceiver : public IEventReceiver
{
public:
	struct SMouseState
    {
        core::position2di Position;
        bool LeftButtonDown;
        bool RightButtonDown;
        SMouseState() : LeftButtonDown(false), RightButtonDown(false) { }
    }MouseState;
	virtual bool OnEvent(const SEvent& event)
	{
		if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
        {
			switch(event.MouseInput.Event)
			{
				case EMIE_LMOUSE_PRESSED_DOWN:
					MouseState.LeftButtonDown = true;
					break;

				case EMIE_LMOUSE_LEFT_UP:
					MouseState.LeftButtonDown = false;
					break;

                case EMIE_RMOUSE_PRESSED_DOWN:
                    MouseState.RightButtonDown = true;
                    break;

                case EMIE_RMOUSE_LEFT_UP:
                    MouseState.RightButtonDown = false;
                    break;

				case EMIE_MOUSE_MOVED:
					MouseState.Position.X = event.MouseInput.X;
					MouseState.Position.Y = event.MouseInput.Y;
					break;

				default:
					break;
			}
		}
		if (event.EventType == EET_KEY_INPUT_EVENT)
			KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
		if (event.EventType == EET_MOUSE_INPUT_EVENT)
		{
			if(event.MouseInput.Event == EMIE_MOUSE_WHEEL)
				Mouse_Delta = event.MouseInput.Wheel;
		}
		return false;
	}
	virtual bool IsKeyDown(EKEY_CODE keyCode) const
	{
		return KeyIsDown[keyCode];
	}
	virtual f32 MouseDelta()
	{
		return Mouse_Delta;
	}
	virtual void ClearMouseDelta()
	{
		Mouse_Delta=0;
	}
	const SMouseState & GetMouseState(void) const
    {
		return MouseState;
    }
	CEventReceiver()
	{
		for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
			KeyIsDown[i] = false;
	}
private:
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
	f32 Mouse_Delta;
};

#endif
