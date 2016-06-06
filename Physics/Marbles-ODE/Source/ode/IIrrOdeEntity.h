#ifndef _IIRRODEENTITY
  #define _IIRRODEENTITY

  #include "IIrrOdeBase.h"

namespace irr
{
  namespace ode
  {
    class CObjData
    {
      public:
        CObjData()
        {
          m_fSlip1=0.0f; m_fSlip2=0.0f;
          m_ax=0.0f; m_ax=0.0f; m_az=0.0f;
          m_iDelay=0;

          m_sSound[0]='\0';
        }

        char m_sName[50];

        irr::c8 m_sSound[50];

        float m_fSlip1,m_fSlip2;
        float m_ax,m_ay,m_az;

        int m_iDelay;
    };

    /**
     * This is the base class for all entities managed by irr::ode
     */
    class IIrrOdeEntity : public IIrrOdeBase
    {
      private:
        CObjData *m_pData;

      protected:
        scene::IMesh      *m_pMesh;  /**< The object's mesh            */
        scene::ISceneNode *m_pNode;  /**< The object's mesh scene node */

        dWorldID m_iWorld; /**< The world to which the object will belong to */
        dSpaceID m_iSpace; /**< The objects space                            */
        dBodyID  m_iBody;  /**< Body data of the object                      */
        dGeomID  m_iGeom;  /**< Geometry data of the object                  */
        dMass    m_dMass;  /**< Mass of the object                           */
        irr::f32 m_fMass;  /**< Value of the Mass                            */

        dVector3 *m_pVertices; /**< vertex array for trimesh geom           */

        int *m_pIndices;       /**< index array for trimesh geom            */
        int  m_iVertexCount;   /**< number of vertices in the vertex array  */
        int  m_iIndexCount;    /**< number of indices in the index array    */

      public:
        IIrrOdeEntity(dWorldID iWorld, dSpaceID iSpace, scene::IMesh *pMesh, scene::IMeshSceneNode *pNode);
        ~IIrrOdeEntity();

        virtual void collisionCallback(IIrrOdeEntity *pOpponent, int iContactCnt, dJointFeedback *pFeedback)=0;  /**< This method must be implemented. It will be called on every collision */

        scene::IMesh      *getMesh     () { return m_pMesh; } /**< Get the animated mesh of the entity           */
        scene::ISceneNode *getSceneNode() { return m_pNode; } /**< Get the animated mesh scene node of the enity */

        dBodyID getBody() { return m_iBody; }
        dGeomID getGeom() { return m_iGeom; }

        void setMesh     (scene::IMesh      *pMesh) { m_pMesh=pMesh; }
        void setSceneNode(scene::ISceneNode *pNode) { m_pNode=pNode; }

        void setPosition(core::vector3df cNewPosition);        /**< Set the position of the entity                           */
        void setForce(core::vector3df cNewForce);              /**< Set the force currently applied to the entity            */
        void setTorque(core::vector3df cNewTorque);            /**< Set the torque currently applied to the entity           */
        void setVelocity(core::vector3df cNewVelocity);        /**< Set the velocity currently applied to the entity         */
        void setAngularVelocity(core::vector3df cNewVelocity); /**< Set the angular velocity currently applied to the entity */

        static void QuaternionToEuler(const dQuaternion quaternion,core::vector3df &euler);  /**< Convert Quaternion to Euler */
        static void EulerToQuaternion(const core::vector3df &euler, dQuaternion quaternion); /**< Convert Euler to Quaternion */

        void updateBeforePhysics();  /**< This must be called if the Entities are updated outside ODE's physics */
        void updateAfterPhysics();   /**< This must be called to update the Irrlicht scene after physics calculation */

        void setSphereGeomData();  /**< Make a sphere of the entity   */
    		void setGeomData();        /**< Set the geom data of the mesh */

    		void setMass(irr::f32 fMass); /**< Set the mass value of the object */
    		void updateMass();            /**< Update the ODE mass structure    */
    		irr::f32 getMass();           /**< Get the mass of the object       */

        void setObjectData(CObjData *pData) { m_pData=pData; }
        CObjData *getData(void) { return m_pData; }
    };
  }
}

#endif
