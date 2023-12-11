/*
** Lua binding: peak
** Generated automatically.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_peak_open (lua_State* tolua_S);

#include "../include/core/Buffer.h"
#include "../include/core/Address.h"
#include "../include/core/Vector3D.h"
#include "../include/core/Vector2D.h"
#include "../include/core/Vector2DI.h"
#include "../include/core/Color.h"
#include "../include/core/Quaternion.h"
#include "../include/core/Script.h"
#include "../include/core/Game.h"
#include "../include/core/Logger.h"
#include "../include/core/InputManager.h"
#include "../include/core/Keycode.h"
#include "../include/core/FileSystem.h"
#include "../include/core/File.h"
#include "../include/core/Timer.h"
#include "../include/core/Util.h"
#include "../include/core/Level.h"
#include "../include/core/SettingsManager.h"
#include "../include/core/Entity.h"
#include "../include/core/Property.h"
#include "../include/core/Link.h"
#include "../include/core/DirectLink.h"
#include "../include/core/EventScriptLink.h"
#include "../include/core/EventRemoteLink.h"
#include "../include/core/Event.h"
#include "../include/core/PropertyScriptLink.h"
#include "../include/core/GameObject.h"
#include "../include/network/Connection.h"
#include "../include/network/BroadcastHost.h"
#include "../include/network/BroadcastClient.h"
#include "../include/graphics/SceneNode.h"
#include "../include/graphics/CameraSceneNode.h"
#include "../include/graphics/GraphicsEngine.h"
#include "../include/graphics/Image.h"
#include "../include/graphics/MeshSceneNode.h"
#include "../include/graphics/Overlay.h"
#include "../include/graphics/TextureOverlay.h"
#include "../include/graphics/LabelOverlay.h"
#include "../include/graphics/OverlayManager.h"
#include "../include/graphics/SceneNodeController.h"
#include "../include/graphics/FollowingController.h"
#include "../include/graphics/RotationController.h"
#include "../include/graphics/Light.h"
#include "../include/graphics/ParticleSystem.h"
#include "../include/graphics/Terrain.h"
#include "../include/physics/World.h"
#include "../include/physics/Body.h"
#include "../include/physics/Shape.h"
#include "../include/physics/Box.h"
#include "../include/physics/Plane.h"
#include "../include/physics/Trimesh.h"
#include "../include/physics/Sphere.h"
#include "../include/physics/Callback.h"
#include "../include/physics/RayCallback.h"
#include "../include/physics/Heightfield.h"
#include "../include/menu/MenuManager.h"
#include "../include/menu/Menu.h"
#include "../include/menu/MenuElement.h"
#include "../include/menu/ListBox.h"
#include "../include/sound/SoundEngine.h"
#include "../include/sound/Sound.h"
#include "../include/sound/Listener.h"
#include "../include/sound/SoundSource.h"
#include "../include/sound/SoundFactory.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_peak__EventRemoteLink (lua_State* tolua_S)
{
 peak::EventRemoteLink* self = (peak::EventRemoteLink*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__FileSystem (lua_State* tolua_S)
{
 peak::FileSystem* self = (peak::FileSystem*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_std__wstring (lua_State* tolua_S)
{
 std::wstring* self = (std::wstring*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__IntProperty (lua_State* tolua_S)
{
 peak::IntProperty* self = (peak::IntProperty*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__Body (lua_State* tolua_S)
{
 peak::Body* self = (peak::Body*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__Trimesh (lua_State* tolua_S)
{
 peak::Trimesh* self = (peak::Trimesh*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__Connection (lua_State* tolua_S)
{
 peak::Connection* self = (peak::Connection*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__Game (lua_State* tolua_S)
{
 peak::Game* self = (peak::Game*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__Property (lua_State* tolua_S)
{
 peak::Property* self = (peak::Property*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__CameraSceneNode (lua_State* tolua_S)
{
 peak::CameraSceneNode* self = (peak::CameraSceneNode*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__GameObject (lua_State* tolua_S)
{
 peak::GameObject* self = (peak::GameObject*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_std__vector_std__string_ (lua_State* tolua_S)
{
 std::vector<std::string>* self = (std::vector<std::string>*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__Vector3DProperty (lua_State* tolua_S)
{
 peak::Vector3DProperty* self = (peak::Vector3DProperty*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__Level (lua_State* tolua_S)
{
 peak::Level* self = (peak::Level*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__Heightfield (lua_State* tolua_S)
{
 peak::Heightfield* self = (peak::Heightfield*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__PointGroup (lua_State* tolua_S)
{
 peak::PointGroup* self = (peak::PointGroup*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__Entity (lua_State* tolua_S)
{
 peak::Entity* self = (peak::Entity*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__Event (lua_State* tolua_S)
{
 peak::Event* self = (peak::Event*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__EventData (lua_State* tolua_S)
{
 peak::EventData* self = (peak::EventData*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__Menu (lua_State* tolua_S)
{
 peak::Menu* self = (peak::Menu*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__SoundSource (lua_State* tolua_S)
{
 peak::SoundSource* self = (peak::SoundSource*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__DirectLink (lua_State* tolua_S)
{
 peak::DirectLink* self = (peak::DirectLink*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__OverlayManager (lua_State* tolua_S)
{
 peak::OverlayManager* self = (peak::OverlayManager*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__Buffer (lua_State* tolua_S)
{
 peak::Buffer* self = (peak::Buffer*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__Link (lua_State* tolua_S)
{
 peak::Link* self = (peak::Link*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__MeshSceneNode (lua_State* tolua_S)
{
 peak::MeshSceneNode* self = (peak::MeshSceneNode*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__Vector3D (lua_State* tolua_S)
{
 peak::Vector3D* self = (peak::Vector3D*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__Color (lua_State* tolua_S)
{
 peak::Color* self = (peak::Color*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__BroadcastHost (lua_State* tolua_S)
{
 peak::BroadcastHost* self = (peak::BroadcastHost*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__ParameterList (lua_State* tolua_S)
{
 peak::ParameterList* self = (peak::ParameterList*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__Vector2DI (lua_State* tolua_S)
{
 peak::Vector2DI* self = (peak::Vector2DI*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__Sphere (lua_State* tolua_S)
{
 peak::Sphere* self = (peak::Sphere*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__File (lua_State* tolua_S)
{
 peak::File* self = (peak::File*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__FollowingController (lua_State* tolua_S)
{
 peak::FollowingController* self = (peak::FollowingController*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__RayCallback (lua_State* tolua_S)
{
 peak::RayCallback* self = (peak::RayCallback*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__Quaternion (lua_State* tolua_S)
{
 peak::Quaternion* self = (peak::Quaternion*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__Image (lua_State* tolua_S)
{
 peak::Image* self = (peak::Image*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__SoundFactory (lua_State* tolua_S)
{
 peak::SoundFactory* self = (peak::SoundFactory*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__TextureOverlay (lua_State* tolua_S)
{
 peak::TextureOverlay* self = (peak::TextureOverlay*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__MenuElement (lua_State* tolua_S)
{
 peak::MenuElement* self = (peak::MenuElement*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__Listener (lua_State* tolua_S)
{
 peak::Listener* self = (peak::Listener*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__PropertyScriptLink (lua_State* tolua_S)
{
 peak::PropertyScriptLink* self = (peak::PropertyScriptLink*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__SoundEngine (lua_State* tolua_S)
{
 peak::SoundEngine* self = (peak::SoundEngine*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__Terrain (lua_State* tolua_S)
{
 peak::Terrain* self = (peak::Terrain*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__Overlay (lua_State* tolua_S)
{
 peak::Overlay* self = (peak::Overlay*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__Sound (lua_State* tolua_S)
{
 peak::Sound* self = (peak::Sound*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__ListBox (lua_State* tolua_S)
{
 peak::ListBox* self = (peak::ListBox*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__Vector2D (lua_State* tolua_S)
{
 peak::Vector2D* self = (peak::Vector2D*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__MenuManager (lua_State* tolua_S)
{
 peak::MenuManager* self = (peak::MenuManager*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__Box (lua_State* tolua_S)
{
 peak::Box* self = (peak::Box*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__LabelOverlay (lua_State* tolua_S)
{
 peak::LabelOverlay* self = (peak::LabelOverlay*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__Script (lua_State* tolua_S)
{
 peak::Script* self = (peak::Script*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__RotationController (lua_State* tolua_S)
{
 peak::RotationController* self = (peak::RotationController*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__Callback (lua_State* tolua_S)
{
 peak::Callback* self = (peak::Callback*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__EventScriptLink (lua_State* tolua_S)
{
 peak::EventScriptLink* self = (peak::EventScriptLink*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__Light (lua_State* tolua_S)
{
 peak::Light* self = (peak::Light*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__Shape (lua_State* tolua_S)
{
 peak::Shape* self = (peak::Shape*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_std__vector_float_ (lua_State* tolua_S)
{
 std::vector<float>* self = (std::vector<float>*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__SceneNode (lua_State* tolua_S)
{
 peak::SceneNode* self = (peak::SceneNode*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__SceneNodeController (lua_State* tolua_S)
{
 peak::SceneNodeController* self = (peak::SceneNodeController*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__Parameter (lua_State* tolua_S)
{
 peak::Parameter* self = (peak::Parameter*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__World (lua_State* tolua_S)
{
 peak::World* self = (peak::World*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__Address (lua_State* tolua_S)
{
 peak::Address* self = (peak::Address*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__Timer (lua_State* tolua_S)
{
 peak::Timer* self = (peak::Timer*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__Plane (lua_State* tolua_S)
{
 peak::Plane* self = (peak::Plane*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__Logger (lua_State* tolua_S)
{
 peak::Logger* self = (peak::Logger*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__CollisionInfo (lua_State* tolua_S)
{
 peak::CollisionInfo* self = (peak::CollisionInfo*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__ShapeData (lua_State* tolua_S)
{
 peak::ShapeData* self = (peak::ShapeData*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__ParticleSystem (lua_State* tolua_S)
{
 peak::ParticleSystem* self = (peak::ParticleSystem*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__BroadcastClient (lua_State* tolua_S)
{
 peak::BroadcastClient* self = (peak::BroadcastClient*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__QuaternionProperty (lua_State* tolua_S)
{
 peak::QuaternionProperty* self = (peak::QuaternionProperty*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__Vector2DProperty (lua_State* tolua_S)
{
 peak::Vector2DProperty* self = (peak::Vector2DProperty*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_peak__ClientInfo (lua_State* tolua_S)
{
 peak::ClientInfo* self = (peak::ClientInfo*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"peak::EventRemoteLink");
 tolua_usertype(tolua_S,"ReferenceCounted");
 tolua_usertype(tolua_S,"peak::FileSystem");
 tolua_usertype(tolua_S,"std::wstring");
 tolua_usertype(tolua_S,"peak::IntProperty");
 tolua_usertype(tolua_S,"peak::Body");
 tolua_usertype(tolua_S,"peak::Trimesh");
 tolua_usertype(tolua_S,"peak::Connection");
 tolua_usertype(tolua_S,"peak::Parameter");
 tolua_usertype(tolua_S,"peak::Property");
 tolua_usertype(tolua_S,"peak::CameraSceneNode");
 tolua_usertype(tolua_S,"peak::GameObject");
 tolua_usertype(tolua_S,"peak::InputManager");
 tolua_usertype(tolua_S,"std::vector<std::string>");
 tolua_usertype(tolua_S,"peak::TimerCallback");
 tolua_usertype(tolua_S,"peak::Vector3DProperty");
 tolua_usertype(tolua_S,"peak::Level");
 tolua_usertype(tolua_S,"peak::Heightfield");
 tolua_usertype(tolua_S,"peak::Address");
 tolua_usertype(tolua_S,"peak::Entity");
 tolua_usertype(tolua_S,"peak::Event");
 tolua_usertype(tolua_S,"peak::EventData");
 tolua_usertype(tolua_S,"peak::Menu");
 tolua_usertype(tolua_S,"peak::SoundSource");
 tolua_usertype(tolua_S,"peak::DirectLink");
 tolua_usertype(tolua_S,"peak::ImageLoader");
 tolua_usertype(tolua_S,"peak::OverlayManager");
 tolua_usertype(tolua_S,"peak::Buffer");
 tolua_usertype(tolua_S,"peak::Link");
 tolua_usertype(tolua_S,"peak::MeshSceneNode");
 tolua_usertype(tolua_S,"peak::ClientInfo");
 tolua_usertype(tolua_S,"peak::Color");
 tolua_usertype(tolua_S,"peak::BroadcastHost");
 tolua_usertype(tolua_S,"peak::ParameterList");
 tolua_usertype(tolua_S,"peak::Label");
 tolua_usertype(tolua_S,"peak::Vector2DI");
 tolua_usertype(tolua_S,"peak::Sphere");
 tolua_usertype(tolua_S,"peak::File");
 tolua_usertype(tolua_S,"peak::Shape");
 tolua_usertype(tolua_S,"peak::RayCallback");
 tolua_usertype(tolua_S,"peak::Quaternion");
 tolua_usertype(tolua_S,"gcn::Widget");
 tolua_usertype(tolua_S,"peak::SoundEngine");
 tolua_usertype(tolua_S,"peak::MenuManager");
 tolua_usertype(tolua_S,"peak::Sound");
 tolua_usertype(tolua_S,"peak::ListModel");
 tolua_usertype(tolua_S,"peak::Image");
 tolua_usertype(tolua_S,"peak::ListBoxListener");
 tolua_usertype(tolua_S,"peak::ListBox");
 tolua_usertype(tolua_S,"peak::Button");
 tolua_usertype(tolua_S,"peak::SoundFactory");
 tolua_usertype(tolua_S,"peak::SettingsManager");
 tolua_usertype(tolua_S,"peak::MenuElement");
 tolua_usertype(tolua_S,"peak::Listener");
 tolua_usertype(tolua_S,"peak::GUIInput");
 tolua_usertype(tolua_S,"peak::PropertyScriptLink");
 tolua_usertype(tolua_S,"peak::CollisionInfo");
 tolua_usertype(tolua_S,"peak::Callback");
 tolua_usertype(tolua_S,"peak::NetworkClient");
 tolua_usertype(tolua_S,"peak::Overlay");
 tolua_usertype(tolua_S,"peak::TextureOverlay");
 tolua_usertype(tolua_S,"peak::Logger");
 tolua_usertype(tolua_S,"peak::Vector2D");
 tolua_usertype(tolua_S,"peak::GraphicsEngine");
 tolua_usertype(tolua_S,"peak::Box");
 tolua_usertype(tolua_S,"peak::Light");
 tolua_usertype(tolua_S,"peak::Script");
 tolua_usertype(tolua_S,"peak::RotationController");
 tolua_usertype(tolua_S,"peak::FollowingController");
 tolua_usertype(tolua_S,"peak::NetworkHost");
 tolua_usertype(tolua_S,"peak::Terrain");
 tolua_usertype(tolua_S,"peak::LabelOverlay");
 tolua_usertype(tolua_S,"std::vector<float>");
 tolua_usertype(tolua_S,"peak::SceneNode");
 tolua_usertype(tolua_S,"peak::SceneNodeController");
 tolua_usertype(tolua_S,"ENetPeer");
 tolua_usertype(tolua_S,"peak::World");
 tolua_usertype(tolua_S,"peak::Game");
 tolua_usertype(tolua_S,"peak::Timer");
 tolua_usertype(tolua_S,"peak::Plane");
 tolua_usertype(tolua_S,"peak::Vector2DProperty");
 tolua_usertype(tolua_S,"peak::BroadcastClient");
 tolua_usertype(tolua_S,"peak::ShapeData");
 tolua_usertype(tolua_S,"peak::ParticleSystem");
 tolua_usertype(tolua_S,"peak::EventScriptLink");
 tolua_usertype(tolua_S,"peak::PointGroup");
 tolua_usertype(tolua_S,"peak::QuaternionProperty");
 tolua_usertype(tolua_S,"peak::Vector3D");
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
   peak::Buffer* tolua_ret = (peak::Buffer*)  Mtolua_new((peak::Buffer)());
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
   peak::Buffer* tolua_ret = (peak::Buffer*)  Mtolua_new((peak::Buffer)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Buffer");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
   peak::Buffer* tolua_ret = (peak::Buffer*)  Mtolua_new((peak::Buffer)(size));
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
   peak::Buffer* tolua_ret = (peak::Buffer*)  Mtolua_new((peak::Buffer)(size));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Buffer");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
   peak::Buffer* tolua_ret = (peak::Buffer*)  Mtolua_new((peak::Buffer)(data,size));
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
   peak::Buffer* tolua_ret = (peak::Buffer*)  Mtolua_new((peak::Buffer)(data,size));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Buffer");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const peak::Buffer",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const peak::Buffer* buf = ((const peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
  {
   peak::Buffer* tolua_ret = (peak::Buffer*)  Mtolua_new((peak::Buffer)(*buf));
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
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const peak::Buffer",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const peak::Buffer* buf = ((const peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
  {
   peak::Buffer* tolua_ret = (peak::Buffer*)  Mtolua_new((peak::Buffer)(*buf));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Buffer");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setSize'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSize'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDataSize'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPosition'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPosition'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readData'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeData'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeInt'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readInt'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeFloat'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readFloat'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeString'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readString'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeByte'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readByte'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeWord'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readWord'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeBool'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readBool'", NULL);
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
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector2D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Buffer* self = (peak::Buffer*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector2D v = *((peak::Vector2D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeVector2D'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readVector2D'", NULL);
#endif
  {
   peak::Vector2D tolua_ret = (peak::Vector2D)  self->readVector2D();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Vector2D)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector2D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector2D));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector2D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Buffer* self = (peak::Buffer*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D v = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeVector3D'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readVector3D'", NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->readVector3D();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Vector3D)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Quaternion",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Buffer* self = (peak::Buffer*)  tolua_tousertype(tolua_S,1,0);
  peak::Quaternion q = *((peak::Quaternion*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeQuaternion'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readQuaternion'", NULL);
#endif
  {
   peak::Quaternion tolua_ret = (peak::Quaternion)  self->readQuaternion();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Quaternion)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Quaternion");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Quaternion));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Quaternion");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getData'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clear'", NULL);
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
   peak::Address* tolua_ret = (peak::Address*)  Mtolua_new((peak::Address)());
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
   peak::Address* tolua_ret = (peak::Address*)  Mtolua_new((peak::Address)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Address");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const peak::Address",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const peak::Address* addr = ((const peak::Address*)  tolua_tousertype(tolua_S,2,0));
  {
   peak::Address* tolua_ret = (peak::Address*)  Mtolua_new((peak::Address)(*addr));
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
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const peak::Address",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const peak::Address* addr = ((const peak::Address*)  tolua_tousertype(tolua_S,2,0));
  {
   peak::Address* tolua_ret = (peak::Address*)  Mtolua_new((peak::Address)(*addr));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Address");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
   peak::Address* tolua_ret = (peak::Address*)  Mtolua_new((peak::Address)(addr));
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
   peak::Address* tolua_ret = (peak::Address*)  Mtolua_new((peak::Address)(addr));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Address");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
   peak::Address* tolua_ret = (peak::Address*)  Mtolua_new((peak::Address)(addr));
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
   peak::Address* tolua_ret = (peak::Address*)  Mtolua_new((peak::Address)(addr));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Address");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
   peak::Address* tolua_ret = (peak::Address*)  Mtolua_new((peak::Address)(ip1,ip2,ip3,ip4,port));
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
   peak::Address* tolua_ret = (peak::Address*)  Mtolua_new((peak::Address)(ip1,ip2,ip3,ip4,port));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Address");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAddress'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAddress'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAddress'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAddress'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAddressString'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPort'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPort'", NULL);
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
   peak::Vector3D* tolua_ret = (peak::Vector3D*)  Mtolua_new((peak::Vector3D)());
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
   peak::Vector3D* tolua_ret = (peak::Vector3D*)  Mtolua_new((peak::Vector3D)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
   peak::Vector3D* tolua_ret = (peak::Vector3D*)  Mtolua_new((peak::Vector3D)(x,y,z));
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
   peak::Vector3D* tolua_ret = (peak::Vector3D*)  Mtolua_new((peak::Vector3D)(x,y,z));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'rotateXZ'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'rotateYZ'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'rotateXY'", NULL);
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

/* method: rotate of class  peak::Vector3D */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Vector3D_rotate00
static int tolua_peak_peak_Vector3D_rotate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Vector3D",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Vector3D* self = (peak::Vector3D*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D rotation = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'rotate'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRotation'", NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getRotation();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Vector3D)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLengthSq'", NULL);
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
   peak::Vector2D* tolua_ret = (peak::Vector2D*)  Mtolua_new((peak::Vector2D)());
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
   peak::Vector2D* tolua_ret = (peak::Vector2D*)  Mtolua_new((peak::Vector2D)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Vector2D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
   peak::Vector2D* tolua_ret = (peak::Vector2D*)  Mtolua_new((peak::Vector2D)(x,y));
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
   peak::Vector2D* tolua_ret = (peak::Vector2D*)  Mtolua_new((peak::Vector2D)(x,y));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Vector2D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'rotate'", NULL);
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
   peak::Vector2DI* tolua_ret = (peak::Vector2DI*)  Mtolua_new((peak::Vector2DI)());
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
   peak::Vector2DI* tolua_ret = (peak::Vector2DI*)  Mtolua_new((peak::Vector2DI)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Vector2DI");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
   peak::Vector2DI* tolua_ret = (peak::Vector2DI*)  Mtolua_new((peak::Vector2DI)(x,y));
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
   peak::Vector2DI* tolua_ret = (peak::Vector2DI*)  Mtolua_new((peak::Vector2DI)(x,y));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Vector2DI");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
   peak::Color* tolua_ret = (peak::Color*)  Mtolua_new((peak::Color)());
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
   peak::Color* tolua_ret = (peak::Color*)  Mtolua_new((peak::Color)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Color");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
   peak::Color* tolua_ret = (peak::Color*)  Mtolua_new((peak::Color)(color));
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
   peak::Color* tolua_ret = (peak::Color*)  Mtolua_new((peak::Color)(color));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Color");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
   peak::Color* tolua_ret = (peak::Color*)  Mtolua_new((peak::Color)(r,g,b,a));
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
   peak::Color* tolua_ret = (peak::Color*)  Mtolua_new((peak::Color)(r,g,b,a));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Color");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRed'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getGreen'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getBlue'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAlpha'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRed'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setGreen'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setBlue'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAlpha'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get'", NULL);
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
   peak::Quaternion* tolua_ret = (peak::Quaternion*)  Mtolua_new((peak::Quaternion)());
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
   peak::Quaternion* tolua_ret = (peak::Quaternion*)  Mtolua_new((peak::Quaternion)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Quaternion");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
  float z = ((float)  tolua_tonumber(tolua_S,4,0));
  float w = ((float)  tolua_tonumber(tolua_S,5,0));
  {
   peak::Quaternion* tolua_ret = (peak::Quaternion*)  Mtolua_new((peak::Quaternion)(x,y,z,w));
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
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
  float z = ((float)  tolua_tonumber(tolua_S,4,0));
  float w = ((float)  tolua_tonumber(tolua_S,5,0));
  {
   peak::Quaternion* tolua_ret = (peak::Quaternion*)  Mtolua_new((peak::Quaternion)(x,y,z,w));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Quaternion");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
   peak::Quaternion* tolua_ret = (peak::Quaternion*)  Mtolua_new((peak::Quaternion)(x,y,z));
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
   peak::Quaternion* tolua_ret = (peak::Quaternion*)  Mtolua_new((peak::Quaternion)(x,y,z));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Quaternion");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::Vector3D euler = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
  {
   peak::Quaternion* tolua_ret = (peak::Quaternion*)  Mtolua_new((peak::Quaternion)(euler));
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
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::Vector3D euler = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
  {
   peak::Quaternion* tolua_ret = (peak::Quaternion*)  Mtolua_new((peak::Quaternion)(euler));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Quaternion");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
   peak::Quaternion* tolua_ret = (peak::Quaternion*)  Mtolua_new((peak::Quaternion)(&q));
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
   peak::Quaternion* tolua_ret = (peak::Quaternion*)  Mtolua_new((peak::Quaternion)(&q));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Quaternion");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
   tolua_pushnumber(tolua_S,(lua_Number)q);
  }
 }
 return 2;
tolua_lerror:
 return tolua_peak_peak_Quaternion_new03_local(tolua_S);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getEuler'", NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getEuler();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Vector3D)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Quaternion* self = (peak::Quaternion*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D vector = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'rotateVector'", NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->rotateVector(vector);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Vector3D)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Quaternion",0,&tolua_err)) ||
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'slerp'", NULL);
#endif
  {
   peak::Quaternion tolua_ret = (peak::Quaternion)  self->slerp(other,value);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Quaternion)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Quaternion");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Quaternion));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Quaternion");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Quaternion",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Quaternion* self = (peak::Quaternion*)  tolua_tousertype(tolua_S,1,0);
  peak::Quaternion* q2 = ((peak::Quaternion*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDifference'", NULL);
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

/* get function: x of class  peak::Quaternion */
#ifndef TOLUA_DISABLE_tolua_get_peak__Quaternion_x
static int tolua_get_peak__Quaternion_x(lua_State* tolua_S)
{
  peak::Quaternion* self = (peak::Quaternion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->x);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: x of class  peak::Quaternion */
#ifndef TOLUA_DISABLE_tolua_set_peak__Quaternion_x
static int tolua_set_peak__Quaternion_x(lua_State* tolua_S)
{
  peak::Quaternion* self = (peak::Quaternion*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: y of class  peak::Quaternion */
#ifndef TOLUA_DISABLE_tolua_get_peak__Quaternion_y
static int tolua_get_peak__Quaternion_y(lua_State* tolua_S)
{
  peak::Quaternion* self = (peak::Quaternion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->y);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: y of class  peak::Quaternion */
#ifndef TOLUA_DISABLE_tolua_set_peak__Quaternion_y
static int tolua_set_peak__Quaternion_y(lua_State* tolua_S)
{
  peak::Quaternion* self = (peak::Quaternion*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: z of class  peak::Quaternion */
#ifndef TOLUA_DISABLE_tolua_get_peak__Quaternion_z
static int tolua_get_peak__Quaternion_z(lua_State* tolua_S)
{
  peak::Quaternion* self = (peak::Quaternion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'z'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->z);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: z of class  peak::Quaternion */
#ifndef TOLUA_DISABLE_tolua_set_peak__Quaternion_z
static int tolua_set_peak__Quaternion_z(lua_State* tolua_S)
{
  peak::Quaternion* self = (peak::Quaternion*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: w of class  peak::Quaternion */
#ifndef TOLUA_DISABLE_tolua_get_peak__Quaternion_w
static int tolua_get_peak__Quaternion_w(lua_State* tolua_S)
{
  peak::Quaternion* self = (peak::Quaternion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'w'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->w);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: w of class  peak::Quaternion */
#ifndef TOLUA_DISABLE_tolua_set_peak__Quaternion_w
static int tolua_set_peak__Quaternion_w(lua_State* tolua_S)
{
  peak::Quaternion* self = (peak::Quaternion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'w'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->w = ((float)  tolua_tonumber(tolua_S,2,0))
;
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
   peak::Parameter* tolua_ret = (peak::Parameter*)  Mtolua_new((peak::Parameter)(value));
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
   peak::Parameter* tolua_ret = (peak::Parameter*)  Mtolua_new((peak::Parameter)(value));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Parameter");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
   peak::Parameter* tolua_ret = (peak::Parameter*)  Mtolua_new((peak::Parameter)(name,ptr));
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
   peak::Parameter* tolua_ret = (peak::Parameter*)  Mtolua_new((peak::Parameter)(name,ptr));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Parameter");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isPointer'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getValue'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getClassName'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPointer'", NULL);
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
   peak::ParameterList* tolua_ret = (peak::ParameterList*)  Mtolua_new((peak::ParameterList)());
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
   peak::ParameterList* tolua_ret = (peak::ParameterList*)  Mtolua_new((peak::ParameterList)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::ParameterList");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
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
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Parameter",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::ParameterList* self = (peak::ParameterList*)  tolua_tousertype(tolua_S,1,0);
  peak::Parameter* parameter = ((peak::Parameter*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'add'", NULL);
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
   peak::Script* tolua_ret = (peak::Script*)  Mtolua_new((peak::Script)());
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
   peak::Script* tolua_ret = (peak::Script*)  Mtolua_new((peak::Script)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Script");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'run'", NULL);
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

/* method: runString of class  peak::Script */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Script_runString00
static int tolua_peak_peak_Script_runString00(lua_State* tolua_S)
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
  std::string data = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'runString'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->runString(data);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'runString'.",&tolua_err);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'callFunction'", NULL);
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
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"peak::ParameterList",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::Script* self = (peak::Script*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  peak::ParameterList* parameters = ((peak::ParameterList*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'callFunction'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'callFunctionI'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'callFunctionII'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'callFunctionIII'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'callFunctionFFF'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'callIntFunction'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'callFloatFunction'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isFunction'", NULL);
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
   peak::World* tolua_ret = (peak::World*)  Mtolua_new((peak::World)());
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
   peak::World* tolua_ret = (peak::World*)  Mtolua_new((peak::World)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::World");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroy'", NULL);
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

/* method: setNetworkID of class  peak::World */
#ifndef TOLUA_DISABLE_tolua_peak_peak_World_setNetworkID00
static int tolua_peak_peak_World_setNetworkID00(lua_State* tolua_S)
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
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setNetworkID'", NULL);
#endif
  {
   self->setNetworkID(id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setNetworkID'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getNetworkID of class  peak::World */
#ifndef TOLUA_DISABLE_tolua_peak_peak_World_getNetworkID00
static int tolua_peak_peak_World_getNetworkID00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getNetworkID'", NULL);
#endif
  {
   int tolua_ret = (int)  self->getNetworkID();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getNetworkID'.",&tolua_err);
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
     !tolua_isusertable(tolua_S,1,"peak::World",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
  {
   peak::World* tolua_ret = (peak::World*)  peak::World::getWorld(id);
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

/* method: setSynchronized of class  peak::World */
#ifndef TOLUA_DISABLE_tolua_peak_peak_World_setSynchronized00
static int tolua_peak_peak_World_setSynchronized00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::World",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::World* self = (peak::World*)  tolua_tousertype(tolua_S,1,0);
  bool sync = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setSynchronized'", NULL);
#endif
  {
   self->setSynchronized(sync);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setSynchronized'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSynchronized of class  peak::World */
#ifndef TOLUA_DISABLE_tolua_peak_peak_World_getSynchronized00
static int tolua_peak_peak_World_getSynchronized00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSynchronized'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getSynchronized();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSynchronized'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addBody of class  peak::World */
#ifndef TOLUA_DISABLE_tolua_peak_peak_World_addBody00
static int tolua_peak_peak_World_addBody00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::World",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Body",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::World* self = (peak::World*)  tolua_tousertype(tolua_S,1,0);
  peak::Body* body = ((peak::Body*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addBody'", NULL);
#endif
  {
   self->addBody(body);
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

/* method: removeBody of class  peak::World */
#ifndef TOLUA_DISABLE_tolua_peak_peak_World_removeBody00
static int tolua_peak_peak_World_removeBody00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::World",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Body",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::World* self = (peak::World*)  tolua_tousertype(tolua_S,1,0);
  peak::Body* body = ((peak::Body*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeBody'", NULL);
#endif
  {
   self->removeBody(body);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeBody'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addSynchronizedBody of class  peak::World */
#ifndef TOLUA_DISABLE_tolua_peak_peak_World_addSynchronizedBody00
static int tolua_peak_peak_World_addSynchronizedBody00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::World",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Body",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::World* self = (peak::World*)  tolua_tousertype(tolua_S,1,0);
  peak::Body* body = ((peak::Body*)  tolua_tousertype(tolua_S,2,0));
  int id = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addSynchronizedBody'", NULL);
#endif
  {
   self->addSynchronizedBody(body,id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addSynchronizedBody'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: update of class  peak::World */
#ifndef TOLUA_DISABLE_tolua_peak_peak_World_update00
static int tolua_peak_peak_World_update00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::World",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::World* self = (peak::World*)  tolua_tousertype(tolua_S,1,0);
  peak::Buffer* data = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'update'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->update(data);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'update'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: sendUpdates of class  peak::World */
#ifndef TOLUA_DISABLE_tolua_peak_peak_World_sendUpdates00
static int tolua_peak_peak_World_sendUpdates00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'sendUpdates'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->sendUpdates(msecs);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sendUpdates'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getBodyNetworkID of class  peak::World */
#ifndef TOLUA_DISABLE_tolua_peak_peak_World_getBodyNetworkID00
static int tolua_peak_peak_World_getBodyNetworkID00(lua_State* tolua_S)
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
   int tolua_ret = (int)  peak::World::getBodyNetworkID();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getBodyNetworkID'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: castRay of class  peak::World */
#ifndef TOLUA_DISABLE_tolua_peak_peak_World_castRay00
static int tolua_peak_peak_World_castRay00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::World",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"peak::Vector3D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::World* self = (peak::World*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D rayfrom = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
  peak::Vector3D rayto = *((peak::Vector3D*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'castRay'", NULL);
#endif
  {
   peak::CollisionInfo tolua_ret = (peak::CollisionInfo)  self->castRay(rayfrom,rayto);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::CollisionInfo)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::CollisionInfo");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::CollisionInfo));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::CollisionInfo");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'castRay'.",&tolua_err);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'doWork'", NULL);
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
   peak::Entity* tolua_ret = (peak::Entity*)  Mtolua_new((peak::Entity)());
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
   peak::Entity* tolua_ret = (peak::Entity*)  Mtolua_new((peak::Entity)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Entity");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
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
     !tolua_isusertype(tolua_S,3,"peak::Buffer",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Entity* self = (peak::Entity*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  peak::Buffer* data = ((peak::Buffer*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'load'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->load(name,data);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setOwner'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getOwner'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setLocal'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isLocal'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setID'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getID'", NULL);
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

/* method: writeStartData of class  peak::Entity */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Entity_writeStartData00
static int tolua_peak_peak_Entity_writeStartData00(lua_State* tolua_S)
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
  peak::Buffer* data = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeStartData'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->writeStartData(data);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'writeStartData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setPosition of class  peak::Entity */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Entity_setPosition00
static int tolua_peak_peak_Entity_setPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Entity",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Entity* self = (peak::Entity*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D position = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPosition'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getName'", NULL);
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

/* method: getBody of class  peak::Entity */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Entity_getBody00
static int tolua_peak_peak_Entity_getBody00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Entity",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Entity* self = (peak::Entity*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getBody'", NULL);
#endif
  {
   peak::Body* tolua_ret = (peak::Body*)  self->getBody(name);
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

/* method: getSceneNode of class  peak::Entity */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Entity_getSceneNode00
static int tolua_peak_peak_Entity_getSceneNode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Entity",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Entity* self = (peak::Entity*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSceneNode'", NULL);
#endif
  {
   peak::SceneNode* tolua_ret = (peak::SceneNode*)  self->getSceneNode(name);
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

/* method: getScript of class  peak::Entity */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Entity_getScript00
static int tolua_peak_peak_Entity_getScript00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getScript'", NULL);
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
   peak::Connection* tolua_ret = (peak::Connection*)  Mtolua_new((peak::Connection)(peer));
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
   peak::Connection* tolua_ret = (peak::Connection*)  Mtolua_new((peak::Connection)(peer));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Connection");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroy'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'disconnect'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isConnected'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hasNewData'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readData'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'sendData'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'injectData'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPeer'", NULL);
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
   peak::BroadcastHost* tolua_ret = (peak::BroadcastHost*)  Mtolua_new((peak::BroadcastHost)());
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
   peak::BroadcastHost* tolua_ret = (peak::BroadcastHost*)  Mtolua_new((peak::BroadcastHost)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::BroadcastHost");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'shutdown'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setInfo'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getInfo'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'doWork'", NULL);
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

/* method: new of class  peak::ClientInfo */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ClientInfo_new00
static int tolua_peak_peak_ClientInfo_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::ClientInfo",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Connection",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"peak::Vector3D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Connection* conn = ((peak::Connection*)  tolua_tousertype(tolua_S,2,0));
  int owner = ((int)  tolua_tonumber(tolua_S,3,0));
  peak::Vector3D focus = *((peak::Vector3D*)  tolua_tousertype(tolua_S,4,0));
  {
   peak::ClientInfo* tolua_ret = (peak::ClientInfo*)  Mtolua_new((peak::ClientInfo)(conn,owner,focus));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::ClientInfo");
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

/* method: new_local of class  peak::ClientInfo */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ClientInfo_new00_local
static int tolua_peak_peak_ClientInfo_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::ClientInfo",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Connection",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"peak::Vector3D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Connection* conn = ((peak::Connection*)  tolua_tousertype(tolua_S,2,0));
  int owner = ((int)  tolua_tonumber(tolua_S,3,0));
  peak::Vector3D focus = *((peak::Vector3D*)  tolua_tousertype(tolua_S,4,0));
  {
   peak::ClientInfo* tolua_ret = (peak::ClientInfo*)  Mtolua_new((peak::ClientInfo)(conn,owner,focus));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::ClientInfo");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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

/* get function: conn of class  peak::ClientInfo */
#ifndef TOLUA_DISABLE_tolua_get_peak__ClientInfo_conn_ptr
static int tolua_get_peak__ClientInfo_conn_ptr(lua_State* tolua_S)
{
  peak::ClientInfo* self = (peak::ClientInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'conn'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)self->conn,"peak::Connection");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: conn of class  peak::ClientInfo */
#ifndef TOLUA_DISABLE_tolua_set_peak__ClientInfo_conn_ptr
static int tolua_set_peak__ClientInfo_conn_ptr(lua_State* tolua_S)
{
  peak::ClientInfo* self = (peak::ClientInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'conn'",NULL);
  if (!tolua_isusertype(tolua_S,2,"peak::Connection",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->conn = ((peak::Connection*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: owner of class  peak::ClientInfo */
#ifndef TOLUA_DISABLE_tolua_get_peak__ClientInfo_owner
static int tolua_get_peak__ClientInfo_owner(lua_State* tolua_S)
{
  peak::ClientInfo* self = (peak::ClientInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'owner'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->owner);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: owner of class  peak::ClientInfo */
#ifndef TOLUA_DISABLE_tolua_set_peak__ClientInfo_owner
static int tolua_set_peak__ClientInfo_owner(lua_State* tolua_S)
{
  peak::ClientInfo* self = (peak::ClientInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'owner'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->owner = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: focus of class  peak::ClientInfo */
#ifndef TOLUA_DISABLE_tolua_get_peak__ClientInfo_focus
static int tolua_get_peak__ClientInfo_focus(lua_State* tolua_S)
{
  peak::ClientInfo* self = (peak::ClientInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'focus'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->focus,"peak::Vector3D");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: focus of class  peak::ClientInfo */
#ifndef TOLUA_DISABLE_tolua_set_peak__ClientInfo_focus
static int tolua_set_peak__ClientInfo_focus(lua_State* tolua_S)
{
  peak::ClientInfo* self = (peak::ClientInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'focus'",NULL);
  if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err)))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->focus = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'", NULL);
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
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Address",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::Game* self = (peak::Game*)  tolua_tousertype(tolua_S,1,0);
  peak::Address addr = *((peak::Address*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'shutdown'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'stopGame'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isServer'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isClient'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setVisible'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isVisible'", NULL);
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

/* method: setFocus of class  peak::Game */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Game_setFocus00
static int tolua_peak_peak_Game_setFocus00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Game",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Game* self = (peak::Game*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D focus = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setFocus'", NULL);
#endif
  {
   self->setFocus(focus);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setFocus'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getFocus of class  peak::Game */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Game_getFocus00
static int tolua_peak_peak_Game_getFocus00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFocus'", NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getFocus();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Vector3D)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFocus'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getClientCount of class  peak::Game */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Game_getClientCount00
static int tolua_peak_peak_Game_getClientCount00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getClientCount'", NULL);
#endif
  {
   int tolua_ret = (int)  self->getClientCount();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getClientCount'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getClientFocus of class  peak::Game */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Game_getClientFocus00
static int tolua_peak_peak_Game_getClientFocus00(lua_State* tolua_S)
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
  int client = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getClientFocus'", NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getClientFocus(client);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Vector3D)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getClientFocus'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getClientConnection of class  peak::Game */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Game_getClientConnection00
static int tolua_peak_peak_Game_getClientConnection00(lua_State* tolua_S)
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
  int client = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getClientConnection'", NULL);
#endif
  {
   peak::Connection* tolua_ret = (peak::Connection*)  self->getClientConnection(client);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Connection");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getClientConnection'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getServerConnection of class  peak::Game */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Game_getServerConnection00
static int tolua_peak_peak_Game_getServerConnection00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getServerConnection'", NULL);
#endif
  {
   peak::Connection* tolua_ret = (peak::Connection*)  self->getServerConnection();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Connection");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getServerConnection'.",&tolua_err);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createEntity'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getEntity'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'deleteEntity'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getEntityCount'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getEntity'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWorld'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'doWork'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getServerScript'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getClientScript'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'log'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setFile'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFile'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clearFile'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setTimestamps'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setConsoleTimestamps'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getConsoleTimestamps'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setFileTimestamps'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFileTimestamps'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPrefix'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setConsolePrefix'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getConsolePrefix'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setFilePrefix'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFilePrefix'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMinimalConsoleLevel'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMinimalConsoleLevel'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMinimalFileLevel'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMinimalFileLevel'", NULL);
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

/* method: new of class  peak::Event */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Event_new00
static int tolua_peak_peak_Event_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Event",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Event* tolua_ret = (peak::Event*)  Mtolua_new((peak::Event)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Event");
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

/* method: new_local of class  peak::Event */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Event_new00_local
static int tolua_peak_peak_Event_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Event",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Event* tolua_ret = (peak::Event*)  Mtolua_new((peak::Event)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Event");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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

/* method: delete of class  peak::Event */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Event_delete00
static int tolua_peak_peak_Event_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Event",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Event* self = (peak::Event*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: trigger of class  peak::Event */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Event_trigger00
static int tolua_peak_peak_Event_trigger00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Event",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::EventData",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Event* self = (peak::Event*)  tolua_tousertype(tolua_S,1,0);
  peak::EventData* data = ((peak::EventData*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'trigger'", NULL);
#endif
  {
   self->trigger(data);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'trigger'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getData of class  peak::Event */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Event_getData00
static int tolua_peak_peak_Event_getData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Event",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Event* self = (peak::Event*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getData'", NULL);
#endif
  {
   peak::EventData* tolua_ret = (peak::EventData*)  self->getData();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::EventData");
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

/* method: addLink of class  peak::Event */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Event_addLink00
static int tolua_peak_peak_Event_addLink00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Event",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Link",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Event* self = (peak::Event*)  tolua_tousertype(tolua_S,1,0);
  peak::Link* link = ((peak::Link*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addLink'", NULL);
#endif
  {
   self->addLink(link);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addLink'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeLink of class  peak::Event */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Event_removeLink00
static int tolua_peak_peak_Event_removeLink00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Event",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Link",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Event* self = (peak::Event*)  tolua_tousertype(tolua_S,1,0);
  peak::Link* link = ((peak::Link*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeLink'", NULL);
#endif
  {
   self->removeLink(link);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeLink'.",&tolua_err);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'shutdown'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'keyboardEvent'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'mouseMoved'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setInputEnabled'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getInputEnabled'", NULL);
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

/* method: doWork of class  peak::InputManager */
#ifndef TOLUA_DISABLE_tolua_peak_peak_InputManager_doWork00
static int tolua_peak_peak_InputManager_doWork00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::InputManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::InputManager* self = (peak::InputManager*)  tolua_tousertype(tolua_S,1,0);
  float msecs = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'doWork'", NULL);
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
   peak::File* tolua_ret = (peak::File*)  Mtolua_new((peak::File)(filename,mode,binary));
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
   peak::File* tolua_ret = (peak::File*)  Mtolua_new((peak::File)(filename,mode,binary));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::File");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'read'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'read'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readLine'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'write'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'write'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'eof'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPosition'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPosition'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'movePosition'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'shutdown'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRoot'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addOverlay'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'openFile'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFileExists'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'listFiles'", NULL);
#endif
  {
   std::vector<std::string> tolua_ret = (std::vector<std::string>)  self->listFiles(directory,extension,recursive);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((std::vector<std::string>)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"std::vector<std::string>");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(std::vector<std::string>));
     tolua_pushusertype(tolua_S,tolua_obj,"std::vector<std::string>");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
   peak::Timer* tolua_ret = (peak::Timer*)  Mtolua_new((peak::Timer)());
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
   peak::Timer* tolua_ret = (peak::Timer*)  Mtolua_new((peak::Timer)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Timer");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'start'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'stop'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'reset'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setTime'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTime'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addCallback'", NULL);
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
    void* tolua_obj = Mtolua_new((peak::Vector3D)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
    void* tolua_obj = Mtolua_new((peak::Vector2D)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector2D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector2D));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector2D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
    void* tolua_obj = Mtolua_new((peak::Vector2DI)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector2DI");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector2DI));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector2DI");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
     !tolua_isboolean(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  bool b = ((bool)  tolua_toboolean(tolua_S,1,0));
  {
   std::string tolua_ret = (std::string)  peak::toString(b);
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
 return tolua_peak_peak_toString01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* function: peak::toString */
#ifndef TOLUA_DISABLE_tolua_peak_peak_toString03
static int tolua_peak_peak_toString03(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     (tolua_isvaluenil(tolua_S,1,&tolua_err) || !tolua_isusertype(tolua_S,1,"peak::Vector3D",0,&tolua_err)) ||
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
 return tolua_peak_peak_toString02(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* function: peak::toString */
#ifndef TOLUA_DISABLE_tolua_peak_peak_toString04
static int tolua_peak_peak_toString04(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     (tolua_isvaluenil(tolua_S,1,&tolua_err) || !tolua_isusertype(tolua_S,1,"peak::Vector2D",0,&tolua_err)) ||
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
 return tolua_peak_peak_toString03(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* function: peak::toString */
#ifndef TOLUA_DISABLE_tolua_peak_peak_toString05
static int tolua_peak_peak_toString05(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     (tolua_isvaluenil(tolua_S,1,&tolua_err) || !tolua_isusertype(tolua_S,1,"peak::Vector2DI",0,&tolua_err)) ||
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
 return tolua_peak_peak_toString04(tolua_S);
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
    void* tolua_obj = Mtolua_new((std::wstring)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"std::wstring");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(std::wstring));
     tolua_pushusertype(tolua_S,tolua_obj,"std::wstring");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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

/* function: peak::lowerCase */
#ifndef TOLUA_DISABLE_tolua_peak_peak_lowerCase00
static int tolua_peak_peak_lowerCase00(lua_State* tolua_S)
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
   std::string tolua_ret = (std::string)  peak::lowerCase(s);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'lowerCase'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: peak::equals */
#ifndef TOLUA_DISABLE_tolua_peak_peak_equals00
static int tolua_peak_peak_equals00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isuserdata(tolua_S,1,0,&tolua_err) ||
     !tolua_isuserdata(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  void* p1 = ((void*)  tolua_touserdata(tolua_S,1,0));
  void* p2 = ((void*)  tolua_touserdata(tolua_S,2,0));
  {
   bool tolua_ret = (bool)  peak::equals(p1,p2);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'equals'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: peak::correct */
#ifndef TOLUA_DISABLE_tolua_peak_peak_correct00
static int tolua_peak_peak_correct00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     (tolua_isvaluenil(tolua_S,1,&tolua_err) || !tolua_isusertype(tolua_S,1,"peak::Vector3D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Vector3D v = *((peak::Vector3D*)  tolua_tousertype(tolua_S,1,0));
  {
   peak::correct(v);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'correct'.",&tolua_err);
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
   peak::SceneNode* tolua_ret = (peak::SceneNode*)  Mtolua_new((peak::SceneNode)(parent));
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
   peak::SceneNode* tolua_ret = (peak::SceneNode*)  Mtolua_new((peak::SceneNode)(parent));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::SceneNode");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPosition'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRotation'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRotation'", NULL);
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
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::SceneNode* self = (peak::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D newpos = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPosition'", NULL);
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
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::SceneNode* self = (peak::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D newrot = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRotation'", NULL);
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
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Quaternion",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::SceneNode* self = (peak::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  peak::Quaternion newrot = *((peak::Quaternion*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRotation'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPosition'", NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getPosition();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Vector3D)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRotationEuler'", NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getRotationEuler();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Vector3D)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRotation'", NULL);
#endif
  {
   peak::Quaternion tolua_ret = (peak::Quaternion)  self->getRotation();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Quaternion)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Quaternion");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Quaternion));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Quaternion");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SceneNode* self = (peak::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D newscale = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setScale'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setParent'", NULL);
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

/* method: getParent of class  peak::SceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SceneNode_getParent00
static int tolua_peak_peak_SceneNode_getParent00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getParent'", NULL);
#endif
  {
   peak::SceneNode* tolua_ret = (peak::SceneNode*)  self->getParent();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::SceneNode");
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setVisible'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isVisible'", NULL);
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

/* method: getBoundingBox of class  peak::SceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_SceneNode_getBoundingBox00
static int tolua_peak_peak_SceneNode_getBoundingBox00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::SceneNode",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"peak::Vector3D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SceneNode* self = (peak::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D* min = ((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
  peak::Vector3D* max = ((peak::Vector3D*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getBoundingBox'", NULL);
#endif
  {
   self->getBoundingBox(*min,*max);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getBoundingBox'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Shape */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Shape_new00
static int tolua_peak_peak_Shape_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Shape",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Shape* tolua_ret = (peak::Shape*)  Mtolua_new((peak::Shape)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Shape");
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

/* method: new_local of class  peak::Shape */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Shape_new00_local
static int tolua_peak_peak_Shape_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Shape",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Shape* tolua_ret = (peak::Shape*)  Mtolua_new((peak::Shape)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Shape");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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

/* method: delete of class  peak::Shape */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Shape_delete00
static int tolua_peak_peak_Shape_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Shape",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Shape* self = (peak::Shape*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: destroy of class  peak::Shape */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Shape_destroy00
static int tolua_peak_peak_Shape_destroy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Shape",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Shape* self = (peak::Shape*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroy'", NULL);
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

/* method: getMass of class  peak::Shape */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Shape_getMass00
static int tolua_peak_peak_Shape_getMass00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Shape",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Shape* self = (peak::Shape*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMass'", NULL);
#endif
  {
   float tolua_ret = (float)  self->getMass();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMass'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInertia of class  peak::Shape */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Shape_getInertia00
static int tolua_peak_peak_Shape_getInertia00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Shape",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Shape* self = (peak::Shape*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getInertia'", NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getInertia();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Vector3D)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInertia'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getBoundingSphere of class  peak::Shape */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Shape_getBoundingSphere00
static int tolua_peak_peak_Shape_getBoundingSphere00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Shape",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"peak::Vector3D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Shape* self = (peak::Shape*)  tolua_tousertype(tolua_S,1,0);
  float radius = ((float)  tolua_tonumber(tolua_S,2,0));
  peak::Vector3D* center = ((peak::Vector3D*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getBoundingSphere'", NULL);
#endif
  {
   self->getBoundingSphere(radius,*center);
   tolua_pushnumber(tolua_S,(lua_Number)radius);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getBoundingSphere'.",&tolua_err);
 return 0;
#endif
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
   peak::Body* tolua_ret = (peak::Body*)  Mtolua_new((peak::Body)());
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
   peak::Body* tolua_ret = (peak::Body*)  Mtolua_new((peak::Body)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Body");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  peak::Body */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Body_create00
static int tolua_peak_peak_Body_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Body",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::World",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::Shape",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Body* self = (peak::Body*)  tolua_tousertype(tolua_S,1,0);
  peak::World* world = ((peak::World*)  tolua_tousertype(tolua_S,2,0));
  peak::Shape* shape = ((peak::Shape*)  tolua_tousertype(tolua_S,3,0));
  bool autodeactivation = ((bool)  tolua_toboolean(tolua_S,4,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'create'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->create(world,shape,autodeactivation);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroy'", NULL);
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
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Body* self = (peak::Body*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D position = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPosition'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPosition'", NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getPosition();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Vector3D)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Quaternion",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Body* self = (peak::Body*)  tolua_tousertype(tolua_S,1,0);
  peak::Quaternion rotation = *((peak::Quaternion*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRotation'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRotation'", NULL);
#endif
  {
   peak::Quaternion tolua_ret = (peak::Quaternion)  self->getRotation();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Quaternion)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Quaternion");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Quaternion));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Quaternion");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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

/* method: setForce of class  peak::Body */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Body_setForce00
static int tolua_peak_peak_Body_setForce00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Body",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Body* self = (peak::Body*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D force = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setForce'", NULL);
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

/* method: setForce of class  peak::Body */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Body_setForce01
static int tolua_peak_peak_Body_setForce01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Body",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"peak::Vector3D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::Body* self = (peak::Body*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D force = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
  peak::Vector3D relativeposition = *((peak::Vector3D*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setForce'", NULL);
#endif
  {
   self->setForce(force,relativeposition);
  }
 }
 return 0;
tolua_lerror:
 return tolua_peak_peak_Body_setForce00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: clearForces of class  peak::Body */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Body_clearForces00
static int tolua_peak_peak_Body_clearForces00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clearForces'", NULL);
#endif
  {
   self->clearForces();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clearForces'.",&tolua_err);
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
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Body* self = (peak::Body*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D linearvel = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setLinearVelocity'", NULL);
#endif
  {
   self->setLinearVelocity(linearvel);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLinearVelocity'", NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getLinearVelocity();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Vector3D)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Body* self = (peak::Body*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D angularvel = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAngularVelocity'", NULL);
#endif
  {
   self->setAngularVelocity(angularvel);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAngularVelocity'", NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getAngularVelocity();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Vector3D)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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

/* method: setGravity of class  peak::Body */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Body_setGravity00
static int tolua_peak_peak_Body_setGravity00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setGravity'", NULL);
#endif
  {
   self->setGravity();
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

/* method: setGravity of class  peak::Body */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Body_setGravity01
static int tolua_peak_peak_Body_setGravity01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Body",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::Body* self = (peak::Body*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D gravity = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setGravity'", NULL);
#endif
  {
   self->setGravity(gravity);
  }
 }
 return 0;
tolua_lerror:
 return tolua_peak_peak_Body_setGravity00(tolua_S);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getGravity'", NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getGravity();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Vector3D)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
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

/* method: setID of class  peak::Body */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Body_setID00
static int tolua_peak_peak_Body_setID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Body",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Body* self = (peak::Body*)  tolua_tousertype(tolua_S,1,0);
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setID'", NULL);
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

/* method: getID of class  peak::Body */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Body_getID00
static int tolua_peak_peak_Body_getID00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getID'", NULL);
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

/* method: getCollisionBody of class  peak::Body */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Body_getCollisionBody00
static int tolua_peak_peak_Body_getCollisionBody00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCollisionBody'", NULL);
#endif
  {
   peak::Body* tolua_ret = (peak::Body*)  self->getCollisionBody();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Body");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCollisionBody'.",&tolua_err);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setEntity'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getEntity'", NULL);
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

/* method: getShape of class  peak::Body */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Body_getShape00
static int tolua_peak_peak_Body_getShape00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getShape'", NULL);
#endif
  {
   peak::Shape* tolua_ret = (peak::Shape*)  self->getShape();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Shape");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getShape'.",&tolua_err);
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
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Terrain* tolua_ret = (peak::Terrain*)  Mtolua_new((peak::Terrain)());
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
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Terrain* tolua_ret = (peak::Terrain*)  Mtolua_new((peak::Terrain)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Terrain");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  peak::Terrain */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Terrain_create00
static int tolua_peak_peak_Terrain_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Terrain",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,5,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,6,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,7,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,8,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,9,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Terrain* self = (peak::Terrain*)  tolua_tousertype(tolua_S,1,0);
  std::string heightmap = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  bool physics = ((bool)  tolua_toboolean(tolua_S,3,0));
  std::string colormap = ((std::string)  tolua_tocppstring(tolua_S,4,0));
  std::string detail0 = ((std::string)  tolua_tocppstring(tolua_S,5,0));
  std::string detail1 = ((std::string)  tolua_tocppstring(tolua_S,6,0));
  std::string detail2 = ((std::string)  tolua_tocppstring(tolua_S,7,0));
  std::string detail3 = ((std::string)  tolua_tocppstring(tolua_S,8,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'create'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->create(heightmap,physics,colormap,detail0,detail1,detail2,detail3);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: destroy of class  peak::Terrain */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Terrain_destroy00
static int tolua_peak_peak_Terrain_destroy00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroy'", NULL);
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

/* method: new of class  peak::ShapeData */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ShapeData_new00
static int tolua_peak_peak_ShapeData_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::ShapeData",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Body",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::Shape",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Body* body = ((peak::Body*)  tolua_tousertype(tolua_S,2,0));
  peak::Shape* shape = ((peak::Shape*)  tolua_tousertype(tolua_S,3,0));
  {
   peak::ShapeData* tolua_ret = (peak::ShapeData*)  Mtolua_new((peak::ShapeData)(body,shape));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::ShapeData");
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

/* method: new_local of class  peak::ShapeData */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ShapeData_new00_local
static int tolua_peak_peak_ShapeData_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::ShapeData",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Body",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::Shape",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Body* body = ((peak::Body*)  tolua_tousertype(tolua_S,2,0));
  peak::Shape* shape = ((peak::Shape*)  tolua_tousertype(tolua_S,3,0));
  {
   peak::ShapeData* tolua_ret = (peak::ShapeData*)  Mtolua_new((peak::ShapeData)(body,shape));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::ShapeData");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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

/* get function: body of class  peak::ShapeData */
#ifndef TOLUA_DISABLE_tolua_get_peak__ShapeData_body_ptr
static int tolua_get_peak__ShapeData_body_ptr(lua_State* tolua_S)
{
  peak::ShapeData* self = (peak::ShapeData*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'body'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)self->body,"peak::Body");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: body of class  peak::ShapeData */
#ifndef TOLUA_DISABLE_tolua_set_peak__ShapeData_body_ptr
static int tolua_set_peak__ShapeData_body_ptr(lua_State* tolua_S)
{
  peak::ShapeData* self = (peak::ShapeData*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: shape of class  peak::ShapeData */
#ifndef TOLUA_DISABLE_tolua_get_peak__ShapeData_shape_ptr
static int tolua_get_peak__ShapeData_shape_ptr(lua_State* tolua_S)
{
  peak::ShapeData* self = (peak::ShapeData*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'shape'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)self->shape,"peak::Shape");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: shape of class  peak::ShapeData */
#ifndef TOLUA_DISABLE_tolua_set_peak__ShapeData_shape_ptr
static int tolua_set_peak__ShapeData_shape_ptr(lua_State* tolua_S)
{
  peak::ShapeData* self = (peak::ShapeData*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'shape'",NULL);
  if (!tolua_isusertype(tolua_S,2,"peak::Shape",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->shape = ((peak::Shape*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::PointGroup */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PointGroup_new00
static int tolua_peak_peak_PointGroup_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::PointGroup",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::PointGroup* tolua_ret = (peak::PointGroup*)  Mtolua_new((peak::PointGroup)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::PointGroup");
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

/* method: new_local of class  peak::PointGroup */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PointGroup_new00_local
static int tolua_peak_peak_PointGroup_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::PointGroup",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::PointGroup* tolua_ret = (peak::PointGroup*)  Mtolua_new((peak::PointGroup)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::PointGroup");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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

/* method: delete of class  peak::PointGroup */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PointGroup_delete00
static int tolua_peak_peak_PointGroup_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::PointGroup",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::PointGroup* self = (peak::PointGroup*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addPoint of class  peak::PointGroup */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PointGroup_addPoint00
static int tolua_peak_peak_PointGroup_addPoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::PointGroup",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::PointGroup* self = (peak::PointGroup*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D point = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addPoint'", NULL);
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

/* method: getPointCount of class  peak::PointGroup */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PointGroup_getPointCount00
static int tolua_peak_peak_PointGroup_getPointCount00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::PointGroup",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::PointGroup* self = (peak::PointGroup*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPointCount'", NULL);
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

/* method: getPoint of class  peak::PointGroup */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PointGroup_getPoint00
static int tolua_peak_peak_PointGroup_getPoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::PointGroup",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::PointGroup* self = (peak::PointGroup*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPoint'", NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getPoint(index);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Vector3D)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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

/* method: deletePoint of class  peak::PointGroup */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PointGroup_deletePoint00
static int tolua_peak_peak_PointGroup_deletePoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::PointGroup",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::PointGroup* self = (peak::PointGroup*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'deletePoint'", NULL);
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

/* method: addRotation of class  peak::PointGroup */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PointGroup_addRotation00
static int tolua_peak_peak_PointGroup_addRotation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::PointGroup",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::PointGroup* self = (peak::PointGroup*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D rotation = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addRotation'", NULL);
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

/* method: getRotationCount of class  peak::PointGroup */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PointGroup_getRotationCount00
static int tolua_peak_peak_PointGroup_getRotationCount00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::PointGroup",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::PointGroup* self = (peak::PointGroup*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRotationCount'", NULL);
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

/* method: getRotation of class  peak::PointGroup */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PointGroup_getRotation00
static int tolua_peak_peak_PointGroup_getRotation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::PointGroup",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::PointGroup* self = (peak::PointGroup*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRotation'", NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getRotation(index);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Vector3D)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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

/* method: deleteRotation of class  peak::PointGroup */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PointGroup_deleteRotation00
static int tolua_peak_peak_PointGroup_deleteRotation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::PointGroup",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::PointGroup* self = (peak::PointGroup*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'deleteRotation'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroy'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPointGroup'", NULL);
#endif
  {
   peak::PointGroup* tolua_ret = (peak::PointGroup*)  self->getPointGroup(name);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::PointGroup");
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

/* method: getScript of class  peak::Level */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Level_getScript00
static int tolua_peak_peak_Level_getScript00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getScript'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLevelCount'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLevelName'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'load'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'save'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getString'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getBool'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getInt'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWString'", NULL);
#endif
  {
   std::wstring tolua_ret = (std::wstring)  self->getWString(name);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((std::wstring)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"std::wstring");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(std::wstring));
     tolua_pushusertype(tolua_S,tolua_obj,"std::wstring");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVector2D'", NULL);
#endif
  {
   peak::Vector2D tolua_ret = (peak::Vector2D)  self->getVector2D(name);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Vector2D)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector2D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector2D));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector2D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVector2DI'", NULL);
#endif
  {
   peak::Vector2DI tolua_ret = (peak::Vector2DI)  self->getVector2DI(name);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Vector2DI)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector2DI");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector2DI));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector2DI");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVector3D'", NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getVector3D(name);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Vector3D)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setString'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setBool'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setInt'", NULL);
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
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"peak::Vector2D",0,&tolua_err)) ||
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setVector2D'", NULL);
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
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"peak::Vector2DI",0,&tolua_err)) ||
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setVector2DI'", NULL);
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
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"peak::Vector3D",0,&tolua_err)) ||
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setVector3D'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getItemList'", NULL);
#endif
  {
   std::vector<std::string> tolua_ret = (std::vector<std::string>)  self->getItemList(section);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((std::vector<std::string>)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"std::vector<std::string>");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(std::vector<std::string>));
     tolua_pushusertype(tolua_S,tolua_obj,"std::vector<std::string>");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'deleteSetting'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'doesExist'", NULL);
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

/* method: new of class  peak::Link */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Link_new00
static int tolua_peak_peak_Link_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Link",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Link* tolua_ret = (peak::Link*)  Mtolua_new((peak::Link)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Link");
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

/* method: new_local of class  peak::Link */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Link_new00_local
static int tolua_peak_peak_Link_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Link",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Link* tolua_ret = (peak::Link*)  Mtolua_new((peak::Link)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Link");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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

/* method: delete of class  peak::Link */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Link_delete00
static int tolua_peak_peak_Link_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Link",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Link* self = (peak::Link*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  peak::Link */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Link_create00
static int tolua_peak_peak_Link_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Link",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Link* self = (peak::Link*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'create'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->create();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: destroy of class  peak::Link */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Link_destroy00
static int tolua_peak_peak_Link_destroy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Link",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Link* self = (peak::Link*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroy'", NULL);
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

/* method: setActive of class  peak::Link */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Link_setActive00
static int tolua_peak_peak_Link_setActive00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Link",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Link* self = (peak::Link*)  tolua_tousertype(tolua_S,1,0);
  bool active = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setActive'", NULL);
#endif
  {
   self->setActive(active);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setActive'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isActive of class  peak::Link */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Link_isActive00
static int tolua_peak_peak_Link_isActive00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Link",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Link* self = (peak::Link*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isActive'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isActive();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isActive'.",&tolua_err);
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
   peak::Overlay* tolua_ret = (peak::Overlay*)  Mtolua_new((peak::Overlay)(parent));
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
   peak::Overlay* tolua_ret = (peak::Overlay*)  Mtolua_new((peak::Overlay)(parent));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Overlay");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setParent'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getParent'", NULL);
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
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector2D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Overlay* self = (peak::Overlay*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector2D position = *((peak::Vector2D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPosition'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPosition'", NULL);
#endif
  {
   peak::Vector2D tolua_ret = (peak::Vector2D)  self->getPosition();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Vector2D)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector2D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector2D));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector2D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector2D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Overlay* self = (peak::Overlay*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector2D size = *((peak::Vector2D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setSize'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSize'", NULL);
#endif
  {
   peak::Vector2D tolua_ret = (peak::Vector2D)  self->getSize();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Vector2D)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector2D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector2D));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector2D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRotation'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRotation'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setClipChildren'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getClipChildren'", NULL);
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
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector2D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Overlay* self = (peak::Overlay*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector2D position = *((peak::Vector2D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAbsolutePosition'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAbsolutePosition'", NULL);
#endif
  {
   peak::Vector2D tolua_ret = (peak::Vector2D)  self->getAbsolutePosition();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Vector2D)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector2D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector2D));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector2D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector2D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Overlay* self = (peak::Overlay*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector2D size = *((peak::Vector2D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAbsoluteSize'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAbsoluteSize'", NULL);
#endif
  {
   peak::Vector2D tolua_ret = (peak::Vector2D)  self->getAbsoluteSize();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Vector2D)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector2D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector2D));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector2D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAbsoluteRotation'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAbsoluteRotation'", NULL);
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

/* method: new of class  peak::GameObject */
#ifndef TOLUA_DISABLE_tolua_peak_peak_GameObject_new00
static int tolua_peak_peak_GameObject_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::GameObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::GameObject* tolua_ret = (peak::GameObject*)  Mtolua_new((peak::GameObject)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::GameObject");
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

/* method: new_local of class  peak::GameObject */
#ifndef TOLUA_DISABLE_tolua_peak_peak_GameObject_new00_local
static int tolua_peak_peak_GameObject_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::GameObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::GameObject* tolua_ret = (peak::GameObject*)  Mtolua_new((peak::GameObject)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::GameObject");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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

/* method: delete of class  peak::GameObject */
#ifndef TOLUA_DISABLE_tolua_peak_peak_GameObject_delete00
static int tolua_peak_peak_GameObject_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::GameObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::GameObject* self = (peak::GameObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getProperty of class  peak::GameObject */
#ifndef TOLUA_DISABLE_tolua_peak_peak_GameObject_getProperty00
static int tolua_peak_peak_GameObject_getProperty00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::GameObject",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::GameObject* self = (peak::GameObject*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getProperty'", NULL);
#endif
  {
   peak::Property* tolua_ret = (peak::Property*)  self->getProperty(name);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Property");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getProperty'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getEvent of class  peak::GameObject */
#ifndef TOLUA_DISABLE_tolua_peak_peak_GameObject_getEvent00
static int tolua_peak_peak_GameObject_getEvent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::GameObject",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::GameObject* self = (peak::GameObject*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getEvent'", NULL);
#endif
  {
   peak::Event* tolua_ret = (peak::Event*)  self->getEvent(name);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Event");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getEvent'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::Property */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Property_delete00
static int tolua_peak_peak_Property_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Property",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Property* self = (peak::Property*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setName of class  peak::Property */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Property_setName00
static int tolua_peak_peak_Property_setName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Property",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Property* self = (peak::Property*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setName'", NULL);
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

/* method: getName of class  peak::Property */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Property_getName00
static int tolua_peak_peak_Property_getName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Property",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Property* self = (peak::Property*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getName'", NULL);
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

/* method: getType of class  peak::Property */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Property_getType00
static int tolua_peak_peak_Property_getType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Property",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Property* self = (peak::Property*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getType'", NULL);
#endif
  {
   peak::PropertyType tolua_ret = (peak::PropertyType)  self->getType();
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

/* method: set of class  peak::Property */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Property_set00
static int tolua_peak_peak_Property_set00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Property",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Property* self = (peak::Property*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D v = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'", NULL);
#endif
  {
   self->set(v);
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

/* method: getVector3D of class  peak::Property */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Property_getVector3D00
static int tolua_peak_peak_Property_getVector3D00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Property",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Property* self = (peak::Property*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVector3D'", NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getVector3D();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Vector3D)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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

/* method: set of class  peak::Property */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Property_set01
static int tolua_peak_peak_Property_set01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Property",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector2D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::Property* self = (peak::Property*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector2D v = *((peak::Vector2D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'", NULL);
#endif
  {
   self->set(v);
  }
 }
 return 0;
tolua_lerror:
 return tolua_peak_peak_Property_set00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVector2D of class  peak::Property */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Property_getVector2D00
static int tolua_peak_peak_Property_getVector2D00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Property",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Property* self = (peak::Property*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVector2D'", NULL);
#endif
  {
   peak::Vector2D tolua_ret = (peak::Vector2D)  self->getVector2D();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Vector2D)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector2D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector2D));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector2D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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

/* method: set of class  peak::Property */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Property_set02
static int tolua_peak_peak_Property_set02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Property",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Quaternion",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::Property* self = (peak::Property*)  tolua_tousertype(tolua_S,1,0);
  peak::Quaternion q = *((peak::Quaternion*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'", NULL);
#endif
  {
   self->set(q);
  }
 }
 return 0;
tolua_lerror:
 return tolua_peak_peak_Property_set01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getQuaternion of class  peak::Property */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Property_getQuaternion00
static int tolua_peak_peak_Property_getQuaternion00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Property",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Property* self = (peak::Property*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getQuaternion'", NULL);
#endif
  {
   peak::Quaternion tolua_ret = (peak::Quaternion)  self->getQuaternion();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Quaternion)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Quaternion");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Quaternion));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Quaternion");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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

/* method: set of class  peak::Property */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Property_set03
static int tolua_peak_peak_Property_set03(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Property",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::Property* self = (peak::Property*)  tolua_tousertype(tolua_S,1,0);
  bool b = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'", NULL);
#endif
  {
   self->set(b);
  }
 }
 return 0;
tolua_lerror:
 return tolua_peak_peak_Property_set02(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getBool of class  peak::Property */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Property_getBool00
static int tolua_peak_peak_Property_getBool00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Property",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Property* self = (peak::Property*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getBool'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getBool();
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

/* method: set of class  peak::Property */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Property_set04
static int tolua_peak_peak_Property_set04(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Property",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::Property* self = (peak::Property*)  tolua_tousertype(tolua_S,1,0);
  int i = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'", NULL);
#endif
  {
   self->set(i);
  }
 }
 return 0;
tolua_lerror:
 return tolua_peak_peak_Property_set03(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInt of class  peak::Property */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Property_getInt00
static int tolua_peak_peak_Property_getInt00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Property",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Property* self = (peak::Property*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getInt'", NULL);
#endif
  {
   int tolua_ret = (int)  self->getInt();
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

/* method: set of class  peak::Property */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Property_set05
static int tolua_peak_peak_Property_set05(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Property",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::Property* self = (peak::Property*)  tolua_tousertype(tolua_S,1,0);
  float f = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'", NULL);
#endif
  {
   self->set(f);
  }
 }
 return 0;
tolua_lerror:
 return tolua_peak_peak_Property_set04(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getFloat of class  peak::Property */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Property_getFloat00
static int tolua_peak_peak_Property_getFloat00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Property",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Property* self = (peak::Property*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFloat'", NULL);
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

/* method: set of class  peak::Property */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Property_set06
static int tolua_peak_peak_Property_set06(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Property",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"std::vector<float>",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::Property* self = (peak::Property*)  tolua_tousertype(tolua_S,1,0);
  std::string members = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  std::vector<float> value = *((std::vector<float>*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'", NULL);
#endif
  {
   self->set(members,value);
  }
 }
 return 0;
tolua_lerror:
 return tolua_peak_peak_Property_set05(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getValueArray of class  peak::Property */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Property_getValueArray00
static int tolua_peak_peak_Property_getValueArray00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Property",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Property* self = (peak::Property*)  tolua_tousertype(tolua_S,1,0);
  std::string members = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getValueArray'", NULL);
#endif
  {
   std::vector<float> tolua_ret = (std::vector<float>)  self->getValueArray(members);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((std::vector<float>)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"std::vector<float>");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(std::vector<float>));
     tolua_pushusertype(tolua_S,tolua_obj,"std::vector<float>");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getValueArray'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: set of class  peak::Property */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Property_set07
static int tolua_peak_peak_Property_set07(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Property",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Property",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::Property* self = (peak::Property*)  tolua_tousertype(tolua_S,1,0);
  peak::Property* p = ((peak::Property*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'", NULL);
#endif
  {
   self->set(p);
  }
 }
 return 0;
tolua_lerror:
 return tolua_peak_peak_Property_set06(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: serialize of class  peak::Property */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Property_serialize00
static int tolua_peak_peak_Property_serialize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Property",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Property* self = (peak::Property*)  tolua_tousertype(tolua_S,1,0);
  peak::Buffer* buffer = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'serialize'", NULL);
#endif
  {
   self->serialize(buffer);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'serialize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: deserialize of class  peak::Property */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Property_deserialize00
static int tolua_peak_peak_Property_deserialize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Property",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Property* self = (peak::Property*)  tolua_tousertype(tolua_S,1,0);
  peak::Buffer* buffer = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'deserialize'", NULL);
#endif
  {
   self->deserialize(buffer);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'deserialize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setGameObject of class  peak::Property */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Property_setGameObject00
static int tolua_peak_peak_Property_setGameObject00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Property",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::GameObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Property* self = (peak::Property*)  tolua_tousertype(tolua_S,1,0);
  peak::GameObject* object = ((peak::GameObject*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setGameObject'", NULL);
#endif
  {
   self->setGameObject(object);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setGameObject'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getGameObject of class  peak::Property */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Property_getGameObject00
static int tolua_peak_peak_Property_getGameObject00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Property",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Property* self = (peak::Property*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getGameObject'", NULL);
#endif
  {
   peak::GameObject* tolua_ret = (peak::GameObject*)  self->getGameObject();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::GameObject");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getGameObject'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addLink of class  peak::Property */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Property_addLink00
static int tolua_peak_peak_Property_addLink00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Property",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Link",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Property* self = (peak::Property*)  tolua_tousertype(tolua_S,1,0);
  peak::Link* link = ((peak::Link*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addLink'", NULL);
#endif
  {
   self->addLink(link);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addLink'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeLink of class  peak::Property */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Property_removeLink00
static int tolua_peak_peak_Property_removeLink00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Property",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Link",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Property* self = (peak::Property*)  tolua_tousertype(tolua_S,1,0);
  peak::Link* link = ((peak::Link*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeLink'", NULL);
#endif
  {
   self->removeLink(link);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeLink'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: update of class  peak::Property */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Property_update00
static int tolua_peak_peak_Property_update00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Property",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Property* self = (peak::Property*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'update'", NULL);
#endif
  {
   self->update();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'update'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: disableUpdates of class  peak::Property */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Property_disableUpdates00
static int tolua_peak_peak_Property_disableUpdates00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Property",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Property* self = (peak::Property*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'disableUpdates'", NULL);
#endif
  {
   self->disableUpdates();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'disableUpdates'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: enableUpdates of class  peak::Property */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Property_enableUpdates00
static int tolua_peak_peak_Property_enableUpdates00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Property",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Property* self = (peak::Property*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'enableUpdates'", NULL);
#endif
  {
   self->enableUpdates();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'enableUpdates'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Vector3DProperty */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Vector3DProperty_new00
static int tolua_peak_peak_Vector3DProperty_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Vector3DProperty",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,""));
  {
   peak::Vector3DProperty* tolua_ret = (peak::Vector3DProperty*)  Mtolua_new((peak::Vector3DProperty)(name));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Vector3DProperty");
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

/* method: new_local of class  peak::Vector3DProperty */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Vector3DProperty_new00_local
static int tolua_peak_peak_Vector3DProperty_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Vector3DProperty",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,""));
  {
   peak::Vector3DProperty* tolua_ret = (peak::Vector3DProperty*)  Mtolua_new((peak::Vector3DProperty)(name));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Vector3DProperty");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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

/* method: getType of class  peak::Vector3DProperty */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Vector3DProperty_getType00
static int tolua_peak_peak_Vector3DProperty_getType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Vector3DProperty",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Vector3DProperty* self = (peak::Vector3DProperty*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getType'", NULL);
#endif
  {
   peak::PropertyType tolua_ret = (peak::PropertyType)  self->getType();
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

/* method: set of class  peak::Vector3DProperty */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Vector3DProperty_set00
static int tolua_peak_peak_Vector3DProperty_set00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Vector3DProperty",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Vector3DProperty* self = (peak::Vector3DProperty*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D v = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'", NULL);
#endif
  {
   self->set(v);
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

/* method: getVector3D of class  peak::Vector3DProperty */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Vector3DProperty_getVector3D00
static int tolua_peak_peak_Vector3DProperty_getVector3D00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Vector3DProperty",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Vector3DProperty* self = (peak::Vector3DProperty*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVector3D'", NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getVector3D();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Vector3D)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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

/* method: set of class  peak::Vector3DProperty */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Vector3DProperty_set01
static int tolua_peak_peak_Vector3DProperty_set01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Vector3DProperty",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"std::vector<float>",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::Vector3DProperty* self = (peak::Vector3DProperty*)  tolua_tousertype(tolua_S,1,0);
  std::string members = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  std::vector<float> value = *((std::vector<float>*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'", NULL);
#endif
  {
   self->set(members,value);
  }
 }
 return 0;
tolua_lerror:
 return tolua_peak_peak_Vector3DProperty_set00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getValueArray of class  peak::Vector3DProperty */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Vector3DProperty_getValueArray00
static int tolua_peak_peak_Vector3DProperty_getValueArray00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Vector3DProperty",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Vector3DProperty* self = (peak::Vector3DProperty*)  tolua_tousertype(tolua_S,1,0);
  std::string members = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getValueArray'", NULL);
#endif
  {
   std::vector<float> tolua_ret = (std::vector<float>)  self->getValueArray(members);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((std::vector<float>)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"std::vector<float>");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(std::vector<float>));
     tolua_pushusertype(tolua_S,tolua_obj,"std::vector<float>");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getValueArray'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: set of class  peak::Vector3DProperty */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Vector3DProperty_set02
static int tolua_peak_peak_Vector3DProperty_set02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Vector3DProperty",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Property",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::Vector3DProperty* self = (peak::Vector3DProperty*)  tolua_tousertype(tolua_S,1,0);
  peak::Property* p = ((peak::Property*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'", NULL);
#endif
  {
   self->set(p);
  }
 }
 return 0;
tolua_lerror:
 return tolua_peak_peak_Vector3DProperty_set01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: serialize of class  peak::Vector3DProperty */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Vector3DProperty_serialize00
static int tolua_peak_peak_Vector3DProperty_serialize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Vector3DProperty",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Vector3DProperty* self = (peak::Vector3DProperty*)  tolua_tousertype(tolua_S,1,0);
  peak::Buffer* buffer = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'serialize'", NULL);
#endif
  {
   self->serialize(buffer);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'serialize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: deserialize of class  peak::Vector3DProperty */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Vector3DProperty_deserialize00
static int tolua_peak_peak_Vector3DProperty_deserialize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Vector3DProperty",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Vector3DProperty* self = (peak::Vector3DProperty*)  tolua_tousertype(tolua_S,1,0);
  peak::Buffer* buffer = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'deserialize'", NULL);
#endif
  {
   self->deserialize(buffer);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'deserialize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Vector2DProperty */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Vector2DProperty_new00
static int tolua_peak_peak_Vector2DProperty_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Vector2DProperty",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,""));
  {
   peak::Vector2DProperty* tolua_ret = (peak::Vector2DProperty*)  Mtolua_new((peak::Vector2DProperty)(name));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Vector2DProperty");
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

/* method: new_local of class  peak::Vector2DProperty */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Vector2DProperty_new00_local
static int tolua_peak_peak_Vector2DProperty_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Vector2DProperty",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,""));
  {
   peak::Vector2DProperty* tolua_ret = (peak::Vector2DProperty*)  Mtolua_new((peak::Vector2DProperty)(name));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Vector2DProperty");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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

/* method: getType of class  peak::Vector2DProperty */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Vector2DProperty_getType00
static int tolua_peak_peak_Vector2DProperty_getType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Vector2DProperty",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Vector2DProperty* self = (peak::Vector2DProperty*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getType'", NULL);
#endif
  {
   peak::PropertyType tolua_ret = (peak::PropertyType)  self->getType();
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

/* method: set of class  peak::Vector2DProperty */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Vector2DProperty_set00
static int tolua_peak_peak_Vector2DProperty_set00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Vector2DProperty",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector2D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Vector2DProperty* self = (peak::Vector2DProperty*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector2D v = *((peak::Vector2D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'", NULL);
#endif
  {
   self->set(v);
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

/* method: getVector2D of class  peak::Vector2DProperty */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Vector2DProperty_getVector2D00
static int tolua_peak_peak_Vector2DProperty_getVector2D00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Vector2DProperty",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Vector2DProperty* self = (peak::Vector2DProperty*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVector2D'", NULL);
#endif
  {
   peak::Vector2D tolua_ret = (peak::Vector2D)  self->getVector2D();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Vector2D)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector2D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector2D));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector2D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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

/* method: set of class  peak::Vector2DProperty */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Vector2DProperty_set01
static int tolua_peak_peak_Vector2DProperty_set01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Vector2DProperty",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"std::vector<float>",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::Vector2DProperty* self = (peak::Vector2DProperty*)  tolua_tousertype(tolua_S,1,0);
  std::string members = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  std::vector<float> value = *((std::vector<float>*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'", NULL);
#endif
  {
   self->set(members,value);
  }
 }
 return 0;
tolua_lerror:
 return tolua_peak_peak_Vector2DProperty_set00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getValueArray of class  peak::Vector2DProperty */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Vector2DProperty_getValueArray00
static int tolua_peak_peak_Vector2DProperty_getValueArray00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Vector2DProperty",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Vector2DProperty* self = (peak::Vector2DProperty*)  tolua_tousertype(tolua_S,1,0);
  std::string members = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getValueArray'", NULL);
#endif
  {
   std::vector<float> tolua_ret = (std::vector<float>)  self->getValueArray(members);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((std::vector<float>)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"std::vector<float>");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(std::vector<float>));
     tolua_pushusertype(tolua_S,tolua_obj,"std::vector<float>");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getValueArray'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: set of class  peak::Vector2DProperty */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Vector2DProperty_set02
static int tolua_peak_peak_Vector2DProperty_set02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Vector2DProperty",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Property",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::Vector2DProperty* self = (peak::Vector2DProperty*)  tolua_tousertype(tolua_S,1,0);
  peak::Property* p = ((peak::Property*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'", NULL);
#endif
  {
   self->set(p);
  }
 }
 return 0;
tolua_lerror:
 return tolua_peak_peak_Vector2DProperty_set01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: serialize of class  peak::Vector2DProperty */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Vector2DProperty_serialize00
static int tolua_peak_peak_Vector2DProperty_serialize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Vector2DProperty",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Vector2DProperty* self = (peak::Vector2DProperty*)  tolua_tousertype(tolua_S,1,0);
  peak::Buffer* buffer = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'serialize'", NULL);
#endif
  {
   self->serialize(buffer);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'serialize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: deserialize of class  peak::Vector2DProperty */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Vector2DProperty_deserialize00
static int tolua_peak_peak_Vector2DProperty_deserialize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Vector2DProperty",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Vector2DProperty* self = (peak::Vector2DProperty*)  tolua_tousertype(tolua_S,1,0);
  peak::Buffer* buffer = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'deserialize'", NULL);
#endif
  {
   self->deserialize(buffer);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'deserialize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::QuaternionProperty */
#ifndef TOLUA_DISABLE_tolua_peak_peak_QuaternionProperty_new00
static int tolua_peak_peak_QuaternionProperty_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::QuaternionProperty",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,""));
  {
   peak::QuaternionProperty* tolua_ret = (peak::QuaternionProperty*)  Mtolua_new((peak::QuaternionProperty)(name));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::QuaternionProperty");
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

/* method: new_local of class  peak::QuaternionProperty */
#ifndef TOLUA_DISABLE_tolua_peak_peak_QuaternionProperty_new00_local
static int tolua_peak_peak_QuaternionProperty_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::QuaternionProperty",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,""));
  {
   peak::QuaternionProperty* tolua_ret = (peak::QuaternionProperty*)  Mtolua_new((peak::QuaternionProperty)(name));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::QuaternionProperty");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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

/* method: getType of class  peak::QuaternionProperty */
#ifndef TOLUA_DISABLE_tolua_peak_peak_QuaternionProperty_getType00
static int tolua_peak_peak_QuaternionProperty_getType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::QuaternionProperty",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::QuaternionProperty* self = (peak::QuaternionProperty*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getType'", NULL);
#endif
  {
   peak::PropertyType tolua_ret = (peak::PropertyType)  self->getType();
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

/* method: set of class  peak::QuaternionProperty */
#ifndef TOLUA_DISABLE_tolua_peak_peak_QuaternionProperty_set00
static int tolua_peak_peak_QuaternionProperty_set00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::QuaternionProperty",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Quaternion",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::QuaternionProperty* self = (peak::QuaternionProperty*)  tolua_tousertype(tolua_S,1,0);
  peak::Quaternion q = *((peak::Quaternion*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'", NULL);
#endif
  {
   self->set(q);
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

/* method: getQuaternion of class  peak::QuaternionProperty */
#ifndef TOLUA_DISABLE_tolua_peak_peak_QuaternionProperty_getQuaternion00
static int tolua_peak_peak_QuaternionProperty_getQuaternion00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::QuaternionProperty",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::QuaternionProperty* self = (peak::QuaternionProperty*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getQuaternion'", NULL);
#endif
  {
   peak::Quaternion tolua_ret = (peak::Quaternion)  self->getQuaternion();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Quaternion)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Quaternion");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Quaternion));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Quaternion");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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

/* method: set of class  peak::QuaternionProperty */
#ifndef TOLUA_DISABLE_tolua_peak_peak_QuaternionProperty_set01
static int tolua_peak_peak_QuaternionProperty_set01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::QuaternionProperty",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"std::vector<float>",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::QuaternionProperty* self = (peak::QuaternionProperty*)  tolua_tousertype(tolua_S,1,0);
  std::string members = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  std::vector<float> value = *((std::vector<float>*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'", NULL);
#endif
  {
   self->set(members,value);
  }
 }
 return 0;
tolua_lerror:
 return tolua_peak_peak_QuaternionProperty_set00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getValueArray of class  peak::QuaternionProperty */
#ifndef TOLUA_DISABLE_tolua_peak_peak_QuaternionProperty_getValueArray00
static int tolua_peak_peak_QuaternionProperty_getValueArray00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::QuaternionProperty",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::QuaternionProperty* self = (peak::QuaternionProperty*)  tolua_tousertype(tolua_S,1,0);
  std::string members = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getValueArray'", NULL);
#endif
  {
   std::vector<float> tolua_ret = (std::vector<float>)  self->getValueArray(members);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((std::vector<float>)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"std::vector<float>");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(std::vector<float>));
     tolua_pushusertype(tolua_S,tolua_obj,"std::vector<float>");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getValueArray'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: set of class  peak::QuaternionProperty */
#ifndef TOLUA_DISABLE_tolua_peak_peak_QuaternionProperty_set02
static int tolua_peak_peak_QuaternionProperty_set02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::QuaternionProperty",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Property",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::QuaternionProperty* self = (peak::QuaternionProperty*)  tolua_tousertype(tolua_S,1,0);
  peak::Property* p = ((peak::Property*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'", NULL);
#endif
  {
   self->set(p);
  }
 }
 return 0;
tolua_lerror:
 return tolua_peak_peak_QuaternionProperty_set01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: serialize of class  peak::QuaternionProperty */
#ifndef TOLUA_DISABLE_tolua_peak_peak_QuaternionProperty_serialize00
static int tolua_peak_peak_QuaternionProperty_serialize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::QuaternionProperty",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::QuaternionProperty* self = (peak::QuaternionProperty*)  tolua_tousertype(tolua_S,1,0);
  peak::Buffer* buffer = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'serialize'", NULL);
#endif
  {
   self->serialize(buffer);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'serialize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: deserialize of class  peak::QuaternionProperty */
#ifndef TOLUA_DISABLE_tolua_peak_peak_QuaternionProperty_deserialize00
static int tolua_peak_peak_QuaternionProperty_deserialize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::QuaternionProperty",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::QuaternionProperty* self = (peak::QuaternionProperty*)  tolua_tousertype(tolua_S,1,0);
  peak::Buffer* buffer = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'deserialize'", NULL);
#endif
  {
   self->deserialize(buffer);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'deserialize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::IntProperty */
#ifndef TOLUA_DISABLE_tolua_peak_peak_IntProperty_new00
static int tolua_peak_peak_IntProperty_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::IntProperty",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,""));
  {
   peak::IntProperty* tolua_ret = (peak::IntProperty*)  Mtolua_new((peak::IntProperty)(name));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::IntProperty");
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

/* method: new_local of class  peak::IntProperty */
#ifndef TOLUA_DISABLE_tolua_peak_peak_IntProperty_new00_local
static int tolua_peak_peak_IntProperty_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::IntProperty",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,""));
  {
   peak::IntProperty* tolua_ret = (peak::IntProperty*)  Mtolua_new((peak::IntProperty)(name));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::IntProperty");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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

/* method: getType of class  peak::IntProperty */
#ifndef TOLUA_DISABLE_tolua_peak_peak_IntProperty_getType00
static int tolua_peak_peak_IntProperty_getType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::IntProperty",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::IntProperty* self = (peak::IntProperty*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getType'", NULL);
#endif
  {
   peak::PropertyType tolua_ret = (peak::PropertyType)  self->getType();
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

/* method: set of class  peak::IntProperty */
#ifndef TOLUA_DISABLE_tolua_peak_peak_IntProperty_set00
static int tolua_peak_peak_IntProperty_set00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::IntProperty",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::IntProperty* self = (peak::IntProperty*)  tolua_tousertype(tolua_S,1,0);
  int i = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'", NULL);
#endif
  {
   self->set(i);
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

/* method: getInt of class  peak::IntProperty */
#ifndef TOLUA_DISABLE_tolua_peak_peak_IntProperty_getInt00
static int tolua_peak_peak_IntProperty_getInt00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::IntProperty",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::IntProperty* self = (peak::IntProperty*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getInt'", NULL);
#endif
  {
   int tolua_ret = (int)  self->getInt();
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

/* method: set of class  peak::IntProperty */
#ifndef TOLUA_DISABLE_tolua_peak_peak_IntProperty_set01
static int tolua_peak_peak_IntProperty_set01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::IntProperty",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::IntProperty* self = (peak::IntProperty*)  tolua_tousertype(tolua_S,1,0);
  float f = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'", NULL);
#endif
  {
   self->set(f);
  }
 }
 return 0;
tolua_lerror:
 return tolua_peak_peak_IntProperty_set00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getFloat of class  peak::IntProperty */
#ifndef TOLUA_DISABLE_tolua_peak_peak_IntProperty_getFloat00
static int tolua_peak_peak_IntProperty_getFloat00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::IntProperty",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::IntProperty* self = (peak::IntProperty*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFloat'", NULL);
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

/* method: set of class  peak::IntProperty */
#ifndef TOLUA_DISABLE_tolua_peak_peak_IntProperty_set02
static int tolua_peak_peak_IntProperty_set02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::IntProperty",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Property",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::IntProperty* self = (peak::IntProperty*)  tolua_tousertype(tolua_S,1,0);
  peak::Property* p = ((peak::Property*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'", NULL);
#endif
  {
   self->set(p);
  }
 }
 return 0;
tolua_lerror:
 return tolua_peak_peak_IntProperty_set01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: serialize of class  peak::IntProperty */
#ifndef TOLUA_DISABLE_tolua_peak_peak_IntProperty_serialize00
static int tolua_peak_peak_IntProperty_serialize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::IntProperty",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::IntProperty* self = (peak::IntProperty*)  tolua_tousertype(tolua_S,1,0);
  peak::Buffer* buffer = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'serialize'", NULL);
#endif
  {
   self->serialize(buffer);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'serialize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: deserialize of class  peak::IntProperty */
#ifndef TOLUA_DISABLE_tolua_peak_peak_IntProperty_deserialize00
static int tolua_peak_peak_IntProperty_deserialize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::IntProperty",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::IntProperty* self = (peak::IntProperty*)  tolua_tousertype(tolua_S,1,0);
  peak::Buffer* buffer = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'deserialize'", NULL);
#endif
  {
   self->deserialize(buffer);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'deserialize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::DirectLink */
#ifndef TOLUA_DISABLE_tolua_peak_peak_DirectLink_new00
static int tolua_peak_peak_DirectLink_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::DirectLink",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::DirectLink* tolua_ret = (peak::DirectLink*)  Mtolua_new((peak::DirectLink)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::DirectLink");
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

/* method: new_local of class  peak::DirectLink */
#ifndef TOLUA_DISABLE_tolua_peak_peak_DirectLink_new00_local
static int tolua_peak_peak_DirectLink_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::DirectLink",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::DirectLink* tolua_ret = (peak::DirectLink*)  Mtolua_new((peak::DirectLink)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::DirectLink");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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

/* method: delete of class  peak::DirectLink */
#ifndef TOLUA_DISABLE_tolua_peak_peak_DirectLink_delete00
static int tolua_peak_peak_DirectLink_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::DirectLink",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::DirectLink* self = (peak::DirectLink*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  peak::DirectLink */
#ifndef TOLUA_DISABLE_tolua_peak_peak_DirectLink_create00
static int tolua_peak_peak_DirectLink_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::DirectLink",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Property",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::Property",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::DirectLink* self = (peak::DirectLink*)  tolua_tousertype(tolua_S,1,0);
  peak::Property* source = ((peak::Property*)  tolua_tousertype(tolua_S,2,0));
  peak::Property* target = ((peak::Property*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'create'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->create(source,target);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: destroy of class  peak::DirectLink */
#ifndef TOLUA_DISABLE_tolua_peak_peak_DirectLink_destroy00
static int tolua_peak_peak_DirectLink_destroy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::DirectLink",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::DirectLink* self = (peak::DirectLink*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroy'", NULL);
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

/* method: update of class  peak::DirectLink */
#ifndef TOLUA_DISABLE_tolua_peak_peak_DirectLink_update00
static int tolua_peak_peak_DirectLink_update00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::DirectLink",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::DirectLink* self = (peak::DirectLink*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'update'", NULL);
#endif
  {
   self->update();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'update'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::EventData */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EventData_new00
static int tolua_peak_peak_EventData_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::EventData",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  {
   peak::EventData* tolua_ret = (peak::EventData*)  Mtolua_new((peak::EventData)(name));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::EventData");
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

/* method: new_local of class  peak::EventData */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EventData_new00_local
static int tolua_peak_peak_EventData_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::EventData",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  {
   peak::EventData* tolua_ret = (peak::EventData*)  Mtolua_new((peak::EventData)(name));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::EventData");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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

/* method: delete of class  peak::EventData */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EventData_delete00
static int tolua_peak_peak_EventData_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::EventData",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::EventData* self = (peak::EventData*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getName of class  peak::EventData */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EventData_getName00
static int tolua_peak_peak_EventData_getName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::EventData",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::EventData* self = (peak::EventData*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getName'", NULL);
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

/* method: set of class  peak::EventData */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EventData_set00
static int tolua_peak_peak_EventData_set00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::EventData",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::EventData* self = (peak::EventData*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  float value = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'", NULL);
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

/* method: set of class  peak::EventData */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EventData_set01
static int tolua_peak_peak_EventData_set01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::EventData",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::EventData* self = (peak::EventData*)  tolua_tousertype(tolua_S,1,0);
  peak::Buffer* buffer = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'", NULL);
#endif
  {
   self->set(buffer);
  }
 }
 return 0;
tolua_lerror:
 return tolua_peak_peak_EventData_set00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: apply of class  peak::EventData */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EventData_apply00
static int tolua_peak_peak_EventData_apply00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::EventData",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Script",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::EventData* self = (peak::EventData*)  tolua_tousertype(tolua_S,1,0);
  peak::Script* script = ((peak::Script*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'apply'", NULL);
#endif
  {
   self->apply(script);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'apply'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: apply of class  peak::EventData */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EventData_apply01
static int tolua_peak_peak_EventData_apply01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::EventData",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Buffer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::EventData* self = (peak::EventData*)  tolua_tousertype(tolua_S,1,0);
  peak::Buffer* buffer = ((peak::Buffer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'apply'", NULL);
#endif
  {
   self->apply(buffer);
  }
 }
 return 0;
tolua_lerror:
 return tolua_peak_peak_EventData_apply00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::EventScriptLink */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EventScriptLink_new00
static int tolua_peak_peak_EventScriptLink_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::EventScriptLink",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::EventScriptLink* tolua_ret = (peak::EventScriptLink*)  Mtolua_new((peak::EventScriptLink)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::EventScriptLink");
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

/* method: new_local of class  peak::EventScriptLink */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EventScriptLink_new00_local
static int tolua_peak_peak_EventScriptLink_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::EventScriptLink",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::EventScriptLink* tolua_ret = (peak::EventScriptLink*)  Mtolua_new((peak::EventScriptLink)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::EventScriptLink");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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

/* method: delete of class  peak::EventScriptLink */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EventScriptLink_delete00
static int tolua_peak_peak_EventScriptLink_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::EventScriptLink",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::EventScriptLink* self = (peak::EventScriptLink*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  peak::EventScriptLink */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EventScriptLink_create00
static int tolua_peak_peak_EventScriptLink_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::EventScriptLink",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Event",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::Script",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::EventScriptLink* self = (peak::EventScriptLink*)  tolua_tousertype(tolua_S,1,0);
  peak::Event* source = ((peak::Event*)  tolua_tousertype(tolua_S,2,0));
  peak::Script* script = ((peak::Script*)  tolua_tousertype(tolua_S,3,0));
  std::string function = ((std::string)  tolua_tocppstring(tolua_S,4,0));
  std::string filter = ((std::string)  tolua_tocppstring(tolua_S,5,""));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'create'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->create(source,script,function,filter);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: destroy of class  peak::EventScriptLink */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EventScriptLink_destroy00
static int tolua_peak_peak_EventScriptLink_destroy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::EventScriptLink",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::EventScriptLink* self = (peak::EventScriptLink*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroy'", NULL);
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

/* method: update of class  peak::EventScriptLink */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EventScriptLink_update00
static int tolua_peak_peak_EventScriptLink_update00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::EventScriptLink",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::EventData",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::EventScriptLink* self = (peak::EventScriptLink*)  tolua_tousertype(tolua_S,1,0);
  peak::EventData* data = ((peak::EventData*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'update'", NULL);
#endif
  {
   self->update(data);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'update'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: update of class  peak::EventScriptLink */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EventScriptLink_update01
static int tolua_peak_peak_EventScriptLink_update01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::EventScriptLink",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::EventScriptLink* self = (peak::EventScriptLink*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'update'", NULL);
#endif
  {
   self->update();
  }
 }
 return 0;
tolua_lerror:
 return tolua_peak_peak_EventScriptLink_update00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::EventRemoteLink */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EventRemoteLink_new00
static int tolua_peak_peak_EventRemoteLink_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::EventRemoteLink",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::EventRemoteLink* tolua_ret = (peak::EventRemoteLink*)  Mtolua_new((peak::EventRemoteLink)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::EventRemoteLink");
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

/* method: new_local of class  peak::EventRemoteLink */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EventRemoteLink_new00_local
static int tolua_peak_peak_EventRemoteLink_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::EventRemoteLink",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::EventRemoteLink* tolua_ret = (peak::EventRemoteLink*)  Mtolua_new((peak::EventRemoteLink)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::EventRemoteLink");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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

/* method: delete of class  peak::EventRemoteLink */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EventRemoteLink_delete00
static int tolua_peak_peak_EventRemoteLink_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::EventRemoteLink",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::EventRemoteLink* self = (peak::EventRemoteLink*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  peak::EventRemoteLink */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EventRemoteLink_create00
static int tolua_peak_peak_EventRemoteLink_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::EventRemoteLink",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Event",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,5,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,6,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::EventRemoteLink* self = (peak::EventRemoteLink*)  tolua_tousertype(tolua_S,1,0);
  peak::Event* source = ((peak::Event*)  tolua_tousertype(tolua_S,2,0));
  int entityid = ((int)  tolua_tonumber(tolua_S,3,0));
  int link = ((int)  tolua_tonumber(tolua_S,4,0));
  std::string filter = ((std::string)  tolua_tocppstring(tolua_S,5,0));
  bool reliable = ((bool)  tolua_toboolean(tolua_S,6,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'create'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->create(source,entityid,link,filter,reliable);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: destroy of class  peak::EventRemoteLink */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EventRemoteLink_destroy00
static int tolua_peak_peak_EventRemoteLink_destroy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::EventRemoteLink",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::EventRemoteLink* self = (peak::EventRemoteLink*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroy'", NULL);
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

/* method: update of class  peak::EventRemoteLink */
#ifndef TOLUA_DISABLE_tolua_peak_peak_EventRemoteLink_update00
static int tolua_peak_peak_EventRemoteLink_update00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::EventRemoteLink",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::EventRemoteLink* self = (peak::EventRemoteLink*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'update'", NULL);
#endif
  {
   self->update();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'update'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::PropertyScriptLink */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PropertyScriptLink_new00
static int tolua_peak_peak_PropertyScriptLink_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::PropertyScriptLink",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::PropertyScriptLink* tolua_ret = (peak::PropertyScriptLink*)  Mtolua_new((peak::PropertyScriptLink)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::PropertyScriptLink");
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

/* method: new_local of class  peak::PropertyScriptLink */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PropertyScriptLink_new00_local
static int tolua_peak_peak_PropertyScriptLink_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::PropertyScriptLink",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::PropertyScriptLink* tolua_ret = (peak::PropertyScriptLink*)  Mtolua_new((peak::PropertyScriptLink)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::PropertyScriptLink");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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

/* method: delete of class  peak::PropertyScriptLink */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PropertyScriptLink_delete00
static int tolua_peak_peak_PropertyScriptLink_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::PropertyScriptLink",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::PropertyScriptLink* self = (peak::PropertyScriptLink*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  peak::PropertyScriptLink */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PropertyScriptLink_create00
static int tolua_peak_peak_PropertyScriptLink_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::PropertyScriptLink",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::Property",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::Script",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::PropertyScriptLink* self = (peak::PropertyScriptLink*)  tolua_tousertype(tolua_S,1,0);
  peak::Property* source = ((peak::Property*)  tolua_tousertype(tolua_S,2,0));
  peak::Script* script = ((peak::Script*)  tolua_tousertype(tolua_S,3,0));
  std::string function = ((std::string)  tolua_tocppstring(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'create'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->create(source,script,function);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: destroy of class  peak::PropertyScriptLink */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PropertyScriptLink_destroy00
static int tolua_peak_peak_PropertyScriptLink_destroy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::PropertyScriptLink",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::PropertyScriptLink* self = (peak::PropertyScriptLink*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroy'", NULL);
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

/* method: update of class  peak::PropertyScriptLink */
#ifndef TOLUA_DISABLE_tolua_peak_peak_PropertyScriptLink_update00
static int tolua_peak_peak_PropertyScriptLink_update00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::PropertyScriptLink",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::PropertyScriptLink* self = (peak::PropertyScriptLink*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'update'", NULL);
#endif
  {
   self->update();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'update'.",&tolua_err);
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
   peak::BroadcastClient* tolua_ret = (peak::BroadcastClient*)  Mtolua_new((peak::BroadcastClient)());
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
   peak::BroadcastClient* tolua_ret = (peak::BroadcastClient*)  Mtolua_new((peak::BroadcastClient)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::BroadcastClient");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'start'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'stop'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clearList'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getServerCount'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getServerInfo'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getServerAddress'", NULL);
#endif
  {
   peak::Address tolua_ret = (peak::Address)  self->getServerAddress(index);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Address)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Address");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Address));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Address");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setCallback'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'doWork'", NULL);
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
   peak::CameraSceneNode* tolua_ret = (peak::CameraSceneNode*)  Mtolua_new((peak::CameraSceneNode)(parent));
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
   peak::CameraSceneNode* tolua_ret = (peak::CameraSceneNode*)  Mtolua_new((peak::CameraSceneNode)(parent));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::CameraSceneNode");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setNearValue'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getNearValue'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setFarValue'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFarValue'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setOrthogonal'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isOrthogonal'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setFOV'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFOV'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAspectRatio'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAspectRatio'", NULL);
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

/* method: render of class  peak::CameraSceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_CameraSceneNode_render00
static int tolua_peak_peak_CameraSceneNode_render00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'render'", NULL);
#endif
  {
   self->render();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'render'.",&tolua_err);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'shutdown'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'renderFrame'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getStandardCamera'", NULL);
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

/* method: saveMessages of class  peak::GraphicsEngine */
#ifndef TOLUA_DISABLE_tolua_peak_peak_GraphicsEngine_saveMessages00
static int tolua_peak_peak_GraphicsEngine_saveMessages00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'saveMessages'", NULL);
#endif
  {
   self->saveMessages();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'saveMessages'.",&tolua_err);
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
   peak::Image* tolua_ret = (peak::Image*)  Mtolua_new((peak::Image)());
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
   peak::Image* tolua_ret = (peak::Image*)  Mtolua_new((peak::Image)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Image");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
   peak::Image* tolua_ret = (peak::Image*)  Mtolua_new((peak::Image)(imagename));
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
   peak::Image* tolua_ret = (peak::Image*)  Mtolua_new((peak::Image)(imagename));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Image");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Image_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  peak::Image */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Image_delete00
static int tolua_peak_peak_Image_delete00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDimension'", NULL);
#endif
  {
   peak::Vector2DI tolua_ret = (peak::Vector2DI)  self->getDimension();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Vector2DI)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector2DI");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector2DI));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector2DI");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'load'", NULL);
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
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector2DI",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Image* self = (peak::Image*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector2DI pixel = *((peak::Vector2DI*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPixel'", NULL);
#endif
  {
   peak::Color tolua_ret = (peak::Color)  self->getPixel(pixel);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Color)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Color");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Color));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Color");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPixel'", NULL);
#endif
  {
   peak::Color tolua_ret = (peak::Color)  self->getPixel(x,y);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Color)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Color");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Color));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Color");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
   peak::MeshSceneNode* tolua_ret = (peak::MeshSceneNode*)  Mtolua_new((peak::MeshSceneNode)(mesh,animated,parent));
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
   peak::MeshSceneNode* tolua_ret = (peak::MeshSceneNode*)  Mtolua_new((peak::MeshSceneNode)(mesh,animated,parent));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::MeshSceneNode");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getEndFrame'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFrameNr'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getStartFrame'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setLoopMode'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAnimationSpeed'", NULL);
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

/* method: pushAnimation of class  peak::MeshSceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_MeshSceneNode_pushAnimation00
static int tolua_peak_peak_MeshSceneNode_pushAnimation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::MeshSceneNode",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::MeshSceneNode* self = (peak::MeshSceneNode*)  tolua_tousertype(tolua_S,1,0);
  std::string animation = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'pushAnimation'", NULL);
#endif
  {
   self->pushAnimation(animation);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'pushAnimation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: popAnimation of class  peak::MeshSceneNode */
#ifndef TOLUA_DISABLE_tolua_peak_peak_MeshSceneNode_popAnimation00
static int tolua_peak_peak_MeshSceneNode_popAnimation00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'popAnimation'", NULL);
#endif
  {
   self->popAnimation();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'popAnimation'.",&tolua_err);
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
   peak::TextureOverlay* tolua_ret = (peak::TextureOverlay*)  Mtolua_new((peak::TextureOverlay)(filename,parent));
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
   peak::TextureOverlay* tolua_ret = (peak::TextureOverlay*)  Mtolua_new((peak::TextureOverlay)(filename,parent));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::TextureOverlay");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'notifySizeChanged'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'render'", NULL);
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
     !tolua_iscppstring(tolua_S,3,1,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"peak::Overlay",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string label = ((std::string)  tolua_tocppstring(tolua_S,2,""));
  std::string font = ((std::string)  tolua_tocppstring(tolua_S,3,""));
  peak::Overlay* parent = ((peak::Overlay*)  tolua_tousertype(tolua_S,4,0));
  {
   peak::LabelOverlay* tolua_ret = (peak::LabelOverlay*)  Mtolua_new((peak::LabelOverlay)(label,font,parent));
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
     !tolua_iscppstring(tolua_S,3,1,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"peak::Overlay",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string label = ((std::string)  tolua_tocppstring(tolua_S,2,""));
  std::string font = ((std::string)  tolua_tocppstring(tolua_S,3,""));
  peak::Overlay* parent = ((peak::Overlay*)  tolua_tousertype(tolua_S,4,0));
  {
   peak::LabelOverlay* tolua_ret = (peak::LabelOverlay*)  Mtolua_new((peak::LabelOverlay)(label,font,parent));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::LabelOverlay");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setLabel'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLabel'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'render'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'shutdown'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRootOverlay'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'render'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setSceneNode'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSceneNode'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'doWork'", NULL);
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
   peak::FollowingController* tolua_ret = (peak::FollowingController*)  Mtolua_new((peak::FollowingController)());
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
   peak::FollowingController* tolua_ret = (peak::FollowingController*)  Mtolua_new((peak::FollowingController)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::FollowingController");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setTarget'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTarget'", NULL);
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
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::FollowingController* self = (peak::FollowingController*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D distance = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setDistance'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDistance'", NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getDistance();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Vector3D)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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

/* method: setRotation of class  peak::FollowingController */
#ifndef TOLUA_DISABLE_tolua_peak_peak_FollowingController_setRotation00
static int tolua_peak_peak_FollowingController_setRotation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::FollowingController",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::FollowingController* self = (peak::FollowingController*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D rotation = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRotation'", NULL);
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

/* method: getRotation of class  peak::FollowingController */
#ifndef TOLUA_DISABLE_tolua_peak_peak_FollowingController_getRotation00
static int tolua_peak_peak_FollowingController_getRotation00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRotation'", NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getRotation();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Vector3D)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMode'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMode'", NULL);
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
   peak::RotationController* tolua_ret = (peak::RotationController*)  Mtolua_new((peak::RotationController)());
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
   peak::RotationController* tolua_ret = (peak::RotationController*)  Mtolua_new((peak::RotationController)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::RotationController");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
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
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::RotationController* self = (peak::RotationController*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D speed = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRotationSpeed'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRotationSpeed'", NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getRotationSpeed();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Vector3D)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'doWork'", NULL);
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

/* method: new of class  peak::Light */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Light_new00
static int tolua_peak_peak_Light_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Light",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::SceneNode",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SceneNode* parent = ((peak::SceneNode*)  tolua_tousertype(tolua_S,2,0));
  {
   peak::Light* tolua_ret = (peak::Light*)  Mtolua_new((peak::Light)(parent));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Light");
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

/* method: new_local of class  peak::Light */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Light_new00_local
static int tolua_peak_peak_Light_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Light",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"peak::SceneNode",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::SceneNode* parent = ((peak::SceneNode*)  tolua_tousertype(tolua_S,2,0));
  {
   peak::Light* tolua_ret = (peak::Light*)  Mtolua_new((peak::Light)(parent));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Light");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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

/* method: delete of class  peak::Light */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Light_delete00
static int tolua_peak_peak_Light_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Light",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Light* self = (peak::Light*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setType of class  peak::Light */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Light_setType00
static int tolua_peak_peak_Light_setType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Light",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Light* self = (peak::Light*)  tolua_tousertype(tolua_S,1,0);
  peak::LightType type = ((peak::LightType) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setType'", NULL);
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

/* method: getType of class  peak::Light */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Light_getType00
static int tolua_peak_peak_Light_getType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Light",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Light* self = (peak::Light*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getType'", NULL);
#endif
  {
   peak::LightType tolua_ret = (peak::LightType)  self->getType();
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

/* method: new of class  peak::ParticleSystem */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ParticleSystem_new00
static int tolua_peak_peak_ParticleSystem_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::ParticleSystem",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::SceneNode",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string filename = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  peak::SceneNode* parent = ((peak::SceneNode*)  tolua_tousertype(tolua_S,3,0));
  {
   peak::ParticleSystem* tolua_ret = (peak::ParticleSystem*)  Mtolua_new((peak::ParticleSystem)(filename,parent));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::ParticleSystem");
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

/* method: new_local of class  peak::ParticleSystem */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ParticleSystem_new00_local
static int tolua_peak_peak_ParticleSystem_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::ParticleSystem",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"peak::SceneNode",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string filename = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  peak::SceneNode* parent = ((peak::SceneNode*)  tolua_tousertype(tolua_S,3,0));
  {
   peak::ParticleSystem* tolua_ret = (peak::ParticleSystem*)  Mtolua_new((peak::ParticleSystem)(filename,parent));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::ParticleSystem");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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

/* method: delete of class  peak::ParticleSystem */
#ifndef TOLUA_DISABLE_tolua_peak_peak_ParticleSystem_delete00
static int tolua_peak_peak_ParticleSystem_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::ParticleSystem",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::ParticleSystem* self = (peak::ParticleSystem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Box */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Box_new00
static int tolua_peak_peak_Box_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Box",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Box* tolua_ret = (peak::Box*)  Mtolua_new((peak::Box)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Box");
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

/* method: new_local of class  peak::Box */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Box_new00_local
static int tolua_peak_peak_Box_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Box",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Box* tolua_ret = (peak::Box*)  Mtolua_new((peak::Box)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Box");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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

/* method: delete of class  peak::Box */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Box_delete00
static int tolua_peak_peak_Box_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Box",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Box* self = (peak::Box*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  peak::Box */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Box_create00
static int tolua_peak_peak_Box_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Box",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Box* self = (peak::Box*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D dimensions = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
  float mass = ((float)  tolua_tonumber(tolua_S,3,0.0f));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'create'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->create(dimensions,mass);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
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
   peak::Plane* tolua_ret = (peak::Plane*)  Mtolua_new((peak::Plane)());
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
   peak::Plane* tolua_ret = (peak::Plane*)  Mtolua_new((peak::Plane)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Plane");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  peak::Plane */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Plane_create00
static int tolua_peak_peak_Plane_create00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'create'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->create();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  peak::Plane */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Plane_create01
static int tolua_peak_peak_Plane_create01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Plane",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::Plane* self = (peak::Plane*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D normal = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
  float constant = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'create'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->create(normal,constant);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Plane_create00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  peak::Plane */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Plane_create02
static int tolua_peak_peak_Plane_create02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Plane",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"peak::Vector3D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::Plane* self = (peak::Plane*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D normal = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
  float constant = ((float)  tolua_tonumber(tolua_S,3,0));
  peak::Vector3D position = *((peak::Vector3D*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'create'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->create(normal,constant,position);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Plane_create01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::Trimesh */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Trimesh_new00
static int tolua_peak_peak_Trimesh_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Trimesh",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Trimesh* tolua_ret = (peak::Trimesh*)  Mtolua_new((peak::Trimesh)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Trimesh");
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

/* method: new_local of class  peak::Trimesh */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Trimesh_new00_local
static int tolua_peak_peak_Trimesh_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Trimesh",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Trimesh* tolua_ret = (peak::Trimesh*)  Mtolua_new((peak::Trimesh)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Trimesh");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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

/* method: delete of class  peak::Trimesh */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Trimesh_delete00
static int tolua_peak_peak_Trimesh_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Trimesh",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Trimesh* self = (peak::Trimesh*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  peak::Trimesh */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Trimesh_create00
static int tolua_peak_peak_Trimesh_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Trimesh",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Trimesh* self = (peak::Trimesh*)  tolua_tousertype(tolua_S,1,0);
  std::string modelfile = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  float mass = ((float)  tolua_tonumber(tolua_S,3,0.0f));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'create'", NULL);
#endif
  {
   self->create(modelfile,mass);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  peak::Trimesh */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Trimesh_create01
static int tolua_peak_peak_Trimesh_create01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Trimesh",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"peak::Vector3D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::Trimesh* self = (peak::Trimesh*)  tolua_tousertype(tolua_S,1,0);
  std::string modelfile = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  float mass = ((float)  tolua_tonumber(tolua_S,3,0));
  peak::Vector3D scale = *((peak::Vector3D*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'create'", NULL);
#endif
  {
   self->create(modelfile,mass,scale);
  }
 }
 return 0;
tolua_lerror:
 return tolua_peak_peak_Trimesh_create00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: destroy of class  peak::Trimesh */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Trimesh_destroy00
static int tolua_peak_peak_Trimesh_destroy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Trimesh",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Trimesh* self = (peak::Trimesh*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroy'", NULL);
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

/* method: new of class  peak::Sphere */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Sphere_new00
static int tolua_peak_peak_Sphere_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Sphere",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Sphere* tolua_ret = (peak::Sphere*)  Mtolua_new((peak::Sphere)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Sphere");
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

/* method: new_local of class  peak::Sphere */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Sphere_new00_local
static int tolua_peak_peak_Sphere_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Sphere",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Sphere* tolua_ret = (peak::Sphere*)  Mtolua_new((peak::Sphere)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Sphere");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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

/* method: delete of class  peak::Sphere */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Sphere_delete00
static int tolua_peak_peak_Sphere_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Sphere",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Sphere* self = (peak::Sphere*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  peak::Sphere */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Sphere_create00
static int tolua_peak_peak_Sphere_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Sphere",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Sphere* self = (peak::Sphere*)  tolua_tousertype(tolua_S,1,0);
  float radius = ((float)  tolua_tonumber(tolua_S,2,0));
  float mass = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'create'", NULL);
#endif
  {
   self->create(radius,mass);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: destroy of class  peak::Sphere */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Sphere_destroy00
static int tolua_peak_peak_Sphere_destroy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Sphere",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Sphere* self = (peak::Sphere*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroy'", NULL);
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

/* method: new of class  peak::Callback */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Callback_new00
static int tolua_peak_peak_Callback_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Callback",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Callback* tolua_ret = (peak::Callback*)  Mtolua_new((peak::Callback)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Callback");
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

/* method: new_local of class  peak::Callback */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Callback_new00_local
static int tolua_peak_peak_Callback_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Callback",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Callback* tolua_ret = (peak::Callback*)  Mtolua_new((peak::Callback)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Callback");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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

/* method: delete of class  peak::Callback */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Callback_delete00
static int tolua_peak_peak_Callback_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Callback",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Callback* self = (peak::Callback*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  peak::Callback */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Callback_create00
static int tolua_peak_peak_Callback_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Callback",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Callback* self = (peak::Callback*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'create'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->create();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: destroy of class  peak::Callback */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Callback_destroy00
static int tolua_peak_peak_Callback_destroy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Callback",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Callback* self = (peak::Callback*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroy'", NULL);
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

/* method: update of class  peak::Callback */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Callback_update00
static int tolua_peak_peak_Callback_update00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Callback",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Callback* self = (peak::Callback*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'update'", NULL);
#endif
  {
   self->update();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'update'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: updateAll of class  peak::Callback */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Callback_updateAll00
static int tolua_peak_peak_Callback_updateAll00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Callback",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Callback::updateAll();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'updateAll'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getType of class  peak::Callback */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Callback_getType00
static int tolua_peak_peak_Callback_getType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Callback",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Callback* self = (peak::Callback*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getType'", NULL);
#endif
  {
   peak::CallbackType tolua_ret = (peak::CallbackType)  self->getType();
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

/* method: new of class  peak::CollisionInfo */
#ifndef TOLUA_DISABLE_tolua_peak_peak_CollisionInfo_new00
static int tolua_peak_peak_CollisionInfo_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::CollisionInfo",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::CollisionInfo* tolua_ret = (peak::CollisionInfo*)  Mtolua_new((peak::CollisionInfo)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::CollisionInfo");
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

/* method: new_local of class  peak::CollisionInfo */
#ifndef TOLUA_DISABLE_tolua_peak_peak_CollisionInfo_new00_local
static int tolua_peak_peak_CollisionInfo_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::CollisionInfo",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::CollisionInfo* tolua_ret = (peak::CollisionInfo*)  Mtolua_new((peak::CollisionInfo)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::CollisionInfo");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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

/* get function: lambda of class  peak::CollisionInfo */
#ifndef TOLUA_DISABLE_tolua_get_peak__CollisionInfo_lambda
static int tolua_get_peak__CollisionInfo_lambda(lua_State* tolua_S)
{
  peak::CollisionInfo* self = (peak::CollisionInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'lambda'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->lambda);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: lambda of class  peak::CollisionInfo */
#ifndef TOLUA_DISABLE_tolua_set_peak__CollisionInfo_lambda
static int tolua_set_peak__CollisionInfo_lambda(lua_State* tolua_S)
{
  peak::CollisionInfo* self = (peak::CollisionInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'lambda'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->lambda = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: point of class  peak::CollisionInfo */
#ifndef TOLUA_DISABLE_tolua_get_peak__CollisionInfo_point
static int tolua_get_peak__CollisionInfo_point(lua_State* tolua_S)
{
  peak::CollisionInfo* self = (peak::CollisionInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'point'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->point,"peak::Vector3D");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: point of class  peak::CollisionInfo */
#ifndef TOLUA_DISABLE_tolua_set_peak__CollisionInfo_point
static int tolua_set_peak__CollisionInfo_point(lua_State* tolua_S)
{
  peak::CollisionInfo* self = (peak::CollisionInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'point'",NULL);
  if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err)))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->point = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: object of class  peak::CollisionInfo */
#ifndef TOLUA_DISABLE_tolua_get_peak__CollisionInfo_object_ptr
static int tolua_get_peak__CollisionInfo_object_ptr(lua_State* tolua_S)
{
  peak::CollisionInfo* self = (peak::CollisionInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'object'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)self->object,"peak::Body");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: object of class  peak::CollisionInfo */
#ifndef TOLUA_DISABLE_tolua_set_peak__CollisionInfo_object_ptr
static int tolua_set_peak__CollisionInfo_object_ptr(lua_State* tolua_S)
{
  peak::CollisionInfo* self = (peak::CollisionInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'object'",NULL);
  if (!tolua_isusertype(tolua_S,2,"peak::Body",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->object = ((peak::Body*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  peak::RayCallback */
#ifndef TOLUA_DISABLE_tolua_peak_peak_RayCallback_new00
static int tolua_peak_peak_RayCallback_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::RayCallback",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::RayCallback* tolua_ret = (peak::RayCallback*)  Mtolua_new((peak::RayCallback)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::RayCallback");
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

/* method: new_local of class  peak::RayCallback */
#ifndef TOLUA_DISABLE_tolua_peak_peak_RayCallback_new00_local
static int tolua_peak_peak_RayCallback_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::RayCallback",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::RayCallback* tolua_ret = (peak::RayCallback*)  Mtolua_new((peak::RayCallback)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::RayCallback");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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

/* method: delete of class  peak::RayCallback */
#ifndef TOLUA_DISABLE_tolua_peak_peak_RayCallback_delete00
static int tolua_peak_peak_RayCallback_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::RayCallback",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::RayCallback* self = (peak::RayCallback*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  peak::RayCallback */
#ifndef TOLUA_DISABLE_tolua_peak_peak_RayCallback_create00
static int tolua_peak_peak_RayCallback_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::RayCallback",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"peak::Vector3D",0,&tolua_err)) ||
     !tolua_isusertype(tolua_S,4,"peak::Script",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::RayCallback* self = (peak::RayCallback*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D rayto = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
  peak::Vector3D rayfrom = *((peak::Vector3D*)  tolua_tousertype(tolua_S,3,0));
  peak::Script* script = ((peak::Script*)  tolua_tousertype(tolua_S,4,0));
  std::string callbackname = ((std::string)  tolua_tocppstring(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'create'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->create(rayto,rayfrom,script,callbackname);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: destroy of class  peak::RayCallback */
#ifndef TOLUA_DISABLE_tolua_peak_peak_RayCallback_destroy00
static int tolua_peak_peak_RayCallback_destroy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::RayCallback",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::RayCallback* self = (peak::RayCallback*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroy'", NULL);
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

/* method: update of class  peak::RayCallback */
#ifndef TOLUA_DISABLE_tolua_peak_peak_RayCallback_update00
static int tolua_peak_peak_RayCallback_update00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::RayCallback",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::RayCallback* self = (peak::RayCallback*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'update'", NULL);
#endif
  {
   self->update();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'update'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isCollision of class  peak::RayCallback */
#ifndef TOLUA_DISABLE_tolua_peak_peak_RayCallback_isCollision00
static int tolua_peak_peak_RayCallback_isCollision00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::RayCallback",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::RayCallback* self = (peak::RayCallback*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isCollision'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isCollision();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isCollision'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setRay of class  peak::RayCallback */
#ifndef TOLUA_DISABLE_tolua_peak_peak_RayCallback_setRay00
static int tolua_peak_peak_RayCallback_setRay00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::RayCallback",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"peak::Vector3D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::RayCallback* self = (peak::RayCallback*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D rayto = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
  peak::Vector3D rayfrom = *((peak::Vector3D*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRay'", NULL);
#endif
  {
   self->setRay(rayto,rayfrom);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setRay'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRay of class  peak::RayCallback */
#ifndef TOLUA_DISABLE_tolua_peak_peak_RayCallback_getRay00
static int tolua_peak_peak_RayCallback_getRay00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::RayCallback",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::RayCallback* self = (peak::RayCallback*)  tolua_tousertype(tolua_S,1,0);
  bool index = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRay'", NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getRay(index);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Vector3D)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRay'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setInfo of class  peak::RayCallback */
#ifndef TOLUA_DISABLE_tolua_peak_peak_RayCallback_setInfo00
static int tolua_peak_peak_RayCallback_setInfo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::RayCallback",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::CollisionInfo",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::RayCallback* self = (peak::RayCallback*)  tolua_tousertype(tolua_S,1,0);
  peak::CollisionInfo info = *((peak::CollisionInfo*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setInfo'", NULL);
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

/* method: getInfo of class  peak::RayCallback */
#ifndef TOLUA_DISABLE_tolua_peak_peak_RayCallback_getInfo00
static int tolua_peak_peak_RayCallback_getInfo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::RayCallback",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::RayCallback* self = (peak::RayCallback*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getInfo'", NULL);
#endif
  {
   peak::CollisionInfo tolua_ret = (peak::CollisionInfo)  self->getInfo();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::CollisionInfo)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::CollisionInfo");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::CollisionInfo));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::CollisionInfo");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
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

/* method: new of class  peak::Heightfield */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Heightfield_new00
static int tolua_peak_peak_Heightfield_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Heightfield",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Heightfield* tolua_ret = (peak::Heightfield*)  Mtolua_new((peak::Heightfield)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Heightfield");
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

/* method: new_local of class  peak::Heightfield */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Heightfield_new00_local
static int tolua_peak_peak_Heightfield_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"peak::Heightfield",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   peak::Heightfield* tolua_ret = (peak::Heightfield*)  Mtolua_new((peak::Heightfield)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Heightfield");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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

/* method: delete of class  peak::Heightfield */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Heightfield_delete00
static int tolua_peak_peak_Heightfield_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Heightfield",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Heightfield* self = (peak::Heightfield*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  peak::Heightfield */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Heightfield_create00
static int tolua_peak_peak_Heightfield_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Heightfield",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Heightfield* self = (peak::Heightfield*)  tolua_tousertype(tolua_S,1,0);
  std::string heightmap = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  float mass = ((float)  tolua_tonumber(tolua_S,3,0.0f));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'create'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->create(heightmap,mass);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  peak::Heightfield */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Heightfield_create01
static int tolua_peak_peak_Heightfield_create01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Heightfield",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"peak::Vector3D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  peak::Heightfield* self = (peak::Heightfield*)  tolua_tousertype(tolua_S,1,0);
  std::string heightmap = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  float mass = ((float)  tolua_tonumber(tolua_S,3,0));
  peak::Vector3D scale = *((peak::Vector3D*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'create'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->create(heightmap,mass,scale);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_peak_peak_Heightfield_create00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: destroy of class  peak::Heightfield */
#ifndef TOLUA_DISABLE_tolua_peak_peak_Heightfield_destroy00
static int tolua_peak_peak_Heightfield_destroy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"peak::Heightfield",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Heightfield* self = (peak::Heightfield*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroy'", NULL);
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
   peak::Menu* tolua_ret = (peak::Menu*)  Mtolua_new((peak::Menu)());
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
   peak::Menu* tolua_ret = (peak::Menu*)  Mtolua_new((peak::Menu)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Menu");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroy'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getName'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'show'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hide'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'pressButton'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setCheckBox'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setListBox'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getListBox'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getButton'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLabel'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getElement'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getScript'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'loadMenu'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'unloadMenu'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'unloadMenu'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMenuCount'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMenu'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMenu'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCurrentMenu'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'shutdown'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'activateMenu'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'activateMenu'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroy'", NULL);
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
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector2D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::MenuElement* self = (peak::MenuElement*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector2D position = *((peak::Vector2D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPosition'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPosition'", NULL);
#endif
  {
   peak::Vector2D tolua_ret = (peak::Vector2D)  self->getPosition();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Vector2D)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector2D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector2D));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector2D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector2D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::MenuElement* self = (peak::MenuElement*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector2D size = *((peak::Vector2D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setSize'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSize'", NULL);
#endif
  {
   peak::Vector2D tolua_ret = (peak::Vector2D)  self->getSize();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Vector2D)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector2D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector2D));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector2D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setID'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getID'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'show'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hide'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getType'", NULL);
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

/* method: getWidget of class  peak::MenuElement */
#ifndef TOLUA_DISABLE_tolua_peak_peak_MenuElement_getWidget00
static int tolua_peak_peak_MenuElement_getWidget00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWidget'", NULL);
#endif
  {
   gcn::Widget* tolua_ret = (gcn::Widget*)  self->getWidget();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"gcn::Widget");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getWidget'.",&tolua_err);
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
   peak::ListBox* tolua_ret = (peak::ListBox*)  Mtolua_new((peak::ListBox)());
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
   peak::ListBox* tolua_ret = (peak::ListBox*)  Mtolua_new((peak::ListBox)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::ListBox");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'show'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hide'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'choose'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSelected'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addItem'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'deleteItem'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getItemCount'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getItem'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clearItems'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'updateItems'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAction'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAction'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getType'", NULL);
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
   peak::Sound* tolua_ret = (peak::Sound*)  Mtolua_new((peak::Sound)());
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
   peak::Sound* tolua_ret = (peak::Sound*)  Mtolua_new((peak::Sound)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Sound");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroy'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSource'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setLooped'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLooped'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setCleanUp'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCleanUp'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPaused'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isPaused'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPlaying'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isPlaying'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setVolume'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVolume'", NULL);
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
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Sound* self = (peak::Sound*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D pos = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPosition'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPosition'", NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getPosition();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Vector3D)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Sound* self = (peak::Sound*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D vel = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setVelocity'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVelocity'", NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getVelocity();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Vector3D)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRelativeToListener'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isRelativeToListener'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'attachToSceneNode'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'doWork'", NULL);
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
   peak::Listener* tolua_ret = (peak::Listener*)  Mtolua_new((peak::Listener)());
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
   peak::Listener* tolua_ret = (peak::Listener*)  Mtolua_new((peak::Listener)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"peak::Listener");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
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
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Listener* self = (peak::Listener*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D pos = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPosition'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPosition'", NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getPosition();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Vector3D)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Vector3D",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Listener* self = (peak::Listener*)  tolua_tousertype(tolua_S,1,0);
  peak::Vector3D vel = *((peak::Vector3D*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setVelocity'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVelocity'", NULL);
#endif
  {
   peak::Vector3D tolua_ret = (peak::Vector3D)  self->getVelocity();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Vector3D)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Vector3D));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Vector3D");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"peak::Quaternion",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  peak::Listener* self = (peak::Listener*)  tolua_tousertype(tolua_S,1,0);
  peak::Quaternion rot = *((peak::Quaternion*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRotation'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRotation'", NULL);
#endif
  {
   peak::Quaternion tolua_ret = (peak::Quaternion)  self->getRotation();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((peak::Quaternion)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Quaternion");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(peak::Quaternion));
     tolua_pushusertype(tolua_S,tolua_obj,"peak::Quaternion");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'attachToSceneNode'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'doWork'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLength'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isSeekable'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isStreamed'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'shutdown'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'running'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'play2D'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'play2D'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'play3D'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'play3D'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getListener'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'doWork'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFile'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getHelloWorld'", NULL);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeSource'", NULL);
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
    tolua_function(tolua_S,"rotate",tolua_peak_peak_Vector3D_rotate00);
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
    tolua_function(tolua_S,"getEuler",tolua_peak_peak_Quaternion_getEuler00);
    tolua_function(tolua_S,"rotateVector",tolua_peak_peak_Quaternion_rotateVector00);
    tolua_function(tolua_S,"slerp",tolua_peak_peak_Quaternion_slerp00);
    tolua_function(tolua_S,"getDifference",tolua_peak_peak_Quaternion_getDifference00);
    tolua_variable(tolua_S,"x",tolua_get_peak__Quaternion_x,tolua_set_peak__Quaternion_x);
    tolua_variable(tolua_S,"y",tolua_get_peak__Quaternion_y,tolua_set_peak__Quaternion_y);
    tolua_variable(tolua_S,"z",tolua_get_peak__Quaternion_z,tolua_set_peak__Quaternion_z);
    tolua_variable(tolua_S,"w",tolua_get_peak__Quaternion_w,tolua_set_peak__Quaternion_w);
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
    tolua_function(tolua_S,"runString",tolua_peak_peak_Script_runString00);
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
   tolua_cclass(tolua_S,"World","peak::World","peak::GameObject",tolua_collect_peak__World);
   #else
   tolua_cclass(tolua_S,"World","peak::World","peak::GameObject",NULL);
   #endif
   tolua_beginmodule(tolua_S,"World");
    tolua_function(tolua_S,"new",tolua_peak_peak_World_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_World_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_World_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_World_delete00);
    tolua_function(tolua_S,"init",tolua_peak_peak_World_init00);
    tolua_function(tolua_S,"destroy",tolua_peak_peak_World_destroy00);
    tolua_function(tolua_S,"setNetworkID",tolua_peak_peak_World_setNetworkID00);
    tolua_function(tolua_S,"getNetworkID",tolua_peak_peak_World_getNetworkID00);
    tolua_function(tolua_S,"getWorld",tolua_peak_peak_World_getWorld00);
    tolua_function(tolua_S,"setSynchronized",tolua_peak_peak_World_setSynchronized00);
    tolua_function(tolua_S,"getSynchronized",tolua_peak_peak_World_getSynchronized00);
    tolua_function(tolua_S,"addBody",tolua_peak_peak_World_addBody00);
    tolua_function(tolua_S,"removeBody",tolua_peak_peak_World_removeBody00);
    tolua_function(tolua_S,"addSynchronizedBody",tolua_peak_peak_World_addSynchronizedBody00);
    tolua_function(tolua_S,"update",tolua_peak_peak_World_update00);
    tolua_function(tolua_S,"sendUpdates",tolua_peak_peak_World_sendUpdates00);
    tolua_function(tolua_S,"getBodyNetworkID",tolua_peak_peak_World_getBodyNetworkID00);
    tolua_function(tolua_S,"castRay",tolua_peak_peak_World_castRay00);
    tolua_function(tolua_S,"doWork",tolua_peak_peak_World_doWork00);
    tolua_function(tolua_S,"doAllWork",tolua_peak_peak_World_doAllWork00);
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"NetworkHost","peak::NetworkHost","",NULL);
   tolua_beginmodule(tolua_S,"NetworkHost");
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"NetworkClient","peak::NetworkClient","",NULL);
   tolua_beginmodule(tolua_S,"NetworkClient");
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Entity","peak::Entity","peak::GameObject",tolua_collect_peak__Entity);
   #else
   tolua_cclass(tolua_S,"Entity","peak::Entity","peak::GameObject",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Entity");
    tolua_function(tolua_S,"new",tolua_peak_peak_Entity_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Entity_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Entity_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_Entity_delete00);
    tolua_function(tolua_S,"load",tolua_peak_peak_Entity_load00);
    tolua_function(tolua_S,"setOwner",tolua_peak_peak_Entity_setOwner00);
    tolua_function(tolua_S,"getOwner",tolua_peak_peak_Entity_getOwner00);
    tolua_function(tolua_S,"setLocal",tolua_peak_peak_Entity_setLocal00);
    tolua_function(tolua_S,"isLocal",tolua_peak_peak_Entity_isLocal00);
    tolua_function(tolua_S,"setID",tolua_peak_peak_Entity_setID00);
    tolua_function(tolua_S,"getID",tolua_peak_peak_Entity_getID00);
    tolua_function(tolua_S,"writeStartData",tolua_peak_peak_Entity_writeStartData00);
    tolua_function(tolua_S,"setPosition",tolua_peak_peak_Entity_setPosition00);
    tolua_function(tolua_S,"getName",tolua_peak_peak_Entity_getName00);
    tolua_function(tolua_S,"getBody",tolua_peak_peak_Entity_getBody00);
    tolua_function(tolua_S,"getSceneNode",tolua_peak_peak_Entity_getSceneNode00);
    tolua_function(tolua_S,"getScript",tolua_peak_peak_Entity_getScript00);
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
   tolua_cclass(tolua_S,"ClientInfo","peak::ClientInfo","",tolua_collect_peak__ClientInfo);
   #else
   tolua_cclass(tolua_S,"ClientInfo","peak::ClientInfo","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"ClientInfo");
    tolua_function(tolua_S,"new",tolua_peak_peak_ClientInfo_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_ClientInfo_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_ClientInfo_new00_local);
    tolua_variable(tolua_S,"conn",tolua_get_peak__ClientInfo_conn_ptr,tolua_set_peak__ClientInfo_conn_ptr);
    tolua_variable(tolua_S,"owner",tolua_get_peak__ClientInfo_owner,tolua_set_peak__ClientInfo_owner);
    tolua_variable(tolua_S,"focus",tolua_get_peak__ClientInfo_focus,tolua_set_peak__ClientInfo_focus);
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
    tolua_function(tolua_S,"setFocus",tolua_peak_peak_Game_setFocus00);
    tolua_function(tolua_S,"getFocus",tolua_peak_peak_Game_getFocus00);
    tolua_function(tolua_S,"getClientCount",tolua_peak_peak_Game_getClientCount00);
    tolua_function(tolua_S,"getClientFocus",tolua_peak_peak_Game_getClientFocus00);
    tolua_function(tolua_S,"getClientConnection",tolua_peak_peak_Game_getClientConnection00);
    tolua_function(tolua_S,"getServerConnection",tolua_peak_peak_Game_getServerConnection00);
    tolua_function(tolua_S,"createEntity",tolua_peak_peak_Game_createEntity00);
    tolua_function(tolua_S,"getEntity",tolua_peak_peak_Game_getEntity00);
    tolua_function(tolua_S,"deleteEntity",tolua_peak_peak_Game_deleteEntity00);
    tolua_function(tolua_S,"getEntityCount",tolua_peak_peak_Game_getEntityCount00);
    tolua_function(tolua_S,"getEntity",tolua_peak_peak_Game_getEntity01);
    tolua_function(tolua_S,"getWorld",tolua_peak_peak_Game_getWorld00);
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
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Event","peak::Event","",tolua_collect_peak__Event);
   #else
   tolua_cclass(tolua_S,"Event","peak::Event","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Event");
    tolua_function(tolua_S,"new",tolua_peak_peak_Event_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Event_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Event_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_Event_delete00);
    tolua_function(tolua_S,"trigger",tolua_peak_peak_Event_trigger00);
    tolua_function(tolua_S,"getData",tolua_peak_peak_Event_getData00);
    tolua_function(tolua_S,"addLink",tolua_peak_peak_Event_addLink00);
    tolua_function(tolua_S,"removeLink",tolua_peak_peak_Event_removeLink00);
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"InputManager","peak::InputManager","peak::GameObject",NULL);
   tolua_beginmodule(tolua_S,"InputManager");
    tolua_function(tolua_S,"get",tolua_peak_peak_InputManager_get00);
    tolua_function(tolua_S,"init",tolua_peak_peak_InputManager_init00);
    tolua_function(tolua_S,"shutdown",tolua_peak_peak_InputManager_shutdown00);
    tolua_function(tolua_S,"keyboardEvent",tolua_peak_peak_InputManager_keyboardEvent00);
    tolua_function(tolua_S,"mouseMoved",tolua_peak_peak_InputManager_mouseMoved00);
    tolua_function(tolua_S,"setInputEnabled",tolua_peak_peak_InputManager_setInputEnabled00);
    tolua_function(tolua_S,"getInputEnabled",tolua_peak_peak_InputManager_getInputEnabled00);
    tolua_function(tolua_S,"doWork",tolua_peak_peak_InputManager_doWork00);
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
   tolua_function(tolua_S,"toString",tolua_peak_peak_toString05);
   tolua_function(tolua_S,"toWString",tolua_peak_peak_toWString00);
   tolua_function(tolua_S,"trimFilename",tolua_peak_peak_trimFilename00);
   tolua_function(tolua_S,"getExtension",tolua_peak_peak_getExtension00);
   tolua_function(tolua_S,"lowerCase",tolua_peak_peak_lowerCase00);
   tolua_function(tolua_S,"equals",tolua_peak_peak_equals00);
   tolua_function(tolua_S,"correct",tolua_peak_peak_correct00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"SceneNode","peak::SceneNode","peak::GameObject",tolua_collect_peak__SceneNode);
   #else
   tolua_cclass(tolua_S,"SceneNode","peak::SceneNode","peak::GameObject",NULL);
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
    tolua_function(tolua_S,"getParent",tolua_peak_peak_SceneNode_getParent00);
    tolua_function(tolua_S,"setVisible",tolua_peak_peak_SceneNode_setVisible00);
    tolua_function(tolua_S,"isVisible",tolua_peak_peak_SceneNode_isVisible00);
    tolua_function(tolua_S,"getBoundingBox",tolua_peak_peak_SceneNode_getBoundingBox00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Shape","peak::Shape","peak::GameObject",tolua_collect_peak__Shape);
   #else
   tolua_cclass(tolua_S,"Shape","peak::Shape","peak::GameObject",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Shape");
    tolua_function(tolua_S,"new",tolua_peak_peak_Shape_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Shape_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Shape_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_Shape_delete00);
    tolua_function(tolua_S,"destroy",tolua_peak_peak_Shape_destroy00);
    tolua_function(tolua_S,"getMass",tolua_peak_peak_Shape_getMass00);
    tolua_function(tolua_S,"getInertia",tolua_peak_peak_Shape_getInertia00);
    tolua_function(tolua_S,"getBoundingSphere",tolua_peak_peak_Shape_getBoundingSphere00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Body","peak::Body","peak::GameObject",tolua_collect_peak__Body);
   #else
   tolua_cclass(tolua_S,"Body","peak::Body","peak::GameObject",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Body");
    tolua_function(tolua_S,"new",tolua_peak_peak_Body_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Body_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Body_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_Body_delete00);
    tolua_function(tolua_S,"create",tolua_peak_peak_Body_create00);
    tolua_function(tolua_S,"destroy",tolua_peak_peak_Body_destroy00);
    tolua_function(tolua_S,"setPosition",tolua_peak_peak_Body_setPosition00);
    tolua_function(tolua_S,"getPosition",tolua_peak_peak_Body_getPosition00);
    tolua_function(tolua_S,"setRotation",tolua_peak_peak_Body_setRotation00);
    tolua_function(tolua_S,"getRotation",tolua_peak_peak_Body_getRotation00);
    tolua_function(tolua_S,"setForce",tolua_peak_peak_Body_setForce00);
    tolua_function(tolua_S,"setForce",tolua_peak_peak_Body_setForce01);
    tolua_function(tolua_S,"clearForces",tolua_peak_peak_Body_clearForces00);
    tolua_function(tolua_S,"setLinearVelocity",tolua_peak_peak_Body_setLinearVelocity00);
    tolua_function(tolua_S,"getLinearVelocity",tolua_peak_peak_Body_getLinearVelocity00);
    tolua_function(tolua_S,"setAngularVelocity",tolua_peak_peak_Body_setAngularVelocity00);
    tolua_function(tolua_S,"getAngularVelocity",tolua_peak_peak_Body_getAngularVelocity00);
    tolua_function(tolua_S,"setGravity",tolua_peak_peak_Body_setGravity00);
    tolua_function(tolua_S,"setGravity",tolua_peak_peak_Body_setGravity01);
    tolua_function(tolua_S,"getGravity",tolua_peak_peak_Body_getGravity00);
    tolua_function(tolua_S,"setID",tolua_peak_peak_Body_setID00);
    tolua_function(tolua_S,"getID",tolua_peak_peak_Body_getID00);
    tolua_function(tolua_S,"getCollisionBody",tolua_peak_peak_Body_getCollisionBody00);
    tolua_function(tolua_S,"setEntity",tolua_peak_peak_Body_setEntity00);
    tolua_function(tolua_S,"getEntity",tolua_peak_peak_Body_getEntity00);
    tolua_function(tolua_S,"getShape",tolua_peak_peak_Body_getShape00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Terrain","peak::Terrain","",tolua_collect_peak__Terrain);
   #else
   tolua_cclass(tolua_S,"Terrain","peak::Terrain","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Terrain");
    tolua_function(tolua_S,"new",tolua_peak_peak_Terrain_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Terrain_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Terrain_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_Terrain_delete00);
    tolua_function(tolua_S,"create",tolua_peak_peak_Terrain_create00);
    tolua_function(tolua_S,"destroy",tolua_peak_peak_Terrain_destroy00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"ShapeData","peak::ShapeData","",tolua_collect_peak__ShapeData);
   #else
   tolua_cclass(tolua_S,"ShapeData","peak::ShapeData","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"ShapeData");
    tolua_function(tolua_S,"new",tolua_peak_peak_ShapeData_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_ShapeData_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_ShapeData_new00_local);
    tolua_variable(tolua_S,"body",tolua_get_peak__ShapeData_body_ptr,tolua_set_peak__ShapeData_body_ptr);
    tolua_variable(tolua_S,"shape",tolua_get_peak__ShapeData_shape_ptr,tolua_set_peak__ShapeData_shape_ptr);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"PointGroup","peak::PointGroup","",tolua_collect_peak__PointGroup);
   #else
   tolua_cclass(tolua_S,"PointGroup","peak::PointGroup","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"PointGroup");
    tolua_function(tolua_S,"new",tolua_peak_peak_PointGroup_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_PointGroup_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_PointGroup_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_PointGroup_delete00);
    tolua_function(tolua_S,"addPoint",tolua_peak_peak_PointGroup_addPoint00);
    tolua_function(tolua_S,"getPointCount",tolua_peak_peak_PointGroup_getPointCount00);
    tolua_function(tolua_S,"getPoint",tolua_peak_peak_PointGroup_getPoint00);
    tolua_function(tolua_S,"deletePoint",tolua_peak_peak_PointGroup_deletePoint00);
    tolua_function(tolua_S,"addRotation",tolua_peak_peak_PointGroup_addRotation00);
    tolua_function(tolua_S,"getRotationCount",tolua_peak_peak_PointGroup_getRotationCount00);
    tolua_function(tolua_S,"getRotation",tolua_peak_peak_PointGroup_getRotation00);
    tolua_function(tolua_S,"deleteRotation",tolua_peak_peak_PointGroup_deleteRotation00);
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
    tolua_function(tolua_S,"getPointGroup",tolua_peak_peak_Level_getPointGroup00);
    tolua_function(tolua_S,"getScript",tolua_peak_peak_Level_getScript00);
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
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Link","peak::Link","",tolua_collect_peak__Link);
   #else
   tolua_cclass(tolua_S,"Link","peak::Link","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Link");
    tolua_function(tolua_S,"new",tolua_peak_peak_Link_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Link_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Link_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_Link_delete00);
    tolua_function(tolua_S,"create",tolua_peak_peak_Link_create00);
    tolua_function(tolua_S,"destroy",tolua_peak_peak_Link_destroy00);
    tolua_function(tolua_S,"setActive",tolua_peak_peak_Link_setActive00);
    tolua_function(tolua_S,"isActive",tolua_peak_peak_Link_isActive00);
   tolua_endmodule(tolua_S);
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
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   tolua_constant(tolua_S,"EPT_Vector3D",peak::EPT_Vector3D);
   tolua_constant(tolua_S,"EPT_Vector2D",peak::EPT_Vector2D);
   tolua_constant(tolua_S,"EPT_Float",peak::EPT_Float);
   tolua_constant(tolua_S,"EPT_Boolean",peak::EPT_Boolean);
   tolua_constant(tolua_S,"EPT_Integer",peak::EPT_Integer);
   tolua_constant(tolua_S,"EPT_Quaternion",peak::EPT_Quaternion);
   tolua_constant(tolua_S,"EPT_Array",peak::EPT_Array);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"GameObject","peak::GameObject","",tolua_collect_peak__GameObject);
   #else
   tolua_cclass(tolua_S,"GameObject","peak::GameObject","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"GameObject");
    tolua_function(tolua_S,"new",tolua_peak_peak_GameObject_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_GameObject_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_GameObject_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_GameObject_delete00);
    tolua_function(tolua_S,"getProperty",tolua_peak_peak_GameObject_getProperty00);
    tolua_function(tolua_S,"getEvent",tolua_peak_peak_GameObject_getEvent00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Property","peak::Property","ReferenceCounted",tolua_collect_peak__Property);
   #else
   tolua_cclass(tolua_S,"Property","peak::Property","ReferenceCounted",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Property");
    tolua_function(tolua_S,"delete",tolua_peak_peak_Property_delete00);
    tolua_function(tolua_S,"setName",tolua_peak_peak_Property_setName00);
    tolua_function(tolua_S,"getName",tolua_peak_peak_Property_getName00);
    tolua_function(tolua_S,"getType",tolua_peak_peak_Property_getType00);
    tolua_function(tolua_S,"set",tolua_peak_peak_Property_set00);
    tolua_function(tolua_S,"getVector3D",tolua_peak_peak_Property_getVector3D00);
    tolua_function(tolua_S,"set",tolua_peak_peak_Property_set01);
    tolua_function(tolua_S,"getVector2D",tolua_peak_peak_Property_getVector2D00);
    tolua_function(tolua_S,"set",tolua_peak_peak_Property_set02);
    tolua_function(tolua_S,"getQuaternion",tolua_peak_peak_Property_getQuaternion00);
    tolua_function(tolua_S,"set",tolua_peak_peak_Property_set03);
    tolua_function(tolua_S,"getBool",tolua_peak_peak_Property_getBool00);
    tolua_function(tolua_S,"set",tolua_peak_peak_Property_set04);
    tolua_function(tolua_S,"getInt",tolua_peak_peak_Property_getInt00);
    tolua_function(tolua_S,"set",tolua_peak_peak_Property_set05);
    tolua_function(tolua_S,"getFloat",tolua_peak_peak_Property_getFloat00);
    tolua_function(tolua_S,"set",tolua_peak_peak_Property_set06);
    tolua_function(tolua_S,"getValueArray",tolua_peak_peak_Property_getValueArray00);
    tolua_function(tolua_S,"set",tolua_peak_peak_Property_set07);
    tolua_function(tolua_S,"serialize",tolua_peak_peak_Property_serialize00);
    tolua_function(tolua_S,"deserialize",tolua_peak_peak_Property_deserialize00);
    tolua_function(tolua_S,"setGameObject",tolua_peak_peak_Property_setGameObject00);
    tolua_function(tolua_S,"getGameObject",tolua_peak_peak_Property_getGameObject00);
    tolua_function(tolua_S,"addLink",tolua_peak_peak_Property_addLink00);
    tolua_function(tolua_S,"removeLink",tolua_peak_peak_Property_removeLink00);
    tolua_function(tolua_S,"update",tolua_peak_peak_Property_update00);
    tolua_function(tolua_S,"disableUpdates",tolua_peak_peak_Property_disableUpdates00);
    tolua_function(tolua_S,"enableUpdates",tolua_peak_peak_Property_enableUpdates00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Vector3DProperty","peak::Vector3DProperty","peak::Property",tolua_collect_peak__Vector3DProperty);
   #else
   tolua_cclass(tolua_S,"Vector3DProperty","peak::Vector3DProperty","peak::Property",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Vector3DProperty");
    tolua_function(tolua_S,"new",tolua_peak_peak_Vector3DProperty_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Vector3DProperty_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Vector3DProperty_new00_local);
    tolua_function(tolua_S,"getType",tolua_peak_peak_Vector3DProperty_getType00);
    tolua_function(tolua_S,"set",tolua_peak_peak_Vector3DProperty_set00);
    tolua_function(tolua_S,"getVector3D",tolua_peak_peak_Vector3DProperty_getVector3D00);
    tolua_function(tolua_S,"set",tolua_peak_peak_Vector3DProperty_set01);
    tolua_function(tolua_S,"getValueArray",tolua_peak_peak_Vector3DProperty_getValueArray00);
    tolua_function(tolua_S,"set",tolua_peak_peak_Vector3DProperty_set02);
    tolua_function(tolua_S,"serialize",tolua_peak_peak_Vector3DProperty_serialize00);
    tolua_function(tolua_S,"deserialize",tolua_peak_peak_Vector3DProperty_deserialize00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Vector2DProperty","peak::Vector2DProperty","peak::Property",tolua_collect_peak__Vector2DProperty);
   #else
   tolua_cclass(tolua_S,"Vector2DProperty","peak::Vector2DProperty","peak::Property",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Vector2DProperty");
    tolua_function(tolua_S,"new",tolua_peak_peak_Vector2DProperty_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Vector2DProperty_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Vector2DProperty_new00_local);
    tolua_function(tolua_S,"getType",tolua_peak_peak_Vector2DProperty_getType00);
    tolua_function(tolua_S,"set",tolua_peak_peak_Vector2DProperty_set00);
    tolua_function(tolua_S,"getVector2D",tolua_peak_peak_Vector2DProperty_getVector2D00);
    tolua_function(tolua_S,"set",tolua_peak_peak_Vector2DProperty_set01);
    tolua_function(tolua_S,"getValueArray",tolua_peak_peak_Vector2DProperty_getValueArray00);
    tolua_function(tolua_S,"set",tolua_peak_peak_Vector2DProperty_set02);
    tolua_function(tolua_S,"serialize",tolua_peak_peak_Vector2DProperty_serialize00);
    tolua_function(tolua_S,"deserialize",tolua_peak_peak_Vector2DProperty_deserialize00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"QuaternionProperty","peak::QuaternionProperty","peak::Property",tolua_collect_peak__QuaternionProperty);
   #else
   tolua_cclass(tolua_S,"QuaternionProperty","peak::QuaternionProperty","peak::Property",NULL);
   #endif
   tolua_beginmodule(tolua_S,"QuaternionProperty");
    tolua_function(tolua_S,"new",tolua_peak_peak_QuaternionProperty_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_QuaternionProperty_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_QuaternionProperty_new00_local);
    tolua_function(tolua_S,"getType",tolua_peak_peak_QuaternionProperty_getType00);
    tolua_function(tolua_S,"set",tolua_peak_peak_QuaternionProperty_set00);
    tolua_function(tolua_S,"getQuaternion",tolua_peak_peak_QuaternionProperty_getQuaternion00);
    tolua_function(tolua_S,"set",tolua_peak_peak_QuaternionProperty_set01);
    tolua_function(tolua_S,"getValueArray",tolua_peak_peak_QuaternionProperty_getValueArray00);
    tolua_function(tolua_S,"set",tolua_peak_peak_QuaternionProperty_set02);
    tolua_function(tolua_S,"serialize",tolua_peak_peak_QuaternionProperty_serialize00);
    tolua_function(tolua_S,"deserialize",tolua_peak_peak_QuaternionProperty_deserialize00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"IntProperty","peak::IntProperty","peak::Property",tolua_collect_peak__IntProperty);
   #else
   tolua_cclass(tolua_S,"IntProperty","peak::IntProperty","peak::Property",NULL);
   #endif
   tolua_beginmodule(tolua_S,"IntProperty");
    tolua_function(tolua_S,"new",tolua_peak_peak_IntProperty_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_IntProperty_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_IntProperty_new00_local);
    tolua_function(tolua_S,"getType",tolua_peak_peak_IntProperty_getType00);
    tolua_function(tolua_S,"set",tolua_peak_peak_IntProperty_set00);
    tolua_function(tolua_S,"getInt",tolua_peak_peak_IntProperty_getInt00);
    tolua_function(tolua_S,"set",tolua_peak_peak_IntProperty_set01);
    tolua_function(tolua_S,"getFloat",tolua_peak_peak_IntProperty_getFloat00);
    tolua_function(tolua_S,"set",tolua_peak_peak_IntProperty_set02);
    tolua_function(tolua_S,"serialize",tolua_peak_peak_IntProperty_serialize00);
    tolua_function(tolua_S,"deserialize",tolua_peak_peak_IntProperty_deserialize00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"DirectLink","peak::DirectLink","peak::Link",tolua_collect_peak__DirectLink);
   #else
   tolua_cclass(tolua_S,"DirectLink","peak::DirectLink","peak::Link",NULL);
   #endif
   tolua_beginmodule(tolua_S,"DirectLink");
    tolua_function(tolua_S,"new",tolua_peak_peak_DirectLink_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_DirectLink_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_DirectLink_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_DirectLink_delete00);
    tolua_function(tolua_S,"create",tolua_peak_peak_DirectLink_create00);
    tolua_function(tolua_S,"destroy",tolua_peak_peak_DirectLink_destroy00);
    tolua_function(tolua_S,"update",tolua_peak_peak_DirectLink_update00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"EventData","peak::EventData","",tolua_collect_peak__EventData);
   #else
   tolua_cclass(tolua_S,"EventData","peak::EventData","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"EventData");
    tolua_function(tolua_S,"new",tolua_peak_peak_EventData_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_EventData_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_EventData_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_EventData_delete00);
    tolua_function(tolua_S,"getName",tolua_peak_peak_EventData_getName00);
    tolua_function(tolua_S,"set",tolua_peak_peak_EventData_set00);
    tolua_function(tolua_S,"set",tolua_peak_peak_EventData_set01);
    tolua_function(tolua_S,"apply",tolua_peak_peak_EventData_apply00);
    tolua_function(tolua_S,"apply",tolua_peak_peak_EventData_apply01);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"EventScriptLink","peak::EventScriptLink","peak::Link",tolua_collect_peak__EventScriptLink);
   #else
   tolua_cclass(tolua_S,"EventScriptLink","peak::EventScriptLink","peak::Link",NULL);
   #endif
   tolua_beginmodule(tolua_S,"EventScriptLink");
    tolua_function(tolua_S,"new",tolua_peak_peak_EventScriptLink_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_EventScriptLink_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_EventScriptLink_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_EventScriptLink_delete00);
    tolua_function(tolua_S,"create",tolua_peak_peak_EventScriptLink_create00);
    tolua_function(tolua_S,"destroy",tolua_peak_peak_EventScriptLink_destroy00);
    tolua_function(tolua_S,"update",tolua_peak_peak_EventScriptLink_update00);
    tolua_function(tolua_S,"update",tolua_peak_peak_EventScriptLink_update01);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"EventRemoteLink","peak::EventRemoteLink","peak::Link",tolua_collect_peak__EventRemoteLink);
   #else
   tolua_cclass(tolua_S,"EventRemoteLink","peak::EventRemoteLink","peak::Link",NULL);
   #endif
   tolua_beginmodule(tolua_S,"EventRemoteLink");
    tolua_function(tolua_S,"new",tolua_peak_peak_EventRemoteLink_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_EventRemoteLink_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_EventRemoteLink_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_EventRemoteLink_delete00);
    tolua_function(tolua_S,"create",tolua_peak_peak_EventRemoteLink_create00);
    tolua_function(tolua_S,"destroy",tolua_peak_peak_EventRemoteLink_destroy00);
    tolua_function(tolua_S,"update",tolua_peak_peak_EventRemoteLink_update00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"PropertyScriptLink","peak::PropertyScriptLink","peak::Link",tolua_collect_peak__PropertyScriptLink);
   #else
   tolua_cclass(tolua_S,"PropertyScriptLink","peak::PropertyScriptLink","peak::Link",NULL);
   #endif
   tolua_beginmodule(tolua_S,"PropertyScriptLink");
    tolua_function(tolua_S,"new",tolua_peak_peak_PropertyScriptLink_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_PropertyScriptLink_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_PropertyScriptLink_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_PropertyScriptLink_delete00);
    tolua_function(tolua_S,"create",tolua_peak_peak_PropertyScriptLink_create00);
    tolua_function(tolua_S,"destroy",tolua_peak_peak_PropertyScriptLink_destroy00);
    tolua_function(tolua_S,"update",tolua_peak_peak_PropertyScriptLink_update00);
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
    tolua_function(tolua_S,"render",tolua_peak_peak_CameraSceneNode_render00);
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
    tolua_function(tolua_S,"saveMessages",tolua_peak_peak_GraphicsEngine_saveMessages00);
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
    tolua_function(tolua_S,"delete",tolua_peak_peak_Image_delete00);
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
    tolua_function(tolua_S,"pushAnimation",tolua_peak_peak_MeshSceneNode_pushAnimation00);
    tolua_function(tolua_S,"popAnimation",tolua_peak_peak_MeshSceneNode_popAnimation00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
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
    tolua_function(tolua_S,"delete",tolua_peak_peak_LabelOverlay_delete00);
    tolua_function(tolua_S,"setLabel",tolua_peak_peak_LabelOverlay_setLabel00);
    tolua_function(tolua_S,"getLabel",tolua_peak_peak_LabelOverlay_getLabel00);
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
    tolua_function(tolua_S,"setRotation",tolua_peak_peak_FollowingController_setRotation00);
    tolua_function(tolua_S,"getRotation",tolua_peak_peak_FollowingController_getRotation00);
    tolua_function(tolua_S,"setMode",tolua_peak_peak_FollowingController_setMode00);
    tolua_function(tolua_S,"getMode",tolua_peak_peak_FollowingController_getMode00);
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
   tolua_constant(tolua_S,"ELT_Point",peak::ELT_Point);
   tolua_constant(tolua_S,"ELT_Direction",peak::ELT_Direction);
   tolua_constant(tolua_S,"ELT_Spot",peak::ELT_Spot);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Light","peak::Light","peak::SceneNode",tolua_collect_peak__Light);
   #else
   tolua_cclass(tolua_S,"Light","peak::Light","peak::SceneNode",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Light");
    tolua_function(tolua_S,"new",tolua_peak_peak_Light_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Light_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Light_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_Light_delete00);
    tolua_function(tolua_S,"setType",tolua_peak_peak_Light_setType00);
    tolua_function(tolua_S,"getType",tolua_peak_peak_Light_getType00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"ParticleSystem","peak::ParticleSystem","peak::SceneNode",tolua_collect_peak__ParticleSystem);
   #else
   tolua_cclass(tolua_S,"ParticleSystem","peak::ParticleSystem","peak::SceneNode",NULL);
   #endif
   tolua_beginmodule(tolua_S,"ParticleSystem");
    tolua_function(tolua_S,"new",tolua_peak_peak_ParticleSystem_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_ParticleSystem_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_ParticleSystem_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_ParticleSystem_delete00);
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
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Box","peak::Box","peak::Shape",tolua_collect_peak__Box);
   #else
   tolua_cclass(tolua_S,"Box","peak::Box","peak::Shape",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Box");
    tolua_function(tolua_S,"new",tolua_peak_peak_Box_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Box_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Box_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_Box_delete00);
    tolua_function(tolua_S,"create",tolua_peak_peak_Box_create00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Plane","peak::Plane","peak::Shape",tolua_collect_peak__Plane);
   #else
   tolua_cclass(tolua_S,"Plane","peak::Plane","peak::Shape",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Plane");
    tolua_function(tolua_S,"new",tolua_peak_peak_Plane_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Plane_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Plane_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_Plane_delete00);
    tolua_function(tolua_S,"create",tolua_peak_peak_Plane_create00);
    tolua_function(tolua_S,"create",tolua_peak_peak_Plane_create01);
    tolua_function(tolua_S,"create",tolua_peak_peak_Plane_create02);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Trimesh","peak::Trimesh","peak::Shape",tolua_collect_peak__Trimesh);
   #else
   tolua_cclass(tolua_S,"Trimesh","peak::Trimesh","peak::Shape",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Trimesh");
    tolua_function(tolua_S,"new",tolua_peak_peak_Trimesh_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Trimesh_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Trimesh_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_Trimesh_delete00);
    tolua_function(tolua_S,"create",tolua_peak_peak_Trimesh_create00);
    tolua_function(tolua_S,"create",tolua_peak_peak_Trimesh_create01);
    tolua_function(tolua_S,"destroy",tolua_peak_peak_Trimesh_destroy00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Sphere","peak::Sphere","peak::Shape",tolua_collect_peak__Sphere);
   #else
   tolua_cclass(tolua_S,"Sphere","peak::Sphere","peak::Shape",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Sphere");
    tolua_function(tolua_S,"new",tolua_peak_peak_Sphere_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Sphere_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Sphere_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_Sphere_delete00);
    tolua_function(tolua_S,"create",tolua_peak_peak_Sphere_create00);
    tolua_function(tolua_S,"destroy",tolua_peak_peak_Sphere_destroy00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   tolua_constant(tolua_S,"ECT_None",peak::ECT_None);
   tolua_constant(tolua_S,"ECT_Ray",peak::ECT_Ray);
   tolua_constant(tolua_S,"ECT_Convex",peak::ECT_Convex);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Callback","peak::Callback","",tolua_collect_peak__Callback);
   #else
   tolua_cclass(tolua_S,"Callback","peak::Callback","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Callback");
    tolua_function(tolua_S,"new",tolua_peak_peak_Callback_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Callback_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Callback_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_Callback_delete00);
    tolua_function(tolua_S,"create",tolua_peak_peak_Callback_create00);
    tolua_function(tolua_S,"destroy",tolua_peak_peak_Callback_destroy00);
    tolua_function(tolua_S,"update",tolua_peak_peak_Callback_update00);
    tolua_function(tolua_S,"updateAll",tolua_peak_peak_Callback_updateAll00);
    tolua_function(tolua_S,"getType",tolua_peak_peak_Callback_getType00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"CollisionInfo","peak::CollisionInfo","",tolua_collect_peak__CollisionInfo);
   #else
   tolua_cclass(tolua_S,"CollisionInfo","peak::CollisionInfo","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"CollisionInfo");
    tolua_function(tolua_S,"new",tolua_peak_peak_CollisionInfo_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_CollisionInfo_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_CollisionInfo_new00_local);
    tolua_variable(tolua_S,"lambda",tolua_get_peak__CollisionInfo_lambda,tolua_set_peak__CollisionInfo_lambda);
    tolua_variable(tolua_S,"point",tolua_get_peak__CollisionInfo_point,tolua_set_peak__CollisionInfo_point);
    tolua_variable(tolua_S,"object",tolua_get_peak__CollisionInfo_object_ptr,tolua_set_peak__CollisionInfo_object_ptr);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"RayCallback","peak::RayCallback","peak::Callback",tolua_collect_peak__RayCallback);
   #else
   tolua_cclass(tolua_S,"RayCallback","peak::RayCallback","peak::Callback",NULL);
   #endif
   tolua_beginmodule(tolua_S,"RayCallback");
    tolua_function(tolua_S,"new",tolua_peak_peak_RayCallback_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_RayCallback_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_RayCallback_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_RayCallback_delete00);
    tolua_function(tolua_S,"create",tolua_peak_peak_RayCallback_create00);
    tolua_function(tolua_S,"destroy",tolua_peak_peak_RayCallback_destroy00);
    tolua_function(tolua_S,"update",tolua_peak_peak_RayCallback_update00);
    tolua_function(tolua_S,"isCollision",tolua_peak_peak_RayCallback_isCollision00);
    tolua_function(tolua_S,"setRay",tolua_peak_peak_RayCallback_setRay00);
    tolua_function(tolua_S,"getRay",tolua_peak_peak_RayCallback_getRay00);
    tolua_function(tolua_S,"setInfo",tolua_peak_peak_RayCallback_setInfo00);
    tolua_function(tolua_S,"getInfo",tolua_peak_peak_RayCallback_getInfo00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"peak",0);
  tolua_beginmodule(tolua_S,"peak");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Heightfield","peak::Heightfield","peak::Shape",tolua_collect_peak__Heightfield);
   #else
   tolua_cclass(tolua_S,"Heightfield","peak::Heightfield","peak::Shape",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Heightfield");
    tolua_function(tolua_S,"new",tolua_peak_peak_Heightfield_new00);
    tolua_function(tolua_S,"new_local",tolua_peak_peak_Heightfield_new00_local);
    tolua_function(tolua_S,".call",tolua_peak_peak_Heightfield_new00_local);
    tolua_function(tolua_S,"delete",tolua_peak_peak_Heightfield_delete00);
    tolua_function(tolua_S,"create",tolua_peak_peak_Heightfield_create00);
    tolua_function(tolua_S,"create",tolua_peak_peak_Heightfield_create01);
    tolua_function(tolua_S,"destroy",tolua_peak_peak_Heightfield_destroy00);
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
   tolua_cclass(tolua_S,"ImageLoader","peak::ImageLoader","",NULL);
   tolua_beginmodule(tolua_S,"ImageLoader");
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"GUIInput","peak::GUIInput","",NULL);
   tolua_beginmodule(tolua_S,"GUIInput");
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
    tolua_function(tolua_S,"getType",tolua_peak_peak_MenuElement_getType00);
    tolua_function(tolua_S,"getWidget",tolua_peak_peak_MenuElement_getWidget00);
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
   tolua_cclass(tolua_S,"ListModel","peak::ListModel","",NULL);
   tolua_beginmodule(tolua_S,"ListModel");
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"ListBoxListener","peak::ListBoxListener","",NULL);
   tolua_beginmodule(tolua_S,"ListBoxListener");
   tolua_endmodule(tolua_S);
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

