#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_ 1

#include "irrlicht.h"

  //##############################################################################################
  // Light constants
  //const irr::video::SColor CONST_LIGHT_FRNT_COL = irr::video::SColor(255,255,243,195);
  const irr::video::SColor CONST_LIGHT_FRNT_COL = irr::video::SColor(255,0,0,0);
  const irr::video::SColor CONST_LIGHT_BACK_COL = irr::video::SColor(255,143,62,155);
  const irr::video::SColor CONST_LIGHT_FILL_COL = irr::video::SColor(255,109,84,144);

  const irr::core::vector3df CONST_LIGHT_FRNT_POS = irr::core::vector3df(300,600,500);
  const irr::core::vector3df CONST_LIGHT_BACK_POS = irr::core::vector3df(-300,200,-500);
  const irr::core::vector3df CONST_LIGHT_FILL_POS = irr::core::vector3df(300,300,-500);

  //##############################################################################################
  // Default material constants
  const irr::video::SColor CMT_DEFAULT_AMBIENT    = irr::video::SColor(255,140,116,130);
  const irr::video::SColor CMT_DEFAULT_DIFFUSE    = irr::video::SColor(255,229,216,195);
  const irr::video::SColor CMT_DEFAULT_EMISSIVE   = irr::video::SColor(255,236,226,209);
  //const irr::video::SColor CMT_DEFAULT_EMISSIVE   = irr::video::SColor(255,1,1,1);
  const irr::video::SColor CMT_DEFAULT_SPECULAR   = irr::video::SColor(255,255,255,255);
  //const irr::video::SColor CMT_DEFAULT_SPECULAR   = irr::video::SColor(255,229,216,195);

  const irr::f32 CMT_DEFAULT_SHININESS  = 21.0f;

  const bool CMT_DEFAULT_LIGHTING     = true;
  const bool CMT_DEFAULT_GOURAUD      = true;
  const bool CMT_DEFAULT_BCKFACECULL  = true;
  const bool CMT_DEFAULT_NRMNORMALS   = false;
  const bool CMT_DEFAULT_FOGENABLE    = false;

#endif // _CONSTANTS_H_
