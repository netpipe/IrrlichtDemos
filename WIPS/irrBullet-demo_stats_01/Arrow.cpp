//CArrow
//A useful graphical arrow in world space, this has nothing to do with physics!
//We will just use this for a simple graphic effect, but this class is handy to
// show things like collision normals, velocity vectors etc.
#include "Arrow.h"
using namespace core;

/////////////////////////////////////////////////
//your pointer to the Irrlicht device
extern irr::IrrlichtDevice* g_pIrrDevice;
#define MY_IRRLICHT_DEVICE g_pIrrDevice
/////////////////////////////////////////////////

CArrow::CArrow(void) : m_Width(0.1f), m_bHead(true), m_Color(GREY), m_NumSides(NUM_ARROW_SIDES), m_bTransparent(false), m_bHasInitialised(false)
{
}
CArrow::CArrow(video::SColor color, f32 width, bool head, bool transparent, s32 numsides)
			   : m_Width(width), m_bHead(head), m_Color(color), m_bTransparent(transparent), m_NumSides(numsides), m_bHasInitialised(false)
{
}
CArrow::~CArrow(void)
{
}

//Init() - intitialise arrow.
//Since it is much easier to create these arrows on stack instead of new ... delete,
// the initialisation is not done in constructor (since Irrlicht may not be initialised yet)
void CArrow::Init()
{
	if (m_bHasInitialised) return;

	if (m_NumSides<3) m_NumSides=3;
	m_pMesh = new scene::SMesh();
	scene::SMeshBuffer* mb = new scene::SMeshBuffer();
	m_pMesh->addMeshBuffer(mb);
	mb->drop();

	scene::ISceneManager* smgr = MY_IRRLICHT_DEVICE->getSceneManager();
	m_pNode = smgr->addMeshSceneNode(m_pMesh);

	if (m_bTransparent) m_pNode->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA );

	m_bHasInitialised=true;
}

//Cleanup()
// - cannot cleanup stack-created CArrows in destructor, since Irrlicht
// may already have cleaned up these objects and pointers will be invalid
//If you need to cleanup manually, call this function.
//Always call Cleanup() before you call delete on a CArrow pointer
void CArrow::Cleanup(void)
{
	if (m_pMesh) m_pMesh->drop();
	if (m_pNode) m_pNode->remove();
	m_pNode=0;
	m_pMesh=0;
}
void CArrow:: SetLength(f32 length)
{
	Init();
	SetMesh(length);
}
void CArrow:: SetPosition(const core::vector3df& pos)
{
	Init();
	if (m_pNode) m_pNode->setPosition(pos);
}
void CArrow:: SetPosition(f32 x, f32 y, f32 z)
{
	Init();
	if (m_pNode) m_pNode->setPosition(vector3df(x,y,z));
}

void CArrow:: SetRotation(const core::vector3df& rot)
{
	Init();
	if (m_pNode) m_pNode->setRotation(rot);
}
void CArrow:: SetRotation(f32 x, f32 y, f32 z)
{
	Init();
	if (m_pNode) m_pNode->setRotation(vector3df(x,y,z));
}
void CArrow::SetEndpoints(const core::vector3df& start, const core::vector3df& end)
{
	Init();
	SetMesh((f32)(end-start).getLength());
	if (m_pNode) {
		m_pNode->setPosition(start);
		m_pNode->setRotation((end-start).getHorizontalAngle());
	}
}
void CArrow::SetTarget(const core::vector3df& dir)
{
	Init();
	SetMesh((f32)dir.getLength());
	if (m_pNode) {
		vector3df v(dir);
		m_pNode->setRotation(v.getHorizontalAngle());
	}
}
void CArrow::SetTarget(f32 x, f32 y, f32 z)
{
	Init();
	SetTarget(vector3df(x,y,z));
}
void CArrow::SetMesh(f32 length)
{
	scene::SMeshBuffer* mb = (scene::SMeshBuffer*) m_pMesh->getMeshBuffer(0);

	mb->Vertices.clear();
	mb->Indices.clear();

	video::S3DVertex vtx;
	vtx.Color=m_Color;
	vtx.Normal.set(0,0,1);

	vector3df v;
	matrix4 mat;
	f32 headwidth, headheight;

	s32 n = m_NumSides;
	f32 angle = 2*PI / n;

	if (m_bHead) {
        headheight = m_Width*8;
		headwidth = m_Width*4;
	}
	else {
        headheight = 0;
		headwidth = 0;
	}
	f32 linelen = length-headheight;

	//setup verts

	//base of line
	for (s32 i=0; i<n; i++) {
		v.set(0,m_Width,0);
		mat.setRotationRadians(vector3df(0,0,i*angle));
		mat.rotateVect(v);
		vtx.Pos = v;
		vtx.Normal = v.normalize();
		mb->Vertices.push_back(vtx);
	}

	//top of line
	for (s32 i=0; i<n; i++) {
		v.set(0,m_Width,linelen);
		mat.setRotationRadians(vector3df(0,0,i*angle));
		mat.rotateVect(v);
		vtx.Pos = v;
		v.set(0,1,0);
		mat.rotateVect(v);
		vtx.Normal = v;
		mb->Vertices.push_back(vtx);
	}

	//base of arrow head
	for (s32 i=0; i<n; i++) {
		v.set(0,m_Width+headwidth,linelen);
		mat.setRotationRadians(vector3df(0,0,i*angle));
		mat.rotateVect(v);
		vtx.Pos = v;
		v.set(0,1,1);
		mat.rotateVect(v);
		vtx.Normal = v.normalize();
		mb->Vertices.push_back(vtx);
	}

	//verts with normals pointing down, for base cap of arrow head (looks clearer)
	for (s32 i=0; i<n; i++) {
		v.set(0,m_Width+headwidth,linelen);
		mat.setRotationRadians(vector3df(0,0,i*angle));
		mat.rotateVect(v);
		vtx.Pos = v;
		vtx.Normal = vector3df(0,0,-1);
		mb->Vertices.push_back(vtx);
	}

	//apex of arrow head
	//normals approximated
	for (s32 i=0; i<n; i++) {
		vtx.Pos = vector3df(0,0,linelen+headheight);
		mat.setRotationRadians(vector3df(0,0,i*angle));
		v.set(0,1,1);
		mat.rotateVect(v);
		vtx.Normal = v.normalize();
		mb->Vertices.push_back(vtx);
	}

	//center of end of cylinders, using side verts for convenience
	vtx.Normal = vector3df(0.0f,0.0f,1.0f);
	vtx.Pos = vector3df(0, 0, linelen);
	mb->Vertices.push_back(vtx);
	vtx.Normal = vector3df(0,0,-1);
	vtx.Pos = vector3df(0, 0, 0);
	mb->Vertices.push_back(vtx);

	//center of cap of arrowhead
	vtx.Pos = vector3df(0,0,linelen);
	vtx.Normal = vector3df(0,0,-1);
	mb->Vertices.push_back(vtx);


	// do indices

	//sides of line
	for (s32 i=0; i<n; i++) {

		mb->Indices.push_back(i);
		mb->Indices.push_back((i+n+1)%n+n);
		mb->Indices.push_back((i+n)%n+n);

		mb->Indices.push_back(i);
		mb->Indices.push_back((i+1)%n);
		mb->Indices.push_back((i+n+1)%n+n);
	}

	//arrow head
	for (s32 i=n*2; i<n*3; i++) {
		mb->Indices.push_back(i); //i%n+n+n
		mb->Indices.push_back((i+1)%n+n*2);
		mb->Indices.push_back(n*4);
	}

	//cylinder caps
	if (m_bHead) {
		for (s32 i=n*3; i<n*4; i++) {
			mb->Indices.push_back(i);
			mb->Indices.push_back(n*5+2);
			mb->Indices.push_back((i+1)%n+n*3);
		}
	}
	else
	{
		for (s32 i=n; i<n*2; i++) {
			mb->Indices.push_back(i);
			mb->Indices.push_back((i+1)%n+n);
			mb->Indices.push_back(n*5);
		}
	}

	//base cap
	for (s32 i=0; i<n; i++) {
		mb->Indices.push_back(i);
		mb->Indices.push_back(n*5+1);
		mb->Indices.push_back((i+1)%n);
	}

	mb->recalculateBoundingBox();
	m_pMesh->recalculateBoundingBox();
}

