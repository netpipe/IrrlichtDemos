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

#ifndef _LEVEL_H_
#define _LEVEL_H_

#include <string>
#include <list>
#include <vector>

#include "core/Vector3D.h"

//tolua_begin
namespace peak
{
	enum LevelObjectType
	{
		ELOT_Terrain,
		ELOT_SceneNode,
		ELOT_PointGroup
	};
	
	class LevelObject
	{
		public:
			virtual ~LevelObject() {};
			
			virtual void setName(std::string name);
			virtual std::string getName(void);
			
			virtual LevelObjectType getType(void) = 0;
			//tolua_end
		private:
			std::string name;
			//tolua_begin
	};
	
	class SceneNode;
	class Shape;
	class Body;
	
	/*class TerrainObject : public LevelObject
	{
		public:
			TerrainObject(std::string heightmap, std::string material, Vector3D position, Vector3D scale, bool server, bool client);
			virtual ~TerrainObject();
			
			virtual LevelObjectType getType(void)
			{
				return ELOT_Terrain;
			}
			//tolua_end
		private:
			Terrain *terrain;
			Heightfield *heightfield;
			//tolua_begin
	};*/
	
	class SceneNodeObject : public LevelObject
	{
		public:
			SceneNodeObject(SceneNode *node, Shape *geom = 0, Body *body = 0);
			virtual ~SceneNodeObject();
			
			virtual LevelObjectType getType(void)
			{
				return ELOT_SceneNode;
			}
			//tolua_end
		private:
			SceneNode *node;
			Shape *geom;
			Body *body;
			//tolua_begin
	};
	
	class PointGroupObject : public LevelObject
	{
		public:
			PointGroupObject();
			~PointGroupObject();
			
			void addPoint(Vector3D point);
			int getPointCount();
			Vector3D getPoint(int index);
			void deletePoint(int index);

			void addRotation(Vector3D rotation);
			int getRotationCount();
			Vector3D getRotation(int index);
			void deleteRotation(int index);
			
			virtual LevelObjectType getType(void)
			{
				return ELOT_PointGroup;
			}
			//tolua_end
		private:
			std::vector<Vector3D> points;
			std::vector<Vector3D> rotations;
			//tolua_begin
	};
	
	/**
	 * \brief Class which allows loading a number of static models, entities and
	 * other level information.
	 */
	class Level
	{
		public:
			static Level *get(void);
			~Level();
			
			/**
			 * \brief Loads the level with the given name.
			 *
			 * The map is loaded from the file "levels/<name>.level".
			 */
			bool init(std::string name, bool server, bool client);
			/**
			 * \brief Destroys everything which was loaded with the level.
			 */
			bool destroy(void);
			
			/**
			 * \brief Returns the LevelObject with the given name.
			 */
			LevelObject *getObject(std::string name);
			/**
			 * \brief Returns the PointGroupObject with the given name.
			 */
			PointGroupObject *getPointGroup(std::string name);
			
			/**
			 * \brief Returns the number of installed levels.
			 */
			int getLevelCount(void);
			/**
			 * \brief Returns the name of a level.
			 * \param index Index of the level. Must be in the range
			 * 0..getLevelCount() - 1.
			 */
			std::string getLevelName(int index);
			
			//tolua_end
		private:
			Level();
			
			std::list<LevelObject*> objects;
			//tolua_begin
	};
}
//tolua_end

#endif

