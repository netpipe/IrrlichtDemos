#ifndef _CIRRODEMANAGER
  #define _CIRRODEMANAGER

  #include "IIrrOdeBase.h"
  #include "IIrrOdeEntity.h"
  #include <list>

  #define MAX_CONTACTS 200

namespace irr
{
  namespace ode
  {
    class CIrrOdeManager : public IIrrOdeBase
    {
      private:
        static dWorldID m_iWorld;
        static dSpaceID m_iSpace;
        static dJointGroupID m_iJointGroup;

        IrrlichtDevice *m_pDevice;               /**< Irrlicht Device */

        core::array<IIrrOdeEntity *> m_cObjects;  /**< List of managed objects */
        int m_iObjCnt;                            /**< Number of objects in m_cObjects */

      public:
        CIrrOdeManager(IrrlichtDevice *pDevice);
        ~CIrrOdeManager();

        void setGravity(core::vector3df &cGravity);  /**< Modify the gravity of the world */

        void worldStep(f32 fTimeDiff);

        void updateBeforePhysics();  /**< Update all objects before physics */
        void updateAfterPhysics();   /**< Update all objects after physics */

        int getObjectCount() { return m_iObjCnt; } /**< Retrieve the number of objects managed */
        IIrrOdeEntity *getObject(int iPos) { return m_cObjects[iPos]; } /**< Get the object at position <iPos> */

        static void nearCollisionCallback(void* data, dGeomID o1, dGeomID o2);

        static dWorldID getWorld() { return m_iWorld; } /**< Retrieve the World ID */
        static dSpaceID getSpace() { return m_iSpace; } /**< Retrieve the Space ID */

        static dJointGroupID getJointGroup() { return m_iJointGroup; } /**< Retrieve the Joint Group ID */

        void addEntity(IIrrOdeEntity *pEntity);  /**< Add an entity to the manager */
    };
  }
}

#endif
