#ifndef _C_PROJECTILE
  #define _C_PROJECTILE

  #include <irrlicht.h>
  #include <IrrOde.h>

class CProjectile {
  protected:
    irr::ode::CIrrOdeBody *m_pBody;
    irr::s32 m_iTtl;
    irr::core::vector3df m_vAcc;

  public:
    CProjectile(irr::ode::CIrrOdeBody *pBody, irr::s32 iTtl, irr::core::vector3df vAcc);

    irr::ode::CIrrOdeBody *getBody();
    irr::s32 getTtl();

    void step();
    void collide();
};

class CProjectileManager : irr::ode::IIrrOdeEventListener {
  protected:
    irr::u32 m_iShots,m_iHits;
    irr::core::list<CProjectile *> m_lShots;

    CProjectile *m_pLast;

    CProjectileManager();
    virtual ~CProjectileManager() { }

  public:
    static CProjectileManager *getSharedInstance();

    virtual bool onEvent(irr::ode::IIrrOdeEvent *pEvent);
    virtual bool handlesEvent(irr::ode::IIrrOdeEvent *pEvent);

    void addShot(CProjectile *p);

    irr::u32 getShots() { return m_iShots; }
    irr::u32 getHits () { return m_iHits ; }

    CProjectile *getLast();
};

#endif
