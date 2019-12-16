/*
Copyright (C) 2008   Christian Reiser

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in the
Software without restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "physics/Sphere.h"

#include <btBulletDynamicsCommon.h>


namespace peak
{

        Sphere::Sphere()
        {
        }
        Sphere::~Sphere()
        {
        }

        void Sphere::create(float radius, float mass)
        {
            transform = new btTransform;
            transform->setIdentity();
            transform->setOrigin(btVector3(0, 0, 0));

            btVector3 position(0, 0, 0);
//            shape = new btMultiSphereShape(btVector3(1, 1, 1), &position, &radius, 1);

    //        inertia = Vector3D(0, 0, 0);
   //         if (mass != 0.0f)
   //         {
   //             btVector3 in;
   //             shape->calculateLocalInertia(mass, in);
   //             inertia = Vector3D(in.x(), in.y(), in.z());
   //         }
  //          this->mass = mass;
        }
        bool Sphere::destroy()
        {
            return true;
        }

}
