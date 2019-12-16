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

#ifndef _egScene_H_
#define _egScene_H_

#include "egPrerequisites.h"
#include "utMath.h"
#include "egPrimitives.h"
#include "egResource.h"
#include "egPipeline.h"
#include <map>
using namespace std;

struct SceneNodeTpl;
class SceneGraphResource;


const int RootNode = 1;


struct SceneNodeTypes
{
	enum List
	{
		Undefined = 0,
		Group,
		Model,
		Mesh,
		Joint,
		Light,
		Camera,
		Emitter
	};
};

struct SceneNodeParams
{
	enum List
	{
		Name = 1,
		AttachmentString
	};
};

struct SceneNodeTpl
{
	int							type;
	string						name;
	Vec3f						trans, rot, scale;
	string						attachmentString;
	vector< SceneNodeTpl * >	children;

	SceneNodeTpl( int type, const string &name ) :
		type( type ), name( name ), scale( Vec3f ( 1, 1, 1 ) )
	{
	}
	
	virtual ~SceneNodeTpl()
	{
		for( uint32 i = 0; i < children.size(); ++i ) delete children[i];
	}
};

class SceneNode
{
protected:
	
	int						_type;
	string					_name;
	bool					_renderable;
	bool					_active;
	NodeHandle				_handle;
	bool					_dirty;
	bool					_transformed;
	Matrix4f				_relTrans, _absTrans;			// Transformation matrices
	BoundingBox				_bBox;							// AABB in world space

	SceneNode				*_parent;		// Parent node
	vector< SceneNode * >	_children;		// Child nodes
	string					_attachment;	// User defined data
	
	void markChildrenDirty();

public:

	float					tmpSortValue;
	
	static bool frontToBackOrder( SceneNode *n1, SceneNode *n2 )
		{ return n1->tmpSortValue < n2->tmpSortValue; }
	static bool backToFrontOrder( SceneNode *n1, SceneNode *n2 )
		{ return n1->tmpSortValue > n2->tmpSortValue; }
	
	SceneNode( const SceneNodeTpl &tpl );
	virtual ~SceneNode();

	void setActivation( bool active ) { _active = active; }
	void getTransform( Vec3f &trans, Vec3f &rot, Vec3f &scale );	// Not virtual for performance
	void setTransform( Vec3f trans, Vec3f rot, Vec3f scale );	// Not virtual for performance
	void setTransform( const Matrix4f &mat );
	const void getTransMatrices( const float **relMat, const float **absMat );

	virtual float getParamf( int param );
	virtual bool setParamf( int param, float value );
	virtual int getParami( int param );
	virtual bool setParami( int param, int value );
	virtual const char *getParamstr( int param );
	virtual bool setParamstr( int param, const char* value );

	virtual BoundingBox *getLocalBBox() { return 0x0; }
	virtual bool canAttach( SceneNode &parent );
	void markDirty();
	bool update();
	virtual bool checkIntersection( const Vec3f &rayOrig, const Vec3f &rayDir, Vec3f &intsPos ) const;

	virtual void onPreUpdate();	// Called before absolute transformation is updated
	virtual void onPostUpdate();	// Called after absolute transformation has been updated
	virtual void onAttach( SceneNode &parentNode );	// Called when node is attached to parent
	virtual void onDetach( SceneNode &parentNode );	// Called when node is detached from parent

	int getType() { return _type; };
	NodeHandle getHandle() { return _handle; }
	SceneNode *getParent() { return _parent; }
	const string &getName() { return _name; }
	vector< SceneNode * > &getChildren() { return _children; }
	Matrix4f &getRelTrans() { return _relTrans; }
	Matrix4f &getAbsTrans() { return _absTrans; }
	BoundingBox &getBBox() { return _bBox; }
	const string &getAttachmentString() { return _attachment; }
	void setAttachmentString( const char* attachmentData ) { _attachment = attachmentData; }
	bool checkTransformFlag( bool reset )
		{ bool b = _transformed; if( reset ) _transformed = false; return b; }

	friend class SceneManager;
	friend class Renderer;
};


struct GroupNodeParams
{
	enum List
	{
		MinDist = 100,
		MaxDist
	};
};

struct GroupNodeTpl : public SceneNodeTpl
{
	float	minDist, maxDist;

	GroupNodeTpl( const string &name ) :
		SceneNodeTpl( SceneNodeTypes::Group, name )
	{
	}
};

class GroupNode : public SceneNode
{
protected:

	float	_minDist, _maxDist;

	GroupNode( const GroupNodeTpl &groupTpl );

public:

	static SceneNodeTpl *parsingFunc( map< string, string > &attribs );
	static SceneNode *factoryFunc( const SceneNodeTpl &nodeTpl );

	float getParamf( int param );
	bool setParamf( int param, float value );

	friend class Renderer;
	friend class SceneManager;
};


typedef SceneNodeTpl *(*NodeTypeParsingFunc)( map< string, string > &attribs );
typedef SceneNode *(*NodeTypeFactoryFunc)( const SceneNodeTpl &tpl );
typedef void (*NodeTypeRenderFunc)( const string &shaderContext, const string &theClass, bool debugView,
								    const Frustum *frust1, const Frustum *frust2, RenderingOrder::List order,
								    int occSet );

struct NodeRegEntry
{
	string					typeString;
	NodeTypeParsingFunc		parsingFunc;
	NodeTypeFactoryFunc		factoryFunc;
	NodeTypeRenderFunc		renderFunc;
};

struct CastRayResult
{
	SceneNode *node;
	float distance;
	Vec3f intersection;
};

class SceneManager
{
protected:

	map< int, NodeRegEntry >	_registry;		// Registry of node types
	vector< SceneNode *>		_nodes;			// _nodes[0] is root node
	vector< SceneNode *>		_lightQueue;
	vector< SceneNode *>		_renderableQueue;
	vector< SceneNode * >		_findResults;
	vector< CastRayResult >     _castRayResults;

	Vec3f                       _rayOrigin;// don't put these values during recursive search on the stack
	Vec3f                       _rayDirection;// dito
	int                         _rayNum;// dito

	void updateQueuesRec( const Frustum &frustum1, const Frustum *frustum2, bool sorted, 
						  SceneNode &node, bool lightQueue, bool renderableQueue );
	NodeHandle parseNode( SceneNodeTpl &tpl, SceneNode *parent );
	void removeNodeRec( SceneNode *node );

	void castRayInternal( SceneNode *node );
public:

	SceneManager();
	~SceneManager();

	void registerType( int type, const string &typeString, NodeTypeParsingFunc pf,
					   NodeTypeFactoryFunc ff, NodeTypeRenderFunc rf );
	NodeRegEntry *findType( int type );
	NodeRegEntry *findType( const string &typeString );
	
	void updateNodes();
	void updateQueues( const Frustum &frustum1, const Frustum *frustum2,
					   RenderingOrder::List order, bool lightQueue, bool renderableQueue );
	
	NodeHandle addNode( SceneNode *node, SceneNode &parent );
	NodeHandle addNodes( SceneNode &parent, SceneGraphResource &sgRes );
	bool removeNode( NodeHandle handle );
	bool relocateNode( NodeHandle node, NodeHandle parent );
	
	int findNodes( SceneNode *startNode, const string &name, int type );
	void clearFindResults() { _findResults.resize( 0 ); }
	SceneNode *getFindResult( int index ) { return (unsigned)index < _findResults.size() ? _findResults[index] : 0x0; }
	
	int castRay( SceneNode *node, const Vec3f &rayOrig, const Vec3f &rayDir, int numNearest );
	bool getCastRayResult( int index, CastRayResult &crr );

	SceneNode &getRootNode() { return *_nodes[0]; }
	SceneNode &getDefCamNode() { return *_nodes[1]; }
	vector< SceneNode * > &getLightQueue() { return _lightQueue; }
	vector< SceneNode * > &getRenderableQueue() { return _renderableQueue; }
	
	SceneNode *resolveNodeHandle( NodeHandle handle )
		{ return (handle != 0 && (unsigned)(handle - 1) < _nodes.size()) ? _nodes[handle - 1] : 0x0; }

	friend class Renderer;
};

#endif // _egScene_H_
