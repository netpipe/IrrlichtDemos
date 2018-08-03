#include "constants.h"
using namespace irr;


///for debugging
irr::ILogger* logger;

///
/*
irrklang::ISoundEngine* irrKlang;
irrklang::ISoundEngine* irrsound=0;
irrklang::ISoundSource* canon_sound=0;
irrklang::ISoundSource* clink_sound=0;
irrklang::ISoundSource* impact_sound=0;
irrklang::ISoundSource* targeting_sound=0;
irrklang::ISoundSource* launch_sound=0;
irrklang::ISoundSource* shield_sound=0;
irrklang::ISound* music_sound;
irrklang::ISound* beep_sound;
irrklang::vec3df sound_dir(0,1,0);
*/


//constants for moving calculation
//interface 2d

const irr::f64 collide_distance = 40.0f;
const irr::u32 tour_mask = 64;
const irr::u32 jet_mask = 128;
const irr::u32 tank_mask = 256;
const irr::u32 missile_mask = 1024;
const irr::core::vector3df offset_camera (0.0,30.0,-45.0);
const irr::core::vector3df offset_angle_node(70.0,-30.0,70.0);
const irr::core::vector3df offset_shield(0.0,-30.0,100.0);


///video constants

const irr::f32 correctionx= 1.65, correctiony = 1.5,correctionz = 1.9;
const irr::video::SColor default_skin_color(200, 144, 72,56);


irr::s32 points = 0;
irr::scene::IMeshSceneNode* canon =0;
irr::scene::IMeshSceneNode* angle_node  =0;
irr::video::ITexture* cross_hair = 0;
irr::scene::ISceneNode* SkyBox = 0;
irr::scene::ICameraSceneNode* Camera ={0};
irr::scene::ILightSceneNode *light;
irr::core::stringw bar_message;
irr::core::stringw fps_text;
irr::video::ITexture *skydome;
irr::scene::ISkinnedMesh* ISM2;
irr::scene::IAnimatedMeshSceneNode* terrain  =0;
irr::scene::IShadowVolumeSceneNode *psvsn[20];//for shadow nodes

