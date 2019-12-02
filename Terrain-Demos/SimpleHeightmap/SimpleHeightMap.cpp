#include "SimpleHeightMap.h"

#include <stdio.h>
#pragma warning( disable : 4996 )



SimpleHeightMap::SimpleHeightMap(scene::ISceneManager* smgr,
																 scene::ISceneNode* parent,
																 s32 id,
																 const io::path& heightMapFileName,
																 const core::vector3df& position,
																 const core::vector3df& rotation,
																 const core::vector3df& scale
																 )
{
	m_Mesh=new scene::SMesh;

	m_position=position;

	int side=129;//<-you might want to get this number from the file, like a 32-bits file gives this: side=sqrt(filesize/4) <-4=the 32 bits
	m_side=side;
	int filesize32=side*side; //how many 32bits numbers we should read

	//unsigned int *data=new unsigned int[filesize32];

	FILE *f;
	f=fopen(heightMapFileName.c_str(),"rb");
	if(f)
	{
		//create the vertexes:
		m_vertices=new video::S3DVertex[filesize32];

		m_data=(float*)malloc(filesize32*4);

		int read=(int)fread(m_data,1,filesize32*4,f);

		//2D position here:
		m_xpos=(int)position.Z;
		m_ypos=(int)position.X;

		m_MapScale=scale.X; //<-must be the same scale for X and Z

		m_width=(int)((side-1)*m_MapScale);
		m_height=(int)((side-1)*m_MapScale);

		m_datapointerwidth=side;
		m_sideminus1=side-1;

		m_size=(float)((side-1)*m_MapScale);




		//read the data from the 32-bits float file (heightmap with float values)
		int p=0;
		float texturedivisor=(float)(side-1);
		for(int x=0;x<side;x++)
		{
			for(int y=0;y<side;y++)
			{
				float tx=1.0f-((float(x))/texturedivisor);
				float ty=((float(y))/texturedivisor);

				float xpos=(float(x))*scale.X+position.X;
				float ypos=m_data[p]*scale.Y+position.Y;
				float zpos=(float(y))*scale.Z+position.Z;

				m_vertices[p]=video::S3DVertex(xpos,ypos,zpos, 0,1,0,	video::SColor(255,255,255,255), tx, ty);

				p++;
			}
		}
		m_vertexcount=p;

		fclose(f);

		//create the indices:
		m_indices=new u32[(side-1)*(side-1)*2*3];

		p=0;
		for(int y=0;y<side-1;y++)
		{
			for(int x=0;x<side-1;x++)
			{
				int v=x+y*side;

				m_indices[p++]=v;
				m_indices[p++]=v+1;
				m_indices[p++]=v+side;

				m_indices[p++]=v+side;
				m_indices[p++]=v+1;
				m_indices[p++]=v+side+1;
			}
		}

		m_primitivecount=p/3;

		//printf("indices=%i\n",p);

		//set the terrains position: (scale is included in the calculations so no need to set that one)
// 		setPosition(position);


		//calculate the normals:

		//calculate all the normals that have 4 neighbors:
		for(int y=1;y<side-1;y++)
		{
			p=y*side+1;
			for(int x=1;x<side-1;x++)
			{
				m_vertices[p].Normal=CalculateNormal(
					m_vertices[p].Pos,
					m_vertices[p-side].Pos,
					m_vertices[p-side+1].Pos,
					m_vertices[p+1].Pos,
					m_vertices[p+side].Pos,
					m_vertices[p+side-1].Pos,
					m_vertices[p-1].Pos);

				p++;
			}
		}

		//do the sides
		for(int i=1;i<side-1;i++)
		{
			//up
			p=i;
			m_vertices[p].Normal=CalculateNormal(
				m_vertices[p].Pos,
				m_vertices[p+1].Pos,
				m_vertices[p+side].Pos,
				m_vertices[p+side-1].Pos,
				m_vertices[p-1].Pos);

			//left
			p=i*side;
			m_vertices[p].Normal=CalculateNormal(
				m_vertices[p].Pos,
				m_vertices[p-side].Pos,
				m_vertices[p-side+1].Pos,
				m_vertices[p+1].Pos,
				m_vertices[p+side].Pos);



			//right
			p=i*side+side-1;
			m_vertices[p].Normal=CalculateNormal(
				m_vertices[p].Pos,
				m_vertices[p+side].Pos,
				m_vertices[p+side-1].Pos,
				m_vertices[p-1].Pos,
				m_vertices[p-side].Pos);



			//down
			p=side*(side-1)+i;
			m_vertices[p].Normal=CalculateNormal(
				m_vertices[p].Pos,
				m_vertices[p-1].Pos,
				m_vertices[p-side].Pos,
				m_vertices[p-side+1].Pos,
				m_vertices[p+1].Pos);
		}

		//do the corners

		//up left
		p=0;
		m_vertices[p].Normal=CalculateNormal(
			m_vertices[p].Pos,
			m_vertices[p+1].Pos,
			m_vertices[p+side].Pos);

		//up right
		p=side-1;
		m_vertices[p].Normal=CalculateNormal(
			m_vertices[p].Pos,
			m_vertices[p+side-1].Pos,
			m_vertices[p-1].Pos);

		//down left
		p=side*(side-1);
		m_vertices[p].Normal=CalculateNormal(
			m_vertices[p].Pos,
			m_vertices[p-side].Pos,
			m_vertices[p-side+1].Pos);

		//down right
		p=side*side-1;
		m_vertices[p].Normal=CalculateNormal(
			m_vertices[p].Pos,
			m_vertices[p-1].Pos,
			m_vertices[p-side].Pos);



		//set the size of the bounding box:

		//set x<->z size of box:
		Box.reset(m_vertices[0].Pos);
		Box.addInternalPoint(m_vertices[side*(side-1)-1].Pos);

		//set y size of box:
		float ymin=m_vertices[0].Pos.Y;
		float ymax=ymin;

		float tmp;

		for(int i=0;i<filesize32;i++)
		{
			tmp=m_vertices[i].Pos.Y;
			if(tmp>ymax)
			{
				ymax=tmp;
			}
			else
			{
				if(tmp<ymin)
					ymin=tmp;
			}
		}

		core::vector3df tmppos=m_vertices[0].Pos;
		tmppos.Y=ymin;
		Box.addInternalPoint(tmppos);

		tmppos.Y=ymax;
		Box.addInternalPoint(tmppos);

		CreateSMesh(smgr,Box);

		m_MeshNode=smgr->addMeshSceneNode(m_Mesh,parent,-1,core::vector3df(0,0,0));

		//m_MeshNode->setPosition(position);
	}
	else
	{
		printf("SimpleHeightMap Error: file not found\n");
	}

}





void SimpleHeightMap::CreateSMesh(scene::ISceneManager* smgr,const core::aabbox3d<f32> &Box)
{
	scene::SMeshBuffer *meshbuffer = new scene::SMeshBuffer();

	int p=0;
	for(int y=0;y<129;y++)
	{
		for(int x=0;x<129;x++)
		{
			meshbuffer->Vertices.push_back(m_vertices[p]);
			p++;
		}
	}

	p=0;
	for(int y=0;y<m_sideminus1;y++)
	{
		for(int x=0;x<m_sideminus1;x++)
		{
			meshbuffer->Indices.push_back(p);
			meshbuffer->Indices.push_back(p+1);
			meshbuffer->Indices.push_back(p+m_side);

			meshbuffer->Indices.push_back(p+1);
			meshbuffer->Indices.push_back(p+1+m_side);
			meshbuffer->Indices.push_back(p+m_side);

			p++;
		}
		p++;
	}

	//meshbuffer->BoundingBox=Box;
	meshbuffer->recalculateBoundingBox();

	printf("BB: %.2f,%.2f,%.2f   %.2f,%.2f,%.2f\n",meshbuffer->BoundingBox.MinEdge.X,meshbuffer->BoundingBox.MinEdge.Y,meshbuffer->BoundingBox.MinEdge.Z,meshbuffer->BoundingBox.MaxEdge.X,meshbuffer->BoundingBox.MaxEdge.Y,meshbuffer->BoundingBox.MaxEdge.Z);


	//use VBO
	meshbuffer->setHardwareMappingHint(scene::EHM_STATIC);

	meshbuffer->setDirty();

	m_Mesh->addMeshBuffer(meshbuffer);

	m_Mesh->recalculateBoundingBox();

	//printf("%i %i\n",meshbuffer->Vertices.size(),meshbuffer->Indices.size());

	delete []m_vertices;
	delete []m_indices;
}


SimpleHeightMap::~SimpleHeightMap()
{
	free(m_data);
	m_Mesh->drop();
}


video::SMaterial& SimpleHeightMap::getMaterial(u32 i)
{
	return m_MeshNode->getMaterial(i);
}

bool SimpleHeightMap::IsOnMap(float x,float z)
{
	//if(!m_IsLoaded)return(false);
	//if(!m_Terrain)return(false);
	if(z>=m_xpos+m_size)return(false);
	if(x>=m_ypos+m_size)return(false);
	if(z<m_xpos)return(false);
	if(x<m_ypos)return(false);
	return(true);
}

f32 SimpleHeightMap::GetHeight(f32 y, f32 x)
{
	float height=0;
	if(x<m_xpos)return(0);
	if(y<m_ypos)return(0);
	if(x>=m_xpos+m_width)return(0);
	if(y>=m_ypos+m_height)return(0);

	//translate pos into map space:
	float fx=x-m_xpos;
	float fy=y-m_ypos;

	//translate pos into vertex space:
	fx/=m_MapScale;
	fy/=m_MapScale;


	//now check which triangle to use:
	// x
	//1|___2
	// |A /|
	// | / |
	// |/_B|___y
	// 0   3
	//get reminders:
	float xrem=fx-((float)((int)fx));
	float yrem=fy-((float)((int)fy));

	int xvertex=(int)fx;
	int yvertex=(int)fy;

	if((xvertex<0)||(xvertex>=m_sideminus1)||(yvertex<0)||(yvertex>=m_sideminus1))
	{
		printf("#error Heightmap calc. position out of bound (%i,%i)\n",xvertex,yvertex);
		return(false);
	}

	unsigned int origo=xvertex+yvertex*m_datapointerwidth;

	if(origo+1+m_datapointerwidth>=257*257)
	{
		printf("#error Heightmap calc. position out of bound (%i>=%i)\n",origo+1+m_datapointerwidth,257*257);
		return(false);
	}

	float r=0;

	//a: xrem>yrem
	if(xrem>yrem)
	{
		//A triangle
		//make an interpolation:
		float h0=m_data[origo]*m_MapScale;
		float h1=m_data[origo+1]*m_MapScale;
		float h2=m_data[origo+1+m_datapointerwidth]*m_MapScale;

		height=h0 + (h2-h1)*yrem + (h1-h0)*xrem;
	}
	else
	{
		//B triangle
		//make an interpolation:
		float h0=m_data[origo]*m_MapScale;
		float h2=m_data[origo+1+m_datapointerwidth]*m_MapScale;
		float h3=m_data[origo+m_datapointerwidth]*m_MapScale;

		height=h0 + (h2-h3)*xrem + (h3-h0)*yrem;
	}

	return(height);
}

//core::vector3df SimpleHeightMap::CalculateNormal(const core::vector3df &center,core::vector3df vx1,core::vector3df vx2,core::vector3df vz3,core::vector3df vz4)


core::vector3df SimpleHeightMap::CalculateNormal(const core::vector3df &X,core::vector3df A,core::vector3df B,core::vector3df C,core::vector3df D,core::vector3df E,core::vector3df F)
{
	//the normal (X) is influenced by 6 other points:

	//   ____A____B
	//  |   /|   /|
	//  |  / |  / |
	//  | /  | /  |
	//  F/___X/___C
	//  |   /|   /|
	//  |  / |  / |
	//  | /  | /  |
	//  E/___D/___|
	//

	//lets calculate 3 normals with cross products between A-X-B, C-X-D and E-X-F
	//add them together and normalize


	core::vector3df c1=A-X;
	c1=c1.crossProduct(B-X);


	core::vector3df c2=C-X;
	c2=c2.crossProduct(D-X);

	core::vector3df c3=E-X;
	c3=c3.crossProduct(F-X);

	core::vector3df n=c1+c2+c3;
	n.normalize();

	return(n);
}


//sides
core::vector3df SimpleHeightMap::CalculateNormal(const core::vector3df &X,core::vector3df A,core::vector3df B,core::vector3df C,core::vector3df D)
{
	//the normal (X) is influenced by 4 other points:

	//  D____X____A
	//  |   /|   /|
	//  |  / |  / |
	//  | /  | /  |
	//  C/___B/___|
	//  |   /|   /|
	//  |  / |  / |
	//  | /  | /  |
	//   /___ /___|
	//

	//lets calculate 2 normals with cross products between A-X-B and C-X-D
	//add them together and normalize

	core::vector3df c1=A-X;
	c1=c1.crossProduct(B-X);

	core::vector3df c2=C-X;
	c2=c2.crossProduct(D-X);

	core::vector3df n=c1+c2;
	n.normalize();

	return(n);
}

//corners
core::vector3df SimpleHeightMap::CalculateNormal(const core::vector3df &X,core::vector3df A,core::vector3df B)
{
	//the normal (X) is influenced by 2 other points:

	//  X____A____
	//  |   /|   /|
	//  |  / |  / |
	//  | /  | /  |
	//  B/___ /___|
	//  |   /|   /|
	//  |  / |  / |
	//  | /  | /  |
	//  |/____/___|
	//

	//lets calculate normal with cross products between A-X-B
	//add them together and normalize

	core::vector3df n=A-X;
	n=n.crossProduct(B-X);

	n.normalize();

	return(n);
}




