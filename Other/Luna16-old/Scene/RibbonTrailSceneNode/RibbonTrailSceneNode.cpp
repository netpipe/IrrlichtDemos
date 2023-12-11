#include "irrlicht.h"
using namespace irr;
#include "RibbonTrailSceneNode.h"

// max quad count a section can have ( its useful for mantain a constant indices array )
const int MAXSECTIONSIZE = 50;

// find the proper parent for node
scene::ISceneNode* RibbonTrailSceneNode::getParentNode( scene::ISceneNode* AParent, IrrlichtDevice* ADevice )
{
	if ( !AParent )
		return ADevice->getSceneManager()->getRootSceneNode();
	else
		return AParent;
}

RibbonTrailSceneNode::RibbonTrailSceneNode( IrrlichtDevice* device, ISceneNode* ParentNode, s32 id,
								 core::vector3df position, core::vector3df rotation, core::vector3df scale
								 ) : scene::ISceneNode( getParentNode( ParentNode, device ), device->getSceneManager(), id, position, rotation, scale )
{
	Device = device;
	lastnode = 0;
	mindistance = 5;
	video = Device->getVideoDriver();
	material.MaterialType = video::EMT_TRANSPARENT_VERTEX_ALPHA;
	material.BackfaceCulling = false;
	setMaterialFlag( video::EMF_LIGHTING, false );
	lasttc = 0;
	maxquads = 100;
//	AutomaticCullingEnabled = false;
	point1.set( -5, 0, 0 );
	point2.set( 5, 0, 0 );
	startingalpha = 255;
	setEnabled( false );
	distanciapercorrida = 0.0f;
	Indices = new u16[ ( MAXSECTIONSIZE - 1 ) * 6 ];
	createIndices( MAXSECTIONSIZE - 1 );
}

// perform clearing on deletion
RibbonTrailSceneNode::~RibbonTrailSceneNode()
{
	delete[] Indices;
	while ( sections.size() > 0 )
	{
		CSection* sec = sections[0];
		while ( sec->items.size() > 0 )
		{
			vitems* it = sec->items[0];
			delete it;
			sec->items.erase( 0, 1 );
		}
		sec->Vertices.clear();
		delete sec;
		sections.erase( 0, 1 );
	}
}

void RibbonTrailSceneNode::setPoint1( core::vector3df newpoint )
{
	point1 = newpoint;
}

void RibbonTrailSceneNode::setPoint2( core::vector3df newpoint )
{
	point2 = newpoint;
}

void RibbonTrailSceneNode::setStartingAlpha( u16 value )
{
	startingalpha = value;
}

void RibbonTrailSceneNode::setMaxQuads( u16 value )
{
	maxquads = value;
}

const core::aabbox3d<f32>& RibbonTrailSceneNode::getBoundingBox() const
{
	return bbox;
}

video::SMaterial& RibbonTrailSceneNode::getMaterial(s32 i)
{
	return material;
}

s32 RibbonTrailSceneNode::getMaterialCount()
{
	return 1;
}

void RibbonTrailSceneNode::OnPreRender()
{
	Device->getSceneManager()->registerNodeForRendering( this, scene::ESNRP_TRANSPARENT );
}

// Transforms a POSITION vector relative to a given node to global coords
core::vector3df RibbonTrailSceneNode::TransformedPosition( core::vector3df APosition )
{
	core::matrix4 m;
	m.setRotationDegrees( getRotation() );
	m.transformVect( APosition );
	APosition *= getScale();
	return APosition + getAbsolutePosition();
};

// [ THIS IS NOT BEING USED ON CODE DUE TO SOME BUG ]
//! Function to know if a given point at Position, turned Rotation degrees, can see point with a given FOV
//! param Position: the position looker is
//! param Rotation: the rotation looker is
//! param point: the point looker is looking at
//! param FOV: Field of View
bool RibbonTrailSceneNode::isPointInFieldOfView( core::vector3df Position, core::vector3df Rotation, core::vector3df point, float FOV )
{
	core::matrix4 mat;
	mat.setTranslation( Position );
	mat.setRotationDegrees( Rotation );
//	core::vector3df look = core::vector3df(mat.M[0,0], mat.M[0,1], mat.M[0,2]);
	core::vector3df ab = point - Position;
//	f32 dot = look.dotProduct( ab );
	float cf = cos( FOV );
	float fv = cf*cf;
//	return (dot > 0) && (dot*dot >= look.getLengthSQ()*ab.getLengthSQ()*cf);
}

void RibbonTrailSceneNode::setMaxDistance( float value )
{
	mindistance = value;
}

// when set to false, close a section and stops rendering
// when set to true, start a new section
void RibbonTrailSceneNode::setEnabled( bool value )
{
	if ( value == Enabled ) return;
	Enabled = value;
	if ( value )
	{
		CSection* sec = new CSection;
		sections.push_back( sec );
		firstsection = sections[0];
		lastsection = sections.getLast();
		lastnode = 0;
		// start adding 2 nodes that correspond to one quad
		addQuad();
		addQuad();
		Update();
	} else
	{
		firstsection = 0;
		lastsection = 0;
	}
}

// Add a new node to section - each 2 nodes ( 4 vertex - 6 indices ) correspond to 1 quad
void RibbonTrailSceneNode::addQuad()
{
	core::vector3df leftp = TransformedPosition( point1 );
	core::vector3df rightp = TransformedPosition( point2 );
	f64 width = leftp.getDistanceFrom( rightp );

	vitems* it = new vitems;
	it->position = getAbsolutePosition();

	// acumula a distancia percorrida pelo node para calcular os texcoords
	if ( lastsection->items.size() > 1 )
		distanciapercorrida += (float)lastsection->items[ 1 ]->position.getDistanceFrom( getAbsolutePosition() );

	it->lpos = leftp;
	it->rpos = rightp;

	// expand the section boundingbox - its used for frustrum culling on render
	if ( lastsection->items.size() == 0 )
	{
		lastsection->BoundingBox.MinEdge = leftp;
		lastsection->BoundingBox.MaxEdge = leftp;
	}
	else
		lastsection->BoundingBox.addInternalPoint( leftp );
	lastsection->BoundingBox.addInternalPoint( rightp );

	lastsection->items.push_front( it );

	lastnode = it;
	f32 fact = (f32)(distanciapercorrida / width);
	video::S3DVertex vt;
	vt.Normal = core::vector3df( 0, 1, 0 );
	vt.Pos = leftp;
	vt.TCoords.set( 0, fact );
	vt.Color = video::SColor( (s32)startingalpha, (s32)startingalpha, (s32)startingalpha, (s32)startingalpha );
	lastsection->Vertices.push_front( vt );

	vt.Normal = core::vector3df( 0, 1, 0 );
	vt.Pos = rightp;
	vt.TCoords.set( 1, fact );
	vt.Color = video::SColor( (s32)startingalpha, (s32)startingalpha, (s32)startingalpha, (s32)startingalpha );
	lastsection->Vertices.push_front( vt );

	if ( lastsection->items.size() >= MAXSECTIONSIZE )
	{
		Enabled = false;
		setEnabled( true );
	}
}

// add a quad of indices for each quad in section
void RibbonTrailSceneNode::createIndices( int indcount )
{
	for ( int ni = 0; ni < indcount; ni++ )
	{
		int ni2 = ni * 2;
		int ni6 = ni * 6;

		int id0 = ni2 + 0;
		int id1 = ni2 + 1;
		int id2 = ni2 + 2;
		int id3 = ni2 + 3;
		Indices[ ni6 + 0 ] = id1;
		Indices[ ni6 + 1 ] = id0;
		Indices[ ni6 + 2 ] = id2;
		Indices[ ni6 + 3 ] = id2;
		Indices[ ni6 + 4 ] = id3;
		Indices[ ni6 + 5 ] = id1;
	}
}

// updating each frame
void RibbonTrailSceneNode::Update()
{
	// calculate position of left and right point, in absolute coordenates, relative to node position and rotation
	core::vector3df leftp = TransformedPosition( point1 );
	core::vector3df rightp = TransformedPosition( point2 );
	// calculate distance betw then
	float width = (float)leftp.getDistanceFrom( rightp );

	vitems* last2 = lastsection->items[ 1 ];

	float dst = (float)getAbsolutePosition().getDistanceFrom( last2->position );

	if ( dst > mindistance )
		addQuad();

	// move 2 endpoints of last created quad to position of SceneNode
	last2 = lastsection->items[ 1 ];
	lastnode->position = getAbsolutePosition();
	dst = (float)getAbsolutePosition().getDistanceFrom( last2->position );
	float fact = ( dst + distanciapercorrida ) / width;
	lastsection->Vertices[ 1 ].Pos = leftp;
	lastsection->Vertices[ 1 ].TCoords.set( 0, fact );
	lastsection->Vertices[ 0 ].Pos = rightp;
	lastsection->Vertices[ 0 ].TCoords.set( 1, fact );
	/// end move last item

	//performed when total quads in scenenode is over predefined maxquads variable
	//totalquads is accumulated each render process
	if ( totalquads > maxquads )
	{
		//fade out last 10 quads
		int fadded = 0;
		int faddedcount = 10;
		CSection* section;
		for ( u32 i = 0; i < sections.size() && fadded < faddedcount; i++ )
		{
			section = sections[ i ];
			for ( int v = section->Vertices.size()-1; v >= 0 && fadded < faddedcount; v-- )
			{
				int al = (int)( fadded * ( startingalpha / faddedcount ) );
				section->Vertices[v].Color.set( al, al, al, al );
				fadded ++;
			}
		}

		// as totalquads > maxquads delete last quad
		delete[] firstsection->items.getLast();
		firstsection->items.erase( firstsection->items.size() - 1 );
		firstsection->Vertices.erase( firstsection->Vertices.size()-2, 2 );
		//if section get empty, erase it
		if ( firstsection->items.size() == 0 )
		{
			delete firstsection;
			firstsection = 0;
			sections.erase( 0 );
			if ( sections.size() > 0 )
				firstsection = sections[0];
		}
	}
}

void RibbonTrailSceneNode::setShowDebug( bool visible )
{
    DebugDataVisible = visible;
}

void RibbonTrailSceneNode::render()
{
	if ( Enabled )
		Update();

	core::matrix4 mat;
	mat.makeIdentity();
	video->setTransform( video::ETS_WORLD, mat );
	video->setMaterial( material );

	// store frustrum bounding box
	//core::aabbox3df bb = SceneManager->getActiveCamera()->getViewFrustum()->getBoundingBox();
	CSection* section;

	// accumulation of total quads
	totalquads = 0;
	for ( u32 irend = 0; irend < sections.size(); irend++ )
	{
		section = sections[ irend ];

		totalquads += section->items.size();

		// if this section->boundingbox is outside view frustrum or not have a complete triangle to draw, continue next
//		if ( !bb.intersectsWithBox( section->BoundingBox ) || section->Vertices.size() < 3 )
	//		continue;

		int tricount = ( section->Vertices.size() - 2 );

		video->drawIndexedTriangleList(
			section->Vertices.pointer(),
			section->Vertices.size(),
			Indices,
			tricount ); // triangles, not indices

		if ( DebugDataVisible )
			video->draw3DBox( section->BoundingBox, video::SColor( 255, 255, 255, 255 ) );
	}

}

