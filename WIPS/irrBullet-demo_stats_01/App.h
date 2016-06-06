// App.h: interface for the CApp class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "BulletPhysicsWorld.h"
#include <vector>
#include "Arrow.h"
#include "LinearMath/btIDebugDraw.h"

using namespace irr;


class CBulletPhysicsObject;
class CWorldObjectMgr;

enum _keyactions {
	KACTION_QUIT,
	KACTION_1,
	KACTION_2,
	KACTION_3,
	KACTION_4,
	KACTION_5,
	KACTION_6,
	KACTION_TOGGLE_SHADOWS,
	KACTION_TOGGLE_AABBS,
	KACTION_TOGGLE_WIREFRAME,
	KACTION_TOGGLE_POINTCLOUD,
	KACTION_TOGGLE_DEACTIVATION,
	KACTION_TOGGLE_STATUES,
	KACTION_TOGGLE_DEBUGWINDOW
};

#define PROF_BULLET "Bullet time"
#define PROF_BULLET_PERCENT "Bullet percentage CPU use"
#define PROF_GAME "Game time"
#define PROF_RENDER "Render Time"


class CApp : public IEventReceiver
{
private:

	//key mapping data
	struct SCamKeyMap
	{
		s32 action;
		EKEY_CODE keycode;
		SCamKeyMap() {};
		SCamKeyMap(s32 a, EKEY_CODE k) : action(a), keycode(k) {}
	};

	std::vector<SCamKeyMap> m_KeyMap;
	s32 m_numActionKeys;

	irr::scene::ICameraSceneNode* m_pCamera;
	irr::scene::ILightSceneNode* m_pLight;
	CBulletPhysicsWorld * m_pPhysicsWorld;
	CWorldObject * m_pTerrainObject, * m_pAltar;
	irr::scene::ISceneNode* m_pStatueNode1, *m_pStatueNode2;
	CWorldObjectMgr* g_pWorldObjectMgr;

	//a simple graphic effect for the 'weapon trails'
	//actually just using a utility class (CArrow) that displays 3D arrows
	class Laser
	{
	private:
		CArrow* m_pArrow;
	public:
		f32 time;
		Laser(const core::vector3df &start, const core::vector3df &end)
		{
			m_pArrow = new CArrow(WHITE, 0.02f,false,true);
			m_pArrow->SetEndpoints(start, end);
			time=0;
		};
		bool Update(f32 elapsedTime) {
			return ((time += elapsedTime) > 0.5f);
		};
		void Cleanup() {
			m_pArrow->Cleanup(); //not in destructor or app will crash on exit, when irrlicht deletes scene nodes
		};
	};
	std::vector<Laser*> m_pLaserArray;

	bool m_bShadows, m_bAABBs, m_bDeactivation, m_bWireFrame, m_bPointCloud, m_bShowStatues, m_bDebugWindow;
	gui::IGUIEditBox *m_pShadowText, *m_pAABBtext, *m_pDeactivationText, *m_pWireframeText, *m_pShowStatuesText, *m_pFPSText, *m_pNumObjectsText, *m_pNumPolysText, *m_pPointCloudText;
	gui::IGUIListBox *m_pBTProfText;

public:

	////////////////////////////////////////////////////////////////////////////
	//simple classes to return key and mouse input state
	class UserInput {
	protected:
		bool down, pressed, released;
	public:
		UserInput() : down(false), pressed(false), released(false) {};
		void Clear()	{
			down=pressed=released=false;
		};
		bool IsDown()	{
			return down;
		};
		virtual void Press()=0;
		void Release()	{
			down=pressed=false, released=true;
		};
		bool IsPressed(){
			bool wasPressed=pressed;
			pressed=false;
			return wasPressed;
		};
		bool IsReleased(){
			bool wasReleased=released;
			released=false;
			return wasReleased;
		};
	};
	class ActionKeys : public UserInput
	{
	public:
		void Press() {
			pressed = !down; //so key repeat will be blocked from setting pressed
			down=true;
			released=false;
		}
	} *m_ActionKeys;

	class MouseKeys : public UserInput
	{
	public:
		void Press() {
			down=pressed=true, released=false;
		};
	} m_MouseKeys[3];
	////////////////////////////////////////////////////////////////////////////


	core::position2d<s32> m_WindowDims;

public:
	CApp();
	virtual	~CApp();
	bool IsMouseKeyDown(s32 key)		{	return m_MouseKeys[key].IsDown();		};
	bool IsMouseKeyPressed(s32 key)		{	return m_MouseKeys[key].IsPressed();	};
	bool IsMouseKeyReleased(s32 key)	{	return m_MouseKeys[key].IsReleased();	};
	bool IsActionKeyDown(s32 key)		{	return m_ActionKeys[key].IsDown();		};
	bool IsActionKeyPressed(s32 key)	{	return m_ActionKeys[key].IsPressed();	};
	bool IsActionKeyReleased(s32 key)	{	return m_ActionKeys[key].IsReleased();	};
	bool OnEvent(const SEvent& event);
	void Update(f32 elapsedTime);
	void AllKeysUp();
	void Run();
	void AddObject(s32 type, core::vector3df pos);
	void ShowDebugWindow(bool);
	void UpdateDebugDisplay();
	void UpdateProfiling(f32);
	void SetDisplayOptions(CWorldObject*);
	void SetText(bool on, irr::gui::IGUIEditBox* pBox);
};


class CMyBtDebugDraw :
	public btIDebugDraw
{
	int m_debugMode;
public:

	CMyBtDebugDraw(void) {};
	virtual ~CMyBtDebugDraw(void) {};
	void	drawLine(const btVector3& from,const btVector3& to,const btVector3& color) {};
	void	drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color) {};
	void	setDebugMode(int debugMode) { m_debugMode = debugMode; };
	void	reportErrorWarning(const char* warningString) {};

	int		getDebugMode() const { return m_debugMode; };
};
