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

#ifndef _GRAPHICSCOMPONENT_H_
#define _GRAPHICSCOMPONENT_H_

#include "entity/Component.h"

#include <map>

//tolua_begin
namespace peak
{
	class SceneNode;
	class Body;
	class PhysicsComponent;
	
	struct SceneNodeData
	{
		SceneNode *scenenode;
		int parentid;
		Body *body;
		
		Vector3D speed;
		Vector3D oldspeed;
		Vector3D position;
		Quaternion rotation;
		unsigned char update;
	};
	
	/**
	 * \brief Class holding the graphical properties of an entity
	 *
	 * This component exists on both the server and the client and is automatically
	 * synchronized.
	 */
	class GraphicsComponent : public Component
	{
		public:
			GraphicsComponent();
			virtual ~GraphicsComponent();
			
			virtual bool init(Entity *entity);
			virtual bool destroy(void);
			
			virtual bool needsUpdate(void);
			virtual int getUpdateSize(void);
			virtual int writeUpdateData(Buffer *buffer);
			
			virtual bool updateFromData(Buffer *buffer);
			
			virtual bool writeCompleteData(Buffer *buffer);
			virtual bool readCompleteData(Buffer *buffer);
			
			/**
			 * \brief Adds a scene node to the component.
			 * \param node SceneNode to add
			 * \param id ID of the scene node
			 * \param parentid ID of the parent or 0
			 */
			void addSceneNode(SceneNode *node, int id, int parentid);
			/**
			 * \brief Returns the scene node with the given id.
			 */
			SceneNode *getSceneNode(int id);
			
			virtual ComponentType getType(void);
			
			virtual bool doWork(float msecs);
		private:
			//tolua_end
			PhysicsComponent *physics;
			
			std::map<int, SceneNodeData> scenenodes;
			//tolua_begin
	};
}
//tolua_end

#endif

