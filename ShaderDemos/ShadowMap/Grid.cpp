#include "Grid.h"

CGrid::CGrid(ISceneNode* parent, ISceneManager* smgr, s32 id, 
		u32 spacing, u32 size,SColor gridcolor,u32 accentlineoffset, 
		video::SColor accentgridcolor, bool axislinestate)	: ISceneNode(parent, smgr, id), 
		m_spacing(spacing), m_size(size), 
		m_gridcolor(gridcolor), m_accentgridcolor(accentgridcolor),
        m_accentlineoffset(accentlineoffset), m_AxisLineState(axislinestate),
		m_XLineColor(video::SColor(255,255,0,0)), m_ZLineColor(video::SColor(255,0,0,255))
{
	// Set the material
	Buffer.Material.Wireframe = false;
	Buffer.Material.Lighting = false;
	Buffer.Material.Thickness = 1;
	Buffer.Material.FogEnable = false;
	Buffer.Material.ZWriteEnable = false;

	// Set the default culling state to Frustum Box
	AutomaticCullingState = EAC_FRUSTUM_BOX;

	RegenerateGrid();
}

void CGrid::OnRegisterSceneNode()
{
	if (IsVisible)
		pScene->registerNodeForRendering(this);

	ISceneNode::OnRegisterSceneNode();
}

void CGrid::RegenerateGrid()
{
	//Clean up memory
	Buffer.Indices.clear();
	Buffer.Vertices.clear();

	u32 m_numVertices = ((m_size / m_spacing) + 1) * 2 * 2;
	if (m_accentlineoffset) m_numVertices += ((m_size / (m_spacing * m_accentlineoffset)) + 1) * 2 * 2;

	if ( m_numVertices > 65535)
	{
		//Too many vertices on 16 bit for for 16bit indices of SMeshBuffer
		//Returning with a blank buffer to avoid segfaulting the entire application
		return;
	}

	//Set our left corner
	vector3df leftMost = vector3df(0);
	leftMost.X -= m_size/2;
	leftMost.Z -= m_size/2;

	//Set our right corner
	vector3df rightMost = vector3df(0);
	rightMost.X += m_size/2;
	rightMost.Z += m_size/2;

	u32 indexIndex = 0;

	//X-axis lines
	for(u32 x = 0; x <= m_size; x+= m_spacing)
	{
		vector3df start = leftMost;
		start.X += x ;

		vector3df end = rightMost;
		end.X = start.X;

		Buffer.Vertices.push_back(S3DVertex(start,vector3df(0,1,0),m_gridcolor,vector2df(0.0f, 0.0f)));
		Buffer.Vertices.push_back(S3DVertex(end,vector3df(0,1,0),m_gridcolor,vector2df(0.0f, 0.0f)));

		Buffer.Indices.push_back(indexIndex++);
		Buffer.Indices.push_back(indexIndex++);
	}

	//Z-axis lines
	for(u32 z = 0; z <= m_size; z+= m_spacing)
	{
		vector3df start = leftMost;
		start.Z += z ;

		vector3df end = rightMost;
		end.Z = start.Z;

		Buffer.Vertices.push_back(S3DVertex(start,vector3df(0,1,0), m_gridcolor,vector2df(0.0f, 0.0f)));
		Buffer.Vertices.push_back(S3DVertex(end,vector3df(0,1,0), m_gridcolor,vector2df(0.0f, 0.0f)));

		Buffer.Indices.push_back(indexIndex++);
		Buffer.Indices.push_back(indexIndex++);
	}

	//Accent lines are only drawn if the offset is greater than 0
	if(m_accentlineoffset > 0)
	{
		//X-axis
		for(u32 x = 0; x <= m_size; x+= m_spacing*m_accentlineoffset)
		{
			core::vector3df start = leftMost;
			start.X += x ;

			core::vector3df end = rightMost;
			end.X = start.X;

			Buffer.Vertices.push_back(S3DVertex(start,vector3df(0,1,0),m_accentgridcolor,vector2df(0.0f, 0.0f)));
			Buffer.Vertices.push_back(S3DVertex(end,vector3df(0,1,0),m_accentgridcolor,vector2df(0.0f, 0.0f)));

			Buffer.Indices.push_back(indexIndex++);
			Buffer.Indices.push_back(indexIndex++);
		}

		//Z-axis
		for(u32 z = 0; z <= m_size; z+= m_spacing*m_accentlineoffset)
		{
			vector3df start = leftMost;
			start.Z += z ;

			vector3df end = rightMost;
			end.Z = start.Z;

			Buffer.Vertices.push_back(S3DVertex(start,vector3df(0,1,0),m_accentgridcolor,vector2df(0.0f, 0.0f)));
			Buffer.Vertices.push_back(S3DVertex(end,vector3df(0,1,0),m_accentgridcolor,vector2df(0.0f, 0.0f)));

			Buffer.Indices.push_back(indexIndex++);
			Buffer.Indices.push_back(indexIndex++);
		}
	}


	// Create our box, it is the size of the grid exactly, plus 1 in the Y axis
	Buffer.BoundingBox = core::aabbox3df(-(f32)m_size/2,-0.5f,-(f32)m_size/2,(f32)m_size/2,0.5f,(f32)m_size/2);
}

void CGrid::render()
{
	//Prep to render
	pVideo->setMaterial(Buffer.Material);
	pVideo->setTransform(ETS_WORLD,AbsoluteTransformation);

	pVideo->drawVertexPrimitiveList(Buffer.getVertices(),Buffer.getVertexCount(),Buffer.getIndices(),Buffer.getVertexCount()/2, video::EVT_STANDARD, scene::EPT_LINES);

	// Axis Lines are only drawn if the State is true
	if(m_AxisLineState)
	{
		pVideo->draw3DLine(vector3df((f32)m_size,0,0),vector3df(-(f32)m_size,0,0),m_XLineColor);
		pVideo->draw3DLine(vector3df(0,0,(f32)m_size),vector3df(0,0,-(f32)m_size),m_ZLineColor);
	}
}

const aabbox3d<f32>& CGrid::getBoundingBox() const
{
	return Buffer.BoundingBox;
}

u32 CGrid::getMaterialCount()
{
	return 1;
}

SMaterial& CGrid::getMaterial(u32 i)
{
	return Buffer.Material;
}