#ifndef _C_MANAGED_SHADOW
  #define _C_MANAGED_SHADOW

  #include <irrlicht.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class CShadowManager;

const int MANAGED_SHADOW_ID=MAKE_IRR_ID('m','g','s','d');
const c8 MANAGED_SHADOW_NAME[0xFF]="CManagedShadow";

class CManagedShadow : public ISceneNode {
  private:
    IShadowVolumeSceneNode *m_pShadow;
		ISceneManager *m_pSceneManager;
    u32 m_iLevel;

  public:
    CManagedShadow(ISceneNode *pParent, ISceneManager *pMgr, s32 iId=-1, const vector3df pos=vector3df(0,0,0), const vector3df rot=vector3df(0,0,0), const vector3df scale=vector3df(1,1,1));

    virtual void render() { }

    virtual const aabbox3df &getBoundingBox() const {
      static aabbox3df aBox;
      return aBox;
    }

    void setLevel(u32 iLevel);
    u32 getLevel();

    virtual ESCENE_NODE_TYPE getType() const;

    void update(u32 theLevel);

    virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
    virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options);

		virtual ISceneNode *clone(ISceneNode* newParent=0, ISceneManager* newManager=0);
};

#endif
