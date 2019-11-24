/*
Copyright (C) 2008  Mathias Gottschlag

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

#include "core/Vector3D.h"
#include "core/Quaternion.h"

//tolua_begin
namespace peak
{
	class SceneNode;
	
	/**
	 * \brief Class which represents the virtual ear of the player
	 */
	class Listener
	{
		public:
			Listener();
			~Listener();
			
			/**
			 * \brief Sets the position of the listener
			 */
			void setPosition(Vector3D pos);
			/**
			 * \brief Returns the position of the listener
			 */
			Vector3D getPosition(void);
			/**
			 * \brief Sets the velocity of the listener
			 */
			void setVelocity(Vector3D vel);
			/**
			 * \brief Returns the velocity of the listener
			 */
			Vector3D getVelocity(void);
			/**
			 * \brief Sets the rotation of the listener
			 */
			void setRotation(Quaternion rot);
			/**
			 * \brief Returns the rotation of the listener
			 */
			Quaternion getRotation(void);
			
			/**
			 * \brief Attaches the listener to a scene node
			 * \param node SceneNode to attach listener to
			 * \param rotation If set to true, the listener is rotated accordingly
			 * \param adjustspeed If set to true, the velocity of the listener is
			 * updated based on the movement of the scene node
			 */
			void attachToSceneNode(SceneNode *node, bool rotation = false, bool adjustspeed = false);
			
			void doWork(float msecs);
		private:
			//tolua_end
			SceneNode *node;
			bool adjustrotation;
			bool adjustspeed;
			Quaternion rotation;
			//tolua_begin
	};
}
//tolua_end

