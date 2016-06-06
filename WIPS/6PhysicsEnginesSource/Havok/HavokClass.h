/*
Copyright (c) 2008 Nils Daumann

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef _HAVOKCLASS_H_
#define _HAVOKCLASS_H_

#include <irrlicht.h>
#include <Common/hkAnimationPhysicsPublicInclude.h>
#include <hr_time.h>
#include <iostream>


using namespace irr;

class HavokRigidBody : public hkpRigidBody
{
	public:
		HavokRigidBody::HavokRigidBody(const hkpRigidBodyCinfo &info)
											: hkpRigidBody(info), userdata(0){}

		scene::ISceneNode *userdata;
};

/**
 * \brief Class which manages havok physics
 */
class HavokClass
{
	public:

		/**
		* \brief Constructor
		*/
		HavokClass(IrrlichtDevice *device_);

		/**
		* \brief Destructor
		*/
		~HavokClass();

		/**
		* \brief Updates the physics
		*/
		void update();

		/**
		* \brief Creates static geometry
		*/
		void createStaticGeometry(const c8 *filename);

		/**
		* \brief Assigns physics to the scene node
		*/
		HavokRigidBody *setPhysics(scene::ISceneNode *node, f32 mass);

	private:
		void updateEntitiesAfterPhysics();

		IrrlichtDevice *device;

		hkThreadMemory *threadmemory;
		hkpWorld *havokworld;

		CStopWatch timer;
		f64 timestep;
};

#endif
