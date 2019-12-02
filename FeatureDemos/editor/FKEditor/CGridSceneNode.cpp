#include "CGridSceneNode.h"

CGridSceneNode* CGridSceneNode::Clone(ISceneNode *newParent, ISceneManager *newSceneManager)
{
	if (!newParent) newParent = Parent;
	if (!newSceneManager) newSceneManager = SceneManager;

	CGridSceneNode* clone = new CGridSceneNode(
		Parent,
		SceneManager,
		ID,
		m_spacing,
		m_size*2,
		m_gridcolor,
		m_accentlineoffset,
		m_accentgridcolor,
		m_AxisLineState);

	clone->SetAxisLineXColor(m_XLineColor);
	clone->SetAxisLineZColor(m_ZLineColor);
	clone->SetMaterial(Material);

	clone->drop();
	return clone;
}

void CGridSceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);

	ISceneNode::OnRegisterSceneNode();
}

void CGridSceneNode::render()
{
	IVideoDriver* driver = SceneManager->getVideoDriver();

	// Set our left corner
	vector3df leftMost = vector3df(0,0,0);
	leftMost.X -= m_size;
	leftMost.Z -= m_size;

	// Set our right corner
	vector3df rightMost = vector3df(0,0,0);
	rightMost.X += m_size;
	rightMost.Z += m_size; 

	// Prep to render
	driver->setMaterial(Material);
	driver->setTransform(ETS_WORLD, AbsoluteTransformation);

	// X-axis lines
	for(u32 x = 0; x <= m_size*2; x+= m_spacing)
	{
		vector3df start = leftMost;
		start.X += x ;

		vector3df end = rightMost;
		end.X = start.X;

		// std::cout<<"Drawing Line From : "<<start.X<<","<<start.Y<<","<<start.Z<<" to "<<end.X<<","<<end.Y<<","<<end.Z<<"\n";
		driver->draw3DLine(start,end,m_gridcolor);
	}

	// Z-axis lines
	for(u32 z = 0; z <= m_size*2; z+= m_spacing)
	{
		vector3df start = leftMost;
		start.Z += z ;

		vector3df end = rightMost;
		end.Z = start.Z;

		driver->draw3DLine(start,end,m_gridcolor);
	}

	// Accent lines are only drawn if the offset is greater than 0
	if(m_accentlineoffset > 0)
	{
		// X-axis
		for(u32 x = 0; x <= m_size*2; x+= m_spacing*m_accentlineoffset)
		{
			vector3df start = leftMost;
			start.X += x ;

			vector3df end = rightMost;
			end.X = start.X;

			driver->draw3DLine(start,end,m_accentgridcolor);
		}

		// Z-axis
		for(u32 z = 0; z <= m_size*2; z+= m_spacing*m_accentlineoffset)
		{
			vector3df start = leftMost;
			start.Z += z ;

			vector3df end = rightMost;
			end.Z = start.Z;

			driver->draw3DLine(start,end,m_accentgridcolor);
		}
	}

	// Axis Lines are only drawn if the State is true
	if(m_AxisLineState)
	{
		driver->draw3DLine(vector3df((f32)m_size,0,0),vector3df(-(f32)m_size,0,0),m_XLineColor);
		driver->draw3DLine(vector3df(0,0,(f32)m_size),vector3df(0,0,-(f32)m_size),m_ZLineColor);
	}
}

const aabbox3d<f32>& CGridSceneNode::getBoundingBox() const
{
	return Box;
}

u32 CGridSceneNode::getMaterialCount()
{
	return 1;
}

SMaterial& CGridSceneNode::getMaterial(u32 i)
{
	return Material;
}

u32 CGridSceneNode::GetSpacing()
{
	return m_spacing;
}

u32 CGridSceneNode::GetSize()
{
	return m_size;
}

u32 CGridSceneNode::GetAccentlineOffset()
{
	return m_accentlineoffset;
}

SColor CGridSceneNode::GetAccentlineColor()
{
	return m_accentgridcolor;
}

SColor CGridSceneNode::GetGridColor()
{
	return m_gridcolor;
}

bool CGridSceneNode::AreAxisLineActive()
{
	return m_AxisLineState;
}

SColor CGridSceneNode::GetAxisLineXColor()
{
	return m_XLineColor;
}

SColor CGridSceneNode::GetAxisLineZColor()
{
	return m_ZLineColor;
}

void CGridSceneNode::SetSpacing(u32 newspacing)
{
	m_spacing = newspacing;
}

void CGridSceneNode::SetSize(u32 newsize)
{
	m_size = newsize;
}

void CGridSceneNode::SetAccentlineColor(SColor newcolor)
{
	m_accentgridcolor = newcolor;
}

void CGridSceneNode::SetAccentlineOffset(u32 newoffset)
{
	m_accentlineoffset = newoffset;
}

void CGridSceneNode::SetGridColor(SColor newcolor)
{
	m_gridcolor = newcolor;
}

void CGridSceneNode::SetAxisLineActive(bool active)
{
	m_AxisLineState = active;
}

void CGridSceneNode::SetAxisLineXColor(SColor XLine)
{
	m_XLineColor = XLine;
}

void CGridSceneNode::SetAxisLineZColor(SColor ZLine)
{
	m_ZLineColor = ZLine;
}

void CGridSceneNode::SetMaterial(SMaterial newMaterial)
{
	Material = newMaterial;
}
