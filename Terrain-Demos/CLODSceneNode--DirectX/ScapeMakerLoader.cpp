//!
//! SCAPEMAKELOADER
//! Object for loading files generated from ScapeMaker (http://www.dplate.de/scapemaker/)
//!
//! CSIMPLEWATERPLANE
//! A waterplane capable of texture scrooling, and render lightmaps ( for use with scapemaker
//!
//! By Paulo Rogério de Oliveira ( paulo_cmv@hotmail.com < MSN - EMAIL > )
//! ----------------------------
//! You can use it for any purpose, as you want, only dont remove this notice.
//! If you make any changes, improvements, or you have comments, sugestions,
//! you can report it back by mail or msn. This way i can try to improve the code.


//! PREPARING SCAPEMAKER

//! Transparent textures must be .tga or .png. Convert your objects in ScapeMaker\Objects before starting terrain creation
//! Must have lightmap.jpg, waterlightmap.jpg, water.jpg and heightmap.png in engine folder
//! Must have terraintexture.bmp in engine/textures folder
//! objects info are copied from engine/objects folder

//! NOTES WHEN LOADING SCAPEMAKER MAPS

//! NOTE THIS DONT LOAD THE .ZIP FILE EXPORTED FROM SCAPEMAKER, it loads some files created
//! from scapemaker that are inside landscapes/<mapname>/ path: i.e: scapemaker/landscapes/river
//! for river map
//! note also that not all this files are necessary, others must be exported by hand

//! 1. Export heightmap.png (256x256 pixels) from Topogray tab and put it under <mapname>/engine folder
//! 2. In Environment tab, uncheck Ignore Objects and generate it. Export textures with lightmaps
//! and put it in <mapname>engine/textures folder
//! 3. In Environment tab, check Ignore Objects and generate it again. This way, you will have a lightmap.jpg
//! file without shadows of trees. This shadows interferes when loader get a pixel from lightmap to
//! determine the overal color (vertex colors) of objects. If you get a lightmap with objects shadow
//! then the loader will pick a pixel under the tree position, that will be always dark (shadow)

//! I choosed to edit lightmap.jpg ( smooth and improve brightness & contrast ) to have better looking objects ( not to dark )

//! SKYBOX
//! Scapemaker have a skybox generating script, so you can use it.
//! rename your skybox textures like river_sb_top.bmp and put it in engine folder
//! top texture must be rotated 90º right
//! bottom texture must be rotated 270º right
//! in future, i will try to correct the skybox script

//! FOR MORE INFO ABOUT NECESSARY FILES, TAKE A LOOK AT RIVER FOLDER

#include <irrlicht.h>

#include <string.h>
#include "SceneReader.h"

#include "CQuadTreeSceneNode.h"
#include "CLODSceneNode.h"
#include "ScapeMakerLoader.h"

#define useQuadTree

CSimpleWaterSceneNode::CSimpleWaterSceneNode( IrrlichtDevice* Device, s32 ID, s32 upPosition, ITerrainSceneNode* terrain, const c8* mapname )
        :ISceneNode( Device->getSceneManager()->getRootSceneNode(), Device->getSceneManager(), ID,
                     core::vector3df( 0, 0, 0 ),
                     core::vector3df( 0, 0, 0 ),
                     core::vector3df( 1, 1, 1 ) )
{
    device = Device;
    driver = device->getVideoDriver();
    BBox.MinEdge = core::vector3df( 0, 0, 0 );
    BBox.MaxEdge = core::vector3df(  255,  255,  255 ) * terrain->getScale();
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 2;
    indices[4] = 3;
    indices[5] = 0;

    watx = 0;

    uppos = upPosition;

    transp = 60;
    rgb = 60;

    vertices[0].Color = video::SColor( transp, rgb, rgb, rgb );
    vertices[0].Normal = core::vector3df( 0, 1, 0 );
    vertices[0].Pos = core::vector3df( 0, uppos, 0 );
    vertices[0].TCoords2 = core::vector2df( 0, 0 );

    vertices[1].Color = video::SColor( transp, rgb, rgb, rgb );
    vertices[1].Normal = core::vector3df( 0, 1, 0 );
    vertices[1].Pos = core::vector3df( 0, uppos, 255 );
    vertices[1].TCoords2 = core::vector2df( 0, 1 );

    vertices[2].Color = video::SColor( transp, rgb, rgb, rgb );
    vertices[2].Normal = core::vector3df( 0, 1, 0 );
    vertices[2].Pos = core::vector3df( 255, uppos, 255 );
    vertices[2].TCoords2 = core::vector2df( 1, 1 );

    vertices[3].Color = video::SColor( transp, rgb, rgb, rgb );
    vertices[3].Normal = core::vector3df( 0, 1, 0 );
    vertices[3].Pos = core::vector3df( 255, uppos, 0 );
    vertices[3].TCoords2 = core::vector2df( 1, 0 );

    char buff[300];
    sprintf( buff, "%s//engine//water.jpg", mapname );
    setMaterialTexture( 0, driver->getTexture( buff ) );
    sprintf( buff, "%s//engine//waterlightmap.jpg", mapname );
    setMaterialTexture( 1, driver->getTexture( buff ) );

    setMaterialFlag( video::EMF_LIGHTING, false );
    setMaterialFlag( video::EMF_BACK_FACE_CULLING, false );
    setMaterialType( video::EMT_LIGHTMAP );

    mat.setScale( core::vector3df( terrain->getScale().X, 1, terrain->getScale().X ) );
}

CSimpleWaterSceneNode::~CSimpleWaterSceneNode()
{}

video::SMaterial& CSimpleWaterSceneNode::getMaterial(s32 i)
{
    return material;
}

s32 CSimpleWaterSceneNode::getMaterialCount()
{
    return 1;
}

const core::aabbox3d<f32>& CSimpleWaterSceneNode::getBoundingBox() const
{
    return BBox;
}

void CSimpleWaterSceneNode::OnPreRender()
{
    if ( isVisible() )
    {
        device->getSceneManager()->registerNodeForRendering( this );//, scene::ESNRP_TRANSPARENT );
//        ISceneNode::OnPreRender();
    }
}

void CSimpleWaterSceneNode::render()
{
    watx += 0.001f;
    if ( watx > 1000000 )
        watx = 0;

    vertices[0].TCoords = core::vector2df( 0+watx, 0+watx );
    vertices[1].TCoords = core::vector2df( 0+watx, 10+watx );
    vertices[2].TCoords = core::vector2df( 10+watx, 10+watx );
    vertices[3].TCoords = core::vector2df( 10+watx, 0+watx );
    driver->setMaterial( material );
    driver->setTransform( video::ETS_WORLD, mat );
    driver->drawIndexedTriangleList( &vertices[0], 4, &indices[0], 2 );
}


float ScapeMakeLoader::getTerrainHeight( video::IImage* TerrainHeightMap, scene::ITerrainSceneNode* Terrain, core::vector3df p )
{
    core::dimension2d<f32> size;
    core::vector3df xz(p.X/Terrain->getScale().X,0.0f,p.Z/Terrain->getScale().Z);

    s32 x1 = (s32) floorf( xz.X );
    s32 z1 = (s32) floorf( xz.Z );

    if ( x1 < 1 ||
            z1 < 1 ||
            x1 > TerrainHeightMap->getDimension().Width - 1 ||
            z1 > TerrainHeightMap->getDimension().Height - 1 )
    {
        // fora do terreno
        return 0;
    }
    f32 ay = TerrainHeightMap->getPixel(x1,z1).getBlue()* Terrain->getScale().Y;
    f32 by = TerrainHeightMap->getPixel(x1+1,z1).getBlue()* Terrain->getScale().Y;
    f32 cy = TerrainHeightMap->getPixel(x1,z1+1).getBlue()* Terrain->getScale().Y;
    f32 dy = TerrainHeightMap->getPixel(x1+1,z1+1).getBlue()* Terrain->getScale().Y;
    f32 u1 = xz.X - (f32)x1;
    f32 v1 = xz.Z - (f32)z1;
    return ay*(1.0f-u1)*(1.0f-v1) + by*u1*(1.0f-v1) + cy*(1.0f-u1)*v1 + dy*u1*v1;
};

// change extension of a given file name
core::stringc ScapeMakeLoader::changeFileExt( core::stringc filename, core::stringc newExt )
{
    int fl = filename.findLast( '.' );
    if ( fl > -1 )
        filename = filename.subString( 0, fl );
    return filename + newExt;
}

void ScapeMakeLoader::LoadObjects( video::IImage* TerrainHeightMap,
                                   video::IImage* TerrainLightMap )
{
#ifdef useQuadTree
    CQuadTreeSceneNode* quad = new CQuadTreeSceneNode(
                                   device->getSceneManager()->getRootSceneNode(),
                                   device->getSceneManager(),
                                   -1,
                                   4 );
    //quad->setDebugDataVisible(true);
#endif

    XMLParser* xmlObjects = new XMLParser( device );

    char buffer[500];
    // get path to objects.txt
    sprintf( buffer, "%s//objects.txt", levelname );

    xmlObjects->parseFile( buffer );
    int objct = xmlObjects->RootLeaf.getChild( 0 )->childCount();

    scene::ISceneManager* manager = device->getSceneManager();
    video::IVideoDriver* driver = device->getVideoDriver();

#ifdef useQuadTree
    // start adding nodes to quadtree
    quad->startAdding();
#endif

    // for each object type
    for ( int x = 0; x < objct; x++ )
    {
        // get name of current object
        const c8* objname = xmlObjects->RootLeaf.getChild(0)->getChild(x)->getChild(0)->getData(0)->data.c_str();

        XMLParser* xmlobjpositions = new XMLParser( device );
        const c8* objpositionsname;
        // get path to positions.txt for current object
        sprintf( buffer, "%s//engine//objects//%s//positions.txt", levelname, objname );
        printf("%s\n", buffer);

        xmlobjpositions->parseFile( buffer );

        ParserLeaf* current = xmlobjpositions->RootLeaf.childs[0];
        // number of objects in map for current type
        int ct = current->childCount();

        XMLParser* xmlobjsettings = new XMLParser( device );
        const c8* settingsfilename;
        // get path to settings.txt for current object
        sprintf( buffer, "%s//engine//objects//%s//settings.txt", levelname, objname );
        printf("%s\n", buffer);

        xmlobjsettings->setShowDebug(true);
        xmlobjsettings->parseFile( buffer );

        ParserLeaf* lodchild = xmlobjsettings->RootLeaf.getChild("lods");

        // if a lod node is not found in xml
        if ( !lodchild )
        {
            delete xmlobjpositions;
            delete xmlobjsettings;
            continue;
        }

        int lodcount = lodchild->childCount();

        core::vector3df sca = terrain->getScale();
        // for each object in map for current type
        for ( int x = 0; x < ct; x++ )
        {
            printf("loop for each object in map %i   lodcount: %i  name: ", x, lodcount);
            printf("%i\n", lodchild->childCount() );
            // get x and z positions
            ParserLeaf* cur = current->getChild( x )->getChild( 0 );
            f32 px = cur->getData( 0 )->FloatValue();
            //
            cur = current->getChild( x )->getChild( 1 );
            f32 pz = cur->getData( 0 )->FloatValue();

            // in irrlicht, terrain have only 240 units size, then if a tree will grow outside
            // terrain, skip tree
            if ( ( px < 0 ) || ( px > 239 ) || ( pz < 16 ) || ( pz > 255 ) )
                continue;
            video::SColor objcolor;
            if ( ( px <= 255 ) && ( pz <= 255 ) )// && ( px > 10 ) && ( pz > 10 ) )
                objcolor = TerrainLightMap->getPixel( px, 255-pz );

            px *= sca.X;
            pz = (255*sca.Z) - (pz*sca.Z);

            // do some correction by hand
            px += 6;
            pz += 25;

            // get y based on terrain height
            f32 py = getTerrainHeight( TerrainHeightMap, terrain, core::vector3df( px, 0, pz ) );

            // get y rotation
            cur = current->getChild( x )->getChild( 2 );
            f32 roty = cur->getData( 0 )->FloatValue();
            // get scale
            cur = current->getChild( x )->getChild( 3 );
            f32 scal = cur->getData( 0 )->FloatValue();

            CLODSceneNode* nod = new CLODSceneNode( device->getSceneManager()->getRootSceneNode(),
                                                    device,
                                                    -1,
                                                    core::vector3df( px, py, pz ),
                                                    core::vector3df( 0, roty, 0 ),
                                                    core::vector3df( sca.X*scal, sca.X*scal, sca.X*scal ) );

            nod->setColor(objcolor);
            nod->setMaterialFlag( video::EMF_LIGHTING, false );

            // for each lod in current object
            for ( int lodinc = 0; lodinc < lodcount; lodinc++ )
            {
                printf("loop for each lod in object %i\n", lodinc);
                // get path for mesh object for current lod
                const c8* lodfilename = lodchild->getChild(lodinc)->getChild(0)->getData(0)->data.c_str();
                sprintf( buffer, "%s//engine//objects//%s//%s", levelname, objname, lodfilename );
                printf("%s\n", buffer);

                bool billb = ( lodchild->getChild(lodinc)->getChild(1)->getData(0)->IntValue() == 1 );
                f32 loddist = lodchild->getChild(lodinc)->getChild(2)->getData(0)->FloatValue();

                scene::IAnimatedMesh* mesh = manager->getMesh( buffer );

                if ( mesh )
                {
                    nod->addLOD( mesh->getMesh(0), loddist*sca.X, billb );
                    nod->setMaterialFlag( video::EMF_LIGHTING, false );
#ifdef useQuadTree

                    quad->addNode( nod );
#endif

                }

            }

        }
        delete xmlobjpositions;
        delete xmlobjsettings;

    }
    delete xmlObjects;

#ifdef useQuadTree

    quad->endAdding();
#endif

    printf("end loading objects\n");
}

scene::ITerrainSceneNode* ScapeMakeLoader::LoadTerrain( const c8* TextureName, const c8* TerrainHeightMapFileName )
{
    video::ITexture* texture = device->getVideoDriver()->getTexture( TextureName );
    scene::ITerrainSceneNode* terrain;

    terrain = device->getSceneManager()->addTerrainSceneNode( TerrainHeightMapFileName,
              0, -1,
              core::vector3df( 0, 0, 0 ),
              core::vector3df( 0.0f, 0.0f, 0.0f),
              core::vector3df( terrainscale, 2.0f, terrainscale ) );

    terrain->setMaterialTexture( 0, texture );
    terrain->setMaterialFlag( video::EMF_LIGHTING, false );

    return terrain;
}

ScapeMakeLoader::ScapeMakeLoader( IrrlichtDevice* Device )
{
    device = Device;
}

ScapeMakeLoader::~ScapeMakeLoader()
{}

scene::ITerrainSceneNode* ScapeMakeLoader::getTerrain()
{
    return terrain;
}

void ScapeMakeLoader::LoadScape( const c8* levelfile, s16 scale )
{
    levelname = levelfile;

    terrainscale = scale;

    char buffer[200];
    char buffer2[200];
    char buffer3[200];
    sprintf( buffer, "%s//engine//textures//terraintexture.jpg", levelname );
    sprintf( buffer2, "%s//engine//heightmap.jpg", levelname );

    terrain = LoadTerrain( buffer, buffer2 );

    sprintf( buffer3, "%s//engine//lightmap.jpg", levelname );

    video::IImage* lmap = device->getVideoDriver()->createImageFromFile( buffer3 );
    video::IImage* hmap = device->getVideoDriver()->createImageFromFile( buffer2 );

    LoadObjects( hmap, lmap );
    lmap->drop();
    hmap->drop();
}
