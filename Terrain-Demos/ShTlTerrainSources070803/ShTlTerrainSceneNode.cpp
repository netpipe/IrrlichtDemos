#include "ShTlTerrainSceneNode.h"

// constructor
ShTlTerrainSceneNode::ShTlTerrainSceneNode(scene::ISceneManager* pSceneManager,
    s32 width, s32 height, f32 tilesize, s32 visiblesize,
    scene::ISceneNode* parent, s32 id)
    : scene::ISceneNode(pSceneManager->getRootSceneNode(), pSceneManager, id)
{
    Size.Width = width;
    Size.Height = height;

    TileSize = tilesize;

    // make sure rendered terrain mesh is not larger than terrain itself
    if(visiblesize > Size.Width) visiblesize = Size.Width;
    if(visiblesize > Size.Height) visiblesize = Size.Height;

    MeshSize.Width = visiblesize;
    MeshSize.Height = visiblesize;

    MeshPosition = core::vector2d<s32>(0,0);

    Fnode = NULL;

    ShStep = 1;

    // create data array

    Data.reset(Size.Width+1, Size.Height+1);

    for(s32 j=0; j<Size.Height+1; j++)
        for(s32 i=0; i<Size.Width+1; i++)
        {
            Data(i,j).Height = 0;
            Data(i,j).Normal = core::vector3df(0,1,0);
            Data(i,j).Color = video::SColor(255,255,255,255);
        }

    // create tile UV data array

    UVdata.reset(Size.Width, Size.Height);

    for(s32 j=0; j<Size.Height; j++)
        for(s32 i=0; i<Size.Width; i++)
        {
            UVdata(i,j).Vertex[0] = core::vector2d<f32>(0,1);
            UVdata(i,j).Vertex[1] = core::vector2d<f32>(0,0);
            UVdata(i,j).Vertex[2] = core::vector2d<f32>(1,0);
            UVdata(i,j).Vertex[3] = core::vector2d<f32>(1,1);
        }

    // calculate number of sectors
    // terrain mesh will be split to 1 or 3 or 5 sectors on each axis depending on size
    s32 w = 1;
    s32 h = 1;
    if(MeshSize.Width >= 30) w = 3;
    if(MeshSize.Height >= 30) h = 3;
    if(MeshSize.Width >= 50) w = 5;
    if(MeshSize.Height >= 50) h = 5;

    // create sectors
    Sector.reset(w, h);

    // find size of sectors in tiles
    w = MeshSize.Width / Sector.width();
    h = MeshSize.Height / Sector.height();

    for(s32 j=0; j<Sector.height(); j++)
        for(s32 i=0; i<Sector.width(); i++)
            Sector(i,j).Size = core::dimension2d<u32>(w, h);

    // find size of center sector in tiles
    w = MeshSize.Width - Sector(0,0).Size.Width * (Sector.width()-1);
    h = MeshSize.Height - Sector(0,0).Size.Height * (Sector.height()-1);

    {s32 j= Sector.height()/2;
        for(s32 i=0; i<Sector.width(); i++) Sector(i,j).Size.Height = h;}

    {s32 i= Sector.width()/2;
        for(s32 j=0; j<Sector.height(); j++) Sector(i,j).Size.Width = w;}

    // find offset of sectors in tiles
    for(s32 j=0; j<Sector.height(); j++)
        for(s32 i=0; i<Sector.width(); i++)
        {
            if(i>0) Sector(i,j).Offset.X = Sector(i-1,j).Offset.X + Sector(i-1,j).Size.Width;
            else Sector(i,j).Offset.X = 0;
            if(j>0) Sector(i,j).Offset.Y = Sector(i,j-1).Offset.Y + Sector(i,j-1).Size.Height;
            else Sector(i,j).Offset.Y = 0;
        }

    // fill sectors with tiles
    for(s32 j=0; j<Sector.height(); j++)
        for(s32 i=0; i<Sector.width(); i++)
        {
            Sector(i,j).Vertex.set_used( Sector(i,j).Size.Width * Sector(i,j).Size.Height * 4);
            Sector(i,j).Index.set_used( Sector(i,j).Size.Width * Sector(i,j).Size.Height * 6);

            u32 n = 0;
            u32 m = 0;
            while(n < Sector(i,j).Index.size())
            {
                Sector(i,j).Index[n] = m;
                n++;
                Sector(i,j).Index[n] = m+1;
                n++;
                Sector(i,j).Index[n] = m+2;
                n++;

                Sector(i,j).Index[n] = m;
                n++;
                Sector(i,j).Index[n] = m+2;
                n++;
                Sector(i,j).Index[n] = m+3;
                n++;
                m+=4;
            }
        }

    // setup tiles
    Tile.reset(MeshSize.Width, MeshSize.Height);
    for(s32 j=0; j<Sector.height(); j++)
        for(s32 i=0; i<Sector.width(); i++)
        {
            u32 k = 0;

            for(u32 n=0; n<Sector(i,j).Size.Height; n++)
                for(u32 m=0; m<Sector(i,j).Size.Width; m++)
                {
                    u32 x = m + Sector(i,j).Offset.X;
                    u32 y = n + Sector(i,j).Offset.Y;

                    Tile(x,y).Vertex[0] = &Sector(i,j).Vertex[k];
                    k++;
                    Tile(x,y).Vertex[1] = &Sector(i,j).Vertex[k];
                    k++;
                    Tile(x,y).Vertex[2] = &Sector(i,j).Vertex[k];
                    k++;
                    Tile(x,y).Vertex[3] = &Sector(i,j).Vertex[k];
                    k++;
                }
        }

    // set color of mesh vertices to white
    // vertex color is not used, 2nd texture layer is used to render tile colors
    for(s32 j=0; j<MeshSize.Height; j++)
        for(s32 i=0; i<MeshSize.Width; i++)
        {
            Tile(i,j).Vertex[0]->Color = video::SColor(255,255,255,255);
            Tile(i,j).Vertex[1]->Color = video::SColor(255,255,255,255);
            Tile(i,j).Vertex[2]->Color = video::SColor(255,255,255,255);
            Tile(i,j).Vertex[3]->Color = video::SColor(255,255,255,255);
        }

    // setup material
    video::SMaterial material;
    material.AmbientColor = video::SColor(255,255,255,255);
    material.DiffuseColor = video::SColor(255,255,255,255);
    material.EmissiveColor = video::SColor(255,0,0,0);
    material.Shininess = 0.0f;
    material.SpecularColor = video::SColor(255,0,0,0);
    material.MaterialType = video::EMT_DETAIL_MAP;

    Material.push_back(material);

    // create 2nd texture layer

    // find size of texture, must be power of two
    s32 tw = 2;
    while(tw < MeshSize.Width) tw = tw + tw;
    s32 th = 2;
    while(th < MeshSize.Height) th = th + th;

    // create texture
    // turn of mipmaps othervise they would need to be regenerated each time texture is updated
    video::IVideoDriver* driver = SceneManager->getVideoDriver();

    bool mmflag = driver->getTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS);
    driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);

    CTexture = driver->addTexture(core::dimension2d<s32>(tw, th), "colortexture", video::ECF_A8R8G8B8);

    driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, mmflag);

    Material[0].TextureLayer[1].Texture = CTexture;
    Material[0].TextureLayer[1].TextureWrap = video::ETC_CLAMP_TO_EDGE;

    // setup UV coordinates of vertices on 2nd texture layer
    f32 ax = (f32)MeshSize.Width / CTexture->getSize().Width / MeshSize.Width;
    f32 ay = (f32)MeshSize.Height/ CTexture->getSize().Height / MeshSize.Height;
    f32 ry = 1.0f - (f32)MeshSize.Height/ CTexture->getSize().Height;

    u32 n = MeshSize.Height-1;
    for(s32 j=0; j<MeshSize.Height; j++)
    {
        for(s32 i=0; i<MeshSize.Width; i++)
        {
            Tile(i,n).Vertex[0]->TCoords2 = core::vector2d<f32>(i*ax, ry+(j+1)*ay);
            Tile(i,n).Vertex[1]->TCoords2 = core::vector2d<f32>(i*ax, ry+j*ay);
            Tile(i,n).Vertex[3]->TCoords2 = core::vector2d<f32>((i+1)*ax, ry+(j+1)*ay);
            Tile(i,n).Vertex[2]->TCoords2 = core::vector2d<f32>((i+1)*ax, ry+j*ay);
        }
        n--;
    }

    // set update vertices flag for sectors
    for(s32 j=0; j<Sector.height(); j++)
        for(s32 i=0; i<Sector.width(); i++) Sector(i,j).UpdateVertices = true;

    // calculate bounding boxes
    BoundingBox.MinEdge = core::vector3df(0,0,0);
    BoundingBox.MaxEdge = core::vector3df(MeshSize.Width*TileSize,0,MeshSize.Height*TileSize);

    for(s32 j=0; j<Sector.height(); j++)
        for(s32 i=0; i<Sector.width(); i++)
        {
            Sector(i,j).BoundingBox.MinEdge =
                core::vector3df(Sector(i,j).Offset.X*TileSize,0,Sector(i,j).Offset.Y*TileSize);

            Sector(i,j).BoundingBox.MaxEdge = core::vector3df(
                (Sector(i,j).Offset.X + Sector(i,j).Size.Width) * TileSize, 0,
                (Sector(i,j).Offset.Y + Sector(i,j).Size.Height) * TileSize);
        }

    // set update texture flag for sectors
    for(s32 j=0; j<Sector.height(); j++)
        for(s32 i=0; i<Sector.width(); i++) Sector(i,j).UpdateTexture = true;


    // turn off automatic culling
    // culling will be done by terrain itself, sector by sector so that sectors
    // which are not on screen will not be renderd to save time
    setAutomaticCulling( scene::EAC_OFF );

}



// destructor
ShTlTerrainSceneNode::~ShTlTerrainSceneNode(){}



// frame
void ShTlTerrainSceneNode::OnRegisterSceneNode()
{
    if (IsVisible)
        SceneManager->registerNodeForRendering(this);

    ISceneNode::OnRegisterSceneNode();
}



// renders terrain
void ShTlTerrainSceneNode::render()
{
    // update position if needed
    if(Fnode) centerAt(Fnode->getPosition());

    video::IVideoDriver* driver = SceneManager->getVideoDriver();
    if (!driver) return;

    driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

    driver->setMaterial(Material[0]);

    SectorsRendered = 0;

    // test if sectors are vissible
    for(s32 j=0; j<Sector.height(); j++)
        for(s32 i=0; i<Sector.width(); i++)
            if( isSectorOnScreen( &Sector(i,j) ) ) Sector(i,j).isVissible = true;
            else Sector(i,j).isVissible = false;

    // update texture if needed
    u32* p = NULL;
    for(s32 j=0; j<Sector.height(); j++)
        for(s32 i=0; i<Sector.width(); i++)
            if(Sector(i,j).isVissible)
                if(Sector(i,j).UpdateTexture)
                {
                    if(!p) p = (u32*)CTexture->lock();

                    updateTexture(p, Sector(i,j));

                    Sector(i,j).UpdateTexture = false;
                }
    if(p) CTexture->unlock();

    // render geometry
    for(s32 j=0; j<Sector.height(); j++)
        for(s32 i=0; i<Sector.width(); i++)
            if(Sector(i,j).isVissible)
            {
                SectorsRendered++;

                // update vertices if needed
                if(Sector(i,j).UpdateVertices)
                {
                    updateVertices(Sector(i,j));
                    Sector(i,j).UpdateVertices = false;
                }

                driver->drawIndexedTriangleList
                    (&Sector(i,j).Vertex[0], Sector(i,j).Vertex.size(),
                    &Sector(i,j).Index[0], Sector(i,j).Index.size()/3);
            }

    // for debuging
    if( DebugDataVisible == scene::EDS_OFF ) return;

    // wireframe overlay
    if (DebugDataVisible == scene::EDS_MESH_WIRE_OVERLAY || DebugDataVisible == scene::EDS_FULL)
	{
        video::SMaterial m;
		m.Lighting = false;
		m.Wireframe = true;
        m.BackfaceCulling = false;
        m.AmbientColor = video::SColor(255,0,255,0);
        m.DiffuseColor = video::SColor(255,0,255,0);
		driver->setMaterial(m);

		for(s32 j=0; j<Sector.height(); j++)
            for(s32 i=0; i<Sector.width(); i++)
            {
                if( Sector(i,j).isVissible )
                {
                    driver->drawIndexedTriangleList
                        (&Sector(i,j).Vertex[0], Sector(i,j).Vertex.size(),
                        &Sector(i,j).Index[0], Sector(i,j).Index.size()/3);
                }
            }
    }

    // terrain bounding box
    if (DebugDataVisible == scene::EDS_BBOX || DebugDataVisible == scene::EDS_FULL)
	{
		video::SMaterial m;
		m.Lighting = false;
		driver->setMaterial(m);

		driver->draw3DBox(BoundingBox, video::SColor(0,255,255,255));
    }

    // sectors bounding boxes
	if (DebugDataVisible == scene::EDS_BBOX_BUFFERS || DebugDataVisible == scene::EDS_FULL)
	{
		video::SMaterial m;
		m.Lighting = false;
		driver->setMaterial(m);

		for(s32 j=0; j<Sector.height(); j++)
            for(s32 i=0; i<Sector.width(); i++)
            {
                driver->draw3DBox(Sector(i,j).BoundingBox, video::SColor(0,255,255,255));
            }
    }

	// normals
	if (DebugDataVisible == scene::EDS_NORMALS || DebugDataVisible == scene::EDS_FULL)
	{
        video::SMaterial m;
		m.Lighting = false;
		driver->setMaterial(m);

        for(s32 j=0; j<Sector.height(); j++)
            for(s32 i=0; i<Sector.width(); i++)
            {
                video::S3DVertex2TCoords* v = &Sector(i,j).Vertex[0];
                video::SColor c ( 255, 128 ,0, 0 );
                    for (u32 n = 0; n<Sector(i,j).Vertex.size(); ++n )
                    {
                        core::vector3df h = v[n].Normal * (TileSize/2);
				        driver->draw3DLine ( v[n].Pos, v[n].Pos + h, c );
                    }
            }
	}
}



// returns the axis aligned bounding box of terrain
const core::aabbox3d<f32>& ShTlTerrainSceneNode::getBoundingBox() const
{
    return BoundingBox;
}



// recalculate terrain bounding box
void ShTlTerrainSceneNode::recalculateBoundingBox()
{
    BoundingBox.MinEdge.X = TileSize * MeshPosition.X;
    BoundingBox.MinEdge.Y = Data(0,0).Height;
    BoundingBox.MinEdge.Z = TileSize * MeshPosition.Y;

    BoundingBox.MaxEdge.X = TileSize * (MeshPosition.X + MeshSize.Width);
    BoundingBox.MaxEdge.Y = Data(0,0).Height;
    BoundingBox.MaxEdge.Z = TileSize * (MeshPosition.Y + MeshSize.Height);

    for(s32 j=0; j<Size.Height+1; j++)
        for(s32 i=0; i<Size.Width+1; i++)
        {
            if(BoundingBox.MinEdge.Y > Data(i,j).Height) BoundingBox.MinEdge.Y = Data(i,j).Height;
            if(BoundingBox.MaxEdge.Y < Data(i,j).Height) BoundingBox.MaxEdge.Y = Data(i,j).Height;
        }


    for(s32 j=0; j<Sector.height(); j++)
        for(s32 i=0; i<Sector.width(); i++)
        {
            Sector(i,j).BoundingBox.MinEdge = core::vector3df(
                                            TileSize * Sector(i,j).Offset.X,
                                            BoundingBox.MinEdge.Y ,
                                            TileSize * Sector(i,j).Offset.Y);

            Sector(i,j).BoundingBox.MaxEdge = core::vector3df(
                    TileSize * (Sector(i,j).Offset.X + Sector(i,j).Size.Width),
                    BoundingBox.MaxEdge.Y,
                    TileSize * (Sector(i,j).Offset.Y + Sector(i,j).Size.Height));
        }
}



// returns amount of materials used by terrain
u32 ShTlTerrainSceneNode::getMaterialCount()
{
    return Material.size();
}



// returns the material of terrain
video::SMaterial& ShTlTerrainSceneNode::getMaterial(u32 i)
{
    return Material[i];
}



// return number of tiles to skip before terrain mesh gets updated
s32 ShTlTerrainSceneNode::getStep()
{
    return ShStep;
}



// set number of tiles to skip before terrain mesh gets updated
// default is 1
// updating slows down rendering and seting step higher will cause terrain to update less ofthen
void ShTlTerrainSceneNode::setStep(u32 newstep)
{
    ShStep = newstep;
}



// return dimensions of whole terrain in tiles
core::dimension2d<s32> ShTlTerrainSceneNode::getSize()
{
    return Size;
}



// return dimension of terrain tile
f32 ShTlTerrainSceneNode::getTileSize()
{
    return TileSize;
}



// set new dimensions of terrain tiles
void ShTlTerrainSceneNode::setTileSize(f32 newsize)
{
    if(newsize != TileSize)
    {
        TileSize = newsize;

        update();
    }
}



// returns dimension of rendered mesh in tiles
core::dimension2d<s32> ShTlTerrainSceneNode::getRenderedSize()
{
    return MeshSize;
}



// return number of sectors
u32 ShTlTerrainSceneNode::getSectorCount()
{
    return Sector.width() * Sector.height();
}



// returns numner of sectors rendered last frame
u32 ShTlTerrainSceneNode::getSectorsRendered()
{
    return SectorsRendered;
}



// return height of terrain spot at terrain coordinates
f32 ShTlTerrainSceneNode::getHeight(u32 w, u32 h)
{
    return Data(w,h).Height;
}



// return height of terrain at any position
f32 ShTlTerrainSceneNode::getHeight(core::vector3df pos)
{
    // find position relative to terrain
    pos = pos - getPosition();

    if(pos.X>=0 && pos.X<(f32)Size.Width*TileSize && pos.Z>=0 && pos.Z<(f32)Size.Height*TileSize)
    {
        // calculatin coordinates of tile
        s32 x = (s32) (pos.X / TileSize);
        s32 y = (s32) (pos.Z / TileSize);

        // calculating position relative to tile
        f32 xtil = pos.X - (x*TileSize);
        f32 ytil = pos.Z - (y*TileSize);

        // finding trianle of tile
        core::triangle3df triangle;
        if(xtil<=ytil)
        {
            // upper left triangle of tile
            triangle.pointA = core::vector3df(0, getHeight(x,y), 0);
            triangle.pointB = core::vector3df(0, getHeight(x,y+1), TileSize);
            triangle.pointC = core::vector3df(TileSize, getHeight(x+1,y+1), TileSize);
        }
        else
        {
            // lower right triangle of tile
            triangle.pointA = core::vector3df(0, getHeight(x,y), 0);
            triangle.pointB = core::vector3df(TileSize, getHeight(x+1,y+1), TileSize);
            triangle.pointC = core::vector3df(TileSize, getHeight(x+1,y), 0);
        }

        // calculating intersection with triangle
        core::vector3df intersection;
        triangle.getIntersectionWithLine(core::vector3df(xtil,0,ytil),core::vector3df(0,1,0),intersection);

        return intersection.Y + getPosition().Y;
    }
    else
        return 0;
}



// set relative height of terrain spot at terrain coordinates
void ShTlTerrainSceneNode::setHeight(u32 w, u32 h, f32 newheight)
{
    Data(w,h).Height = newheight;

    // recalculate bounding boxes

    if(newheight > BoundingBox.MaxEdge.Y)
    {
        BoundingBox.MaxEdge.Y = newheight;

        for(s32 j=0; j<Sector.height(); j++)
            for(s32 i=0; i<Sector.width(); i++)
                Sector(i,j).BoundingBox.MaxEdge.Y = newheight;
    }

    if(newheight < BoundingBox.MinEdge.Y)
    {
        BoundingBox.MinEdge.Y = newheight;

        for(s32 j=0; j<Sector.height(); j++)
            for(s32 i=0; i<Sector.width(); i++)
                Sector(i,j).BoundingBox.MinEdge.Y = newheight;
    }
}



// return normal of terrain at terrain coordinates
core::vector3df ShTlTerrainSceneNode::getNormal(u32 w, u32 h)
{
    return Data(w,h).Normal;
}

// set normal of terrain at terrain coordinates
void ShTlTerrainSceneNode::setNormal(s32 w, s32 h, core::vector3df newnormal)
{
    Data(w,h).Normal = newnormal;
}



// recalculate normal at terrain coordinates
void ShTlTerrainSceneNode::recalculateNormal(s32 w, s32 h)
{
    core::vector3df v0, v1;
    core::vector3df n0, n1, n2, n3, n4, n5;

    // calculate vector to point 0,-1
    if(h > 0) // check if not out of array
        v0 = core::vector3df(0, getHeight(w,h-1)-getHeight(w,h), -TileSize);
    else
        v0 = core::vector3df(0, 0, -TileSize);
    // calculate vector to point -1,-1
    if(w > 0 && h > 0) // check if not out of array
        v1 = core::vector3df(-TileSize, getHeight(w-1,h-1)-getHeight(w,h), -TileSize);
    else
        v1 = core::vector3df(-TileSize, 0, -TileSize);
    n0 = v0.crossProduct(v1);

    // calculate vector to point -1,-1
    v0 = v1;
    // calculate vector to point -1,0
    if(w > 0)
        v1 = core::vector3df(-TileSize, getHeight(w-1,h)-getHeight(w,h), 0);
    else
        v1 = core::vector3df(-TileSize, 0, 0);
    n1 = v0.crossProduct(v1);

    // calculate vector to point -1,0
    v0 = v1;
    // calculate vector to point 0,1
    if(h < Size.Height)
        v1 = core::vector3df(0, getHeight(w,h+1)-getHeight(w,h), TileSize);
    else
        v1 = core::vector3df(0, 0, TileSize);
    n2 = v0.crossProduct(v1);

    // calculate vector to point 0,1
    v0 = v1;
    // calculate vector to point 1,1
    if(w < Size.Width && h < Size.Height)
        v1 = core::vector3df(TileSize, getHeight(w+1,h+1)-getHeight(w,h), TileSize);
    else
        v1 = core::vector3df(TileSize, 0, TileSize);
    n3 = v0.crossProduct(v1);

    // calculate vector to point 1,1
    v0 = v1;
    // calculate vector to point 1,0
    if(w < Size.Width)
        v1 = core::vector3df(TileSize, getHeight(w+1,h)-getHeight(w,h), 0);
    else
        v1 = core::vector3df(TileSize, 0, 0);
    n4 = v0.crossProduct(v1);

    // calculate vector to point 1,0
    v0 = v1;
    // calculate vector to point 0,-1
    if(h > 0)
        v1 = core::vector3df(0, getHeight(w,h-1)-getHeight(w,h), -TileSize);
    else
        v1 = core::vector3df(0, 0, -TileSize);
    n5 = v0.crossProduct(v1);

    // calculate normals of 4 tiles around point
    core::vector3df m0, m1, m2, m3;
    m0 = (n1 - n0) /2 + n0;
    m1 = n2;
    m2 = (n4 - n3) /2 + n3;
    m3 = n5;

    // calculate normals between oposing tiles
    core::vector3df k0, k1;
    k0 = (m2 - m0) /2 + m0;
    k1 = (m3 - m1) /2 + m1;

    // calculate normal of point
    core::vector3df n = (k1 - k0) /2 + k0;
    n.normalize();
    setNormal(w,h,n);
}

// recalculare normals of whole terrain making it look smooth under light
void ShTlTerrainSceneNode::smoothNormals()
{
    for(s32 j=0; j<Size.Height+1; j++)
        for(s32 i=0; i<Size.Width+1; i++) recalculateNormal(i, j);
}



// get texture coordinates of tile corner
core::vector2d<f32> ShTlTerrainSceneNode::getTileUV(u32 w, u32 h, TILE_VERTEX corner)
{
    return UVdata(w,h).Vertex[corner];
}



// set texture coordinates of tile
void ShTlTerrainSceneNode::setTileUV(u32 w, u32 h, core::vector2d<f32> UVlowerLeft,
    core::vector2d<f32> UVupperLeft, core::vector2d<f32> UVupperRight,
    core::vector2d<f32> UVlowerRight)
{
    UVdata(w,h).Vertex[0] = UVlowerLeft;
    UVdata(w,h).Vertex[1] = UVupperLeft;
    UVdata(w,h).Vertex[2] = UVupperRight;
    UVdata(w,h).Vertex[3] = UVlowerRight;
}



// stretch texture over whole terrain
void ShTlTerrainSceneNode::stretchTexture(core::vector2d<f32> scale)
{
    f32 ax = scale.X / Size.Width;
    f32 ay = scale.X / Size.Height;

    u32 n = Size.Height-1;
    for(s32 j=0; j<Size.Height; j++)
    {
        for(s32 i=0; i<Size.Width; i++)
        {
            UVdata(i,n).Vertex[1] = core::vector2d<f32>(i*ax, j*ay);
            UVdata(i,n).Vertex[0] = core::vector2d<f32>(i*ax, j*ay+ay);
            UVdata(i,n).Vertex[3] = core::vector2d<f32>(i*ax+ax, j*ay+ay);
            UVdata(i,n).Vertex[2] = core::vector2d<f32>(i*ax+ax, j*ay);
        }
        n--;
    }
}



// stretch texture over every tile individualy
void ShTlTerrainSceneNode::stretchTextureOverTile(core::vector2d<f32> scale)
{
    for(s32 j=0; j<Size.Height; j++)
        for(s32 i=0; i<Size.Width; i++)
        {
            UVdata(i,j).Vertex[1] = core::vector2d<f32>(0, 0);
            UVdata(i,j).Vertex[0] = core::vector2d<f32>(0, scale.Y);
            UVdata(i,j).Vertex[3] = core::vector2d<f32>(scale.X, scale.Y);
            UVdata(i,j).Vertex[2] = core::vector2d<f32>(scale.X, 0);
        }
}



// rotate texture of tile 90 degrees
void ShTlTerrainSceneNode::rotateTileTexture90(u32 w, u32 h)
{
    core::vector2d<f32> tmp = UVdata(w,h).Vertex[3];

    UVdata(w,h).Vertex[3] = UVdata(w,h).Vertex[2];
    UVdata(w,h).Vertex[2] = UVdata(w,h).Vertex[1];
    UVdata(w,h).Vertex[1] = UVdata(w,h).Vertex[0];
    UVdata(w,h).Vertex[0] = tmp;
}



// rotate texture of tile 180 degrees
void ShTlTerrainSceneNode::rotateTileTexture180(u32 w, u32 h)
{
    core::vector2d<f32> tmp = UVdata(w,h).Vertex[3];

    UVdata(w,h).Vertex[3] = UVdata(w,h).Vertex[1];
    UVdata(w,h).Vertex[1] = tmp;

    tmp = UVdata(w,h).Vertex[2];

    UVdata(w,h).Vertex[2] = UVdata(w,h).Vertex[0];
    UVdata(w,h).Vertex[0] = tmp;
}



// rotate texture of tile 270 degrees
void ShTlTerrainSceneNode::rotateTileTexture270(u32 w, u32 h)
{
    core::vector2d<f32> tmp = UVdata(w,h).Vertex[3];

    UVdata(w,h).Vertex[3] = UVdata(w,h).Vertex[0];
    UVdata(w,h).Vertex[0] = UVdata(w,h).Vertex[1];
    UVdata(w,h).Vertex[1] = UVdata(w,h).Vertex[2];
    UVdata(w,h).Vertex[2] = tmp;
}



// flip (mirror) texture of tile horizontaly
void ShTlTerrainSceneNode::flipTileTextureHorizontal(u32 w, u32 h)
{
    core::vector2d<f32> tmp = UVdata(w,h).Vertex[3];

    UVdata(w,h).Vertex[3] = UVdata(w,h).Vertex[0];
    UVdata(w,h).Vertex[0] = tmp;

    tmp = UVdata(w,h).Vertex[2];

    UVdata(w,h).Vertex[2] = UVdata(w,h).Vertex[1];
    UVdata(w,h).Vertex[1] = tmp;
}



// flip (mirror) texture of tile verticaly
void ShTlTerrainSceneNode::flipTileTextureVertical(u32 w, u32 h)
{
    core::vector2d<f32> tmp = UVdata(w,h).Vertex[3];

    UVdata(w,h).Vertex[3] = UVdata(w,h).Vertex[2];
    UVdata(w,h).Vertex[2] = tmp;

    tmp = UVdata(w,h).Vertex[0];

    UVdata(w,h).Vertex[0] = UVdata(w,h).Vertex[1];
    UVdata(w,h).Vertex[1] = tmp;
}



// get color of tile at terrain coordinates
video::SColor ShTlTerrainSceneNode::getColor(u32 w, u32 h)
{
    return Data(w,h).Color;
}



// set color of tile at terrain coordinates
void ShTlTerrainSceneNode::setColor(u32 w, u32 h, video::SColor newcolor)
{
    Data(w,h).Color = newcolor;
}



// set rendered mesh position relative to terrain
void ShTlTerrainSceneNode::setMeshPosition(core::vector2d<s32> pos)
{
    // correct if out of terrain bounds
    if(pos.X < 0) pos.X = 0;
    if(pos.Y < 0) pos.Y = 0;

    if(pos.X > (s32)(Size.Width - MeshSize.Width)) pos.X = (s32)(Size.Width - MeshSize.Width);
    if(pos.Y > (s32)(Size.Height - MeshSize.Height)) pos.Y = (s32)(Size.Height - MeshSize.Height);

    // update
    if(pos.X > MeshPosition.X + ShStep || pos.X < MeshPosition.X - ShStep ||
       pos.Y > MeshPosition.Y + ShStep || pos.Y < MeshPosition.Y - ShStep)
    {
        MeshPosition = pos;

        update();
    }
}



// center rendered mesh at 3d coordinates
void ShTlTerrainSceneNode::centerAt(core::vector3d<f32> pos)
{
    // find position relative to terrain
    pos.X = pos.X - getPosition().X;
    pos.Z = pos.Z - getPosition().Z;

    // offset center
    pos.X = pos.X - TileSize * MeshSize.Width/2;
    pos.Z = pos.Z - TileSize * MeshSize.Height/2;

    // find equivalent in tiles
    s32 x = (s32)(pos.X / TileSize);
    s32 y = (s32)(pos.Z / TileSize);

    setMeshPosition(core::vector2d<s32>(x,y));
}



// update rendered mesh
void ShTlTerrainSceneNode::update()
{
    // update position of bounding boxes
    BoundingBox.MinEdge = core::vector3df(MeshPosition.X*TileSize, BoundingBox.MinEdge.Y, MeshPosition.Y*TileSize);
    BoundingBox.MaxEdge = core::vector3df( (MeshPosition.X+MeshSize.Width)*TileSize, BoundingBox.MaxEdge.Y, (MeshPosition.Y+MeshSize.Height)*TileSize);

    for(s32 j=0; j<Sector.height(); j++)
        for(s32 i=0; i<Sector.width(); i++)
        {
            Sector(i,j).BoundingBox.MinEdge = core::vector3df(
                (MeshPosition.X + Sector(i,j).Offset.X) * TileSize,
                Sector(i,j).BoundingBox.MinEdge.Y,
                (MeshPosition.Y + Sector(i,j).Offset.Y) * TileSize);

            Sector(i,j).BoundingBox.MaxEdge = core::vector3df(
                (MeshPosition.X + Sector(i,j).Offset.X + Sector(i,j).Size.Width) * TileSize,
                Sector(i,j).BoundingBox.MaxEdge.Y,
                (MeshPosition.Y + Sector(i,j).Offset.Y + Sector(i,j).Size.Height) * TileSize);
        }

    // set update texture flag for sectors
    for(s32 j=0; j<Sector.height(); j++)
        for(s32 i=0; i<Sector.width(); i++) Sector(i,j).UpdateTexture = true;

    // set update vertices flag for sectors
    for(s32 j=0; j<Sector.height(); j++)
        for(s32 i=0; i<Sector.width(); i++) Sector(i,j).UpdateVertices = true;
}



// set scene node terrain should be automaticly rendered arround
// can be camera or player node for example
void ShTlTerrainSceneNode::follow(scene::ISceneNode* node)
{
    Fnode = node;
}



// cancel pervious function
// stop following scene node if any
void ShTlTerrainSceneNode::stop()
{
    Fnode = NULL;
}



// test if 3d line colide with terrain
// returns true if yes, false if not and store intersection in "outIntersection" vector
bool ShTlTerrainSceneNode::getIntersectionWithLine( core::line3d<f32> line, core::vector3df &outIntersection)
{
    //get relative position of line to terrain
    line.start = line.start - getPosition();
    line.end = line.end - getPosition();

    // find 2d vector of line
    core::vector2d<f32> vector;
    vector.X = line.end.X - line.start.X;
    vector.Y = line.end.Z - line.start.Z;

    // calculate variables used later in calculating coordinates on line
    // used  equations are: "x = y * a + c" and "y = x * b + d"
    // where "a = x / y" and "b = y / x"
    f32 a = 0;
    if(vector.Y != 0) a = vector.X / vector.Y;

    f32 b = 0;
    if(vector.X != 0) b = vector.Y / vector.X;

    // calculate starting and ending tiles
    core::vector3d<s32> start;
    start.X = (s32)(line.start.X / TileSize);
    start.Y = (s32)(line.start.Z / TileSize);

    core::vector3d<s32> end;
    end.X = (s32)(line.end.X / TileSize);
    end.Y = (s32)(line.end.Z / TileSize);

    // some variables dependant on line orientation
    s32 xs = 1;
    s32 xc = 0;
    if(vector.X < 0)
    {
        xs = -1;
        xc = 1;
    }

    s32 ys = 1;
    s32 yc = 0;
    if(vector.Y < 0)
    {
        ys = -1;
        yc = 1;
    }

    // calculate tiles which can intersect with line based on their 2d position
    // then test for intersection

    // take y and calculate x coordinates of tile
    if( core::abs_(vector.X) > core::abs_(vector.Y) )
    {
        s32 x = start.X;
        s32 y;

        for(y=start.Y+yc; y!=end.Y+yc; y+=ys)
        {
            f32 tmp = (f32)(y+ys) * TileSize - line.start.Z;
            tmp = a * tmp + line.start.X;
            s32 xn = (s32)(tmp / TileSize);

            for(x; x!=xn+xs; x+=xs)
                if( getIntersectionWithTile(x, y-yc, line,  outIntersection) )
                {
                    outIntersection += getPosition();
                    return true;
                }

            x = xn;
        }

        for(x; x!=end.X+xs; x+=xs)
            if( getIntersectionWithTile(x, y-yc, line,  outIntersection) )
            {
                outIntersection += getPosition();
                return true;
            }
    }
    // take x and calculate y coordinates of tile
    else
    {
        s32 y = start.Y;
        s32 x;

        for(x=start.X+xc; x!=end.X+xc; x+=xs)
        {
            f32 tmp = (f32)(x+xs) * TileSize - line.start.X;
            tmp = b * tmp + line.start.Z;
            s32 yn = (s32)(tmp / TileSize);

            for(y; y!=yn+ys; y+=ys)
                if( getIntersectionWithTile(x-xc, y, line,  outIntersection) )
                {
                    outIntersection += getPosition();
                    return true;
                }

            y = yn;
        }

        for(y; y!=end.Y+ys; y+=ys)
            if( getIntersectionWithTile(x-xc, y, line,  outIntersection) )
            {
                outIntersection += getPosition();
                return true;
            }
    }

    return false;
}



// load height data from texture
void ShTlTerrainSceneNode::loadHeightMap(const c8 *filename, f32 scale, u32 w, u32 h)
{
    video::IVideoDriver* driver = SceneManager->getVideoDriver();
    if(!driver) return;

    video::IImage *image = driver->createImageFromFile(filename);
    if(!image) return;

    s32 tw = image->getDimension().Width;
    s32 th = image->getDimension().Height;

    s32 we = w + tw;
    if(we > Size.Width+1) we = Size.Width+1;
    s32 he = h + th;
    if(he > Size.Height+1) he = Size.Height+1;

    tw = 0;
    th = 0;

    for(s32 j=h; j<he; j++)
    {
        for(s32 i=w; i<we; i++)
        {
            video::SColor color = image->getPixel(tw, th);

            Data(i,j).Height = (f32)color.getLuminance()/255 * scale;

            tw++;
        }
        tw = 0;
        th++;
    }

    image->drop();

    recalculateBoundingBox();

    smoothNormals();
}



// load color data from texture
void ShTlTerrainSceneNode::loadColorMap(const c8 *filename, u32 w, u32 h)
{
    video::IVideoDriver* driver = SceneManager->getVideoDriver();
    if(!driver) return;

    video::IImage *image = driver->createImageFromFile(filename);
    if(!image) return;

    s32 tw = (u32)image->getDimension().Width;
    s32 th = (u32)image->getDimension().Height;

    s32 we = w + tw;
    if(we > Size.Width+1) we = Size.Width+1;
    s32 he = h + th;
    if(he > Size.Height+1) he = Size.Height+1;

    tw = 0;
    th = 0;

    for(s32 j=h; j<he; j++)
    {
        for(s32 i=w; i<we; i++)
        {
            video::SColor color = image->getPixel(tw, th);

            Data(i,j).Color = color;

            tw++;
        }
        tw = 0;
        th++;
    }

    image->drop();
}



// return true if sector is on screen
bool ShTlTerrainSceneNode::isSectorOnScreen(TlTSector* sctr)
{
    // bounding box of sector
    core::aabbox3d<f32> box = sctr->BoundingBox;

    // get absolute position of bounding box
    box.MinEdge = box.MinEdge + getPosition();
    box.MaxEdge = box.MaxEdge + getPosition();

    // get camera frustrum planes
    const scene::SViewFrustum* frustrum = SceneManager->getActiveCamera()->getViewFrustum();
	core::plane3d<f32> Left = frustrum->planes[scene::SViewFrustum::VF_LEFT_PLANE];
    core::plane3d<f32> Right = frustrum->planes[scene::SViewFrustum::VF_RIGHT_PLANE];
    core::plane3d<f32> Top = frustrum->planes[scene::SViewFrustum::VF_TOP_PLANE];
    core::plane3d<f32> Bottom = frustrum->planes[scene::SViewFrustum::VF_BOTTOM_PLANE];
    core::plane3d<f32> Near = frustrum->planes[scene::SViewFrustum::VF_NEAR_PLANE];
    core::plane3d<f32> Far = frustrum->planes[scene::SViewFrustum::VF_FAR_PLANE];

    // test sector bounding box against planes
    s32 leftRel, rightRel, topRel, bottomRel, nearRel, farRel;

    nearRel = box.classifyPlaneRelation(Near);
    if(nearRel == core::ISREL3D_FRONT) return false;

    leftRel = box.classifyPlaneRelation(Left);
    if(leftRel == core::ISREL3D_FRONT) return false;

    rightRel = box.classifyPlaneRelation(Right);
    if(rightRel == core::ISREL3D_FRONT) return false;

    bottomRel = box.classifyPlaneRelation(Bottom);
    if(bottomRel == core::ISREL3D_FRONT) return false;

    farRel = box.classifyPlaneRelation(Far);
    if(farRel == core::ISREL3D_FRONT) return false;

    topRel = box.classifyPlaneRelation(Top);
    if(topRel == core::ISREL3D_FRONT) return false;

    return true;
}



// update vertices of sector
void ShTlTerrainSceneNode::updateVertices(TlTSector &sector)
{
	scene::SMeshBuffer* MeshBuffer=new scene::SMeshBuffer();
	scene::SMesh* Mesh=new scene::SMesh();

    for(u32 j=sector.Offset.Y; j<sector.Offset.Y+sector.Size.Height; j++)
        for(u32 i=sector.Offset.X; i<sector.Offset.X+sector.Size.Width; i++)
        {
            // positon of tile relative to terrain
            u32 x = i + MeshPosition.X;
            u32 y = j + MeshPosition.Y;

            // update position
            Tile(i,j).Vertex[0]->Pos = core::vector3df(x*TileSize, Data(x,y).Height, y*TileSize);
            Tile(i,j).Vertex[1]->Pos = core::vector3df(x*TileSize, Data(x,y+1).Height, (y+1)*TileSize);
            Tile(i,j).Vertex[2]->Pos = core::vector3df( (x+1)*TileSize, Data(x+1,y+1).Height, (y+1)*TileSize);
            Tile(i,j).Vertex[3]->Pos = core::vector3df( (x+1)*TileSize, Data(x+1,y).Height, y*TileSize);

            // update normals
            Tile(i,j).Vertex[0]->Normal = Data(x,y).Normal;
            Tile(i,j).Vertex[1]->Normal = Data(x,y+1).Normal;
            Tile(i,j).Vertex[2]->Normal = Data(x+1,y+1).Normal;
            Tile(i,j).Vertex[3]->Normal = Data(x+1,y).Normal;

            // update texture coordinates
            Tile(i,j).Vertex[0]->TCoords = UVdata(x,y).Vertex[0];
            Tile(i,j).Vertex[1]->TCoords = UVdata(x,y).Vertex[1];
            Tile(i,j).Vertex[2]->TCoords = UVdata(x,y).Vertex[2];
            Tile(i,j).Vertex[3]->TCoords = UVdata(x,y).Vertex[3];

			for (int z=0;z<4;z++)
				MeshBuffer->Vertices.push_back(video::S3DVertex(
						Tile(i,j).Vertex[z]->Pos,
						core::vector3df(0,0,0),	video::SColor(0,0,0,0), core::vector2df(0,0)));
        }

	for (u32 z=0;z<sector.Index.size();z++)
	{
		MeshBuffer->Indices.push_back(
					sector.Index[z]);
	}

	Mesh->addMeshBuffer(MeshBuffer);
	MeshBuffer->drop();

	Mesh->drop();
}



// update 2nd texture layer
void ShTlTerrainSceneNode::updateTexture(u32* p, TlTSector &sector)
{
    u32 x, y;

    // in case created texure is larger than terrain mesh, update one more pixel
    // on each axis to get rid of unused pixels at the border blended in to used ones
    u32 w = 0;
    if(MeshSize.Width < CTexture->getSize().Width) w = 1;
    u32 h = 0;
    if(MeshSize.Height < CTexture->getSize().Height) h = 1;

    for(u32 j=sector.Offset.Y; j<sector.Offset.Y + sector.Size.Height + h; j++)
    {
        u32 n = CTexture->getSize().Height-1 - j;
        for(u32 i=sector.Offset.X; i<sector.Offset.X + sector.Size.Width + w; i++)
        {
            x = i + MeshPosition.X;
            y = j + MeshPosition.Y;

            p[n*CTexture->getSize().Width + i] = Data(x,y).Color.color;
        }
    }
}



// test if 3d line colide with tile
// returns true if yes, false if not and store intersection in "outIntersection" vector
bool ShTlTerrainSceneNode::getIntersectionWithTile(s32 w, s32 h, core::line3d<f32> line,  core::vector3df &outIntersection)
{
    // test if not out of terrain bounds
    if(w < 0) return false;
    if(h < 0) return false;
    if(w > Size.Width-1) return false;
    if(h > Size.Height-1) return false;

    // vertices
    core::vector3df v0(w*TileSize, getHeight(w,h), h*TileSize);
    core::vector3df v1(w*TileSize, getHeight(w,h+1), (h+1)*TileSize);
    core::vector3df v2((w+1)*TileSize, getHeight(w+1,h+1), (h+1)*TileSize);
    core::vector3df v3((w+1)*TileSize, getHeight(w+1,h), h*TileSize);

    // firsth test collision with tile bounding box
    core::aabbox3d<f32> box;
    box.reset(v0);
    box.addInternalPoint(v1);
    box.addInternalPoint(v2);
    box.addInternalPoint(v3);

    if (!box.intersectsWithLine(line)) return false;;

    // test collision with tile itself

    core::triangle3df triangle;

    // test upper left trialgle of tile
    triangle.set(v0,v1,v2);
    bool collision1 = triangle.getIntersectionWithLimitedLine(line, outIntersection);

    // test lower right triangle of tile
    triangle.set(v0,v2,v3);
    core::vector3df intersect2;
    bool collision2 = triangle.getIntersectionWithLimitedLine(line, intersect2);

    // compare results and decide what to return
    if(collision2)
    {
        // if line collide with both triangles, test which colision is closer to start of line
        if(collision1)
        {
            f32 distance1 = line.start.getDistanceFromSQ(outIntersection);
            f32 distance2 = line.start.getDistanceFromSQ(intersect2);
            if(distance2 < distance1) outIntersection = intersect2;
            return true;
        }
        // if it collide only with second one
        else
        {
            outIntersection = intersect2;
            return true;
        }
    }

    return collision1;
}
