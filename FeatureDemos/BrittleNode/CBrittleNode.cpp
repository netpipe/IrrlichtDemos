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
//   License:	Free to use by anyone for any project, so long				//
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

#include "CBrittleNode.h"
#include <algorithm>


namespace irr {

CBrittleNode::CBrittleNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id)
	: scene::ISceneNode(parent, mgr, id)
{
	smgr = mgr;
	Vertices.clear(); 
	Indices = NULL;
	IndexCount = 0;
	VertCount = 0;
	Count = 0;
	IsNormalMapped = false;
	NormalVerts.clear();

	Box.reset(core::vector3df(0,0,0));

}

CBrittleNode::~CBrittleNode()
{

	if ( Indices )
	{
		delete[] Indices;
		Indices = NULL;
	}

}

void CBrittleNode::LoadMesh( char *File, video::ITexture *Diffuse, video::ITexture *Normal )
{
	//Load the Mesh
	scene::IAnimatedMesh *Mesh= smgr->getMesh(File);

	//Get the First mesh from the mesh buffer
	scene::IMeshBuffer *MeshBuffer = Mesh->getMesh(0)->getMeshBuffer(0);

	//Get theMaterial data and store it for later use
	Material = Mesh->getMesh(0)->getMeshBuffer(0)->getMaterial();
	//Mesh->drop();

	if ( Diffuse != NULL )
		Material.setTexture(0, Diffuse );


	//New we need to get the Verts and the Index buffer to use it to draw
	video::S3DVertex *Verts = (video::S3DVertex*)MeshBuffer->getVertices();

	u16 *IndexBuffer = MeshBuffer->getIndices();

	VertCount = MeshBuffer->getVertexCount();
	IndexCount = MeshBuffer->getIndexCount();

	Vertices.resize( IndexCount * 3 );
	

	Indices = new u16[IndexCount];	

	//Generate the Bounding Box
	for (int i = 0; i < IndexCount; i++)
	{
		//Vertices[i] = Verts[IndexBuffer[i]];
		Vertices[i] =  Verts[IndexBuffer[i]];
		Box.addInternalPoint( Vertices[i].Pos );
		Indices[i] = i;
	}

	//Clear the Triangle List, and fill it with all the triangles
	//This is used for my own collision tests
	mTriangleList.clear();

	for (int i = 0; i < IndexCount; i+=3)
	{
		core::triangle3df Triangle;

		Triangle.set( Verts[Indices[i]].Pos, Verts[Indices[i+1]].Pos, Verts[Indices[i+2]].Pos );

		mTriangleList.push_back(Triangle);
	}

	//If the Mesh is normal mapped, we need to generate a mesh with tangents
	if ( Normal )
	{
		Material.setTexture(1, Normal );
		Material.MaterialType = video::EMT_NORMAL_MAP_SOLID;

		this->CreateNormalMappedMesh();
	}

}

//Creates a Vertex buffer with tangents etc
void CBrittleNode::CreateNormalMappedMesh()
{
	IsNormalMapped = true;
	NormalVerts.clear();
	NormalVerts.resize( IndexCount*3 );
//	NormalVerts = new video::S3DVertexTangents[IndexCount*3];

	for (s32 i=0; i<IndexCount; ++i)
	{
		NormalVerts[Indices[i]] = 
			video::S3DVertexTangents(
			Vertices[Indices[i]].Pos, Vertices[Indices[i]].Color, Vertices[Indices[i]].TCoords);
	}


	for (s32 i=0; i<IndexCount; i+=3)
	{
		calculateTangents(
			NormalVerts[Indices[i+0]].Normal,
			NormalVerts[Indices[i+0]].Tangent,
			NormalVerts[Indices[i+0]].Binormal,
			NormalVerts[Indices[i+0]].Pos,
			NormalVerts[Indices[i+1]].Pos,
			NormalVerts[Indices[i+2]].Pos,
			NormalVerts[Indices[i+0]].TCoords,
			NormalVerts[Indices[i+1]].TCoords,
			NormalVerts[Indices[i+2]].TCoords);

		calculateTangents(
			NormalVerts[Indices[i+1]].Normal,
			NormalVerts[Indices[i+1]].Tangent,
			NormalVerts[Indices[i+1]].Binormal,
			NormalVerts[Indices[i+1]].Pos,
			NormalVerts[Indices[i+2]].Pos,
			NormalVerts[Indices[i+0]].Pos,
			NormalVerts[Indices[i+1]].TCoords,
			NormalVerts[Indices[i+2]].TCoords,
			NormalVerts[Indices[i+0]].TCoords);

		calculateTangents(
			NormalVerts[Indices[i+2]].Normal,
			NormalVerts[Indices[i+2]].Tangent,
			NormalVerts[Indices[i+2]].Binormal,
			NormalVerts[Indices[i+2]].Pos,
			NormalVerts[Indices[i+0]].Pos,
			NormalVerts[Indices[i+1]].Pos,
			NormalVerts[Indices[i+2]].TCoords,
			NormalVerts[Indices[i+0]].TCoords,
			NormalVerts[Indices[i+1]].TCoords);
	}
}

//After the mesh is cracked, we need to set it up again
void CBrittleNode::SetUpMesh( video::S3DVertex *Verts, u16 VertCount, u16 *Indicies, u32 IndexCount )
{
	//Delete the old vertices
	Vertices.clear();
	
	if ( Indices )
		delete[] Indices;

	//Create the nice new buffers for them
	Vertices.resize( VertCount );
	//Vertices = new video::S3DVertex [VertCount];
	Indices = new u16[IndexCount];

	//Store the new size of the Buffers
	this->IndexCount = IndexCount;
	this->VertCount = VertCount;

	//Populate it
	//Why the hell arn't i using memcpy????
	for (unsigned int i = 0; i < IndexCount; i++)
	{
		Indices[i] = Indicies[i];
	}


	//Repopulate the triangelist for collision
	mTriangleList.clear();

	for (unsigned int i = 0; i < IndexCount; i+=3)
	{
		core::triangle3df Triangle;

		Triangle.set( Verts[Indices[i]].Pos, Verts[Indices[i+1]].Pos, Verts[Indices[i+2]].Pos );

		mTriangleList.push_back(Triangle);
	}


	for (int i = 0; i < VertCount; i++)
	{
		mCentre = mCentre + Verts[i].Pos;
	}

	mCentre = mCentre / VertCount;

	Box.reset( mCentre );

	for (int i = 0; i < VertCount; i++)
	{
		Verts[i].Pos = Verts[i].Pos - mCentre;
		Vertices[i] = Verts[i];
		Box.addInternalPoint( Vertices[i].Pos );
	}

	if ( IsNormalMapped )
		this->CreateNormalMappedMesh();


}

bool CBrittleNode::IntersectsWithLine( core::line3df &Line, core::vector3df &Point )
{
	//core::vector3df LineVector = Line.getVector().normalize();
	Line.start = Line.start - this->getPosition();
	Line.end = Line.end - this->getPosition();

	for(unsigned int i = 0; i < mTriangleList.size(); i++)
	{
		//if ( mTriangleList[i].getNormal().dotProduct( LineVector ) > 0 ) //Its facing me, do a collision check
		{
			if (mTriangleList[i].getIntersectionWithLimitedLine( Line, Point ) )
				return true;
		}
	}

	return false;
}

//The core of this node, this huge un optimized but working 600 line function
//Pass in a point and a Normal to cut perpendicular to
//The normal always has to be facing up
CBrittleNode *CBrittleNode::CrackMesh(core::vector3df Point, core::vector3df Normal)
{
	//The plane everything is cut along
	core::plane3df CutPlane( Point, Normal.normalize() );

	//Does this plane intersect the bounding box?
	if ( Box.classifyPlaneRelation( CutPlane ) != core::ISREL3D_CLIPPED )
		return NULL;

	//The arrays for the new meshes
	std::vector<video::S3DVertex> VertsSide1;
	std::vector<video::S3DVertex> VertsSide2;
	core::array<u16> IndexSide1;
	core::array<u16>  IndexSide2;

	//And array to store the intersections, to weld up the end of the mesh
	core::array<video::S3DVertex> Intersections;


	//Ok now this is the core of this function
	//We work our way through the triangle list
	//If the triangle does not cross the plane it is alloted into one or the other
	//Vertex and Index arrays
	//If the triangle does intersect, it has to be cut, and new triangles generated.

	for(int i = 0; i < IndexCount; i+=3)
	{
		//Get the current Triangle Indexes
		unsigned int vi1 = Indices[i];
		unsigned int vi2 = Indices[i+1];
		unsigned int vi3 = Indices[i+2];

		//Get the centre point of the plane
		core::vector3df PlanePoint = CutPlane.getMemberPoint();

		//Create a lines from the triangles to the plane
		core::vector3df LineToCentreOfPlane1 = Vertices[vi1].Pos - PlanePoint;
		core::vector3df LineToCentreOfPlane2 = Vertices[vi2].Pos - PlanePoint;
		core::vector3df LineToCentreOfPlane3 = Vertices[vi3].Pos - PlanePoint;

		//Get what side the vertices lie on
		bool Result1 = CutPlane.isFrontFacing( LineToCentreOfPlane1 );
		bool Result2 = CutPlane.isFrontFacing( LineToCentreOfPlane2 );
		bool Result3 = CutPlane.isFrontFacing( LineToCentreOfPlane3 );

		//If they are all on one side or the other then its good
		if ( Result1 && Result2 && Result3 )
		{
			VertsSide1.push_back( Vertices[vi1] );
			IndexSide1.push_back( VertsSide1.size() - 1 );
			VertsSide1.push_back( Vertices[vi2] );
			IndexSide1.push_back( VertsSide1.size() - 1 );
			VertsSide1.push_back( Vertices[vi3] );
			IndexSide1.push_back( VertsSide1.size() - 1 );

		}
		else if ( !Result1 && !Result2 && !Result3 ) 
		{
			VertsSide2.push_back( Vertices[vi1] );
			IndexSide2.push_back( VertsSide2.size() - 1 );
			VertsSide2.push_back( Vertices[vi2] );
			IndexSide2.push_back( VertsSide2.size() - 1 );
			VertsSide2.push_back( Vertices[vi3] );
			IndexSide2.push_back( VertsSide2.size() - 1 );

		}
		else //Oh dear we need to do something about this tri
		{
			//Create 3 lines that make the Tri
			core::vector3df Line1 =  Vertices[vi1].Pos  -  Vertices[vi2].Pos;
			core::vector3df Line2 =  Vertices[vi2].Pos  -  Vertices[vi3].Pos;
			core::vector3df Line3 =  Vertices[vi3].Pos  -  Vertices[vi1].Pos;
			//There will be 2 lines of the triangle that are intersected with the plane
			//Draw a diagram and it proves it
			//But we done know which 2 lines of the three will be intersected so we will have to do a test
			core::vector3df Intersection1, Intersection2, Intersection3;

			//Get the intersections
			bool RInter1 = CutPlane.getIntersectionWithLimitedLine( Vertices[vi1].Pos, Vertices[vi2].Pos, Intersection1 );
			bool RInter2 = CutPlane.getIntersectionWithLimitedLine( Vertices[vi2].Pos, Vertices[vi3].Pos, Intersection2 );
			bool RInter3 = CutPlane.getIntersectionWithLimitedLine( Vertices[vi3].Pos, Vertices[vi1].Pos, Intersection3 );
			//If that line has been intersected, then we need to add the intersections to our list
			if (RInter1)
			{
				video::S3DVertex NewVert;
				NewVert.Pos = Intersection1;
				NewVert.TCoords = Vertices[vi1].TCoords;
				NewVert.Color = Vertices[vi1].Color;
				Intersections.push_back( NewVert );
			}

			if (RInter2)
			{
				video::S3DVertex NewVert;
				NewVert.Pos = Intersection2;
				NewVert.TCoords = Vertices[vi2].TCoords;
				NewVert.Color = Vertices[vi2].Color;
				Intersections.push_back( NewVert );
			}

			if (RInter3)
			{
				video::S3DVertex NewVert;
				NewVert.Pos = Intersection3;
				NewVert.TCoords = Vertices[vi3].TCoords;
				NewVert.Color = Vertices[vi3].Color;
				Intersections.push_back( NewVert );
			}
			/*
			   1
			   /\
			  /  \
			 /____\
			2      3
			*/
			//Depending on which line was not in the intersected we can split up the triangle
			//Line 1 is comprised of points 1 and 2, and it is not intersected by the plane
			//Then Line 1-3 and 2-3 have been intersected
			//So Point 3 lies on one side of the plane
			//And points 1 & 2 are on the other side
			//The rest of the if statements are based on similar code
			if (!RInter1)
			{
				if (Result3)
				{
					VertsSide1.push_back( Vertices[vi3] );
					IndexSide1.push_back( VertsSide1.size() - 1 );

					//Need to create a new vertex for this new triangle
					video::S3DVertex NewVert;
					NewVert.Pos = Intersection3;
					//Thew UV is going to have to interpolated
					float LerpF = static_cast<float>((Vertices[vi3].Pos - Intersection3).getLength() / Line3.getLength());

					NewVert.TCoords.interpolate( Vertices[vi1].TCoords, Vertices[vi3].TCoords, LerpF );


					VertsSide1.push_back( NewVert );
					IndexSide1.push_back( VertsSide1.size() - 1 );

					NewVert.Pos = Intersection2;
					NewVert.Color = Vertices[vi3].Color;
					//Thew UV is going to have to interpolated
					LerpF = static_cast<float>((Vertices[vi3].Pos - Intersection2).getLength() / Line2.getLength());

					NewVert.TCoords.interpolate( Vertices[vi2].TCoords, Vertices[vi3].TCoords, LerpF );

					VertsSide1.push_back( NewVert );
					IndexSide1.push_back( VertsSide1.size() - 1 );

					//Now for the new Triangles
					NewVert.Pos = Intersection2;
					//Thew UV is going to have to interpolated
					LerpF = static_cast<float>((Vertices[vi3].Pos - Intersection2).getLength() / Line2.getLength());

					NewVert.TCoords.interpolate( Vertices[vi2].TCoords, Vertices[vi3].TCoords, LerpF );

					VertsSide2.push_back( NewVert );
					IndexSide2.push_back( VertsSide2.size() - 1 );

					VertsSide2.push_back( Vertices[vi1] );
					IndexSide2.push_back( VertsSide2.size() - 1 );

					VertsSide2.push_back( Vertices[vi2] );
					IndexSide2.push_back( VertsSide2.size() - 1 );

					VertsSide2.push_back( Vertices[vi1] );
					IndexSide2.push_back( VertsSide2.size() - 1 );

					VertsSide2.push_back( NewVert );
					IndexSide2.push_back( VertsSide2.size() - 1 );

					NewVert.Pos = Intersection3;
					//Thew UV is going to have to interpolated
					LerpF = static_cast<float>((Vertices[vi3].Pos - Intersection3).getLength() / Line3.getLength());

					NewVert.TCoords.interpolate( Vertices[vi1].TCoords, Vertices[vi3].TCoords, LerpF );

					VertsSide2.push_back( NewVert );
					IndexSide2.push_back( VertsSide2.size() - 1 );



				} else
				{
					VertsSide2.push_back( Vertices[vi3] );
					IndexSide2.push_back( VertsSide2.size() - 1 );

					//Need to create a new vertex for this new triangle
					video::S3DVertex NewVert;
					NewVert.Color = Vertices[vi3].Color;
					NewVert.Pos = Intersection3;
					//Thew UV is going to have to interpolated
					float LerpF = static_cast<float>((Vertices[vi3].Pos - Intersection3).getLength() / Line3.getLength());

					NewVert.TCoords.interpolate( Vertices[vi1].TCoords, Vertices[vi3].TCoords, LerpF );


					VertsSide2.push_back( NewVert );
					IndexSide2.push_back( VertsSide2.size() - 1 );

					NewVert.Pos = Intersection2;
					//Thew UV is going to have to interpolated
					LerpF = static_cast<float>((Vertices[vi3].Pos - Intersection2).getLength() / Line2.getLength());

					NewVert.TCoords.interpolate( Vertices[vi3].TCoords, Vertices[vi2].TCoords, 1-LerpF );


					VertsSide2.push_back( NewVert );
					IndexSide2.push_back( VertsSide2.size() - 1 );

					//Now for the new Triangles
					NewVert.Pos = Intersection2;
					//Thew UV is going to have to interpolated
					LerpF = static_cast<float>((Vertices[vi3].Pos - Intersection2).getLength() / Line2.getLength());

					NewVert.TCoords.interpolate( Vertices[vi3].TCoords, Vertices[vi2].TCoords, 1-LerpF );


					VertsSide1.push_back( NewVert );
					IndexSide1.push_back( VertsSide1.size() - 1 );

					VertsSide1.push_back( Vertices[vi1] );
					IndexSide1.push_back( VertsSide1.size() - 1 );

					VertsSide1.push_back( Vertices[vi2] );
					IndexSide1.push_back( VertsSide1.size() - 1 );

					VertsSide1.push_back( Vertices[vi1] );
					IndexSide1.push_back( VertsSide1.size() - 1 );

					VertsSide1.push_back( NewVert );
					IndexSide1.push_back( VertsSide1.size() - 1 );

					NewVert.Pos = Intersection3;
					//Thew UV is going to have to interpolated
					LerpF = static_cast<float>((Vertices[vi3].Pos - Intersection3).getLength() / Line3.getLength());

					NewVert.TCoords.interpolate( Vertices[vi1].TCoords, Vertices[vi3].TCoords, LerpF );

					VertsSide1.push_back( NewVert );
					IndexSide1.push_back( VertsSide1.size() - 1 );
				}

			}
			else if (!RInter2)
			{
				if (Result1)
				{
					VertsSide1.push_back( Vertices[vi1] );
					IndexSide1.push_back( VertsSide1.size() - 1 );

					//Need to create a new vertex for this new triangle
					video::S3DVertex NewVert;
					NewVert.Color = Vertices[vi3].Color;
					NewVert.Pos = Intersection1;
					//Thew UV is going to have to interpolated
					float LerpF = static_cast<float>((Vertices[vi1].Pos - Intersection1).getLength() / Line1.getLength());

					NewVert.TCoords.interpolate( Vertices[vi1].TCoords, Vertices[vi2].TCoords, 1-LerpF );


					VertsSide1.push_back( NewVert );
					IndexSide1.push_back( VertsSide1.size() - 1 );

					NewVert.Pos = Intersection3;
					//Thew UV is going to have to interpolated
					LerpF = static_cast<float>((Vertices[vi1].Pos - Intersection3).getLength() / Line3.getLength());

					NewVert.TCoords.interpolate( Vertices[vi3].TCoords, Vertices[vi1].TCoords, LerpF );

					VertsSide1.push_back( NewVert );
					IndexSide1.push_back( VertsSide1.size() - 1 );

					//Now for the new Triangles
					NewVert.Pos = Intersection3;
					//Thew UV is going to have to interpolated
					LerpF = static_cast<float>((Vertices[vi1].Pos - Intersection3).getLength() / Line3.getLength());

					NewVert.TCoords.interpolate( Vertices[vi3].TCoords, Vertices[vi1].TCoords, LerpF );


					VertsSide2.push_back( NewVert );
					IndexSide2.push_back( VertsSide2.size() - 1 );

					VertsSide2.push_back( Vertices[vi2] );
					IndexSide2.push_back( VertsSide2.size() - 1 );

					VertsSide2.push_back( Vertices[vi3] );
					IndexSide2.push_back( VertsSide2.size() - 1 );

					VertsSide2.push_back( NewVert );
					IndexSide2.push_back( VertsSide2.size() - 1 );

					NewVert.Pos = Intersection1;
					//Thew UV is going to have to interpolated
					LerpF = static_cast<float>((Vertices[vi1].Pos - Intersection1).getLength() / Line1.getLength());

					NewVert.TCoords.interpolate( Vertices[vi1].TCoords, Vertices[vi2].TCoords, 1-LerpF );

					VertsSide2.push_back( NewVert );
					IndexSide2.push_back( VertsSide2.size() - 1 );

					VertsSide2.push_back( Vertices[vi2] );
					IndexSide2.push_back( VertsSide2.size() - 1 );


				} else
				{
					VertsSide2.push_back( Vertices[vi1] );
					IndexSide2.push_back( VertsSide2.size() - 1 );

					//Need to create a new vertex for this new triangle
					video::S3DVertex NewVert;
					NewVert.Pos = Intersection1;
					NewVert.Color = Vertices[vi3].Color;
					//Thew UV is going to have to interpolated
					float LerpF = static_cast<float>((Vertices[vi1].Pos - Intersection1).getLength() / Line1.getLength());

					NewVert.TCoords.interpolate( Vertices[vi1].TCoords, Vertices[vi2].TCoords, 1-LerpF );


					VertsSide2.push_back( NewVert );
					IndexSide2.push_back( VertsSide2.size() - 1 );

					NewVert.Pos = Intersection3;
					//Thew UV is going to have to interpolated
					LerpF = static_cast<float>((Vertices[vi1].Pos - Intersection3).getLength() / Line3.getLength());

					NewVert.TCoords.interpolate( Vertices[vi3].TCoords, Vertices[vi1].TCoords, LerpF );

					VertsSide2.push_back( NewVert );
					IndexSide2.push_back( VertsSide2.size() - 1 );

					//Now for the new Triangles
					NewVert.Pos = Intersection3;
					//Thew UV is going to have to interpolated
					LerpF = static_cast<float>((Vertices[vi1].Pos - Intersection3).getLength() / Line3.getLength());

					NewVert.TCoords.interpolate( Vertices[vi3].TCoords, Vertices[vi1].TCoords, LerpF );

					VertsSide1.push_back( NewVert );
					IndexSide1.push_back( VertsSide1.size() - 1 );

					VertsSide1.push_back( Vertices[vi2] );
					IndexSide1.push_back( VertsSide1.size() - 1 );

					VertsSide1.push_back( Vertices[vi3] );
					IndexSide1.push_back( VertsSide1.size() - 1 );

					VertsSide1.push_back( NewVert );
					IndexSide1.push_back( VertsSide1.size() - 1 );

					NewVert.Pos = Intersection1;
					//Thew UV is going to have to interpolated
					LerpF = static_cast<float>((Vertices[vi1].Pos - Intersection1).getLength() / Line1.getLength());

					NewVert.TCoords.interpolate( Vertices[vi1].TCoords, Vertices[vi2].TCoords, 1-LerpF );

					VertsSide1.push_back( NewVert );
					IndexSide1.push_back( VertsSide1.size() - 1 );

					VertsSide1.push_back( Vertices[vi2] );
					IndexSide1.push_back( VertsSide1.size() - 1 );
				}
			}
			else if (!RInter3) // This one is GOOD!!!
			{
				if (!Result1)
				{
					float LerpF = 0;
					video::S3DVertex NewVert;
					VertsSide1.push_back( Vertices[vi2] );
					IndexSide1.push_back( VertsSide1.size() - 1 );

					//Need to create a new vertex for this new triangle
					NewVert.Pos = Intersection2;
					//Thew UV is going to have to interpolated
					LerpF = static_cast<float>((Vertices[vi2].Pos - Intersection2).getLength() / Line2.getLength());

					NewVert.TCoords.interpolate( Vertices[vi2].TCoords, Vertices[vi3].TCoords, 1-LerpF );


					VertsSide1.push_back( NewVert );
					IndexSide1.push_back( VertsSide1.size() - 1 );

					NewVert.Pos = Intersection1;
					NewVert.Color = Vertices[vi2].Color;
					//Thew UV is going to have to interpolated
					LerpF = static_cast<float>((Vertices[vi2].Pos - Intersection1).getLength() / Line1.getLength());

					NewVert.TCoords.interpolate( Vertices[vi2].TCoords, Vertices[vi1].TCoords, 1-LerpF );

					VertsSide1.push_back( NewVert );
					IndexSide1.push_back( VertsSide1.size() - 1 );

					//Now for the new Triangles
					NewVert.Pos = Intersection2;
					//Thew UV is going to have to interpolated
					LerpF = static_cast<float>((Vertices[vi2].Pos - Intersection2).getLength() / Line2.getLength());
					NewVert.TCoords.interpolate( Vertices[vi2].TCoords, Vertices[vi3].TCoords, 1-LerpF );

					VertsSide2.push_back( NewVert );
					IndexSide2.push_back( VertsSide2.size() - 1 );

					VertsSide2.push_back( Vertices[vi3] );
					IndexSide2.push_back( VertsSide2.size() - 1 );

					VertsSide2.push_back( Vertices[vi1] );
					IndexSide2.push_back( VertsSide2.size() - 1 );

					VertsSide2.push_back( NewVert );
					IndexSide2.push_back( VertsSide2.size() - 1 );

					VertsSide2.push_back( Vertices[vi1] );
					IndexSide2.push_back( VertsSide2.size() - 1 );

					NewVert.Pos = Intersection1;
					//Thew UV is going to have to interpolated
					LerpF = static_cast<float>((Vertices[vi2].Pos - Intersection1).getLength() / Line1.getLength());

					NewVert.TCoords.interpolate( Vertices[vi2].TCoords, Vertices[vi1].TCoords, 1-LerpF );

					VertsSide2.push_back( NewVert );
					IndexSide2.push_back( VertsSide2.size() - 1 );



				} else
				{
					float LerpF = 0;
					VertsSide2.push_back( Vertices[vi2] );
					IndexSide2.push_back( VertsSide2.size() - 1 );

					//Need to create a new vertex for this new triangle
					video::S3DVertex NewVert;
					NewVert.Color = Vertices[vi2].Color;
					NewVert.Pos = Intersection2;
					//Thew UV is going to have to interpolated
					LerpF = static_cast<float>((Vertices[vi2].Pos - Intersection2).getLength() / Line2.getLength());

					NewVert.TCoords.interpolate( Vertices[vi2].TCoords, Vertices[vi3].TCoords, 1-LerpF );


					VertsSide2.push_back( NewVert );
					IndexSide2.push_back( VertsSide2.size() - 1 );

					NewVert.Pos = Intersection1;
					//Thew UV is going to have to interpolated
					LerpF = static_cast<float>((Vertices[vi2].Pos - Intersection1).getLength() / Line1.getLength());

					NewVert.TCoords.interpolate( Vertices[vi2].TCoords, Vertices[vi1].TCoords, 1-LerpF );

					VertsSide2.push_back( NewVert );
					IndexSide2.push_back( VertsSide2.size() - 1 );

					//Now for the new Triangles
					NewVert.Pos = Intersection2;
					//Thew UV is going to have to interpolated
					LerpF = static_cast<float>((Vertices[vi2].Pos - Intersection2).getLength() / Line2.getLength());
					NewVert.TCoords.interpolate( Vertices[vi2].TCoords, Vertices[vi3].TCoords, 1-LerpF );

					VertsSide1.push_back( NewVert );
					IndexSide1.push_back( VertsSide1.size() - 1 );

					VertsSide1.push_back( Vertices[vi3] );
					IndexSide1.push_back( VertsSide1.size() - 1 );

					VertsSide1.push_back( Vertices[vi1] );
					IndexSide1.push_back( VertsSide1.size() - 1 );

					VertsSide1.push_back( NewVert );
					IndexSide1.push_back( VertsSide1.size() - 1 );

					VertsSide1.push_back( Vertices[vi1] );
					IndexSide1.push_back( VertsSide1.size() - 1 );

					NewVert.Pos = Intersection1;
					//Thew UV is going to have to interpolated
					LerpF = static_cast<float>((Vertices[vi2].Pos - Intersection1).getLength() / Line1.getLength());

					NewVert.TCoords.interpolate( Vertices[vi2].TCoords, Vertices[vi1].TCoords, 1-LerpF );

					VertsSide1.push_back( NewVert );
					IndexSide1.push_back( VertsSide1.size() - 1 );
				}
			}
		}

	}

	//Need to Loop through and find duplicated intersections and remove them
	for (unsigned int x = 0; x < Intersections.size(); x++)
	{
		for (unsigned int y = 0; y < Intersections.size(); y++)
		{
			if ( x != y )
				if ( (Intersections[x].Pos.X < Intersections[y].Pos.X + 0.01f)
					&& (Intersections[x].Pos.X > Intersections[y].Pos.X - 0.01f))
					if ( (Intersections[x].Pos.Y < Intersections[y].Pos.Y + 0.01f)
						&& (Intersections[x].Pos.Y > Intersections[y].Pos.Y - 0.01f))
						if ( (Intersections[x].Pos.Z < Intersections[y].Pos.Z + 0.01f)
							&& (Intersections[x].Pos.Z > Intersections[y].Pos.Z - 0.01f))
						{
							Intersections.erase( y );
							y--;
						}
		}
	}

	//Now we need to use the new intersected vertices that were generated and produce the caps
	//Get the current side of the Vertex buffer, so we can use this to help generate the index buffer
	int OriginIndex1 = VertsSide1.size();
	int OriginIndex2 = VertsSide2.size();
	
	if ( Intersections.size() == 0 )
	{
		return NULL;
	}

	//This is the origin point, all the cap triangles are produced with this as the start point
	video::S3DVertex Origin = Intersections[0];

	//Erase it out of the list
	//So we dont accidently find it again, and try to make a triangle with it as 2 corners
	Intersections.erase( 0 );

	////We need the centre of one of our new meshes
	//core::vector3df Centre(0,0,0);

	//for (unsigned int i = 0; i < VertsSide1.size(); i++)
	//{
	//	Centre = Centre + VertsSide1[i].Pos;
	//}

	//Centre = Centre / static_cast<float>(VertsSide1.size());

	////We use the centre point and our intersection point
	////To create a Vector
	////We compare this vector to the cross product of the origin point
	////And the 2 new corners of the triangle, so we get the correct winding order
	//core::vector3df CentreToIntersect( Centre - Intersections[0].Pos );

	//CentreToIntersect.normalize();

	//Point A for our cross product calc
	core::vector3df PointA = Intersections[0].Pos;

	//We try to work our way around the intersections in a circle
	//We remove the points we have used as we go
	//Point of interest is the current B point of our new triangle
	//We are finding the closest point to that which will be our C point
	//Now we have A, B, and C we create the triangle
	//The PointOfInterest is then removed from the list
	//And the ClosetPoint is now the Point of interest and it all starts again
	int		PointOfInterest = 0;
	int		ClosestPoint = 1;

	while(Intersections.size() > 1 )
	{
		//Add Point A to the Index Buffer
		IndexSide1.push_back(OriginIndex1);
		IndexSide2.push_back(OriginIndex2);


		float CurrentDistance = 10000000;
		//Find the closest point to Point B (PointofInterest)
		for (unsigned int t = 0; t < Intersections.size(); t++)
		{
			if ( t != PointOfInterest )
				if ( (Intersections[PointOfInterest].Pos - Intersections[t].Pos).getLengthSQ() < CurrentDistance )
				{
					CurrentDistance = (Intersections[PointOfInterest].Pos - Intersections[t].Pos).getLengthSQ();
					ClosestPoint = t;
				}
		}

		//Using a Point A,B,C find the Normal of the Triangle
		core::vector3df TriNormal = ( Intersections[ClosestPoint].Pos - PointA).crossProduct( Intersections[PointOfInterest].Pos - PointA ).normalize();

		//Depending on the result of the dot product of the Face Normal
		//With the Vector heading out from the centre of the new mesh
		//Towards the cap add the vertexs to the mesh accordinly
		if ( TriNormal.dotProduct( Normal ) > 0 )
		{
			VertsSide1.push_back(Intersections[ClosestPoint]);
			IndexSide1.push_back( VertsSide1.size() - 1);

			VertsSide1.push_back(Intersections[PointOfInterest]);
			IndexSide1.push_back( VertsSide1.size() - 1);

			VertsSide2.push_back(Intersections[PointOfInterest]);
			IndexSide2.push_back( VertsSide2.size() - 1);

			VertsSide2.push_back(Intersections[ClosestPoint]);
			IndexSide2.push_back( VertsSide2.size() - 1);
		} else
		{
			VertsSide1.push_back(Intersections[PointOfInterest]);
			IndexSide1.push_back( VertsSide1.size() - 1);

			VertsSide1.push_back(Intersections[ClosestPoint]);
			IndexSide1.push_back( VertsSide1.size() - 1);

			VertsSide2.push_back(Intersections[ClosestPoint]);
			IndexSide2.push_back( VertsSide2.size() - 1);

			VertsSide2.push_back(Intersections[PointOfInterest]);
			IndexSide2.push_back( VertsSide2.size() - 1);
		}

		//Erase our point of interest
		Intersections.erase( PointOfInterest );
		//Our Point of Interest becomes our ClosestPoint
		if ( ClosestPoint < PointOfInterest )
			PointOfInterest = ClosestPoint;
		else
			PointOfInterest = ClosestPoint-1;

	}

	//This now copies our mesh data into the mesh buffers to be used
	//A little pointless and will be optimized later
	this->SetUpMesh( &VertsSide2[0], VertsSide2.size(), &IndexSide2[0], IndexSide2.size());

	//Create a new node
	CBrittleNode *NewNode = new CBrittleNode( smgr->getRootSceneNode(), smgr, 0 );

	//Set its data up
	NewNode->Material = Material;
	NewNode->IsNormalMapped = this->IsNormalMapped;
	NewNode->SetUpMesh( &VertsSide1[0], VertsSide1.size(), &IndexSide1[0], IndexSide1.size() );

	//The positions of the meshes have changed slightly
	//A new origin has been calculated for them which is near their centre
	NewNode->setPosition( this->getPosition() + NewNode->mCentre + core::vector3df(0, 1.f, 0) );
	this->setPosition( this->getPosition() + mCentre );

	return NewNode;
}

void CBrittleNode::calculateTangents(
	core::vector3df& normal,
	core::vector3df& tangent,
	core::vector3df& binormal,
	core::vector3df& vt1, core::vector3df& vt2, core::vector3df& vt3, // vertices
	core::vector2df& tc1, core::vector2df& tc2, core::vector2df& tc3) // texture coords
{

	core::vector3df v1 = vt1 - vt2;
	core::vector3df v2 = vt3 - vt1;
	normal = v2.crossProduct(v1);
	normal.normalize();
	normal = normal * -1;

	// binormal

	f32 deltaX1 = tc1.X - tc2.X;
	f32 deltaX2 = tc3.X - tc1.X;
	binormal = (v1 * deltaX2) - (v2 * deltaX1);
	binormal.normalize();

	// tangent

	f32 deltaY1 = tc1.Y - tc2.Y;
	f32 deltaY2 = tc3.Y - tc1.Y;
	tangent = (v1 * deltaY2) - (v2 * deltaY1);
	tangent.normalize();

	// adjust

	core::vector3df txb = tangent.crossProduct(binormal);
	if (txb.dotProduct(normal) < 0.0f)
	{
		tangent *= -1.0f;
		binormal *= -1.0f;
	}

}

std::vector< video::S3DVertex>& CBrittleNode::GetVerts()
{
	return Vertices;
}



} //Namespace irr