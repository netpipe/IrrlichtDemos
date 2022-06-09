#ifndef CLASS_WATER_SHADER_H_INC
#define CLASS_WATER_SHADER_H_INC

#include <irrlicht.h>

class CWaterShader : public irr::video::IShaderConstantSetCallBack
{
private :

  CWaterShader();
  CWaterShader(const CWaterShader &);
  CWaterShader& operator = (const CWaterShader &);

  irr::video::IVideoDriver    *driver;
  irr::scene::ISceneManager   *smgr;
  irr::ITimer                 *timer;

  const irr::video::SMaterial *currentMaterial;

  const irr::io::path         matWaterShader;
  irr::s32                    matWater;

  void OnSetMaterial(const irr::video::SMaterial &material);
  void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData);

public :

  static CWaterShader& instance()
  {
     static CWaterShader _instance;
     return _instance;
  }

  const irr::s32 createMaterial(irr::IrrlichtDevice* pDevice);
  const irr::s32 getMaterial() { return matWater; }
};

#endif
