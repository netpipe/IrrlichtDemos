#include "OpenSteer/SimpleVehicle.h"
#include "OpenSteer/OpenSteerDemo.h"
#include <OpenSteer/Clock.h>
#include <irrlicht.h>

using namespace OpenSteer;
using namespace irr;

    class MyObject : public SimpleVehicle
    {
    public:
        scene::ISceneNode * MeshNode;

        // constructor
        MyObject(scene::ISceneNode * inMeshNode ) {
            MeshNode = inMeshNode;
            reset ();
            MeshNode->setPosition(core::vector3df( position().x, position().y,  position().z ) );

        }

         double distance( core::vector3df b )
         {

            double v1 = ( b.X - MeshNode->getPosition().X );
            double v2 = ( b.Y - MeshNode->getPosition().Y );
            double total = 0;

            v1 = v1 * v1;
            v2 = v2 * v2;

            total = sqrt( v1 + v2 );
            return total;
        }

        // reset state
        void reset (void)
        {
            SimpleVehicle::reset (); // reset the vehicle
            setSpeed (0.1f);         // speed along Forward direction.
            setMaxForce (0.3f);      // steering force is clipped to this magnitude
            setMaxSpeed (1.0f);         // velocity is clipped to this magnitude
            /// ***************** Atention HERE
            randomizeHeadingOnXZPlane();
        }

        // per frame simulation update
        void update (const float currentTime, const float elapsedTime)
        {
            applySteeringForce (Vec3 (-2, 0, -3), elapsedTime);
            MeshNode->setPosition(core::vector3df (position().x, position().y, position().z) );
/// extrafix
  MeshNode->setRotation(curva);
  curva = MeshNode->getRotation();
  curva.X = -90;
  MeshNode->setRotation(curva);

   // Forward
   core::matrix4 m;
   core::vector3df vel = core::vector3df( velocidade,0,0);
   m.setRotationDegrees(MeshNode->getRotation());
   m.transformVect(vel);
   MeshNode->setPosition(MeshNode->getPosition() + vel);
   MeshNode->updateAbsolutePosition();

//   nextstep : (calculate all forces)
//   getPosition: return next position
//   getRotate : return X Y Z rotation

///
        }

    };
