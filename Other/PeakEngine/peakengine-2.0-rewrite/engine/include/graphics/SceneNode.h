/*
Copyright (C) 2008   Lukas Kropatschek, Mathias Gottschlag

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

#ifndef _SCENENODE_H_
#define _SCENENODE_H_

#include "core/Vector3D.h"
#include "core/Quaternion.h"

#include <list>
#include <vector>

namespace peak
{
	/**
	 * \brief Base class for all 3D scene nodes
	 */
	class SceneNode
	{
		public:
			SceneNode();
			virtual ~SceneNode();
			
			void create(int node = 0, SceneNode *parent = 0);
			
			/**
			 * \brief Sets the position of the scene node.
			 */
			virtual void setPosition(float x, float y, float z);
			
			/**
			 * \brief Sets the rotation of the scene node.
			 */
			virtual void setRotation(float x, float y, float z);
			
			/**
			 * \brief Sets the rotation of the scene node.
			 */
			virtual void setRotation(float w, float x, float y, float z);
			/**
			 * \brief Sets the position of the scene node.
			 * \param newpos Vector3D containing the coordinates for the new position.
			 */
			virtual void setPosition(Vector3D newpos);
			/**
			 * \brief Sets the rotation of the scene node.
			 * \param newrot Vector3D containing the coordinates for the new rotation.
			 */
			virtual void setRotation(Vector3D newrot);
			
			/**
			 * \brief Sets the rotation of the scene node.
			 * \param newrot Quaternion containing the coordinates for the new rotation.
			 */
			virtual void setRotation(Quaternion newrot);
			/**
			 * \brief Returns the current position of the scene node.
			 * \return position as Vector3D.
			 */
			virtual Vector3D getPosition(void);
			/**
			 * \brief Returns the current rotation of the scene node.
			 * \return rotation as Vector3D.
			 */
			virtual Vector3D getRotationEuler(void);
			
			/**
			 * \brief Returns the current rotation of the scene node.
			 * \return rotation as Quaternion.
			 */
			virtual Quaternion getRotation(void);
			/**
			 * \brief Scales the scene node.
			 * \param newscale the new scale for the scene node.
			 */
			virtual void setScale(Vector3D newscale);
			/**
			 * \brief Sets the parent of the scene node.
			 * \param newparent the parent scene node.
			 */
			virtual void setParent(SceneNode *newparent);
			
			/**
			 * \brief Returns the parent of the scene node
			 */
			virtual SceneNode *getParent(void);
			
			/**
			 * \brief Sets if the scene node should be visible.
			 * \param flag true = visible
			 */
			virtual void setVisible(bool flag);
			/**
			 * \brief Returns if the scene node is visible or not.
			 * \return true if visible.
			 */
			virtual bool isVisible(void);
			
			/**
			 * \brief Returns the global bounding box of the scene node.
			 */
			virtual void getBoundingBox(Vector3D &min, Vector3D &max);
			
			/**
			 * \brief Adds a child to the scene node.
			 *
			 * For internal use only.
			 * \param child Scene node to add
			 */
			void addChild(SceneNode *child);
			/**
			 * \brief Removes a child from the scene node.
			 *
			 * For internal use only.
			 * \param child Scene node to remove
			 */
			void removeChild(SceneNode *child);
			
			/**
			 * \brief Returns the Horde3D node handle
			 */
			int getNode(void);
			
		protected:
			SceneNode *parent;
			
			std::list<SceneNode*> children;
			
			int node;
			bool visible;
	};
}

#endif

