
#ifndef _CONSTANTS_H_INCLUDED
#define _CONSTANTS_H_INCLUDED
#include <irrlicht.h>
//#include <irrKlang.h>

extern irr::ILogger* logger;
/*
extern irrklang::ISoundEngine* irrsound;
extern irrklang::ISoundSource* canon_sound;
extern irrklang::ISoundSource* clink_sound;
extern irrklang::ISoundSource* impact_sound;
extern irrklang::ISoundSource* targeting_sound;
extern irrklang::ISoundSource* launch_sound;
extern irrklang::ISoundSource* shield_sound;
extern irrklang::ISound* music_sound;
extern irrklang::ISound* beep_sound;
extern irrklang::vec3df sound_dir;
*/
//keep them equal for shadow off-on in init_level except tour number
extern const int tank_number;
extern const int jet_number;
extern const int shell_number;
extern const int obu_number;
extern const int missile_number;
extern const int tower_number;
extern const int number_of_level;


//for video display
extern const irr::f32 correctionx, correctiony,correctionz;
extern const irr::video::SColor default_skin_color;



extern irr::s32 points;
extern irr::scene::IMeshSceneNode* canon;
extern irr::scene::IMeshSceneNode* angle_node;
extern irr::video::ITexture* cross_hair;
extern irr::scene::ISceneNode* SkyBox;
extern irr::scene::ICameraSceneNode* Camera;
extern irr::scene::ILightSceneNode *light;
extern irr::core::stringw bar_message;
extern irr::core::stringw fps_text;
extern irr::video::ITexture *skydome;
extern irr::scene::ISkinnedMesh* ISM2;
extern irr::scene::IAnimatedMeshSceneNode* terrain;
extern irr::scene::IShadowVolumeSceneNode *psvsn[20];

extern const irr::f64 collide_distance;
extern const irr::u32 tour_mask;
extern const irr::u32 jet_mask;
extern const irr::u32 tank_mask;
extern const irr::u32 missile_mask;
extern const irr::core::vector3df offset_camera;
extern const irr::core::vector3df offset_angle_node;
extern const irr::core::vector3df offset_shield;

#endif//constants included
