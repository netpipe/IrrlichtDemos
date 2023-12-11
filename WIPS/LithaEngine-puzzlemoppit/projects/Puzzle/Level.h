
#ifndef LEVEL_H
#define LEVEL_H

#include "Litha.h"
#include "Enums.h"
#include <deque>
#include "level_stats.h"

class Map;
class IMapEventOwner;
class MainState;
class DefaultEvent;

extern bool gridBasedMovement;
extern bool invertMouseX;
extern bool invertMouseY;

/*

// Perhaps replace MapObject in Map.h with this.
// Also replace all uses of litha IObject with this...
// And have a MapObjectFactory to create IMapObjects.
class IMapObject
{
	core::vector3di previousCoord;
	bool previousCoordSet;
	
public:
	IMapObject()
	{
		previousCoordSet = false;
	}
	
	virtual E_OBJECT_TYPE GetType() = 0;
	
	virtual bool IsMovable() = 0;
	
	virtual bool HasPreviousCoord()
	{
		return previousCoordSet;
	}
	
	virtual core::vector2di GetPreviousCoord()
	{
		return previousCoord;
	}
	
	// Internal use only.
	// Called by Map.StartObjectMove() when the object is to be moved.
	virtual void OnStartMove(core::vector3di oldCoord, core::vector3di newCoord)
	{
		previousCoord = oldCoord;
		previousCoordSet = true;
	}
};

*/

struct MapObjectMove
{
	u32 startTimeMs;
	u32 finishTimeMs; // when move should finish
	core::vector3di startMapCoord; // added this back in, since we need it for OnLeaveLocation
	core::vector3df startMapPos;	// changed to float as player may not have exact map position
									// to interpolate from
	core::vector3di endMapCoord;
};

// An entity that is not part of the grid, yet still occupies
// grid squares when moving across the grid.
struct Actor
{
	// Character is proxied as a slightly convoluted way to give a position offset.
	IProxyTransformable *entity;
	core::vector3di centreLocation;
	bool centreLocationSet;
	// locations occupied by actor in last update
	std::vector<core::vector3di> lastLocations;
	
	ICharacter *GetCharacter()
	{
		return (ICharacter *)entity->GetProxiedTransformable();
	}
};

enum TUTORIAL_TRIGGER_TYPE
{
	TTT_NORMAL = 0,
	TTT_X,
	TTT_Y,
	TTT_Z,
	TTT_SURROUND // when hits any of 8 squares surrounding this square (but excluding the square itself)
};

struct TutorialText
{
	std::vector<core::stringc> lines;
	f32 displayTime; // length text is displayed for
	f32 delayTime; // length after trigger before text is displayed
	
	TUTORIAL_TRIGGER_TYPE type;
	core::vector3di trigger;
};

struct UndoState
{
	struct MapLoc
	{
		core::vector3di coord;
		s32 objectType;
		s32 eventType;
		
		bool hasPreviousCoord;
		core::vector3di previousCoord;
		
		bool is_sliding;
	};
	
	std::vector<MapLoc> mapContents;
	
	std::vector<TutorialText> tutorialTexts; // those remaining...
	
	LevelStats stats;
	
	core::vector2df cameraAngle;
	f32 cameraZoom;
	f32 playerAngle;
};

// Assumption: All map squares are exactly 1.0 unit in size.
// Player is modelled as a unit sphere (with radius 0.5).
class Level : public IUpdatable, public IWantInput, public IWantEvents
{
	IEngine *engine;
	IWorld *world;
	IRenderSystem *renderSystem;
	
	IThirdPersonCameraController *thirdPersonCamera;
	
	video::ITexture *cloudShadowTexture;
	
	// Main state. Used to trigger level restart.
	// (and destruction of this Level instance)
	MainState *mainState;
	
	// may differ from global, e.g. if we are in final scene.
	bool localGridBasedMovement;
	
	// On-disk name of this level file.
	core::stringc fileName;
	
	std::deque<UndoState> undoHistory;
	f32 timeSinceLastUndoSave;
	
	// should optimisations be performed?
	// not useful when in editor, as may want to remove objects individually.
	bool combineMeshes;
	scene::IMeshSceneNode *combinedLevelMesh;
	std::vector<IShader *> combinedLevelMeshShaders;
	
	// for delayed player push
	core::vector3di pushStartLoc;
	core::vector3di pushEndLoc;
	
	LevelStats stats;
	
	std::vector<TutorialText> tutorialTexts;
	std::vector<gui::IGUIElement *> tutorialTextElements;
	
	IEventQueue *tutorialEventQueue;
	
	void ClearTutorialTextElements()
	{
		for (u32 i = 0; i < tutorialTextElements.size(); i ++)
			tutorialTextElements[i]->remove();
		
		tutorialTextElements.clear();
	}
	
	std::vector<IMesh *> endLevelPlayerEffects;
	
	// Should OnEnterLocation logic be processed?
	// This is disabled in Editor.
	bool logicEnabled;
	
	// is level ending?
	bool touchedEndLevelPortal;
	f32 portalTouchedTime;
	
	Map *map;
	
	std::vector<IMapEventOwner *> mapEventInstances;
	
	std::vector<MapObjectMove> mapObjectMoves;
	
	std::vector<Actor> actors;
	
	std::vector<Event> playerPushEvent;
	
	ICharacterController *playerController;
	
	// some special behaviour for the player collisions
	IInteractionPreparator *playerInteractionPreparator;
	
	// randomly added irrlicht nodes that should be removed on destruction
	std::vector<scene::ISceneNode *> irrNodes;
	
	// List of locations with objects newly created or moved in to.
	// (events need to be called generated in Update)
	std::vector<core::vector3di> locationsEntered;
	
	// as soon as move starts (locationsEntered is when move has finished)
	std::vector<core::vector3di> locationsStartedToEnter;
	
	// opposite of the above
	std::vector<core::vector3di> locationsLeft;
	std::vector<core::vector3di> locationsLeftNew;//new location of each of those left
	
	// A default event called for each map location entered (as well as custom events)
	DefaultEvent *defaultEvent;
	
	
	// Utility sound
	// used for various sounds that are only ever likely to be played one at
	// a time. e.g. the block push sound.
	ISound3D *sound;
	
	
	// Set true when in final level. A hack.
	// so we get footstep sounds when walking on sand (and there are no blocks underneath)
	bool alwaysFootsteps;
	
	
	// used for detecting when to change type of footstep sound
	E_OBJECT_TYPE lastObjectBelowPlayer;
	
	
	bool isBoundingBoxEmpty;
	core::aabbox3df boundingBox;
	
	bool isPlayerPushing;
	
	// Y coord that is lower than all objects and events
	f32 lowestPoint;
	
	void ExpandBB(core::vector3di coord);
	
	void SaveUndoState(const UndoState &state);
	UndoState CreateUndoState();
	void ApplyUndo(bool died = false);
	bool ExistsUndo() { return !undoHistory.empty(); }
	
	// when player falls off world, camera may get moved
	// due to world being in line of sight.
	// this should only occur once.
	bool fallCameraMoved;
	
	core::vector3df levelCentrePoint;
	
public:
	
	void ForceFootsteps(bool alwaysEnabled)
	{
		alwaysFootsteps = alwaysEnabled;
	}
	
	core::vector3df GetCentrePoint() { return levelCentrePoint; }
	
	// get map coordinate from a 3D position
	core::vector3di GetCoord(core::vector3df pos);
	
	// get coordinate that is at the Actor's centre
	core::vector3di GetActorCoord(Actor &c) { return GetCoord( c.entity->GetPosition() ); }
	
	// Player is always first Actor added in constructor.
	Actor &GetPlayerActor() { ASSERT(actors.size()); return actors[0]; }
	
	// get 3D position from map coordinate
	core::vector3df GetPosFromCoord(core::vector3di mapCoord);
	
	// is actor touching (however slightly) a particular map coordinate?
	bool IsActorTouchingCoord(Actor &actor, core::vector3di mapCoord, f32 scale = 1.0);
	
	// get list of map coordinates intersected (however slightly) by an Actor
	// *excludes* the coordinate at the centre of the Actor
	std::vector<core::vector3di> GetActorIntersectingCoords(Actor &actor);
	
	void PutDebugCube(core::vector3di mapCoord);
	
	// Apply the default environment shaders to a mesh.
	void ApplyDefaultShaders(IMesh *mesh, video::E_MATERIAL_TYPE baseMaterial = video::EMT_SOLID);
	void ApplyDefaultShadersIrr(scene::IMesh *mesh, video::E_MATERIAL_TYPE baseMaterial = video::EMT_SOLID);
	void ApplyLandShaders(IMesh *mesh, video::E_MATERIAL_TYPE baseMaterial = video::EMT_SOLID);
	//void ApplyCliffGrassShaders(IMesh *mesh);
	void ApplyWoodShaders(IMesh *mesh);
	void ApplyIceShaders(IMesh *mesh);
	void ApplyBalloonShaders(IMesh *mesh);
	void ApplyPlainAlphaShaders(IMesh *mesh, u8 alpha);
	
	void AddActor(core::vector3di mapCoord, ICharacter *entity);
	void CreatePlayer(core::vector3di mapCoord);
	//void CreateMonster(core::vector3di mapCoord);
	
	void SetPlayerAlpha(u8 alpha);
	void SetMeshAlpha(IMesh *mesh, u8 alpha);
	void SetMeshColour(IMesh *mesh, video::SColor col);
	
	IMesh *AddObject( core::vector3di mapCoord, const c8 *meshName, bool movable, E_OBJECT_TYPE type, bool solid = true );
	
	void GroundBlockUV_Adjust(IMesh *mesh, core::vector3di mapCoord);
	void GroundBlockUV_Adjust2(IMesh *mesh, core::vector3di mapCoord);
	
	// Called for each ground block to perhaps add flowers and other decoration.
	void MaybeAddFlower(core::vector3df absPos);
	
	// this also adds some level effects like the fan vortex particle system
	void OptimiseLevel();
	
	void AddFanParticleSystem(core::vector3di mapCoord, f32 height);
	
	void CreateObject(core::vector3di mapCoord, E_OBJECT_TYPE type);
	void CreateEvent(core::vector3di mapCoord, E_EVENT_TYPE type);
	
	void RemoveObject(core::vector3di mapCoord);
	
	// Called by Update when the player performs a push action towards a map location
	// Note that this only includes a direct push, and will NOT be called if the player
	// touches a corner of the map coordinate.
	void PlayerPushed(core::vector3di mapCoord, core::vector3di pushVec);
	
	// Called when an object enters a location on the map.
	// (called once on initialisation of each object and also after each
	// time an object moves into a new location).
	void OnEnterLocation(core::vector3di coord);
	
	// Called as soon as an object starts moving. (unlike above which waits til the move finishes)
	// Also unlike above, this is NOT called on game start. Only called when an object has actually moved.
	// coord is the destination.
	void OnStartMove(core::vector3di coord);
	
	// Called when an object leaves a particular location.
	void OnLeaveLocation(core::vector3di coord, core::vector3di newCoord);
	
	// apply grid based gravity (StartMapObjectMove downwards)
	void PerhapsGravity(core::vector3di coord);
	
//public:
	
	// MainState pointer is now optional (can be NULL), since we may create a level without
	// mainstate existing for preview in start screen.
	Level(MainState *mainState, core::stringc fileName, std::deque<UndoState> *undoHistory = NULL);
	~Level();
	
	// used by options menu... for sfx testing...
	ISound3D *GetUtilSound() { return sound; }
	
	// Actually gives control to player among other things.
	void Start();
	
	// hacked in for startscreen preview
	// replaces current level with a new one,
	// instantly without any fades or anything.
	void ReplaceWith(core::stringc fileName);
	
	// Is the level ending? (i.e. fade off and other effects in progress)
	bool IsEnding() { return touchedEndLevelPortal; }
	
	core::stringc GetFileName() { return fileName; }
	core::stringc GetShortName() { return os::path::basename(fileName); }
	
	void SetLogicEnabled(bool logicEnabled) { this->logicEnabled = logicEnabled; }
	
	void ClearEndLevelTeleportEffects();
	
	void OnButtonDown(s32 id);
	void OnEvent(const Event &event);
	
	// hacked in.
	void ShowEndLevelScreen();
	
	void StartNextLevelTransition(bool stopMoving = true);
	void TouchedEndLevelPortal(ITransformable *portal);
	
	Map *GetMap() { return map; }
	
	void Save();
	void Load(UndoState *undoState = NULL);
	
	ICharacter *GetPlayer() { return GetPlayerActor().GetCharacter(); }
	IThirdPersonCameraController *GetCamera() { return thirdPersonCamera; }
	
	// Starts an animated move of an object from one location to another, over time
	// Moves are updated and finished when necessary in Update
	bool StartMapObjectMove(core::vector3di source, core::vector3di dest, bool forceMove = false);
	
	// called by OnEvent
	// handles events related to tutorial text display
	void HandleTutorialEvents(const Event &event);
	
	// called by lift event to signal a move
	void LiftMoveInc() { stats.elevatorMoves ++; }
	const LevelStats &GetLevelStats();
	
	// Called at a certain FPS as defined in Engine.
	void Update(f32 dt);
	
	// There are hacks, and then there are *hacks*. This is in the latter category.
	// Used by some events/moving objects to push the player out of the way.
	// Only works on player intersecting squares, the actual centre location occupied by the player
	// cannot be moved.
	// Basically, this just removes the player's claim to this location. Collisions and
	// physics are expected to do the rest.
	void ShovePlayerOutOfTheWay(core::vector3di coord);
	
	// hack.
	// immediately finishes the player's current move.
	// (presumably a move due to gravity or suchlike)
	void ClearPlayerMapObjectMove();
	
private:
	void OnPause();
	void OnResume();
};

#endif

