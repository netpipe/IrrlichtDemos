#ifndef CLASS_BLOOD_SHADER_H_INC
#define CLASS_BLOOD_SHADER_H_INC

#include <irrlicht.h>

class CBloodShader : public irr::video::IShaderConstantSetCallBack
{
private :

  CBloodShader();
  CBloodShader(const CBloodShader &);
  CBloodShader& operator = (const CBloodShader &);

  irr::video::IVideoDriver *driver;
  irr::ITimer              *timer;

  irr::s32                 matBlood;

  void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData);

public :

  static CBloodShader& instance()
  {
     static CBloodShader _instance;
     return _instance;
  }

  const irr::s32 createMaterial(irr::IrrlichtDevice* pDevice);
  const irr::s32 getMaterial() { return matBlood; }
};

#endif
