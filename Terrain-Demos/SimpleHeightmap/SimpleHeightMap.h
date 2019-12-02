/*
Use at own risk, do whatever you want with it
as long as you don't change this remark :-)

Created by Ludvig Larsson aka Valmond
*/


#include <irrlicht.h>

using namespace irr;

class SimpleHeightMap //: public scene::ISceneNode
{
public:

	SimpleHeightMap(scene::ISceneManager* smgr,
		scene::ISceneNode* parent,
		s32 id,
		const io::path& heightMapFileName,
		const core::vector3df& position = core::vector3df(0.0f,0.0f,0.0f),
		const core::vector3df& rotation = core::vector3df(0.0f,0.0f,0.0f),
		const core::vector3df& scale = core::vector3df(1.0f,1.0f,1.0f)
		);

	~SimpleHeightMap();

	video::SMaterial& getMaterial(u32 i);

	scene::IMeshSceneNode *GetMeshNode(){return(m_MeshNode);}

	bool IsOnMap(float x,float z);

	f32 getHeight(f32 x, f32 y){return(GetHeight(x,y));}

	f32 GetHeight(f32 y, f32 x);


	//mimmick functions of the ITerrainSceneNode node so we can swap them easily:
	void setMaterialFlag(video::E_MATERIAL_FLAG flag, bool newvalue)
	{
		m_MeshNode->setMaterialFlag(flag,newvalue);
	}

	void setMaterialType(video::E_MATERIAL_TYPE newType)
	{
		m_MeshNode->setMaterialType(newType);
	}

	const core::vector3df& getPosition() const
	{
		return m_position;
	}

	void setVisible(bool isVisible)
	{
		m_MeshNode->setVisible(isVisible);
	}


private:

	core::vector3df CalculateNormal(const core::vector3df &X,core::vector3df A,core::vector3df B,core::vector3df C,core::vector3df D,core::vector3df E,core::vector3df F);
	core::vector3df CalculateNormal(const core::vector3df &X,core::vector3df A,core::vector3df B,core::vector3df C,core::vector3df D);//sides
	core::vector3df CalculateNormal(const core::vector3df &X,core::vector3df A,core::vector3df B);//corners

	//takes the vertices and incices and creates the SMesh
	void CreateSMesh(scene::ISceneManager* smgr,const core::aabbox3d<f32> &Box);

	//data so we can use GetHeight()
	float *m_data;

	//position data for the terrain:
	int m_xpos;
	int m_ypos;
	int m_width;
	int m_height;

	core::vector3df m_position;

	//internal use
	int m_datapointerwidth; //like 129 or 257
	int m_sideminus1;
	float m_size;
	int m_side;

	float m_MapScale;

	//bounding box:
	core::aabbox3d<f32> Box;


	scene::IMeshSceneNode *m_MeshNode;

	//mesh:
	scene::SMesh* m_Mesh;

	video::S3DVertex *m_vertices;
	int m_vertexcount;
	u32 *m_indices;
	int m_primitivecount;

};
