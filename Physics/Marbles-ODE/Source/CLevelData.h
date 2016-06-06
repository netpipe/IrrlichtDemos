#ifndef _CLEVELDATA
  #define _CLEVELDATA

  #include <irrlicht.h>
  //#include <irrklang.h>
  #include <ode/ode.h>
  #include "ode/CIrrOdeManager.h"

  #include "CLevels.h"
  #include "CMarble.h"
  #include "CLevelObject.h"

class CObjects
{
  public:
    int m_iObjCnt;
    irr::core::array<irr::ode::CObjData *> m_pObjectData;

    void loadObjectData(irr::IrrlichtDevice *pDevice);
};

class CLevelData
{
  public:
    typedef struct _CHECK
    {
      CLevelObject *pObject;
      int           iNum;
      int           iRespawnX, iRespawnY, iRespawnZ;

      char sImg[255],sImgOk[255],sLight[50];

      irr::video::ITexture *pImgTexture, *pImgTextureOk;
      irr::gui::IGUIImage *pImg, *pImgOk;
      irr::gui::IGUIEditBox *pEdTime;
    }
    _CHECK;

    int m_iStartX, m_iStartY, m_iStartZ, m_iChallange;
    float m_fRespawn;
    char m_sLevelName[255];
    char m_sGameOver[100];

    irr::core::array<CLevelObject *> m_pObjects;
    irr::core::array<_CHECK       *> m_pChecks ;

    int m_iObjectCnt;
    int m_iCheckCnt;

    static CLevelData *loadLevelData(const char *sFileName, irr::IrrlichtDevice *pDevice ,irr::ode::CIrrOdeManager *pManager, CMarble *pMarble);

    CObjects m_cObjectClasses;

    CLevelData();
    ~CLevelData();
};

#endif
