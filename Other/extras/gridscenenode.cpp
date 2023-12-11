#include "CGridSceneNode.h"

using namespace irr;

CGridSceneNode::SGrid::SGrid()
    : IsVisible(true)
	, Alignment(CENTER)
	, Spacing(8.f)
	, Size(2048.f, 2048.f)
	, Offset(0.f, 0.f, 0.f)
	, GridColor( irr::video::SColor(255,128,128,128))
	, MaxRenderDist(FLT_MAX)
	, GridDirty(true)
	, BoundingBoxDirty(true)
	, MeshBuffer(0)
{
    // Set the material
	MeshBuffer = new irr::scene::CDynamicMeshBuffer(irr::video::EVT_STANDARD, irr::video::EIT_16BIT);
	setDefaultMaterial();
}

CGridSceneNode::SGrid::~SGrid()
{
	if ( MeshBuffer )
		MeshBuffer->drop();
}

CGridSceneNode::SGrid::SGrid(const SGrid& other)
: MeshBuffer(0)
{
	MeshBuffer = new irr::scene::CDynamicMeshBuffer(irr::video::EVT_STANDARD, irr::video::EIT_16BIT);
	*this = other;
}

CGridSceneNode::SGrid& CGridSceneNode::SGrid::operator=(const CGridSceneNode::SGrid& other)
{
	if (&other == this )
		return *this;

	IsVisible = other.IsVisible;
	Alignment = other.Alignment;
	Spacing = other.Spacing;
	Size = other.Size;
	Offset = other.Offset;
	GridColor = other.GridColor;
	MaxRenderDist = other.MaxRenderDist;
	setMaterial(other.getMaterial());
	// note, each object keeps it's own MeshBuffer for the lifetime and just changes the content
	GridDirty = true;

	return *this;
}

void CGridSceneNode::SGrid::setDefaultMaterial()
{
	if( !MeshBuffer )
		return;
    MeshBuffer->getMaterial().Wireframe = false;
    MeshBuffer->getMaterial().Lighting = false;
    MeshBuffer->getMaterial().Thickness = 1;
    MeshBuffer->getMaterial().FogEnable = false;
    MeshBuffer->getMaterial().ZWriteEnable = true;
	MeshBuffer->getMaterial().ZBuffer = true;
    MeshBuffer->getMaterial().BackfaceCulling = true;
    MeshBuffer->getMaterial().AntiAliasing = false;
}

bool CGridSceneNode::SGrid::canUseGridLine(irr::f32 pos, bool axisX, const CGridSceneNode* const gridNode)
{
	if ( !gridNode )
		return true;

	const irr::f32 epsilonHalf = 0.1f;
	for ( irr::u32 i=0; i<gridNode->getNumberOfGrids(); ++i )
	{
		const SGrid & otherGrid = gridNode->getGrid(i);
		if ( &otherGrid == this )
			continue;
		if ( otherGrid.getSpacing() <= getSpacing() )
			continue;	// we want the biggest spacing to show up most
		if ( !otherGrid.isVisible() )
			continue;

		irr::core::vector2df otherLeftTop, otherRightBottom;
		otherGrid.calcGridInside(otherLeftTop, otherRightBottom);
		irr::f32 dist = axisX ? irr::core::abs_(pos - otherLeftTop.X) : irr::core::abs_(pos - otherLeftTop.Y);
		irr::f32 mod = fmodf(dist+epsilonHalf, otherGrid.getSpacing());
		if ( irr::core::equals( mod, 0.f, 2*epsilonHalf ) )
			return false;
	}
	return true;
}


void CGridSceneNode::SGrid::calcGridInside(irr::core::vector2df& leftTop, irr::core::vector2df& rightBottom) const
{
    leftTop = irr::core::vector2df(0,0);
	rightBottom = leftTop;

    //Set our corners
	if ( Alignment == CENTER )
	{
		rightBottom.X = Spacing * (int)(0.5f*Size.Width/Spacing);
		rightBottom.Y = Spacing * (int)(0.5f*Size.Height/Spacing);
		leftTop.X = -rightBottom.X;
		leftTop.Y = -rightBottom.Y;
	}
	else
	{
		leftTop.X = -Size.Width/2.f;
		leftTop.Y = -Size.Height/2.f;
		rightBottom.X = leftTop.X + Spacing * (int)(Size.Width/Spacing);
		rightBottom.Y = leftTop.Y + Spacing * (int)(Size.Height/Spacing);
	}
	leftTop.X += Offset.X;
	leftTop.Y += Offset.Z;
	rightBottom.X += Offset.X;
	rightBottom.Y += Offset.Z;
}

void CGridSceneNode::SGrid::regenerateMesh(const CGridSceneNode* const gridNode)
{
	GridDirty = false;

    //Clean up memory
	MeshBuffer->getIndexBuffer().set_used(0);
	MeshBuffer->getVertexBuffer().set_used(0);
 
	u32 numVertices = ((u32)(Size.Width / Spacing) + 1) * 2 + ((u32)(Size.Height / Spacing) + 1) * 2;
    if ( numVertices > 65535)
    {
        //Too many vertices on 16 bit for for 16bit indices of SMeshBuffer
        //Returning with a blank buffer to avoid segfaulting the entire application
        return;
    }

	if ( Spacing <= 0 )
		return;
 
	// inner has the last outside borders, while outer has the full size
	core::vector2df innerLeftTop, innerRightBottom;
	calcGridInside(innerLeftTop, innerRightBottom);
	core::vector3df outerLeftTop(-Size.Width/2.f,0,-Size.Height/2.f);
    core::vector3df outerRightBottom(Size.Width/2.f,0,Size.Height/2.f);
	outerLeftTop += Offset;
	outerRightBottom += Offset;
 
    u32 indexIndex = 0;
 
    //lines parallel to X-axis 
	core::vector3df start = outerLeftTop;
	core::vector3df end = outerRightBottom;
	for (f32 x = 0.f; x <= (innerRightBottom.X - innerLeftTop.X); x+= Spacing)
    {
        start.X = x + innerLeftTop.X;

		if ( !canUseGridLine(start.X, true, gridNode) )
			continue;

        end.X = start.X;
 
        MeshBuffer->getVertexBuffer().push_back(video::S3DVertex(start, core::vector3df(0,1,0), GridColor, core::vector2df(0.0f, 0.0f)));
        MeshBuffer->getVertexBuffer().push_back(video::S3DVertex(end, core::vector3df(0,1,0), GridColor, core::vector2df(0.0f, 0.0f)));
 
		MeshBuffer->getIndexBuffer().push_back(indexIndex++);
        MeshBuffer->getIndexBuffer().push_back(indexIndex++);
    }
 
    //lines parallel to Z-axis
	start = outerLeftTop;
	end = outerRightBottom;
	for (f32 z = 0.f; z <= (innerRightBottom.Y - innerLeftTop.Y); z+= Spacing)
    {
        start.Z = z + innerLeftTop.Y;

		if ( !canUseGridLine(start.Z, false, gridNode) )
			continue;
 
        end.Z = start.Z;
 
        MeshBuffer->getVertexBuffer().push_back(video::S3DVertex(start, core::vector3df(0,1,0), GridColor, core::vector2df(0.0f, 0.0f)));
        MeshBuffer->getVertexBuffer().push_back(video::S3DVertex(end, core::vector3df(0,1,0), GridColor, core::vector2df(0.0f, 0.0f)));
 
        MeshBuffer->getIndexBuffer().push_back(indexIndex++);
        MeshBuffer->getIndexBuffer().push_back(indexIndex++);
    }
 
    // Create our box, it is the size of the grid exactly, plus 1 in the Y axis
	irr::core::aabbox3df bbox(-(f32)Size.Width/2.f,-0.5f,-(f32)Size.Height/2.f,(f32)Size.Width/2.f,0.5f,(f32)Size.Height/2.f);
	bbox.MinEdge += Offset;
	bbox.MaxEdge += Offset;
	MeshBuffer->setBoundingBox(bbox);

	BoundingBoxDirty = true;
}

void CGridSceneNode::SGrid::setVisible(bool visible)
{
	IsVisible = visible;
	GridDirty = true;	// grids must be recalculated because they don't have lines used by another visible grid
}

bool CGridSceneNode::SGrid::isVisible() const
{
	return IsVisible; 
}

void CGridSceneNode::SGrid::setAlignment(CGridSceneNode::EAlign align)
{
	Alignment = align;
	GridDirty = true;
}

CGridSceneNode::EAlign CGridSceneNode::SGrid::getAlignment() const
{
	return Alignment;
}

void CGridSceneNode::SGrid::setSpacing(f32 newspacing)
{
    Spacing = newspacing;
    GridDirty = true;
}

f32 CGridSceneNode::SGrid::getSpacing() const
{
    return Spacing;
}

void CGridSceneNode::SGrid::setSize(const irr::core::dimension2df& newsize)
{
	Size = newsize;
    GridDirty = true;
}
 
const irr::core::dimension2df& CGridSceneNode::SGrid::getSize() const
{
    return Size;
}

void CGridSceneNode::SGrid::setOffset(const irr::core::vector3df& offset)
{
	Offset = offset;
	GridDirty = true;
}

const irr::core::vector3df& CGridSceneNode::SGrid::getOffset() const
{
	return Offset;
}

void CGridSceneNode::SGrid::setGridColor(video::SColor newcolor)
{
    GridColor = newcolor;
    GridDirty = true;
}

video::SColor CGridSceneNode::SGrid::getGridColor() const
{
    return GridColor;
}

void CGridSceneNode::SGrid::setMaxRenderDistance(irr::f32 dist)
{
	MaxRenderDist = dist;
}

irr::f32 CGridSceneNode::SGrid::getMaxRenderDistance() const
{
	return MaxRenderDist;
}

void CGridSceneNode::SGrid::setMaterial(const video::SMaterial& newMaterial)
{
    MeshBuffer->getMaterial() = newMaterial;
}

const irr::video::SMaterial& CGridSceneNode::SGrid::getMaterial() const
{
	return MeshBuffer->getMaterial();
}

irr::video::SMaterial& CGridSceneNode::SGrid::getMaterial()
{
	return MeshBuffer->getMaterial();
}

void CGridSceneNode::SGrid::getGridLines(irr::core::array<irr::core::line2df> &lines, const irr::core::matrix4* transformation) const
{
	video::S3DVertex* vertices = MeshBuffer->getVertexBuffer().pointer();
	for ( u32 i=0; i < MeshBuffer->getVertexBuffer().size() / 2; ++i )
	{
		core::vector3df vs = vertices[i*2].Pos;
		core::vector3df ve = vertices[i*2+1].Pos;

		if ( transformation )
		{
			transformation->transformVect(vs);
			transformation->transformVect(ve);
		}

		lines.push_back( core::line2df( vs.X, vs.Z, ve.X, ve.Z) );
	}
}

const irr::core::aabbox3d<irr::f32>& CGridSceneNode::SGrid::getBoundingBox() const
{
	return MeshBuffer->getBoundingBox();
}

void CGridSceneNode::SGrid::render(irr::video::IVideoDriver * driver )
{
	if ( !isVisible() )
		return;

	driver->setMaterial(MeshBuffer->getMaterial());
 
	driver->drawVertexPrimitiveList(MeshBuffer->getVertexBuffer().getData(),	// const void* vertices
									MeshBuffer->getVertexBuffer().size(),		// u32 vertexCount
									MeshBuffer->getIndexBuffer().getData(),		// const void* indexList
									MeshBuffer->getVertexBuffer().size()/2,		// u32 primCount
									MeshBuffer->getVertexType(),				// E_VERTEX_TYPE vType
									scene::EPT_LINES,							// scene::E_PRIMITIVE_TYPE pType
									MeshBuffer->getIndexType() );				// E_INDEX_TYPE iType
}



// ----------------------------------------------------------------------------
CGridSceneNode::CGridSceneNode(ISceneNode* parent, scene::ISceneManager* smgr, s32 id, irr::u32 numGrids)
        : ISceneNode(parent, smgr, id), SelectionMesh(0), SelectionMeshBuffer(0), ShowSelectionOnce(false)
{
    // Set the default culling state to Frustum Box
    AutomaticCullingState = scene::EAC_FRUSTUM_BOX;

	for ( irr::u32 i=0; i<numGrids;++i )
	{
		Grids.push_back(SGrid());
	}

	SelectionMeshBuffer = new irr::scene::CDynamicMeshBuffer(irr::video::EVT_STANDARD, irr::video::EIT_16BIT);
	SelectionMesh = new irr::scene::SMesh();
	SelectionMesh->addMeshBuffer( SelectionMeshBuffer );
	SelectionMeshBuffer->getMaterial().Wireframe = false;
    SelectionMeshBuffer->getMaterial().Lighting = false;
    SelectionMeshBuffer->getMaterial().FogEnable = false;
    SelectionMeshBuffer->getMaterial().BackfaceCulling = false;
}

CGridSceneNode::~CGridSceneNode()
{
	if ( SelectionMesh )
		SelectionMesh->drop();
	if ( SelectionMeshBuffer )
		SelectionMeshBuffer->drop();
}
 
CGridSceneNode* CGridSceneNode::clone(scene::ISceneNode *newParent, scene::ISceneManager *newSceneManager)
{
    if (!newParent) newParent = Parent;
    if (!newSceneManager) newSceneManager = SceneManager;
 
    CGridSceneNode* clone = new CGridSceneNode(
        Parent,
        SceneManager,
        ID,
		Grids.size() );
    if (!clone)
        return 0;

	for ( irr::u32 i=0; i< Grids.size(); ++i )
	{
		SGrid & grid = clone->getGrid(i);
		grid = getGrid(i);
	}

    clone->drop();
    return clone;
}
 
void CGridSceneNode::OnRegisterSceneNode()
{
	if ( hasDirtyGrid() )
		regenerateMeshes();
	if( hasDirtyBoundingBox() )
	{
		rebuildBoundingBox();
		rebuildSelectionMesh();
	}

    if (IsVisible)
        SceneManager->registerNodeForRendering(this);
 
    ISceneNode::OnRegisterSceneNode();
}

void CGridSceneNode::setGridVisibilityByDistToCamera()
{
	for ( irr::u32 i=0; i<Grids.size(); ++i )
	{
		Grids[i].setVisible(true);

		if ( !SceneManager->getActiveCamera()->isOrthogonal() )
		{
			irr::core::plane3df gridPlane(0,Grids[i].getOffset().Y,0,0,1,0);
			AbsoluteTransformation.transformPlane(gridPlane);
			irr::f32 distToCam = gridPlane.getDistanceTo( SceneManager->getActiveCamera()->getAbsolutePosition() );
			if ( !SceneManager->getActiveCamera()->isOrthogonal() && distToCam > Grids[i].getMaxRenderDistance() )
			{
				Grids[i].setVisible(false);
			}
		}
	}
}

void CGridSceneNode::render()
{
    video::IVideoDriver* driver = SceneManager->getVideoDriver();
 
	if ( !driver )
		return;

//	setGridVisibilityByDistToCamera();

	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
	for ( irr::u32 i=0; i<Grids.size(); ++i )
	{
		Grids[i].render(driver);
	}

	if ( ShowSelectionOnce )
	{
		ShowSelectionOnce = false;
		driver->setMaterial(SelectionMeshBuffer->getMaterial());
 
		driver->drawVertexPrimitiveList(SelectionMeshBuffer->getVertexBuffer().getData(),	// const void* vertices
									SelectionMeshBuffer->getVertexBuffer().size(),		// u32 vertexCount
									SelectionMeshBuffer->getIndexBuffer().getData(),	// const void* indexList
									SelectionMeshBuffer->getIndexBuffer().size()/3,		// u32 primCount
									SelectionMeshBuffer->getVertexType(),				// E_VERTEX_TYPE vType
									scene::EPT_TRIANGLES,								// scene::E_PRIMITIVE_TYPE pType
									SelectionMeshBuffer->getIndexType() );				// E_INDEX_TYPE iType
	}
}

bool CGridSceneNode::hasDirtyBoundingBox() const
{
	bool hasDirty = false;
	for ( irr::u32 i=0; i<Grids.size(); ++i )
	{
		if ( Grids[i].BoundingBoxDirty )
		{
			hasDirty = true;
			break;
		}
	}
	return hasDirty;
}

bool CGridSceneNode::hasDirtyGrid() const
{
	bool hasDirty = false;
	for ( irr::u32 i=0; i<Grids.size(); ++i )
	{
		if ( Grids[i].GridDirty )
		{
			hasDirty = true;
			break;
		}
	}
	return hasDirty;
}

void CGridSceneNode::regenerateMeshes()
{
	for ( irr::u32 i=0; i<Grids.size(); ++i )
	{
		Grids[i].regenerateMesh(this);
	}
}

void CGridSceneNode::rebuildBoundingBox()
{
	if ( Grids.empty() )
		BoundingBox.reset( irr::core::vector3df(FLT_MAX,FLT_MAX,FLT_MAX) );
	else
	{
		// recalculate when one boundingbox has changed
		bool hasDirty = false;
		for ( irr::u32 i=0; i<Grids.size(); ++i )
		{
			if ( Grids[i].BoundingBoxDirty )
			{
				hasDirty = true;
				break;
			}
		}

		if ( hasDirty )
		{
			BoundingBox = Grids[0].getBoundingBox();
			Grids[0].BoundingBoxDirty = false;
			for ( irr::u32 i=1; i<Grids.size(); ++i )
			{
				BoundingBox.addInternalBox( Grids[i].getBoundingBox() );
				Grids[i].BoundingBoxDirty = false;
			}
		}
	}
}

void CGridSceneNode::rebuildSelectionMesh()
{
	SelectionMeshBuffer->getVertexBuffer().reallocate(4);
	SelectionMeshBuffer->getVertexBuffer().set_used(0);

	irr::video::SColor col(128, 0, 0, 255);
	irr::core::vector3df edges[8];
	BoundingBox.getEdges(edges);
	irr::video::S3DVertex* vertices = SelectionMeshBuffer->getVertexBuffer().pointer();
	for ( int i=0; i<4; ++i )
		SelectionMeshBuffer->getVertexBuffer().push_back ( video::S3DVertex((edges[i*2]+edges[i*2+1])/2, core::vector3df(0,1,0), col, core::vector2df(0.0f, 0.0f)) );

	if ( SelectionMeshBuffer->getIndexBuffer().size() < 6 )
	{
		SelectionMeshBuffer->getIndexBuffer().reallocate(6);
		SelectionMeshBuffer->getIndexBuffer().set_used(0);
		SelectionMeshBuffer->getIndexBuffer().push_back(0);
		SelectionMeshBuffer->getIndexBuffer().push_back(1);
		SelectionMeshBuffer->getIndexBuffer().push_back(2);
		SelectionMeshBuffer->getIndexBuffer().push_back(2);
		SelectionMeshBuffer->getIndexBuffer().push_back(1);
		SelectionMeshBuffer->getIndexBuffer().push_back(3);
	}

	SelectionMeshBuffer->setDirty();

	irr::scene::ITriangleSelector * s = SceneManager->createTriangleSelector(SelectionMesh, this);
	setTriangleSelector(s);
	s->drop();
}

const core::aabbox3d<f32>& CGridSceneNode::getBoundingBox() const
{
	return BoundingBox;
}
 
u32 CGridSceneNode::getMaterialCount() const
{
	return Grids.size();
}
 
video::SMaterial& CGridSceneNode::getMaterial(u32 i)
{
	return Grids[i].getMaterial();
}

irr::u32 CGridSceneNode::getNumberOfGrids() const
{
	return Grids.size();
}

irr::u32 CGridSceneNode::addGrid()
{
	Grids.push_back(SGrid());
	return Grids.size()-1;
}

void CGridSceneNode::removeGrid(irr::u32 index)
{
	Grids.erase(index, 1);
}

CGridSceneNode::SGrid& CGridSceneNode::getGrid(irr::u32 index)
{
	return Grids[index];
}

const CGridSceneNode::SGrid& CGridSceneNode::getGrid(irr::u32 index) const
{
	return Grids[index];
}

void CGridSceneNode::setGridsSize(const irr::core::dimension2df& newsize)
{
	GridsSize = newsize;
	for ( irr::u32 i=0; i < Grids.size(); ++i )
	{
		Grids[i].setSize(newsize);
	}
}

const irr::core::dimension2df& CGridSceneNode::getGridsSize() const
{
	return GridsSize;
}
