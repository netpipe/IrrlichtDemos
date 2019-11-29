/*
** Lua binding: peak
** Generated automatically by tolua++-1.0.92 on Mon Jun  1 16:36:31 2009.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_peak_open (lua_State* tolua_S);

#include "engine/include/core/Buffer.h"
#include "engine/include/core/Address.h"
#include "engine/include/core/Vector3D.h"
#include "engine/include/core/Vector2D.h"
#include "engine/include/core/Vector2DI.h"
#include "engine/include/core/Color.h"
#include "engine/include/core/Quaternion.h"
#include "engine/include/core/Script.h"
#include "engine/include/core/Game.h"
#include "engine/include/core/Logger.h"
#include "engine/include/core/InputManager.h"
#include "engine/include/core/Keycode.h"
#include "engine/include/core/FileSystem.h"
#include "engine/include/core/File.h"
#include "engine/include/core/Timer.h"
#include "engine/include/core/Util.h"
#include "engine/include/core/Level.h"
#include "engine/include/core/SettingsManager.h"
#include "engine/include/network/Connection.h"
#include "engine/include/network/BroadcastHost.h"
#include "engine/include/network/BroadcastClient.h"
#include "engine/include/graphic/SceneNode.h"
#include "engine/include/graphic/CubeSceneNode.h"
#include "engine/include/graphic/CameraSceneNode.h"
#include "engine/include/graphic/GraphicsEngine.h"
#include "engine/include/graphic/Image.h"
#include "engine/include/graphic/MeshSceneNode.h"
#include "engine/include/graphic/Material.h"
#include "engine/include/graphic/Overlay.h"
#include "engine/include/graphic/RectangleOverlay.h"
#include "engine/include/graphic/TextureOverlay.h"
#include "engine/include/graphic/LabelOverlay.h"
#include "engine/include/graphic/OverlayManager.h"
#include "engine/include/graphic/Terrain.h"
#include "engine/include/graphic/SkyBox.h"
#include "engine/include/graphic/SkyDome.h"
#include "engine/include/graphic/SceneNodeController.h"
#include "engine/include/graphic/FollowingController.h"
#include "engine/include/graphic/RotationController.h"
#include "engine/include/physics/World.h"
#include "engine/include/physics/Body.h"
#include "engine/include/physics/Plane.h"
#include "engine/include/physics/Cube.h"
#include "engine/include/physics/Cylinder.h"
#include "engine/include/physics/Geometry.h"
#include "engine/include/physics/Joint.h"
#include "engine/include/physics/Hinge.h"
#include "engine/include/physics/Ray.h"
#include "engine/include/physics/Collision.h"
#include "engine/include/entity/Entity.h"
#include "engine/include/entity/Component.h"
#include "engine/include/entity/PhysicsComponent.h"
#include "engine/include/entity/GraphicsComponent.h"
#include "engine/include/entity/ServerScriptComponent.h"
#include "engine/include/entity/EntityVariables.h"
#include "engine/include/menu/MenuManager.h"
#include "engine/include/menu/Menu.h"
#include "engine/include/menu/MenuElement.h"
#include "engine/include/menu/ListBox.h"
#include "engine/include/sound/SoundEngine.h"
#include "engine/include/sound/Sound.h"
#include "engine/include/sound/Listener.h"
#include "engine/include/sound/SoundSource.h"
#include "engine/include/sound/SoundFactory.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_peak__FileSystem (lua_State* tolua_S)
{
 peak::FileSystem* self = (peak::FileSystem*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_std__wstring (lua_State* tolua_S)
{
 std::wstring* self = (std::wstring*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__Body (lua_State* tolua_S)
{
 peak::Body* self = (peak::Body*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__Connection (lua_State* tolua_S)
{
 peak::Connection* self = (peak::Connection*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__Parameter (lua_State* tolua_S)
{
 peak::Parameter* self = (peak::Parameter*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__CameraSceneNode (lua_State* tolua_S)
{
 peak::CameraSceneNode* self = (peak::CameraSceneNode*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_std__vector_std__string_ (lua_State* tolua_S)
{
 std::vector<std::string>* self = (std::vector<std::string>*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__Level (lua_State* tolua_S)
{
 peak::Level* self = (peak::Level*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__Address (lua_State* tolua_S)
{
 peak::Address* self = (peak::Address*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__Entity (lua_State* tolua_S)
{
 peak::Entity* self = (peak::Entity*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__Variable (lua_State* tolua_S)
{
 peak::Variable* self = (peak::Variable*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__Menu (lua_State* tolua_S)
{
 peak::Menu* self = (peak::Menu*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__ContactInfo (lua_State* tolua_S)
{
 peak::ContactInfo* self = (peak::ContactInfo*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_dBodyID (lua_State* tolua_S)
{
 dBodyID* self = (dBodyID*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__Geometry (lua_State* tolua_S)
{
 peak::Geometry* self = (peak::Geometry*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__OverlayManager (lua_State* tolua_S)
{
 peak::OverlayManager* self = (peak::OverlayManager*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__Buffer (lua_State* tolua_S)
{
 peak::Buffer* self = (peak::Buffer*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__MeshSceneNode (lua_State* tolua_S)
{
 peak::MeshSceneNode* self = (peak::MeshSceneNode*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__EntityVariables (lua_State* tolua_S)
{
 peak::EntityVariables* self = (peak::EntityVariables*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__Vector3D (lua_State* tolua_S)
{
 peak::Vector3D* self = (peak::Vector3D*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__Color (lua_State* tolua_S)
{
 peak::Color* self = (peak::Color*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__BroadcastHost (lua_State* tolua_S)
{
 peak::BroadcastHost* self = (peak::BroadcastHost*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__ParameterList (lua_State* tolua_S)
{
 peak::ParameterList* self = (peak::ParameterList*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__Vector2DI (lua_State* tolua_S)
{
 peak::Vector2DI* self = (peak::Vector2DI*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__File (lua_State* tolua_S)
{
 peak::File* self = (peak::File*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__FollowingController (lua_State* tolua_S)
{
 peak::FollowingController* self = (peak::FollowingController*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__LevelObject (lua_State* tolua_S)
{
 peak::LevelObject* self = (peak::LevelObject*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__Quaternion (lua_State* tolua_S)
{
 peak::Quaternion* self = (peak::Quaternion*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__TerrainObject (lua_State* tolua_S)
{
 peak::TerrainObject* self = (peak::TerrainObject*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__PhysicsComponent (lua_State* tolua_S)
{
 peak::PhysicsComponent* self = (peak::PhysicsComponent*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__CubeSceneNode (lua_State* tolua_S)
{
 peak::CubeSceneNode* self = (peak::CubeSceneNode*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__Image (lua_State* tolua_S)
{
 peak::Image* self = (peak::Image*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__SoundSource (lua_State* tolua_S)
{
 peak::SoundSource* self = (peak::SoundSource*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__Ray (lua_State* tolua_S)
{
 peak::Ray* self = (peak::Ray*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__SoundEngine (lua_State* tolua_S)
{
 peak::SoundEngine* self = (peak::SoundEngine*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__SoundFactory (lua_State* tolua_S)
{
 peak::SoundFactory* self = (peak::SoundFactory*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__Logger (lua_State* tolua_S)
{
 peak::Logger* self = (peak::Logger*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_dSpaceID (lua_State* tolua_S)
{
 dSpaceID* self = (dSpaceID*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__Listener (lua_State* tolua_S)
{
 peak::Listener* self = (peak::Listener*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__ServerScriptComponent (lua_State* tolua_S)
{
 peak::ServerScriptComponent* self = (peak::ServerScriptComponent*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__Sound (lua_State* tolua_S)
{
 peak::Sound* self = (peak::Sound*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_dGeomID (lua_State* tolua_S)
{
 dGeomID* self = (dGeomID*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__PointGroupObject (lua_State* tolua_S)
{
 peak::PointGroupObject* self = (peak::PointGroupObject*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__Terrain (lua_State* tolua_S)
{
 peak::Terrain* self = (peak::Terrain*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__Overlay (lua_State* tolua_S)
{
 peak::Overlay* self = (peak::Overlay*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__ListBox (lua_State* tolua_S)
{
 peak::ListBox* self = (peak::ListBox*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__SkyBox (lua_State* tolua_S)
{
 peak::SkyBox* self = (peak::SkyBox*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__Vector2D (lua_State* tolua_S)
{
 peak::Vector2D* self = (peak::Vector2D*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__MenuElement (lua_State* tolua_S)
{
 peak::MenuElement* self = (peak::MenuElement*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__MenuManager (lua_State* tolua_S)
{
 peak::MenuManager* self = (peak::MenuManager*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__GraphicsComponent (lua_State* tolua_S)
{
 peak::GraphicsComponent* self = (peak::GraphicsComponent*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__Script (lua_State* tolua_S)
{
 peak::Script* self = (peak::Script*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__RotationController (lua_State* tolua_S)
{
 peak::RotationController* self = (peak::RotationController*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__Hinge (lua_State* tolua_S)
{
 peak::Hinge* self = (peak::Hinge*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__Joint (lua_State* tolua_S)
{
 peak::Joint* self = (peak::Joint*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__SkyDome (lua_State* tolua_S)
{
 peak::SkyDome* self = (peak::SkyDome*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__Game (lua_State* tolua_S)
{
 peak::Game* self = (peak::Game*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__World (lua_State* tolua_S)
{
 peak::World* self = (peak::World*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__SceneNode (lua_State* tolua_S)
{
 peak::SceneNode* self = (peak::SceneNode*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__SceneNodeController (lua_State* tolua_S)
{
 peak::SceneNodeController* self = (peak::SceneNodeController*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__Cube (lua_State* tolua_S)
{
 peak::Cube* self = (peak::Cube*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_dWorldID (lua_State* tolua_S)
{
 dWorldID* self = (dWorldID*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__TextureOverlay (lua_State* tolua_S)
{
 peak::TextureOverlay* self = (peak::TextureOverlay*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__Timer (lua_State* tolua_S)
{
 peak::Timer* self = (peak::Timer*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__Plane (lua_State* tolua_S)
{
 peak::Plane* self = (peak::Plane*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__Component (lua_State* tolua_S)
{
 peak::Component* self = (peak::Component*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_dJointGroupID (lua_State* tolua_S)
{
 dJointGroupID* self = (dJointGroupID*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__SceneNodeObject (lua_State* tolua_S)
{
 peak::SceneNodeObject* self = (peak::SceneNodeObject*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__LabelOverlay (lua_State* tolua_S)
{
 peak::LabelOverlay* self = (peak::LabelOverlay*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__RectangleOverlay (lua_State* tolua_S)
{
 peak::RectangleOverlay* self = (peak::RectangleOverlay*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__Material (lua_State* tolua_S)
{
 peak::Material* self = (peak::Material*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__BroadcastClient (lua_State* tolua_S)
{
 peak::BroadcastClient* self = (peak::BroadcastClient*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_peak__Cylinder (lua_State* tolua_S)
{
 peak::Cylinder* self = (peak::Cylinder*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"ReferenceCounted");
 tolua_usertype(tolua_S,"peak::FileSystem");
 tolua_usertype(tolua_S,"std::vector<peak::ContactInfo>");
 tolua_usertype(tolua_S,"std::wstring");
 tolua_usertype(tolua_S,"std::vector<peak::Contact>");
 tolua_usertype(tolua_S,"peak::Body");
 tolua_usertype(tolua_S,"ENetPeer");
 tolua_usertype(tolua_S,"peak::Parameter");
 tolua_usertype(tolua_S,"peak::CameraSceneNode");
 tolua_usertype(tolua_S,"peak::InputManager");
 tolua_usertype(tolua_S,"std::vector<std::string>");
 tolua_usertype(tolua_S,"peak::ClientScriptComponent");
 tolua_usertype(tolua_S,"peak::TimerCallback");
 tolua_usertype(tolua_S,"peak::Level");
 tolua_usertype(tolua_S,"peak::Heightfield");
 tolua_usertype(tolua_S,"peak::Address");
 tolua_usertype(tolua_S,"peak::Entity");
 tolua_usertype(tolua_S,"peak::Variable");
 tolua_usertype(tolua_S,"peak::ServerObject");
 tolua_usertype(tolua_S,"peak::Menu");
 tolua_usertype(tolua_S,"peak::RectangleOverlay");
 tolua_usertype(tolua_S,"dBodyID");
 tolua_usertype(tolua_S,"peak::Geometry");
 tolua_usertype(tolua_S,"peak::OverlayManager");
 tolua_usertype(tolua_S,"peak::Buffer");
 tolua_usertype(tolua_S,"peak::MeshSceneNode");
 tolua_usertype(tolua_S,"peak::EntityVariables");
 tolua_usertype(tolua_S,"peak::Vector3D");
 tolua_usertype(tolua_S,"peak::Color");
 tolua_usertype(tolua_S,"peak::Collision");
 tolua_usertype(tolua_S,"peak::SoundEngine");
 tolua_usertype(tolua_S,"peak::SoundSource");
 tolua_usertype(tolua_S,"peak::BroadcastHost");
 tolua_usertype(tolua_S,"peak::ParameterList");
 tolua_usertype(tolua_S,"peak::Label");
 tolua_usertype(tolua_S,"peak::CollisionInfo");
 tolua_usertype(tolua_S,"peak::Vector2DI");
 tolua_usertype(tolua_S,"peak::Sound");
 tolua_usertype(tolua_S,"peak::ListBox");
 tolua_usertype(tolua_S,"peak::Logger");
 tolua_usertype(tolua_S,"peak::MenuElement");
 tolua_usertype(tolua_S,"peak::File");
 tolua_usertype(tolua_S,"peak::MenuManager");
 tolua_usertype(tolua_S,"peak::IniFile");
 tolua_usertype(tolua_S,"peak::FollowingController");
 tolua_usertype(tolua_S,"peak::LevelObject");
 tolua_usertype(tolua_S,"peak::Quaternion");
 tolua_usertype(tolua_S,"peak::TerrainObject");
 tolua_usertype(tolua_S,"peak::PhysicsComponent");
 tolua_usertype(tolua_S,"peak::CubeSceneNode");
 tolua_usertype(tolua_S,"peak::SceneNodeData");
 tolua_usertype(tolua_S,"peak::Component");
 tolua_usertype(tolua_S,"peak::Image");
 tolua_usertype(tolua_S,"peak::Hinge");
 tolua_usertype(tolua_S,"peak::Ray");
 tolua_usertype(tolua_S,"peak::GraphicsComponent");
 tolua_usertype(tolua_S,"peak::SoundFactory");
 tolua_usertype(tolua_S,"peak::SettingsManager");
 tolua_usertype(tolua_S,"dSpaceID");
 tolua_usertype(tolua_S,"peak::Listener");
 tolua_usertype(tolua_S,"peak::ServerScriptComponent");
 tolua_usertype(tolua_S,"peak::Joint");
 tolua_usertype(tolua_S,"dGeomID");
 tolua_usertype(tolua_S,"peak::PointGroupObject");
 tolua_usertype(tolua_S,"peak::Terrain");
 tolua_usertype(tolua_S,"peak::Overlay");
 tolua_usertype(tolua_S,"peak::Contact");
 tolua_usertype(tolua_S,"peak::SkyBox");
 tolua_usertype(tolua_S,"peak::Vector2D");
 tolua_usertype(tolua_S,"peak::GraphicsEngine");
 tolua_usertype(tolua_S,"peak::SceneNodeObject");
 tolua_usertype(tolua_S,"dWorldID");
 tolua_usertype(tolua_S,"peak::Script");
 tolua_usertype(tolua_S,"peak::RotationController");
 tolua_usertype(tolua_S,"peak::Cube");
 tolua_usertype(tolua_S,"peak::NetworkHost");
 tolua_usertype(tolua_S,"peak::SkyDome");
 tolua_usertype(tolua_S,"peak::Game");
 tolua_usertype(tolua_S,"peak::ContactInfo");
 tolua_usertype(tolua_S,"peak::SceneNode");
 tolua_usertype(tolua_S,"peak::SceneNodeController");
 tolua_usertype(tolua_S,"peak::BroadcastClient");
 tolua_usertype(tolua_S,"peak::World");
 tolua_usertype(tolua_S,"peak::Connection");
 tolua_usertype(tolua_S,"peak::Timer");
 tolua_usertype(tolua_S,"peak::Plane");
 tolua_usertype(tolua_S,"peak::CollisionCallback");
 tolua_usertype(tolua_S,"dJointGroupID");
 tolua_usertype(tolua_S,"peak::Button");
 tolua_usertype(tolua_S,"peak::LabelOverlay");
 tolua_usertype(tolua_S,"peak::TextureOverlay");
 tolua_usertype(tolua_S,"peak::Material");
 tolua_usertype(tolua_S,"peak::NetworkClient");
 tolua_usertype(tolua_S,"peak::Cylinder");
}

/* method: new of class  peak::Buffer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Buffer_new00
static int tolua_peak_peak_Buffer_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Buffer* tolua_ret = (peak::Buffer*)  new peak::Buffer();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Buffer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Buffer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Buffer_new00_local
static int tolua_peak_peak_Buffer_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Buffer* tolua_ret = (peak::Buffer*)  new peak::Buffer();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Buffer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Buffer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Buffer_new01
static int tolua_peak_peak_Buffer_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  unsigned int size = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  {
   peak::Buffer* tolua_ret = (peak::Buffer*)  new peak::Buffer(size);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Buffer");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Buffer_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Buffer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Buffer_new01_local
static int tolua_peak_peak_Buffer_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  unsigned int size = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  {
   peak::Buffer* tolua_ret = (peak::Buffer*)  new peak::Buffer(size);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Buffer");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Buffer_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Buffer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Buffer_new02
static int tolua_peak_peak_Buffer_new02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Buffer",0,&tolua_err) ||
     !tolua_isuserdata(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  void* data = ((void*)  tolua_touserdata(tolua_S,2,0));
  unsigned int size = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
  {
   peak::Buffer* tolua_ret = (peak::Buffer*)  new peak::Buffer(data,size);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Buffer");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Buffer_new01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Buffer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Buffer_new02_local
static int tolua_peak_peak_Buffer_new02_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Buffer",0,&tolua_err) ||
     !tolua_isuserdata(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  void* data = ((void*)  tolua_touserdata(tolua_S,2,0));
  unsigned int size = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
  {
   peak::Buffer* tolua_ret = (peak::Buffer*)  new peak::Buffer(data,size);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Buffer");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Buffer_new01_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Buffer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Buffer_new03
static int tolua_peak_peak_Buffer_new03(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Buffer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const peak::Buffer* buf = ((const peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
  {
   peak::Buffer* tolua_ret = (peak::Buffer*)  new peak::Buffer(*buf);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Buffer");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Buffer_new02(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Buffer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Buffer_new03_local
static int tolua_peak_peak_Buffer_new03_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Buffer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const peak::Buffer* buf = ((const peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
  {
   peak::Buffer* tolua_ret = (peak::Buffer*)  new peak::Buffer(*buf);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Buffer");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Buffer_new02_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::Buffer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Buffer_delete00
static int tolua_peak_peak_Buffer_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Buffer* self = (peak::Buffer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setSize of class  peak::Buffer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Buffer_setSize00
static int tolua_peak_peak_Buffer_setSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Buffer* self = (peak::Buffer*)  tolua_tousertype(tolua_S,1,0);
  unsigned int size = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setSize'",NULL);
#endif
  {
   self->setSize(size);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSize of class  peak::Buffer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Buffer_getSize00
static int tolua_peak_peak_Buffer_getSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Buffer* self = (peak::Buffer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSize'",NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->getSize();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getDataSize of class  peak::Buffer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Buffer_getDataSize00
static int tolua_peak_peak_Buffer_getDataSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Buffer* self = (peak::Buffer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDataSize'",NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->getDataSize();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getDataSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPosition of class  peak::Buffer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Buffer_getPosition00
static int tolua_peak_peak_Buffer_getPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Buffer* self = (peak::Buffer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPosition'",NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->getPosition();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setPosition of class  peak::Buffer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Buffer_setPosition00
static int tolua_peak_peak_Buffer_setPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Buffer* self = (peak::Buffer*)  tolua_tousertype(tolua_S,1,0);
  unsigned int position = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  bool relative = ((bool)  tolua_toboolean(tolua_S,3,false));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPosition'",NULL);
#endif
  {
   self->setPosition(position,relative);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: readData of class  peak::Buffer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Buffer_readData00
static int tolua_peak_peak_Buffer_readData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Buffer",0,&tolua_err) ||
     !tolua_isuserdata(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Buffer* self = (peak::Buffer*)  tolua_tousertype(tolua_S,1,0);
  void* dest = ((void*)  tolua_touserdata(tolua_S,2,0));
  unsigned int size = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readData'",NULL);
#endif
  {
   int tolua_ret = (int)  self->readData(dest,size);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'readData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: writeData of class  peak::Buffer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Buffer_writeData00
static int tolua_peak_peak_Buffer_writeData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Buffer",0,&tolua_err) ||
     !tolua_isuserdata(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Buffer* self = (peak::Buffer*)  tolua_tousertype(tolua_S,1,0);
  const void* source = ((const void*)  tolua_touserdata(tolua_S,2,0));
  unsigned int size = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
  bool resize = ((bool)  tolua_toboolean(tolua_S,4,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeData'",NULL);
#endif
  {
   int tolua_ret = (int)  self->writeData(source,size,resize);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'writeData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: writeInt of class  peak::Buffer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Buffer_writeInt00
static int tolua_peak_peak_Buffer_writeInt00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Buffer* self = (peak::Buffer*)  tolua_tousertype(tolua_S,1,0);
  int value = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeInt'",NULL);
#endif
  {
   self->writeInt(value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'writeInt'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: readInt of class  peak::Buffer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Buffer_readInt00
static int tolua_peak_peak_Buffer_readInt00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Buffer* self = (peak::Buffer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readInt'",NULL);
#endif
  {
   int tolua_ret = (int)  self->readInt();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'readInt'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: writeFloat of class  peak::Buffer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Buffer_writeFloat00
static int tolua_peak_peak_Buffer_writeFloat00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Buffer* self = (peak::Buffer*)  tolua_tousertype(tolua_S,1,0);
  float value = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeFloat'",NULL);
#endif
  {
   self->writeFloat(value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'writeFloat'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: readFloat of class  peak::Buffer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Buffer_readFloat00
static int tolua_peak_peak_Buffer_readFloat00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Buffer* self = (peak::Buffer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readFloat'",NULL);
#endif
  {
   float tolua_ret = (float)  self->readFloat();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'readFloat'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: writeString of class  peak::Buffer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Buffer_writeString00
static int tolua_peak_peak_Buffer_writeString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Buffer",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Buffer* self = (peak::Buffer*)  tolua_tousertype(tolua_S,1,0);
  std::string str = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeString'",NULL);
#endif
  {
   self->writeString(str);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'writeString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: readString of class  peak::Buffer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Buffer_readString00
static int tolua_peak_peak_Buffer_readString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Buffer* self = (peak::Buffer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readString'",NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->readString();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'readString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: writeByte of class  peak::Buffer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Buffer_writeByte00
static int tolua_peak_peak_Buffer_writeByte00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Buffer* self = (peak::Buffer*)  tolua_tousertype(tolua_S,1,0);
  unsigned char value = ((unsigned char)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeByte'",NULL);
#endif
  {
   self->writeByte(value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'writeByte'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: readByte of class  peak::Buffer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Buffer_readByte00
static int tolua_peak_peak_Buffer_readByte00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Buffer* self = (peak::Buffer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readByte'",NULL);
#endif
  {
   unsigned char tolua_ret = (unsigned char)  self->readByte();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'readByte'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: writeWord of class  peak::Buffer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Buffer_writeWord00
static int tolua_peak_peak_Buffer_writeWord00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Buffer* self = (peak::Buffer*)  tolua_tousertype(tolua_S,1,0);
  short value = ((short)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeWord'",NULL);
#endif
  {
   self->writeWord(value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'writeWord'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: readWord of class  peak::Buffer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Buffer_readWord00
static int tolua_peak_peak_Buffer_readWord00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Buffer* self = (peak::Buffer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readWord'",NULL);
#endif
  {
   short tolua_ret = (short)  self->readWord();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'readWord'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: writeBool of class  peak::Buffer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Buffer_writeBool00
static int tolua_peak_peak_Buffer_writeBool00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Buffer",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Buffer* self = (peak::Buffer*)  tolua_tousertype(tolua_S,1,0);
  bool flag = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeBool'",NULL);
#endif
  {
   self->writeBool(flag);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'writeBool'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: readBool of class  peak::Buffer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Buffer_readBool00
static int tolua_peak_peak_Buffer_readBool00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Buffer* self = (peak::Buffer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readBool'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->readBool();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'readBool'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: writeVector2D of class  peak::Buffer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Buffer_writeVector2D00
static int tolua_peak_peak_Buffer_writeVector2D00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Buffer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Vector2D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Buffer* self = (peak::Buffer*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector2D v = *((peak::Vector2D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeVector2D'",NULL);
#endif
  {
   self->writeVector2D(v);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'writeVector2D'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: readVector2D of class  peak::Buffer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Buffer_readVector2D00
static int tolua_peak_peak_Buffer_readVector2D00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Buffer* self = (peak::Buffer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readVector2D'",NULL);
#endif
  {
   peak::Vector2D tolua_ret = (peak::Vector2D)  self->readVector2D();
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Vector2D(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector2D");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector2D));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector2D");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'readVector2D'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: writeVector3D of class  peak::Buffer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Buffer_writeVector3D00
static int tolua_peak_peak_Buffer_writeVector3D00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Buffer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Buffer* self = (peak::Buffer*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D v = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeVector3D'",NULL);
#endif
  {
   self->writeVector3D(v);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'writeVector3D'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: readVector3D of class  peak::Buffer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Buffer_readVector3D00
static int tolua_peak_peak_Buffer_readVector3D00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Buffer* self = (peak::Buffer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readVector3D'",NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->readVector3D();
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Vector3D(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'readVector3D'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: writeQuaternion of class  peak::Buffer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Buffer_writeQuaternion00
static int tolua_peak_peak_Buffer_writeQuaternion00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Buffer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Buffer* self = (peak::Buffer*)  tolua_tousertype(tolua_S,1,0);
  peak::Quaternion q = *((peak::Quaternion*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeQuaternion'",NULL);
#endif
  {
   self->writeQuaternion(q);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'writeQuaternion'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: readQuaternion of class  peak::Buffer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Buffer_readQuaternion00
static int tolua_peak_peak_Buffer_readQuaternion00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Buffer* self = (peak::Buffer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readQuaternion'",NULL);
#endif
  {
   peak::Quaternion tolua_ret = (peak::Quaternion)  self->readQuaternion();
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Quaternion(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Quaternion");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Quaternion));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Quaternion");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'readQuaternion'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getData of class  peak::Buffer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Buffer_getData00
static int tolua_peak_peak_Buffer_getData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Buffer* self = (peak::Buffer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getData'",NULL);
#endif
  {
   char* tolua_ret = (char*)  self->getData();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clear of class  peak::Buffer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Buffer_clear00
static int tolua_peak_peak_Buffer_clear00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Buffer* self = (peak::Buffer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clear'",NULL);
#endif
  {
   self->clear();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clear'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Address */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Address_new00
static int tolua_peak_peak_Address_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Address",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Address* tolua_ret = (peak::Address*)  new peak::Address();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Address");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Address */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Address_new00_local
static int tolua_peak_peak_Address_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Address",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Address* tolua_ret = (peak::Address*)  new peak::Address();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Address");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Address */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Address_new01
static int tolua_peak_peak_Address_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Address",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const peak::Address",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const peak::Address* addr = ((const peak::Address*)  tolua_tousertype(tolua_S,2,0));
  {
   peak::Address* tolua_ret = (peak::Address*)  new peak::Address(*addr);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Address");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Address_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Address */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Address_new01_local
static int tolua_peak_peak_Address_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Address",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const peak::Address",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const peak::Address* addr = ((const peak::Address*)  tolua_tousertype(tolua_S,2,0));
  {
   peak::Address* tolua_ret = (peak::Address*)  new peak::Address(*addr);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Address");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Address_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Address */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Address_new02
static int tolua_peak_peak_Address_new02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Address",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  std::string addr = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  {
   peak::Address* tolua_ret = (peak::Address*)  new peak::Address(addr);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Address");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Address_new01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Address */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Address_new02_local
static int tolua_peak_peak_Address_new02_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Address",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  std::string addr = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  {
   peak::Address* tolua_ret = (peak::Address*)  new peak::Address(addr);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Address");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Address_new01_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Address */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Address_new03
static int tolua_peak_peak_Address_new03(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Address",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const char* addr = ((const char*)  tolua_tostring(tolua_S,2,0));
  {
   peak::Address* tolua_ret = (peak::Address*)  new peak::Address(addr);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Address");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Address_new02(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Address */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Address_new03_local
static int tolua_peak_peak_Address_new03_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Address",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const char* addr = ((const char*)  tolua_tostring(tolua_S,2,0));
  {
   peak::Address* tolua_ret = (peak::Address*)  new peak::Address(addr);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Address");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Address_new02_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Address */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Address_new04
static int tolua_peak_peak_Address_new04(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Address",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  unsigned char ip1 = ((unsigned char)  tolua_tonumber(tolua_S,2,0));
  unsigned char ip2 = ((unsigned char)  tolua_tonumber(tolua_S,3,0));
  unsigned char ip3 = ((unsigned char)  tolua_tonumber(tolua_S,4,0));
  unsigned char ip4 = ((unsigned char)  tolua_tonumber(tolua_S,5,0));
  unsigned short port = ((unsigned short)  tolua_tonumber(tolua_S,6,0));
  {
   peak::Address* tolua_ret = (peak::Address*)  new peak::Address(ip1,ip2,ip3,ip4,port);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Address");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Address_new03(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Address */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Address_new04_local
static int tolua_peak_peak_Address_new04_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Address",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  unsigned char ip1 = ((unsigned char)  tolua_tonumber(tolua_S,2,0));
  unsigned char ip2 = ((unsigned char)  tolua_tonumber(tolua_S,3,0));
  unsigned char ip3 = ((unsigned char)  tolua_tonumber(tolua_S,4,0));
  unsigned char ip4 = ((unsigned char)  tolua_tonumber(tolua_S,5,0));
  unsigned short port = ((unsigned short)  tolua_tonumber(tolua_S,6,0));
  {
   peak::Address* tolua_ret = (peak::Address*)  new peak::Address(ip1,ip2,ip3,ip4,port);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Address");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Address_new03_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::Address */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Address_delete00
static int tolua_peak_peak_Address_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Address",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Address* self = (peak::Address*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAddress of class  peak::Address */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Address_setAddress00
static int tolua_peak_peak_Address_setAddress00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Address",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Address* self = (peak::Address*)  tolua_tousertype(tolua_S,1,0);
  unsigned int addr = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAddress'",NULL);
#endif
  {
   self->setAddress(addr);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAddress'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAddress of class  peak::Address */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Address_getAddress00
static int tolua_peak_peak_Address_getAddress00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Address",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Address* self = (peak::Address*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAddress'",NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->getAddress();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAddress'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAddress of class  peak::Address */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Address_setAddress01
static int tolua_peak_peak_Address_setAddress01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Address",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::Address* self = (peak::Address*)  tolua_tousertype(tolua_S,1,0);
  unsigned char ip1 = ((unsigned char)  tolua_tonumber(tolua_S,2,0));
  unsigned char ip2 = ((unsigned char)  tolua_tonumber(tolua_S,3,0));
  unsigned char ip3 = ((unsigned char)  tolua_tonumber(tolua_S,4,0));
  unsigned char ip4 = ((unsigned char)  tolua_tonumber(tolua_S,5,0));
  unsigned short port = ((unsigned short)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAddress'",NULL);
#endif
  {
   self->setAddress(ip1,ip2,ip3,ip4,port);
  }
 }
 return 0;
tolua_lerror:
 return tolua_peak_peak_Address_setAddress00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAddress of class  peak::Address */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Address_setAddress02
static int tolua_peak_peak_Address_setAddress02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Address",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::Address* self = (peak::Address*)  tolua_tousertype(tolua_S,1,0);
  std::string addr = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAddress'",NULL);
#endif
  {
   self->setAddress(addr);
  }
 }
 return 0;
tolua_lerror:
 return tolua_peak_peak_Address_setAddress01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAddressString of class  peak::Address */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Address_getAddressString00
static int tolua_peak_peak_Address_getAddressString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Address",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Address* self = (peak::Address*)  tolua_tousertype(tolua_S,1,0);
  bool port = ((bool)  tolua_toboolean(tolua_S,2,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAddressString'",NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getAddressString(port);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAddressString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setPort of class  peak::Address */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Address_setPort00
static int tolua_peak_peak_Address_setPort00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Address",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Address* self = (peak::Address*)  tolua_tousertype(tolua_S,1,0);
  unsigned short port = ((unsigned short)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPort'",NULL);
#endif
  {
   self->setPort(port);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setPort'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPort of class  peak::Address */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Address_getPort00
static int tolua_peak_peak_Address_getPort00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Address",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Address* self = (peak::Address*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPort'",NULL);
#endif
  {
   unsigned short tolua_ret = (unsigned short)  self->getPort();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPort'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Vector3D */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Vector3D_new00
static int tolua_peak_peak_Vector3D_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Vector3D* tolua_ret = (peak::Vector3D*)  new peak::Vector3D();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Vector3D");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Vector3D */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Vector3D_new00_local
static int tolua_peak_peak_Vector3D_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Vector3D* tolua_ret = (peak::Vector3D*)  new peak::Vector3D();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Vector3D");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Vector3D */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Vector3D_new01
static int tolua_peak_peak_Vector3D_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
  float z = ((float)  tolua_tonumber(tolua_S,4,0));
  {
   peak::Vector3D* tolua_ret = (peak::Vector3D*)  new peak::Vector3D(x,y,z);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Vector3D");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Vector3D_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Vector3D */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Vector3D_new01_local
static int tolua_peak_peak_Vector3D_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
  float z = ((float)  tolua_tonumber(tolua_S,4,0));
  {
   peak::Vector3D* tolua_ret = (peak::Vector3D*)  new peak::Vector3D(x,y,z);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Vector3D");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Vector3D_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::Vector3D */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Vector3D_delete00
static int tolua_peak_peak_Vector3D_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Vector3D* self = (peak::Vector3D*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: rotateXZ of class  peak::Vector3D */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Vector3D_rotateXZ00
static int tolua_peak_peak_Vector3D_rotateXZ00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Vector3D* self = (peak::Vector3D*)  tolua_tousertype(tolua_S,1,0);
  float radians = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'rotateXZ'",NULL);
#endif
  {
   self->rotateXZ(radians);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'rotateXZ'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: rotateYZ of class  peak::Vector3D */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Vector3D_rotateYZ00
static int tolua_peak_peak_Vector3D_rotateYZ00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Vector3D* self = (peak::Vector3D*)  tolua_tousertype(tolua_S,1,0);
  float radians = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'rotateYZ'",NULL);
#endif
  {
   self->rotateYZ(radians);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'rotateYZ'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: rotateXY of class  peak::Vector3D */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Vector3D_rotateXY00
static int tolua_peak_peak_Vector3D_rotateXY00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Vector3D* self = (peak::Vector3D*)  tolua_tousertype(tolua_S,1,0);
  float radians = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'rotateXY'",NULL);
#endif
  {
   self->rotateXY(radians);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'rotateXY'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRotation of class  peak::Vector3D */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Vector3D_getRotation00
static int tolua_peak_peak_Vector3D_getRotation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Vector3D* self = (peak::Vector3D*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRotation'",NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getRotation();
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Vector3D(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRotation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getLengthSq of class  peak::Vector3D */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Vector3D_getLengthSq00
static int tolua_peak_peak_Vector3D_getLengthSq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Vector3D* self = (peak::Vector3D*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLengthSq'",NULL);
#endif
  {
   float tolua_ret = (float)  self->getLengthSq();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLengthSq'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: x of class  peak::Vector3D */
#ifndef TOLUA_DISABLE_tolua_get_peak__Vector3D_x
static int tolua_get_peak__Vector3D_x(lua_State* tolua_S)
{
  peak::Vector3D* self = (peak::Vector3D*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->x);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: x of class  peak::Vector3D */
#ifndef TOLUA_DISABLE_tolua_set_peak__Vector3D_x
static int tolua_set_peak__Vector3D_x(lua_State* tolua_S)
{
  peak::Vector3D* self = (peak::Vector3D*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->x = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: y of class  peak::Vector3D */
#ifndef TOLUA_DISABLE_tolua_get_peak__Vector3D_y
static int tolua_get_peak__Vector3D_y(lua_State* tolua_S)
{
  peak::Vector3D* self = (peak::Vector3D*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->y);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: y of class  peak::Vector3D */
#ifndef TOLUA_DISABLE_tolua_set_peak__Vector3D_y
static int tolua_set_peak__Vector3D_y(lua_State* tolua_S)
{
  peak::Vector3D* self = (peak::Vector3D*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->y = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: z of class  peak::Vector3D */
#ifndef TOLUA_DISABLE_tolua_get_peak__Vector3D_z
static int tolua_get_peak__Vector3D_z(lua_State* tolua_S)
{
  peak::Vector3D* self = (peak::Vector3D*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'z'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->z);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: z of class  peak::Vector3D */
#ifndef TOLUA_DISABLE_tolua_set_peak__Vector3D_z
static int tolua_set_peak__Vector3D_z(lua_State* tolua_S)
{
  peak::Vector3D* self = (peak::Vector3D*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'z'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->z = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Vector2D */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Vector2D_new00
static int tolua_peak_peak_Vector2D_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Vector2D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Vector2D* tolua_ret = (peak::Vector2D*)  new peak::Vector2D();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Vector2D");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Vector2D */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Vector2D_new00_local
static int tolua_peak_peak_Vector2D_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Vector2D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Vector2D* tolua_ret = (peak::Vector2D*)  new peak::Vector2D();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Vector2D");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Vector2D */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Vector2D_new01
static int tolua_peak_peak_Vector2D_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Vector2D",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
  {
   peak::Vector2D* tolua_ret = (peak::Vector2D*)  new peak::Vector2D(x,y);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Vector2D");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Vector2D_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Vector2D */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Vector2D_new01_local
static int tolua_peak_peak_Vector2D_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Vector2D",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
  {
   peak::Vector2D* tolua_ret = (peak::Vector2D*)  new peak::Vector2D(x,y);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Vector2D");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Vector2D_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: rotate of class  peak::Vector2D */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Vector2D_rotate00
static int tolua_peak_peak_Vector2D_rotate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Vector2D",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Vector2D* self = (peak::Vector2D*)  tolua_tousertype(tolua_S,1,0);
  float rotation = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'rotate'",NULL);
#endif
  {
   self->rotate(rotation);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'rotate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: x of class  peak::Vector2D */
#ifndef TOLUA_DISABLE_tolua_get_peak__Vector2D_x
static int tolua_get_peak__Vector2D_x(lua_State* tolua_S)
{
  peak::Vector2D* self = (peak::Vector2D*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->x);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: x of class  peak::Vector2D */
#ifndef TOLUA_DISABLE_tolua_set_peak__Vector2D_x
static int tolua_set_peak__Vector2D_x(lua_State* tolua_S)
{
  peak::Vector2D* self = (peak::Vector2D*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->x = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: y of class  peak::Vector2D */
#ifndef TOLUA_DISABLE_tolua_get_peak__Vector2D_y
static int tolua_get_peak__Vector2D_y(lua_State* tolua_S)
{
  peak::Vector2D* self = (peak::Vector2D*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->y);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: y of class  peak::Vector2D */
#ifndef TOLUA_DISABLE_tolua_set_peak__Vector2D_y
static int tolua_set_peak__Vector2D_y(lua_State* tolua_S)
{
  peak::Vector2D* self = (peak::Vector2D*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->y = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Vector2DI */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Vector2DI_new00
static int tolua_peak_peak_Vector2DI_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Vector2DI",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Vector2DI* tolua_ret = (peak::Vector2DI*)  new peak::Vector2DI();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Vector2DI");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Vector2DI */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Vector2DI_new00_local
static int tolua_peak_peak_Vector2DI_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Vector2DI",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Vector2DI* tolua_ret = (peak::Vector2DI*)  new peak::Vector2DI();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Vector2DI");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Vector2DI */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Vector2DI_new01
static int tolua_peak_peak_Vector2DI_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Vector2DI",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
  {
   peak::Vector2DI* tolua_ret = (peak::Vector2DI*)  new peak::Vector2DI(x,y);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Vector2DI");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Vector2DI_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Vector2DI */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Vector2DI_new01_local
static int tolua_peak_peak_Vector2DI_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Vector2DI",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
  {
   peak::Vector2DI* tolua_ret = (peak::Vector2DI*)  new peak::Vector2DI(x,y);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Vector2DI");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Vector2DI_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* get function: x of class  peak::Vector2DI */
#ifndef TOLUA_DISABLE_tolua_get_peak__Vector2DI_x
static int tolua_get_peak__Vector2DI_x(lua_State* tolua_S)
{
  peak::Vector2DI* self = (peak::Vector2DI*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->x);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: x of class  peak::Vector2DI */
#ifndef TOLUA_DISABLE_tolua_set_peak__Vector2DI_x
static int tolua_set_peak__Vector2DI_x(lua_State* tolua_S)
{
  peak::Vector2DI* self = (peak::Vector2DI*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->x = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: y of class  peak::Vector2DI */
#ifndef TOLUA_DISABLE_tolua_get_peak__Vector2DI_y
static int tolua_get_peak__Vector2DI_y(lua_State* tolua_S)
{
  peak::Vector2DI* self = (peak::Vector2DI*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->y);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: y of class  peak::Vector2DI */
#ifndef TOLUA_DISABLE_tolua_set_peak__Vector2DI_y
static int tolua_set_peak__Vector2DI_y(lua_State* tolua_S)
{
  peak::Vector2DI* self = (peak::Vector2DI*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->y = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Color */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Color_new00
static int tolua_peak_peak_Color_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Color",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Color* tolua_ret = (peak::Color*)  new peak::Color();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Color");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Color */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Color_new00_local
static int tolua_peak_peak_Color_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Color",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Color* tolua_ret = (peak::Color*)  new peak::Color();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Color");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Color */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Color_new01
static int tolua_peak_peak_Color_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Color",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  unsigned int color = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  {
   peak::Color* tolua_ret = (peak::Color*)  new peak::Color(color);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Color");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Color_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Color */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Color_new01_local
static int tolua_peak_peak_Color_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Color",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  unsigned int color = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  {
   peak::Color* tolua_ret = (peak::Color*)  new peak::Color(color);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Color");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Color_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Color */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Color_new02
static int tolua_peak_peak_Color_new02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Color",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  unsigned char r = ((unsigned char)  tolua_tonumber(tolua_S,2,0));
  unsigned char g = ((unsigned char)  tolua_tonumber(tolua_S,3,0));
  unsigned char b = ((unsigned char)  tolua_tonumber(tolua_S,4,0));
  unsigned char a = ((unsigned char)  tolua_tonumber(tolua_S,5,255));
  {
   peak::Color* tolua_ret = (peak::Color*)  new peak::Color(r,g,b,a);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Color");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Color_new01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Color */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Color_new02_local
static int tolua_peak_peak_Color_new02_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Color",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  unsigned char r = ((unsigned char)  tolua_tonumber(tolua_S,2,0));
  unsigned char g = ((unsigned char)  tolua_tonumber(tolua_S,3,0));
  unsigned char b = ((unsigned char)  tolua_tonumber(tolua_S,4,0));
  unsigned char a = ((unsigned char)  tolua_tonumber(tolua_S,5,255));
  {
   peak::Color* tolua_ret = (peak::Color*)  new peak::Color(r,g,b,a);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Color");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Color_new01_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRed of class  peak::Color */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Color_getRed00
static int tolua_peak_peak_Color_getRed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Color",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Color* self = (peak::Color*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRed'",NULL);
#endif
  {
   unsigned char tolua_ret = (unsigned char)  self->getRed();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRed'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getGreen of class  peak::Color */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Color_getGreen00
static int tolua_peak_peak_Color_getGreen00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Color",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Color* self = (peak::Color*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getGreen'",NULL);
#endif
  {
   unsigned char tolua_ret = (unsigned char)  self->getGreen();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getGreen'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getBlue of class  peak::Color */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Color_getBlue00
static int tolua_peak_peak_Color_getBlue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Color",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Color* self = (peak::Color*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getBlue'",NULL);
#endif
  {
   unsigned char tolua_ret = (unsigned char)  self->getBlue();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getBlue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAlpha of class  peak::Color */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Color_getAlpha00
static int tolua_peak_peak_Color_getAlpha00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Color",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Color* self = (peak::Color*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAlpha'",NULL);
#endif
  {
   unsigned char tolua_ret = (unsigned char)  self->getAlpha();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAlpha'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setRed of class  peak::Color */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Color_setRed00
static int tolua_peak_peak_Color_setRed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Color",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Color* self = (peak::Color*)  tolua_tousertype(tolua_S,1,0);
  unsigned char red = ((unsigned char)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRed'",NULL);
#endif
  {
   self->setRed(red);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setRed'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setGreen of class  peak::Color */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Color_setGreen00
static int tolua_peak_peak_Color_setGreen00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Color",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Color* self = (peak::Color*)  tolua_tousertype(tolua_S,1,0);
  unsigned char green = ((unsigned char)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setGreen'",NULL);
#endif
  {
   self->setGreen(green);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setGreen'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setBlue of class  peak::Color */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Color_setBlue00
static int tolua_peak_peak_Color_setBlue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Color",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Color* self = (peak::Color*)  tolua_tousertype(tolua_S,1,0);
  unsigned char blue = ((unsigned char)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setBlue'",NULL);
#endif
  {
   self->setBlue(blue);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setBlue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAlpha of class  peak::Color */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Color_setAlpha00
static int tolua_peak_peak_Color_setAlpha00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Color",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Color* self = (peak::Color*)  tolua_tousertype(tolua_S,1,0);
  unsigned char alpha = ((unsigned char)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAlpha'",NULL);
#endif
  {
   self->setAlpha(alpha);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAlpha'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: set of class  peak::Color */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Color_set00
static int tolua_peak_peak_Color_set00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Color",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Color* self = (peak::Color*)  tolua_tousertype(tolua_S,1,0);
  unsigned char r = ((unsigned char)  tolua_tonumber(tolua_S,2,0));
  unsigned char g = ((unsigned char)  tolua_tonumber(tolua_S,3,0));
  unsigned char b = ((unsigned char)  tolua_tonumber(tolua_S,4,0));
  unsigned char a = ((unsigned char)  tolua_tonumber(tolua_S,5,255));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'",NULL);
#endif
  {
   self->set(r,g,b,a);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: set of class  peak::Color */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Color_set01
static int tolua_peak_peak_Color_set01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Color",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::Color* self = (peak::Color*)  tolua_tousertype(tolua_S,1,0);
  unsigned int color = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'",NULL);
#endif
  {
   self->set(color);
  }
 }
 return 0;
tolua_lerror:
 return tolua_peak_peak_Color_set00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: get of class  peak::Color */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Color_get00
static int tolua_peak_peak_Color_get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Color",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Color* self = (peak::Color*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get'",NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->get();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Quaternion */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Quaternion_new00
static int tolua_peak_peak_Quaternion_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Quaternion* tolua_ret = (peak::Quaternion*)  new peak::Quaternion();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Quaternion");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Quaternion */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Quaternion_new00_local
static int tolua_peak_peak_Quaternion_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Quaternion* tolua_ret = (peak::Quaternion*)  new peak::Quaternion();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Quaternion");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Quaternion */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Quaternion_new01
static int tolua_peak_peak_Quaternion_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Quaternion",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  float w = ((float)  tolua_tonumber(tolua_S,2,0));
  float x = ((float)  tolua_tonumber(tolua_S,3,0));
  float y = ((float)  tolua_tonumber(tolua_S,4,0));
  float z = ((float)  tolua_tonumber(tolua_S,5,0));
  {
   peak::Quaternion* tolua_ret = (peak::Quaternion*)  new peak::Quaternion(w,x,y,z);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Quaternion");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Quaternion_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Quaternion */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Quaternion_new01_local
static int tolua_peak_peak_Quaternion_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Quaternion",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  float w = ((float)  tolua_tonumber(tolua_S,2,0));
  float x = ((float)  tolua_tonumber(tolua_S,3,0));
  float y = ((float)  tolua_tonumber(tolua_S,4,0));
  float z = ((float)  tolua_tonumber(tolua_S,5,0));
  {
   peak::Quaternion* tolua_ret = (peak::Quaternion*)  new peak::Quaternion(w,x,y,z);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Quaternion");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Quaternion_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Quaternion */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Quaternion_new02
static int tolua_peak_peak_Quaternion_new02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Quaternion",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
  float z = ((float)  tolua_tonumber(tolua_S,4,0));
  {
   peak::Quaternion* tolua_ret = (peak::Quaternion*)  new peak::Quaternion(x,y,z);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Quaternion");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Quaternion_new01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Quaternion */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Quaternion_new02_local
static int tolua_peak_peak_Quaternion_new02_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Quaternion",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
  float z = ((float)  tolua_tonumber(tolua_S,4,0));
  {
   peak::Quaternion* tolua_ret = (peak::Quaternion*)  new peak::Quaternion(x,y,z);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Quaternion");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Quaternion_new01_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Quaternion */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Quaternion_new03
static int tolua_peak_peak_Quaternion_new03(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Quaternion",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::Vector3D euler = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
  {
   peak::Quaternion* tolua_ret = (peak::Quaternion*)  new peak::Quaternion(euler);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Quaternion");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Quaternion_new02(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Quaternion */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Quaternion_new03_local
static int tolua_peak_peak_Quaternion_new03_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Quaternion",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::Vector3D euler = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
  {
   peak::Quaternion* tolua_ret = (peak::Quaternion*)  new peak::Quaternion(euler);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Quaternion");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Quaternion_new02_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Quaternion */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Quaternion_new04
static int tolua_peak_peak_Quaternion_new04(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Quaternion",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  float q = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   peak::Quaternion* tolua_ret = (peak::Quaternion*)  new peak::Quaternion(&q);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Quaternion");
   tolua_pushnumber(tolua_S,(lua_Number)q);
  }
 }
 return 2;
tolua_lerror:
 return tolua_peak_peak_Quaternion_new03(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Quaternion */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Quaternion_new04_local
static int tolua_peak_peak_Quaternion_new04_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Quaternion",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  float q = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   peak::Quaternion* tolua_ret = (peak::Quaternion*)  new peak::Quaternion(&q);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Quaternion");
   tolua_pushnumber(tolua_S,(lua_Number)q);
  }
 }
 return 2;
tolua_lerror:
 return tolua_peak_peak_Quaternion_new03_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Quaternion */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Quaternion_new05
static int tolua_peak_peak_Quaternion_new05(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Quaternion",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  float angle = ((float)  tolua_tonumber(tolua_S,2,0));
  peak::Vector3D axis = *((peak::Vector3D*)  tolua_tousertype(tolua_S,3,0));
  {
   peak::Quaternion* tolua_ret = (peak::Quaternion*)  new peak::Quaternion(angle,axis);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Quaternion");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Quaternion_new04(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Quaternion */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Quaternion_new05_local
static int tolua_peak_peak_Quaternion_new05_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Quaternion",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  float angle = ((float)  tolua_tonumber(tolua_S,2,0));
  peak::Vector3D axis = *((peak::Vector3D*)  tolua_tousertype(tolua_S,3,0));
  {
   peak::Quaternion* tolua_ret = (peak::Quaternion*)  new peak::Quaternion(angle,axis);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Quaternion");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Quaternion_new04_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: fromAngleAxis of class  peak::Quaternion */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Quaternion_fromAngleAxis00
static int tolua_peak_peak_Quaternion_fromAngleAxis00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Quaternion",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Quaternion* self = (peak::Quaternion*)  tolua_tousertype(tolua_S,1,0);
  float angle = ((float)  tolua_tonumber(tolua_S,2,0));
  peak::Vector3D axis = *((peak::Vector3D*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'fromAngleAxis'",NULL);
#endif
  {
   self->fromAngleAxis(angle,axis);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'fromAngleAxis'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: toAngleAxis of class  peak::Quaternion */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Quaternion_toAngleAxis00
static int tolua_peak_peak_Quaternion_toAngleAxis00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Quaternion",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Quaternion* self = (peak::Quaternion*)  tolua_tousertype(tolua_S,1,0);
  float angle = ((float)  tolua_tonumber(tolua_S,2,0));
  peak::Vector3D* axis = ((peak::Vector3D*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'toAngleAxis'",NULL);
#endif
  {
   self->toAngleAxis(angle,*axis);
   tolua_pushnumber(tolua_S,(lua_Number)angle);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'toAngleAxis'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getYaw of class  peak::Quaternion */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Quaternion_getYaw00
static int tolua_peak_peak_Quaternion_getYaw00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Quaternion* self = (peak::Quaternion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getYaw'",NULL);
#endif
  {
   float tolua_ret = (float)  self->getYaw();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getYaw'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getEuler of class  peak::Quaternion */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Quaternion_getEuler00
static int tolua_peak_peak_Quaternion_getEuler00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Quaternion* self = (peak::Quaternion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getEuler'",NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getEuler();
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Vector3D(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getEuler'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: rotateVector of class  peak::Quaternion */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Quaternion_rotateVector00
static int tolua_peak_peak_Quaternion_rotateVector00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Quaternion",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Quaternion* self = (peak::Quaternion*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D vector = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'rotateVector'",NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->rotateVector(vector);
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Vector3D(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'rotateVector'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: slerp of class  peak::Quaternion */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Quaternion_slerp00
static int tolua_peak_peak_Quaternion_slerp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Quaternion",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Quaternion",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Quaternion* self = (peak::Quaternion*)  tolua_tousertype(tolua_S,1,0);
  peak::Quaternion other = *((peak::Quaternion*)  tolua_tousertype(tolua_S,2,0));
  float value = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'slerp'",NULL);
#endif
  {
   peak::Quaternion tolua_ret = (peak::Quaternion)  self->slerp(other,value);
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Quaternion(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Quaternion");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Quaternion));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Quaternion");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'slerp'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getDifference of class  peak::Quaternion */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Quaternion_getDifference00
static int tolua_peak_peak_Quaternion_getDifference00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Quaternion",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Quaternion* self = (peak::Quaternion*)  tolua_tousertype(tolua_S,1,0);
  peak::Quaternion* q2 = ((peak::Quaternion*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDifference'",NULL);
#endif
  {
   float tolua_ret = (float)  self->getDifference(*q2);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getDifference'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: q of class  peak::Quaternion */
#ifndef TOLUA_DISABLE_tolua_get_peak_peak_Quaternion_q
static int tolua_get_peak_peak_Quaternion_q(lua_State* tolua_S)
{
 int tolua_index;
  peak::Quaternion* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (peak::Quaternion*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=4)
  tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->q[tolua_index]);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: q of class  peak::Quaternion */
#ifndef TOLUA_DISABLE_tolua_set_peak_peak_Quaternion_q
static int tolua_set_peak_peak_Quaternion_q(lua_State* tolua_S)
{
 int tolua_index;
  peak::Quaternion* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (peak::Quaternion*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=4)
  tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
  self->q[tolua_index] = ((float)  tolua_tonumber(tolua_S,3,0));
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Parameter */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Parameter_new00
static int tolua_peak_peak_Parameter_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Parameter",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  double value = ((double)  tolua_tonumber(tolua_S,2,0));
  {
   peak::Parameter* tolua_ret = (peak::Parameter*)  new peak::Parameter(value);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Parameter");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Parameter */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Parameter_new00_local
static int tolua_peak_peak_Parameter_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Parameter",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  double value = ((double)  tolua_tonumber(tolua_S,2,0));
  {
   peak::Parameter* tolua_ret = (peak::Parameter*)  new peak::Parameter(value);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Parameter");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Parameter */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Parameter_new01
static int tolua_peak_peak_Parameter_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Parameter",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isuserdata(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  void* ptr = ((void*)  tolua_touserdata(tolua_S,3,0));
  {
   peak::Parameter* tolua_ret = (peak::Parameter*)  new peak::Parameter(name,ptr);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Parameter");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Parameter_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Parameter */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Parameter_new01_local
static int tolua_peak_peak_Parameter_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Parameter",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isuserdata(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  void* ptr = ((void*)  tolua_touserdata(tolua_S,3,0));
  {
   peak::Parameter* tolua_ret = (peak::Parameter*)  new peak::Parameter(name,ptr);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Parameter");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Parameter_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: isPointer of class  peak::Parameter */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Parameter_isPointer00
static int tolua_peak_peak_Parameter_isPointer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Parameter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Parameter* self = (peak::Parameter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isPointer'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isPointer();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isPointer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getValue of class  peak::Parameter */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Parameter_getValue00
static int tolua_peak_peak_Parameter_getValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Parameter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Parameter* self = (peak::Parameter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getValue'",NULL);
#endif
  {
   double tolua_ret = (double)  self->getValue();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getClassName of class  peak::Parameter */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Parameter_getClassName00
static int tolua_peak_peak_Parameter_getClassName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Parameter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Parameter* self = (peak::Parameter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getClassName'",NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getClassName();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getClassName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPointer of class  peak::Parameter */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Parameter_getPointer00
static int tolua_peak_peak_Parameter_getPointer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Parameter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Parameter* self = (peak::Parameter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPointer'",NULL);
#endif
  {
   void* tolua_ret = (void*)  self->getPointer();
   tolua_pushuserdata(tolua_S,(void*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPointer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::ParameterList */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ParameterList_new00
static int tolua_peak_peak_ParameterList_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::ParameterList",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::ParameterList* tolua_ret = (peak::ParameterList*)  new peak::ParameterList();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::ParameterList");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::ParameterList */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ParameterList_new00_local
static int tolua_peak_peak_ParameterList_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::ParameterList",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::ParameterList* tolua_ret = (peak::ParameterList*)  new peak::ParameterList();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::ParameterList");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::ParameterList */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ParameterList_delete00
static int tolua_peak_peak_ParameterList_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::ParameterList",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::ParameterList* self = (peak::ParameterList*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: add of class  peak::ParameterList */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ParameterList_add00
static int tolua_peak_peak_ParameterList_add00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::ParameterList",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Parameter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::ParameterList* self = (peak::ParameterList*)  tolua_tousertype(tolua_S,1,0);
  peak::Parameter* parameter = ((peak::Parameter*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'add'",NULL);
#endif
  {
   self->add(*parameter);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'add'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Script */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Script_new00
static int tolua_peak_peak_Script_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Script",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Script* tolua_ret = (peak::Script*)  new peak::Script();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Script");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Script */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Script_new00_local
static int tolua_peak_peak_Script_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Script",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Script* tolua_ret = (peak::Script*)  new peak::Script();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Script");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::Script */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Script_delete00
static int tolua_peak_peak_Script_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Script",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Script* self = (peak::Script*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: run of class  peak::Script */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Script_run00
static int tolua_peak_peak_Script_run00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Script",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Script* self = (peak::Script*)  tolua_tousertype(tolua_S,1,0);
  std::string filename = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'run'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->run(filename);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'run'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: callFunction of class  peak::Script */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Script_callFunction00
static int tolua_peak_peak_Script_callFunction00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Script",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Script* self = (peak::Script*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'callFunction'",NULL);
#endif
  {
   self->callFunction(name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'callFunction'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: callFunction of class  peak::Script */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Script_callFunction01
static int tolua_peak_peak_Script_callFunction01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Script",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::ParameterList",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::Script* self = (peak::Script*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  peak::ParameterList* parameters = ((peak::ParameterList*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'callFunction'",NULL);
#endif
  {
   self->callFunction(name,*parameters);
  }
 }
 return 0;
tolua_lerror:
 return tolua_peak_peak_Script_callFunction00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: callFunctionI of class  peak::Script */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Script_callFunctionI00
static int tolua_peak_peak_Script_callFunctionI00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Script",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Script* self = (peak::Script*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  int arg1 = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'callFunctionI'",NULL);
#endif
  {
   self->callFunctionI(name,arg1);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'callFunctionI'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: callFunctionII of class  peak::Script */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Script_callFunctionII00
static int tolua_peak_peak_Script_callFunctionII00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Script",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Script* self = (peak::Script*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  int arg1 = ((int)  tolua_tonumber(tolua_S,3,0));
  int arg2 = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'callFunctionII'",NULL);
#endif
  {
   self->callFunctionII(name,arg1,arg2);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'callFunctionII'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: callFunctionIII of class  peak::Script */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Script_callFunctionIII00
static int tolua_peak_peak_Script_callFunctionIII00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Script",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Script* self = (peak::Script*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  int arg1 = ((int)  tolua_tonumber(tolua_S,3,0));
  int arg2 = ((int)  tolua_tonumber(tolua_S,4,0));
  int arg3 = ((int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'callFunctionIII'",NULL);
#endif
  {
   self->callFunctionIII(name,arg1,arg2,arg3);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'callFunctionIII'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: callFunctionFFF of class  peak::Script */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Script_callFunctionFFF00
static int tolua_peak_peak_Script_callFunctionFFF00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Script",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Script* self = (peak::Script*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  float arg1 = ((float)  tolua_tonumber(tolua_S,3,0));
  float arg2 = ((float)  tolua_tonumber(tolua_S,4,0));
  float arg3 = ((float)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'callFunctionFFF'",NULL);
#endif
  {
   self->callFunctionFFF(name,arg1,arg2,arg3);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'callFunctionFFF'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: callIntFunction of class  peak::Script */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Script_callIntFunction00
static int tolua_peak_peak_Script_callIntFunction00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Script",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Script* self = (peak::Script*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'callIntFunction'",NULL);
#endif
  {
   int tolua_ret = (int)  self->callIntFunction(name);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'callIntFunction'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: callFloatFunction of class  peak::Script */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Script_callFloatFunction00
static int tolua_peak_peak_Script_callFloatFunction00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Script",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Script* self = (peak::Script*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'callFloatFunction'",NULL);
#endif
  {
   float tolua_ret = (float)  self->callFloatFunction(name);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'callFloatFunction'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isFunction of class  peak::Script */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Script_isFunction00
static int tolua_peak_peak_Script_isFunction00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Script",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Script* self = (peak::Script*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isFunction'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isFunction(name);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isFunction'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::World */
#ifndef TOLUA_DISABLE_tolua_peak_peak_World_new00
static int tolua_peak_peak_World_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::World",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::World* tolua_ret = (peak::World*)  new peak::World();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::World");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::World */
#ifndef TOLUA_DISABLE_tolua_peak_peak_World_new00_local
static int tolua_peak_peak_World_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::World",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::World* tolua_ret = (peak::World*)  new peak::World();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::World");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::World */
#ifndef TOLUA_DISABLE_tolua_peak_peak_World_delete00
static int tolua_peak_peak_World_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::World",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::World* self = (peak::World*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: init of class  peak::World */
#ifndef TOLUA_DISABLE_tolua_peak_peak_World_init00
static int tolua_peak_peak_World_init00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::World",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::World* self = (peak::World*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->init();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'init'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: destroy of class  peak::World */
#ifndef TOLUA_DISABLE_tolua_peak_peak_World_destroy00
static int tolua_peak_peak_World_destroy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::World",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::World* self = (peak::World*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroy'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->destroy();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'destroy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: collide of class  peak::World */
#ifndef TOLUA_DISABLE_tolua_peak_peak_World_collide00
static int tolua_peak_peak_World_collide00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::World",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Geometry",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::World* self = (peak::World*)  tolua_tousertype(tolua_S,1,0);
  peak::Geometry* geom = ((peak::Geometry*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'collide'",NULL);
#endif
  {
   int tolua_ret = (int)  self->collide(geom);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'collide'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getContactPoints of class  peak::World */
#ifndef TOLUA_DISABLE_tolua_peak_peak_World_getContactPoints00
static int tolua_peak_peak_World_getContactPoints00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::World",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::World* self = (peak::World*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getContactPoints'",NULL);
#endif
  {
   std::vector<peak::ContactInfo>* tolua_ret = (std::vector<peak::ContactInfo>*)  self->getContactPoints();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"std::vector<peak::ContactInfo>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getContactPoints'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getContactPoint of class  peak::World */
#ifndef TOLUA_DISABLE_tolua_peak_peak_World_getContactPoint00
static int tolua_peak_peak_World_getContactPoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::World",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::World* self = (peak::World*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getContactPoint'",NULL);
#endif
  {
   peak::ContactInfo tolua_ret = (peak::ContactInfo)  self->getContactPoint(index);
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::ContactInfo(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::ContactInfo");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::ContactInfo));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::ContactInfo");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getContactPoint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getID of class  peak::World */
#ifndef TOLUA_DISABLE_tolua_peak_peak_World_getID00
static int tolua_peak_peak_World_getID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::World",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::World* self = (peak::World*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getID'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getID();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getID'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: doWork of class  peak::World */
#ifndef TOLUA_DISABLE_tolua_peak_peak_World_doWork00
static int tolua_peak_peak_World_doWork00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::World",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::World* self = (peak::World*)  tolua_tousertype(tolua_S,1,0);
  float msecs = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'doWork'",NULL);
#endif
  {
   self->doWork(msecs);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'doWork'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: doAllWork of class  peak::World */
#ifndef TOLUA_DISABLE_tolua_peak_peak_World_doAllWork00
static int tolua_peak_peak_World_doAllWork00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::World",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float msecs = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   peak::World::doAllWork(msecs);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'doAllWork'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getWorld of class  peak::World */
#ifndef TOLUA_DISABLE_tolua_peak_peak_World_getWorld00
static int tolua_peak_peak_World_getWorld00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::World",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::World* self = (peak::World*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWorld'",NULL);
#endif
  {
   dWorldID tolua_ret = (dWorldID)  self->getWorld();
   {
#ifdef __cplusplus
    void* tolua_obj = new dWorldID(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"dWorldID");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(dWorldID));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"dWorldID");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getWorld'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSpace of class  peak::World */
#ifndef TOLUA_DISABLE_tolua_peak_peak_World_getSpace00
static int tolua_peak_peak_World_getSpace00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::World",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::World* self = (peak::World*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSpace'",NULL);
#endif
  {
   dSpaceID tolua_ret = (dSpaceID)  self->getSpace();
   {
#ifdef __cplusplus
    void* tolua_obj = new dSpaceID(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"dSpaceID");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(dSpaceID));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"dSpaceID");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSpace'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getContactGroup of class  peak::World */
#ifndef TOLUA_DISABLE_tolua_peak_peak_World_getContactGroup00
static int tolua_peak_peak_World_getContactGroup00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::World",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::World* self = (peak::World*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getContactGroup'",NULL);
#endif
  {
   dJointGroupID tolua_ret = (dJointGroupID)  self->getContactGroup();
   {
#ifdef __cplusplus
    void* tolua_obj = new dJointGroupID(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"dJointGroupID");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(dJointGroupID));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"dJointGroupID");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getContactGroup'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getWorld of class  peak::World */
#ifndef TOLUA_DISABLE_tolua_peak_peak_World_getWorld01
static int tolua_peak_peak_World_getWorld01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::World",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
  {
   peak::World* tolua_ret = (peak::World*)  peak::World::getWorld(id);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::World");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_World_getWorld00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Entity */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Entity_new00
static int tolua_peak_peak_Entity_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Entity* tolua_ret = (peak::Entity*)  new peak::Entity();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Entity");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Entity */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Entity_new00_local
static int tolua_peak_peak_Entity_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Entity* tolua_ret = (peak::Entity*)  new peak::Entity();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Entity");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::Entity */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Entity_delete00
static int tolua_peak_peak_Entity_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Entity* self = (peak::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: load of class  peak::Entity */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Entity_load00
static int tolua_peak_peak_Entity_load00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Entity",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Entity* self = (peak::Entity*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  bool server = ((bool)  tolua_toboolean(tolua_S,3,0));
  bool graphics = ((bool)  tolua_toboolean(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'load'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->load(name,server,graphics);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: destroy of class  peak::Entity */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Entity_destroy00
static int tolua_peak_peak_Entity_destroy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Entity* self = (peak::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroy'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->destroy();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'destroy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getName of class  peak::Entity */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Entity_getName00
static int tolua_peak_peak_Entity_getName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Entity* self = (peak::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getName'",NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getName();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setID of class  peak::Entity */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Entity_setID00
static int tolua_peak_peak_Entity_setID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Entity",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Entity* self = (peak::Entity*)  tolua_tousertype(tolua_S,1,0);
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setID'",NULL);
#endif
  {
   self->setID(id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setID'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getID of class  peak::Entity */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Entity_getID00
static int tolua_peak_peak_Entity_getID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Entity* self = (peak::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getID'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getID();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getID'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setLocal of class  peak::Entity */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Entity_setLocal00
static int tolua_peak_peak_Entity_setLocal00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Entity",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Entity* self = (peak::Entity*)  tolua_tousertype(tolua_S,1,0);
  bool local = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setLocal'",NULL);
#endif
  {
   self->setLocal(local);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setLocal'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isLocal of class  peak::Entity */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Entity_isLocal00
static int tolua_peak_peak_Entity_isLocal00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Entity* self = (peak::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isLocal'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isLocal();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isLocal'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setOwner of class  peak::Entity */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Entity_setOwner00
static int tolua_peak_peak_Entity_setOwner00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Entity",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Entity* self = (peak::Entity*)  tolua_tousertype(tolua_S,1,0);
  int owner = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setOwner'",NULL);
#endif
  {
   self->setOwner(owner);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setOwner'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getOwner of class  peak::Entity */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Entity_getOwner00
static int tolua_peak_peak_Entity_getOwner00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Entity* self = (peak::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getOwner'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getOwner();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getOwner'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setPriority of class  peak::Entity */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Entity_setPriority00
static int tolua_peak_peak_Entity_setPriority00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Entity",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Entity* self = (peak::Entity*)  tolua_tousertype(tolua_S,1,0);
  int priority = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPriority'",NULL);
#endif
  {
   self->setPriority(priority);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setPriority'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPriority of class  peak::Entity */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Entity_getPriority00
static int tolua_peak_peak_Entity_getPriority00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Entity* self = (peak::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPriority'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getPriority();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPriority'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setCurrentPriority of class  peak::Entity */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Entity_setCurrentPriority00
static int tolua_peak_peak_Entity_setCurrentPriority00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Entity",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Entity* self = (peak::Entity*)  tolua_tousertype(tolua_S,1,0);
  int priority = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setCurrentPriority'",NULL);
#endif
  {
   self->setCurrentPriority(priority);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setCurrentPriority'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getCurrentPriority of class  peak::Entity */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Entity_getCurrentPriority00
static int tolua_peak_peak_Entity_getCurrentPriority00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Entity* self = (peak::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCurrentPriority'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getCurrentPriority();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCurrentPriority'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: needsUpdate of class  peak::Entity */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Entity_needsUpdate00
static int tolua_peak_peak_Entity_needsUpdate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Entity* self = (peak::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'needsUpdate'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->needsUpdate();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'needsUpdate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getUpdateSize of class  peak::Entity */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Entity_getUpdateSize00
static int tolua_peak_peak_Entity_getUpdateSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Entity* self = (peak::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getUpdateSize'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getUpdateSize();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getUpdateSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: writeUpdateData of class  peak::Entity */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Entity_writeUpdateData00
static int tolua_peak_peak_Entity_writeUpdateData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Entity",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Entity* self = (peak::Entity*)  tolua_tousertype(tolua_S,1,0);
  peak::Buffer* buffer = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeUpdateData'",NULL);
#endif
  {
   int tolua_ret = (int)  self->writeUpdateData(buffer);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'writeUpdateData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: updateFromData of class  peak::Entity */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Entity_updateFromData00
static int tolua_peak_peak_Entity_updateFromData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Entity",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Entity* self = (peak::Entity*)  tolua_tousertype(tolua_S,1,0);
  peak::Buffer* buffer = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'updateFromData'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->updateFromData(buffer);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'updateFromData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: writeCompleteData of class  peak::Entity */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Entity_writeCompleteData00
static int tolua_peak_peak_Entity_writeCompleteData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Entity",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Entity* self = (peak::Entity*)  tolua_tousertype(tolua_S,1,0);
  peak::Buffer* buffer = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeCompleteData'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->writeCompleteData(buffer);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'writeCompleteData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: readCompleteData of class  peak::Entity */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Entity_readCompleteData00
static int tolua_peak_peak_Entity_readCompleteData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Entity",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Entity* self = (peak::Entity*)  tolua_tousertype(tolua_S,1,0);
  peak::Buffer* buffer = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readCompleteData'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->readCompleteData(buffer);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'readCompleteData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addComponent of class  peak::Entity */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Entity_addComponent00
static int tolua_peak_peak_Entity_addComponent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Entity",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Component",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Entity* self = (peak::Entity*)  tolua_tousertype(tolua_S,1,0);
  peak::Component* component = ((peak::Component*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addComponent'",NULL);
#endif
  {
   self->addComponent(component);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addComponent'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: deleteComponent of class  peak::Entity */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Entity_deleteComponent00
static int tolua_peak_peak_Entity_deleteComponent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Entity",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Entity* self = (peak::Entity*)  tolua_tousertype(tolua_S,1,0);
  peak::ComponentType type = ((peak::ComponentType) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'deleteComponent'",NULL);
#endif
  {
   self->deleteComponent(type);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'deleteComponent'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getComponent of class  peak::Entity */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Entity_getComponent00
static int tolua_peak_peak_Entity_getComponent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Entity",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Entity* self = (peak::Entity*)  tolua_tousertype(tolua_S,1,0);
  peak::ComponentType type = ((peak::ComponentType) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getComponent'",NULL);
#endif
  {
   peak::Component* tolua_ret = (peak::Component*)  self->getComponent(type);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Component");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getComponent'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPhysicsComponent of class  peak::Entity */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Entity_getPhysicsComponent00
static int tolua_peak_peak_Entity_getPhysicsComponent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Entity* self = (peak::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPhysicsComponent'",NULL);
#endif
  {
   peak::PhysicsComponent* tolua_ret = (peak::PhysicsComponent*)  self->getPhysicsComponent();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::PhysicsComponent");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPhysicsComponent'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getGraphicsComponent of class  peak::Entity */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Entity_getGraphicsComponent00
static int tolua_peak_peak_Entity_getGraphicsComponent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Entity* self = (peak::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getGraphicsComponent'",NULL);
#endif
  {
   peak::GraphicsComponent* tolua_ret = (peak::GraphicsComponent*)  self->getGraphicsComponent();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::GraphicsComponent");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getGraphicsComponent'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getServerScriptComponent of class  peak::Entity */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Entity_getServerScriptComponent00
static int tolua_peak_peak_Entity_getServerScriptComponent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Entity* self = (peak::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getServerScriptComponent'",NULL);
#endif
  {
   peak::ServerScriptComponent* tolua_ret = (peak::ServerScriptComponent*)  self->getServerScriptComponent();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::ServerScriptComponent");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getServerScriptComponent'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVariables of class  peak::Entity */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Entity_getVariables00
static int tolua_peak_peak_Entity_getVariables00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Entity* self = (peak::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVariables'",NULL);
#endif
  {
   peak::EntityVariables* tolua_ret = (peak::EntityVariables*)  self->getVariables();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::EntityVariables");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVariables'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: doWork of class  peak::Entity */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Entity_doWork00
static int tolua_peak_peak_Entity_doWork00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Entity",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Entity* self = (peak::Entity*)  tolua_tousertype(tolua_S,1,0);
  float msecs = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'doWork'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->doWork(msecs);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'doWork'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Connection */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Connection_new00
static int tolua_peak_peak_Connection_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Connection",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"ENetPeer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ENetPeer* peer = ((ENetPeer*)  tolua_tousertype(tolua_S,2,0));
  {
   peak::Connection* tolua_ret = (peak::Connection*)  new peak::Connection(peer);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Connection");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Connection */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Connection_new00_local
static int tolua_peak_peak_Connection_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Connection",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"ENetPeer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ENetPeer* peer = ((ENetPeer*)  tolua_tousertype(tolua_S,2,0));
  {
   peak::Connection* tolua_ret = (peak::Connection*)  new peak::Connection(peer);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Connection");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::Connection */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Connection_delete00
static int tolua_peak_peak_Connection_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Connection",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Connection* self = (peak::Connection*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: destroy of class  peak::Connection */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Connection_destroy00
static int tolua_peak_peak_Connection_destroy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Connection",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Connection* self = (peak::Connection*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroy'",NULL);
#endif
  {
   self->destroy();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'destroy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: disconnect of class  peak::Connection */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Connection_disconnect00
static int tolua_peak_peak_Connection_disconnect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Connection",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Connection* self = (peak::Connection*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'disconnect'",NULL);
#endif
  {
   self->disconnect();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'disconnect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isConnected of class  peak::Connection */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Connection_isConnected00
static int tolua_peak_peak_Connection_isConnected00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Connection",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Connection* self = (peak::Connection*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isConnected'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isConnected();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isConnected'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: hasNewData of class  peak::Connection */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Connection_hasNewData00
static int tolua_peak_peak_Connection_hasNewData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Connection",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Connection* self = (peak::Connection*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hasNewData'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->hasNewData();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hasNewData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: readData of class  peak::Connection */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Connection_readData00
static int tolua_peak_peak_Connection_readData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Connection",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Connection* self = (peak::Connection*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readData'",NULL);
#endif
  {
   peak::Buffer* tolua_ret = (peak::Buffer*)  self->readData();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Buffer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'readData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: sendData of class  peak::Connection */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Connection_sendData00
static int tolua_peak_peak_Connection_sendData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Connection",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Buffer",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Connection* self = (peak::Connection*)  tolua_tousertype(tolua_S,1,0);
  peak::Buffer* data = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
  bool reliable = ((bool)  tolua_toboolean(tolua_S,3,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'sendData'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->sendData(data,reliable);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sendData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: injectData of class  peak::Connection */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Connection_injectData00
static int tolua_peak_peak_Connection_injectData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Connection",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Connection* self = (peak::Connection*)  tolua_tousertype(tolua_S,1,0);
  peak::Buffer* data = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'injectData'",NULL);
#endif
  {
   self->injectData(data);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'injectData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPeer of class  peak::Connection */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Connection_getPeer00
static int tolua_peak_peak_Connection_getPeer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Connection",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Connection* self = (peak::Connection*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPeer'",NULL);
#endif
  {
   ENetPeer* tolua_ret = (ENetPeer*)  self->getPeer();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"ENetPeer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPeer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::BroadcastHost */
#ifndef TOLUA_DISABLE_tolua_peak_peak_BroadcastHost_new00
static int tolua_peak_peak_BroadcastHost_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::BroadcastHost",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::BroadcastHost* tolua_ret = (peak::BroadcastHost*)  new peak::BroadcastHost();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::BroadcastHost");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::BroadcastHost */
#ifndef TOLUA_DISABLE_tolua_peak_peak_BroadcastHost_new00_local
static int tolua_peak_peak_BroadcastHost_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::BroadcastHost",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::BroadcastHost* tolua_ret = (peak::BroadcastHost*)  new peak::BroadcastHost();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::BroadcastHost");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::BroadcastHost */
#ifndef TOLUA_DISABLE_tolua_peak_peak_BroadcastHost_delete00
static int tolua_peak_peak_BroadcastHost_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::BroadcastHost",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::BroadcastHost* self = (peak::BroadcastHost*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: init of class  peak::BroadcastHost */
#ifndef TOLUA_DISABLE_tolua_peak_peak_BroadcastHost_init00
static int tolua_peak_peak_BroadcastHost_init00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::BroadcastHost",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::BroadcastHost* self = (peak::BroadcastHost*)  tolua_tousertype(tolua_S,1,0);
  int port = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->init(port);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'init'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: shutdown of class  peak::BroadcastHost */
#ifndef TOLUA_DISABLE_tolua_peak_peak_BroadcastHost_shutdown00
static int tolua_peak_peak_BroadcastHost_shutdown00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::BroadcastHost",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::BroadcastHost* self = (peak::BroadcastHost*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'shutdown'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->shutdown();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'shutdown'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setInfo of class  peak::BroadcastHost */
#ifndef TOLUA_DISABLE_tolua_peak_peak_BroadcastHost_setInfo00
static int tolua_peak_peak_BroadcastHost_setInfo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::BroadcastHost",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::BroadcastHost* self = (peak::BroadcastHost*)  tolua_tousertype(tolua_S,1,0);
  std::string info = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setInfo'",NULL);
#endif
  {
   self->setInfo(info);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setInfo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInfo of class  peak::BroadcastHost */
#ifndef TOLUA_DISABLE_tolua_peak_peak_BroadcastHost_getInfo00
static int tolua_peak_peak_BroadcastHost_getInfo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::BroadcastHost",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::BroadcastHost* self = (peak::BroadcastHost*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getInfo'",NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getInfo();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInfo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: doWork of class  peak::BroadcastHost */
#ifndef TOLUA_DISABLE_tolua_peak_peak_BroadcastHost_doWork00
static int tolua_peak_peak_BroadcastHost_doWork00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::BroadcastHost",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::BroadcastHost* self = (peak::BroadcastHost*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'doWork'",NULL);
#endif
  {
   self->doWork();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'doWork'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get of class  peak::Game */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Game_get00
static int tolua_peak_peak_Game_get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Game",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Game* tolua_ret = (peak::Game*)  peak::Game::get();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Game");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::Game */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Game_delete00
static int tolua_peak_peak_Game_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Game",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Game* self = (peak::Game*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: init of class  peak::Game */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Game_init00
static int tolua_peak_peak_Game_init00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Game",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Game* self = (peak::Game*)  tolua_tousertype(tolua_S,1,0);
  bool client = ((bool)  tolua_toboolean(tolua_S,2,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->init(client);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'init'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: init of class  peak::Game */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Game_init01
static int tolua_peak_peak_Game_init01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Game",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Address",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::Game* self = (peak::Game*)  tolua_tousertype(tolua_S,1,0);
  peak::Address addr = *((peak::Address*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->init(addr);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Game_init00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: shutdown of class  peak::Game */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Game_shutdown00
static int tolua_peak_peak_Game_shutdown00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Game",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Game* self = (peak::Game*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'shutdown'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->shutdown();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'shutdown'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: stopGame of class  peak::Game */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Game_stopGame00
static int tolua_peak_peak_Game_stopGame00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Game",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Game* self = (peak::Game*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'stopGame'",NULL);
#endif
  {
   self->stopGame();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'stopGame'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isServer of class  peak::Game */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Game_isServer00
static int tolua_peak_peak_Game_isServer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Game",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Game* self = (peak::Game*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isServer'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isServer();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isServer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isClient of class  peak::Game */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Game_isClient00
static int tolua_peak_peak_Game_isClient00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Game",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Game* self = (peak::Game*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isClient'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isClient();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isClient'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setVisible of class  peak::Game */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Game_setVisible00
static int tolua_peak_peak_Game_setVisible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Game",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Game* self = (peak::Game*)  tolua_tousertype(tolua_S,1,0);
  bool visible = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setVisible'",NULL);
#endif
  {
   self->setVisible(visible);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setVisible'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isVisible of class  peak::Game */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Game_isVisible00
static int tolua_peak_peak_Game_isVisible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Game",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Game* self = (peak::Game*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isVisible'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isVisible();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isVisible'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createEntity of class  peak::Game */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Game_createEntity00
static int tolua_peak_peak_Game_createEntity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Game",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Game* self = (peak::Game*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  int owner = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createEntity'",NULL);
#endif
  {
   peak::Entity* tolua_ret = (peak::Entity*)  self->createEntity(name,owner);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Entity");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createEntity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getEntity of class  peak::Game */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Game_getEntity00
static int tolua_peak_peak_Game_getEntity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Game",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Game* self = (peak::Game*)  tolua_tousertype(tolua_S,1,0);
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getEntity'",NULL);
#endif
  {
   peak::Entity* tolua_ret = (peak::Entity*)  self->getEntity(id);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Entity");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getEntity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: deleteEntity of class  peak::Game */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Game_deleteEntity00
static int tolua_peak_peak_Game_deleteEntity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Game",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Entity",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Game* self = (peak::Game*)  tolua_tousertype(tolua_S,1,0);
  peak::Entity* entity = ((peak::Entity*)  tolua_tousertype(tolua_S,2,0));
  bool now = ((bool)  tolua_toboolean(tolua_S,3,false));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'deleteEntity'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->deleteEntity(entity,now);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'deleteEntity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getEntityCount of class  peak::Game */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Game_getEntityCount00
static int tolua_peak_peak_Game_getEntityCount00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Game",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Game* self = (peak::Game*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getEntityCount'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getEntityCount(name);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getEntityCount'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getEntity of class  peak::Game */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Game_getEntity01
static int tolua_peak_peak_Game_getEntity01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Game",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::Game* self = (peak::Game*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  int index = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getEntity'",NULL);
#endif
  {
   peak::Entity* tolua_ret = (peak::Entity*)  self->getEntity(name,index);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Entity");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Game_getEntity00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getWorld of class  peak::Game */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Game_getWorld00
static int tolua_peak_peak_Game_getWorld00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Game",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Game* self = (peak::Game*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWorld'",NULL);
#endif
  {
   peak::World* tolua_ret = (peak::World*)  self->getWorld();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::World");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getWorld'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: injectAction of class  peak::Game */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Game_injectAction00
static int tolua_peak_peak_Game_injectAction00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Game",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Game* self = (peak::Game*)  tolua_tousertype(tolua_S,1,0);
  std::string action = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  bool state = ((bool)  tolua_toboolean(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'injectAction'",NULL);
#endif
  {
   self->injectAction(action,state);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'injectAction'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: injectMouseMovement of class  peak::Game */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Game_injectMouseMovement00
static int tolua_peak_peak_Game_injectMouseMovement00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Game",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Game* self = (peak::Game*)  tolua_tousertype(tolua_S,1,0);
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
  float z = ((float)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'injectMouseMovement'",NULL);
#endif
  {
   self->injectMouseMovement(x,y,z);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'injectMouseMovement'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: registerEntityActionInput of class  peak::Game */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Game_registerEntityActionInput00
static int tolua_peak_peak_Game_registerEntityActionInput00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Game",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Entity",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Game* self = (peak::Game*)  tolua_tousertype(tolua_S,1,0);
  peak::Entity* entity = ((peak::Entity*)  tolua_tousertype(tolua_S,2,0));
  std::string action = ((std::string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'registerEntityActionInput'",NULL);
#endif
  {
   self->registerEntityActionInput(entity,action);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'registerEntityActionInput'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: registerEntityMouseInput of class  peak::Game */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Game_registerEntityMouseInput00
static int tolua_peak_peak_Game_registerEntityMouseInput00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Game",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Game* self = (peak::Game*)  tolua_tousertype(tolua_S,1,0);
  peak::Entity* entity = ((peak::Entity*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'registerEntityMouseInput'",NULL);
#endif
  {
   self->registerEntityMouseInput(entity);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'registerEntityMouseInput'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRegisteredEntity of class  peak::Game */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Game_getRegisteredEntity00
static int tolua_peak_peak_Game_getRegisteredEntity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Game",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Game* self = (peak::Game*)  tolua_tousertype(tolua_S,1,0);
  std::string action = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRegisteredEntity'",NULL);
#endif
  {
   peak::Entity* tolua_ret = (peak::Entity*)  self->getRegisteredEntity(action);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Entity");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRegisteredEntity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: doWork of class  peak::Game */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Game_doWork00
static int tolua_peak_peak_Game_doWork00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Game",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Game* self = (peak::Game*)  tolua_tousertype(tolua_S,1,0);
  float msecs = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'doWork'",NULL);
#endif
  {
   self->doWork(msecs);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'doWork'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getServerScript of class  peak::Game */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Game_getServerScript00
static int tolua_peak_peak_Game_getServerScript00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Game",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Game* self = (peak::Game*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getServerScript'",NULL);
#endif
  {
   peak::Script* tolua_ret = (peak::Script*)  self->getServerScript();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Script");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getServerScript'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getClientScript of class  peak::Game */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Game_getClientScript00
static int tolua_peak_peak_Game_getClientScript00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Game",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Game* self = (peak::Game*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getClientScript'",NULL);
#endif
  {
   peak::Script* tolua_ret = (peak::Script*)  self->getClientScript();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Script");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getClientScript'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get of class  peak::Logger */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Logger_get00
static int tolua_peak_peak_Logger_get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Logger",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Logger* tolua_ret = (peak::Logger*)  peak::Logger::get();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Logger");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::Logger */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Logger_delete00
static int tolua_peak_peak_Logger_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Logger",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Logger* self = (peak::Logger*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: log of class  peak::Logger */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Logger_log00
static int tolua_peak_peak_Logger_log00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Logger",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Logger* self = (peak::Logger*)  tolua_tousertype(tolua_S,1,0);
  peak::ELoggingLevel level = ((peak::ELoggingLevel) (int)  tolua_tonumber(tolua_S,2,0));
  std::string msg = ((std::string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'log'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->log(level,msg);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'log'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setFile of class  peak::Logger */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Logger_setFile00
static int tolua_peak_peak_Logger_setFile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Logger",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Logger* self = (peak::Logger*)  tolua_tousertype(tolua_S,1,0);
  std::string filename = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setFile'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->setFile(filename);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setFile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getFile of class  peak::Logger */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Logger_getFile00
static int tolua_peak_peak_Logger_getFile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Logger",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Logger* self = (peak::Logger*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFile'",NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getFile();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clearFile of class  peak::Logger */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Logger_clearFile00
static int tolua_peak_peak_Logger_clearFile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Logger",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Logger* self = (peak::Logger*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clearFile'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->clearFile();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clearFile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setTimestamps of class  peak::Logger */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Logger_setTimestamps00
static int tolua_peak_peak_Logger_setTimestamps00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Logger",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Logger* self = (peak::Logger*)  tolua_tousertype(tolua_S,1,0);
  bool enabled = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setTimestamps'",NULL);
#endif
  {
   self->setTimestamps(enabled);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setTimestamps'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setConsoleTimestamps of class  peak::Logger */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Logger_setConsoleTimestamps00
static int tolua_peak_peak_Logger_setConsoleTimestamps00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Logger",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Logger* self = (peak::Logger*)  tolua_tousertype(tolua_S,1,0);
  bool enabled = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setConsoleTimestamps'",NULL);
#endif
  {
   self->setConsoleTimestamps(enabled);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setConsoleTimestamps'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getConsoleTimestamps of class  peak::Logger */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Logger_getConsoleTimestamps00
static int tolua_peak_peak_Logger_getConsoleTimestamps00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Logger",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Logger* self = (peak::Logger*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getConsoleTimestamps'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getConsoleTimestamps();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getConsoleTimestamps'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setFileTimestamps of class  peak::Logger */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Logger_setFileTimestamps00
static int tolua_peak_peak_Logger_setFileTimestamps00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Logger",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Logger* self = (peak::Logger*)  tolua_tousertype(tolua_S,1,0);
  bool enabled = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setFileTimestamps'",NULL);
#endif
  {
   self->setFileTimestamps(enabled);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setFileTimestamps'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getFileTimestamps of class  peak::Logger */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Logger_getFileTimestamps00
static int tolua_peak_peak_Logger_getFileTimestamps00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Logger",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Logger* self = (peak::Logger*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFileTimestamps'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getFileTimestamps();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFileTimestamps'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setPrefix of class  peak::Logger */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Logger_setPrefix00
static int tolua_peak_peak_Logger_setPrefix00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Logger",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Logger* self = (peak::Logger*)  tolua_tousertype(tolua_S,1,0);
  peak::EPrefixType type = ((peak::EPrefixType) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPrefix'",NULL);
#endif
  {
   self->setPrefix(type);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setPrefix'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setConsolePrefix of class  peak::Logger */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Logger_setConsolePrefix00
static int tolua_peak_peak_Logger_setConsolePrefix00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Logger",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Logger* self = (peak::Logger*)  tolua_tousertype(tolua_S,1,0);
  peak::EPrefixType type = ((peak::EPrefixType) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setConsolePrefix'",NULL);
#endif
  {
   self->setConsolePrefix(type);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setConsolePrefix'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getConsolePrefix of class  peak::Logger */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Logger_getConsolePrefix00
static int tolua_peak_peak_Logger_getConsolePrefix00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Logger",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Logger* self = (peak::Logger*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getConsolePrefix'",NULL);
#endif
  {
   peak::EPrefixType tolua_ret = (peak::EPrefixType)  self->getConsolePrefix();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getConsolePrefix'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setFilePrefix of class  peak::Logger */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Logger_setFilePrefix00
static int tolua_peak_peak_Logger_setFilePrefix00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Logger",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Logger* self = (peak::Logger*)  tolua_tousertype(tolua_S,1,0);
  peak::EPrefixType type = ((peak::EPrefixType) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setFilePrefix'",NULL);
#endif
  {
   self->setFilePrefix(type);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setFilePrefix'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getFilePrefix of class  peak::Logger */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Logger_getFilePrefix00
static int tolua_peak_peak_Logger_getFilePrefix00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Logger",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Logger* self = (peak::Logger*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFilePrefix'",NULL);
#endif
  {
   peak::EPrefixType tolua_ret = (peak::EPrefixType)  self->getFilePrefix();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFilePrefix'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setMinimalConsoleLevel of class  peak::Logger */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Logger_setMinimalConsoleLevel00
static int tolua_peak_peak_Logger_setMinimalConsoleLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Logger",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Logger* self = (peak::Logger*)  tolua_tousertype(tolua_S,1,0);
  peak::ELoggingLevel level = ((peak::ELoggingLevel) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMinimalConsoleLevel'",NULL);
#endif
  {
   self->setMinimalConsoleLevel(level);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMinimalConsoleLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMinimalConsoleLevel of class  peak::Logger */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Logger_getMinimalConsoleLevel00
static int tolua_peak_peak_Logger_getMinimalConsoleLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Logger",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Logger* self = (peak::Logger*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMinimalConsoleLevel'",NULL);
#endif
  {
   peak::ELoggingLevel tolua_ret = (peak::ELoggingLevel)  self->getMinimalConsoleLevel();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMinimalConsoleLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setMinimalFileLevel of class  peak::Logger */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Logger_setMinimalFileLevel00
static int tolua_peak_peak_Logger_setMinimalFileLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Logger",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Logger* self = (peak::Logger*)  tolua_tousertype(tolua_S,1,0);
  peak::ELoggingLevel level = ((peak::ELoggingLevel) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMinimalFileLevel'",NULL);
#endif
  {
   self->setMinimalFileLevel(level);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMinimalFileLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMinimalFileLevel of class  peak::Logger */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Logger_getMinimalFileLevel00
static int tolua_peak_peak_Logger_getMinimalFileLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Logger",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Logger* self = (peak::Logger*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMinimalFileLevel'",NULL);
#endif
  {
   peak::ELoggingLevel tolua_ret = (peak::ELoggingLevel)  self->getMinimalFileLevel();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMinimalFileLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get of class  peak::InputManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_InputManager_get00
static int tolua_peak_peak_InputManager_get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::InputManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::InputManager* tolua_ret = (peak::InputManager*)  peak::InputManager::get();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::InputManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: init of class  peak::InputManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_InputManager_init00
static int tolua_peak_peak_InputManager_init00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::InputManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::InputManager* self = (peak::InputManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->init();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'init'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: shutdown of class  peak::InputManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_InputManager_shutdown00
static int tolua_peak_peak_InputManager_shutdown00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::InputManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::InputManager* self = (peak::InputManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'shutdown'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->shutdown();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'shutdown'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: keyboardEvent of class  peak::InputManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_InputManager_keyboardEvent00
static int tolua_peak_peak_InputManager_keyboardEvent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::InputManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::InputManager* self = (peak::InputManager*)  tolua_tousertype(tolua_S,1,0);
  peak::Keycode code = ((peak::Keycode) (int)  tolua_tonumber(tolua_S,2,0));
  bool pressed = ((bool)  tolua_toboolean(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'keyboardEvent'",NULL);
#endif
  {
   self->keyboardEvent(code,pressed);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'keyboardEvent'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: mouseMoved of class  peak::InputManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_InputManager_mouseMoved00
static int tolua_peak_peak_InputManager_mouseMoved00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::InputManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::InputManager* self = (peak::InputManager*)  tolua_tousertype(tolua_S,1,0);
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'mouseMoved'",NULL);
#endif
  {
   self->mouseMoved(x,y);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'mouseMoved'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setInputEnabled of class  peak::InputManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_InputManager_setInputEnabled00
static int tolua_peak_peak_InputManager_setInputEnabled00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::InputManager",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::InputManager* self = (peak::InputManager*)  tolua_tousertype(tolua_S,1,0);
  bool enabled = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setInputEnabled'",NULL);
#endif
  {
   self->setInputEnabled(enabled);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setInputEnabled'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInputEnabled of class  peak::InputManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_InputManager_getInputEnabled00
static int tolua_peak_peak_InputManager_getInputEnabled00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::InputManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::InputManager* self = (peak::InputManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getInputEnabled'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getInputEnabled();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInputEnabled'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: peak::getKey */
#ifndef TOLUA_DISABLE_tolua_peak_peak_getKey00
static int tolua_peak_peak_getKey00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,1,0));
  {
   peak::Keycode tolua_ret = (peak::Keycode)  peak::getKey(name);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getKey'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::File */
#ifndef TOLUA_DISABLE_tolua_peak_peak_File_new00
static int tolua_peak_peak_File_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::File",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string filename = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  peak::EFileMode mode = ((peak::EFileMode) (int)  tolua_tonumber(tolua_S,3,0));
  bool binary = ((bool)  tolua_toboolean(tolua_S,4,false));
  {
   peak::File* tolua_ret = (peak::File*)  new peak::File(filename,mode,binary);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::File");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::File */
#ifndef TOLUA_DISABLE_tolua_peak_peak_File_new00_local
static int tolua_peak_peak_File_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::File",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string filename = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  peak::EFileMode mode = ((peak::EFileMode) (int)  tolua_tonumber(tolua_S,3,0));
  bool binary = ((bool)  tolua_toboolean(tolua_S,4,false));
  {
   peak::File* tolua_ret = (peak::File*)  new peak::File(filename,mode,binary);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::File");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::File */
#ifndef TOLUA_DISABLE_tolua_peak_peak_File_delete00
static int tolua_peak_peak_File_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::File",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::File* self = (peak::File*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: read of class  peak::File */
#ifndef TOLUA_DISABLE_tolua_peak_peak_File_read00
static int tolua_peak_peak_File_read00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::File",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::File* self = (peak::File*)  tolua_tousertype(tolua_S,1,0);
  char* buffer = ((char*)  tolua_tostring(tolua_S,2,0));
  int length = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'read'",NULL);
#endif
  {
   int tolua_ret = (int)  self->read(buffer,length);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'read'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: read of class  peak::File */
#ifndef TOLUA_DISABLE_tolua_peak_peak_File_read01
static int tolua_peak_peak_File_read01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::File",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::File* self = (peak::File*)  tolua_tousertype(tolua_S,1,0);
  peak::Buffer* buffer = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
  int length = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'read'",NULL);
#endif
  {
   int tolua_ret = (int)  self->read(buffer,length);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_File_read00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: readLine of class  peak::File */
#ifndef TOLUA_DISABLE_tolua_peak_peak_File_readLine00
static int tolua_peak_peak_File_readLine00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::File",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::File* self = (peak::File*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readLine'",NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->readLine();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'readLine'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: write of class  peak::File */
#ifndef TOLUA_DISABLE_tolua_peak_peak_File_write00
static int tolua_peak_peak_File_write00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::File",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::File* self = (peak::File*)  tolua_tousertype(tolua_S,1,0);
  const char* buffer = ((const char*)  tolua_tostring(tolua_S,2,0));
  int length = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'write'",NULL);
#endif
  {
   int tolua_ret = (int)  self->write(buffer,length);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'write'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: write of class  peak::File */
#ifndef TOLUA_DISABLE_tolua_peak_peak_File_write01
static int tolua_peak_peak_File_write01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::File",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::File* self = (peak::File*)  tolua_tousertype(tolua_S,1,0);
  peak::Buffer* buffer = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
  int length = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'write'",NULL);
#endif
  {
   int tolua_ret = (int)  self->write(buffer,length);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_File_write00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: eof of class  peak::File */
#ifndef TOLUA_DISABLE_tolua_peak_peak_File_eof00
static int tolua_peak_peak_File_eof00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::File",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::File* self = (peak::File*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'eof'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->eof();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'eof'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPosition of class  peak::File */
#ifndef TOLUA_DISABLE_tolua_peak_peak_File_getPosition00
static int tolua_peak_peak_File_getPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::File",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::File* self = (peak::File*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPosition'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getPosition();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setPosition of class  peak::File */
#ifndef TOLUA_DISABLE_tolua_peak_peak_File_setPosition00
static int tolua_peak_peak_File_setPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::File",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::File* self = (peak::File*)  tolua_tousertype(tolua_S,1,0);
  int pos = ((int)  tolua_tonumber(tolua_S,2,0));
  bool end = ((bool)  tolua_toboolean(tolua_S,3,false));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPosition'",NULL);
#endif
  {
   int tolua_ret = (int)  self->setPosition(pos,end);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: movePosition of class  peak::File */
#ifndef TOLUA_DISABLE_tolua_peak_peak_File_movePosition00
static int tolua_peak_peak_File_movePosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::File",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::File* self = (peak::File*)  tolua_tousertype(tolua_S,1,0);
  int pos = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'movePosition'",NULL);
#endif
  {
   int tolua_ret = (int)  self->movePosition(pos);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'movePosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get of class  peak::FileSystem */
#ifndef TOLUA_DISABLE_tolua_peak_peak_FileSystem_get00
static int tolua_peak_peak_FileSystem_get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::FileSystem",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::FileSystem* tolua_ret = (peak::FileSystem*)  peak::FileSystem::get();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::FileSystem");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::FileSystem */
#ifndef TOLUA_DISABLE_tolua_peak_peak_FileSystem_delete00
static int tolua_peak_peak_FileSystem_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::FileSystem",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::FileSystem* self = (peak::FileSystem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: init of class  peak::FileSystem */
#ifndef TOLUA_DISABLE_tolua_peak_peak_FileSystem_init00
static int tolua_peak_peak_FileSystem_init00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::FileSystem",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::FileSystem* self = (peak::FileSystem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->init();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'init'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: shutdown of class  peak::FileSystem */
#ifndef TOLUA_DISABLE_tolua_peak_peak_FileSystem_shutdown00
static int tolua_peak_peak_FileSystem_shutdown00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::FileSystem",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::FileSystem* self = (peak::FileSystem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'shutdown'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->shutdown();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'shutdown'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setRoot of class  peak::FileSystem */
#ifndef TOLUA_DISABLE_tolua_peak_peak_FileSystem_setRoot00
static int tolua_peak_peak_FileSystem_setRoot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::FileSystem",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::FileSystem* self = (peak::FileSystem*)  tolua_tousertype(tolua_S,1,0);
  std::string path = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRoot'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->setRoot(path);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setRoot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addOverlay of class  peak::FileSystem */
#ifndef TOLUA_DISABLE_tolua_peak_peak_FileSystem_addOverlay00
static int tolua_peak_peak_FileSystem_addOverlay00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::FileSystem",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::FileSystem* self = (peak::FileSystem*)  tolua_tousertype(tolua_S,1,0);
  std::string path = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addOverlay'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->addOverlay(path);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addOverlay'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: openFile of class  peak::FileSystem */
#ifndef TOLUA_DISABLE_tolua_peak_peak_FileSystem_openFile00
static int tolua_peak_peak_FileSystem_openFile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::FileSystem",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::FileSystem* self = (peak::FileSystem*)  tolua_tousertype(tolua_S,1,0);
  std::string path = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  peak::EFileMode mode = ((peak::EFileMode) (int)  tolua_tonumber(tolua_S,3,0));
  bool binary = ((bool)  tolua_toboolean(tolua_S,4,false));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'openFile'",NULL);
#endif
  {
   peak::File* tolua_ret = (peak::File*)  self->openFile(path,mode,binary);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::File");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'openFile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getFileExists of class  peak::FileSystem */
#ifndef TOLUA_DISABLE_tolua_peak_peak_FileSystem_getFileExists00
static int tolua_peak_peak_FileSystem_getFileExists00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::FileSystem",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::FileSystem* self = (peak::FileSystem*)  tolua_tousertype(tolua_S,1,0);
  std::string path = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFileExists'",NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getFileExists(path);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFileExists'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: listFiles of class  peak::FileSystem */
#ifndef TOLUA_DISABLE_tolua_peak_peak_FileSystem_listFiles00
static int tolua_peak_peak_FileSystem_listFiles00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::FileSystem",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,1,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::FileSystem* self = (peak::FileSystem*)  tolua_tousertype(tolua_S,1,0);
  std::string directory = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  std::string extension = ((std::string)  tolua_tocppstring(tolua_S,3,""));
  bool recursive = ((bool)  tolua_toboolean(tolua_S,4,false));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'listFiles'",NULL);
#endif
  {
   std::vector<std::string> tolua_ret = (std::vector<std::string>)  self->listFiles(directory,extension,recursive);
   {
#ifdef __cplusplus
    void* tolua_obj = new std::vector<std::string>(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"std::vector<std::string>");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(std::vector<std::string>));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"std::vector<std::string>");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'listFiles'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Timer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Timer_new00
static int tolua_peak_peak_Timer_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Timer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Timer* tolua_ret = (peak::Timer*)  new peak::Timer();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Timer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Timer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Timer_new00_local
static int tolua_peak_peak_Timer_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Timer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Timer* tolua_ret = (peak::Timer*)  new peak::Timer();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Timer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::Timer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Timer_delete00
static int tolua_peak_peak_Timer_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Timer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Timer* self = (peak::Timer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: start of class  peak::Timer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Timer_start00
static int tolua_peak_peak_Timer_start00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Timer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Timer* self = (peak::Timer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'start'",NULL);
#endif
  {
   self->start();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'start'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: stop of class  peak::Timer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Timer_stop00
static int tolua_peak_peak_Timer_stop00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Timer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Timer* self = (peak::Timer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'stop'",NULL);
#endif
  {
   self->stop();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'stop'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: reset of class  peak::Timer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Timer_reset00
static int tolua_peak_peak_Timer_reset00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Timer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Timer* self = (peak::Timer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'reset'",NULL);
#endif
  {
   self->reset();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'reset'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setTime of class  peak::Timer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Timer_setTime00
static int tolua_peak_peak_Timer_setTime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Timer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Timer* self = (peak::Timer*)  tolua_tousertype(tolua_S,1,0);
  float time = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setTime'",NULL);
#endif
  {
   self->setTime(time);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setTime'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTime of class  peak::Timer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Timer_getTime00
static int tolua_peak_peak_Timer_getTime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Timer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Timer* self = (peak::Timer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTime'",NULL);
#endif
  {
   float tolua_ret = (float)  self->getTime();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTime'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addCallback of class  peak::Timer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Timer_addCallback00
static int tolua_peak_peak_Timer_addCallback00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Timer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::Script",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Timer* self = (peak::Timer*)  tolua_tousertype(tolua_S,1,0);
  float time = ((float)  tolua_tonumber(tolua_S,2,0));
  peak::Script* script = ((peak::Script*)  tolua_tousertype(tolua_S,3,0));
  std::string function = ((std::string)  tolua_tocppstring(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addCallback'",NULL);
#endif
  {
   self->addCallback(time,script,function);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addCallback'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: doAllWork of class  peak::Timer */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Timer_doAllWork00
static int tolua_peak_peak_Timer_doAllWork00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Timer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Timer::doAllWork();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'doAllWork'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: timer of class  peak::TimerCallback */
#ifndef TOLUA_DISABLE_tolua_get_peak__TimerCallback_timer_ptr
static int tolua_get_peak__TimerCallback_timer_ptr(lua_State* tolua_S)
{
  peak::TimerCallback* self = (peak::TimerCallback*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'timer'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)self->timer,"peak::Timer");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: timer of class  peak::TimerCallback */
#ifndef TOLUA_DISABLE_tolua_set_peak__TimerCallback_timer_ptr
static int tolua_set_peak__TimerCallback_timer_ptr(lua_State* tolua_S)
{
  peak::TimerCallback* self = (peak::TimerCallback*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'timer'",NULL);
  if (!tolua_isusertype(tolua_S,2,"peak::Timer",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->timer = ((peak::Timer*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: time of class  peak::TimerCallback */
#ifndef TOLUA_DISABLE_tolua_get_peak__TimerCallback_time
static int tolua_get_peak__TimerCallback_time(lua_State* tolua_S)
{
  peak::TimerCallback* self = (peak::TimerCallback*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'time'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->time);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: time of class  peak::TimerCallback */
#ifndef TOLUA_DISABLE_tolua_set_peak__TimerCallback_time
static int tolua_set_peak__TimerCallback_time(lua_State* tolua_S)
{
  peak::TimerCallback* self = (peak::TimerCallback*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'time'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->time = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: script of class  peak::TimerCallback */
#ifndef TOLUA_DISABLE_tolua_get_peak__TimerCallback_script_ptr
static int tolua_get_peak__TimerCallback_script_ptr(lua_State* tolua_S)
{
  peak::TimerCallback* self = (peak::TimerCallback*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'script'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)self->script,"peak::Script");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: script of class  peak::TimerCallback */
#ifndef TOLUA_DISABLE_tolua_set_peak__TimerCallback_script_ptr
static int tolua_set_peak__TimerCallback_script_ptr(lua_State* tolua_S)
{
  peak::TimerCallback* self = (peak::TimerCallback*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'script'",NULL);
  if (!tolua_isusertype(tolua_S,2,"peak::Script",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->script = ((peak::Script*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: function of class  peak::TimerCallback */
#ifndef TOLUA_DISABLE_tolua_get_peak__TimerCallback_function
static int tolua_get_peak__TimerCallback_function(lua_State* tolua_S)
{
  peak::TimerCallback* self = (peak::TimerCallback*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'function'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->function);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: function of class  peak::TimerCallback */
#ifndef TOLUA_DISABLE_tolua_set_peak__TimerCallback_function
static int tolua_set_peak__TimerCallback_function(lua_State* tolua_S)
{
  peak::TimerCallback* self = (peak::TimerCallback*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'function'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->function = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* function: peak::trimString */
#ifndef TOLUA_DISABLE_tolua_peak_peak_trimString00
static int tolua_peak_peak_trimString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string str = ((std::string)  tolua_tocppstring(tolua_S,1,0));
  {
   int tolua_ret = (int)  peak::trimString(&str);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)str);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'trimString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: peak::splitString */
#ifndef TOLUA_DISABLE_tolua_peak_peak_splitString00
static int tolua_peak_peak_splitString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string str = ((std::string)  tolua_tocppstring(tolua_S,1,0));
  char c = ((char)  tolua_tonumber(tolua_S,2,0));
  std::string str1 = ((std::string)  tolua_tocppstring(tolua_S,3,0));
  std::string str2 = ((std::string)  tolua_tocppstring(tolua_S,4,0));
  {
   bool tolua_ret = (bool)  peak::splitString(str,c,&str1,&str2);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)str1);
   tolua_pushcppstring(tolua_S,(const char*)str2);
  }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'splitString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: peak::splitString */
#ifndef TOLUA_DISABLE_tolua_peak_peak_splitString01
static int tolua_peak_peak_splitString01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  std::string str = ((std::string)  tolua_tocppstring(tolua_S,1,0));
  std::string split = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  std::string str1 = ((std::string)  tolua_tocppstring(tolua_S,3,0));
  std::string str2 = ((std::string)  tolua_tocppstring(tolua_S,4,0));
  {
   bool tolua_ret = (bool)  peak::splitString(str,split,&str1,&str2);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)str1);
   tolua_pushcppstring(tolua_S,(const char*)str2);
  }
 }
 return 3;
tolua_lerror:
 return tolua_peak_peak_splitString00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* function: peak::toFloat */
#ifndef TOLUA_DISABLE_tolua_peak_peak_toFloat00
static int tolua_peak_peak_toFloat00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string str = ((std::string)  tolua_tocppstring(tolua_S,1,0));
  {
   float tolua_ret = (float)  peak::toFloat(str);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'toFloat'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: peak::toInt */
#ifndef TOLUA_DISABLE_tolua_peak_peak_toInt00
static int tolua_peak_peak_toInt00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string str = ((std::string)  tolua_tocppstring(tolua_S,1,0));
  {
   int tolua_ret = (int)  peak::toInt(str);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'toInt'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: peak::toVector3D */
#ifndef TOLUA_DISABLE_tolua_peak_peak_toVector3D00
static int tolua_peak_peak_toVector3D00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string str = ((std::string)  tolua_tocppstring(tolua_S,1,0));
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  peak::toVector3D(str);
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Vector3D(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'toVector3D'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: peak::toVector2D */
#ifndef TOLUA_DISABLE_tolua_peak_peak_toVector2D00
static int tolua_peak_peak_toVector2D00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string str = ((std::string)  tolua_tocppstring(tolua_S,1,0));
  {
   peak::Vector2D tolua_ret = (peak::Vector2D)  peak::toVector2D(str);
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Vector2D(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector2D");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector2D));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector2D");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'toVector2D'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: peak::toVector2DI */
#ifndef TOLUA_DISABLE_tolua_peak_peak_toVector2DI00
static int tolua_peak_peak_toVector2DI00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string str = ((std::string)  tolua_tocppstring(tolua_S,1,0));
  {
   peak::Vector2DI tolua_ret = (peak::Vector2DI)  peak::toVector2DI(str);
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Vector2DI(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector2DI");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector2DI));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector2DI");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'toVector2DI'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: peak::toBool */
#ifndef TOLUA_DISABLE_tolua_peak_peak_toBool00
static int tolua_peak_peak_toBool00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string str = ((std::string)  tolua_tocppstring(tolua_S,1,0));
  {
   bool tolua_ret = (bool)  peak::toBool(str);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'toBool'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: peak::toString */
#ifndef TOLUA_DISABLE_tolua_peak_peak_toString00
static int tolua_peak_peak_toString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int v = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   std::string tolua_ret = (std::string)  peak::toString(v);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'toString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: peak::toString */
#ifndef TOLUA_DISABLE_tolua_peak_peak_toString01
static int tolua_peak_peak_toString01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  float v = ((float)  tolua_tonumber(tolua_S,1,0));
  {
   std::string tolua_ret = (std::string)  peak::toString(v);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_toString00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* function: peak::toString */
#ifndef TOLUA_DISABLE_tolua_peak_peak_toString02
static int tolua_peak_peak_toString02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::Vector3D v = *((peak::Vector3D*)  tolua_tousertype(tolua_S,1,0));
  {
   std::string tolua_ret = (std::string)  peak::toString(v);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_toString01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* function: peak::toString */
#ifndef TOLUA_DISABLE_tolua_peak_peak_toString03
static int tolua_peak_peak_toString03(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Vector2D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::Vector2D v = *((peak::Vector2D*)  tolua_tousertype(tolua_S,1,0));
  {
   std::string tolua_ret = (std::string)  peak::toString(v);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_toString02(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* function: peak::toString */
#ifndef TOLUA_DISABLE_tolua_peak_peak_toString04
static int tolua_peak_peak_toString04(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Vector2DI",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::Vector2DI v = *((peak::Vector2DI*)  tolua_tousertype(tolua_S,1,0));
  {
   std::string tolua_ret = (std::string)  peak::toString(v);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_toString03(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* function: peak::toWString */
#ifndef TOLUA_DISABLE_tolua_peak_peak_toWString00
static int tolua_peak_peak_toWString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string s = ((std::string)  tolua_tocppstring(tolua_S,1,0));
  {
   std::wstring tolua_ret = (std::wstring)  peak::toWString(s);
   {
#ifdef __cplusplus
    void* tolua_obj = new std::wstring(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"std::wstring");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(std::wstring));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"std::wstring");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'toWString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: peak::trimFilename */
#ifndef TOLUA_DISABLE_tolua_peak_peak_trimFilename00
static int tolua_peak_peak_trimFilename00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string filename = ((std::string)  tolua_tocppstring(tolua_S,1,0));
  {
   std::string tolua_ret = (std::string)  peak::trimFilename(filename);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'trimFilename'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: peak::getExtension */
#ifndef TOLUA_DISABLE_tolua_peak_peak_getExtension00
static int tolua_peak_peak_getExtension00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string filename = ((std::string)  tolua_tocppstring(tolua_S,1,0));
  {
   std::string tolua_ret = (std::string)  peak::getExtension(filename);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getExtension'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::LevelObject */
#ifndef TOLUA_DISABLE_tolua_peak_peak_LevelObject_delete00
static int tolua_peak_peak_LevelObject_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::LevelObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::LevelObject* self = (peak::LevelObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setName of class  peak::LevelObject */
#ifndef TOLUA_DISABLE_tolua_peak_peak_LevelObject_setName00
static int tolua_peak_peak_LevelObject_setName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::LevelObject",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::LevelObject* self = (peak::LevelObject*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setName'",NULL);
#endif
  {
   self->setName(name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getName of class  peak::LevelObject */
#ifndef TOLUA_DISABLE_tolua_peak_peak_LevelObject_getName00
static int tolua_peak_peak_LevelObject_getName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::LevelObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::LevelObject* self = (peak::LevelObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getName'",NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getName();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getType of class  peak::LevelObject */
#ifndef TOLUA_DISABLE_tolua_peak_peak_LevelObject_getType00
static int tolua_peak_peak_LevelObject_getType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::LevelObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::LevelObject* self = (peak::LevelObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getType'",NULL);
#endif
  {
   peak::LevelObjectType tolua_ret = (peak::LevelObjectType)  self->getType();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Terrain */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Terrain_new00
static int tolua_peak_peak_Terrain_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Terrain",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::SceneNode",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string heightmap = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  peak::SceneNode* parent = ((peak::SceneNode*)  tolua_tousertype(tolua_S,3,0));
  {
   peak::Terrain* tolua_ret = (peak::Terrain*)  new peak::Terrain(heightmap,parent);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Terrain");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Terrain */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Terrain_new00_local
static int tolua_peak_peak_Terrain_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Terrain",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::SceneNode",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string heightmap = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  peak::SceneNode* parent = ((peak::SceneNode*)  tolua_tousertype(tolua_S,3,0));
  {
   peak::Terrain* tolua_ret = (peak::Terrain*)  new peak::Terrain(heightmap,parent);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Terrain");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::Terrain */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Terrain_delete00
static int tolua_peak_peak_Terrain_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Terrain",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Terrain* self = (peak::Terrain*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::SceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SceneNode_new00
static int tolua_peak_peak_SceneNode_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::SceneNode",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::SceneNode",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SceneNode* parent = ((peak::SceneNode*)  tolua_tousertype(tolua_S,2,0));
  {
   peak::SceneNode* tolua_ret = (peak::SceneNode*)  new peak::SceneNode(parent);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::SceneNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::SceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SceneNode_new00_local
static int tolua_peak_peak_SceneNode_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::SceneNode",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::SceneNode",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SceneNode* parent = ((peak::SceneNode*)  tolua_tousertype(tolua_S,2,0));
  {
   peak::SceneNode* tolua_ret = (peak::SceneNode*)  new peak::SceneNode(parent);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::SceneNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::SceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SceneNode_delete00
static int tolua_peak_peak_SceneNode_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SceneNode* self = (peak::SceneNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setPosition of class  peak::SceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SceneNode_setPosition00
static int tolua_peak_peak_SceneNode_setPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SceneNode",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SceneNode* self = (peak::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
  float z = ((float)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPosition'",NULL);
#endif
  {
   self->setPosition(x,y,z);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setRotation of class  peak::SceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SceneNode_setRotation00
static int tolua_peak_peak_SceneNode_setRotation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SceneNode",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SceneNode* self = (peak::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
  float z = ((float)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRotation'",NULL);
#endif
  {
   self->setRotation(x,y,z);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setRotation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setRotation of class  peak::SceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SceneNode_setRotation01
static int tolua_peak_peak_SceneNode_setRotation01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SceneNode",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::SceneNode* self = (peak::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  float w = ((float)  tolua_tonumber(tolua_S,2,0));
  float x = ((float)  tolua_tonumber(tolua_S,3,0));
  float y = ((float)  tolua_tonumber(tolua_S,4,0));
  float z = ((float)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRotation'",NULL);
#endif
  {
   self->setRotation(w,x,y,z);
  }
 }
 return 0;
tolua_lerror:
 return tolua_peak_peak_SceneNode_setRotation00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: setPosition of class  peak::SceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SceneNode_setPosition01
static int tolua_peak_peak_SceneNode_setPosition01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SceneNode",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::SceneNode* self = (peak::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D newpos = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPosition'",NULL);
#endif
  {
   self->setPosition(newpos);
  }
 }
 return 0;
tolua_lerror:
 return tolua_peak_peak_SceneNode_setPosition00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: setRotation of class  peak::SceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SceneNode_setRotation02
static int tolua_peak_peak_SceneNode_setRotation02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SceneNode",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::SceneNode* self = (peak::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D newrot = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRotation'",NULL);
#endif
  {
   self->setRotation(newrot);
  }
 }
 return 0;
tolua_lerror:
 return tolua_peak_peak_SceneNode_setRotation01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: setRotation of class  peak::SceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SceneNode_setRotation03
static int tolua_peak_peak_SceneNode_setRotation03(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SceneNode",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::SceneNode* self = (peak::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  peak::Quaternion newrot = *((peak::Quaternion*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRotation'",NULL);
#endif
  {
   self->setRotation(newrot);
  }
 }
 return 0;
tolua_lerror:
 return tolua_peak_peak_SceneNode_setRotation02(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPosition of class  peak::SceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SceneNode_getPosition00
static int tolua_peak_peak_SceneNode_getPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SceneNode* self = (peak::SceneNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPosition'",NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getPosition();
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Vector3D(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRotationEuler of class  peak::SceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SceneNode_getRotationEuler00
static int tolua_peak_peak_SceneNode_getRotationEuler00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SceneNode* self = (peak::SceneNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRotationEuler'",NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getRotationEuler();
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Vector3D(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRotationEuler'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRotation of class  peak::SceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SceneNode_getRotation00
static int tolua_peak_peak_SceneNode_getRotation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SceneNode* self = (peak::SceneNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRotation'",NULL);
#endif
  {
   peak::Quaternion tolua_ret = (peak::Quaternion)  self->getRotation();
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Quaternion(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Quaternion");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Quaternion));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Quaternion");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRotation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setScale of class  peak::SceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SceneNode_setScale00
static int tolua_peak_peak_SceneNode_setScale00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SceneNode",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SceneNode* self = (peak::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D newscale = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setScale'",NULL);
#endif
  {
   self->setScale(newscale);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setScale'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setParent of class  peak::SceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SceneNode_setParent00
static int tolua_peak_peak_SceneNode_setParent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SceneNode",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::SceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SceneNode* self = (peak::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  peak::SceneNode* newparent = ((peak::SceneNode*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setParent'",NULL);
#endif
  {
   self->setParent(newparent);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setParent'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setVisible of class  peak::SceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SceneNode_setVisible00
static int tolua_peak_peak_SceneNode_setVisible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SceneNode",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SceneNode* self = (peak::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  bool flag = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setVisible'",NULL);
#endif
  {
   self->setVisible(flag);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setVisible'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isVisible of class  peak::SceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SceneNode_isVisible00
static int tolua_peak_peak_SceneNode_isVisible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SceneNode* self = (peak::SceneNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isVisible'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isVisible();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isVisible'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setMaterial of class  peak::SceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SceneNode_setMaterial00
static int tolua_peak_peak_SceneNode_setMaterial00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SceneNode",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SceneNode* self = (peak::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  std::string material = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMaterial'",NULL);
#endif
  {
   self->setMaterial(material);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMaterial'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setMaterial of class  peak::SceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SceneNode_setMaterial01
static int tolua_peak_peak_SceneNode_setMaterial01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SceneNode",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Material",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::SceneNode* self = (peak::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  peak::Material* material = ((peak::Material*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMaterial'",NULL);
#endif
  {
   self->setMaterial(*material);
  }
 }
 return 0;
tolua_lerror:
 return tolua_peak_peak_SceneNode_setMaterial00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMaterial of class  peak::SceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SceneNode_getMaterial00
static int tolua_peak_peak_SceneNode_getMaterial00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SceneNode* self = (peak::SceneNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMaterial'",NULL);
#endif
  {
   peak::Material tolua_ret = (peak::Material)  self->getMaterial();
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Material(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Material");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Material));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Material");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMaterial'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: reloadMaterial of class  peak::SceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SceneNode_reloadMaterial00
static int tolua_peak_peak_SceneNode_reloadMaterial00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SceneNode* self = (peak::SceneNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'reloadMaterial'",NULL);
#endif
  {
   self->reloadMaterial();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'reloadMaterial'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Geometry */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Geometry_new00
static int tolua_peak_peak_Geometry_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Geometry",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Geometry* tolua_ret = (peak::Geometry*)  new peak::Geometry();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Geometry");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Geometry */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Geometry_new00_local
static int tolua_peak_peak_Geometry_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Geometry",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Geometry* tolua_ret = (peak::Geometry*)  new peak::Geometry();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Geometry");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::Geometry */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Geometry_delete00
static int tolua_peak_peak_Geometry_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Geometry",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Geometry* self = (peak::Geometry*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: destroy of class  peak::Geometry */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Geometry_destroy00
static int tolua_peak_peak_Geometry_destroy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Geometry",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Geometry* self = (peak::Geometry*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroy'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->destroy();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'destroy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setPosition of class  peak::Geometry */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Geometry_setPosition00
static int tolua_peak_peak_Geometry_setPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Geometry",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Geometry* self = (peak::Geometry*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D pos = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPosition'",NULL);
#endif
  {
   self->setPosition(pos);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setBody of class  peak::Geometry */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Geometry_setBody00
static int tolua_peak_peak_Geometry_setBody00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Geometry",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Body",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Geometry* self = (peak::Geometry*)  tolua_tousertype(tolua_S,1,0);
  peak::Body* body = ((peak::Body*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setBody'",NULL);
#endif
  {
   self->setBody(body);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setBody'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getBody of class  peak::Geometry */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Geometry_getBody00
static int tolua_peak_peak_Geometry_getBody00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Geometry",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Geometry* self = (peak::Geometry*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getBody'",NULL);
#endif
  {
   peak::Body* tolua_ret = (peak::Body*)  self->getBody();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Body");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getBody'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getCollisionInfo of class  peak::Geometry */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Geometry_getCollisionInfo00
static int tolua_peak_peak_Geometry_getCollisionInfo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Geometry",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Geometry* self = (peak::Geometry*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCollisionInfo'",NULL);
#endif
  {
   peak::CollisionInfo* tolua_ret = (peak::CollisionInfo*)  self->getCollisionInfo();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::CollisionInfo");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCollisionInfo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getGeom of class  peak::Geometry */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Geometry_getGeom00
static int tolua_peak_peak_Geometry_getGeom00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Geometry",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Geometry* self = (peak::Geometry*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getGeom'",NULL);
#endif
  {
   dGeomID tolua_ret = (dGeomID)  self->getGeom();
   {
#ifdef __cplusplus
    void* tolua_obj = new dGeomID(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"dGeomID");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(dGeomID));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"dGeomID");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getGeom'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::TerrainObject */
#ifndef TOLUA_DISABLE_tolua_peak_peak_TerrainObject_new00
static int tolua_peak_peak_TerrainObject_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::TerrainObject",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,6,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,7,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string heightmap = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  std::string material = ((std::string)  tolua_tocppstring(tolua_S,3,0));
  peak::Vector3D position = *((peak::Vector3D*)  tolua_tousertype(tolua_S,4,0));
  peak::Vector3D scale = *((peak::Vector3D*)  tolua_tousertype(tolua_S,5,0));
  bool server = ((bool)  tolua_toboolean(tolua_S,6,0));
  bool client = ((bool)  tolua_toboolean(tolua_S,7,0));
  {
   peak::TerrainObject* tolua_ret = (peak::TerrainObject*)  new peak::TerrainObject(heightmap,material,position,scale,server,client);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::TerrainObject");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::TerrainObject */
#ifndef TOLUA_DISABLE_tolua_peak_peak_TerrainObject_new00_local
static int tolua_peak_peak_TerrainObject_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::TerrainObject",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,6,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,7,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string heightmap = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  std::string material = ((std::string)  tolua_tocppstring(tolua_S,3,0));
  peak::Vector3D position = *((peak::Vector3D*)  tolua_tousertype(tolua_S,4,0));
  peak::Vector3D scale = *((peak::Vector3D*)  tolua_tousertype(tolua_S,5,0));
  bool server = ((bool)  tolua_toboolean(tolua_S,6,0));
  bool client = ((bool)  tolua_toboolean(tolua_S,7,0));
  {
   peak::TerrainObject* tolua_ret = (peak::TerrainObject*)  new peak::TerrainObject(heightmap,material,position,scale,server,client);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::TerrainObject");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::TerrainObject */
#ifndef TOLUA_DISABLE_tolua_peak_peak_TerrainObject_delete00
static int tolua_peak_peak_TerrainObject_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::TerrainObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::TerrainObject* self = (peak::TerrainObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getType of class  peak::TerrainObject */
#ifndef TOLUA_DISABLE_tolua_peak_peak_TerrainObject_getType00
static int tolua_peak_peak_TerrainObject_getType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::TerrainObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::TerrainObject* self = (peak::TerrainObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getType'",NULL);
#endif
  {
   peak::LevelObjectType tolua_ret = (peak::LevelObjectType)  self->getType();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::SceneNodeObject */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SceneNodeObject_new00
static int tolua_peak_peak_SceneNodeObject_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::SceneNodeObject",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::SceneNode",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::Geometry",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SceneNode* node = ((peak::SceneNode*)  tolua_tousertype(tolua_S,2,0));
  peak::Geometry* geom = ((peak::Geometry*)  tolua_tousertype(tolua_S,3,0));
  {
   peak::SceneNodeObject* tolua_ret = (peak::SceneNodeObject*)  new peak::SceneNodeObject(node,geom);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::SceneNodeObject");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::SceneNodeObject */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SceneNodeObject_new00_local
static int tolua_peak_peak_SceneNodeObject_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::SceneNodeObject",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::SceneNode",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::Geometry",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SceneNode* node = ((peak::SceneNode*)  tolua_tousertype(tolua_S,2,0));
  peak::Geometry* geom = ((peak::Geometry*)  tolua_tousertype(tolua_S,3,0));
  {
   peak::SceneNodeObject* tolua_ret = (peak::SceneNodeObject*)  new peak::SceneNodeObject(node,geom);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::SceneNodeObject");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::SceneNodeObject */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SceneNodeObject_delete00
static int tolua_peak_peak_SceneNodeObject_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SceneNodeObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SceneNodeObject* self = (peak::SceneNodeObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getType of class  peak::SceneNodeObject */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SceneNodeObject_getType00
static int tolua_peak_peak_SceneNodeObject_getType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SceneNodeObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SceneNodeObject* self = (peak::SceneNodeObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getType'",NULL);
#endif
  {
   peak::LevelObjectType tolua_ret = (peak::LevelObjectType)  self->getType();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::PointGroupObject */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PointGroupObject_new00
static int tolua_peak_peak_PointGroupObject_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::PointGroupObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::PointGroupObject* tolua_ret = (peak::PointGroupObject*)  new peak::PointGroupObject();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::PointGroupObject");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::PointGroupObject */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PointGroupObject_new00_local
static int tolua_peak_peak_PointGroupObject_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::PointGroupObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::PointGroupObject* tolua_ret = (peak::PointGroupObject*)  new peak::PointGroupObject();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::PointGroupObject");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::PointGroupObject */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PointGroupObject_delete00
static int tolua_peak_peak_PointGroupObject_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::PointGroupObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::PointGroupObject* self = (peak::PointGroupObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addPoint of class  peak::PointGroupObject */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PointGroupObject_addPoint00
static int tolua_peak_peak_PointGroupObject_addPoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::PointGroupObject",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::PointGroupObject* self = (peak::PointGroupObject*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D point = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addPoint'",NULL);
#endif
  {
   self->addPoint(point);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addPoint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPointCount of class  peak::PointGroupObject */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PointGroupObject_getPointCount00
static int tolua_peak_peak_PointGroupObject_getPointCount00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::PointGroupObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::PointGroupObject* self = (peak::PointGroupObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPointCount'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getPointCount();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPointCount'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPoint of class  peak::PointGroupObject */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PointGroupObject_getPoint00
static int tolua_peak_peak_PointGroupObject_getPoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::PointGroupObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::PointGroupObject* self = (peak::PointGroupObject*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPoint'",NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getPoint(index);
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Vector3D(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPoint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: deletePoint of class  peak::PointGroupObject */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PointGroupObject_deletePoint00
static int tolua_peak_peak_PointGroupObject_deletePoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::PointGroupObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::PointGroupObject* self = (peak::PointGroupObject*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'deletePoint'",NULL);
#endif
  {
   self->deletePoint(index);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'deletePoint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addRotation of class  peak::PointGroupObject */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PointGroupObject_addRotation00
static int tolua_peak_peak_PointGroupObject_addRotation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::PointGroupObject",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::PointGroupObject* self = (peak::PointGroupObject*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D rotation = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addRotation'",NULL);
#endif
  {
   self->addRotation(rotation);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addRotation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRotationCount of class  peak::PointGroupObject */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PointGroupObject_getRotationCount00
static int tolua_peak_peak_PointGroupObject_getRotationCount00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::PointGroupObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::PointGroupObject* self = (peak::PointGroupObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRotationCount'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getRotationCount();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRotationCount'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRotation of class  peak::PointGroupObject */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PointGroupObject_getRotation00
static int tolua_peak_peak_PointGroupObject_getRotation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::PointGroupObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::PointGroupObject* self = (peak::PointGroupObject*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRotation'",NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getRotation(index);
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Vector3D(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRotation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: deleteRotation of class  peak::PointGroupObject */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PointGroupObject_deleteRotation00
static int tolua_peak_peak_PointGroupObject_deleteRotation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::PointGroupObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::PointGroupObject* self = (peak::PointGroupObject*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'deleteRotation'",NULL);
#endif
  {
   self->deleteRotation(index);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'deleteRotation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getType of class  peak::PointGroupObject */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PointGroupObject_getType00
static int tolua_peak_peak_PointGroupObject_getType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::PointGroupObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::PointGroupObject* self = (peak::PointGroupObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getType'",NULL);
#endif
  {
   peak::LevelObjectType tolua_ret = (peak::LevelObjectType)  self->getType();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get of class  peak::Level */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Level_get00
static int tolua_peak_peak_Level_get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Level",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Level* tolua_ret = (peak::Level*)  peak::Level::get();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Level");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::Level */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Level_delete00
static int tolua_peak_peak_Level_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Level",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Level* self = (peak::Level*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: init of class  peak::Level */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Level_init00
static int tolua_peak_peak_Level_init00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Level",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Level* self = (peak::Level*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  bool server = ((bool)  tolua_toboolean(tolua_S,3,0));
  bool client = ((bool)  tolua_toboolean(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->init(name,server,client);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'init'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: destroy of class  peak::Level */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Level_destroy00
static int tolua_peak_peak_Level_destroy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Level",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Level* self = (peak::Level*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroy'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->destroy();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'destroy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getObject of class  peak::Level */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Level_getObject00
static int tolua_peak_peak_Level_getObject00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Level",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Level* self = (peak::Level*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getObject'",NULL);
#endif
  {
   peak::LevelObject* tolua_ret = (peak::LevelObject*)  self->getObject(name);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::LevelObject");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getObject'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPointGroup of class  peak::Level */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Level_getPointGroup00
static int tolua_peak_peak_Level_getPointGroup00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Level",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Level* self = (peak::Level*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPointGroup'",NULL);
#endif
  {
   peak::PointGroupObject* tolua_ret = (peak::PointGroupObject*)  self->getPointGroup(name);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::PointGroupObject");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPointGroup'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getLevelCount of class  peak::Level */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Level_getLevelCount00
static int tolua_peak_peak_Level_getLevelCount00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Level",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Level* self = (peak::Level*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLevelCount'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getLevelCount();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLevelCount'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getLevelName of class  peak::Level */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Level_getLevelName00
static int tolua_peak_peak_Level_getLevelName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Level",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Level* self = (peak::Level*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLevelName'",NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getLevelName(index);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLevelName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get of class  peak::SettingsManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SettingsManager_get00
static int tolua_peak_peak_SettingsManager_get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::SettingsManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::SettingsManager* tolua_ret = (peak::SettingsManager*)  peak::SettingsManager::get();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::SettingsManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: load of class  peak::SettingsManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SettingsManager_load00
static int tolua_peak_peak_SettingsManager_load00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SettingsManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SettingsManager* self = (peak::SettingsManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'load'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->load();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: save of class  peak::SettingsManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SettingsManager_save00
static int tolua_peak_peak_SettingsManager_save00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SettingsManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SettingsManager* self = (peak::SettingsManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'save'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->save();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'save'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getString of class  peak::SettingsManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SettingsManager_getString00
static int tolua_peak_peak_SettingsManager_getString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SettingsManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SettingsManager* self = (peak::SettingsManager*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getString'",NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getString(name);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getBool of class  peak::SettingsManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SettingsManager_getBool00
static int tolua_peak_peak_SettingsManager_getBool00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SettingsManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SettingsManager* self = (peak::SettingsManager*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getBool'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getBool(name);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getBool'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInt of class  peak::SettingsManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SettingsManager_getInt00
static int tolua_peak_peak_SettingsManager_getInt00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SettingsManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SettingsManager* self = (peak::SettingsManager*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getInt'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getInt(name);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInt'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getWString of class  peak::SettingsManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SettingsManager_getWString00
static int tolua_peak_peak_SettingsManager_getWString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SettingsManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SettingsManager* self = (peak::SettingsManager*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWString'",NULL);
#endif
  {
   std::wstring tolua_ret = (std::wstring)  self->getWString(name);
   {
#ifdef __cplusplus
    void* tolua_obj = new std::wstring(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"std::wstring");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(std::wstring));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"std::wstring");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getWString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVector2D of class  peak::SettingsManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SettingsManager_getVector2D00
static int tolua_peak_peak_SettingsManager_getVector2D00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SettingsManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SettingsManager* self = (peak::SettingsManager*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVector2D'",NULL);
#endif
  {
   peak::Vector2D tolua_ret = (peak::Vector2D)  self->getVector2D(name);
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Vector2D(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector2D");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector2D));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector2D");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVector2D'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVector2DI of class  peak::SettingsManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SettingsManager_getVector2DI00
static int tolua_peak_peak_SettingsManager_getVector2DI00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SettingsManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SettingsManager* self = (peak::SettingsManager*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVector2DI'",NULL);
#endif
  {
   peak::Vector2DI tolua_ret = (peak::Vector2DI)  self->getVector2DI(name);
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Vector2DI(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector2DI");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector2DI));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector2DI");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVector2DI'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVector3D of class  peak::SettingsManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SettingsManager_getVector3D00
static int tolua_peak_peak_SettingsManager_getVector3D00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SettingsManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SettingsManager* self = (peak::SettingsManager*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVector3D'",NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getVector3D(name);
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Vector3D(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVector3D'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setString of class  peak::SettingsManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SettingsManager_setString00
static int tolua_peak_peak_SettingsManager_setString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SettingsManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SettingsManager* self = (peak::SettingsManager*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  std::string value = ((std::string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setString'",NULL);
#endif
  {
   self->setString(name,value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setBool of class  peak::SettingsManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SettingsManager_setBool00
static int tolua_peak_peak_SettingsManager_setBool00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SettingsManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SettingsManager* self = (peak::SettingsManager*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  bool value = ((bool)  tolua_toboolean(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setBool'",NULL);
#endif
  {
   self->setBool(name,value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setBool'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setInt of class  peak::SettingsManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SettingsManager_setInt00
static int tolua_peak_peak_SettingsManager_setInt00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SettingsManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SettingsManager* self = (peak::SettingsManager*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  int value = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setInt'",NULL);
#endif
  {
   self->setInt(name,value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setInt'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setVector2D of class  peak::SettingsManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SettingsManager_setVector2D00
static int tolua_peak_peak_SettingsManager_setVector2D00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SettingsManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::Vector2D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SettingsManager* self = (peak::SettingsManager*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  peak::Vector2D value = *((peak::Vector2D*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setVector2D'",NULL);
#endif
  {
   self->setVector2D(name,value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setVector2D'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setVector2DI of class  peak::SettingsManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SettingsManager_setVector2DI00
static int tolua_peak_peak_SettingsManager_setVector2DI00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SettingsManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::Vector2DI",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SettingsManager* self = (peak::SettingsManager*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  peak::Vector2DI value = *((peak::Vector2DI*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setVector2DI'",NULL);
#endif
  {
   self->setVector2DI(name,value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setVector2DI'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setVector3D of class  peak::SettingsManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SettingsManager_setVector3D00
static int tolua_peak_peak_SettingsManager_setVector3D00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SettingsManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SettingsManager* self = (peak::SettingsManager*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  peak::Vector3D value = *((peak::Vector3D*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setVector3D'",NULL);
#endif
  {
   self->setVector3D(name,value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setVector3D'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getItemList of class  peak::SettingsManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SettingsManager_getItemList00
static int tolua_peak_peak_SettingsManager_getItemList00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SettingsManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SettingsManager* self = (peak::SettingsManager*)  tolua_tousertype(tolua_S,1,0);
  std::string section = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getItemList'",NULL);
#endif
  {
   std::vector<std::string> tolua_ret = (std::vector<std::string>)  self->getItemList(section);
   {
#ifdef __cplusplus
    void* tolua_obj = new std::vector<std::string>(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"std::vector<std::string>");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(std::vector<std::string>));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"std::vector<std::string>");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getItemList'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: deleteSetting of class  peak::SettingsManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SettingsManager_deleteSetting00
static int tolua_peak_peak_SettingsManager_deleteSetting00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SettingsManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SettingsManager* self = (peak::SettingsManager*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'deleteSetting'",NULL);
#endif
  {
   self->deleteSetting(name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'deleteSetting'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: doesExist of class  peak::SettingsManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SettingsManager_doesExist00
static int tolua_peak_peak_SettingsManager_doesExist00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SettingsManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SettingsManager* self = (peak::SettingsManager*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'doesExist'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->doesExist(name);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'doesExist'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::BroadcastClient */
#ifndef TOLUA_DISABLE_tolua_peak_peak_BroadcastClient_new00
static int tolua_peak_peak_BroadcastClient_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::BroadcastClient",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::BroadcastClient* tolua_ret = (peak::BroadcastClient*)  new peak::BroadcastClient();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::BroadcastClient");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::BroadcastClient */
#ifndef TOLUA_DISABLE_tolua_peak_peak_BroadcastClient_new00_local
static int tolua_peak_peak_BroadcastClient_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::BroadcastClient",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::BroadcastClient* tolua_ret = (peak::BroadcastClient*)  new peak::BroadcastClient();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::BroadcastClient");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::BroadcastClient */
#ifndef TOLUA_DISABLE_tolua_peak_peak_BroadcastClient_delete00
static int tolua_peak_peak_BroadcastClient_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::BroadcastClient",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::BroadcastClient* self = (peak::BroadcastClient*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: start of class  peak::BroadcastClient */
#ifndef TOLUA_DISABLE_tolua_peak_peak_BroadcastClient_start00
static int tolua_peak_peak_BroadcastClient_start00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::BroadcastClient",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::BroadcastClient* self = (peak::BroadcastClient*)  tolua_tousertype(tolua_S,1,0);
  int port = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'start'",NULL);
#endif
  {
   self->start(port);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'start'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: stop of class  peak::BroadcastClient */
#ifndef TOLUA_DISABLE_tolua_peak_peak_BroadcastClient_stop00
static int tolua_peak_peak_BroadcastClient_stop00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::BroadcastClient",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::BroadcastClient* self = (peak::BroadcastClient*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'stop'",NULL);
#endif
  {
   self->stop();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'stop'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clearList of class  peak::BroadcastClient */
#ifndef TOLUA_DISABLE_tolua_peak_peak_BroadcastClient_clearList00
static int tolua_peak_peak_BroadcastClient_clearList00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::BroadcastClient",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::BroadcastClient* self = (peak::BroadcastClient*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clearList'",NULL);
#endif
  {
   self->clearList();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clearList'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getServerCount of class  peak::BroadcastClient */
#ifndef TOLUA_DISABLE_tolua_peak_peak_BroadcastClient_getServerCount00
static int tolua_peak_peak_BroadcastClient_getServerCount00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::BroadcastClient",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::BroadcastClient* self = (peak::BroadcastClient*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getServerCount'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getServerCount();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getServerCount'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getServerInfo of class  peak::BroadcastClient */
#ifndef TOLUA_DISABLE_tolua_peak_peak_BroadcastClient_getServerInfo00
static int tolua_peak_peak_BroadcastClient_getServerInfo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::BroadcastClient",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::BroadcastClient* self = (peak::BroadcastClient*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getServerInfo'",NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getServerInfo(index);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getServerInfo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getServerAddress of class  peak::BroadcastClient */
#ifndef TOLUA_DISABLE_tolua_peak_peak_BroadcastClient_getServerAddress00
static int tolua_peak_peak_BroadcastClient_getServerAddress00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::BroadcastClient",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::BroadcastClient* self = (peak::BroadcastClient*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getServerAddress'",NULL);
#endif
  {
   peak::Address tolua_ret = (peak::Address)  self->getServerAddress(index);
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Address(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Address");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Address));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Address");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getServerAddress'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setCallback of class  peak::BroadcastClient */
#ifndef TOLUA_DISABLE_tolua_peak_peak_BroadcastClient_setCallback00
static int tolua_peak_peak_BroadcastClient_setCallback00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::BroadcastClient",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Script",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::BroadcastClient* self = (peak::BroadcastClient*)  tolua_tousertype(tolua_S,1,0);
  peak::Script* script = ((peak::Script*)  tolua_tousertype(tolua_S,2,0));
  std::string function = ((std::string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setCallback'",NULL);
#endif
  {
   self->setCallback(script,function);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setCallback'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: doWork of class  peak::BroadcastClient */
#ifndef TOLUA_DISABLE_tolua_peak_peak_BroadcastClient_doWork00
static int tolua_peak_peak_BroadcastClient_doWork00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::BroadcastClient",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::BroadcastClient* self = (peak::BroadcastClient*)  tolua_tousertype(tolua_S,1,0);
  float msecs = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'doWork'",NULL);
#endif
  {
   self->doWork(msecs);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'doWork'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: doAllWork of class  peak::BroadcastClient */
#ifndef TOLUA_DISABLE_tolua_peak_peak_BroadcastClient_doAllWork00
static int tolua_peak_peak_BroadcastClient_doAllWork00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::BroadcastClient",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float msecs = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   peak::BroadcastClient::doAllWork(msecs);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'doAllWork'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::CubeSceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_CubeSceneNode_new00
static int tolua_peak_peak_CubeSceneNode_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::CubeSceneNode",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::SceneNode",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float size = ((float)  tolua_tonumber(tolua_S,2,1));
  peak::SceneNode* parent = ((peak::SceneNode*)  tolua_tousertype(tolua_S,3,0));
  {
   peak::CubeSceneNode* tolua_ret = (peak::CubeSceneNode*)  new peak::CubeSceneNode(size,parent);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::CubeSceneNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::CubeSceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_CubeSceneNode_new00_local
static int tolua_peak_peak_CubeSceneNode_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::CubeSceneNode",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::SceneNode",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float size = ((float)  tolua_tonumber(tolua_S,2,1));
  peak::SceneNode* parent = ((peak::SceneNode*)  tolua_tousertype(tolua_S,3,0));
  {
   peak::CubeSceneNode* tolua_ret = (peak::CubeSceneNode*)  new peak::CubeSceneNode(size,parent);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::CubeSceneNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::CameraSceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_CameraSceneNode_new00
static int tolua_peak_peak_CameraSceneNode_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::CameraSceneNode",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::SceneNode",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SceneNode* parent = ((peak::SceneNode*)  tolua_tousertype(tolua_S,2,0));
  {
   peak::CameraSceneNode* tolua_ret = (peak::CameraSceneNode*)  new peak::CameraSceneNode(parent);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::CameraSceneNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::CameraSceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_CameraSceneNode_new00_local
static int tolua_peak_peak_CameraSceneNode_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::CameraSceneNode",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::SceneNode",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SceneNode* parent = ((peak::SceneNode*)  tolua_tousertype(tolua_S,2,0));
  {
   peak::CameraSceneNode* tolua_ret = (peak::CameraSceneNode*)  new peak::CameraSceneNode(parent);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::CameraSceneNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::CameraSceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_CameraSceneNode_delete00
static int tolua_peak_peak_CameraSceneNode_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::CameraSceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::CameraSceneNode* self = (peak::CameraSceneNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setNearValue of class  peak::CameraSceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_CameraSceneNode_setNearValue00
static int tolua_peak_peak_CameraSceneNode_setNearValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::CameraSceneNode",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::CameraSceneNode* self = (peak::CameraSceneNode*)  tolua_tousertype(tolua_S,1,0);
  float nearvalue = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setNearValue'",NULL);
#endif
  {
   self->setNearValue(nearvalue);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setNearValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getNearValue of class  peak::CameraSceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_CameraSceneNode_getNearValue00
static int tolua_peak_peak_CameraSceneNode_getNearValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::CameraSceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::CameraSceneNode* self = (peak::CameraSceneNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getNearValue'",NULL);
#endif
  {
   float tolua_ret = (float)  self->getNearValue();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getNearValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setFarValue of class  peak::CameraSceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_CameraSceneNode_setFarValue00
static int tolua_peak_peak_CameraSceneNode_setFarValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::CameraSceneNode",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::CameraSceneNode* self = (peak::CameraSceneNode*)  tolua_tousertype(tolua_S,1,0);
  float nearvalue = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setFarValue'",NULL);
#endif
  {
   self->setFarValue(nearvalue);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setFarValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getFarValue of class  peak::CameraSceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_CameraSceneNode_getFarValue00
static int tolua_peak_peak_CameraSceneNode_getFarValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::CameraSceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::CameraSceneNode* self = (peak::CameraSceneNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFarValue'",NULL);
#endif
  {
   float tolua_ret = (float)  self->getFarValue();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFarValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setOrthogonal of class  peak::CameraSceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_CameraSceneNode_setOrthogonal00
static int tolua_peak_peak_CameraSceneNode_setOrthogonal00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::CameraSceneNode",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::CameraSceneNode* self = (peak::CameraSceneNode*)  tolua_tousertype(tolua_S,1,0);
  bool orthogonal = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setOrthogonal'",NULL);
#endif
  {
   self->setOrthogonal(orthogonal);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setOrthogonal'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isOrthogonal of class  peak::CameraSceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_CameraSceneNode_isOrthogonal00
static int tolua_peak_peak_CameraSceneNode_isOrthogonal00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::CameraSceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::CameraSceneNode* self = (peak::CameraSceneNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isOrthogonal'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isOrthogonal();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isOrthogonal'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setFOV of class  peak::CameraSceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_CameraSceneNode_setFOV00
static int tolua_peak_peak_CameraSceneNode_setFOV00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::CameraSceneNode",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::CameraSceneNode* self = (peak::CameraSceneNode*)  tolua_tousertype(tolua_S,1,0);
  float fov = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setFOV'",NULL);
#endif
  {
   self->setFOV(fov);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setFOV'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getFOV of class  peak::CameraSceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_CameraSceneNode_getFOV00
static int tolua_peak_peak_CameraSceneNode_getFOV00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::CameraSceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::CameraSceneNode* self = (peak::CameraSceneNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFOV'",NULL);
#endif
  {
   float tolua_ret = (float)  self->getFOV();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFOV'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAspectRatio of class  peak::CameraSceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_CameraSceneNode_setAspectRatio00
static int tolua_peak_peak_CameraSceneNode_setAspectRatio00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::CameraSceneNode",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::CameraSceneNode* self = (peak::CameraSceneNode*)  tolua_tousertype(tolua_S,1,0);
  float ratio = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAspectRatio'",NULL);
#endif
  {
   self->setAspectRatio(ratio);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAspectRatio'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAspectRatio of class  peak::CameraSceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_CameraSceneNode_getAspectRatio00
static int tolua_peak_peak_CameraSceneNode_getAspectRatio00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::CameraSceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::CameraSceneNode* self = (peak::CameraSceneNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAspectRatio'",NULL);
#endif
  {
   float tolua_ret = (float)  self->getAspectRatio();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAspectRatio'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get of class  peak::GraphicsEngine */
#ifndef TOLUA_DISABLE_tolua_peak_peak_GraphicsEngine_get00
static int tolua_peak_peak_GraphicsEngine_get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::GraphicsEngine",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::GraphicsEngine* tolua_ret = (peak::GraphicsEngine*)  peak::GraphicsEngine::get();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::GraphicsEngine");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: init of class  peak::GraphicsEngine */
#ifndef TOLUA_DISABLE_tolua_peak_peak_GraphicsEngine_init00
static int tolua_peak_peak_GraphicsEngine_init00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::GraphicsEngine",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::GraphicsEngine* self = (peak::GraphicsEngine*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->init();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'init'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: shutdown of class  peak::GraphicsEngine */
#ifndef TOLUA_DISABLE_tolua_peak_peak_GraphicsEngine_shutdown00
static int tolua_peak_peak_GraphicsEngine_shutdown00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::GraphicsEngine",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::GraphicsEngine* self = (peak::GraphicsEngine*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'shutdown'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->shutdown();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'shutdown'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: renderFrame of class  peak::GraphicsEngine */
#ifndef TOLUA_DISABLE_tolua_peak_peak_GraphicsEngine_renderFrame00
static int tolua_peak_peak_GraphicsEngine_renderFrame00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::GraphicsEngine",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::GraphicsEngine* self = (peak::GraphicsEngine*)  tolua_tousertype(tolua_S,1,0);
  float msecs = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'renderFrame'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->renderFrame(msecs);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'renderFrame'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getStandardCamera of class  peak::GraphicsEngine */
#ifndef TOLUA_DISABLE_tolua_peak_peak_GraphicsEngine_getStandardCamera00
static int tolua_peak_peak_GraphicsEngine_getStandardCamera00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::GraphicsEngine",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::GraphicsEngine* self = (peak::GraphicsEngine*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getStandardCamera'",NULL);
#endif
  {
   peak::CameraSceneNode* tolua_ret = (peak::CameraSceneNode*)  self->getStandardCamera();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::CameraSceneNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getStandardCamera'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Image */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Image_new00
static int tolua_peak_peak_Image_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Image",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Image* tolua_ret = (peak::Image*)  new peak::Image();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Image");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Image */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Image_new00_local
static int tolua_peak_peak_Image_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Image",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Image* tolua_ret = (peak::Image*)  new peak::Image();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Image");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Image */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Image_new01
static int tolua_peak_peak_Image_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Image",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  std::string imagename = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  {
   peak::Image* tolua_ret = (peak::Image*)  new peak::Image(imagename);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Image");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Image_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Image */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Image_new01_local
static int tolua_peak_peak_Image_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Image",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  std::string imagename = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  {
   peak::Image* tolua_ret = (peak::Image*)  new peak::Image(imagename);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Image");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Image_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getDimension of class  peak::Image */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Image_getDimension00
static int tolua_peak_peak_Image_getDimension00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Image",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Image* self = (peak::Image*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDimension'",NULL);
#endif
  {
   peak::Vector2DI tolua_ret = (peak::Vector2DI)  self->getDimension();
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Vector2DI(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector2DI");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector2DI));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector2DI");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getDimension'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: load of class  peak::Image */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Image_load00
static int tolua_peak_peak_Image_load00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Image",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Image* self = (peak::Image*)  tolua_tousertype(tolua_S,1,0);
  std::string imagename = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'load'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->load(imagename);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPixel of class  peak::Image */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Image_getPixel00
static int tolua_peak_peak_Image_getPixel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Image",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Vector2DI",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Image* self = (peak::Image*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector2DI pixel = *((peak::Vector2DI*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPixel'",NULL);
#endif
  {
   peak::Color tolua_ret = (peak::Color)  self->getPixel(pixel);
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Color(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Color");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Color));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Color");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPixel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPixel of class  peak::Image */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Image_getPixel01
static int tolua_peak_peak_Image_getPixel01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Image",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::Image* self = (peak::Image*)  tolua_tousertype(tolua_S,1,0);
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPixel'",NULL);
#endif
  {
   peak::Color tolua_ret = (peak::Color)  self->getPixel(x,y);
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Color(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Color");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Color));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Color");
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Image_getPixel00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::MeshSceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_MeshSceneNode_new00
static int tolua_peak_peak_MeshSceneNode_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::MeshSceneNode",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"peak::SceneNode",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string mesh = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  bool animated = ((bool)  tolua_toboolean(tolua_S,3,false));
  peak::SceneNode* parent = ((peak::SceneNode*)  tolua_tousertype(tolua_S,4,0));
  {
   peak::MeshSceneNode* tolua_ret = (peak::MeshSceneNode*)  new peak::MeshSceneNode(mesh,animated,parent);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::MeshSceneNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::MeshSceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_MeshSceneNode_new00_local
static int tolua_peak_peak_MeshSceneNode_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::MeshSceneNode",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"peak::SceneNode",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string mesh = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  bool animated = ((bool)  tolua_toboolean(tolua_S,3,false));
  peak::SceneNode* parent = ((peak::SceneNode*)  tolua_tousertype(tolua_S,4,0));
  {
   peak::MeshSceneNode* tolua_ret = (peak::MeshSceneNode*)  new peak::MeshSceneNode(mesh,animated,parent);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::MeshSceneNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::MeshSceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_MeshSceneNode_delete00
static int tolua_peak_peak_MeshSceneNode_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::MeshSceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::MeshSceneNode* self = (peak::MeshSceneNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getEndFrame of class  peak::MeshSceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_MeshSceneNode_getEndFrame00
static int tolua_peak_peak_MeshSceneNode_getEndFrame00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::MeshSceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::MeshSceneNode* self = (peak::MeshSceneNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getEndFrame'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getEndFrame();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getEndFrame'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getFrameNr of class  peak::MeshSceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_MeshSceneNode_getFrameNr00
static int tolua_peak_peak_MeshSceneNode_getFrameNr00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::MeshSceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::MeshSceneNode* self = (peak::MeshSceneNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFrameNr'",NULL);
#endif
  {
   float tolua_ret = (float)  self->getFrameNr();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFrameNr'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getStartFrame of class  peak::MeshSceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_MeshSceneNode_getStartFrame00
static int tolua_peak_peak_MeshSceneNode_getStartFrame00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::MeshSceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::MeshSceneNode* self = (peak::MeshSceneNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getStartFrame'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getStartFrame();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getStartFrame'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setLoopMode of class  peak::MeshSceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_MeshSceneNode_setLoopMode00
static int tolua_peak_peak_MeshSceneNode_setLoopMode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::MeshSceneNode",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::MeshSceneNode* self = (peak::MeshSceneNode*)  tolua_tousertype(tolua_S,1,0);
  bool flag = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setLoopMode'",NULL);
#endif
  {
   self->setLoopMode(flag);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setLoopMode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAnimationSpeed of class  peak::MeshSceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_MeshSceneNode_setAnimationSpeed00
static int tolua_peak_peak_MeshSceneNode_setAnimationSpeed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::MeshSceneNode",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::MeshSceneNode* self = (peak::MeshSceneNode*)  tolua_tousertype(tolua_S,1,0);
  float speed = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAnimationSpeed'",NULL);
#endif
  {
   self->setAnimationSpeed(speed);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAnimationSpeed'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Material */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Material_new00
static int tolua_peak_peak_Material_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Material",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Material* tolua_ret = (peak::Material*)  new peak::Material();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Material");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Material */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Material_new00_local
static int tolua_peak_peak_Material_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Material",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Material* tolua_ret = (peak::Material*)  new peak::Material();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Material");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::Material */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Material_delete00
static int tolua_peak_peak_Material_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Material",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Material* self = (peak::Material*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: load of class  peak::Material */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Material_load00
static int tolua_peak_peak_Material_load00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Material",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Material* self = (peak::Material*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'load'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->load(name);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setType of class  peak::Material */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Material_setType00
static int tolua_peak_peak_Material_setType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Material",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Material* self = (peak::Material*)  tolua_tousertype(tolua_S,1,0);
  peak::MaterialType type = ((peak::MaterialType) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setType'",NULL);
#endif
  {
   self->setType(type);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getType of class  peak::Material */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Material_getType00
static int tolua_peak_peak_Material_getType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Material",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Material* self = (peak::Material*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getType'",NULL);
#endif
  {
   peak::MaterialType tolua_ret = (peak::MaterialType)  self->getType();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setTextureCount of class  peak::Material */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Material_setTextureCount00
static int tolua_peak_peak_Material_setTextureCount00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Material",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Material* self = (peak::Material*)  tolua_tousertype(tolua_S,1,0);
  int count = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setTextureCount'",NULL);
#endif
  {
   self->setTextureCount(count);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setTextureCount'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTextureCount of class  peak::Material */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Material_getTextureCount00
static int tolua_peak_peak_Material_getTextureCount00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Material",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Material* self = (peak::Material*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTextureCount'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getTextureCount();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTextureCount'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setTexture of class  peak::Material */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Material_setTexture00
static int tolua_peak_peak_Material_setTexture00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Material",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Material* self = (peak::Material*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  std::string texture = ((std::string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setTexture'",NULL);
#endif
  {
   self->setTexture(index,texture);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setTexture'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTexture of class  peak::Material */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Material_getTexture00
static int tolua_peak_peak_Material_getTexture00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Material",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Material* self = (peak::Material*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTexture'",NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getTexture(index);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTexture'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setTextureScale of class  peak::Material */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Material_setTextureScale00
static int tolua_peak_peak_Material_setTextureScale00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Material",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::Vector2D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Material* self = (peak::Material*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  peak::Vector2D scale = *((peak::Vector2D*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setTextureScale'",NULL);
#endif
  {
   self->setTextureScale(index,scale);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setTextureScale'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTextureScale of class  peak::Material */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Material_getTextureScale00
static int tolua_peak_peak_Material_getTextureScale00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Material",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Material* self = (peak::Material*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTextureScale'",NULL);
#endif
  {
   peak::Vector2D tolua_ret = (peak::Vector2D)  self->getTextureScale(index);
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Vector2D(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector2D");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector2D));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector2D");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTextureScale'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setWireframe of class  peak::Material */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Material_setWireframe00
static int tolua_peak_peak_Material_setWireframe00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Material",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Material* self = (peak::Material*)  tolua_tousertype(tolua_S,1,0);
  bool wireframe = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setWireframe'",NULL);
#endif
  {
   self->setWireframe(wireframe);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setWireframe'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isWireframe of class  peak::Material */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Material_isWireframe00
static int tolua_peak_peak_Material_isWireframe00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Material",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Material* self = (peak::Material*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isWireframe'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isWireframe();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isWireframe'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setLighting of class  peak::Material */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Material_setLighting00
static int tolua_peak_peak_Material_setLighting00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Material",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Material* self = (peak::Material*)  tolua_tousertype(tolua_S,1,0);
  bool lighting = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setLighting'",NULL);
#endif
  {
   self->setLighting(lighting);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setLighting'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getLighting of class  peak::Material */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Material_getLighting00
static int tolua_peak_peak_Material_getLighting00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Material",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Material* self = (peak::Material*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLighting'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getLighting();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLighting'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setBackfaceCulling of class  peak::Material */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Material_setBackfaceCulling00
static int tolua_peak_peak_Material_setBackfaceCulling00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Material",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Material* self = (peak::Material*)  tolua_tousertype(tolua_S,1,0);
  bool culling = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setBackfaceCulling'",NULL);
#endif
  {
   self->setBackfaceCulling(culling);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setBackfaceCulling'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getBackfaceCulling of class  peak::Material */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Material_getBackfaceCulling00
static int tolua_peak_peak_Material_getBackfaceCulling00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Material",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Material* self = (peak::Material*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getBackfaceCulling'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getBackfaceCulling();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getBackfaceCulling'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Overlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Overlay_new00
static int tolua_peak_peak_Overlay_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Overlay",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Overlay",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Overlay* parent = ((peak::Overlay*)  tolua_tousertype(tolua_S,2,0));
  {
   peak::Overlay* tolua_ret = (peak::Overlay*)  new peak::Overlay(parent);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Overlay");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Overlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Overlay_new00_local
static int tolua_peak_peak_Overlay_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Overlay",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Overlay",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Overlay* parent = ((peak::Overlay*)  tolua_tousertype(tolua_S,2,0));
  {
   peak::Overlay* tolua_ret = (peak::Overlay*)  new peak::Overlay(parent);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Overlay");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::Overlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Overlay_delete00
static int tolua_peak_peak_Overlay_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Overlay",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Overlay* self = (peak::Overlay*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setParent of class  peak::Overlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Overlay_setParent00
static int tolua_peak_peak_Overlay_setParent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Overlay",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Overlay",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Overlay* self = (peak::Overlay*)  tolua_tousertype(tolua_S,1,0);
  peak::Overlay* overlay = ((peak::Overlay*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setParent'",NULL);
#endif
  {
   self->setParent(overlay);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setParent'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getParent of class  peak::Overlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Overlay_getParent00
static int tolua_peak_peak_Overlay_getParent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Overlay",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Overlay* self = (peak::Overlay*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getParent'",NULL);
#endif
  {
   peak::Overlay* tolua_ret = (peak::Overlay*)  self->getParent();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Overlay");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getParent'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setPosition of class  peak::Overlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Overlay_setPosition00
static int tolua_peak_peak_Overlay_setPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Overlay",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Vector2D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Overlay* self = (peak::Overlay*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector2D position = *((peak::Vector2D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPosition'",NULL);
#endif
  {
   self->setPosition(position);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPosition of class  peak::Overlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Overlay_getPosition00
static int tolua_peak_peak_Overlay_getPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Overlay",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Overlay* self = (peak::Overlay*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPosition'",NULL);
#endif
  {
   peak::Vector2D tolua_ret = (peak::Vector2D)  self->getPosition();
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Vector2D(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector2D");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector2D));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector2D");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setSize of class  peak::Overlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Overlay_setSize00
static int tolua_peak_peak_Overlay_setSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Overlay",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Vector2D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Overlay* self = (peak::Overlay*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector2D size = *((peak::Vector2D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setSize'",NULL);
#endif
  {
   self->setSize(size);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSize of class  peak::Overlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Overlay_getSize00
static int tolua_peak_peak_Overlay_getSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Overlay",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Overlay* self = (peak::Overlay*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSize'",NULL);
#endif
  {
   peak::Vector2D tolua_ret = (peak::Vector2D)  self->getSize();
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Vector2D(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector2D");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector2D));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector2D");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setRotation of class  peak::Overlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Overlay_setRotation00
static int tolua_peak_peak_Overlay_setRotation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Overlay",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Overlay* self = (peak::Overlay*)  tolua_tousertype(tolua_S,1,0);
  float rotation = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRotation'",NULL);
#endif
  {
   self->setRotation(rotation);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setRotation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRotation of class  peak::Overlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Overlay_getRotation00
static int tolua_peak_peak_Overlay_getRotation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Overlay",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Overlay* self = (peak::Overlay*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRotation'",NULL);
#endif
  {
   float tolua_ret = (float)  self->getRotation();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRotation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setClipChildren of class  peak::Overlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Overlay_setClipChildren00
static int tolua_peak_peak_Overlay_setClipChildren00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Overlay",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Overlay* self = (peak::Overlay*)  tolua_tousertype(tolua_S,1,0);
  bool clipchildren = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setClipChildren'",NULL);
#endif
  {
   self->setClipChildren(clipchildren);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setClipChildren'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getClipChildren of class  peak::Overlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Overlay_getClipChildren00
static int tolua_peak_peak_Overlay_getClipChildren00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Overlay",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Overlay* self = (peak::Overlay*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getClipChildren'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getClipChildren();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getClipChildren'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAbsolutePosition of class  peak::Overlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Overlay_setAbsolutePosition00
static int tolua_peak_peak_Overlay_setAbsolutePosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Overlay",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Vector2D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Overlay* self = (peak::Overlay*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector2D position = *((peak::Vector2D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAbsolutePosition'",NULL);
#endif
  {
   self->setAbsolutePosition(position);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAbsolutePosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAbsolutePosition of class  peak::Overlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Overlay_getAbsolutePosition00
static int tolua_peak_peak_Overlay_getAbsolutePosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Overlay",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Overlay* self = (peak::Overlay*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAbsolutePosition'",NULL);
#endif
  {
   peak::Vector2D tolua_ret = (peak::Vector2D)  self->getAbsolutePosition();
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Vector2D(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector2D");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector2D));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector2D");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAbsolutePosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAbsoluteSize of class  peak::Overlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Overlay_setAbsoluteSize00
static int tolua_peak_peak_Overlay_setAbsoluteSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Overlay",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Vector2D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Overlay* self = (peak::Overlay*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector2D size = *((peak::Vector2D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAbsoluteSize'",NULL);
#endif
  {
   self->setAbsoluteSize(size);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAbsoluteSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAbsoluteSize of class  peak::Overlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Overlay_getAbsoluteSize00
static int tolua_peak_peak_Overlay_getAbsoluteSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Overlay",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Overlay* self = (peak::Overlay*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAbsoluteSize'",NULL);
#endif
  {
   peak::Vector2D tolua_ret = (peak::Vector2D)  self->getAbsoluteSize();
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Vector2D(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector2D");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector2D));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector2D");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAbsoluteSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAbsoluteRotation of class  peak::Overlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Overlay_setAbsoluteRotation00
static int tolua_peak_peak_Overlay_setAbsoluteRotation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Overlay",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Overlay* self = (peak::Overlay*)  tolua_tousertype(tolua_S,1,0);
  float rotation = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAbsoluteRotation'",NULL);
#endif
  {
   self->setAbsoluteRotation(rotation);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAbsoluteRotation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAbsoluteRotation of class  peak::Overlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Overlay_getAbsoluteRotation00
static int tolua_peak_peak_Overlay_getAbsoluteRotation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Overlay",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Overlay* self = (peak::Overlay*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAbsoluteRotation'",NULL);
#endif
  {
   float tolua_ret = (float)  self->getAbsoluteRotation();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAbsoluteRotation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: notifySizeChanged of class  peak::Overlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Overlay_notifySizeChanged00
static int tolua_peak_peak_Overlay_notifySizeChanged00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Overlay",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Overlay* self = (peak::Overlay*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'notifySizeChanged'",NULL);
#endif
  {
   self->notifySizeChanged();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'notifySizeChanged'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: render of class  peak::Overlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Overlay_render00
static int tolua_peak_peak_Overlay_render00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Overlay",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Overlay* self = (peak::Overlay*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'render'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->render();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'render'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::RectangleOverlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_RectangleOverlay_new00
static int tolua_peak_peak_RectangleOverlay_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::RectangleOverlay",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::RectangleOverlay* tolua_ret = (peak::RectangleOverlay*)  new peak::RectangleOverlay();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::RectangleOverlay");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::RectangleOverlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_RectangleOverlay_new00_local
static int tolua_peak_peak_RectangleOverlay_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::RectangleOverlay",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::RectangleOverlay* tolua_ret = (peak::RectangleOverlay*)  new peak::RectangleOverlay();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::RectangleOverlay");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::RectangleOverlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_RectangleOverlay_new01
static int tolua_peak_peak_RectangleOverlay_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::RectangleOverlay",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Color",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::Overlay",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::Color color = *((peak::Color*)  tolua_tousertype(tolua_S,2,0));
  peak::Overlay* parent = ((peak::Overlay*)  tolua_tousertype(tolua_S,3,0));
  {
   peak::RectangleOverlay* tolua_ret = (peak::RectangleOverlay*)  new peak::RectangleOverlay(color,parent);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::RectangleOverlay");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_RectangleOverlay_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::RectangleOverlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_RectangleOverlay_new01_local
static int tolua_peak_peak_RectangleOverlay_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::RectangleOverlay",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Color",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::Overlay",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::Color color = *((peak::Color*)  tolua_tousertype(tolua_S,2,0));
  peak::Overlay* parent = ((peak::Overlay*)  tolua_tousertype(tolua_S,3,0));
  {
   peak::RectangleOverlay* tolua_ret = (peak::RectangleOverlay*)  new peak::RectangleOverlay(color,parent);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::RectangleOverlay");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_RectangleOverlay_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::RectangleOverlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_RectangleOverlay_new02
static int tolua_peak_peak_RectangleOverlay_new02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::RectangleOverlay",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Color",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::Color",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"peak::Color",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"peak::Color",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,6,"peak::Overlay",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::Color color1 = *((peak::Color*)  tolua_tousertype(tolua_S,2,0));
  peak::Color color2 = *((peak::Color*)  tolua_tousertype(tolua_S,3,0));
  peak::Color color3 = *((peak::Color*)  tolua_tousertype(tolua_S,4,0));
  peak::Color color4 = *((peak::Color*)  tolua_tousertype(tolua_S,5,0));
  peak::Overlay* parent = ((peak::Overlay*)  tolua_tousertype(tolua_S,6,0));
  {
   peak::RectangleOverlay* tolua_ret = (peak::RectangleOverlay*)  new peak::RectangleOverlay(color1,color2,color3,color4,parent);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::RectangleOverlay");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_RectangleOverlay_new01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::RectangleOverlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_RectangleOverlay_new02_local
static int tolua_peak_peak_RectangleOverlay_new02_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::RectangleOverlay",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Color",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::Color",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"peak::Color",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"peak::Color",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,6,"peak::Overlay",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::Color color1 = *((peak::Color*)  tolua_tousertype(tolua_S,2,0));
  peak::Color color2 = *((peak::Color*)  tolua_tousertype(tolua_S,3,0));
  peak::Color color3 = *((peak::Color*)  tolua_tousertype(tolua_S,4,0));
  peak::Color color4 = *((peak::Color*)  tolua_tousertype(tolua_S,5,0));
  peak::Overlay* parent = ((peak::Overlay*)  tolua_tousertype(tolua_S,6,0));
  {
   peak::RectangleOverlay* tolua_ret = (peak::RectangleOverlay*)  new peak::RectangleOverlay(color1,color2,color3,color4,parent);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::RectangleOverlay");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_RectangleOverlay_new01_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::RectangleOverlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_RectangleOverlay_delete00
static int tolua_peak_peak_RectangleOverlay_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::RectangleOverlay",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::RectangleOverlay* self = (peak::RectangleOverlay*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: render of class  peak::RectangleOverlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_RectangleOverlay_render00
static int tolua_peak_peak_RectangleOverlay_render00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::RectangleOverlay",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::RectangleOverlay* self = (peak::RectangleOverlay*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'render'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->render();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'render'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::TextureOverlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_TextureOverlay_new00
static int tolua_peak_peak_TextureOverlay_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::TextureOverlay",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::Overlay",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string filename = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  peak::Overlay* parent = ((peak::Overlay*)  tolua_tousertype(tolua_S,3,0));
  {
   peak::TextureOverlay* tolua_ret = (peak::TextureOverlay*)  new peak::TextureOverlay(filename,parent);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::TextureOverlay");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::TextureOverlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_TextureOverlay_new00_local
static int tolua_peak_peak_TextureOverlay_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::TextureOverlay",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::Overlay",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string filename = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  peak::Overlay* parent = ((peak::Overlay*)  tolua_tousertype(tolua_S,3,0));
  {
   peak::TextureOverlay* tolua_ret = (peak::TextureOverlay*)  new peak::TextureOverlay(filename,parent);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::TextureOverlay");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::TextureOverlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_TextureOverlay_delete00
static int tolua_peak_peak_TextureOverlay_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::TextureOverlay",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::TextureOverlay* self = (peak::TextureOverlay*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: notifySizeChanged of class  peak::TextureOverlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_TextureOverlay_notifySizeChanged00
static int tolua_peak_peak_TextureOverlay_notifySizeChanged00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::TextureOverlay",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::TextureOverlay* self = (peak::TextureOverlay*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'notifySizeChanged'",NULL);
#endif
  {
   self->notifySizeChanged();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'notifySizeChanged'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: render of class  peak::TextureOverlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_TextureOverlay_render00
static int tolua_peak_peak_TextureOverlay_render00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::TextureOverlay",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::TextureOverlay* self = (peak::TextureOverlay*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'render'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->render();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'render'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::LabelOverlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_LabelOverlay_new00
static int tolua_peak_peak_LabelOverlay_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::LabelOverlay",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string label = ((std::string)  tolua_tocppstring(tolua_S,2,""));
  {
   peak::LabelOverlay* tolua_ret = (peak::LabelOverlay*)  new peak::LabelOverlay(label);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::LabelOverlay");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::LabelOverlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_LabelOverlay_new00_local
static int tolua_peak_peak_LabelOverlay_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::LabelOverlay",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string label = ((std::string)  tolua_tocppstring(tolua_S,2,""));
  {
   peak::LabelOverlay* tolua_ret = (peak::LabelOverlay*)  new peak::LabelOverlay(label);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::LabelOverlay");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::LabelOverlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_LabelOverlay_new01
static int tolua_peak_peak_LabelOverlay_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::LabelOverlay",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::Color",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,1,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"peak::Overlay",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  std::string label = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  peak::Color color = *((peak::Color*)  tolua_tousertype(tolua_S,3,0));
  std::string font = ((std::string)  tolua_tocppstring(tolua_S,4,""));
  peak::Overlay* parent = ((peak::Overlay*)  tolua_tousertype(tolua_S,5,0));
  {
   peak::LabelOverlay* tolua_ret = (peak::LabelOverlay*)  new peak::LabelOverlay(label,color,font,parent);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::LabelOverlay");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_LabelOverlay_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::LabelOverlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_LabelOverlay_new01_local
static int tolua_peak_peak_LabelOverlay_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::LabelOverlay",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::Color",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,1,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"peak::Overlay",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  std::string label = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  peak::Color color = *((peak::Color*)  tolua_tousertype(tolua_S,3,0));
  std::string font = ((std::string)  tolua_tocppstring(tolua_S,4,""));
  peak::Overlay* parent = ((peak::Overlay*)  tolua_tousertype(tolua_S,5,0));
  {
   peak::LabelOverlay* tolua_ret = (peak::LabelOverlay*)  new peak::LabelOverlay(label,color,font,parent);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::LabelOverlay");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_LabelOverlay_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::LabelOverlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_LabelOverlay_delete00
static int tolua_peak_peak_LabelOverlay_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::LabelOverlay",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::LabelOverlay* self = (peak::LabelOverlay*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setLabel of class  peak::LabelOverlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_LabelOverlay_setLabel00
static int tolua_peak_peak_LabelOverlay_setLabel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::LabelOverlay",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::LabelOverlay* self = (peak::LabelOverlay*)  tolua_tousertype(tolua_S,1,0);
  std::string label = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setLabel'",NULL);
#endif
  {
   self->setLabel(label);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setLabel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getLabel of class  peak::LabelOverlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_LabelOverlay_getLabel00
static int tolua_peak_peak_LabelOverlay_getLabel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::LabelOverlay",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::LabelOverlay* self = (peak::LabelOverlay*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLabel'",NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getLabel();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLabel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setColor of class  peak::LabelOverlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_LabelOverlay_setColor00
static int tolua_peak_peak_LabelOverlay_setColor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::LabelOverlay",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Color",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::LabelOverlay* self = (peak::LabelOverlay*)  tolua_tousertype(tolua_S,1,0);
  peak::Color color = *((peak::Color*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setColor'",NULL);
#endif
  {
   self->setColor(color);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setColor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getColor of class  peak::LabelOverlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_LabelOverlay_getColor00
static int tolua_peak_peak_LabelOverlay_getColor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::LabelOverlay",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::LabelOverlay* self = (peak::LabelOverlay*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getColor'",NULL);
#endif
  {
   peak::Color tolua_ret = (peak::Color)  self->getColor();
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Color(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Color");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Color));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Color");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getColor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: render of class  peak::LabelOverlay */
#ifndef TOLUA_DISABLE_tolua_peak_peak_LabelOverlay_render00
static int tolua_peak_peak_LabelOverlay_render00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::LabelOverlay",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::LabelOverlay* self = (peak::LabelOverlay*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'render'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->render();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'render'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get of class  peak::OverlayManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_OverlayManager_get00
static int tolua_peak_peak_OverlayManager_get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::OverlayManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::OverlayManager* tolua_ret = (peak::OverlayManager*)  peak::OverlayManager::get();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::OverlayManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::OverlayManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_OverlayManager_delete00
static int tolua_peak_peak_OverlayManager_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::OverlayManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::OverlayManager* self = (peak::OverlayManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: init of class  peak::OverlayManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_OverlayManager_init00
static int tolua_peak_peak_OverlayManager_init00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::OverlayManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::OverlayManager* self = (peak::OverlayManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->init();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'init'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: shutdown of class  peak::OverlayManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_OverlayManager_shutdown00
static int tolua_peak_peak_OverlayManager_shutdown00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::OverlayManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::OverlayManager* self = (peak::OverlayManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'shutdown'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->shutdown();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'shutdown'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRootOverlay of class  peak::OverlayManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_OverlayManager_getRootOverlay00
static int tolua_peak_peak_OverlayManager_getRootOverlay00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::OverlayManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::OverlayManager* self = (peak::OverlayManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRootOverlay'",NULL);
#endif
  {
   peak::Overlay* tolua_ret = (peak::Overlay*)  self->getRootOverlay();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Overlay");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRootOverlay'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: render of class  peak::OverlayManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_OverlayManager_render00
static int tolua_peak_peak_OverlayManager_render00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::OverlayManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::OverlayManager* self = (peak::OverlayManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'render'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->render();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'render'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::SkyBox */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SkyBox_new00
static int tolua_peak_peak_SkyBox_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::SkyBox",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,5,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,6,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,7,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,8,"peak::SceneNode",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,9,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string top = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  std::string bottom = ((std::string)  tolua_tocppstring(tolua_S,3,0));
  std::string left = ((std::string)  tolua_tocppstring(tolua_S,4,0));
  std::string right = ((std::string)  tolua_tocppstring(tolua_S,5,0));
  std::string front = ((std::string)  tolua_tocppstring(tolua_S,6,0));
  std::string back = ((std::string)  tolua_tocppstring(tolua_S,7,0));
  peak::SceneNode* parent = ((peak::SceneNode*)  tolua_tousertype(tolua_S,8,0));
  {
   peak::SkyBox* tolua_ret = (peak::SkyBox*)  new peak::SkyBox(top,bottom,left,right,front,back,parent);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::SkyBox");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::SkyBox */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SkyBox_new00_local
static int tolua_peak_peak_SkyBox_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::SkyBox",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,5,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,6,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,7,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,8,"peak::SceneNode",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,9,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string top = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  std::string bottom = ((std::string)  tolua_tocppstring(tolua_S,3,0));
  std::string left = ((std::string)  tolua_tocppstring(tolua_S,4,0));
  std::string right = ((std::string)  tolua_tocppstring(tolua_S,5,0));
  std::string front = ((std::string)  tolua_tocppstring(tolua_S,6,0));
  std::string back = ((std::string)  tolua_tocppstring(tolua_S,7,0));
  peak::SceneNode* parent = ((peak::SceneNode*)  tolua_tousertype(tolua_S,8,0));
  {
   peak::SkyBox* tolua_ret = (peak::SkyBox*)  new peak::SkyBox(top,bottom,left,right,front,back,parent);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::SkyBox");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::SkyBox */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SkyBox_delete00
static int tolua_peak_peak_SkyBox_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SkyBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SkyBox* self = (peak::SkyBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::SkyDome */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SkyDome_new00
static int tolua_peak_peak_SkyDome_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::SkyDome",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"peak::SceneNode",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string texture = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  float height = ((float)  tolua_tonumber(tolua_S,3,0));
  peak::SceneNode* parent = ((peak::SceneNode*)  tolua_tousertype(tolua_S,4,0));
  {
   peak::SkyDome* tolua_ret = (peak::SkyDome*)  new peak::SkyDome(texture,height,parent);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::SkyDome");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::SkyDome */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SkyDome_new00_local
static int tolua_peak_peak_SkyDome_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::SkyDome",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"peak::SceneNode",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string texture = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  float height = ((float)  tolua_tonumber(tolua_S,3,0));
  peak::SceneNode* parent = ((peak::SceneNode*)  tolua_tousertype(tolua_S,4,0));
  {
   peak::SkyDome* tolua_ret = (peak::SkyDome*)  new peak::SkyDome(texture,height,parent);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::SkyDome");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::SkyDome */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SkyDome_delete00
static int tolua_peak_peak_SkyDome_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SkyDome",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SkyDome* self = (peak::SkyDome*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::SceneNodeController */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SceneNodeController_delete00
static int tolua_peak_peak_SceneNodeController_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SceneNodeController",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SceneNodeController* self = (peak::SceneNodeController*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setSceneNode of class  peak::SceneNodeController */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SceneNodeController_setSceneNode00
static int tolua_peak_peak_SceneNodeController_setSceneNode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SceneNodeController",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::SceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SceneNodeController* self = (peak::SceneNodeController*)  tolua_tousertype(tolua_S,1,0);
  peak::SceneNode* node = ((peak::SceneNode*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setSceneNode'",NULL);
#endif
  {
   self->setSceneNode(node);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setSceneNode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSceneNode of class  peak::SceneNodeController */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SceneNodeController_getSceneNode00
static int tolua_peak_peak_SceneNodeController_getSceneNode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SceneNodeController",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SceneNodeController* self = (peak::SceneNodeController*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSceneNode'",NULL);
#endif
  {
   peak::SceneNode* tolua_ret = (peak::SceneNode*)  self->getSceneNode();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::SceneNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSceneNode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: doAllWork of class  peak::SceneNodeController */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SceneNodeController_doAllWork00
static int tolua_peak_peak_SceneNodeController_doAllWork00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::SceneNodeController",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float msecs = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   peak::SceneNodeController::doAllWork(msecs);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'doAllWork'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: doWork of class  peak::SceneNodeController */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SceneNodeController_doWork00
static int tolua_peak_peak_SceneNodeController_doWork00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SceneNodeController",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SceneNodeController* self = (peak::SceneNodeController*)  tolua_tousertype(tolua_S,1,0);
  float msecs = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'doWork'",NULL);
#endif
  {
   self->doWork(msecs);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'doWork'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::FollowingController */
#ifndef TOLUA_DISABLE_tolua_peak_peak_FollowingController_new00
static int tolua_peak_peak_FollowingController_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::FollowingController",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::FollowingController* tolua_ret = (peak::FollowingController*)  new peak::FollowingController();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::FollowingController");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::FollowingController */
#ifndef TOLUA_DISABLE_tolua_peak_peak_FollowingController_new00_local
static int tolua_peak_peak_FollowingController_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::FollowingController",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::FollowingController* tolua_ret = (peak::FollowingController*)  new peak::FollowingController();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::FollowingController");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::FollowingController */
#ifndef TOLUA_DISABLE_tolua_peak_peak_FollowingController_delete00
static int tolua_peak_peak_FollowingController_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::FollowingController",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::FollowingController* self = (peak::FollowingController*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setTarget of class  peak::FollowingController */
#ifndef TOLUA_DISABLE_tolua_peak_peak_FollowingController_setTarget00
static int tolua_peak_peak_FollowingController_setTarget00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::FollowingController",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::SceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::FollowingController* self = (peak::FollowingController*)  tolua_tousertype(tolua_S,1,0);
  peak::SceneNode* target = ((peak::SceneNode*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setTarget'",NULL);
#endif
  {
   self->setTarget(target);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setTarget'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTarget of class  peak::FollowingController */
#ifndef TOLUA_DISABLE_tolua_peak_peak_FollowingController_getTarget00
static int tolua_peak_peak_FollowingController_getTarget00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::FollowingController",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::FollowingController* self = (peak::FollowingController*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTarget'",NULL);
#endif
  {
   peak::SceneNode* tolua_ret = (peak::SceneNode*)  self->getTarget();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::SceneNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTarget'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setDistance of class  peak::FollowingController */
#ifndef TOLUA_DISABLE_tolua_peak_peak_FollowingController_setDistance00
static int tolua_peak_peak_FollowingController_setDistance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::FollowingController",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::FollowingController* self = (peak::FollowingController*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D distance = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setDistance'",NULL);
#endif
  {
   self->setDistance(distance);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setDistance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getDistance of class  peak::FollowingController */
#ifndef TOLUA_DISABLE_tolua_peak_peak_FollowingController_getDistance00
static int tolua_peak_peak_FollowingController_getDistance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::FollowingController",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::FollowingController* self = (peak::FollowingController*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDistance'",NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getDistance();
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Vector3D(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getDistance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setMode of class  peak::FollowingController */
#ifndef TOLUA_DISABLE_tolua_peak_peak_FollowingController_setMode00
static int tolua_peak_peak_FollowingController_setMode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::FollowingController",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::FollowingController* self = (peak::FollowingController*)  tolua_tousertype(tolua_S,1,0);
  peak::FollowingControllerMode mode = ((peak::FollowingControllerMode) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMode'",NULL);
#endif
  {
   self->setMode(mode);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMode of class  peak::FollowingController */
#ifndef TOLUA_DISABLE_tolua_peak_peak_FollowingController_getMode00
static int tolua_peak_peak_FollowingController_getMode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::FollowingController",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::FollowingController* self = (peak::FollowingController*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMode'",NULL);
#endif
  {
   peak::FollowingControllerMode tolua_ret = (peak::FollowingControllerMode)  self->getMode();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: doWork of class  peak::FollowingController */
#ifndef TOLUA_DISABLE_tolua_peak_peak_FollowingController_doWork00
static int tolua_peak_peak_FollowingController_doWork00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::FollowingController",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::FollowingController* self = (peak::FollowingController*)  tolua_tousertype(tolua_S,1,0);
  float msecs = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'doWork'",NULL);
#endif
  {
   self->doWork(msecs);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'doWork'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::RotationController */
#ifndef TOLUA_DISABLE_tolua_peak_peak_RotationController_new00
static int tolua_peak_peak_RotationController_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::RotationController",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::RotationController* tolua_ret = (peak::RotationController*)  new peak::RotationController();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::RotationController");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::RotationController */
#ifndef TOLUA_DISABLE_tolua_peak_peak_RotationController_new00_local
static int tolua_peak_peak_RotationController_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::RotationController",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::RotationController* tolua_ret = (peak::RotationController*)  new peak::RotationController();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::RotationController");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::RotationController */
#ifndef TOLUA_DISABLE_tolua_peak_peak_RotationController_delete00
static int tolua_peak_peak_RotationController_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::RotationController",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::RotationController* self = (peak::RotationController*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setRotationSpeed of class  peak::RotationController */
#ifndef TOLUA_DISABLE_tolua_peak_peak_RotationController_setRotationSpeed00
static int tolua_peak_peak_RotationController_setRotationSpeed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::RotationController",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::RotationController* self = (peak::RotationController*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D speed = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRotationSpeed'",NULL);
#endif
  {
   self->setRotationSpeed(speed);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setRotationSpeed'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRotationSpeed of class  peak::RotationController */
#ifndef TOLUA_DISABLE_tolua_peak_peak_RotationController_getRotationSpeed00
static int tolua_peak_peak_RotationController_getRotationSpeed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::RotationController",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::RotationController* self = (peak::RotationController*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRotationSpeed'",NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getRotationSpeed();
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Vector3D(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRotationSpeed'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: doWork of class  peak::RotationController */
#ifndef TOLUA_DISABLE_tolua_peak_peak_RotationController_doWork00
static int tolua_peak_peak_RotationController_doWork00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::RotationController",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::RotationController* self = (peak::RotationController*)  tolua_tousertype(tolua_S,1,0);
  float msecs = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'doWork'",NULL);
#endif
  {
   self->doWork(msecs);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'doWork'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: pos of class  peak::ContactInfo */
#ifndef TOLUA_DISABLE_tolua_get_peak__ContactInfo_pos
static int tolua_get_peak__ContactInfo_pos(lua_State* tolua_S)
{
  peak::ContactInfo* self = (peak::ContactInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pos'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->pos,"peak::Vector3D");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: pos of class  peak::ContactInfo */
#ifndef TOLUA_DISABLE_tolua_set_peak__ContactInfo_pos
static int tolua_set_peak__ContactInfo_pos(lua_State* tolua_S)
{
  peak::ContactInfo* self = (peak::ContactInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pos'",NULL);
  if (!tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->pos = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: geom of class  peak::ContactInfo */
#ifndef TOLUA_DISABLE_tolua_get_peak__ContactInfo_geom_ptr
static int tolua_get_peak__ContactInfo_geom_ptr(lua_State* tolua_S)
{
  peak::ContactInfo* self = (peak::ContactInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'geom'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)self->geom,"peak::Geometry");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: geom of class  peak::ContactInfo */
#ifndef TOLUA_DISABLE_tolua_set_peak__ContactInfo_geom_ptr
static int tolua_set_peak__ContactInfo_geom_ptr(lua_State* tolua_S)
{
  peak::ContactInfo* self = (peak::ContactInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'geom'",NULL);
  if (!tolua_isusertype(tolua_S,2,"peak::Geometry",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->geom = ((peak::Geometry*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Body */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Body_new00
static int tolua_peak_peak_Body_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Body",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Body* tolua_ret = (peak::Body*)  new peak::Body();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Body");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Body */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Body_new00_local
static int tolua_peak_peak_Body_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Body",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Body* tolua_ret = (peak::Body*)  new peak::Body();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Body");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::Body */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Body_delete00
static int tolua_peak_peak_Body_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Body",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Body* self = (peak::Body*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: init of class  peak::Body */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Body_init00
static int tolua_peak_peak_Body_init00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Body",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::World",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::Entity",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Body* self = (peak::Body*)  tolua_tousertype(tolua_S,1,0);
  peak::World* world = ((peak::World*)  tolua_tousertype(tolua_S,2,0));
  peak::Entity* entity = ((peak::Entity*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->init(world,entity);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'init'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: destroy of class  peak::Body */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Body_destroy00
static int tolua_peak_peak_Body_destroy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Body",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Body* self = (peak::Body*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroy'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->destroy();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'destroy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setPosition of class  peak::Body */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Body_setPosition00
static int tolua_peak_peak_Body_setPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Body",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Body* self = (peak::Body*)  tolua_tousertype(tolua_S,1,0);
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
  float z = ((float)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPosition'",NULL);
#endif
  {
   self->setPosition(x,y,z);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setPosition of class  peak::Body */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Body_setPosition01
static int tolua_peak_peak_Body_setPosition01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Body",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::Body* self = (peak::Body*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D pos = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPosition'",NULL);
#endif
  {
   self->setPosition(pos);
  }
 }
 return 0;
tolua_lerror:
 return tolua_peak_peak_Body_setPosition00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPosition of class  peak::Body */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Body_getPosition00
static int tolua_peak_peak_Body_getPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Body",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Body* self = (peak::Body*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPosition'",NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getPosition();
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Vector3D(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setRotation of class  peak::Body */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Body_setRotation00
static int tolua_peak_peak_Body_setRotation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Body",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Body* self = (peak::Body*)  tolua_tousertype(tolua_S,1,0);
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
  float z = ((float)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRotation'",NULL);
#endif
  {
   self->setRotation(x,y,z);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setRotation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setRotation of class  peak::Body */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Body_setRotation01
static int tolua_peak_peak_Body_setRotation01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Body",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::Body* self = (peak::Body*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D rot = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRotation'",NULL);
#endif
  {
   self->setRotation(rot);
  }
 }
 return 0;
tolua_lerror:
 return tolua_peak_peak_Body_setRotation00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: setRotation of class  peak::Body */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Body_setRotation02
static int tolua_peak_peak_Body_setRotation02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Body",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::Body* self = (peak::Body*)  tolua_tousertype(tolua_S,1,0);
  peak::Quaternion rot = *((peak::Quaternion*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRotation'",NULL);
#endif
  {
   self->setRotation(rot);
  }
 }
 return 0;
tolua_lerror:
 return tolua_peak_peak_Body_setRotation01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRotation of class  peak::Body */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Body_getRotation00
static int tolua_peak_peak_Body_getRotation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Body",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Body* self = (peak::Body*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRotation'",NULL);
#endif
  {
   peak::Quaternion tolua_ret = (peak::Quaternion)  self->getRotation();
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Quaternion(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Quaternion");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Quaternion));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Quaternion");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRotation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setLinearVelocity of class  peak::Body */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Body_setLinearVelocity00
static int tolua_peak_peak_Body_setLinearVelocity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Body",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Body* self = (peak::Body*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D vel = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setLinearVelocity'",NULL);
#endif
  {
   self->setLinearVelocity(vel);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setLinearVelocity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getLinearVelocity of class  peak::Body */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Body_getLinearVelocity00
static int tolua_peak_peak_Body_getLinearVelocity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Body",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Body* self = (peak::Body*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLinearVelocity'",NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getLinearVelocity();
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Vector3D(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLinearVelocity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAngularVelocity of class  peak::Body */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Body_setAngularVelocity00
static int tolua_peak_peak_Body_setAngularVelocity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Body",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Body* self = (peak::Body*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D vel = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAngularVelocity'",NULL);
#endif
  {
   self->setAngularVelocity(vel);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAngularVelocity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAngularVelocity of class  peak::Body */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Body_getAngularVelocity00
static int tolua_peak_peak_Body_getAngularVelocity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Body",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Body* self = (peak::Body*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAngularVelocity'",NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getAngularVelocity();
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Vector3D(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAngularVelocity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addForce of class  peak::Body */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Body_addForce00
static int tolua_peak_peak_Body_addForce00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Body",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Body* self = (peak::Body*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D force = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addForce'",NULL);
#endif
  {
   self->addForce(force);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addForce'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setForce of class  peak::Body */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Body_setForce00
static int tolua_peak_peak_Body_setForce00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Body",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Body* self = (peak::Body*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D force = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setForce'",NULL);
#endif
  {
   self->setForce(force);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setForce'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getForce of class  peak::Body */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Body_getForce00
static int tolua_peak_peak_Body_getForce00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Body",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Body* self = (peak::Body*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getForce'",NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getForce();
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Vector3D(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getForce'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setRelativeForce of class  peak::Body */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Body_setRelativeForce00
static int tolua_peak_peak_Body_setRelativeForce00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Body",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Body* self = (peak::Body*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D force = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRelativeForce'",NULL);
#endif
  {
   self->setRelativeForce(force);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setRelativeForce'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRelativeForce of class  peak::Body */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Body_getRelativeForce00
static int tolua_peak_peak_Body_getRelativeForce00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Body",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Body* self = (peak::Body*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRelativeForce'",NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getRelativeForce();
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Vector3D(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRelativeForce'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setEntity of class  peak::Body */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Body_setEntity00
static int tolua_peak_peak_Body_setEntity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Body",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Body* self = (peak::Body*)  tolua_tousertype(tolua_S,1,0);
  peak::Entity* entity = ((peak::Entity*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setEntity'",NULL);
#endif
  {
   self->setEntity(entity);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setEntity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getEntity of class  peak::Body */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Body_getEntity00
static int tolua_peak_peak_Body_getEntity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Body",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Body* self = (peak::Body*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getEntity'",NULL);
#endif
  {
   peak::Entity* tolua_ret = (peak::Entity*)  self->getEntity();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Entity");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getEntity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setGravity of class  peak::Body */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Body_setGravity00
static int tolua_peak_peak_Body_setGravity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Body",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Body* self = (peak::Body*)  tolua_tousertype(tolua_S,1,0);
  bool enabled = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setGravity'",NULL);
#endif
  {
   self->setGravity(enabled);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setGravity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getGravity of class  peak::Body */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Body_getGravity00
static int tolua_peak_peak_Body_getGravity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Body",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Body* self = (peak::Body*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getGravity'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getGravity();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getGravity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getBody of class  peak::Body */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Body_getBody00
static int tolua_peak_peak_Body_getBody00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Body",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Body* self = (peak::Body*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getBody'",NULL);
#endif
  {
   dBodyID tolua_ret = (dBodyID)  self->getBody();
   {
#ifdef __cplusplus
    void* tolua_obj = new dBodyID(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"dBodyID");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(dBodyID));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"dBodyID");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getBody'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Plane */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Plane_new00
static int tolua_peak_peak_Plane_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Plane",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Plane* tolua_ret = (peak::Plane*)  new peak::Plane();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Plane");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Plane */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Plane_new00_local
static int tolua_peak_peak_Plane_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Plane",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Plane* tolua_ret = (peak::Plane*)  new peak::Plane();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Plane");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::Plane */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Plane_delete00
static int tolua_peak_peak_Plane_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Plane",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Plane* self = (peak::Plane*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: init of class  peak::Plane */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Plane_init00
static int tolua_peak_peak_Plane_init00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Plane",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::World",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Plane* self = (peak::Plane*)  tolua_tousertype(tolua_S,1,0);
  peak::World* world = ((peak::World*)  tolua_tousertype(tolua_S,2,0));
  peak::Vector3D normal = *((peak::Vector3D*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->init(world,normal);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'init'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: destroy of class  peak::Plane */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Plane_destroy00
static int tolua_peak_peak_Plane_destroy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Plane",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Plane* self = (peak::Plane*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroy'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->destroy();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'destroy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Cube */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Cube_new00
static int tolua_peak_peak_Cube_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Cube",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Cube* tolua_ret = (peak::Cube*)  new peak::Cube();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Cube");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Cube */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Cube_new00_local
static int tolua_peak_peak_Cube_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Cube",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Cube* tolua_ret = (peak::Cube*)  new peak::Cube();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Cube");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::Cube */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Cube_delete00
static int tolua_peak_peak_Cube_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Cube",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Cube* self = (peak::Cube*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: init of class  peak::Cube */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Cube_init00
static int tolua_peak_peak_Cube_init00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Cube",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::World",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Cube* self = (peak::Cube*)  tolua_tousertype(tolua_S,1,0);
  peak::World* world = ((peak::World*)  tolua_tousertype(tolua_S,2,0));
  float sx = ((float)  tolua_tonumber(tolua_S,3,0));
  float sy = ((float)  tolua_tonumber(tolua_S,4,0));
  float sz = ((float)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->init(world,sx,sy,sz);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'init'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: destroy of class  peak::Cube */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Cube_destroy00
static int tolua_peak_peak_Cube_destroy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Cube",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Cube* self = (peak::Cube*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroy'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->destroy();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'destroy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Cylinder */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Cylinder_new00
static int tolua_peak_peak_Cylinder_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Cylinder",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Cylinder* tolua_ret = (peak::Cylinder*)  new peak::Cylinder();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Cylinder");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Cylinder */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Cylinder_new00_local
static int tolua_peak_peak_Cylinder_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Cylinder",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Cylinder* tolua_ret = (peak::Cylinder*)  new peak::Cylinder();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Cylinder");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::Cylinder */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Cylinder_delete00
static int tolua_peak_peak_Cylinder_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Cylinder",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Cylinder* self = (peak::Cylinder*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: init of class  peak::Cylinder */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Cylinder_init00
static int tolua_peak_peak_Cylinder_init00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Cylinder",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::World",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Cylinder* self = (peak::Cylinder*)  tolua_tousertype(tolua_S,1,0);
  peak::World* world = ((peak::World*)  tolua_tousertype(tolua_S,2,0));
  float radius = ((float)  tolua_tonumber(tolua_S,3,0));
  float length = ((float)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->init(world,radius,length);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'init'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: init of class  peak::Cylinder */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Cylinder_init01
static int tolua_peak_peak_Cylinder_init01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Cylinder",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::World",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"peak::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::Cylinder* self = (peak::Cylinder*)  tolua_tousertype(tolua_S,1,0);
  peak::World* world = ((peak::World*)  tolua_tousertype(tolua_S,2,0));
  float radius = ((float)  tolua_tonumber(tolua_S,3,0));
  float length = ((float)  tolua_tonumber(tolua_S,4,0));
  peak::Quaternion rot = *((peak::Quaternion*)  tolua_tousertype(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->init(world,radius,length,rot);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Cylinder_init00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: destroy of class  peak::Cylinder */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Cylinder_destroy00
static int tolua_peak_peak_Cylinder_destroy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Cylinder",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Cylinder* self = (peak::Cylinder*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroy'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->destroy();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'destroy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: script of class  peak::CollisionCallback */
#ifndef TOLUA_DISABLE_tolua_get_peak__CollisionCallback_script_ptr
static int tolua_get_peak__CollisionCallback_script_ptr(lua_State* tolua_S)
{
  peak::CollisionCallback* self = (peak::CollisionCallback*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'script'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)self->script,"peak::Script");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: script of class  peak::CollisionCallback */
#ifndef TOLUA_DISABLE_tolua_set_peak__CollisionCallback_script_ptr
static int tolua_set_peak__CollisionCallback_script_ptr(lua_State* tolua_S)
{
  peak::CollisionCallback* self = (peak::CollisionCallback*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'script'",NULL);
  if (!tolua_isusertype(tolua_S,2,"peak::Script",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->script = ((peak::Script*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: functionname of class  peak::CollisionCallback */
#ifndef TOLUA_DISABLE_tolua_get_peak__CollisionCallback_functionname
static int tolua_get_peak__CollisionCallback_functionname(lua_State* tolua_S)
{
  peak::CollisionCallback* self = (peak::CollisionCallback*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'functionname'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->functionname);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: functionname of class  peak::CollisionCallback */
#ifndef TOLUA_DISABLE_tolua_set_peak__CollisionCallback_functionname
static int tolua_set_peak__CollisionCallback_functionname(lua_State* tolua_S)
{
  peak::CollisionCallback* self = (peak::CollisionCallback*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'functionname'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->functionname = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: friction of class  peak::CollisionInfo */
#ifndef TOLUA_DISABLE_tolua_get_peak__CollisionInfo_friction
static int tolua_get_peak__CollisionInfo_friction(lua_State* tolua_S)
{
  peak::CollisionInfo* self = (peak::CollisionInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'friction'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->friction);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: friction of class  peak::CollisionInfo */
#ifndef TOLUA_DISABLE_tolua_set_peak__CollisionInfo_friction
static int tolua_set_peak__CollisionInfo_friction(lua_State* tolua_S)
{
  peak::CollisionInfo* self = (peak::CollisionInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'friction'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->friction = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: bounce of class  peak::CollisionInfo */
#ifndef TOLUA_DISABLE_tolua_get_peak__CollisionInfo_bounce
static int tolua_get_peak__CollisionInfo_bounce(lua_State* tolua_S)
{
  peak::CollisionInfo* self = (peak::CollisionInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'bounce'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->bounce);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: bounce of class  peak::CollisionInfo */
#ifndef TOLUA_DISABLE_tolua_set_peak__CollisionInfo_bounce
static int tolua_set_peak__CollisionInfo_bounce(lua_State* tolua_S)
{
  peak::CollisionInfo* self = (peak::CollisionInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'bounce'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->bounce = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: bouncevelocity of class  peak::CollisionInfo */
#ifndef TOLUA_DISABLE_tolua_get_peak__CollisionInfo_bouncevelocity
static int tolua_get_peak__CollisionInfo_bouncevelocity(lua_State* tolua_S)
{
  peak::CollisionInfo* self = (peak::CollisionInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'bouncevelocity'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->bouncevelocity);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: bouncevelocity of class  peak::CollisionInfo */
#ifndef TOLUA_DISABLE_tolua_set_peak__CollisionInfo_bouncevelocity
static int tolua_set_peak__CollisionInfo_bouncevelocity(lua_State* tolua_S)
{
  peak::CollisionInfo* self = (peak::CollisionInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'bouncevelocity'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->bouncevelocity = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: callback1 of class  peak::CollisionInfo */
#ifndef TOLUA_DISABLE_tolua_get_peak__CollisionInfo_callback1
static int tolua_get_peak__CollisionInfo_callback1(lua_State* tolua_S)
{
  peak::CollisionInfo* self = (peak::CollisionInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'callback1'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->callback1,"peak::CollisionCallback");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: callback1 of class  peak::CollisionInfo */
#ifndef TOLUA_DISABLE_tolua_set_peak__CollisionInfo_callback1
static int tolua_set_peak__CollisionInfo_callback1(lua_State* tolua_S)
{
  peak::CollisionInfo* self = (peak::CollisionInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'callback1'",NULL);
  if (!tolua_isusertype(tolua_S,2,"peak::CollisionCallback",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->callback1 = *((peak::CollisionCallback*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: callback2 of class  peak::CollisionInfo */
#ifndef TOLUA_DISABLE_tolua_get_peak__CollisionInfo_callback2
static int tolua_get_peak__CollisionInfo_callback2(lua_State* tolua_S)
{
  peak::CollisionInfo* self = (peak::CollisionInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'callback2'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->callback2,"peak::CollisionCallback");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: callback2 of class  peak::CollisionInfo */
#ifndef TOLUA_DISABLE_tolua_set_peak__CollisionInfo_callback2
static int tolua_set_peak__CollisionInfo_callback2(lua_State* tolua_S)
{
  peak::CollisionInfo* self = (peak::CollisionInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'callback2'",NULL);
  if (!tolua_isusertype(tolua_S,2,"peak::CollisionCallback",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->callback2 = *((peak::CollisionCallback*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::Joint */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Joint_delete00
static int tolua_peak_peak_Joint_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Joint",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Joint* self = (peak::Joint*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: init of class  peak::Joint */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Joint_init00
static int tolua_peak_peak_Joint_init00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Joint",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::World",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Joint* self = (peak::Joint*)  tolua_tousertype(tolua_S,1,0);
  peak::World* world = ((peak::World*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->init(world);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'init'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: destroy of class  peak::Joint */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Joint_destroy00
static int tolua_peak_peak_Joint_destroy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Joint",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Joint* self = (peak::Joint*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroy'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->destroy();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'destroy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setBody1 of class  peak::Joint */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Joint_setBody100
static int tolua_peak_peak_Joint_setBody100(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Joint",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Body",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Joint* self = (peak::Joint*)  tolua_tousertype(tolua_S,1,0);
  peak::Body* body1 = ((peak::Body*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setBody1'",NULL);
#endif
  {
   self->setBody1(body1);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setBody1'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setBody2 of class  peak::Joint */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Joint_setBody200
static int tolua_peak_peak_Joint_setBody200(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Joint",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Body",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Joint* self = (peak::Joint*)  tolua_tousertype(tolua_S,1,0);
  peak::Body* body2 = ((peak::Body*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setBody2'",NULL);
#endif
  {
   self->setBody2(body2);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setBody2'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Hinge */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Hinge_new00
static int tolua_peak_peak_Hinge_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Hinge",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Hinge* tolua_ret = (peak::Hinge*)  new peak::Hinge();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Hinge");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Hinge */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Hinge_new00_local
static int tolua_peak_peak_Hinge_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Hinge",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Hinge* tolua_ret = (peak::Hinge*)  new peak::Hinge();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Hinge");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::Hinge */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Hinge_delete00
static int tolua_peak_peak_Hinge_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Hinge",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Hinge* self = (peak::Hinge*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: init of class  peak::Hinge */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Hinge_init00
static int tolua_peak_peak_Hinge_init00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Hinge",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::World",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Hinge* self = (peak::Hinge*)  tolua_tousertype(tolua_S,1,0);
  peak::World* world = ((peak::World*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->init(world);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'init'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAnchor of class  peak::Hinge */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Hinge_setAnchor00
static int tolua_peak_peak_Hinge_setAnchor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Hinge",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Hinge* self = (peak::Hinge*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D anchor = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAnchor'",NULL);
#endif
  {
   self->setAnchor(anchor);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAnchor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAxis of class  peak::Hinge */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Hinge_setAxis00
static int tolua_peak_peak_Hinge_setAxis00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Hinge",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Hinge* self = (peak::Hinge*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D axis = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAxis'",NULL);
#endif
  {
   self->setAxis(axis);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAxis'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Ray */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Ray_new00
static int tolua_peak_peak_Ray_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Ray",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Ray* tolua_ret = (peak::Ray*)  new peak::Ray();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Ray");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Ray */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Ray_new00_local
static int tolua_peak_peak_Ray_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Ray",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Ray* tolua_ret = (peak::Ray*)  new peak::Ray();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Ray");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::Ray */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Ray_delete00
static int tolua_peak_peak_Ray_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Ray",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Ray* self = (peak::Ray*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: init of class  peak::Ray */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Ray_init00
static int tolua_peak_peak_Ray_init00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Ray",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::World",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Ray* self = (peak::Ray*)  tolua_tousertype(tolua_S,1,0);
  peak::World* world = ((peak::World*)  tolua_tousertype(tolua_S,2,0));
  peak::Vector3D direction = *((peak::Vector3D*)  tolua_tousertype(tolua_S,3,0));
  float length = ((float)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->init(world,direction,length);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'init'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: destroy of class  peak::Ray */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Ray_destroy00
static int tolua_peak_peak_Ray_destroy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Ray",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Ray* self = (peak::Ray*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroy'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->destroy();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'destroy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: position of class  peak::Contact */
#ifndef TOLUA_DISABLE_tolua_get_peak__Contact_position
static int tolua_get_peak__Contact_position(lua_State* tolua_S)
{
  peak::Contact* self = (peak::Contact*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'position'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->position,"peak::Vector3D");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: position of class  peak::Contact */
#ifndef TOLUA_DISABLE_tolua_set_peak__Contact_position
static int tolua_set_peak__Contact_position(lua_State* tolua_S)
{
  peak::Contact* self = (peak::Contact*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'position'",NULL);
  if (!tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->position = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: normal of class  peak::Contact */
#ifndef TOLUA_DISABLE_tolua_get_peak__Contact_normal
static int tolua_get_peak__Contact_normal(lua_State* tolua_S)
{
  peak::Contact* self = (peak::Contact*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'normal'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->normal,"peak::Vector3D");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: normal of class  peak::Contact */
#ifndef TOLUA_DISABLE_tolua_set_peak__Contact_normal
static int tolua_set_peak__Contact_normal(lua_State* tolua_S)
{
  peak::Contact* self = (peak::Contact*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'normal'",NULL);
  if (!tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->normal = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: depth of class  peak::Contact */
#ifndef TOLUA_DISABLE_tolua_get_peak__Contact_depth
static int tolua_get_peak__Contact_depth(lua_State* tolua_S)
{
  peak::Contact* self = (peak::Contact*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'depth'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->depth);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: depth of class  peak::Contact */
#ifndef TOLUA_DISABLE_tolua_set_peak__Contact_depth
static int tolua_set_peak__Contact_depth(lua_State* tolua_S)
{
  peak::Contact* self = (peak::Contact*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'depth'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->depth = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getContact of class  peak::Collision */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Collision_getContact00
static int tolua_peak_peak_Collision_getContact00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Collision",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Collision* self = (peak::Collision*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getContact'",NULL);
#endif
  {
   peak::Contact* tolua_ret = (peak::Contact*)  self->getContact(index);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Contact");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getContact'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getContactCount of class  peak::Collision */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Collision_getContactCount00
static int tolua_peak_peak_Collision_getContactCount00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Collision",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Collision* self = (peak::Collision*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getContactCount'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getContactCount();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getContactCount'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: contacts of class  peak::Collision */
#ifndef TOLUA_DISABLE_tolua_get_peak__Collision_contacts
static int tolua_get_peak__Collision_contacts(lua_State* tolua_S)
{
  peak::Collision* self = (peak::Collision*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'contacts'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->contacts,"std::vector<peak::Contact>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: contacts of class  peak::Collision */
#ifndef TOLUA_DISABLE_tolua_set_peak__Collision_contacts
static int tolua_set_peak__Collision_contacts(lua_State* tolua_S)
{
  peak::Collision* self = (peak::Collision*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'contacts'",NULL);
  if (!tolua_isusertype(tolua_S,2,"std::vector<peak::Contact>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->contacts = *((std::vector<peak::Contact>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: speed of class  peak::Collision */
#ifndef TOLUA_DISABLE_tolua_get_peak__Collision_speed
static int tolua_get_peak__Collision_speed(lua_State* tolua_S)
{
  peak::Collision* self = (peak::Collision*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'speed'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->speed,"peak::Vector3D");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: speed of class  peak::Collision */
#ifndef TOLUA_DISABLE_tolua_set_peak__Collision_speed
static int tolua_set_peak__Collision_speed(lua_State* tolua_S)
{
  peak::Collision* self = (peak::Collision*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'speed'",NULL);
  if (!tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->speed = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: geom1 of class  peak::Collision */
#ifndef TOLUA_DISABLE_tolua_get_peak__Collision_geom1_ptr
static int tolua_get_peak__Collision_geom1_ptr(lua_State* tolua_S)
{
  peak::Collision* self = (peak::Collision*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'geom1'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)self->geom1,"peak::Geometry");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: geom1 of class  peak::Collision */
#ifndef TOLUA_DISABLE_tolua_set_peak__Collision_geom1_ptr
static int tolua_set_peak__Collision_geom1_ptr(lua_State* tolua_S)
{
  peak::Collision* self = (peak::Collision*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'geom1'",NULL);
  if (!tolua_isusertype(tolua_S,2,"peak::Geometry",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->geom1 = ((peak::Geometry*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: geom2 of class  peak::Collision */
#ifndef TOLUA_DISABLE_tolua_get_peak__Collision_geom2_ptr
static int tolua_get_peak__Collision_geom2_ptr(lua_State* tolua_S)
{
  peak::Collision* self = (peak::Collision*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'geom2'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)self->geom2,"peak::Geometry");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: geom2 of class  peak::Collision */
#ifndef TOLUA_DISABLE_tolua_set_peak__Collision_geom2_ptr
static int tolua_set_peak__Collision_geom2_ptr(lua_State* tolua_S)
{
  peak::Collision* self = (peak::Collision*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'geom2'",NULL);
  if (!tolua_isusertype(tolua_S,2,"peak::Geometry",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->geom2 = ((peak::Geometry*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::PhysicsComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PhysicsComponent_new00
static int tolua_peak_peak_PhysicsComponent_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::PhysicsComponent",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::PhysicsComponent* tolua_ret = (peak::PhysicsComponent*)  new peak::PhysicsComponent();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::PhysicsComponent");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::PhysicsComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PhysicsComponent_new00_local
static int tolua_peak_peak_PhysicsComponent_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::PhysicsComponent",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::PhysicsComponent* tolua_ret = (peak::PhysicsComponent*)  new peak::PhysicsComponent();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::PhysicsComponent");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::PhysicsComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PhysicsComponent_delete00
static int tolua_peak_peak_PhysicsComponent_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::PhysicsComponent",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::PhysicsComponent* self = (peak::PhysicsComponent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: init of class  peak::PhysicsComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PhysicsComponent_init00
static int tolua_peak_peak_PhysicsComponent_init00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::PhysicsComponent",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::PhysicsComponent* self = (peak::PhysicsComponent*)  tolua_tousertype(tolua_S,1,0);
  peak::Entity* entity = ((peak::Entity*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->init(entity);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'init'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: destroy of class  peak::PhysicsComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PhysicsComponent_destroy00
static int tolua_peak_peak_PhysicsComponent_destroy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::PhysicsComponent",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::PhysicsComponent* self = (peak::PhysicsComponent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroy'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->destroy();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'destroy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: needsUpdate of class  peak::PhysicsComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PhysicsComponent_needsUpdate00
static int tolua_peak_peak_PhysicsComponent_needsUpdate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::PhysicsComponent",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::PhysicsComponent* self = (peak::PhysicsComponent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'needsUpdate'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->needsUpdate();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'needsUpdate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getUpdateSize of class  peak::PhysicsComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PhysicsComponent_getUpdateSize00
static int tolua_peak_peak_PhysicsComponent_getUpdateSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::PhysicsComponent",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::PhysicsComponent* self = (peak::PhysicsComponent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getUpdateSize'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getUpdateSize();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getUpdateSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: writeUpdateData of class  peak::PhysicsComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PhysicsComponent_writeUpdateData00
static int tolua_peak_peak_PhysicsComponent_writeUpdateData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::PhysicsComponent",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::PhysicsComponent* self = (peak::PhysicsComponent*)  tolua_tousertype(tolua_S,1,0);
  peak::Buffer* buffer = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeUpdateData'",NULL);
#endif
  {
   int tolua_ret = (int)  self->writeUpdateData(buffer);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'writeUpdateData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: updateFromData of class  peak::PhysicsComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PhysicsComponent_updateFromData00
static int tolua_peak_peak_PhysicsComponent_updateFromData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::PhysicsComponent",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::PhysicsComponent* self = (peak::PhysicsComponent*)  tolua_tousertype(tolua_S,1,0);
  peak::Buffer* buffer = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'updateFromData'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->updateFromData(buffer);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'updateFromData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: writeCompleteData of class  peak::PhysicsComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PhysicsComponent_writeCompleteData00
static int tolua_peak_peak_PhysicsComponent_writeCompleteData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::PhysicsComponent",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::PhysicsComponent* self = (peak::PhysicsComponent*)  tolua_tousertype(tolua_S,1,0);
  peak::Buffer* buffer = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeCompleteData'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->writeCompleteData(buffer);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'writeCompleteData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: readCompleteData of class  peak::PhysicsComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PhysicsComponent_readCompleteData00
static int tolua_peak_peak_PhysicsComponent_readCompleteData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::PhysicsComponent",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::PhysicsComponent* self = (peak::PhysicsComponent*)  tolua_tousertype(tolua_S,1,0);
  peak::Buffer* buffer = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readCompleteData'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->readCompleteData(buffer);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'readCompleteData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: move of class  peak::PhysicsComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PhysicsComponent_move00
static int tolua_peak_peak_PhysicsComponent_move00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::PhysicsComponent",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::PhysicsComponent* self = (peak::PhysicsComponent*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D pos = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'move'",NULL);
#endif
  {
   self->move(pos);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'move'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addBody of class  peak::PhysicsComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PhysicsComponent_addBody00
static int tolua_peak_peak_PhysicsComponent_addBody00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::PhysicsComponent",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Body",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::PhysicsComponent* self = (peak::PhysicsComponent*)  tolua_tousertype(tolua_S,1,0);
  peak::Body* body = ((peak::Body*)  tolua_tousertype(tolua_S,2,0));
  int id = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addBody'",NULL);
#endif
  {
   self->addBody(body,id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addBody'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getBody of class  peak::PhysicsComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PhysicsComponent_getBody00
static int tolua_peak_peak_PhysicsComponent_getBody00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::PhysicsComponent",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::PhysicsComponent* self = (peak::PhysicsComponent*)  tolua_tousertype(tolua_S,1,0);
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getBody'",NULL);
#endif
  {
   peak::Body* tolua_ret = (peak::Body*)  self->getBody(id);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Body");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getBody'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addJoint of class  peak::PhysicsComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PhysicsComponent_addJoint00
static int tolua_peak_peak_PhysicsComponent_addJoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::PhysicsComponent",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Joint",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::PhysicsComponent* self = (peak::PhysicsComponent*)  tolua_tousertype(tolua_S,1,0);
  peak::Joint* joint = ((peak::Joint*)  tolua_tousertype(tolua_S,2,0));
  int id = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addJoint'",NULL);
#endif
  {
   self->addJoint(joint,id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addJoint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getJoint of class  peak::PhysicsComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PhysicsComponent_getJoint00
static int tolua_peak_peak_PhysicsComponent_getJoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::PhysicsComponent",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::PhysicsComponent* self = (peak::PhysicsComponent*)  tolua_tousertype(tolua_S,1,0);
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getJoint'",NULL);
#endif
  {
   peak::Joint* tolua_ret = (peak::Joint*)  self->getJoint(id);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Joint");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getJoint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addGeom of class  peak::PhysicsComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PhysicsComponent_addGeom00
static int tolua_peak_peak_PhysicsComponent_addGeom00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::PhysicsComponent",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Geometry",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::PhysicsComponent* self = (peak::PhysicsComponent*)  tolua_tousertype(tolua_S,1,0);
  peak::Geometry* geom = ((peak::Geometry*)  tolua_tousertype(tolua_S,2,0));
  int id = ((int)  tolua_tonumber(tolua_S,3,0));
  int parentid = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addGeom'",NULL);
#endif
  {
   self->addGeom(geom,id,parentid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addGeom'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getGeom of class  peak::PhysicsComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PhysicsComponent_getGeom00
static int tolua_peak_peak_PhysicsComponent_getGeom00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::PhysicsComponent",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::PhysicsComponent* self = (peak::PhysicsComponent*)  tolua_tousertype(tolua_S,1,0);
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getGeom'",NULL);
#endif
  {
   peak::Geometry* tolua_ret = (peak::Geometry*)  self->getGeom(id);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Geometry");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getGeom'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getType of class  peak::PhysicsComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PhysicsComponent_getType00
static int tolua_peak_peak_PhysicsComponent_getType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::PhysicsComponent",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::PhysicsComponent* self = (peak::PhysicsComponent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getType'",NULL);
#endif
  {
   peak::ComponentType tolua_ret = (peak::ComponentType)  self->getType();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: doWork of class  peak::PhysicsComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PhysicsComponent_doWork00
static int tolua_peak_peak_PhysicsComponent_doWork00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::PhysicsComponent",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::PhysicsComponent* self = (peak::PhysicsComponent*)  tolua_tousertype(tolua_S,1,0);
  float msecs = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'doWork'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->doWork(msecs);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'doWork'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::GraphicsComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_GraphicsComponent_new00
static int tolua_peak_peak_GraphicsComponent_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::GraphicsComponent",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::GraphicsComponent* tolua_ret = (peak::GraphicsComponent*)  new peak::GraphicsComponent();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::GraphicsComponent");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::GraphicsComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_GraphicsComponent_new00_local
static int tolua_peak_peak_GraphicsComponent_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::GraphicsComponent",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::GraphicsComponent* tolua_ret = (peak::GraphicsComponent*)  new peak::GraphicsComponent();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::GraphicsComponent");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::GraphicsComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_GraphicsComponent_delete00
static int tolua_peak_peak_GraphicsComponent_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::GraphicsComponent",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::GraphicsComponent* self = (peak::GraphicsComponent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: init of class  peak::GraphicsComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_GraphicsComponent_init00
static int tolua_peak_peak_GraphicsComponent_init00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::GraphicsComponent",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::GraphicsComponent* self = (peak::GraphicsComponent*)  tolua_tousertype(tolua_S,1,0);
  peak::Entity* entity = ((peak::Entity*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->init(entity);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'init'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: destroy of class  peak::GraphicsComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_GraphicsComponent_destroy00
static int tolua_peak_peak_GraphicsComponent_destroy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::GraphicsComponent",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::GraphicsComponent* self = (peak::GraphicsComponent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroy'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->destroy();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'destroy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: needsUpdate of class  peak::GraphicsComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_GraphicsComponent_needsUpdate00
static int tolua_peak_peak_GraphicsComponent_needsUpdate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::GraphicsComponent",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::GraphicsComponent* self = (peak::GraphicsComponent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'needsUpdate'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->needsUpdate();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'needsUpdate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getUpdateSize of class  peak::GraphicsComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_GraphicsComponent_getUpdateSize00
static int tolua_peak_peak_GraphicsComponent_getUpdateSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::GraphicsComponent",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::GraphicsComponent* self = (peak::GraphicsComponent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getUpdateSize'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getUpdateSize();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getUpdateSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: writeUpdateData of class  peak::GraphicsComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_GraphicsComponent_writeUpdateData00
static int tolua_peak_peak_GraphicsComponent_writeUpdateData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::GraphicsComponent",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::GraphicsComponent* self = (peak::GraphicsComponent*)  tolua_tousertype(tolua_S,1,0);
  peak::Buffer* buffer = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeUpdateData'",NULL);
#endif
  {
   int tolua_ret = (int)  self->writeUpdateData(buffer);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'writeUpdateData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: updateFromData of class  peak::GraphicsComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_GraphicsComponent_updateFromData00
static int tolua_peak_peak_GraphicsComponent_updateFromData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::GraphicsComponent",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::GraphicsComponent* self = (peak::GraphicsComponent*)  tolua_tousertype(tolua_S,1,0);
  peak::Buffer* buffer = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'updateFromData'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->updateFromData(buffer);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'updateFromData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: writeCompleteData of class  peak::GraphicsComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_GraphicsComponent_writeCompleteData00
static int tolua_peak_peak_GraphicsComponent_writeCompleteData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::GraphicsComponent",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::GraphicsComponent* self = (peak::GraphicsComponent*)  tolua_tousertype(tolua_S,1,0);
  peak::Buffer* buffer = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeCompleteData'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->writeCompleteData(buffer);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'writeCompleteData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: readCompleteData of class  peak::GraphicsComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_GraphicsComponent_readCompleteData00
static int tolua_peak_peak_GraphicsComponent_readCompleteData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::GraphicsComponent",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::GraphicsComponent* self = (peak::GraphicsComponent*)  tolua_tousertype(tolua_S,1,0);
  peak::Buffer* buffer = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readCompleteData'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->readCompleteData(buffer);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'readCompleteData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addSceneNode of class  peak::GraphicsComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_GraphicsComponent_addSceneNode00
static int tolua_peak_peak_GraphicsComponent_addSceneNode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::GraphicsComponent",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::SceneNode",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::GraphicsComponent* self = (peak::GraphicsComponent*)  tolua_tousertype(tolua_S,1,0);
  peak::SceneNode* node = ((peak::SceneNode*)  tolua_tousertype(tolua_S,2,0));
  int id = ((int)  tolua_tonumber(tolua_S,3,0));
  int parentid = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addSceneNode'",NULL);
#endif
  {
   self->addSceneNode(node,id,parentid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addSceneNode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSceneNode of class  peak::GraphicsComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_GraphicsComponent_getSceneNode00
static int tolua_peak_peak_GraphicsComponent_getSceneNode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::GraphicsComponent",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::GraphicsComponent* self = (peak::GraphicsComponent*)  tolua_tousertype(tolua_S,1,0);
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSceneNode'",NULL);
#endif
  {
   peak::SceneNode* tolua_ret = (peak::SceneNode*)  self->getSceneNode(id);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::SceneNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSceneNode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getType of class  peak::GraphicsComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_GraphicsComponent_getType00
static int tolua_peak_peak_GraphicsComponent_getType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::GraphicsComponent",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::GraphicsComponent* self = (peak::GraphicsComponent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getType'",NULL);
#endif
  {
   peak::ComponentType tolua_ret = (peak::ComponentType)  self->getType();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: doWork of class  peak::GraphicsComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_GraphicsComponent_doWork00
static int tolua_peak_peak_GraphicsComponent_doWork00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::GraphicsComponent",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::GraphicsComponent* self = (peak::GraphicsComponent*)  tolua_tousertype(tolua_S,1,0);
  float msecs = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'doWork'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->doWork(msecs);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'doWork'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::ServerScriptComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ServerScriptComponent_new00
static int tolua_peak_peak_ServerScriptComponent_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::ServerScriptComponent",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::ServerScriptComponent* tolua_ret = (peak::ServerScriptComponent*)  new peak::ServerScriptComponent();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::ServerScriptComponent");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::ServerScriptComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ServerScriptComponent_new00_local
static int tolua_peak_peak_ServerScriptComponent_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::ServerScriptComponent",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::ServerScriptComponent* tolua_ret = (peak::ServerScriptComponent*)  new peak::ServerScriptComponent();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::ServerScriptComponent");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::ServerScriptComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ServerScriptComponent_delete00
static int tolua_peak_peak_ServerScriptComponent_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::ServerScriptComponent",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::ServerScriptComponent* self = (peak::ServerScriptComponent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: init of class  peak::ServerScriptComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ServerScriptComponent_init00
static int tolua_peak_peak_ServerScriptComponent_init00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::ServerScriptComponent",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::ServerScriptComponent* self = (peak::ServerScriptComponent*)  tolua_tousertype(tolua_S,1,0);
  peak::Entity* entity = ((peak::Entity*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->init(entity);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'init'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: destroy of class  peak::ServerScriptComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ServerScriptComponent_destroy00
static int tolua_peak_peak_ServerScriptComponent_destroy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::ServerScriptComponent",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::ServerScriptComponent* self = (peak::ServerScriptComponent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroy'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->destroy();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'destroy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: needsUpdate of class  peak::ServerScriptComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ServerScriptComponent_needsUpdate00
static int tolua_peak_peak_ServerScriptComponent_needsUpdate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::ServerScriptComponent",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::ServerScriptComponent* self = (peak::ServerScriptComponent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'needsUpdate'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->needsUpdate();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'needsUpdate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getUpdateSize of class  peak::ServerScriptComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ServerScriptComponent_getUpdateSize00
static int tolua_peak_peak_ServerScriptComponent_getUpdateSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::ServerScriptComponent",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::ServerScriptComponent* self = (peak::ServerScriptComponent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getUpdateSize'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getUpdateSize();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getUpdateSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: writeUpdateData of class  peak::ServerScriptComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ServerScriptComponent_writeUpdateData00
static int tolua_peak_peak_ServerScriptComponent_writeUpdateData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::ServerScriptComponent",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::ServerScriptComponent* self = (peak::ServerScriptComponent*)  tolua_tousertype(tolua_S,1,0);
  peak::Buffer* buffer = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeUpdateData'",NULL);
#endif
  {
   int tolua_ret = (int)  self->writeUpdateData(buffer);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'writeUpdateData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: updateFromData of class  peak::ServerScriptComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ServerScriptComponent_updateFromData00
static int tolua_peak_peak_ServerScriptComponent_updateFromData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::ServerScriptComponent",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::ServerScriptComponent* self = (peak::ServerScriptComponent*)  tolua_tousertype(tolua_S,1,0);
  peak::Buffer* buffer = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'updateFromData'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->updateFromData(buffer);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'updateFromData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: writeCompleteData of class  peak::ServerScriptComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ServerScriptComponent_writeCompleteData00
static int tolua_peak_peak_ServerScriptComponent_writeCompleteData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::ServerScriptComponent",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::ServerScriptComponent* self = (peak::ServerScriptComponent*)  tolua_tousertype(tolua_S,1,0);
  peak::Buffer* buffer = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeCompleteData'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->writeCompleteData(buffer);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'writeCompleteData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: readCompleteData of class  peak::ServerScriptComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ServerScriptComponent_readCompleteData00
static int tolua_peak_peak_ServerScriptComponent_readCompleteData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::ServerScriptComponent",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::ServerScriptComponent* self = (peak::ServerScriptComponent*)  tolua_tousertype(tolua_S,1,0);
  peak::Buffer* buffer = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readCompleteData'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->readCompleteData(buffer);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'readCompleteData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: runScript of class  peak::ServerScriptComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ServerScriptComponent_runScript00
static int tolua_peak_peak_ServerScriptComponent_runScript00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::ServerScriptComponent",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::ServerScriptComponent* self = (peak::ServerScriptComponent*)  tolua_tousertype(tolua_S,1,0);
  std::string file = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'runScript'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->runScript(file);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'runScript'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getType of class  peak::ServerScriptComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ServerScriptComponent_getType00
static int tolua_peak_peak_ServerScriptComponent_getType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::ServerScriptComponent",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::ServerScriptComponent* self = (peak::ServerScriptComponent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getType'",NULL);
#endif
  {
   peak::ComponentType tolua_ret = (peak::ComponentType)  self->getType();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: doWork of class  peak::ServerScriptComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ServerScriptComponent_doWork00
static int tolua_peak_peak_ServerScriptComponent_doWork00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::ServerScriptComponent",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::ServerScriptComponent* self = (peak::ServerScriptComponent*)  tolua_tousertype(tolua_S,1,0);
  float msecs = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'doWork'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->doWork(msecs);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'doWork'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getScript of class  peak::ServerScriptComponent */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ServerScriptComponent_getScript00
static int tolua_peak_peak_ServerScriptComponent_getScript00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::ServerScriptComponent",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::ServerScriptComponent* self = (peak::ServerScriptComponent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getScript'",NULL);
#endif
  {
   peak::Script* tolua_ret = (peak::Script*)  self->getScript();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Script");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getScript'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::EntityVariables */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EntityVariables_new00
static int tolua_peak_peak_EntityVariables_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::EntityVariables",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::EntityVariables* tolua_ret = (peak::EntityVariables*)  new peak::EntityVariables();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::EntityVariables");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::EntityVariables */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EntityVariables_new00_local
static int tolua_peak_peak_EntityVariables_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::EntityVariables",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::EntityVariables* tolua_ret = (peak::EntityVariables*)  new peak::EntityVariables();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::EntityVariables");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::EntityVariables */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EntityVariables_delete00
static int tolua_peak_peak_EntityVariables_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::EntityVariables",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::EntityVariables* self = (peak::EntityVariables*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: load of class  peak::EntityVariables */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EntityVariables_load00
static int tolua_peak_peak_EntityVariables_load00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::EntityVariables",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::IniFile",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::EntityVariables* self = (peak::EntityVariables*)  tolua_tousertype(tolua_S,1,0);
  peak::IniFile* file = ((peak::IniFile*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'load'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->load(file);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: destroy of class  peak::EntityVariables */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EntityVariables_destroy00
static int tolua_peak_peak_EntityVariables_destroy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::EntityVariables",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::EntityVariables* self = (peak::EntityVariables*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroy'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->destroy();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'destroy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setInteger of class  peak::EntityVariables */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EntityVariables_setInteger00
static int tolua_peak_peak_EntityVariables_setInteger00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::EntityVariables",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::EntityVariables* self = (peak::EntityVariables*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  int value = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setInteger'",NULL);
#endif
  {
   self->setInteger(name,value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setInteger'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setFloat of class  peak::EntityVariables */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EntityVariables_setFloat00
static int tolua_peak_peak_EntityVariables_setFloat00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::EntityVariables",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::EntityVariables* self = (peak::EntityVariables*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  float value = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setFloat'",NULL);
#endif
  {
   self->setFloat(name,value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setFloat'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: set of class  peak::EntityVariables */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EntityVariables_set00
static int tolua_peak_peak_EntityVariables_set00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::EntityVariables",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::EntityVariables* self = (peak::EntityVariables*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  peak::Vector3D value = *((peak::Vector3D*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'",NULL);
#endif
  {
   self->set(name,value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: set of class  peak::EntityVariables */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EntityVariables_set01
static int tolua_peak_peak_EntityVariables_set01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::EntityVariables",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::Vector2D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::EntityVariables* self = (peak::EntityVariables*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  peak::Vector2D value = *((peak::Vector2D*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'",NULL);
#endif
  {
   self->set(name,value);
  }
 }
 return 0;
tolua_lerror:
 return tolua_peak_peak_EntityVariables_set00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: set of class  peak::EntityVariables */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EntityVariables_set02
static int tolua_peak_peak_EntityVariables_set02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::EntityVariables",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::EntityVariables* self = (peak::EntityVariables*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  peak::Quaternion value = *((peak::Quaternion*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'",NULL);
#endif
  {
   self->set(name,value);
  }
 }
 return 0;
tolua_lerror:
 return tolua_peak_peak_EntityVariables_set01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: set of class  peak::EntityVariables */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EntityVariables_set03
static int tolua_peak_peak_EntityVariables_set03(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::EntityVariables",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::EntityVariables* self = (peak::EntityVariables*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  std::string value = ((std::string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'",NULL);
#endif
  {
   self->set(name,value);
  }
 }
 return 0;
tolua_lerror:
 return tolua_peak_peak_EntityVariables_set02(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInteger of class  peak::EntityVariables */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EntityVariables_getInteger00
static int tolua_peak_peak_EntityVariables_getInteger00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::EntityVariables",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::EntityVariables* self = (peak::EntityVariables*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getInteger'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getInteger(name);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInteger'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getFloat of class  peak::EntityVariables */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EntityVariables_getFloat00
static int tolua_peak_peak_EntityVariables_getFloat00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::EntityVariables",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::EntityVariables* self = (peak::EntityVariables*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFloat'",NULL);
#endif
  {
   float tolua_ret = (float)  self->getFloat(name);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFloat'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVector3D of class  peak::EntityVariables */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EntityVariables_getVector3D00
static int tolua_peak_peak_EntityVariables_getVector3D00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::EntityVariables",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::EntityVariables* self = (peak::EntityVariables*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVector3D'",NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getVector3D(name);
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Vector3D(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVector3D'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVector2D of class  peak::EntityVariables */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EntityVariables_getVector2D00
static int tolua_peak_peak_EntityVariables_getVector2D00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::EntityVariables",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::EntityVariables* self = (peak::EntityVariables*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVector2D'",NULL);
#endif
  {
   peak::Vector2D tolua_ret = (peak::Vector2D)  self->getVector2D(name);
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Vector2D(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector2D");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector2D));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector2D");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVector2D'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getQuaternion of class  peak::EntityVariables */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EntityVariables_getQuaternion00
static int tolua_peak_peak_EntityVariables_getQuaternion00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::EntityVariables",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::EntityVariables* self = (peak::EntityVariables*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getQuaternion'",NULL);
#endif
  {
   peak::Quaternion tolua_ret = (peak::Quaternion)  self->getQuaternion(name);
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Quaternion(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Quaternion");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Quaternion));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Quaternion");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getQuaternion'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getString of class  peak::EntityVariables */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EntityVariables_getString00
static int tolua_peak_peak_EntityVariables_getString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::EntityVariables",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::EntityVariables* self = (peak::EntityVariables*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getString'",NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getString(name);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: needsUpdate of class  peak::EntityVariables */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EntityVariables_needsUpdate00
static int tolua_peak_peak_EntityVariables_needsUpdate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::EntityVariables",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::EntityVariables* self = (peak::EntityVariables*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'needsUpdate'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->needsUpdate();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'needsUpdate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getUpdateSize of class  peak::EntityVariables */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EntityVariables_getUpdateSize00
static int tolua_peak_peak_EntityVariables_getUpdateSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::EntityVariables",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::EntityVariables* self = (peak::EntityVariables*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getUpdateSize'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getUpdateSize();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getUpdateSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: writeUpdateData of class  peak::EntityVariables */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EntityVariables_writeUpdateData00
static int tolua_peak_peak_EntityVariables_writeUpdateData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::EntityVariables",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::EntityVariables* self = (peak::EntityVariables*)  tolua_tousertype(tolua_S,1,0);
  peak::Buffer* buffer = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeUpdateData'",NULL);
#endif
  {
   int tolua_ret = (int)  self->writeUpdateData(buffer);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'writeUpdateData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: updateFromData of class  peak::EntityVariables */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EntityVariables_updateFromData00
static int tolua_peak_peak_EntityVariables_updateFromData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::EntityVariables",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::EntityVariables* self = (peak::EntityVariables*)  tolua_tousertype(tolua_S,1,0);
  peak::Buffer* buffer = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'updateFromData'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->updateFromData(buffer);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'updateFromData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: writeCompleteData of class  peak::EntityVariables */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EntityVariables_writeCompleteData00
static int tolua_peak_peak_EntityVariables_writeCompleteData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::EntityVariables",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::EntityVariables* self = (peak::EntityVariables*)  tolua_tousertype(tolua_S,1,0);
  peak::Buffer* buffer = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeCompleteData'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->writeCompleteData(buffer);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'writeCompleteData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: readCompleteData of class  peak::EntityVariables */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EntityVariables_readCompleteData00
static int tolua_peak_peak_EntityVariables_readCompleteData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::EntityVariables",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::EntityVariables* self = (peak::EntityVariables*)  tolua_tousertype(tolua_S,1,0);
  peak::Buffer* buffer = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readCompleteData'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->readCompleteData(buffer);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'readCompleteData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: wasUpdated of class  peak::EntityVariables */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EntityVariables_wasUpdated00
static int tolua_peak_peak_EntityVariables_wasUpdated00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::EntityVariables",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::EntityVariables* self = (peak::EntityVariables*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'wasUpdated'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->wasUpdated(name);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'wasUpdated'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVariable of class  peak::EntityVariables */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EntityVariables_getVariable00
static int tolua_peak_peak_EntityVariables_getVariable00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::EntityVariables",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::EntityVariables* self = (peak::EntityVariables*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVariable'",NULL);
#endif
  {
   peak::Variable* tolua_ret = (peak::Variable*)  self->getVariable(name);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Variable");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVariable'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVariable of class  peak::EntityVariables */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EntityVariables_getVariable01
static int tolua_peak_peak_EntityVariables_getVariable01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::EntityVariables",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::EntityVariables* self = (peak::EntityVariables*)  tolua_tousertype(tolua_S,1,0);
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVariable'",NULL);
#endif
  {
   peak::Variable* tolua_ret = (peak::Variable*)  self->getVariable(id);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Variable");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_EntityVariables_getVariable00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::Component */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Component_delete00
static int tolua_peak_peak_Component_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Component",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Component* self = (peak::Component*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: init of class  peak::Component */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Component_init00
static int tolua_peak_peak_Component_init00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Component",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Component* self = (peak::Component*)  tolua_tousertype(tolua_S,1,0);
  peak::Entity* entity = ((peak::Entity*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->init(entity);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'init'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: destroy of class  peak::Component */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Component_destroy00
static int tolua_peak_peak_Component_destroy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Component",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Component* self = (peak::Component*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroy'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->destroy();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'destroy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: needsUpdate of class  peak::Component */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Component_needsUpdate00
static int tolua_peak_peak_Component_needsUpdate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Component",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Component* self = (peak::Component*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'needsUpdate'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->needsUpdate();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'needsUpdate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getUpdateSize of class  peak::Component */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Component_getUpdateSize00
static int tolua_peak_peak_Component_getUpdateSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Component",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Component* self = (peak::Component*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getUpdateSize'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getUpdateSize();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getUpdateSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: writeUpdateData of class  peak::Component */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Component_writeUpdateData00
static int tolua_peak_peak_Component_writeUpdateData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Component",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Component* self = (peak::Component*)  tolua_tousertype(tolua_S,1,0);
  peak::Buffer* buffer = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeUpdateData'",NULL);
#endif
  {
   int tolua_ret = (int)  self->writeUpdateData(buffer);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'writeUpdateData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: updateFromData of class  peak::Component */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Component_updateFromData00
static int tolua_peak_peak_Component_updateFromData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Component",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Component* self = (peak::Component*)  tolua_tousertype(tolua_S,1,0);
  peak::Buffer* buffer = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'updateFromData'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->updateFromData(buffer);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'updateFromData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: writeCompleteData of class  peak::Component */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Component_writeCompleteData00
static int tolua_peak_peak_Component_writeCompleteData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Component",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Component* self = (peak::Component*)  tolua_tousertype(tolua_S,1,0);
  peak::Buffer* buffer = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeCompleteData'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->writeCompleteData(buffer);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'writeCompleteData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: readCompleteData of class  peak::Component */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Component_readCompleteData00
static int tolua_peak_peak_Component_readCompleteData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Component",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Component* self = (peak::Component*)  tolua_tousertype(tolua_S,1,0);
  peak::Buffer* buffer = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readCompleteData'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->readCompleteData(buffer);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'readCompleteData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getType of class  peak::Component */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Component_getType00
static int tolua_peak_peak_Component_getType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Component",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Component* self = (peak::Component*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getType'",NULL);
#endif
  {
   peak::ComponentType tolua_ret = (peak::ComponentType)  self->getType();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: doWork of class  peak::Component */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Component_doWork00
static int tolua_peak_peak_Component_doWork00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Component",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Component* self = (peak::Component*)  tolua_tousertype(tolua_S,1,0);
  float msecs = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'doWork'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->doWork(msecs);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'doWork'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: scenenode of class  peak::SceneNodeData */
#ifndef TOLUA_DISABLE_tolua_get_peak__SceneNodeData_scenenode_ptr
static int tolua_get_peak__SceneNodeData_scenenode_ptr(lua_State* tolua_S)
{
  peak::SceneNodeData* self = (peak::SceneNodeData*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'scenenode'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)self->scenenode,"peak::SceneNode");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: scenenode of class  peak::SceneNodeData */
#ifndef TOLUA_DISABLE_tolua_set_peak__SceneNodeData_scenenode_ptr
static int tolua_set_peak__SceneNodeData_scenenode_ptr(lua_State* tolua_S)
{
  peak::SceneNodeData* self = (peak::SceneNodeData*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'scenenode'",NULL);
  if (!tolua_isusertype(tolua_S,2,"peak::SceneNode",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->scenenode = ((peak::SceneNode*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: parentid of class  peak::SceneNodeData */
#ifndef TOLUA_DISABLE_tolua_get_peak__SceneNodeData_parentid
static int tolua_get_peak__SceneNodeData_parentid(lua_State* tolua_S)
{
  peak::SceneNodeData* self = (peak::SceneNodeData*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'parentid'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->parentid);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: parentid of class  peak::SceneNodeData */
#ifndef TOLUA_DISABLE_tolua_set_peak__SceneNodeData_parentid
static int tolua_set_peak__SceneNodeData_parentid(lua_State* tolua_S)
{
  peak::SceneNodeData* self = (peak::SceneNodeData*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'parentid'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->parentid = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: body of class  peak::SceneNodeData */
#ifndef TOLUA_DISABLE_tolua_get_peak__SceneNodeData_body_ptr
static int tolua_get_peak__SceneNodeData_body_ptr(lua_State* tolua_S)
{
  peak::SceneNodeData* self = (peak::SceneNodeData*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'body'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)self->body,"peak::Body");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: body of class  peak::SceneNodeData */
#ifndef TOLUA_DISABLE_tolua_set_peak__SceneNodeData_body_ptr
static int tolua_set_peak__SceneNodeData_body_ptr(lua_State* tolua_S)
{
  peak::SceneNodeData* self = (peak::SceneNodeData*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'body'",NULL);
  if (!tolua_isusertype(tolua_S,2,"peak::Body",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->body = ((peak::Body*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: speed of class  peak::SceneNodeData */
#ifndef TOLUA_DISABLE_tolua_get_peak__SceneNodeData_speed
static int tolua_get_peak__SceneNodeData_speed(lua_State* tolua_S)
{
  peak::SceneNodeData* self = (peak::SceneNodeData*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'speed'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->speed,"peak::Vector3D");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: speed of class  peak::SceneNodeData */
#ifndef TOLUA_DISABLE_tolua_set_peak__SceneNodeData_speed
static int tolua_set_peak__SceneNodeData_speed(lua_State* tolua_S)
{
  peak::SceneNodeData* self = (peak::SceneNodeData*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'speed'",NULL);
  if (!tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->speed = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: oldspeed of class  peak::SceneNodeData */
#ifndef TOLUA_DISABLE_tolua_get_peak__SceneNodeData_oldspeed
static int tolua_get_peak__SceneNodeData_oldspeed(lua_State* tolua_S)
{
  peak::SceneNodeData* self = (peak::SceneNodeData*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'oldspeed'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->oldspeed,"peak::Vector3D");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: oldspeed of class  peak::SceneNodeData */
#ifndef TOLUA_DISABLE_tolua_set_peak__SceneNodeData_oldspeed
static int tolua_set_peak__SceneNodeData_oldspeed(lua_State* tolua_S)
{
  peak::SceneNodeData* self = (peak::SceneNodeData*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'oldspeed'",NULL);
  if (!tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->oldspeed = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: position of class  peak::SceneNodeData */
#ifndef TOLUA_DISABLE_tolua_get_peak__SceneNodeData_position
static int tolua_get_peak__SceneNodeData_position(lua_State* tolua_S)
{
  peak::SceneNodeData* self = (peak::SceneNodeData*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'position'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->position,"peak::Vector3D");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: position of class  peak::SceneNodeData */
#ifndef TOLUA_DISABLE_tolua_set_peak__SceneNodeData_position
static int tolua_set_peak__SceneNodeData_position(lua_State* tolua_S)
{
  peak::SceneNodeData* self = (peak::SceneNodeData*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'position'",NULL);
  if (!tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->position = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: rotation of class  peak::SceneNodeData */
#ifndef TOLUA_DISABLE_tolua_get_peak__SceneNodeData_rotation
static int tolua_get_peak__SceneNodeData_rotation(lua_State* tolua_S)
{
  peak::SceneNodeData* self = (peak::SceneNodeData*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'rotation'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->rotation,"peak::Quaternion");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: rotation of class  peak::SceneNodeData */
#ifndef TOLUA_DISABLE_tolua_set_peak__SceneNodeData_rotation
static int tolua_set_peak__SceneNodeData_rotation(lua_State* tolua_S)
{
  peak::SceneNodeData* self = (peak::SceneNodeData*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'rotation'",NULL);
  if (!tolua_isusertype(tolua_S,2,"peak::Quaternion",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->rotation = *((peak::Quaternion*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: update of class  peak::SceneNodeData */
#ifndef TOLUA_DISABLE_tolua_get_peak__SceneNodeData_unsigned_update
static int tolua_get_peak__SceneNodeData_unsigned_update(lua_State* tolua_S)
{
  peak::SceneNodeData* self = (peak::SceneNodeData*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'update'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->update);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: update of class  peak::SceneNodeData */
#ifndef TOLUA_DISABLE_tolua_set_peak__SceneNodeData_unsigned_update
static int tolua_set_peak__SceneNodeData_unsigned_update(lua_State* tolua_S)
{
  peak::SceneNodeData* self = (peak::SceneNodeData*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'update'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->update = ((unsigned char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Variable */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Variable_new00
static int tolua_peak_peak_Variable_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Variable",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Variable* tolua_ret = (peak::Variable*)  new peak::Variable();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Variable");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Variable */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Variable_new00_local
static int tolua_peak_peak_Variable_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Variable",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Variable* tolua_ret = (peak::Variable*)  new peak::Variable();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Variable");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInteger of class  peak::Variable */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Variable_getInteger00
static int tolua_peak_peak_Variable_getInteger00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Variable",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Variable* self = (peak::Variable*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getInteger'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getInteger();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInteger'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getFloat of class  peak::Variable */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Variable_getFloat00
static int tolua_peak_peak_Variable_getFloat00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Variable",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Variable* self = (peak::Variable*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFloat'",NULL);
#endif
  {
   float tolua_ret = (float)  self->getFloat();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFloat'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVector3D of class  peak::Variable */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Variable_getVector3D00
static int tolua_peak_peak_Variable_getVector3D00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Variable",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Variable* self = (peak::Variable*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVector3D'",NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getVector3D();
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Vector3D(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVector3D'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVector2D of class  peak::Variable */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Variable_getVector2D00
static int tolua_peak_peak_Variable_getVector2D00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Variable",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Variable* self = (peak::Variable*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVector2D'",NULL);
#endif
  {
   peak::Vector2D tolua_ret = (peak::Vector2D)  self->getVector2D();
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Vector2D(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector2D");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector2D));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector2D");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVector2D'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getQuaternion of class  peak::Variable */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Variable_getQuaternion00
static int tolua_peak_peak_Variable_getQuaternion00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Variable",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Variable* self = (peak::Variable*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getQuaternion'",NULL);
#endif
  {
   peak::Quaternion tolua_ret = (peak::Quaternion)  self->getQuaternion();
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Quaternion(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Quaternion");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Quaternion));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Quaternion");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getQuaternion'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getString of class  peak::Variable */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Variable_getString00
static int tolua_peak_peak_Variable_getString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Variable",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Variable* self = (peak::Variable*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getString'",NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getString();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: wasUpdated of class  peak::Variable */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Variable_wasUpdated00
static int tolua_peak_peak_Variable_wasUpdated00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Variable",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Variable* self = (peak::Variable*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'wasUpdated'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->wasUpdated();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'wasUpdated'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Menu */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Menu_new00
static int tolua_peak_peak_Menu_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Menu",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Menu* tolua_ret = (peak::Menu*)  new peak::Menu();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Menu");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Menu */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Menu_new00_local
static int tolua_peak_peak_Menu_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Menu",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Menu* tolua_ret = (peak::Menu*)  new peak::Menu();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Menu");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::Menu */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Menu_delete00
static int tolua_peak_peak_Menu_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Menu",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Menu* self = (peak::Menu*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: init of class  peak::Menu */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Menu_init00
static int tolua_peak_peak_Menu_init00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Menu",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Menu* self = (peak::Menu*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->init(name);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'init'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: destroy of class  peak::Menu */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Menu_destroy00
static int tolua_peak_peak_Menu_destroy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Menu",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Menu* self = (peak::Menu*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroy'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->destroy();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'destroy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getName of class  peak::Menu */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Menu_getName00
static int tolua_peak_peak_Menu_getName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Menu",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Menu* self = (peak::Menu*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getName'",NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getName();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: show of class  peak::Menu */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Menu_show00
static int tolua_peak_peak_Menu_show00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Menu",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Menu* self = (peak::Menu*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'show'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->show();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'show'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: hide of class  peak::Menu */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Menu_hide00
static int tolua_peak_peak_Menu_hide00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Menu",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Menu* self = (peak::Menu*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hide'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->hide();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hide'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: pressButton of class  peak::Menu */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Menu_pressButton00
static int tolua_peak_peak_Menu_pressButton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Menu",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Menu* self = (peak::Menu*)  tolua_tousertype(tolua_S,1,0);
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'pressButton'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->pressButton(id);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'pressButton'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setCheckBox of class  peak::Menu */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Menu_setCheckBox00
static int tolua_peak_peak_Menu_setCheckBox00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Menu",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Menu* self = (peak::Menu*)  tolua_tousertype(tolua_S,1,0);
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
  bool value = ((bool)  tolua_toboolean(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setCheckBox'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->setCheckBox(id,value);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setCheckBox'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setListBox of class  peak::Menu */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Menu_setListBox00
static int tolua_peak_peak_Menu_setListBox00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Menu",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Menu* self = (peak::Menu*)  tolua_tousertype(tolua_S,1,0);
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
  int selected = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setListBox'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->setListBox(id,selected);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setListBox'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getListBox of class  peak::Menu */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Menu_getListBox00
static int tolua_peak_peak_Menu_getListBox00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Menu",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Menu* self = (peak::Menu*)  tolua_tousertype(tolua_S,1,0);
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getListBox'",NULL);
#endif
  {
   peak::ListBox* tolua_ret = (peak::ListBox*)  self->getListBox(id);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::ListBox");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getListBox'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getButton of class  peak::Menu */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Menu_getButton00
static int tolua_peak_peak_Menu_getButton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Menu",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Menu* self = (peak::Menu*)  tolua_tousertype(tolua_S,1,0);
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getButton'",NULL);
#endif
  {
   peak::Button* tolua_ret = (peak::Button*)  self->getButton(id);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Button");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getButton'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getLabel of class  peak::Menu */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Menu_getLabel00
static int tolua_peak_peak_Menu_getLabel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Menu",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Menu* self = (peak::Menu*)  tolua_tousertype(tolua_S,1,0);
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLabel'",NULL);
#endif
  {
   peak::Label* tolua_ret = (peak::Label*)  self->getLabel(id);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Label");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLabel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getElement of class  peak::Menu */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Menu_getElement00
static int tolua_peak_peak_Menu_getElement00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Menu",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Menu* self = (peak::Menu*)  tolua_tousertype(tolua_S,1,0);
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getElement'",NULL);
#endif
  {
   peak::MenuElement* tolua_ret = (peak::MenuElement*)  self->getElement(id);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::MenuElement");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getElement'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getScript of class  peak::Menu */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Menu_getScript00
static int tolua_peak_peak_Menu_getScript00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Menu",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Menu* self = (peak::Menu*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getScript'",NULL);
#endif
  {
   peak::Script* tolua_ret = (peak::Script*)  self->getScript();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Script");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getScript'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get of class  peak::MenuManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_MenuManager_get00
static int tolua_peak_peak_MenuManager_get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::MenuManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::MenuManager* tolua_ret = (peak::MenuManager*)  peak::MenuManager::get();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::MenuManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::MenuManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_MenuManager_delete00
static int tolua_peak_peak_MenuManager_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::MenuManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::MenuManager* self = (peak::MenuManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: loadMenu of class  peak::MenuManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_MenuManager_loadMenu00
static int tolua_peak_peak_MenuManager_loadMenu00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::MenuManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::MenuManager* self = (peak::MenuManager*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'loadMenu'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->loadMenu(name);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'loadMenu'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: unloadMenu of class  peak::MenuManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_MenuManager_unloadMenu00
static int tolua_peak_peak_MenuManager_unloadMenu00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::MenuManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::MenuManager* self = (peak::MenuManager*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'unloadMenu'",NULL);
#endif
  {
   self->unloadMenu(name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'unloadMenu'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: unloadMenu of class  peak::MenuManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_MenuManager_unloadMenu01
static int tolua_peak_peak_MenuManager_unloadMenu01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::MenuManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Menu",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::MenuManager* self = (peak::MenuManager*)  tolua_tousertype(tolua_S,1,0);
  peak::Menu* menu = ((peak::Menu*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'unloadMenu'",NULL);
#endif
  {
   self->unloadMenu(menu);
  }
 }
 return 0;
tolua_lerror:
 return tolua_peak_peak_MenuManager_unloadMenu00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMenuCount of class  peak::MenuManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_MenuManager_getMenuCount00
static int tolua_peak_peak_MenuManager_getMenuCount00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::MenuManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::MenuManager* self = (peak::MenuManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMenuCount'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getMenuCount();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMenuCount'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMenu of class  peak::MenuManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_MenuManager_getMenu00
static int tolua_peak_peak_MenuManager_getMenu00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::MenuManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::MenuManager* self = (peak::MenuManager*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMenu'",NULL);
#endif
  {
   peak::Menu* tolua_ret = (peak::Menu*)  self->getMenu(index);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Menu");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMenu'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMenu of class  peak::MenuManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_MenuManager_getMenu01
static int tolua_peak_peak_MenuManager_getMenu01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::MenuManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::MenuManager* self = (peak::MenuManager*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMenu'",NULL);
#endif
  {
   peak::Menu* tolua_ret = (peak::Menu*)  self->getMenu(name);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Menu");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_MenuManager_getMenu00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getCurrentMenu of class  peak::MenuManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_MenuManager_getCurrentMenu00
static int tolua_peak_peak_MenuManager_getCurrentMenu00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::MenuManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::MenuManager* self = (peak::MenuManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCurrentMenu'",NULL);
#endif
  {
   peak::Menu* tolua_ret = (peak::Menu*)  self->getCurrentMenu();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Menu");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCurrentMenu'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: init of class  peak::MenuManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_MenuManager_init00
static int tolua_peak_peak_MenuManager_init00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::MenuManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::MenuManager* self = (peak::MenuManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->init();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'init'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: shutdown of class  peak::MenuManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_MenuManager_shutdown00
static int tolua_peak_peak_MenuManager_shutdown00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::MenuManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::MenuManager* self = (peak::MenuManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'shutdown'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->shutdown();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'shutdown'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: activateMenu of class  peak::MenuManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_MenuManager_activateMenu00
static int tolua_peak_peak_MenuManager_activateMenu00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::MenuManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::MenuManager* self = (peak::MenuManager*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'activateMenu'",NULL);
#endif
  {
   self->activateMenu(name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'activateMenu'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: activateMenu of class  peak::MenuManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_MenuManager_activateMenu01
static int tolua_peak_peak_MenuManager_activateMenu01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::MenuManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Menu",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::MenuManager* self = (peak::MenuManager*)  tolua_tousertype(tolua_S,1,0);
  peak::Menu* menu = ((peak::Menu*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'activateMenu'",NULL);
#endif
  {
   self->activateMenu(menu);
  }
 }
 return 0;
tolua_lerror:
 return tolua_peak_peak_MenuManager_activateMenu00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::MenuElement */
#ifndef TOLUA_DISABLE_tolua_peak_peak_MenuElement_delete00
static int tolua_peak_peak_MenuElement_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::MenuElement",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::MenuElement* self = (peak::MenuElement*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: init of class  peak::MenuElement */
#ifndef TOLUA_DISABLE_tolua_peak_peak_MenuElement_init00
static int tolua_peak_peak_MenuElement_init00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::MenuElement",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Menu",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::MenuElement",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::MenuElement* self = (peak::MenuElement*)  tolua_tousertype(tolua_S,1,0);
  peak::Menu* menu = ((peak::Menu*)  tolua_tousertype(tolua_S,2,0));
  peak::MenuElement* parent = ((peak::MenuElement*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->init(menu,parent);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'init'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: destroy of class  peak::MenuElement */
#ifndef TOLUA_DISABLE_tolua_peak_peak_MenuElement_destroy00
static int tolua_peak_peak_MenuElement_destroy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::MenuElement",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::MenuElement* self = (peak::MenuElement*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroy'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->destroy();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'destroy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setPosition of class  peak::MenuElement */
#ifndef TOLUA_DISABLE_tolua_peak_peak_MenuElement_setPosition00
static int tolua_peak_peak_MenuElement_setPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::MenuElement",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Vector2D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::MenuElement* self = (peak::MenuElement*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector2D position = *((peak::Vector2D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPosition'",NULL);
#endif
  {
   self->setPosition(position);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPosition of class  peak::MenuElement */
#ifndef TOLUA_DISABLE_tolua_peak_peak_MenuElement_getPosition00
static int tolua_peak_peak_MenuElement_getPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::MenuElement",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::MenuElement* self = (peak::MenuElement*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPosition'",NULL);
#endif
  {
   peak::Vector2D tolua_ret = (peak::Vector2D)  self->getPosition();
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Vector2D(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector2D");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector2D));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector2D");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setSize of class  peak::MenuElement */
#ifndef TOLUA_DISABLE_tolua_peak_peak_MenuElement_setSize00
static int tolua_peak_peak_MenuElement_setSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::MenuElement",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Vector2D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::MenuElement* self = (peak::MenuElement*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector2D size = *((peak::Vector2D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setSize'",NULL);
#endif
  {
   self->setSize(size);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSize of class  peak::MenuElement */
#ifndef TOLUA_DISABLE_tolua_peak_peak_MenuElement_getSize00
static int tolua_peak_peak_MenuElement_getSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::MenuElement",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::MenuElement* self = (peak::MenuElement*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSize'",NULL);
#endif
  {
   peak::Vector2D tolua_ret = (peak::Vector2D)  self->getSize();
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Vector2D(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector2D");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector2D));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector2D");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setID of class  peak::MenuElement */
#ifndef TOLUA_DISABLE_tolua_peak_peak_MenuElement_setID00
static int tolua_peak_peak_MenuElement_setID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::MenuElement",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::MenuElement* self = (peak::MenuElement*)  tolua_tousertype(tolua_S,1,0);
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setID'",NULL);
#endif
  {
   self->setID(id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setID'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getID of class  peak::MenuElement */
#ifndef TOLUA_DISABLE_tolua_peak_peak_MenuElement_getID00
static int tolua_peak_peak_MenuElement_getID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::MenuElement",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::MenuElement* self = (peak::MenuElement*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getID'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getID();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getID'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: show of class  peak::MenuElement */
#ifndef TOLUA_DISABLE_tolua_peak_peak_MenuElement_show00
static int tolua_peak_peak_MenuElement_show00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::MenuElement",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::MenuElement* self = (peak::MenuElement*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'show'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->show();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'show'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: hide of class  peak::MenuElement */
#ifndef TOLUA_DISABLE_tolua_peak_peak_MenuElement_hide00
static int tolua_peak_peak_MenuElement_hide00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::MenuElement",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::MenuElement* self = (peak::MenuElement*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hide'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->hide();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hide'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setText of class  peak::MenuElement */
#ifndef TOLUA_DISABLE_tolua_peak_peak_MenuElement_setText00
static int tolua_peak_peak_MenuElement_setText00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::MenuElement",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::MenuElement* self = (peak::MenuElement*)  tolua_tousertype(tolua_S,1,0);
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setText'",NULL);
#endif
  {
   self->setText(text);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setText'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getText of class  peak::MenuElement */
#ifndef TOLUA_DISABLE_tolua_peak_peak_MenuElement_getText00
static int tolua_peak_peak_MenuElement_getText00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::MenuElement",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::MenuElement* self = (peak::MenuElement*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getText'",NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getText();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getText'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getType of class  peak::MenuElement */
#ifndef TOLUA_DISABLE_tolua_peak_peak_MenuElement_getType00
static int tolua_peak_peak_MenuElement_getType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::MenuElement",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::MenuElement* self = (peak::MenuElement*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getType'",NULL);
#endif
  {
   peak::EMenuElementType tolua_ret = (peak::EMenuElementType)  self->getType();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::ListBox */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ListBox_new00
static int tolua_peak_peak_ListBox_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::ListBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::ListBox* tolua_ret = (peak::ListBox*)  new peak::ListBox();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::ListBox");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::ListBox */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ListBox_new00_local
static int tolua_peak_peak_ListBox_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::ListBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::ListBox* tolua_ret = (peak::ListBox*)  new peak::ListBox();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::ListBox");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::ListBox */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ListBox_delete00
static int tolua_peak_peak_ListBox_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::ListBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::ListBox* self = (peak::ListBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: init of class  peak::ListBox */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ListBox_init00
static int tolua_peak_peak_ListBox_init00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::ListBox",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Menu",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::MenuElement",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::ListBox* self = (peak::ListBox*)  tolua_tousertype(tolua_S,1,0);
  peak::Menu* menu = ((peak::Menu*)  tolua_tousertype(tolua_S,2,0));
  peak::MenuElement* parent = ((peak::MenuElement*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->init(menu,parent);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'init'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: show of class  peak::ListBox */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ListBox_show00
static int tolua_peak_peak_ListBox_show00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::ListBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::ListBox* self = (peak::ListBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'show'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->show();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'show'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: hide of class  peak::ListBox */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ListBox_hide00
static int tolua_peak_peak_ListBox_hide00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::ListBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::ListBox* self = (peak::ListBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hide'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->hide();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hide'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: choose of class  peak::ListBox */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ListBox_choose00
static int tolua_peak_peak_ListBox_choose00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::ListBox",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::ListBox* self = (peak::ListBox*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'choose'",NULL);
#endif
  {
   self->choose(index);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'choose'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSelected of class  peak::ListBox */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ListBox_getSelected00
static int tolua_peak_peak_ListBox_getSelected00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::ListBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::ListBox* self = (peak::ListBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSelected'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getSelected();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSelected'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addItem of class  peak::ListBox */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ListBox_addItem00
static int tolua_peak_peak_ListBox_addItem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::ListBox",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::ListBox* self = (peak::ListBox*)  tolua_tousertype(tolua_S,1,0);
  std::string label = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  int index = ((int)  tolua_tonumber(tolua_S,3,-1));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addItem'",NULL);
#endif
  {
   self->addItem(label,index);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addItem'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: deleteItem of class  peak::ListBox */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ListBox_deleteItem00
static int tolua_peak_peak_ListBox_deleteItem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::ListBox",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::ListBox* self = (peak::ListBox*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'deleteItem'",NULL);
#endif
  {
   self->deleteItem(index);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'deleteItem'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getItemCount of class  peak::ListBox */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ListBox_getItemCount00
static int tolua_peak_peak_ListBox_getItemCount00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::ListBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::ListBox* self = (peak::ListBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getItemCount'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getItemCount();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getItemCount'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getItem of class  peak::ListBox */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ListBox_getItem00
static int tolua_peak_peak_ListBox_getItem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::ListBox",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::ListBox* self = (peak::ListBox*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getItem'",NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getItem(index);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getItem'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clearItems of class  peak::ListBox */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ListBox_clearItems00
static int tolua_peak_peak_ListBox_clearItems00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::ListBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::ListBox* self = (peak::ListBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clearItems'",NULL);
#endif
  {
   self->clearItems();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clearItems'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: updateItems of class  peak::ListBox */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ListBox_updateItems00
static int tolua_peak_peak_ListBox_updateItems00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::ListBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::ListBox* self = (peak::ListBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'updateItems'",NULL);
#endif
  {
   self->updateItems();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'updateItems'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAction of class  peak::ListBox */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ListBox_setAction00
static int tolua_peak_peak_ListBox_setAction00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::ListBox",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::ListBox* self = (peak::ListBox*)  tolua_tousertype(tolua_S,1,0);
  std::string action = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAction'",NULL);
#endif
  {
   self->setAction(action);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAction'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAction of class  peak::ListBox */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ListBox_getAction00
static int tolua_peak_peak_ListBox_getAction00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::ListBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::ListBox* self = (peak::ListBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAction'",NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getAction();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAction'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getType of class  peak::ListBox */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ListBox_getType00
static int tolua_peak_peak_ListBox_getType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::ListBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::ListBox* self = (peak::ListBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getType'",NULL);
#endif
  {
   peak::EMenuElementType tolua_ret = (peak::EMenuElementType)  self->getType();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Sound */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Sound_new00
static int tolua_peak_peak_Sound_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Sound",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Sound* tolua_ret = (peak::Sound*)  new peak::Sound();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Sound");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Sound */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Sound_new00_local
static int tolua_peak_peak_Sound_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Sound",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Sound* tolua_ret = (peak::Sound*)  new peak::Sound();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Sound");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::Sound */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Sound_delete00
static int tolua_peak_peak_Sound_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Sound",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Sound* self = (peak::Sound*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: init of class  peak::Sound */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Sound_init00
static int tolua_peak_peak_Sound_init00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Sound",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::SoundSource",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Sound* self = (peak::Sound*)  tolua_tousertype(tolua_S,1,0);
  peak::SoundSource* source = ((peak::SoundSource*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->init(source);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'init'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: destroy of class  peak::Sound */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Sound_destroy00
static int tolua_peak_peak_Sound_destroy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Sound",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Sound* self = (peak::Sound*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroy'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->destroy();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'destroy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSource of class  peak::Sound */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Sound_getSource00
static int tolua_peak_peak_Sound_getSource00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Sound",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Sound* self = (peak::Sound*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSource'",NULL);
#endif
  {
   peak::SoundSource* tolua_ret = (peak::SoundSource*)  self->getSource();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::SoundSource");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSource'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setLooped of class  peak::Sound */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Sound_setLooped00
static int tolua_peak_peak_Sound_setLooped00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Sound",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Sound* self = (peak::Sound*)  tolua_tousertype(tolua_S,1,0);
  bool looped = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setLooped'",NULL);
#endif
  {
   self->setLooped(looped);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setLooped'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getLooped of class  peak::Sound */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Sound_getLooped00
static int tolua_peak_peak_Sound_getLooped00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Sound",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Sound* self = (peak::Sound*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLooped'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getLooped();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLooped'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setCleanUp of class  peak::Sound */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Sound_setCleanUp00
static int tolua_peak_peak_Sound_setCleanUp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Sound",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Sound* self = (peak::Sound*)  tolua_tousertype(tolua_S,1,0);
  bool cleanup = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setCleanUp'",NULL);
#endif
  {
   self->setCleanUp(cleanup);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setCleanUp'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getCleanUp of class  peak::Sound */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Sound_getCleanUp00
static int tolua_peak_peak_Sound_getCleanUp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Sound",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Sound* self = (peak::Sound*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCleanUp'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getCleanUp();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCleanUp'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setPaused of class  peak::Sound */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Sound_setPaused00
static int tolua_peak_peak_Sound_setPaused00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Sound",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Sound* self = (peak::Sound*)  tolua_tousertype(tolua_S,1,0);
  bool paused = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPaused'",NULL);
#endif
  {
   self->setPaused(paused);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setPaused'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isPaused of class  peak::Sound */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Sound_isPaused00
static int tolua_peak_peak_Sound_isPaused00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Sound",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Sound* self = (peak::Sound*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isPaused'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isPaused();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isPaused'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setPlaying of class  peak::Sound */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Sound_setPlaying00
static int tolua_peak_peak_Sound_setPlaying00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Sound",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Sound* self = (peak::Sound*)  tolua_tousertype(tolua_S,1,0);
  bool playing = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPlaying'",NULL);
#endif
  {
   self->setPlaying(playing);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setPlaying'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isPlaying of class  peak::Sound */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Sound_isPlaying00
static int tolua_peak_peak_Sound_isPlaying00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Sound",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Sound* self = (peak::Sound*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isPlaying'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isPlaying();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isPlaying'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setVolume of class  peak::Sound */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Sound_setVolume00
static int tolua_peak_peak_Sound_setVolume00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Sound",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Sound* self = (peak::Sound*)  tolua_tousertype(tolua_S,1,0);
  float volume = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setVolume'",NULL);
#endif
  {
   self->setVolume(volume);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setVolume'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVolume of class  peak::Sound */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Sound_getVolume00
static int tolua_peak_peak_Sound_getVolume00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Sound",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Sound* self = (peak::Sound*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVolume'",NULL);
#endif
  {
   float tolua_ret = (float)  self->getVolume();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVolume'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setPosition of class  peak::Sound */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Sound_setPosition00
static int tolua_peak_peak_Sound_setPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Sound",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Sound* self = (peak::Sound*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D pos = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPosition'",NULL);
#endif
  {
   self->setPosition(pos);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPosition of class  peak::Sound */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Sound_getPosition00
static int tolua_peak_peak_Sound_getPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Sound",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Sound* self = (peak::Sound*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPosition'",NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getPosition();
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Vector3D(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setVelocity of class  peak::Sound */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Sound_setVelocity00
static int tolua_peak_peak_Sound_setVelocity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Sound",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Sound* self = (peak::Sound*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D vel = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setVelocity'",NULL);
#endif
  {
   self->setVelocity(vel);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setVelocity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVelocity of class  peak::Sound */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Sound_getVelocity00
static int tolua_peak_peak_Sound_getVelocity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Sound",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Sound* self = (peak::Sound*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVelocity'",NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getVelocity();
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Vector3D(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVelocity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setRelativeToListener of class  peak::Sound */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Sound_setRelativeToListener00
static int tolua_peak_peak_Sound_setRelativeToListener00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Sound",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Sound* self = (peak::Sound*)  tolua_tousertype(tolua_S,1,0);
  bool rel = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRelativeToListener'",NULL);
#endif
  {
   self->setRelativeToListener(rel);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setRelativeToListener'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isRelativeToListener of class  peak::Sound */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Sound_isRelativeToListener00
static int tolua_peak_peak_Sound_isRelativeToListener00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Sound",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Sound* self = (peak::Sound*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isRelativeToListener'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isRelativeToListener();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isRelativeToListener'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: attachToSceneNode of class  peak::Sound */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Sound_attachToSceneNode00
static int tolua_peak_peak_Sound_attachToSceneNode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Sound",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::SceneNode",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Sound* self = (peak::Sound*)  tolua_tousertype(tolua_S,1,0);
  peak::SceneNode* node = ((peak::SceneNode*)  tolua_tousertype(tolua_S,2,0));
  bool adjustspeed = ((bool)  tolua_toboolean(tolua_S,3,false));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'attachToSceneNode'",NULL);
#endif
  {
   self->attachToSceneNode(node,adjustspeed);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'attachToSceneNode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: doWork of class  peak::Sound */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Sound_doWork00
static int tolua_peak_peak_Sound_doWork00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Sound",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Sound* self = (peak::Sound*)  tolua_tousertype(tolua_S,1,0);
  float msecs = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'doWork'",NULL);
#endif
  {
   self->doWork(msecs);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'doWork'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Listener */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Listener_new00
static int tolua_peak_peak_Listener_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Listener",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Listener* tolua_ret = (peak::Listener*)  new peak::Listener();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Listener");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  peak::Listener */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Listener_new00_local
static int tolua_peak_peak_Listener_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Listener",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Listener* tolua_ret = (peak::Listener*)  new peak::Listener();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"peak::Listener");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::Listener */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Listener_delete00
static int tolua_peak_peak_Listener_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Listener",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Listener* self = (peak::Listener*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setPosition of class  peak::Listener */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Listener_setPosition00
static int tolua_peak_peak_Listener_setPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Listener",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Listener* self = (peak::Listener*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D pos = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPosition'",NULL);
#endif
  {
   self->setPosition(pos);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPosition of class  peak::Listener */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Listener_getPosition00
static int tolua_peak_peak_Listener_getPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Listener",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Listener* self = (peak::Listener*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPosition'",NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getPosition();
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Vector3D(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setVelocity of class  peak::Listener */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Listener_setVelocity00
static int tolua_peak_peak_Listener_setVelocity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Listener",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Listener* self = (peak::Listener*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D vel = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setVelocity'",NULL);
#endif
  {
   self->setVelocity(vel);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setVelocity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVelocity of class  peak::Listener */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Listener_getVelocity00
static int tolua_peak_peak_Listener_getVelocity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Listener",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Listener* self = (peak::Listener*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVelocity'",NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getVelocity();
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Vector3D(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Vector3D");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVelocity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setRotation of class  peak::Listener */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Listener_setRotation00
static int tolua_peak_peak_Listener_setRotation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Listener",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Listener* self = (peak::Listener*)  tolua_tousertype(tolua_S,1,0);
  peak::Quaternion rot = *((peak::Quaternion*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRotation'",NULL);
#endif
  {
   self->setRotation(rot);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setRotation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRotation of class  peak::Listener */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Listener_getRotation00
static int tolua_peak_peak_Listener_getRotation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Listener",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Listener* self = (peak::Listener*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRotation'",NULL);
#endif
  {
   peak::Quaternion tolua_ret = (peak::Quaternion)  self->getRotation();
   {
#ifdef __cplusplus
    void* tolua_obj = new peak::Quaternion(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Quaternion");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Quaternion));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"peak::Quaternion");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRotation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: attachToSceneNode of class  peak::Listener */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Listener_attachToSceneNode00
static int tolua_peak_peak_Listener_attachToSceneNode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Listener",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::SceneNode",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Listener* self = (peak::Listener*)  tolua_tousertype(tolua_S,1,0);
  peak::SceneNode* node = ((peak::SceneNode*)  tolua_tousertype(tolua_S,2,0));
  bool rotation = ((bool)  tolua_toboolean(tolua_S,3,false));
  bool adjustspeed = ((bool)  tolua_toboolean(tolua_S,4,false));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'attachToSceneNode'",NULL);
#endif
  {
   self->attachToSceneNode(node,rotation,adjustspeed);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'attachToSceneNode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: doWork of class  peak::Listener */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Listener_doWork00
static int tolua_peak_peak_Listener_doWork00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Listener",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Listener* self = (peak::Listener*)  tolua_tousertype(tolua_S,1,0);
  float msecs = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'doWork'",NULL);
#endif
  {
   self->doWork(msecs);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'doWork'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::SoundSource */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SoundSource_delete00
static int tolua_peak_peak_SoundSource_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SoundSource",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SoundSource* self = (peak::SoundSource*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getLength of class  peak::SoundSource */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SoundSource_getLength00
static int tolua_peak_peak_SoundSource_getLength00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SoundSource",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SoundSource* self = (peak::SoundSource*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLength'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getLength();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLength'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isSeekable of class  peak::SoundSource */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SoundSource_isSeekable00
static int tolua_peak_peak_SoundSource_isSeekable00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SoundSource",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SoundSource* self = (peak::SoundSource*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isSeekable'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isSeekable();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isSeekable'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isStreamed of class  peak::SoundSource */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SoundSource_isStreamed00
static int tolua_peak_peak_SoundSource_isStreamed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SoundSource",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SoundSource* self = (peak::SoundSource*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isStreamed'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isStreamed();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isStreamed'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get of class  peak::SoundEngine */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SoundEngine_get00
static int tolua_peak_peak_SoundEngine_get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::SoundEngine",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::SoundEngine* tolua_ret = (peak::SoundEngine*)  peak::SoundEngine::get();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::SoundEngine");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::SoundEngine */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SoundEngine_delete00
static int tolua_peak_peak_SoundEngine_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SoundEngine",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SoundEngine* self = (peak::SoundEngine*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: init of class  peak::SoundEngine */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SoundEngine_init00
static int tolua_peak_peak_SoundEngine_init00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SoundEngine",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SoundEngine* self = (peak::SoundEngine*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->init();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'init'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: shutdown of class  peak::SoundEngine */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SoundEngine_shutdown00
static int tolua_peak_peak_SoundEngine_shutdown00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SoundEngine",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SoundEngine* self = (peak::SoundEngine*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'shutdown'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->shutdown();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'shutdown'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: running of class  peak::SoundEngine */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SoundEngine_running00
static int tolua_peak_peak_SoundEngine_running00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SoundEngine",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SoundEngine* self = (peak::SoundEngine*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'running'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->running();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'running'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: play2D of class  peak::SoundEngine */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SoundEngine_play2D00
static int tolua_peak_peak_SoundEngine_play2D00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SoundEngine",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isboolean(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SoundEngine* self = (peak::SoundEngine*)  tolua_tousertype(tolua_S,1,0);
  std::string filename = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  bool paused = ((bool)  tolua_toboolean(tolua_S,3,false));
  bool looped = ((bool)  tolua_toboolean(tolua_S,4,false));
  bool cleanup = ((bool)  tolua_toboolean(tolua_S,5,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'play2D'",NULL);
#endif
  {
   peak::Sound* tolua_ret = (peak::Sound*)  self->play2D(filename,paused,looped,cleanup);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Sound");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'play2D'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: play2D of class  peak::SoundEngine */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SoundEngine_play2D01
static int tolua_peak_peak_SoundEngine_play2D01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SoundEngine",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::SoundSource",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isboolean(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::SoundEngine* self = (peak::SoundEngine*)  tolua_tousertype(tolua_S,1,0);
  peak::SoundSource* source = ((peak::SoundSource*)  tolua_tousertype(tolua_S,2,0));
  bool paused = ((bool)  tolua_toboolean(tolua_S,3,false));
  bool looped = ((bool)  tolua_toboolean(tolua_S,4,false));
  bool cleanup = ((bool)  tolua_toboolean(tolua_S,5,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'play2D'",NULL);
#endif
  {
   peak::Sound* tolua_ret = (peak::Sound*)  self->play2D(source,paused,looped,cleanup);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Sound");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_SoundEngine_play2D00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: play3D of class  peak::SoundEngine */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SoundEngine_play3D00
static int tolua_peak_peak_SoundEngine_play3D00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SoundEngine",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isboolean(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SoundEngine* self = (peak::SoundEngine*)  tolua_tousertype(tolua_S,1,0);
  std::string filename = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  bool paused = ((bool)  tolua_toboolean(tolua_S,3,false));
  bool looped = ((bool)  tolua_toboolean(tolua_S,4,false));
  bool cleanup = ((bool)  tolua_toboolean(tolua_S,5,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'play3D'",NULL);
#endif
  {
   peak::Sound* tolua_ret = (peak::Sound*)  self->play3D(filename,paused,looped,cleanup);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Sound");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'play3D'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: play3D of class  peak::SoundEngine */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SoundEngine_play3D01
static int tolua_peak_peak_SoundEngine_play3D01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SoundEngine",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::SoundSource",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isboolean(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::SoundEngine* self = (peak::SoundEngine*)  tolua_tousertype(tolua_S,1,0);
  peak::SoundSource* source = ((peak::SoundSource*)  tolua_tousertype(tolua_S,2,0));
  bool paused = ((bool)  tolua_toboolean(tolua_S,3,false));
  bool looped = ((bool)  tolua_toboolean(tolua_S,4,false));
  bool cleanup = ((bool)  tolua_toboolean(tolua_S,5,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'play3D'",NULL);
#endif
  {
   peak::Sound* tolua_ret = (peak::Sound*)  self->play3D(source,paused,looped,cleanup);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Sound");
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_SoundEngine_play3D00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getListener of class  peak::SoundEngine */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SoundEngine_getListener00
static int tolua_peak_peak_SoundEngine_getListener00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SoundEngine",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SoundEngine* self = (peak::SoundEngine*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getListener'",NULL);
#endif
  {
   peak::Listener* tolua_ret = (peak::Listener*)  self->getListener();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Listener");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getListener'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: doWork of class  peak::SoundEngine */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SoundEngine_doWork00
static int tolua_peak_peak_SoundEngine_doWork00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SoundEngine",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SoundEngine* self = (peak::SoundEngine*)  tolua_tousertype(tolua_S,1,0);
  float msecs = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'doWork'",NULL);
#endif
  {
   self->doWork(msecs);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'doWork'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeSoundFromList of class  peak::SoundEngine */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SoundEngine_removeSoundFromList00
static int tolua_peak_peak_SoundEngine_removeSoundFromList00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SoundEngine",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Sound",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SoundEngine* self = (peak::SoundEngine*)  tolua_tousertype(tolua_S,1,0);
  peak::Sound* sound = ((peak::Sound*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeSoundFromList'",NULL);
#endif
  {
   self->removeSoundFromList(sound);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeSoundFromList'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get of class  peak::SoundFactory */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SoundFactory_get00
static int tolua_peak_peak_SoundFactory_get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::SoundFactory",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::SoundFactory* tolua_ret = (peak::SoundFactory*)  peak::SoundFactory::get();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::SoundFactory");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::SoundFactory */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SoundFactory_delete00
static int tolua_peak_peak_SoundFactory_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SoundFactory",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SoundFactory* self = (peak::SoundFactory*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getFile of class  peak::SoundFactory */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SoundFactory_getFile00
static int tolua_peak_peak_SoundFactory_getFile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SoundFactory",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SoundFactory* self = (peak::SoundFactory*)  tolua_tousertype(tolua_S,1,0);
  std::string filename = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  bool streamed = ((bool)  tolua_toboolean(tolua_S,3,false));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFile'",NULL);
#endif
  {
   peak::SoundSource* tolua_ret = (peak::SoundSource*)  self->getFile(filename,streamed);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::SoundSource");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getHelloWorld of class  peak::SoundFactory */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SoundFactory_getHelloWorld00
static int tolua_peak_peak_SoundFactory_getHelloWorld00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SoundFactory",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SoundFactory* self = (peak::SoundFactory*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getHelloWorld'",NULL);
#endif
  {
   peak::SoundSource* tolua_ret = (peak::SoundSource*)  self->getHelloWorld();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::SoundSource");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getHelloWorld'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeSource of class  peak::SoundFactory */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SoundFactory_removeSource00
static int tolua_peak_peak_SoundFactory_removeSource00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SoundFactory",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::SoundSource",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SoundFactory* self = (peak::SoundFactory*)  tolua_tousertype(tolua_S,1,0);
  peak::SoundSource* source = ((peak::SoundSource*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeSource'",NULL);
#endif
  {
   self->removeSource(source);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeSource'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_peak_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Buffer","peak::Buffer","",tolua_collect_peak__Buffer);
   #else
   tolua_cclass(tolua_S,"Buffer","peak::Buffer","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Buffer");
    tolua_function(tolua_S,"new",tolua_peak_peak_Buffer_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Buffer_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Buffer_new00_local);
    tolua_function(tolua_S,"new",tolua_peak_peak_Buffer_new01);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Buffer_new01_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Buffer_new01_local);
    tolua_function(tolua_S,"new",tolua_peak_peak_Buffer_new02);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Buffer_new02_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Buffer_new02_local);
    tolua_function(tolua_S,"new",tolua_peak_peak_Buffer_new03);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Buffer_new03_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Buffer_new03_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_Buffer_delete00);
    tolua_function(tolua_S,"setSize",tolua_peak_peak_Buffer_setSize00);
    tolua_function(tolua_S,"getSize",tolua_peak_peak_Buffer_getSize00);
    tolua_function(tolua_S,"getDataSize",tolua_peak_peak_Buffer_getDataSize00);
    tolua_function(tolua_S,"getPosition",tolua_peak_peak_Buffer_getPosition00);
    tolua_function(tolua_S,"setPosition",tolua_peak_peak_Buffer_setPosition00);
    tolua_function(tolua_S,"readData",tolua_peak_peak_Buffer_readData00);
    tolua_function(tolua_S,"writeData",tolua_peak_peak_Buffer_writeData00);
    tolua_function(tolua_S,"writeInt",tolua_peak_peak_Buffer_writeInt00);
    tolua_function(tolua_S,"readInt",tolua_peak_peak_Buffer_readInt00);
    tolua_function(tolua_S,"writeFloat",tolua_peak_peak_Buffer_writeFloat00);
    tolua_function(tolua_S,"readFloat",tolua_peak_peak_Buffer_readFloat00);
    tolua_function(tolua_S,"writeString",tolua_peak_peak_Buffer_writeString00);
    tolua_function(tolua_S,"readString",tolua_peak_peak_Buffer_readString00);
    tolua_function(tolua_S,"writeByte",tolua_peak_peak_Buffer_writeByte00);
    tolua_function(tolua_S,"readByte",tolua_peak_peak_Buffer_readByte00);
    tolua_function(tolua_S,"writeWord",tolua_peak_peak_Buffer_writeWord00);
    tolua_function(tolua_S,"readWord",tolua_peak_peak_Buffer_readWord00);
    tolua_function(tolua_S,"writeBool",tolua_peak_peak_Buffer_writeBool00);
    tolua_function(tolua_S,"readBool",tolua_peak_peak_Buffer_readBool00);
    tolua_function(tolua_S,"writeVector2D",tolua_peak_peak_Buffer_writeVector2D00);
    tolua_function(tolua_S,"readVector2D",tolua_peak_peak_Buffer_readVector2D00);
    tolua_function(tolua_S,"writeVector3D",tolua_peak_peak_Buffer_writeVector3D00);
    tolua_function(tolua_S,"readVector3D",tolua_peak_peak_Buffer_readVector3D00);
    tolua_function(tolua_S,"writeQuaternion",tolua_peak_peak_Buffer_writeQuaternion00);
    tolua_function(tolua_S,"readQuaternion",tolua_peak_peak_Buffer_readQuaternion00);
    tolua_function(tolua_S,"getData",tolua_peak_peak_Buffer_getData00);
    tolua_function(tolua_S,"clear",tolua_peak_peak_Buffer_clear00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Address","peak::Address","",tolua_collect_peak__Address);
   #else
   tolua_cclass(tolua_S,"Address","peak::Address","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Address");
    tolua_function(tolua_S,"new",tolua_peak_peak_Address_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Address_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Address_new00_local);
    tolua_function(tolua_S,"new",tolua_peak_peak_Address_new01);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Address_new01_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Address_new01_local);
    tolua_function(tolua_S,"new",tolua_peak_peak_Address_new02);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Address_new02_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Address_new02_local);
    tolua_function(tolua_S,"new",tolua_peak_peak_Address_new03);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Address_new03_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Address_new03_local);
    tolua_function(tolua_S,"new",tolua_peak_peak_Address_new04);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Address_new04_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Address_new04_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_Address_delete00);
    tolua_function(tolua_S,"setAddress",tolua_peak_peak_Address_setAddress00);
    tolua_function(tolua_S,"getAddress",tolua_peak_peak_Address_getAddress00);
    tolua_function(tolua_S,"setAddress",tolua_peak_peak_Address_setAddress01);
    tolua_function(tolua_S,"setAddress",tolua_peak_peak_Address_setAddress02);
    tolua_function(tolua_S,"getAddressString",tolua_peak_peak_Address_getAddressString00);
    tolua_function(tolua_S,"setPort",tolua_peak_peak_Address_setPort00);
    tolua_function(tolua_S,"getPort",tolua_peak_peak_Address_getPort00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Vector3D","peak::Vector3D","",tolua_collect_peak__Vector3D);
   #else
   tolua_cclass(tolua_S,"Vector3D","peak::Vector3D","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Vector3D");
    tolua_function(tolua_S,"new",tolua_peak_peak_Vector3D_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Vector3D_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Vector3D_new00_local);
    tolua_function(tolua_S,"new",tolua_peak_peak_Vector3D_new01);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Vector3D_new01_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Vector3D_new01_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_Vector3D_delete00);
    tolua_function(tolua_S,"rotateXZ",tolua_peak_peak_Vector3D_rotateXZ00);
    tolua_function(tolua_S,"rotateYZ",tolua_peak_peak_Vector3D_rotateYZ00);
    tolua_function(tolua_S,"rotateXY",tolua_peak_peak_Vector3D_rotateXY00);
    tolua_function(tolua_S,"getRotation",tolua_peak_peak_Vector3D_getRotation00);
    tolua_function(tolua_S,"getLengthSq",tolua_peak_peak_Vector3D_getLengthSq00);
    tolua_variable(tolua_S,"x",tolua_get_peak__Vector3D_x,tolua_set_peak__Vector3D_x);
    tolua_variable(tolua_S,"y",tolua_get_peak__Vector3D_y,tolua_set_peak__Vector3D_y);
    tolua_variable(tolua_S,"z",tolua_get_peak__Vector3D_z,tolua_set_peak__Vector3D_z);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Vector2D","peak::Vector2D","",tolua_collect_peak__Vector2D);
   #else
   tolua_cclass(tolua_S,"Vector2D","peak::Vector2D","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Vector2D");
    tolua_function(tolua_S,"new",tolua_peak_peak_Vector2D_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Vector2D_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Vector2D_new00_local);
    tolua_function(tolua_S,"new",tolua_peak_peak_Vector2D_new01);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Vector2D_new01_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Vector2D_new01_local);
    tolua_function(tolua_S,"rotate",tolua_peak_peak_Vector2D_rotate00);
    tolua_variable(tolua_S,"x",tolua_get_peak__Vector2D_x,tolua_set_peak__Vector2D_x);
    tolua_variable(tolua_S,"y",tolua_get_peak__Vector2D_y,tolua_set_peak__Vector2D_y);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Vector2DI","peak::Vector2DI","",tolua_collect_peak__Vector2DI);
   #else
   tolua_cclass(tolua_S,"Vector2DI","peak::Vector2DI","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Vector2DI");
    tolua_function(tolua_S,"new",tolua_peak_peak_Vector2DI_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Vector2DI_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Vector2DI_new00_local);
    tolua_function(tolua_S,"new",tolua_peak_peak_Vector2DI_new01);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Vector2DI_new01_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Vector2DI_new01_local);
    tolua_variable(tolua_S,"x",tolua_get_peak__Vector2DI_x,tolua_set_peak__Vector2DI_x);
    tolua_variable(tolua_S,"y",tolua_get_peak__Vector2DI_y,tolua_set_peak__Vector2DI_y);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Color","peak::Color","",tolua_collect_peak__Color);
   #else
   tolua_cclass(tolua_S,"Color","peak::Color","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Color");
    tolua_function(tolua_S,"new",tolua_peak_peak_Color_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Color_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Color_new00_local);
    tolua_function(tolua_S,"new",tolua_peak_peak_Color_new01);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Color_new01_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Color_new01_local);
    tolua_function(tolua_S,"new",tolua_peak_peak_Color_new02);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Color_new02_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Color_new02_local);
    tolua_function(tolua_S,"getRed",tolua_peak_peak_Color_getRed00);
    tolua_function(tolua_S,"getGreen",tolua_peak_peak_Color_getGreen00);
    tolua_function(tolua_S,"getBlue",tolua_peak_peak_Color_getBlue00);
    tolua_function(tolua_S,"getAlpha",tolua_peak_peak_Color_getAlpha00);
    tolua_function(tolua_S,"setRed",tolua_peak_peak_Color_setRed00);
    tolua_function(tolua_S,"setGreen",tolua_peak_peak_Color_setGreen00);
    tolua_function(tolua_S,"setBlue",tolua_peak_peak_Color_setBlue00);
    tolua_function(tolua_S,"setAlpha",tolua_peak_peak_Color_setAlpha00);
    tolua_function(tolua_S,"set",tolua_peak_peak_Color_set00);
    tolua_function(tolua_S,"set",tolua_peak_peak_Color_set01);
    tolua_function(tolua_S,"get",tolua_peak_peak_Color_get00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Quaternion","peak::Quaternion","",tolua_collect_peak__Quaternion);
   #else
   tolua_cclass(tolua_S,"Quaternion","peak::Quaternion","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Quaternion");
    tolua_function(tolua_S,"new",tolua_peak_peak_Quaternion_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Quaternion_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Quaternion_new00_local);
    tolua_function(tolua_S,"new",tolua_peak_peak_Quaternion_new01);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Quaternion_new01_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Quaternion_new01_local);
    tolua_function(tolua_S,"new",tolua_peak_peak_Quaternion_new02);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Quaternion_new02_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Quaternion_new02_local);
    tolua_function(tolua_S,"new",tolua_peak_peak_Quaternion_new03);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Quaternion_new03_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Quaternion_new03_local);
    tolua_function(tolua_S,"new",tolua_peak_peak_Quaternion_new04);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Quaternion_new04_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Quaternion_new04_local);
    tolua_function(tolua_S,"new",tolua_peak_peak_Quaternion_new05);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Quaternion_new05_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Quaternion_new05_local);
    tolua_function(tolua_S,"fromAngleAxis",tolua_peak_peak_Quaternion_fromAngleAxis00);
    tolua_function(tolua_S,"toAngleAxis",tolua_peak_peak_Quaternion_toAngleAxis00);
    tolua_function(tolua_S,"getYaw",tolua_peak_peak_Quaternion_getYaw00);
    tolua_function(tolua_S,"getEuler",tolua_peak_peak_Quaternion_getEuler00);
    tolua_function(tolua_S,"rotateVector",tolua_peak_peak_Quaternion_rotateVector00);
    tolua_function(tolua_S,"slerp",tolua_peak_peak_Quaternion_slerp00);
    tolua_function(tolua_S,"getDifference",tolua_peak_peak_Quaternion_getDifference00);
    tolua_array(tolua_S,"q",tolua_get_peak_peak_Quaternion_q,tolua_set_peak_peak_Quaternion_q);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Parameter","peak::Parameter","",tolua_collect_peak__Parameter);
   #else
   tolua_cclass(tolua_S,"Parameter","peak::Parameter","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Parameter");
    tolua_function(tolua_S,"new",tolua_peak_peak_Parameter_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Parameter_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Parameter_new00_local);
    tolua_function(tolua_S,"new",tolua_peak_peak_Parameter_new01);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Parameter_new01_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Parameter_new01_local);
    tolua_function(tolua_S,"isPointer",tolua_peak_peak_Parameter_isPointer00);
    tolua_function(tolua_S,"getValue",tolua_peak_peak_Parameter_getValue00);
    tolua_function(tolua_S,"getClassName",tolua_peak_peak_Parameter_getClassName00);
    tolua_function(tolua_S,"getPointer",tolua_peak_peak_Parameter_getPointer00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"ParameterList","peak::ParameterList","",tolua_collect_peak__ParameterList);
   #else
   tolua_cclass(tolua_S,"ParameterList","peak::ParameterList","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"ParameterList");
    tolua_function(tolua_S,"new",tolua_peak_peak_ParameterList_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_ParameterList_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_ParameterList_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_ParameterList_delete00);
    tolua_function(tolua_S,"add",tolua_peak_peak_ParameterList_add00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Script","peak::Script","",tolua_collect_peak__Script);
   #else
   tolua_cclass(tolua_S,"Script","peak::Script","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Script");
    tolua_function(tolua_S,"new",tolua_peak_peak_Script_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Script_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Script_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_Script_delete00);
    tolua_function(tolua_S,"run",tolua_peak_peak_Script_run00);
    tolua_function(tolua_S,"callFunction",tolua_peak_peak_Script_callFunction00);
    tolua_function(tolua_S,"callFunction",tolua_peak_peak_Script_callFunction01);
    tolua_function(tolua_S,"callFunctionI",tolua_peak_peak_Script_callFunctionI00);
    tolua_function(tolua_S,"callFunctionII",tolua_peak_peak_Script_callFunctionII00);
    tolua_function(tolua_S,"callFunctionIII",tolua_peak_peak_Script_callFunctionIII00);
    tolua_function(tolua_S,"callFunctionFFF",tolua_peak_peak_Script_callFunctionFFF00);
    tolua_function(tolua_S,"callIntFunction",tolua_peak_peak_Script_callIntFunction00);
    tolua_function(tolua_S,"callFloatFunction",tolua_peak_peak_Script_callFloatFunction00);
    tolua_function(tolua_S,"isFunction",tolua_peak_peak_Script_isFunction00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"World","peak::World","",tolua_collect_peak__World);
   #else
   tolua_cclass(tolua_S,"World","peak::World","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"World");
    tolua_function(tolua_S,"new",tolua_peak_peak_World_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_World_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_World_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_World_delete00);
    tolua_function(tolua_S,"init",tolua_peak_peak_World_init00);
    tolua_function(tolua_S,"destroy",tolua_peak_peak_World_destroy00);
    tolua_function(tolua_S,"collide",tolua_peak_peak_World_collide00);
    tolua_function(tolua_S,"getContactPoints",tolua_peak_peak_World_getContactPoints00);
    tolua_function(tolua_S,"getContactPoint",tolua_peak_peak_World_getContactPoint00);
    tolua_function(tolua_S,"getID",tolua_peak_peak_World_getID00);
    tolua_function(tolua_S,"doWork",tolua_peak_peak_World_doWork00);
    tolua_function(tolua_S,"doAllWork",tolua_peak_peak_World_doAllWork00);
    tolua_function(tolua_S,"getWorld",tolua_peak_peak_World_getWorld00);
    tolua_function(tolua_S,"getSpace",tolua_peak_peak_World_getSpace00);
    tolua_function(tolua_S,"getContactGroup",tolua_peak_peak_World_getContactGroup00);
    tolua_function(tolua_S,"getWorld",tolua_peak_peak_World_getWorld01);
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"NetworkHost","peak::NetworkHost","",NULL);
   tolua_beginmodule(tolua_S,"NetworkHost");
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"NetworkClient","peak::NetworkClient","",NULL);
   tolua_beginmodule(tolua_S,"NetworkClient");
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Entity","peak::Entity","",tolua_collect_peak__Entity);
   #else
   tolua_cclass(tolua_S,"Entity","peak::Entity","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Entity");
    tolua_function(tolua_S,"new",tolua_peak_peak_Entity_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Entity_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Entity_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_Entity_delete00);
    tolua_function(tolua_S,"load",tolua_peak_peak_Entity_load00);
    tolua_function(tolua_S,"destroy",tolua_peak_peak_Entity_destroy00);
    tolua_function(tolua_S,"getName",tolua_peak_peak_Entity_getName00);
    tolua_function(tolua_S,"setID",tolua_peak_peak_Entity_setID00);
    tolua_function(tolua_S,"getID",tolua_peak_peak_Entity_getID00);
    tolua_function(tolua_S,"setLocal",tolua_peak_peak_Entity_setLocal00);
    tolua_function(tolua_S,"isLocal",tolua_peak_peak_Entity_isLocal00);
    tolua_function(tolua_S,"setOwner",tolua_peak_peak_Entity_setOwner00);
    tolua_function(tolua_S,"getOwner",tolua_peak_peak_Entity_getOwner00);
    tolua_function(tolua_S,"setPriority",tolua_peak_peak_Entity_setPriority00);
    tolua_function(tolua_S,"getPriority",tolua_peak_peak_Entity_getPriority00);
    tolua_function(tolua_S,"setCurrentPriority",tolua_peak_peak_Entity_setCurrentPriority00);
    tolua_function(tolua_S,"getCurrentPriority",tolua_peak_peak_Entity_getCurrentPriority00);
    tolua_function(tolua_S,"needsUpdate",tolua_peak_peak_Entity_needsUpdate00);
    tolua_function(tolua_S,"getUpdateSize",tolua_peak_peak_Entity_getUpdateSize00);
    tolua_function(tolua_S,"writeUpdateData",tolua_peak_peak_Entity_writeUpdateData00);
    tolua_function(tolua_S,"updateFromData",tolua_peak_peak_Entity_updateFromData00);
    tolua_function(tolua_S,"writeCompleteData",tolua_peak_peak_Entity_writeCompleteData00);
    tolua_function(tolua_S,"readCompleteData",tolua_peak_peak_Entity_readCompleteData00);
    tolua_function(tolua_S,"addComponent",tolua_peak_peak_Entity_addComponent00);
    tolua_function(tolua_S,"deleteComponent",tolua_peak_peak_Entity_deleteComponent00);
    tolua_function(tolua_S,"getComponent",tolua_peak_peak_Entity_getComponent00);
    tolua_function(tolua_S,"getPhysicsComponent",tolua_peak_peak_Entity_getPhysicsComponent00);
    tolua_function(tolua_S,"getGraphicsComponent",tolua_peak_peak_Entity_getGraphicsComponent00);
    tolua_function(tolua_S,"getServerScriptComponent",tolua_peak_peak_Entity_getServerScriptComponent00);
    tolua_function(tolua_S,"getVariables",tolua_peak_peak_Entity_getVariables00);
    tolua_function(tolua_S,"doWork",tolua_peak_peak_Entity_doWork00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Connection","peak::Connection","",tolua_collect_peak__Connection);
   #else
   tolua_cclass(tolua_S,"Connection","peak::Connection","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Connection");
    tolua_function(tolua_S,"new",tolua_peak_peak_Connection_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Connection_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Connection_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_Connection_delete00);
    tolua_function(tolua_S,"destroy",tolua_peak_peak_Connection_destroy00);
    tolua_function(tolua_S,"disconnect",tolua_peak_peak_Connection_disconnect00);
    tolua_function(tolua_S,"isConnected",tolua_peak_peak_Connection_isConnected00);
    tolua_function(tolua_S,"hasNewData",tolua_peak_peak_Connection_hasNewData00);
    tolua_function(tolua_S,"readData",tolua_peak_peak_Connection_readData00);
    tolua_function(tolua_S,"sendData",tolua_peak_peak_Connection_sendData00);
    tolua_function(tolua_S,"injectData",tolua_peak_peak_Connection_injectData00);
    tolua_function(tolua_S,"getPeer",tolua_peak_peak_Connection_getPeer00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"BroadcastHost","peak::BroadcastHost","",tolua_collect_peak__BroadcastHost);
   #else
   tolua_cclass(tolua_S,"BroadcastHost","peak::BroadcastHost","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"BroadcastHost");
    tolua_function(tolua_S,"new",tolua_peak_peak_BroadcastHost_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_BroadcastHost_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_BroadcastHost_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_BroadcastHost_delete00);
    tolua_function(tolua_S,"init",tolua_peak_peak_BroadcastHost_init00);
    tolua_function(tolua_S,"shutdown",tolua_peak_peak_BroadcastHost_shutdown00);
    tolua_function(tolua_S,"setInfo",tolua_peak_peak_BroadcastHost_setInfo00);
    tolua_function(tolua_S,"getInfo",tolua_peak_peak_BroadcastHost_getInfo00);
    tolua_function(tolua_S,"doWork",tolua_peak_peak_BroadcastHost_doWork00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Game","peak::Game","",tolua_collect_peak__Game);
   #else
   tolua_cclass(tolua_S,"Game","peak::Game","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Game");
    tolua_function(tolua_S,"get",tolua_peak_peak_Game_get00);
    tolua_function(tolua_S,"delete",tolua_peak_peak_Game_delete00);
    tolua_function(tolua_S,"init",tolua_peak_peak_Game_init00);
    tolua_function(tolua_S,"init",tolua_peak_peak_Game_init01);
    tolua_function(tolua_S,"shutdown",tolua_peak_peak_Game_shutdown00);
    tolua_function(tolua_S,"stopGame",tolua_peak_peak_Game_stopGame00);
    tolua_function(tolua_S,"isServer",tolua_peak_peak_Game_isServer00);
    tolua_function(tolua_S,"isClient",tolua_peak_peak_Game_isClient00);
    tolua_function(tolua_S,"setVisible",tolua_peak_peak_Game_setVisible00);
    tolua_function(tolua_S,"isVisible",tolua_peak_peak_Game_isVisible00);
    tolua_function(tolua_S,"createEntity",tolua_peak_peak_Game_createEntity00);
    tolua_function(tolua_S,"getEntity",tolua_peak_peak_Game_getEntity00);
    tolua_function(tolua_S,"deleteEntity",tolua_peak_peak_Game_deleteEntity00);
    tolua_function(tolua_S,"getEntityCount",tolua_peak_peak_Game_getEntityCount00);
    tolua_function(tolua_S,"getEntity",tolua_peak_peak_Game_getEntity01);
    tolua_function(tolua_S,"getWorld",tolua_peak_peak_Game_getWorld00);
    tolua_function(tolua_S,"injectAction",tolua_peak_peak_Game_injectAction00);
    tolua_function(tolua_S,"injectMouseMovement",tolua_peak_peak_Game_injectMouseMovement00);
    tolua_function(tolua_S,"registerEntityActionInput",tolua_peak_peak_Game_registerEntityActionInput00);
    tolua_function(tolua_S,"registerEntityMouseInput",tolua_peak_peak_Game_registerEntityMouseInput00);
    tolua_function(tolua_S,"getRegisteredEntity",tolua_peak_peak_Game_getRegisteredEntity00);
    tolua_function(tolua_S,"doWork",tolua_peak_peak_Game_doWork00);
    tolua_function(tolua_S,"getServerScript",tolua_peak_peak_Game_getServerScript00);
    tolua_function(tolua_S,"getClientScript",tolua_peak_peak_Game_getClientScript00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   tolua_constant(tolua_S,"ELL_Nothing",peak::ELL_Nothing);
   tolua_constant(tolua_S,"ELL_Error",peak::ELL_Error);
   tolua_constant(tolua_S,"ELL_Warning",peak::ELL_Warning);
   tolua_constant(tolua_S,"ELL_Information",peak::ELL_Information);
   tolua_constant(tolua_S,"ELL_Debug",peak::ELL_Debug);
   tolua_constant(tolua_S,"EPT_None",peak::EPT_None);
   tolua_constant(tolua_S,"EPT_Debug",peak::EPT_Debug);
   tolua_constant(tolua_S,"EPT_Information",peak::EPT_Information);
   tolua_constant(tolua_S,"EPT_Warning",peak::EPT_Warning);
   tolua_constant(tolua_S,"EPT_Error",peak::EPT_Error);
   tolua_constant(tolua_S,"EPT_All",peak::EPT_All);
   tolua_constant(tolua_S,"EPT_BeforeTimestamp",peak::EPT_BeforeTimestamp);
   tolua_constant(tolua_S,"EPT_Abbreviations",peak::EPT_Abbreviations);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Logger","peak::Logger","",tolua_collect_peak__Logger);
   #else
   tolua_cclass(tolua_S,"Logger","peak::Logger","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Logger");
    tolua_function(tolua_S,"get",tolua_peak_peak_Logger_get00);
    tolua_function(tolua_S,"delete",tolua_peak_peak_Logger_delete00);
    tolua_function(tolua_S,"log",tolua_peak_peak_Logger_log00);
    tolua_function(tolua_S,"setFile",tolua_peak_peak_Logger_setFile00);
    tolua_function(tolua_S,"getFile",tolua_peak_peak_Logger_getFile00);
    tolua_function(tolua_S,"clearFile",tolua_peak_peak_Logger_clearFile00);
    tolua_function(tolua_S,"setTimestamps",tolua_peak_peak_Logger_setTimestamps00);
    tolua_function(tolua_S,"setConsoleTimestamps",tolua_peak_peak_Logger_setConsoleTimestamps00);
    tolua_function(tolua_S,"getConsoleTimestamps",tolua_peak_peak_Logger_getConsoleTimestamps00);
    tolua_function(tolua_S,"setFileTimestamps",tolua_peak_peak_Logger_setFileTimestamps00);
    tolua_function(tolua_S,"getFileTimestamps",tolua_peak_peak_Logger_getFileTimestamps00);
    tolua_function(tolua_S,"setPrefix",tolua_peak_peak_Logger_setPrefix00);
    tolua_function(tolua_S,"setConsolePrefix",tolua_peak_peak_Logger_setConsolePrefix00);
    tolua_function(tolua_S,"getConsolePrefix",tolua_peak_peak_Logger_getConsolePrefix00);
    tolua_function(tolua_S,"setFilePrefix",tolua_peak_peak_Logger_setFilePrefix00);
    tolua_function(tolua_S,"getFilePrefix",tolua_peak_peak_Logger_getFilePrefix00);
    tolua_function(tolua_S,"setMinimalConsoleLevel",tolua_peak_peak_Logger_setMinimalConsoleLevel00);
    tolua_function(tolua_S,"getMinimalConsoleLevel",tolua_peak_peak_Logger_getMinimalConsoleLevel00);
    tolua_function(tolua_S,"setMinimalFileLevel",tolua_peak_peak_Logger_setMinimalFileLevel00);
    tolua_function(tolua_S,"getMinimalFileLevel",tolua_peak_peak_Logger_getMinimalFileLevel00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   tolua_cclass(tolua_S,"InputManager","peak::InputManager","",NULL);
   tolua_beginmodule(tolua_S,"InputManager");
    tolua_function(tolua_S,"get",tolua_peak_peak_InputManager_get00);
    tolua_function(tolua_S,"init",tolua_peak_peak_InputManager_init00);
    tolua_function(tolua_S,"shutdown",tolua_peak_peak_InputManager_shutdown00);
    tolua_function(tolua_S,"keyboardEvent",tolua_peak_peak_InputManager_keyboardEvent00);
    tolua_function(tolua_S,"mouseMoved",tolua_peak_peak_InputManager_mouseMoved00);
    tolua_function(tolua_S,"setInputEnabled",tolua_peak_peak_InputManager_setInputEnabled00);
    tolua_function(tolua_S,"getInputEnabled",tolua_peak_peak_InputManager_getInputEnabled00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   tolua_constant(tolua_S,"EK_NONE",peak::EK_NONE);
   tolua_constant(tolua_S,"EK_LBUTTON",peak::EK_LBUTTON);
   tolua_constant(tolua_S,"EK_RBUTTON",peak::EK_RBUTTON);
   tolua_constant(tolua_S,"EK_CANCEL",peak::EK_CANCEL);
   tolua_constant(tolua_S,"EK_MBUTTON",peak::EK_MBUTTON);
   tolua_constant(tolua_S,"EK_XBUTTON1",peak::EK_XBUTTON1);
   tolua_constant(tolua_S,"EK_XBUTTON2",peak::EK_XBUTTON2);
   tolua_constant(tolua_S,"EK_BACK",peak::EK_BACK);
   tolua_constant(tolua_S,"EK_TAB",peak::EK_TAB);
   tolua_constant(tolua_S,"EK_CLEAR",peak::EK_CLEAR);
   tolua_constant(tolua_S,"EK_RETURN",peak::EK_RETURN);
   tolua_constant(tolua_S,"EK_SHIFT",peak::EK_SHIFT);
   tolua_constant(tolua_S,"EK_CONTROL",peak::EK_CONTROL);
   tolua_constant(tolua_S,"EK_MENU",peak::EK_MENU);
   tolua_constant(tolua_S,"EK_PAUSE",peak::EK_PAUSE);
   tolua_constant(tolua_S,"EK_CAPITAL",peak::EK_CAPITAL);
   tolua_constant(tolua_S,"EK_KANA",peak::EK_KANA);
   tolua_constant(tolua_S,"EK_HANGUEL",peak::EK_HANGUEL);
   tolua_constant(tolua_S,"EK_HANGUL",peak::EK_HANGUL);
   tolua_constant(tolua_S,"EK_JUNJA",peak::EK_JUNJA);
   tolua_constant(tolua_S,"EK_FINAL",peak::EK_FINAL);
   tolua_constant(tolua_S,"EK_HANJA",peak::EK_HANJA);
   tolua_constant(tolua_S,"EK_KANJI",peak::EK_KANJI);
   tolua_constant(tolua_S,"EK_ESCAPE",peak::EK_ESCAPE);
   tolua_constant(tolua_S,"EK_CONVERT",peak::EK_CONVERT);
   tolua_constant(tolua_S,"EK_NONCONVERT",peak::EK_NONCONVERT);
   tolua_constant(tolua_S,"EK_ACCEPT",peak::EK_ACCEPT);
   tolua_constant(tolua_S,"EK_MODECHANGE",peak::EK_MODECHANGE);
   tolua_constant(tolua_S,"EK_SPACE",peak::EK_SPACE);
   tolua_constant(tolua_S,"EK_PRIOR",peak::EK_PRIOR);
   tolua_constant(tolua_S,"EK_NEXT",peak::EK_NEXT);
   tolua_constant(tolua_S,"EK_END",peak::EK_END);
   tolua_constant(tolua_S,"EK_HOME",peak::EK_HOME);
   tolua_constant(tolua_S,"EK_LEFT",peak::EK_LEFT);
   tolua_constant(tolua_S,"EK_UP",peak::EK_UP);
   tolua_constant(tolua_S,"EK_RIGHT",peak::EK_RIGHT);
   tolua_constant(tolua_S,"EK_DOWN",peak::EK_DOWN);
   tolua_constant(tolua_S,"EK_SELECT",peak::EK_SELECT);
   tolua_constant(tolua_S,"EK_PRINT",peak::EK_PRINT);
   tolua_constant(tolua_S,"EK_EXECUT",peak::EK_EXECUT);
   tolua_constant(tolua_S,"EK_SNAPSHOT",peak::EK_SNAPSHOT);
   tolua_constant(tolua_S,"EK_INSERT",peak::EK_INSERT);
   tolua_constant(tolua_S,"EK_DELETE",peak::EK_DELETE);
   tolua_constant(tolua_S,"EK_HELP",peak::EK_HELP);
   tolua_constant(tolua_S,"EK_KEY_0",peak::EK_KEY_0);
   tolua_constant(tolua_S,"EK_KEY_1",peak::EK_KEY_1);
   tolua_constant(tolua_S,"EK_KEY_2",peak::EK_KEY_2);
   tolua_constant(tolua_S,"EK_KEY_3",peak::EK_KEY_3);
   tolua_constant(tolua_S,"EK_KEY_4",peak::EK_KEY_4);
   tolua_constant(tolua_S,"EK_KEY_5",peak::EK_KEY_5);
   tolua_constant(tolua_S,"EK_KEY_6",peak::EK_KEY_6);
   tolua_constant(tolua_S,"EK_KEY_7",peak::EK_KEY_7);
   tolua_constant(tolua_S,"EK_KEY_8",peak::EK_KEY_8);
   tolua_constant(tolua_S,"EK_KEY_9",peak::EK_KEY_9);
   tolua_constant(tolua_S,"EK_KEY_A",peak::EK_KEY_A);
   tolua_constant(tolua_S,"EK_KEY_B",peak::EK_KEY_B);
   tolua_constant(tolua_S,"EK_KEY_C",peak::EK_KEY_C);
   tolua_constant(tolua_S,"EK_KEY_D",peak::EK_KEY_D);
   tolua_constant(tolua_S,"EK_KEY_E",peak::EK_KEY_E);
   tolua_constant(tolua_S,"EK_KEY_F",peak::EK_KEY_F);
   tolua_constant(tolua_S,"EK_KEY_G",peak::EK_KEY_G);
   tolua_constant(tolua_S,"EK_KEY_H",peak::EK_KEY_H);
   tolua_constant(tolua_S,"EK_KEY_I",peak::EK_KEY_I);
   tolua_constant(tolua_S,"EK_KEY_J",peak::EK_KEY_J);
   tolua_constant(tolua_S,"EK_KEY_K",peak::EK_KEY_K);
   tolua_constant(tolua_S,"EK_KEY_L",peak::EK_KEY_L);
   tolua_constant(tolua_S,"EK_KEY_M",peak::EK_KEY_M);
   tolua_constant(tolua_S,"EK_KEY_N",peak::EK_KEY_N);
   tolua_constant(tolua_S,"EK_KEY_O",peak::EK_KEY_O);
   tolua_constant(tolua_S,"EK_KEY_P",peak::EK_KEY_P);
   tolua_constant(tolua_S,"EK_KEY_Q",peak::EK_KEY_Q);
   tolua_constant(tolua_S,"EK_KEY_R",peak::EK_KEY_R);
   tolua_constant(tolua_S,"EK_KEY_S",peak::EK_KEY_S);
   tolua_constant(tolua_S,"EK_KEY_T",peak::EK_KEY_T);
   tolua_constant(tolua_S,"EK_KEY_U",peak::EK_KEY_U);
   tolua_constant(tolua_S,"EK_KEY_V",peak::EK_KEY_V);
   tolua_constant(tolua_S,"EK_KEY_W",peak::EK_KEY_W);
   tolua_constant(tolua_S,"EK_KEY_X",peak::EK_KEY_X);
   tolua_constant(tolua_S,"EK_KEY_Y",peak::EK_KEY_Y);
   tolua_constant(tolua_S,"EK_KEY_Z",peak::EK_KEY_Z);
   tolua_constant(tolua_S,"EK_LWIN",peak::EK_LWIN);
   tolua_constant(tolua_S,"EK_RWIN",peak::EK_RWIN);
   tolua_constant(tolua_S,"EK_APPS",peak::EK_APPS);
   tolua_constant(tolua_S,"EK_SLEEP",peak::EK_SLEEP);
   tolua_constant(tolua_S,"EK_NUMPAD0",peak::EK_NUMPAD0);
   tolua_constant(tolua_S,"EK_NUMPAD1",peak::EK_NUMPAD1);
   tolua_constant(tolua_S,"EK_NUMPAD2",peak::EK_NUMPAD2);
   tolua_constant(tolua_S,"EK_NUMPAD3",peak::EK_NUMPAD3);
   tolua_constant(tolua_S,"EK_NUMPAD4",peak::EK_NUMPAD4);
   tolua_constant(tolua_S,"EK_NUMPAD5",peak::EK_NUMPAD5);
   tolua_constant(tolua_S,"EK_NUMPAD6",peak::EK_NUMPAD6);
   tolua_constant(tolua_S,"EK_NUMPAD7",peak::EK_NUMPAD7);
   tolua_constant(tolua_S,"EK_NUMPAD8",peak::EK_NUMPAD8);
   tolua_constant(tolua_S,"EK_NUMPAD9",peak::EK_NUMPAD9);
   tolua_constant(tolua_S,"EK_MULTIPLY",peak::EK_MULTIPLY);
   tolua_constant(tolua_S,"EK_ADD",peak::EK_ADD);
   tolua_constant(tolua_S,"EK_SEPARATOR",peak::EK_SEPARATOR);
   tolua_constant(tolua_S,"EK_SUBTRACT",peak::EK_SUBTRACT);
   tolua_constant(tolua_S,"EK_DECIMAL",peak::EK_DECIMAL);
   tolua_constant(tolua_S,"EK_DIVIDE",peak::EK_DIVIDE);
   tolua_constant(tolua_S,"EK_F1",peak::EK_F1);
   tolua_constant(tolua_S,"EK_F2",peak::EK_F2);
   tolua_constant(tolua_S,"EK_F3",peak::EK_F3);
   tolua_constant(tolua_S,"EK_F4",peak::EK_F4);
   tolua_constant(tolua_S,"EK_F5",peak::EK_F5);
   tolua_constant(tolua_S,"EK_F6",peak::EK_F6);
   tolua_constant(tolua_S,"EK_F7",peak::EK_F7);
   tolua_constant(tolua_S,"EK_F8",peak::EK_F8);
   tolua_constant(tolua_S,"EK_F9",peak::EK_F9);
   tolua_constant(tolua_S,"EK_F10",peak::EK_F10);
   tolua_constant(tolua_S,"EK_F11",peak::EK_F11);
   tolua_constant(tolua_S,"EK_F12",peak::EK_F12);
   tolua_constant(tolua_S,"EK_F13",peak::EK_F13);
   tolua_constant(tolua_S,"EK_F14",peak::EK_F14);
   tolua_constant(tolua_S,"EK_F15",peak::EK_F15);
   tolua_constant(tolua_S,"EK_F16",peak::EK_F16);
   tolua_constant(tolua_S,"EK_F17",peak::EK_F17);
   tolua_constant(tolua_S,"EK_F18",peak::EK_F18);
   tolua_constant(tolua_S,"EK_F19",peak::EK_F19);
   tolua_constant(tolua_S,"EK_F20",peak::EK_F20);
   tolua_constant(tolua_S,"EK_F21",peak::EK_F21);
   tolua_constant(tolua_S,"EK_F22",peak::EK_F22);
   tolua_constant(tolua_S,"EK_F23",peak::EK_F23);
   tolua_constant(tolua_S,"EK_F24",peak::EK_F24);
   tolua_constant(tolua_S,"EK_NUMLOCK",peak::EK_NUMLOCK);
   tolua_constant(tolua_S,"EK_SCROLL",peak::EK_SCROLL);
   tolua_constant(tolua_S,"EK_LSHIFT",peak::EK_LSHIFT);
   tolua_constant(tolua_S,"EK_RSHIFT",peak::EK_RSHIFT);
   tolua_constant(tolua_S,"EK_LCONTROL",peak::EK_LCONTROL);
   tolua_constant(tolua_S,"EK_RCONTROL",peak::EK_RCONTROL);
   tolua_constant(tolua_S,"EK_LMENU",peak::EK_LMENU);
   tolua_constant(tolua_S,"EK_RMENU",peak::EK_RMENU);
   tolua_constant(tolua_S,"EK_PLUS",peak::EK_PLUS);
   tolua_constant(tolua_S,"EK_COMMA",peak::EK_COMMA);
   tolua_constant(tolua_S,"EK_MINUS",peak::EK_MINUS);
   tolua_constant(tolua_S,"EK_PERIOD",peak::EK_PERIOD);
   tolua_constant(tolua_S,"EK_ATTN",peak::EK_ATTN);
   tolua_constant(tolua_S,"EK_CRSEL",peak::EK_CRSEL);
   tolua_constant(tolua_S,"EK_EXSEL",peak::EK_EXSEL);
   tolua_constant(tolua_S,"EK_EREOF",peak::EK_EREOF);
   tolua_constant(tolua_S,"EK_PLAY",peak::EK_PLAY);
   tolua_constant(tolua_S,"EK_ZOOM",peak::EK_ZOOM);
   tolua_constant(tolua_S,"EK_PA1",peak::EK_PA1);
   tolua_constant(tolua_S,"EK_OEM_CLEAR",peak::EK_OEM_CLEAR);
   tolua_constant(tolua_S,"EK_COUNT",peak::EK_COUNT);
   tolua_function(tolua_S,"getKey",tolua_peak_peak_getKey00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"File","peak::File","",tolua_collect_peak__File);
   #else
   tolua_cclass(tolua_S,"File","peak::File","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"File");
    tolua_function(tolua_S,"new",tolua_peak_peak_File_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_File_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_File_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_File_delete00);
    tolua_function(tolua_S,"read",tolua_peak_peak_File_read00);
    tolua_function(tolua_S,"read",tolua_peak_peak_File_read01);
    tolua_function(tolua_S,"readLine",tolua_peak_peak_File_readLine00);
    tolua_function(tolua_S,"write",tolua_peak_peak_File_write00);
    tolua_function(tolua_S,"write",tolua_peak_peak_File_write01);
    tolua_function(tolua_S,"eof",tolua_peak_peak_File_eof00);
    tolua_function(tolua_S,"getPosition",tolua_peak_peak_File_getPosition00);
    tolua_function(tolua_S,"setPosition",tolua_peak_peak_File_setPosition00);
    tolua_function(tolua_S,"movePosition",tolua_peak_peak_File_movePosition00);
   tolua_endmodule(tolua_S);
   tolua_constant(tolua_S,"EFM_Read",peak::EFM_Read);
   tolua_constant(tolua_S,"EFM_Write",peak::EFM_Write);
   tolua_constant(tolua_S,"EFM_ReadWrite",peak::EFM_ReadWrite);
   tolua_constant(tolua_S,"EFM_Append",peak::EFM_Append);
   tolua_constant(tolua_S,"EFM_ReadAppend",peak::EFM_ReadAppend);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"FileSystem","peak::FileSystem","",tolua_collect_peak__FileSystem);
   #else
   tolua_cclass(tolua_S,"FileSystem","peak::FileSystem","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"FileSystem");
    tolua_function(tolua_S,"get",tolua_peak_peak_FileSystem_get00);
    tolua_function(tolua_S,"delete",tolua_peak_peak_FileSystem_delete00);
    tolua_function(tolua_S,"init",tolua_peak_peak_FileSystem_init00);
    tolua_function(tolua_S,"shutdown",tolua_peak_peak_FileSystem_shutdown00);
    tolua_function(tolua_S,"setRoot",tolua_peak_peak_FileSystem_setRoot00);
    tolua_function(tolua_S,"addOverlay",tolua_peak_peak_FileSystem_addOverlay00);
    tolua_function(tolua_S,"openFile",tolua_peak_peak_FileSystem_openFile00);
    tolua_function(tolua_S,"getFileExists",tolua_peak_peak_FileSystem_getFileExists00);
    tolua_function(tolua_S,"listFiles",tolua_peak_peak_FileSystem_listFiles00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Timer","peak::Timer","",tolua_collect_peak__Timer);
   #else
   tolua_cclass(tolua_S,"Timer","peak::Timer","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Timer");
    tolua_function(tolua_S,"new",tolua_peak_peak_Timer_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Timer_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Timer_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_Timer_delete00);
    tolua_function(tolua_S,"start",tolua_peak_peak_Timer_start00);
    tolua_function(tolua_S,"stop",tolua_peak_peak_Timer_stop00);
    tolua_function(tolua_S,"reset",tolua_peak_peak_Timer_reset00);
    tolua_function(tolua_S,"setTime",tolua_peak_peak_Timer_setTime00);
    tolua_function(tolua_S,"getTime",tolua_peak_peak_Timer_getTime00);
    tolua_function(tolua_S,"addCallback",tolua_peak_peak_Timer_addCallback00);
    tolua_function(tolua_S,"doAllWork",tolua_peak_peak_Timer_doAllWork00);
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"TimerCallback","peak::TimerCallback","",NULL);
   tolua_beginmodule(tolua_S,"TimerCallback");
    tolua_variable(tolua_S,"timer",tolua_get_peak__TimerCallback_timer_ptr,tolua_set_peak__TimerCallback_timer_ptr);
    tolua_variable(tolua_S,"time",tolua_get_peak__TimerCallback_time,tolua_set_peak__TimerCallback_time);
    tolua_variable(tolua_S,"script",tolua_get_peak__TimerCallback_script_ptr,tolua_set_peak__TimerCallback_script_ptr);
    tolua_variable(tolua_S,"function",tolua_get_peak__TimerCallback_function,tolua_set_peak__TimerCallback_function);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   tolua_function(tolua_S,"trimString",tolua_peak_peak_trimString00);
   tolua_function(tolua_S,"splitString",tolua_peak_peak_splitString00);
   tolua_function(tolua_S,"splitString",tolua_peak_peak_splitString01);
   tolua_function(tolua_S,"toFloat",tolua_peak_peak_toFloat00);
   tolua_function(tolua_S,"toInt",tolua_peak_peak_toInt00);
   tolua_function(tolua_S,"toVector3D",tolua_peak_peak_toVector3D00);
   tolua_function(tolua_S,"toVector2D",tolua_peak_peak_toVector2D00);
   tolua_function(tolua_S,"toVector2DI",tolua_peak_peak_toVector2DI00);
   tolua_function(tolua_S,"toBool",tolua_peak_peak_toBool00);
   tolua_function(tolua_S,"toString",tolua_peak_peak_toString00);
   tolua_function(tolua_S,"toString",tolua_peak_peak_toString01);
   tolua_function(tolua_S,"toString",tolua_peak_peak_toString02);
   tolua_function(tolua_S,"toString",tolua_peak_peak_toString03);
   tolua_function(tolua_S,"toString",tolua_peak_peak_toString04);
   tolua_function(tolua_S,"toWString",tolua_peak_peak_toWString00);
   tolua_function(tolua_S,"trimFilename",tolua_peak_peak_trimFilename00);
   tolua_function(tolua_S,"getExtension",tolua_peak_peak_getExtension00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   tolua_constant(tolua_S,"ELOT_Terrain",peak::ELOT_Terrain);
   tolua_constant(tolua_S,"ELOT_SceneNode",peak::ELOT_SceneNode);
   tolua_constant(tolua_S,"ELOT_PointGroup",peak::ELOT_PointGroup);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"LevelObject","peak::LevelObject","",tolua_collect_peak__LevelObject);
   #else
   tolua_cclass(tolua_S,"LevelObject","peak::LevelObject","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"LevelObject");
    tolua_function(tolua_S,"delete",tolua_peak_peak_LevelObject_delete00);
    tolua_function(tolua_S,"setName",tolua_peak_peak_LevelObject_setName00);
    tolua_function(tolua_S,"getName",tolua_peak_peak_LevelObject_getName00);
    tolua_function(tolua_S,"getType",tolua_peak_peak_LevelObject_getType00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Terrain","peak::Terrain","peak::SceneNode",tolua_collect_peak__Terrain);
   #else
   tolua_cclass(tolua_S,"Terrain","peak::Terrain","peak::SceneNode",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Terrain");
    tolua_function(tolua_S,"new",tolua_peak_peak_Terrain_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Terrain_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Terrain_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_Terrain_delete00);
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"Heightfield","peak::Heightfield","",NULL);
   tolua_beginmodule(tolua_S,"Heightfield");
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"SceneNode","peak::SceneNode","",tolua_collect_peak__SceneNode);
   #else
   tolua_cclass(tolua_S,"SceneNode","peak::SceneNode","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"SceneNode");
    tolua_function(tolua_S,"new",tolua_peak_peak_SceneNode_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_SceneNode_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_SceneNode_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_SceneNode_delete00);
    tolua_function(tolua_S,"setPosition",tolua_peak_peak_SceneNode_setPosition00);
    tolua_function(tolua_S,"setRotation",tolua_peak_peak_SceneNode_setRotation00);
    tolua_function(tolua_S,"setRotation",tolua_peak_peak_SceneNode_setRotation01);
    tolua_function(tolua_S,"setPosition",tolua_peak_peak_SceneNode_setPosition01);
    tolua_function(tolua_S,"setRotation",tolua_peak_peak_SceneNode_setRotation02);
    tolua_function(tolua_S,"setRotation",tolua_peak_peak_SceneNode_setRotation03);
    tolua_function(tolua_S,"getPosition",tolua_peak_peak_SceneNode_getPosition00);
    tolua_function(tolua_S,"getRotationEuler",tolua_peak_peak_SceneNode_getRotationEuler00);
    tolua_function(tolua_S,"getRotation",tolua_peak_peak_SceneNode_getRotation00);
    tolua_function(tolua_S,"setScale",tolua_peak_peak_SceneNode_setScale00);
    tolua_function(tolua_S,"setParent",tolua_peak_peak_SceneNode_setParent00);
    tolua_function(tolua_S,"setVisible",tolua_peak_peak_SceneNode_setVisible00);
    tolua_function(tolua_S,"isVisible",tolua_peak_peak_SceneNode_isVisible00);
    tolua_function(tolua_S,"setMaterial",tolua_peak_peak_SceneNode_setMaterial00);
    tolua_function(tolua_S,"setMaterial",tolua_peak_peak_SceneNode_setMaterial01);
    tolua_function(tolua_S,"getMaterial",tolua_peak_peak_SceneNode_getMaterial00);
    tolua_function(tolua_S,"reloadMaterial",tolua_peak_peak_SceneNode_reloadMaterial00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Geometry","peak::Geometry","",tolua_collect_peak__Geometry);
   #else
   tolua_cclass(tolua_S,"Geometry","peak::Geometry","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Geometry");
    tolua_function(tolua_S,"new",tolua_peak_peak_Geometry_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Geometry_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Geometry_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_Geometry_delete00);
    tolua_function(tolua_S,"destroy",tolua_peak_peak_Geometry_destroy00);
    tolua_function(tolua_S,"setPosition",tolua_peak_peak_Geometry_setPosition00);
    tolua_function(tolua_S,"setBody",tolua_peak_peak_Geometry_setBody00);
    tolua_function(tolua_S,"getBody",tolua_peak_peak_Geometry_getBody00);
    tolua_function(tolua_S,"getCollisionInfo",tolua_peak_peak_Geometry_getCollisionInfo00);
    tolua_function(tolua_S,"getGeom",tolua_peak_peak_Geometry_getGeom00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"TerrainObject","peak::TerrainObject","peak::LevelObject",tolua_collect_peak__TerrainObject);
   #else
   tolua_cclass(tolua_S,"TerrainObject","peak::TerrainObject","peak::LevelObject",NULL);
   #endif
   tolua_beginmodule(tolua_S,"TerrainObject");
    tolua_function(tolua_S,"new",tolua_peak_peak_TerrainObject_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_TerrainObject_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_TerrainObject_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_TerrainObject_delete00);
    tolua_function(tolua_S,"getType",tolua_peak_peak_TerrainObject_getType00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"SceneNodeObject","peak::SceneNodeObject","peak::LevelObject",tolua_collect_peak__SceneNodeObject);
   #else
   tolua_cclass(tolua_S,"SceneNodeObject","peak::SceneNodeObject","peak::LevelObject",NULL);
   #endif
   tolua_beginmodule(tolua_S,"SceneNodeObject");
    tolua_function(tolua_S,"new",tolua_peak_peak_SceneNodeObject_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_SceneNodeObject_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_SceneNodeObject_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_SceneNodeObject_delete00);
    tolua_function(tolua_S,"getType",tolua_peak_peak_SceneNodeObject_getType00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"PointGroupObject","peak::PointGroupObject","peak::LevelObject",tolua_collect_peak__PointGroupObject);
   #else
   tolua_cclass(tolua_S,"PointGroupObject","peak::PointGroupObject","peak::LevelObject",NULL);
   #endif
   tolua_beginmodule(tolua_S,"PointGroupObject");
    tolua_function(tolua_S,"new",tolua_peak_peak_PointGroupObject_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_PointGroupObject_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_PointGroupObject_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_PointGroupObject_delete00);
    tolua_function(tolua_S,"addPoint",tolua_peak_peak_PointGroupObject_addPoint00);
    tolua_function(tolua_S,"getPointCount",tolua_peak_peak_PointGroupObject_getPointCount00);
    tolua_function(tolua_S,"getPoint",tolua_peak_peak_PointGroupObject_getPoint00);
    tolua_function(tolua_S,"deletePoint",tolua_peak_peak_PointGroupObject_deletePoint00);
    tolua_function(tolua_S,"addRotation",tolua_peak_peak_PointGroupObject_addRotation00);
    tolua_function(tolua_S,"getRotationCount",tolua_peak_peak_PointGroupObject_getRotationCount00);
    tolua_function(tolua_S,"getRotation",tolua_peak_peak_PointGroupObject_getRotation00);
    tolua_function(tolua_S,"deleteRotation",tolua_peak_peak_PointGroupObject_deleteRotation00);
    tolua_function(tolua_S,"getType",tolua_peak_peak_PointGroupObject_getType00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Level","peak::Level","",tolua_collect_peak__Level);
   #else
   tolua_cclass(tolua_S,"Level","peak::Level","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Level");
    tolua_function(tolua_S,"get",tolua_peak_peak_Level_get00);
    tolua_function(tolua_S,"delete",tolua_peak_peak_Level_delete00);
    tolua_function(tolua_S,"init",tolua_peak_peak_Level_init00);
    tolua_function(tolua_S,"destroy",tolua_peak_peak_Level_destroy00);
    tolua_function(tolua_S,"getObject",tolua_peak_peak_Level_getObject00);
    tolua_function(tolua_S,"getPointGroup",tolua_peak_peak_Level_getPointGroup00);
    tolua_function(tolua_S,"getLevelCount",tolua_peak_peak_Level_getLevelCount00);
    tolua_function(tolua_S,"getLevelName",tolua_peak_peak_Level_getLevelName00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   tolua_cclass(tolua_S,"SettingsManager","peak::SettingsManager","",NULL);
   tolua_beginmodule(tolua_S,"SettingsManager");
    tolua_function(tolua_S,"get",tolua_peak_peak_SettingsManager_get00);
    tolua_function(tolua_S,"load",tolua_peak_peak_SettingsManager_load00);
    tolua_function(tolua_S,"save",tolua_peak_peak_SettingsManager_save00);
    tolua_function(tolua_S,"getString",tolua_peak_peak_SettingsManager_getString00);
    tolua_function(tolua_S,"getBool",tolua_peak_peak_SettingsManager_getBool00);
    tolua_function(tolua_S,"getInt",tolua_peak_peak_SettingsManager_getInt00);
    tolua_function(tolua_S,"getWString",tolua_peak_peak_SettingsManager_getWString00);
    tolua_function(tolua_S,"getVector2D",tolua_peak_peak_SettingsManager_getVector2D00);
    tolua_function(tolua_S,"getVector2DI",tolua_peak_peak_SettingsManager_getVector2DI00);
    tolua_function(tolua_S,"getVector3D",tolua_peak_peak_SettingsManager_getVector3D00);
    tolua_function(tolua_S,"setString",tolua_peak_peak_SettingsManager_setString00);
    tolua_function(tolua_S,"setBool",tolua_peak_peak_SettingsManager_setBool00);
    tolua_function(tolua_S,"setInt",tolua_peak_peak_SettingsManager_setInt00);
    tolua_function(tolua_S,"setVector2D",tolua_peak_peak_SettingsManager_setVector2D00);
    tolua_function(tolua_S,"setVector2DI",tolua_peak_peak_SettingsManager_setVector2DI00);
    tolua_function(tolua_S,"setVector3D",tolua_peak_peak_SettingsManager_setVector3D00);
    tolua_function(tolua_S,"getItemList",tolua_peak_peak_SettingsManager_getItemList00);
    tolua_function(tolua_S,"deleteSetting",tolua_peak_peak_SettingsManager_deleteSetting00);
    tolua_function(tolua_S,"doesExist",tolua_peak_peak_SettingsManager_doesExist00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"BroadcastClient","peak::BroadcastClient","",tolua_collect_peak__BroadcastClient);
   #else
   tolua_cclass(tolua_S,"BroadcastClient","peak::BroadcastClient","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"BroadcastClient");
    tolua_function(tolua_S,"new",tolua_peak_peak_BroadcastClient_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_BroadcastClient_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_BroadcastClient_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_BroadcastClient_delete00);
    tolua_function(tolua_S,"start",tolua_peak_peak_BroadcastClient_start00);
    tolua_function(tolua_S,"stop",tolua_peak_peak_BroadcastClient_stop00);
    tolua_function(tolua_S,"clearList",tolua_peak_peak_BroadcastClient_clearList00);
    tolua_function(tolua_S,"getServerCount",tolua_peak_peak_BroadcastClient_getServerCount00);
    tolua_function(tolua_S,"getServerInfo",tolua_peak_peak_BroadcastClient_getServerInfo00);
    tolua_function(tolua_S,"getServerAddress",tolua_peak_peak_BroadcastClient_getServerAddress00);
    tolua_function(tolua_S,"setCallback",tolua_peak_peak_BroadcastClient_setCallback00);
    tolua_function(tolua_S,"doWork",tolua_peak_peak_BroadcastClient_doWork00);
    tolua_function(tolua_S,"doAllWork",tolua_peak_peak_BroadcastClient_doAllWork00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"CubeSceneNode","peak::CubeSceneNode","peak::SceneNode",tolua_collect_peak__CubeSceneNode);
   #else
   tolua_cclass(tolua_S,"CubeSceneNode","peak::CubeSceneNode","peak::SceneNode",NULL);
   #endif
   tolua_beginmodule(tolua_S,"CubeSceneNode");
    tolua_function(tolua_S,"new",tolua_peak_peak_CubeSceneNode_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_CubeSceneNode_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_CubeSceneNode_new00_local);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"CameraSceneNode","peak::CameraSceneNode","peak::SceneNode",tolua_collect_peak__CameraSceneNode);
   #else
   tolua_cclass(tolua_S,"CameraSceneNode","peak::CameraSceneNode","peak::SceneNode",NULL);
   #endif
   tolua_beginmodule(tolua_S,"CameraSceneNode");
    tolua_function(tolua_S,"new",tolua_peak_peak_CameraSceneNode_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_CameraSceneNode_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_CameraSceneNode_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_CameraSceneNode_delete00);
    tolua_function(tolua_S,"setNearValue",tolua_peak_peak_CameraSceneNode_setNearValue00);
    tolua_function(tolua_S,"getNearValue",tolua_peak_peak_CameraSceneNode_getNearValue00);
    tolua_function(tolua_S,"setFarValue",tolua_peak_peak_CameraSceneNode_setFarValue00);
    tolua_function(tolua_S,"getFarValue",tolua_peak_peak_CameraSceneNode_getFarValue00);
    tolua_function(tolua_S,"setOrthogonal",tolua_peak_peak_CameraSceneNode_setOrthogonal00);
    tolua_function(tolua_S,"isOrthogonal",tolua_peak_peak_CameraSceneNode_isOrthogonal00);
    tolua_function(tolua_S,"setFOV",tolua_peak_peak_CameraSceneNode_setFOV00);
    tolua_function(tolua_S,"getFOV",tolua_peak_peak_CameraSceneNode_getFOV00);
    tolua_function(tolua_S,"setAspectRatio",tolua_peak_peak_CameraSceneNode_setAspectRatio00);
    tolua_function(tolua_S,"getAspectRatio",tolua_peak_peak_CameraSceneNode_getAspectRatio00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   tolua_cclass(tolua_S,"GraphicsEngine","peak::GraphicsEngine","",NULL);
   tolua_beginmodule(tolua_S,"GraphicsEngine");
    tolua_function(tolua_S,"get",tolua_peak_peak_GraphicsEngine_get00);
    tolua_function(tolua_S,"init",tolua_peak_peak_GraphicsEngine_init00);
    tolua_function(tolua_S,"shutdown",tolua_peak_peak_GraphicsEngine_shutdown00);
    tolua_function(tolua_S,"renderFrame",tolua_peak_peak_GraphicsEngine_renderFrame00);
    tolua_function(tolua_S,"getStandardCamera",tolua_peak_peak_GraphicsEngine_getStandardCamera00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Image","peak::Image","",tolua_collect_peak__Image);
   #else
   tolua_cclass(tolua_S,"Image","peak::Image","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Image");
    tolua_function(tolua_S,"new",tolua_peak_peak_Image_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Image_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Image_new00_local);
    tolua_function(tolua_S,"new",tolua_peak_peak_Image_new01);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Image_new01_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Image_new01_local);
    tolua_function(tolua_S,"getDimension",tolua_peak_peak_Image_getDimension00);
    tolua_function(tolua_S,"load",tolua_peak_peak_Image_load00);
    tolua_function(tolua_S,"getPixel",tolua_peak_peak_Image_getPixel00);
    tolua_function(tolua_S,"getPixel",tolua_peak_peak_Image_getPixel01);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"MeshSceneNode","peak::MeshSceneNode","peak::SceneNode",tolua_collect_peak__MeshSceneNode);
   #else
   tolua_cclass(tolua_S,"MeshSceneNode","peak::MeshSceneNode","peak::SceneNode",NULL);
   #endif
   tolua_beginmodule(tolua_S,"MeshSceneNode");
    tolua_function(tolua_S,"new",tolua_peak_peak_MeshSceneNode_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_MeshSceneNode_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_MeshSceneNode_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_MeshSceneNode_delete00);
    tolua_function(tolua_S,"getEndFrame",tolua_peak_peak_MeshSceneNode_getEndFrame00);
    tolua_function(tolua_S,"getFrameNr",tolua_peak_peak_MeshSceneNode_getFrameNr00);
    tolua_function(tolua_S,"getStartFrame",tolua_peak_peak_MeshSceneNode_getStartFrame00);
    tolua_function(tolua_S,"setLoopMode",tolua_peak_peak_MeshSceneNode_setLoopMode00);
    tolua_function(tolua_S,"setAnimationSpeed",tolua_peak_peak_MeshSceneNode_setAnimationSpeed00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   tolua_constant(tolua_S,"EMT_Solid",peak::EMT_Solid);
   tolua_constant(tolua_S,"EMT_VertexAlpha",peak::EMT_VertexAlpha);
   tolua_constant(tolua_S,"EMT_AlphaChannel",peak::EMT_AlphaChannel);
   tolua_constant(tolua_S,"EMT_Add",peak::EMT_Add);
   tolua_constant(tolua_S,"EMT_DetailMap",peak::EMT_DetailMap);
   tolua_constant(tolua_S,"EMT_ParallaxMap",peak::EMT_ParallaxMap);
   tolua_constant(tolua_S,"EMT_NormalMap",peak::EMT_NormalMap);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Material","peak::Material","",tolua_collect_peak__Material);
   #else
   tolua_cclass(tolua_S,"Material","peak::Material","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Material");
    tolua_function(tolua_S,"new",tolua_peak_peak_Material_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Material_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Material_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_Material_delete00);
    tolua_function(tolua_S,"load",tolua_peak_peak_Material_load00);
    tolua_function(tolua_S,"setType",tolua_peak_peak_Material_setType00);
    tolua_function(tolua_S,"getType",tolua_peak_peak_Material_getType00);
    tolua_function(tolua_S,"setTextureCount",tolua_peak_peak_Material_setTextureCount00);
    tolua_function(tolua_S,"getTextureCount",tolua_peak_peak_Material_getTextureCount00);
    tolua_function(tolua_S,"setTexture",tolua_peak_peak_Material_setTexture00);
    tolua_function(tolua_S,"getTexture",tolua_peak_peak_Material_getTexture00);
    tolua_function(tolua_S,"setTextureScale",tolua_peak_peak_Material_setTextureScale00);
    tolua_function(tolua_S,"getTextureScale",tolua_peak_peak_Material_getTextureScale00);
    tolua_function(tolua_S,"setWireframe",tolua_peak_peak_Material_setWireframe00);
    tolua_function(tolua_S,"isWireframe",tolua_peak_peak_Material_isWireframe00);
    tolua_function(tolua_S,"setLighting",tolua_peak_peak_Material_setLighting00);
    tolua_function(tolua_S,"getLighting",tolua_peak_peak_Material_getLighting00);
    tolua_function(tolua_S,"setBackfaceCulling",tolua_peak_peak_Material_setBackfaceCulling00);
    tolua_function(tolua_S,"getBackfaceCulling",tolua_peak_peak_Material_getBackfaceCulling00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Overlay","peak::Overlay","",tolua_collect_peak__Overlay);
   #else
   tolua_cclass(tolua_S,"Overlay","peak::Overlay","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Overlay");
    tolua_function(tolua_S,"new",tolua_peak_peak_Overlay_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Overlay_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Overlay_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_Overlay_delete00);
    tolua_function(tolua_S,"setParent",tolua_peak_peak_Overlay_setParent00);
    tolua_function(tolua_S,"getParent",tolua_peak_peak_Overlay_getParent00);
    tolua_function(tolua_S,"setPosition",tolua_peak_peak_Overlay_setPosition00);
    tolua_function(tolua_S,"getPosition",tolua_peak_peak_Overlay_getPosition00);
    tolua_function(tolua_S,"setSize",tolua_peak_peak_Overlay_setSize00);
    tolua_function(tolua_S,"getSize",tolua_peak_peak_Overlay_getSize00);
    tolua_function(tolua_S,"setRotation",tolua_peak_peak_Overlay_setRotation00);
    tolua_function(tolua_S,"getRotation",tolua_peak_peak_Overlay_getRotation00);
    tolua_function(tolua_S,"setClipChildren",tolua_peak_peak_Overlay_setClipChildren00);
    tolua_function(tolua_S,"getClipChildren",tolua_peak_peak_Overlay_getClipChildren00);
    tolua_function(tolua_S,"setAbsolutePosition",tolua_peak_peak_Overlay_setAbsolutePosition00);
    tolua_function(tolua_S,"getAbsolutePosition",tolua_peak_peak_Overlay_getAbsolutePosition00);
    tolua_function(tolua_S,"setAbsoluteSize",tolua_peak_peak_Overlay_setAbsoluteSize00);
    tolua_function(tolua_S,"getAbsoluteSize",tolua_peak_peak_Overlay_getAbsoluteSize00);
    tolua_function(tolua_S,"setAbsoluteRotation",tolua_peak_peak_Overlay_setAbsoluteRotation00);
    tolua_function(tolua_S,"getAbsoluteRotation",tolua_peak_peak_Overlay_getAbsoluteRotation00);
    tolua_function(tolua_S,"notifySizeChanged",tolua_peak_peak_Overlay_notifySizeChanged00);
    tolua_function(tolua_S,"render",tolua_peak_peak_Overlay_render00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"RectangleOverlay","peak::RectangleOverlay","peak::Overlay",tolua_collect_peak__RectangleOverlay);
   #else
   tolua_cclass(tolua_S,"RectangleOverlay","peak::RectangleOverlay","peak::Overlay",NULL);
   #endif
   tolua_beginmodule(tolua_S,"RectangleOverlay");
    tolua_function(tolua_S,"new",tolua_peak_peak_RectangleOverlay_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_RectangleOverlay_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_RectangleOverlay_new00_local);
    tolua_function(tolua_S,"new",tolua_peak_peak_RectangleOverlay_new01);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_RectangleOverlay_new01_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_RectangleOverlay_new01_local);
    tolua_function(tolua_S,"new",tolua_peak_peak_RectangleOverlay_new02);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_RectangleOverlay_new02_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_RectangleOverlay_new02_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_RectangleOverlay_delete00);
    tolua_function(tolua_S,"render",tolua_peak_peak_RectangleOverlay_render00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"TextureOverlay","peak::TextureOverlay","peak::Overlay",tolua_collect_peak__TextureOverlay);
   #else
   tolua_cclass(tolua_S,"TextureOverlay","peak::TextureOverlay","peak::Overlay",NULL);
   #endif
   tolua_beginmodule(tolua_S,"TextureOverlay");
    tolua_function(tolua_S,"new",tolua_peak_peak_TextureOverlay_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_TextureOverlay_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_TextureOverlay_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_TextureOverlay_delete00);
    tolua_function(tolua_S,"notifySizeChanged",tolua_peak_peak_TextureOverlay_notifySizeChanged00);
    tolua_function(tolua_S,"render",tolua_peak_peak_TextureOverlay_render00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"LabelOverlay","peak::LabelOverlay","peak::Overlay",tolua_collect_peak__LabelOverlay);
   #else
   tolua_cclass(tolua_S,"LabelOverlay","peak::LabelOverlay","peak::Overlay",NULL);
   #endif
   tolua_beginmodule(tolua_S,"LabelOverlay");
    tolua_function(tolua_S,"new",tolua_peak_peak_LabelOverlay_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_LabelOverlay_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_LabelOverlay_new00_local);
    tolua_function(tolua_S,"new",tolua_peak_peak_LabelOverlay_new01);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_LabelOverlay_new01_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_LabelOverlay_new01_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_LabelOverlay_delete00);
    tolua_function(tolua_S,"setLabel",tolua_peak_peak_LabelOverlay_setLabel00);
    tolua_function(tolua_S,"getLabel",tolua_peak_peak_LabelOverlay_getLabel00);
    tolua_function(tolua_S,"setColor",tolua_peak_peak_LabelOverlay_setColor00);
    tolua_function(tolua_S,"getColor",tolua_peak_peak_LabelOverlay_getColor00);
    tolua_function(tolua_S,"render",tolua_peak_peak_LabelOverlay_render00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"OverlayManager","peak::OverlayManager","",tolua_collect_peak__OverlayManager);
   #else
   tolua_cclass(tolua_S,"OverlayManager","peak::OverlayManager","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"OverlayManager");
    tolua_function(tolua_S,"get",tolua_peak_peak_OverlayManager_get00);
    tolua_function(tolua_S,"delete",tolua_peak_peak_OverlayManager_delete00);
    tolua_function(tolua_S,"init",tolua_peak_peak_OverlayManager_init00);
    tolua_function(tolua_S,"shutdown",tolua_peak_peak_OverlayManager_shutdown00);
    tolua_function(tolua_S,"getRootOverlay",tolua_peak_peak_OverlayManager_getRootOverlay00);
    tolua_function(tolua_S,"render",tolua_peak_peak_OverlayManager_render00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"SkyBox","peak::SkyBox","peak::SceneNode",tolua_collect_peak__SkyBox);
   #else
   tolua_cclass(tolua_S,"SkyBox","peak::SkyBox","peak::SceneNode",NULL);
   #endif
   tolua_beginmodule(tolua_S,"SkyBox");
    tolua_function(tolua_S,"new",tolua_peak_peak_SkyBox_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_SkyBox_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_SkyBox_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_SkyBox_delete00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"SkyDome","peak::SkyDome","peak::SceneNode",tolua_collect_peak__SkyDome);
   #else
   tolua_cclass(tolua_S,"SkyDome","peak::SkyDome","peak::SceneNode",NULL);
   #endif
   tolua_beginmodule(tolua_S,"SkyDome");
    tolua_function(tolua_S,"new",tolua_peak_peak_SkyDome_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_SkyDome_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_SkyDome_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_SkyDome_delete00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"SceneNodeController","peak::SceneNodeController","",tolua_collect_peak__SceneNodeController);
   #else
   tolua_cclass(tolua_S,"SceneNodeController","peak::SceneNodeController","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"SceneNodeController");
    tolua_function(tolua_S,"delete",tolua_peak_peak_SceneNodeController_delete00);
    tolua_function(tolua_S,"setSceneNode",tolua_peak_peak_SceneNodeController_setSceneNode00);
    tolua_function(tolua_S,"getSceneNode",tolua_peak_peak_SceneNodeController_getSceneNode00);
    tolua_function(tolua_S,"doAllWork",tolua_peak_peak_SceneNodeController_doAllWork00);
    tolua_function(tolua_S,"doWork",tolua_peak_peak_SceneNodeController_doWork00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   tolua_constant(tolua_S,"EFCM_PositionApplyX",peak::EFCM_PositionApplyX);
   tolua_constant(tolua_S,"EFCM_PositionApplyY",peak::EFCM_PositionApplyY);
   tolua_constant(tolua_S,"EFCM_PositionApplyZ",peak::EFCM_PositionApplyZ);
   tolua_constant(tolua_S,"EFCM_PositionApplyAll",peak::EFCM_PositionApplyAll);
   tolua_constant(tolua_S,"EFCM_RotationApplyX",peak::EFCM_RotationApplyX);
   tolua_constant(tolua_S,"EFCM_RotationApplyY",peak::EFCM_RotationApplyY);
   tolua_constant(tolua_S,"EFCM_RotationApplyZ",peak::EFCM_RotationApplyZ);
   tolua_constant(tolua_S,"EFCM_RotationApplyAll",peak::EFCM_RotationApplyAll);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"FollowingController","peak::FollowingController","peak::SceneNodeController",tolua_collect_peak__FollowingController);
   #else
   tolua_cclass(tolua_S,"FollowingController","peak::FollowingController","peak::SceneNodeController",NULL);
   #endif
   tolua_beginmodule(tolua_S,"FollowingController");
    tolua_function(tolua_S,"new",tolua_peak_peak_FollowingController_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_FollowingController_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_FollowingController_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_FollowingController_delete00);
    tolua_function(tolua_S,"setTarget",tolua_peak_peak_FollowingController_setTarget00);
    tolua_function(tolua_S,"getTarget",tolua_peak_peak_FollowingController_getTarget00);
    tolua_function(tolua_S,"setDistance",tolua_peak_peak_FollowingController_setDistance00);
    tolua_function(tolua_S,"getDistance",tolua_peak_peak_FollowingController_getDistance00);
    tolua_function(tolua_S,"setMode",tolua_peak_peak_FollowingController_setMode00);
    tolua_function(tolua_S,"getMode",tolua_peak_peak_FollowingController_getMode00);
    tolua_function(tolua_S,"doWork",tolua_peak_peak_FollowingController_doWork00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"RotationController","peak::RotationController","peak::SceneNodeController",tolua_collect_peak__RotationController);
   #else
   tolua_cclass(tolua_S,"RotationController","peak::RotationController","peak::SceneNodeController",NULL);
   #endif
   tolua_beginmodule(tolua_S,"RotationController");
    tolua_function(tolua_S,"new",tolua_peak_peak_RotationController_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_RotationController_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_RotationController_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_RotationController_delete00);
    tolua_function(tolua_S,"setRotationSpeed",tolua_peak_peak_RotationController_setRotationSpeed00);
    tolua_function(tolua_S,"getRotationSpeed",tolua_peak_peak_RotationController_getRotationSpeed00);
    tolua_function(tolua_S,"doWork",tolua_peak_peak_RotationController_doWork00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"ContactInfo","peak::ContactInfo","",tolua_collect_peak__ContactInfo);
   #else
   tolua_cclass(tolua_S,"ContactInfo","peak::ContactInfo","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"ContactInfo");
    tolua_variable(tolua_S,"pos",tolua_get_peak__ContactInfo_pos,tolua_set_peak__ContactInfo_pos);
    tolua_variable(tolua_S,"geom",tolua_get_peak__ContactInfo_geom_ptr,tolua_set_peak__ContactInfo_geom_ptr);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Body","peak::Body","",tolua_collect_peak__Body);
   #else
   tolua_cclass(tolua_S,"Body","peak::Body","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Body");
    tolua_function(tolua_S,"new",tolua_peak_peak_Body_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Body_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Body_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_Body_delete00);
    tolua_function(tolua_S,"init",tolua_peak_peak_Body_init00);
    tolua_function(tolua_S,"destroy",tolua_peak_peak_Body_destroy00);
    tolua_function(tolua_S,"setPosition",tolua_peak_peak_Body_setPosition00);
    tolua_function(tolua_S,"setPosition",tolua_peak_peak_Body_setPosition01);
    tolua_function(tolua_S,"getPosition",tolua_peak_peak_Body_getPosition00);
    tolua_function(tolua_S,"setRotation",tolua_peak_peak_Body_setRotation00);
    tolua_function(tolua_S,"setRotation",tolua_peak_peak_Body_setRotation01);
    tolua_function(tolua_S,"setRotation",tolua_peak_peak_Body_setRotation02);
    tolua_function(tolua_S,"getRotation",tolua_peak_peak_Body_getRotation00);
    tolua_function(tolua_S,"setLinearVelocity",tolua_peak_peak_Body_setLinearVelocity00);
    tolua_function(tolua_S,"getLinearVelocity",tolua_peak_peak_Body_getLinearVelocity00);
    tolua_function(tolua_S,"setAngularVelocity",tolua_peak_peak_Body_setAngularVelocity00);
    tolua_function(tolua_S,"getAngularVelocity",tolua_peak_peak_Body_getAngularVelocity00);
    tolua_function(tolua_S,"addForce",tolua_peak_peak_Body_addForce00);
    tolua_function(tolua_S,"setForce",tolua_peak_peak_Body_setForce00);
    tolua_function(tolua_S,"getForce",tolua_peak_peak_Body_getForce00);
    tolua_function(tolua_S,"setRelativeForce",tolua_peak_peak_Body_setRelativeForce00);
    tolua_function(tolua_S,"getRelativeForce",tolua_peak_peak_Body_getRelativeForce00);
    tolua_function(tolua_S,"setEntity",tolua_peak_peak_Body_setEntity00);
    tolua_function(tolua_S,"getEntity",tolua_peak_peak_Body_getEntity00);
    tolua_function(tolua_S,"setGravity",tolua_peak_peak_Body_setGravity00);
    tolua_function(tolua_S,"getGravity",tolua_peak_peak_Body_getGravity00);
    tolua_function(tolua_S,"getBody",tolua_peak_peak_Body_getBody00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Plane","peak::Plane","peak::Geometry",tolua_collect_peak__Plane);
   #else
   tolua_cclass(tolua_S,"Plane","peak::Plane","peak::Geometry",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Plane");
    tolua_function(tolua_S,"new",tolua_peak_peak_Plane_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Plane_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Plane_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_Plane_delete00);
    tolua_function(tolua_S,"init",tolua_peak_peak_Plane_init00);
    tolua_function(tolua_S,"destroy",tolua_peak_peak_Plane_destroy00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Cube","peak::Cube","peak::Geometry",tolua_collect_peak__Cube);
   #else
   tolua_cclass(tolua_S,"Cube","peak::Cube","peak::Geometry",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Cube");
    tolua_function(tolua_S,"new",tolua_peak_peak_Cube_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Cube_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Cube_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_Cube_delete00);
    tolua_function(tolua_S,"init",tolua_peak_peak_Cube_init00);
    tolua_function(tolua_S,"destroy",tolua_peak_peak_Cube_destroy00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Cylinder","peak::Cylinder","peak::Geometry",tolua_collect_peak__Cylinder);
   #else
   tolua_cclass(tolua_S,"Cylinder","peak::Cylinder","peak::Geometry",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Cylinder");
    tolua_function(tolua_S,"new",tolua_peak_peak_Cylinder_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Cylinder_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Cylinder_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_Cylinder_delete00);
    tolua_function(tolua_S,"init",tolua_peak_peak_Cylinder_init00);
    tolua_function(tolua_S,"init",tolua_peak_peak_Cylinder_init01);
    tolua_function(tolua_S,"destroy",tolua_peak_peak_Cylinder_destroy00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   tolua_cclass(tolua_S,"ServerObject","peak::ServerObject","",NULL);
   tolua_beginmodule(tolua_S,"ServerObject");
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"CollisionCallback","peak::CollisionCallback","",NULL);
   tolua_beginmodule(tolua_S,"CollisionCallback");
    tolua_variable(tolua_S,"script",tolua_get_peak__CollisionCallback_script_ptr,tolua_set_peak__CollisionCallback_script_ptr);
    tolua_variable(tolua_S,"functionname",tolua_get_peak__CollisionCallback_functionname,tolua_set_peak__CollisionCallback_functionname);
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"CollisionInfo","peak::CollisionInfo","",NULL);
   tolua_beginmodule(tolua_S,"CollisionInfo");
    tolua_variable(tolua_S,"friction",tolua_get_peak__CollisionInfo_friction,tolua_set_peak__CollisionInfo_friction);
    tolua_variable(tolua_S,"bounce",tolua_get_peak__CollisionInfo_bounce,tolua_set_peak__CollisionInfo_bounce);
    tolua_variable(tolua_S,"bouncevelocity",tolua_get_peak__CollisionInfo_bouncevelocity,tolua_set_peak__CollisionInfo_bouncevelocity);
    tolua_variable(tolua_S,"callback1",tolua_get_peak__CollisionInfo_callback1,tolua_set_peak__CollisionInfo_callback1);
    tolua_variable(tolua_S,"callback2",tolua_get_peak__CollisionInfo_callback2,tolua_set_peak__CollisionInfo_callback2);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Joint","peak::Joint","",tolua_collect_peak__Joint);
   #else
   tolua_cclass(tolua_S,"Joint","peak::Joint","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Joint");
    tolua_function(tolua_S,"delete",tolua_peak_peak_Joint_delete00);
    tolua_function(tolua_S,"init",tolua_peak_peak_Joint_init00);
    tolua_function(tolua_S,"destroy",tolua_peak_peak_Joint_destroy00);
    tolua_function(tolua_S,"setBody1",tolua_peak_peak_Joint_setBody100);
    tolua_function(tolua_S,"setBody2",tolua_peak_peak_Joint_setBody200);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Hinge","peak::Hinge","peak::Joint",tolua_collect_peak__Hinge);
   #else
   tolua_cclass(tolua_S,"Hinge","peak::Hinge","peak::Joint",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Hinge");
    tolua_function(tolua_S,"new",tolua_peak_peak_Hinge_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Hinge_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Hinge_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_Hinge_delete00);
    tolua_function(tolua_S,"init",tolua_peak_peak_Hinge_init00);
    tolua_function(tolua_S,"setAnchor",tolua_peak_peak_Hinge_setAnchor00);
    tolua_function(tolua_S,"setAxis",tolua_peak_peak_Hinge_setAxis00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Ray","peak::Ray","peak::Geometry",tolua_collect_peak__Ray);
   #else
   tolua_cclass(tolua_S,"Ray","peak::Ray","peak::Geometry",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Ray");
    tolua_function(tolua_S,"new",tolua_peak_peak_Ray_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Ray_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Ray_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_Ray_delete00);
    tolua_function(tolua_S,"init",tolua_peak_peak_Ray_init00);
    tolua_function(tolua_S,"destroy",tolua_peak_peak_Ray_destroy00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   tolua_cclass(tolua_S,"Contact","peak::Contact","",NULL);
   tolua_beginmodule(tolua_S,"Contact");
    tolua_variable(tolua_S,"position",tolua_get_peak__Contact_position,tolua_set_peak__Contact_position);
    tolua_variable(tolua_S,"normal",tolua_get_peak__Contact_normal,tolua_set_peak__Contact_normal);
    tolua_variable(tolua_S,"depth",tolua_get_peak__Contact_depth,tolua_set_peak__Contact_depth);
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"Collision","peak::Collision","",NULL);
   tolua_beginmodule(tolua_S,"Collision");
    tolua_function(tolua_S,"getContact",tolua_peak_peak_Collision_getContact00);
    tolua_function(tolua_S,"getContactCount",tolua_peak_peak_Collision_getContactCount00);
    tolua_variable(tolua_S,"contacts",tolua_get_peak__Collision_contacts,tolua_set_peak__Collision_contacts);
    tolua_variable(tolua_S,"speed",tolua_get_peak__Collision_speed,tolua_set_peak__Collision_speed);
    tolua_variable(tolua_S,"geom1",tolua_get_peak__Collision_geom1_ptr,tolua_set_peak__Collision_geom1_ptr);
    tolua_variable(tolua_S,"geom2",tolua_get_peak__Collision_geom2_ptr,tolua_set_peak__Collision_geom2_ptr);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"PhysicsComponent","peak::PhysicsComponent","peak::Component",tolua_collect_peak__PhysicsComponent);
   #else
   tolua_cclass(tolua_S,"PhysicsComponent","peak::PhysicsComponent","peak::Component",NULL);
   #endif
   tolua_beginmodule(tolua_S,"PhysicsComponent");
    tolua_function(tolua_S,"new",tolua_peak_peak_PhysicsComponent_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_PhysicsComponent_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_PhysicsComponent_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_PhysicsComponent_delete00);
    tolua_function(tolua_S,"init",tolua_peak_peak_PhysicsComponent_init00);
    tolua_function(tolua_S,"destroy",tolua_peak_peak_PhysicsComponent_destroy00);
    tolua_function(tolua_S,"needsUpdate",tolua_peak_peak_PhysicsComponent_needsUpdate00);
    tolua_function(tolua_S,"getUpdateSize",tolua_peak_peak_PhysicsComponent_getUpdateSize00);
    tolua_function(tolua_S,"writeUpdateData",tolua_peak_peak_PhysicsComponent_writeUpdateData00);
    tolua_function(tolua_S,"updateFromData",tolua_peak_peak_PhysicsComponent_updateFromData00);
    tolua_function(tolua_S,"writeCompleteData",tolua_peak_peak_PhysicsComponent_writeCompleteData00);
    tolua_function(tolua_S,"readCompleteData",tolua_peak_peak_PhysicsComponent_readCompleteData00);
    tolua_function(tolua_S,"move",tolua_peak_peak_PhysicsComponent_move00);
    tolua_function(tolua_S,"addBody",tolua_peak_peak_PhysicsComponent_addBody00);
    tolua_function(tolua_S,"getBody",tolua_peak_peak_PhysicsComponent_getBody00);
    tolua_function(tolua_S,"addJoint",tolua_peak_peak_PhysicsComponent_addJoint00);
    tolua_function(tolua_S,"getJoint",tolua_peak_peak_PhysicsComponent_getJoint00);
    tolua_function(tolua_S,"addGeom",tolua_peak_peak_PhysicsComponent_addGeom00);
    tolua_function(tolua_S,"getGeom",tolua_peak_peak_PhysicsComponent_getGeom00);
    tolua_function(tolua_S,"getType",tolua_peak_peak_PhysicsComponent_getType00);
    tolua_function(tolua_S,"doWork",tolua_peak_peak_PhysicsComponent_doWork00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"GraphicsComponent","peak::GraphicsComponent","peak::Component",tolua_collect_peak__GraphicsComponent);
   #else
   tolua_cclass(tolua_S,"GraphicsComponent","peak::GraphicsComponent","peak::Component",NULL);
   #endif
   tolua_beginmodule(tolua_S,"GraphicsComponent");
    tolua_function(tolua_S,"new",tolua_peak_peak_GraphicsComponent_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_GraphicsComponent_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_GraphicsComponent_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_GraphicsComponent_delete00);
    tolua_function(tolua_S,"init",tolua_peak_peak_GraphicsComponent_init00);
    tolua_function(tolua_S,"destroy",tolua_peak_peak_GraphicsComponent_destroy00);
    tolua_function(tolua_S,"needsUpdate",tolua_peak_peak_GraphicsComponent_needsUpdate00);
    tolua_function(tolua_S,"getUpdateSize",tolua_peak_peak_GraphicsComponent_getUpdateSize00);
    tolua_function(tolua_S,"writeUpdateData",tolua_peak_peak_GraphicsComponent_writeUpdateData00);
    tolua_function(tolua_S,"updateFromData",tolua_peak_peak_GraphicsComponent_updateFromData00);
    tolua_function(tolua_S,"writeCompleteData",tolua_peak_peak_GraphicsComponent_writeCompleteData00);
    tolua_function(tolua_S,"readCompleteData",tolua_peak_peak_GraphicsComponent_readCompleteData00);
    tolua_function(tolua_S,"addSceneNode",tolua_peak_peak_GraphicsComponent_addSceneNode00);
    tolua_function(tolua_S,"getSceneNode",tolua_peak_peak_GraphicsComponent_getSceneNode00);
    tolua_function(tolua_S,"getType",tolua_peak_peak_GraphicsComponent_getType00);
    tolua_function(tolua_S,"doWork",tolua_peak_peak_GraphicsComponent_doWork00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"ServerScriptComponent","peak::ServerScriptComponent","peak::Component",tolua_collect_peak__ServerScriptComponent);
   #else
   tolua_cclass(tolua_S,"ServerScriptComponent","peak::ServerScriptComponent","peak::Component",NULL);
   #endif
   tolua_beginmodule(tolua_S,"ServerScriptComponent");
    tolua_function(tolua_S,"new",tolua_peak_peak_ServerScriptComponent_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_ServerScriptComponent_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_ServerScriptComponent_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_ServerScriptComponent_delete00);
    tolua_function(tolua_S,"init",tolua_peak_peak_ServerScriptComponent_init00);
    tolua_function(tolua_S,"destroy",tolua_peak_peak_ServerScriptComponent_destroy00);
    tolua_function(tolua_S,"needsUpdate",tolua_peak_peak_ServerScriptComponent_needsUpdate00);
    tolua_function(tolua_S,"getUpdateSize",tolua_peak_peak_ServerScriptComponent_getUpdateSize00);
    tolua_function(tolua_S,"writeUpdateData",tolua_peak_peak_ServerScriptComponent_writeUpdateData00);
    tolua_function(tolua_S,"updateFromData",tolua_peak_peak_ServerScriptComponent_updateFromData00);
    tolua_function(tolua_S,"writeCompleteData",tolua_peak_peak_ServerScriptComponent_writeCompleteData00);
    tolua_function(tolua_S,"readCompleteData",tolua_peak_peak_ServerScriptComponent_readCompleteData00);
    tolua_function(tolua_S,"runScript",tolua_peak_peak_ServerScriptComponent_runScript00);
    tolua_function(tolua_S,"getType",tolua_peak_peak_ServerScriptComponent_getType00);
    tolua_function(tolua_S,"doWork",tolua_peak_peak_ServerScriptComponent_doWork00);
    tolua_function(tolua_S,"getScript",tolua_peak_peak_ServerScriptComponent_getScript00);
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"ClientScriptComponent","peak::ClientScriptComponent","",NULL);
   tolua_beginmodule(tolua_S,"ClientScriptComponent");
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"EntityVariables","peak::EntityVariables","",tolua_collect_peak__EntityVariables);
   #else
   tolua_cclass(tolua_S,"EntityVariables","peak::EntityVariables","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"EntityVariables");
    tolua_function(tolua_S,"new",tolua_peak_peak_EntityVariables_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_EntityVariables_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_EntityVariables_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_EntityVariables_delete00);
    tolua_function(tolua_S,"load",tolua_peak_peak_EntityVariables_load00);
    tolua_function(tolua_S,"destroy",tolua_peak_peak_EntityVariables_destroy00);
    tolua_function(tolua_S,"setInteger",tolua_peak_peak_EntityVariables_setInteger00);
    tolua_function(tolua_S,"setFloat",tolua_peak_peak_EntityVariables_setFloat00);
    tolua_function(tolua_S,"set",tolua_peak_peak_EntityVariables_set00);
    tolua_function(tolua_S,"set",tolua_peak_peak_EntityVariables_set01);
    tolua_function(tolua_S,"set",tolua_peak_peak_EntityVariables_set02);
    tolua_function(tolua_S,"set",tolua_peak_peak_EntityVariables_set03);
    tolua_function(tolua_S,"getInteger",tolua_peak_peak_EntityVariables_getInteger00);
    tolua_function(tolua_S,"getFloat",tolua_peak_peak_EntityVariables_getFloat00);
    tolua_function(tolua_S,"getVector3D",tolua_peak_peak_EntityVariables_getVector3D00);
    tolua_function(tolua_S,"getVector2D",tolua_peak_peak_EntityVariables_getVector2D00);
    tolua_function(tolua_S,"getQuaternion",tolua_peak_peak_EntityVariables_getQuaternion00);
    tolua_function(tolua_S,"getString",tolua_peak_peak_EntityVariables_getString00);
    tolua_function(tolua_S,"needsUpdate",tolua_peak_peak_EntityVariables_needsUpdate00);
    tolua_function(tolua_S,"getUpdateSize",tolua_peak_peak_EntityVariables_getUpdateSize00);
    tolua_function(tolua_S,"writeUpdateData",tolua_peak_peak_EntityVariables_writeUpdateData00);
    tolua_function(tolua_S,"updateFromData",tolua_peak_peak_EntityVariables_updateFromData00);
    tolua_function(tolua_S,"writeCompleteData",tolua_peak_peak_EntityVariables_writeCompleteData00);
    tolua_function(tolua_S,"readCompleteData",tolua_peak_peak_EntityVariables_readCompleteData00);
    tolua_function(tolua_S,"wasUpdated",tolua_peak_peak_EntityVariables_wasUpdated00);
    tolua_function(tolua_S,"getVariable",tolua_peak_peak_EntityVariables_getVariable00);
    tolua_function(tolua_S,"getVariable",tolua_peak_peak_EntityVariables_getVariable01);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   tolua_constant(tolua_S,"ECT_ServerScript",peak::ECT_ServerScript);
   tolua_constant(tolua_S,"ECT_Physics",peak::ECT_Physics);
   tolua_constant(tolua_S,"ECT_Graphics",peak::ECT_Graphics);
   tolua_constant(tolua_S,"ECT_ClientScript",peak::ECT_ClientScript);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Component","peak::Component","",tolua_collect_peak__Component);
   #else
   tolua_cclass(tolua_S,"Component","peak::Component","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Component");
    tolua_function(tolua_S,"delete",tolua_peak_peak_Component_delete00);
    tolua_function(tolua_S,"init",tolua_peak_peak_Component_init00);
    tolua_function(tolua_S,"destroy",tolua_peak_peak_Component_destroy00);
    tolua_function(tolua_S,"needsUpdate",tolua_peak_peak_Component_needsUpdate00);
    tolua_function(tolua_S,"getUpdateSize",tolua_peak_peak_Component_getUpdateSize00);
    tolua_function(tolua_S,"writeUpdateData",tolua_peak_peak_Component_writeUpdateData00);
    tolua_function(tolua_S,"updateFromData",tolua_peak_peak_Component_updateFromData00);
    tolua_function(tolua_S,"writeCompleteData",tolua_peak_peak_Component_writeCompleteData00);
    tolua_function(tolua_S,"readCompleteData",tolua_peak_peak_Component_readCompleteData00);
    tolua_function(tolua_S,"getType",tolua_peak_peak_Component_getType00);
    tolua_function(tolua_S,"doWork",tolua_peak_peak_Component_doWork00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   tolua_cclass(tolua_S,"SceneNodeData","peak::SceneNodeData","",NULL);
   tolua_beginmodule(tolua_S,"SceneNodeData");
    tolua_variable(tolua_S,"scenenode",tolua_get_peak__SceneNodeData_scenenode_ptr,tolua_set_peak__SceneNodeData_scenenode_ptr);
    tolua_variable(tolua_S,"parentid",tolua_get_peak__SceneNodeData_parentid,tolua_set_peak__SceneNodeData_parentid);
    tolua_variable(tolua_S,"body",tolua_get_peak__SceneNodeData_body_ptr,tolua_set_peak__SceneNodeData_body_ptr);
    tolua_variable(tolua_S,"speed",tolua_get_peak__SceneNodeData_speed,tolua_set_peak__SceneNodeData_speed);
    tolua_variable(tolua_S,"oldspeed",tolua_get_peak__SceneNodeData_oldspeed,tolua_set_peak__SceneNodeData_oldspeed);
    tolua_variable(tolua_S,"position",tolua_get_peak__SceneNodeData_position,tolua_set_peak__SceneNodeData_position);
    tolua_variable(tolua_S,"rotation",tolua_get_peak__SceneNodeData_rotation,tolua_set_peak__SceneNodeData_rotation);
    tolua_variable(tolua_S,"update",tolua_get_peak__SceneNodeData_unsigned_update,tolua_set_peak__SceneNodeData_unsigned_update);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   tolua_cclass(tolua_S,"IniFile","peak::IniFile","",NULL);
   tolua_beginmodule(tolua_S,"IniFile");
   tolua_endmodule(tolua_S);
   tolua_constant(tolua_S,"EVT_None",peak::EVT_None);
   tolua_constant(tolua_S,"EVT_Integer",peak::EVT_Integer);
   tolua_constant(tolua_S,"EVT_Word",peak::EVT_Word);
   tolua_constant(tolua_S,"EVT_Byte",peak::EVT_Byte);
   tolua_constant(tolua_S,"EVT_Float",peak::EVT_Float);
   tolua_constant(tolua_S,"EVT_Vector3D",peak::EVT_Vector3D);
   tolua_constant(tolua_S,"EVT_Vector2D",peak::EVT_Vector2D);
   tolua_constant(tolua_S,"EVT_Quaternion",peak::EVT_Quaternion);
   tolua_constant(tolua_S,"EVT_String",peak::EVT_String);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Variable","peak::Variable","",tolua_collect_peak__Variable);
   #else
   tolua_cclass(tolua_S,"Variable","peak::Variable","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Variable");
    tolua_function(tolua_S,"new",tolua_peak_peak_Variable_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Variable_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Variable_new00_local);
    tolua_function(tolua_S,"getInteger",tolua_peak_peak_Variable_getInteger00);
    tolua_function(tolua_S,"getFloat",tolua_peak_peak_Variable_getFloat00);
    tolua_function(tolua_S,"getVector3D",tolua_peak_peak_Variable_getVector3D00);
    tolua_function(tolua_S,"getVector2D",tolua_peak_peak_Variable_getVector2D00);
    tolua_function(tolua_S,"getQuaternion",tolua_peak_peak_Variable_getQuaternion00);
    tolua_function(tolua_S,"getString",tolua_peak_peak_Variable_getString00);
    tolua_function(tolua_S,"wasUpdated",tolua_peak_peak_Variable_wasUpdated00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Menu","peak::Menu","",tolua_collect_peak__Menu);
   #else
   tolua_cclass(tolua_S,"Menu","peak::Menu","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Menu");
    tolua_function(tolua_S,"new",tolua_peak_peak_Menu_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Menu_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Menu_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_Menu_delete00);
    tolua_function(tolua_S,"init",tolua_peak_peak_Menu_init00);
    tolua_function(tolua_S,"destroy",tolua_peak_peak_Menu_destroy00);
    tolua_function(tolua_S,"getName",tolua_peak_peak_Menu_getName00);
    tolua_function(tolua_S,"show",tolua_peak_peak_Menu_show00);
    tolua_function(tolua_S,"hide",tolua_peak_peak_Menu_hide00);
    tolua_function(tolua_S,"pressButton",tolua_peak_peak_Menu_pressButton00);
    tolua_function(tolua_S,"setCheckBox",tolua_peak_peak_Menu_setCheckBox00);
    tolua_function(tolua_S,"setListBox",tolua_peak_peak_Menu_setListBox00);
    tolua_function(tolua_S,"getListBox",tolua_peak_peak_Menu_getListBox00);
    tolua_function(tolua_S,"getButton",tolua_peak_peak_Menu_getButton00);
    tolua_function(tolua_S,"getLabel",tolua_peak_peak_Menu_getLabel00);
    tolua_function(tolua_S,"getElement",tolua_peak_peak_Menu_getElement00);
    tolua_function(tolua_S,"getScript",tolua_peak_peak_Menu_getScript00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"MenuManager","peak::MenuManager","",tolua_collect_peak__MenuManager);
   #else
   tolua_cclass(tolua_S,"MenuManager","peak::MenuManager","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"MenuManager");
    tolua_function(tolua_S,"get",tolua_peak_peak_MenuManager_get00);
    tolua_function(tolua_S,"delete",tolua_peak_peak_MenuManager_delete00);
    tolua_function(tolua_S,"loadMenu",tolua_peak_peak_MenuManager_loadMenu00);
    tolua_function(tolua_S,"unloadMenu",tolua_peak_peak_MenuManager_unloadMenu00);
    tolua_function(tolua_S,"unloadMenu",tolua_peak_peak_MenuManager_unloadMenu01);
    tolua_function(tolua_S,"getMenuCount",tolua_peak_peak_MenuManager_getMenuCount00);
    tolua_function(tolua_S,"getMenu",tolua_peak_peak_MenuManager_getMenu00);
    tolua_function(tolua_S,"getMenu",tolua_peak_peak_MenuManager_getMenu01);
    tolua_function(tolua_S,"getCurrentMenu",tolua_peak_peak_MenuManager_getCurrentMenu00);
    tolua_function(tolua_S,"init",tolua_peak_peak_MenuManager_init00);
    tolua_function(tolua_S,"shutdown",tolua_peak_peak_MenuManager_shutdown00);
    tolua_function(tolua_S,"activateMenu",tolua_peak_peak_MenuManager_activateMenu00);
    tolua_function(tolua_S,"activateMenu",tolua_peak_peak_MenuManager_activateMenu01);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"MenuElement","peak::MenuElement","",tolua_collect_peak__MenuElement);
   #else
   tolua_cclass(tolua_S,"MenuElement","peak::MenuElement","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"MenuElement");
    tolua_function(tolua_S,"delete",tolua_peak_peak_MenuElement_delete00);
    tolua_function(tolua_S,"init",tolua_peak_peak_MenuElement_init00);
    tolua_function(tolua_S,"destroy",tolua_peak_peak_MenuElement_destroy00);
    tolua_function(tolua_S,"setPosition",tolua_peak_peak_MenuElement_setPosition00);
    tolua_function(tolua_S,"getPosition",tolua_peak_peak_MenuElement_getPosition00);
    tolua_function(tolua_S,"setSize",tolua_peak_peak_MenuElement_setSize00);
    tolua_function(tolua_S,"getSize",tolua_peak_peak_MenuElement_getSize00);
    tolua_function(tolua_S,"setID",tolua_peak_peak_MenuElement_setID00);
    tolua_function(tolua_S,"getID",tolua_peak_peak_MenuElement_getID00);
    tolua_function(tolua_S,"show",tolua_peak_peak_MenuElement_show00);
    tolua_function(tolua_S,"hide",tolua_peak_peak_MenuElement_hide00);
    tolua_function(tolua_S,"setText",tolua_peak_peak_MenuElement_setText00);
    tolua_function(tolua_S,"getText",tolua_peak_peak_MenuElement_getText00);
    tolua_function(tolua_S,"getType",tolua_peak_peak_MenuElement_getType00);
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"Button","peak::Button","",NULL);
   tolua_beginmodule(tolua_S,"Button");
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"Label","peak::Label","",NULL);
   tolua_beginmodule(tolua_S,"Label");
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"ListBox","peak::ListBox","peak::MenuElement",tolua_collect_peak__ListBox);
   #else
   tolua_cclass(tolua_S,"ListBox","peak::ListBox","peak::MenuElement",NULL);
   #endif
   tolua_beginmodule(tolua_S,"ListBox");
    tolua_function(tolua_S,"new",tolua_peak_peak_ListBox_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_ListBox_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_ListBox_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_ListBox_delete00);
    tolua_function(tolua_S,"init",tolua_peak_peak_ListBox_init00);
    tolua_function(tolua_S,"show",tolua_peak_peak_ListBox_show00);
    tolua_function(tolua_S,"hide",tolua_peak_peak_ListBox_hide00);
    tolua_function(tolua_S,"choose",tolua_peak_peak_ListBox_choose00);
    tolua_function(tolua_S,"getSelected",tolua_peak_peak_ListBox_getSelected00);
    tolua_function(tolua_S,"addItem",tolua_peak_peak_ListBox_addItem00);
    tolua_function(tolua_S,"deleteItem",tolua_peak_peak_ListBox_deleteItem00);
    tolua_function(tolua_S,"getItemCount",tolua_peak_peak_ListBox_getItemCount00);
    tolua_function(tolua_S,"getItem",tolua_peak_peak_ListBox_getItem00);
    tolua_function(tolua_S,"clearItems",tolua_peak_peak_ListBox_clearItems00);
    tolua_function(tolua_S,"updateItems",tolua_peak_peak_ListBox_updateItems00);
    tolua_function(tolua_S,"setAction",tolua_peak_peak_ListBox_setAction00);
    tolua_function(tolua_S,"getAction",tolua_peak_peak_ListBox_getAction00);
    tolua_function(tolua_S,"getType",tolua_peak_peak_ListBox_getType00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   tolua_constant(tolua_S,"EMET_Frame",peak::EMET_Frame);
   tolua_constant(tolua_S,"EMET_Button",peak::EMET_Button);
   tolua_constant(tolua_S,"EMET_Label",peak::EMET_Label);
   tolua_constant(tolua_S,"EMET_ListBox",peak::EMET_ListBox);
   tolua_constant(tolua_S,"EMET_CheckBox",peak::EMET_CheckBox);
   tolua_constant(tolua_S,"EMET_ComboBox",peak::EMET_ComboBox);
   tolua_constant(tolua_S,"EMET_EditBox",peak::EMET_EditBox);
   tolua_constant(tolua_S,"EMET_RadioBox",peak::EMET_RadioBox);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Sound","peak::Sound","",tolua_collect_peak__Sound);
   #else
   tolua_cclass(tolua_S,"Sound","peak::Sound","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Sound");
    tolua_function(tolua_S,"new",tolua_peak_peak_Sound_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Sound_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Sound_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_Sound_delete00);
    tolua_function(tolua_S,"init",tolua_peak_peak_Sound_init00);
    tolua_function(tolua_S,"destroy",tolua_peak_peak_Sound_destroy00);
    tolua_function(tolua_S,"getSource",tolua_peak_peak_Sound_getSource00);
    tolua_function(tolua_S,"setLooped",tolua_peak_peak_Sound_setLooped00);
    tolua_function(tolua_S,"getLooped",tolua_peak_peak_Sound_getLooped00);
    tolua_function(tolua_S,"setCleanUp",tolua_peak_peak_Sound_setCleanUp00);
    tolua_function(tolua_S,"getCleanUp",tolua_peak_peak_Sound_getCleanUp00);
    tolua_function(tolua_S,"setPaused",tolua_peak_peak_Sound_setPaused00);
    tolua_function(tolua_S,"isPaused",tolua_peak_peak_Sound_isPaused00);
    tolua_function(tolua_S,"setPlaying",tolua_peak_peak_Sound_setPlaying00);
    tolua_function(tolua_S,"isPlaying",tolua_peak_peak_Sound_isPlaying00);
    tolua_function(tolua_S,"setVolume",tolua_peak_peak_Sound_setVolume00);
    tolua_function(tolua_S,"getVolume",tolua_peak_peak_Sound_getVolume00);
    tolua_function(tolua_S,"setPosition",tolua_peak_peak_Sound_setPosition00);
    tolua_function(tolua_S,"getPosition",tolua_peak_peak_Sound_getPosition00);
    tolua_function(tolua_S,"setVelocity",tolua_peak_peak_Sound_setVelocity00);
    tolua_function(tolua_S,"getVelocity",tolua_peak_peak_Sound_getVelocity00);
    tolua_function(tolua_S,"setRelativeToListener",tolua_peak_peak_Sound_setRelativeToListener00);
    tolua_function(tolua_S,"isRelativeToListener",tolua_peak_peak_Sound_isRelativeToListener00);
    tolua_function(tolua_S,"attachToSceneNode",tolua_peak_peak_Sound_attachToSceneNode00);
    tolua_function(tolua_S,"doWork",tolua_peak_peak_Sound_doWork00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Listener","peak::Listener","",tolua_collect_peak__Listener);
   #else
   tolua_cclass(tolua_S,"Listener","peak::Listener","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Listener");
    tolua_function(tolua_S,"new",tolua_peak_peak_Listener_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Listener_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Listener_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_Listener_delete00);
    tolua_function(tolua_S,"setPosition",tolua_peak_peak_Listener_setPosition00);
    tolua_function(tolua_S,"getPosition",tolua_peak_peak_Listener_getPosition00);
    tolua_function(tolua_S,"setVelocity",tolua_peak_peak_Listener_setVelocity00);
    tolua_function(tolua_S,"getVelocity",tolua_peak_peak_Listener_getVelocity00);
    tolua_function(tolua_S,"setRotation",tolua_peak_peak_Listener_setRotation00);
    tolua_function(tolua_S,"getRotation",tolua_peak_peak_Listener_getRotation00);
    tolua_function(tolua_S,"attachToSceneNode",tolua_peak_peak_Listener_attachToSceneNode00);
    tolua_function(tolua_S,"doWork",tolua_peak_peak_Listener_doWork00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"SoundSource","peak::SoundSource","ReferenceCounted",tolua_collect_peak__SoundSource);
   #else
   tolua_cclass(tolua_S,"SoundSource","peak::SoundSource","ReferenceCounted",NULL);
   #endif
   tolua_beginmodule(tolua_S,"SoundSource");
    tolua_function(tolua_S,"delete",tolua_peak_peak_SoundSource_delete00);
    tolua_function(tolua_S,"getLength",tolua_peak_peak_SoundSource_getLength00);
    tolua_function(tolua_S,"isSeekable",tolua_peak_peak_SoundSource_isSeekable00);
    tolua_function(tolua_S,"isStreamed",tolua_peak_peak_SoundSource_isStreamed00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"SoundEngine","peak::SoundEngine","",tolua_collect_peak__SoundEngine);
   #else
   tolua_cclass(tolua_S,"SoundEngine","peak::SoundEngine","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"SoundEngine");
    tolua_function(tolua_S,"get",tolua_peak_peak_SoundEngine_get00);
    tolua_function(tolua_S,"delete",tolua_peak_peak_SoundEngine_delete00);
    tolua_function(tolua_S,"init",tolua_peak_peak_SoundEngine_init00);
    tolua_function(tolua_S,"shutdown",tolua_peak_peak_SoundEngine_shutdown00);
    tolua_function(tolua_S,"running",tolua_peak_peak_SoundEngine_running00);
    tolua_function(tolua_S,"play2D",tolua_peak_peak_SoundEngine_play2D00);
    tolua_function(tolua_S,"play2D",tolua_peak_peak_SoundEngine_play2D01);
    tolua_function(tolua_S,"play3D",tolua_peak_peak_SoundEngine_play3D00);
    tolua_function(tolua_S,"play3D",tolua_peak_peak_SoundEngine_play3D01);
    tolua_function(tolua_S,"getListener",tolua_peak_peak_SoundEngine_getListener00);
    tolua_function(tolua_S,"doWork",tolua_peak_peak_SoundEngine_doWork00);
    tolua_function(tolua_S,"removeSoundFromList",tolua_peak_peak_SoundEngine_removeSoundFromList00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"SoundFactory","peak::SoundFactory","",tolua_collect_peak__SoundFactory);
   #else
   tolua_cclass(tolua_S,"SoundFactory","peak::SoundFactory","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"SoundFactory");
    tolua_function(tolua_S,"get",tolua_peak_peak_SoundFactory_get00);
    tolua_function(tolua_S,"delete",tolua_peak_peak_SoundFactory_delete00);
    tolua_function(tolua_S,"getFile",tolua_peak_peak_SoundFactory_getFile00);
    tolua_function(tolua_S,"getHelloWorld",tolua_peak_peak_SoundFactory_getHelloWorld00);
    tolua_function(tolua_S,"removeSource",tolua_peak_peak_SoundFactory_removeSource00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_peak (lua_State* tolua_S) {
 return tolua_peak_open(tolua_S);
};
#endif

