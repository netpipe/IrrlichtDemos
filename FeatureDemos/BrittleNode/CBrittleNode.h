//////////////////////////////////////////////////////////////////////////////
//																			//
//	CBrittleNode v0.5b														//
//	Description: A node that can be cut dynamically by a plane				//
//																			//
//	Author:	Caswal John Parker												//
//	Website:   http://caswal.com (Currently Down as of 5/7/07)				//
//	Email:     caswal at caswal dot com (Currently Down as of 5/7/07)		//
//				caswal at gmail dot com										//
//																			//
//	Date:		5/7/2007													//
//   Copyright:	2007														//
//																			//
//   Licenese:	Free to use by anyone for any project, so long				//
//				as credit is given to me (Caswal John Parker),				//
//				and any other contributes									//
//																			//
//	Acknowledgements: 														//
//				Irrlicht community for being so fantastic, Niko and others	//
//				for creating such an awesome 3d Renderer					//
//																			//
//  Apologies:	I worked on this project for a while, and searched the		//
//				Irrlicht forums on many subjects, if I have copied source	//
//				and not acknowledged you, I apologise						//
//																			//
//																			//
//////////////////////////////////////////////////////////////////////////////

#ifndef _CBRITTLENODE_H_
#define _CBRITTLENODE_H_

#include <irrlicht.h>

//Irrlicts array type has no resize to it
//Just reallocate, this is too limiting
#include <vector>

namespace irr {

class CBrittleNode : public scene::ISceneNode
{

public:

	CBrittleNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id);

	virtual ~CBrittleNode();

	void LoadMesh( char *File, video::ITexture *Diffuse = 0, video::ITexture *Normal = 0);
	//void SetUpMesh( std::vector<video::S3DVertex> &Verts, std::vector<u16> &Indcies);
	void SetUpMesh( video::S3DVertex *Verts, u16 VertCount, u16 *Indicies, u32 IndexCount );

	//Pass in a line in global coordinates, it will return you a point where there is a collision
	//In mesh space coordinates, Used in conjuction with CrackMesh
	//The normal always has to be facing up
	bool IntersectsWithLine( core::line3df &Line, core::vector3df &Point );

	//A point in mesh space coordinates, and a Normal to create a plane.
	CBrittleNode * CrackMesh( core::vector3df Point, core::vector3df Normal); 

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
		if (!IsNormalMapped)
			driver->drawIndexedTriangleList(&Vertices[0], VertCount, &Indices[0], IndexCount / 3);
		else
			driver->drawIndexedTriangleList(&NormalVerts[0], VertCount, &Indices[0], IndexCount / 3);
	}


	virtual const core::aabbox3d<f32>& getBoundingBox() const
	{
		return Box;
	}

	virtual u32 getMaterialCount()
	{
		return 1;
	}

	virtual video::SMaterial& getMaterial(u32 num)
	{
		return Material;
	}	
	
	std::vector< video::S3DVertex>& GetVerts();

private:
	core::aabbox3d<f32> Box;
	std::vector< video::S3DVertex> Vertices;
	std::vector<video::S3DVertexTangents> NormalVerts;
	

	video::SMaterial Material;
	u16 *Indices;
	int VertCount;
	int IndexCount;
	int Count;
	scene::ISceneManager* smgr;
	bool IsNormalMapped;
	

	core::vector3df mCentre;

	//Used for collision
	core::array< core::triangle3df> mTriangleList;

	static void calculateTangents(core::vector3df& normal, 
		core::vector3df& tangent, 
		core::vector3df& binormal, 
		core::vector3df& vt1, core::vector3df& vt2, core::vector3df& vt3,
		core::vector2df& tc1, core::vector2df& tc2, core::vector2df& tc3);

	void CreateNormalMappedMesh();

};

} //Namespace IRR

#endif