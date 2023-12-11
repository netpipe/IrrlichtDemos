#include "irrlicht.h"

#pragma comment(lib, "D:\\irrlicht-1.4.2\\lib\\Win32-visualstudio\\Irrlicht.lib")

#include <math.h>
#include <sstream>
using namespace irr;
using namespace scene;
using namespace core;
#include "simplexnoise1234.h"

class CVertexQuad
{
private:
	int sl;
public:
	// sizelenght is the lenght of one side of the grid ( ie 16 for full lod, 2 for lesser lod )
	CVertexQuad( int sideLength )
	{
		sl = sideLength;
		Vertices = (video::S3DVertex2TCoords*)malloc( sizeof( video::S3DVertex2TCoords ) * ((sl+1)*(sl+1)) );
	}
	~CVertexQuad()
	{
		free( Vertices );
	}
	// get an index on matrix, based on x,y coordinates
	u16 getIndex( int x, int y )
	{
		return x+(y*(sl+1));
	}
	void set( int x, int y, video::S3DVertex2TCoords vertex )
	{
		Vertices[getIndex(x,y)] = vertex;
	}
	video::S3DVertex2TCoords get( int x, int y )
	{
		return Vertices[x+(y*(sl+1))];
	}
	video::S3DVertex2TCoords* Vertices;
};

// this class holds vertices and indices for each lod in each patch
//
class CLOD
{
private:
	CVertexQuad* Vertices;
	//video::S3DVertex2TCoords* Vertices;
	//CIndicesQuad* Indices;
	u16* Indices;
	int lods[4];

	float getHeight(float x, float z)
	{
		int octaves=5;
		float persistance =2;
		int scale=10;  // noise scaler, not the same as patch scaler
		float total = 0;

		//amplify?
		int ax=0;
		int az=0;

		for (int i=2; i < octaves; i++)
		{
			float frequency = pow(2.0f, i);
			float amplitude = pow(persistance, i);
			total += SimplexNoise1234::noise(((x+ax)/scale)/ frequency, ((z+az)/scale)/ frequency) * amplitude;
		}

		return (total);
	};

	// calculate indices for a given lod
	void calculateIndices()
	{
		Indices = (u16*)malloc( sizeof(u16) * ((nquads*nquads)*6) );
		int i = 0;
		for ( int y = 0; y < nquads; y++ )
			for ( int x = 0; x < nquads; x++ )
			{
				int j0 = Vertices->getIndex(   x,   y );
				int j1 = Vertices->getIndex( x+1,   y );
				int j2 = Vertices->getIndex(   x, y+1 );
				int j3 = Vertices->getIndex( x+1, y+1 );

				Indices[i  ] = j0;
				Indices[i+1] = j2;
				Indices[i+2] = j1;

				Indices[i+3] = j1;
				Indices[i+4] = j2;
				Indices[i+5] = j3;
				i+= 6;
			}
	}

	vector3df position;
	int nquads;
	int lod;
	float quadsize;
	int nPatches;

	// calculate a grid of vertices for a given lod
	void buildGrid()
	{
		Vertices = new CVertexQuad( nquads );
		//Vertices = (video::S3DVertex2TCoords*)malloc( sizeof( video::S3DVertex2TCoords ) * ((nquads+1)*(nquads+1)) );
		for ( int y = 0; y <= nquads; y++ )
			for ( int x = 0; x <=nquads; x++ )
			{
				float s = ( nquads * quadsize ) / 2.0f;
				vector3df loc;
				loc.X = position.X + ( x * quadsize );// - s;
				loc.Z = position.Z + ( y * quadsize );// - s;
				loc.Y = position.Y + getHeight( position.X + ( x * quadsize ), position.Z + ( y * quadsize ) );

				if ( x == 0 && y == 0 )
				{
					box.MinEdge = loc;
					box.MaxEdge = loc;
				}
				else
					box.addInternalPoint( loc );

				// total side size of terrain in units
				double ts = 80.0f * nPatches;
				double mts = ts / 2.0f;

				double u1 = ( (loc.X+mts) / ts );
				double v1 = ( (loc.Z+mts) / ts );
				// if number of patches is pair (%2==0), need to offset texture UV by 0.5f
				//if ( nPatches % 2 != 0 )
				//{
				//	u1 += 0.5f;
				//	v1 += 0.5f;
				//}

				Vertices->set( x, y, video::S3DVertex2TCoords(
					loc,
					vector3df( 0, 1, 0 ),
					video::SColor(255,255,255,255),
					vector2df( u1, v1 ),
					vector2df( (x*quadsize)/10, (y*quadsize)/10 )
					) );
			}

		calculateIndices();
	}

public:
	aabbox3df box;
	// aLod: 0 maximum detail - 3 minimum detail
	CLOD( int aLod, vector3df aPosition, int numPatches )
	{
		nPatches = numPatches;
		lods[0] = 1;
		lods[1] = 2;
		lods[2] = 4;
		lods[3] = 8;

		lod = aLod;
		position = aPosition;
		quadsize = 5.0f * lods[lod];
		nquads = 80 / quadsize;

		buildGrid();
	}
	~CLOD()
	{
		free( Vertices );
		free( Indices );
	}
	// Render this lod
	void Render( video::IVideoDriver* driver )
	{
		driver->drawIndexedTriangleList( &Vertices->Vertices[0], (nquads+1)*(nquads+1), &Indices[0], (nquads*nquads)*2 );
	}
};

// represents a terrain visible patch
class CPatch
{
private:
	core::aabbox3d<f32> box;
	video::SMaterial Material;
	bool showBoundingBox;

	CLOD* lod0;
	CLOD* lod1;
	CLOD* lod2;
	CLOD* lod3;

	vector3df position;

	float maxLodDistance;
public:

	// position is passed as patch index ( 0,0 - 0,1 - 1,1 - 3,5, etc )
	CPatch( vector3df aPosition, /*int px, int pz, */int nPatches )
	{
		maxLodDistance = 80;
		position = aPosition;// vector2df( px*80,pz*80 );

		lod0 = new CLOD( 0, position, nPatches );
		lod1 = new CLOD( 1, position, nPatches );
		lod2 = new CLOD( 2, position, nPatches );
		lod3 = new CLOD( 3, position, nPatches );
		box = lod0->box;
		box.addInternalBox( lod1->box );
		box.addInternalBox( lod2->box );
		box.addInternalBox( lod3->box );

		showBoundingBox = false;
	}

	~CPatch()
	{
		delete lod0;
		delete lod1;
		delete lod2;
		delete lod3;
	}

	void DrawLOD( irr::scene::ISceneManager* smgr, video::IVideoDriver* driver )
	{
		ICameraSceneNode* cam = smgr->getActiveCamera();
		cam->updateAbsolutePosition();
		cam->setFarValue(5000);
		vector3df cpos = cam->getAbsolutePosition();
		// calculate distance btw this patch and active camera
		float dst = cpos.getDistanceFrom( vector3df( position.X,position.Y,position.Z) );

		int ld = dst / maxLodDistance;
		if ( ld < 0 ) ld = 0;
		if ( ld > 4 ) ld = 4;

		if ( ld == 0 || ld == 1 ) lod0->Render( driver );
			else if ( ld == 2 ) lod1->Render( driver );
				else if ( ld == 3 ) lod2->Render( driver );
					else if ( ld == 4 ) lod3->Render( driver );

		// if required, render the patch boundingbox
		if ( showBoundingBox )
			driver->draw3DBox( box, video::SColor( 255, 255, 255, 255 ) );
	}

	virtual const core::aabbox3d<f32>& getBoundingBox() const
	{
		return box;
	}

	bool setShowBoundingBox( bool value )
	{
		showBoundingBox = value;
	}

	// all four lods are distributed btw camera view distance
	void setMaxLodDistance( float value )
	{
		maxLodDistance = value;
	}
};

// ctrunk is the quadtree thing. it starts with a large boundingbox and breaks it until its the size of a patch
class CTrunk
{
private:
	CTrunk* parent;
	int nPatches;
	// given a boundingbox, if its larger than patch size, break this in four new CTrunk,
	// otherwise, give it a patch for rendering
	// level indicates 0: uninitialized - 1: Trunk->subdivided - 2: leaf-patch
	void initBox( core::aabbox3df aBox )
	{
		level = 0;
		box = aBox;
		if ( box.getExtent().X > 80 )
		{
			level = 1;
			int mb = box.getExtent().X / 2;
			vector3df p = box.MinEdge;

			childs[0] = new CTrunk( aabbox3df( p.X, 0, p.Z, p.X+mb, 100, p.Z+mb ), this );
			childs[1] = new CTrunk( aabbox3df( p.X+mb, 0, p.Z, p.X+mb+mb, 100, p.Z+mb ), this );
			childs[2] = new CTrunk( aabbox3df( p.X, 0, p.Z+mb, p.X+mb, 100, p.Z+mb+mb ), this );
			childs[3] = new CTrunk( aabbox3df( p.X+mb, 0, p.Z+mb, p.X+mb+mb, 100, p.Z+mb+mb ), this );
		}
		else
		{
			level = 2;
			patch = new CPatch( box.MinEdge, nPatches );
			box.addInternalBox( patch->getBoundingBox() );

			CTrunk* p = parent;
			//while ( p != 0 )
			//{
			//	p->box.addInternalBox( patch->getBoundingBox() );
			//	p = p->parent;
			//}
		}
	}
public:
	core::aabbox3df box;
	CTrunk* childs[4];
	CPatch* patch;
	int level;

	CTrunk( int numpatches )
	{
		patch = 0;
		nPatches = numpatches;
		int np = (numpatches * 80) / 2;
		initBox( aabbox3df( -np, 0, -np, np, 100, np ) );
		parent = 0;
	}
	CTrunk( core::aabbox3df aBox, CTrunk* aParent )
	{
		patch = 0;
		nPatches = aParent->nPatches;
		parent = aParent;
		initBox( aBox );
	}
	~CTrunk()
	{
		delete childs[0];
		delete childs[1];
		delete childs[2];
		delete childs[3];
		delete patch;
	}
	void Render( irr::scene::ISceneManager* smgr, video::IVideoDriver* driver )
	{
		// if its a patch level, render the patch
		if ( level == 2 )
		{
			patch->DrawLOD( smgr, driver );
		}
		else if ( level == 1 )
		{
			// if its a trunk level, test if its boundingbox is in active camera view
			// if it is, call Render for its four childs, otherwise, cancel at this point
			irr::scene::ICameraSceneNode* camera = smgr->getActiveCamera();
			if ( box.intersectsWithBox( camera->getViewFrustum()->getBoundingBox() ) )
			{
				childs[0]->Render( smgr, driver );
				childs[1]->Render( smgr, driver );
				childs[2]->Render( smgr, driver );
				childs[3]->Render( smgr, driver );
			}
		}
	}
	void setMaxLodDistance( float value )
	{
		if ( level == 1 )
			patch->setMaxLodDistance( value );
		else if ( level == 2 )
		{
			childs[0]->setMaxLodDistance( value );
			childs[1]->setMaxLodDistance( value );
			childs[2]->setMaxLodDistance( value );
			childs[3]->setMaxLodDistance( value );
		}
	}
};

class CQuadTreeTerrain : public scene::ISceneNode
{
private:
	CTrunk* trunk;
	core::aabbox3d<f32> Box;
	video::SMaterial Material;
	video::SMaterial Detail;
public:

	CQuadTreeTerrain( scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id, int numpatches ) : scene::ISceneNode(parent, mgr, id)
	{
		trunk = new CTrunk( numpatches );
		Material.Wireframe = false;
		Material.Lighting = false;

		Box = trunk->box;
	}

	~CQuadTreeTerrain()
	{
		delete trunk;
	}

	virtual void OnRegisterSceneNode()
	{
		if (IsVisible)
			SceneManager->registerNodeForRendering(this);

		ISceneNode::OnRegisterSceneNode();
	}

	virtual void render()
	{
		video::IVideoDriver* driver = SceneManager->getVideoDriver();
		driver->setMaterial(Material);
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

		trunk->Render( SceneManager, driver );
	}

	virtual const core::aabbox3d<f32>& getBoundingBox() const
	{
		return Box;
	}

	virtual u32 getMaterialCount() const
	{
		return 2;
	}

	virtual video::SMaterial& getMaterial(u32 i)
	{
		if ( i == 0 )
			return Material;
		else if ( i == 1 )
			return Detail;
	}

	void setMaxLodDistance( float value )
	{
		trunk->setMaxLodDistance( value );
	}
};

int main()
{

	IrrlichtDevice *device =createDevice(video::EDT_OPENGL, core::dimension2d<s32>(640, 480), 16, false);
	device->setWindowCaption(L"QuadTree + SIMPLEX - Irrlicht Engine Demo");
	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS( );
	// farvalue influences the rendering speed, coz patches out of camera are not rendered
	cam->setFarValue( 5000 );

	smgr->addLightSceneNode(NULL, core::vector3df(0,50,0), video::SColorf(255.0f, 255.0f, 255.0f), 500, -1);

	CQuadTreeTerrain* ct;
	irr::c8* tex = "tex1.jpg";
	irr::c8* detail = "detailmap3.jpg";

	// numpatches represents the patch count at one side ( ie. a value of 30 will generate 30 x 30 patches )
	// here a terrain with a 10000 patches ( terrain with 8000 x 8000 units - each patch have 80 x 80 units )
	ct = new CQuadTreeTerrain( smgr->getRootSceneNode(), smgr, 45, 30 );

	ct->setMaterialTexture(0, driver->getTexture(tex));
	ct->setMaterialTexture(1, driver->getTexture(detail));
	ct->setMaterialType(video::EMT_DETAIL_MAP);

	u32 frames = 0;
	while (device->run())
	{
		driver->beginScene(true, true, video::SColor(0,100,100,100));
		smgr->drawAll();
		driver->endScene();
		if (++frames==100)
		{
			core::stringw str = L"Irrlicht Engine [";
			str += driver->getName();
			str += L"] FPS: ";
			str += (s32)driver->getFPS();

			device->setWindowCaption(str.c_str());
			frames=0;
		}
		device->sleep(5,0);
	}
	device->drop();
	return 0;
	delete ct;
}

