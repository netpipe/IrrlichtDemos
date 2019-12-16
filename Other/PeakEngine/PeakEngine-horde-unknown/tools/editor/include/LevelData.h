
/*
Copyright (C) 2008   Mathias Gottschlag

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

#ifndef _LEVELDATA_H_
#define _LEVELDATA_H_

#include <string>
#include <vector>

#include "support/tinyxml.h"

class LevelNode
{
	public:
		LevelNode();
		virtual ~LevelNode();
		
		bool load(LevelNode *parent);
		
		void addChild(LevelNode *node);
		
		int getSceneNode(void) { return scenenode; };
		
	protected:
		std::vector<LevelNode*> children;
		int scenenode;
};

class MeshLevelNode : public LevelNode
{
	public:
		MeshLevelNode();
		~MeshLevelNode();
		
		bool load(TiXmlElement *element, LevelNode *parent);
	private:
		int model;
};

class TerrainLevelNode : public LevelNode
{
	public:
		TerrainLevelNode();
		~TerrainLevelNode();
		
		bool load(TiXmlElement *element, LevelNode *parent);
};

class LevelData
{
	public:
		LevelData();
		~LevelData();
		
		bool load(std::string file);
		bool save(std::string file);
		
		void show(void);
		void hide(void);
	private:
		LevelNode *rootnode;
};

#endif

