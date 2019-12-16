// *************************************************************************************************
//
// Horde3D
//   Next-Generation Graphics Engine
// --------------------------------------
// Copyright (C) 2006-2008 Nicolas Schulz
//
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// *************************************************************************************************

#ifndef _egSceneGraphRes_H_
#define _egSceneGraphRes_H_

#include "egPrerequisites.h"
#include "egScene.h"
#include "utMath.h"
#include "egParticle.h"

struct XMLNode;


class SceneGraphResource : public Resource
{
private:

	SceneNodeTpl	*_rootNode;

	void parseBaseAttributes( XMLNode &xmlNode, SceneNodeTpl &nodeTpl );
	void parseNode( XMLNode &xmlNode, SceneNodeTpl *parentTpl );

public:

	static Resource *factoryFunc( const string &name, int flags )
		{ return new SceneGraphResource( name, flags ); }
	
	SceneGraphResource( const string &name, int flags );
	~SceneGraphResource();
	
	void initDefault();
	void release();
	bool load( const char *data, int size );

	SceneNodeTpl *getRootNode() { return _rootNode; }

	friend class SceneManager;
};

typedef SmartResPtr< SceneGraphResource > PSceneGraphResource;


struct ReferenceNodeTpl : public SceneNodeTpl
{
	PSceneGraphResource		sgRes;

	ReferenceNodeTpl( const string &name, SceneGraphResource *sgRes ) :
		SceneNodeTpl( 0, name ), sgRes( sgRes )
	{
	}
};

#endif // _egSceneGraphRes_H_
