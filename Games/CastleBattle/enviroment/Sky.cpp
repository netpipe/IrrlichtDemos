#include "Sky.h"
using namespace irr;

Sky::Sky(scene::ISceneManager* smgr, irr::video::IVideoDriver* driver, core::stringw texture, f32 radius){
    this->sky = smgr->addSkyDomeSceneNode(driver->getTexture(texture.c_str()),16,8,0.95f,2.0f,radius);
}
