//!
//! CLODSCENENODE
//! Irrlicht scenenode for loading multiple mesh lods
//!
//!
//! By Paulo Rogério de Oliveira ( paulo_cmv@hotmail.com < MSN - EMAIL > )
//! ----------------------------
//! You can use it for any purpose, as you want, only dont remove this notice.
//! If you make any changes, improvements, or you have comments, sugestions,
//! you can report it back by mail or msn. This way i can try to improve the code.

#include "MyMaterial.h"
#include "CLODSceneNode.h"
#include "irrlicht.h"

using namespace irr;

//MyMaterialD3D9_ALPHA* MaterialD3D9_ALPHA = 0;
s32 myMaterialType = 0;

scene::ISceneNode* getMyParent( scene::ISceneManager* smgr, scene::ISceneNode* aParent )
{
    if ( aParent )
        return aParent;
    else
        return smgr->getRootSceneNode();
}

// Slowly fades a mesh
void CLODMesh::performFading()
{
    if ( fadeRate != 0.0f )
    {
        u32 elapsedtime = tmr->getTime() - lastTick;
        if ( elapsedtime != 0 )
            transp += elapsedtime * fadeRate;
    }
    lastTick = tmr->getTime();

    if ( transp < 0.0f )
    {
        transp = 0;
        fadeRate = 0.0f;
    }
    if ( transp > 255.0f )
    {
        transp = 255;
        fadeRate = 0.0f;
    }
}

// extract all materials from mesh
void CLODMesh::getAllMaterials()
{
    if (mesh)
    {
        video::SMaterial* mat;
        for (s32 i=0; i<mesh->getMeshBufferCount(); ++i)
        {
            scene::IMeshBuffer* mb = mesh->getMeshBuffer(i);
            if (mb)
                materials.push_back( mb->getMaterial() );
        }
    }
}

void CLODMesh::setViewDistance( f32 newViewDistance )
{
    maxViewDistance = newViewDistance;
}

void CLODMesh::setTransparency( s32 newTransparency )
{
    transp = newTransparency;
}

// to get off one bug in IMeshManipulator
void CLODMesh::setVertexColors( scene::IMeshBuffer* mb )
{
    void* v = mb->getVertices();
    s32 vtxcnt = mb->getVertexCount();
    s32 i = 0;

    switch(mb->getVertexType())
    {
        case video::EVT_STANDARD:
            for (; i<vtxcnt; ++i) ((video::S3DVertex*)v)[i].Color = color;
            break;
        case video::EVT_2TCOORDS:
            for (; i<vtxcnt; ++i) ((video::S3DVertex2TCoords*)v)[i].Color = color;
            break;
        case video::EVT_TANGENTS:
            for (; i<vtxcnt; ++i) ((video::S3DVertexTangents*)v)[i].Color = color;
    }
}

// Constructor
CLODMesh::CLODMesh( IrrlichtDevice* device, scene::IMesh* Mesh )
{
    mesh = Mesh;
    mesh->grab();
    SceneManager = device->getSceneManager();
    driver = SceneManager->getVideoDriver();
    tmr = device->getTimer();
    lastTick = tmr->getTime();
    isBillBoard = false;
    transp = 255;
}

// destructor
CLODMesh::~CLODMesh()
{
    mesh->drop();
}

// max distance which this lod can be viewed
f32 CLODMesh::getViewDistance()
{
    return maxViewDistance;
}

//
s32 CLODMesh::getMaterialCount()
{
    if ( mesh )
        return materials.size();
    else
        return 0;
}

//
video::SMaterial& CLODMesh::getMaterial(s32 i)
{
    if ( ( i < 0 ) || ( i >= materials.size() ) )
        return *((video::SMaterial*)0);

    if ( mesh )
        return materials[i];
    else
        return *((video::SMaterial*)0);
}

// set type for all materials
void CLODMesh::setMaterialType(video::E_MATERIAL_TYPE newType)
{
    for (s32 i=0; i<getMaterialCount(); ++i)
        getMaterial(i).MaterialType = newType;
}

// return mesh for this lod
scene::IMesh* CLODMesh::getMesh()
{
    return mesh;
}

// return current transparency ( alpha )
s32 CLODMesh::getTransparency()
{
    return transp;
}

// set texture for all MeshBuffers
void CLODMesh::setMaterialTexture(s32 textureLayer, video::ITexture* texture)
{
    if (textureLayer<0 || textureLayer>= video::MATERIAL_MAX_TEXTURES)
        return;

    ///for (s32 i=0; i<getMaterialCount(); ++i)
///        getMaterial(i).Textures[textureLayer] = texture;
}

const core::aabbox3d<f32>& CLODMesh::getBoundingBox() const
{
    if ( mesh )
        return mesh->getBoundingBox();
    else
        return core::aabbox3df();
}

void CLODMesh::render()
{
    // update fading with time
    performFading();

    // if dont have a mesh, dont have a driver, or its to transparent, dont render it
    if ( !mesh || !driver || ( transp <= 3 ) )
        return;

    color.setAlpha( transp );

    video::IMaterialRenderer* rnd;
    scene::IMeshBuffer* mb;
    // standard rendering routine
    for (s32 i=0; i<mesh->getMeshBufferCount(); ++i)
    {
        mb = mesh->getMeshBuffer(i);
        rnd = driver->getMaterialRenderer( materials[i].MaterialType );
        driver->setMaterial( materials[i] );
        setVertexColors( mb );
        driver->drawMeshBuffer(mb);
    }
}

// start fading in this lod, from current alpha to 255
void CLODMesh::FadeIn( f32 newFadeRate )
{
    fadeRate = newFadeRate;
}

// start fading out this lod, from current alpha to 0
void CLODMesh::FadeOut( f32 newFadeRate )
{
    fadeRate = -newFadeRate;
}










CLODSceneNode::~CLODSceneNode()
{
    while ( LODs.getSize() > 0 )
    {
        delete *(LODs.begin());
    }
    LODs.empty();
}

s32 CLODSceneNode::getMaterialCount()
{
    s32 mcount = 0;
    for ( int x = 0; x < LODs.getSize(); x++ )
        mcount += (*(LODs.begin()+x))->getMaterialCount();
    return mcount;
}

void CLODSceneNode::setMaterialFlag(video::E_MATERIAL_FLAG flag, bool newvalue)
{
 ///   for (s32 lod=0; lod<LODs.getSize(); lod++)
 ///      for (s32 i=0; i<(*(LODs.begin()+lod))->getMaterialCount(); ++i)
///            (*(LODs.begin()+lod))->getMaterial(i).Flags[flag] = newvalue;
}

const core::aabbox3d<f32>& CLODSceneNode::getBoundingBox() const
{
    return BBox;
}

// set diffuse color to modulate with texture
void CLODSceneNode::setColor( video::SColor newColor )
{
    color = newColor;
}

// add a new lod newMesh, that can be viewed from maxViewDistance on previous lod up to maxViewDistance
// if BillBoard is true, this LOD will rotate to look the camera
void CLODSceneNode::addLOD( scene::IMesh* newMesh, f32 maxViewDistance, bool BillBoard )
{
    if ( !newMesh )
        return;
    CLODMesh* L = new CLODMesh( device, newMesh );
    LODs.push_back( L );
    L->getAllMaterials();
    L->setMaterialType( (video::E_MATERIAL_TYPE)StandardMaterialType );
    L->setViewDistance( maxViewDistance );
    L->color = color;
    L->isBillBoard = BillBoard;
    BBox = newMesh->getBoundingBox();
    for ( int i = 1; i < LODs.getSize(); i++ )
        BBox.addInternalBox( (*(LODs.begin()+i))->getBoundingBox() );
}

// return lod by index
CLODMesh* CLODSceneNode::getLOD( s16 index )
{
    return (*(LODs.begin()+index));
}

// calculate which lod is to be showed at this distance from camera
void CLODSceneNode::OnPreRender()
{
//    ISceneNode::OnPreRender();

    // if dont have any lod, return
    if ( ( !IsVisible ) || ( LODs.getSize() == 0 ) )
        return;

    // by default, this node is visible
    bool vis = true;

    // update each 500 miliseconds
    if ( (tmr->getTime() - lastTick) > 500 )
    {
        lastTick = tmr->getTime();

        vis = false;

        // distance from camera
        scene::ICameraSceneNode* cam = SceneManager->getActiveCamera();
        f32 curdist = cam->getAbsolutePosition().getDistanceFrom( getAbsolutePosition() );
        f32 maxcamv = cam->getFarValue() - 30.0f;

        int ic = 0;

        float faixadistmaior = 0;
        float faixadistmenor = 0;

        CLODMesh* LOD;
        // find out the current lod index
        for ( core::list< CLODMesh* >::Iterator child = LODs.begin(); child != LODs.end(); child++ )
        {
            LOD = (*(child));
            if ( ic == 0 )
                faixadistmenor = 0;
            else
                faixadistmenor = (*(child-1))->getViewDistance();

            faixadistmaior = LOD->getViewDistance();
            // if this lod go away the camera FarValue, limit it to camera FarValue distance
            faixadistmaior = faixadistmaior > maxcamv ? maxcamv : faixadistmaior;

            // this lod scenenode is in visible layer
            if ( ( curdist >= faixadistmaior ) || ( curdist <= faixadistmenor ) )
            {
                // if its not transparent this moment, turn it to transparent
                if ( LOD->getTransparency() >= 3 )
                    LOD->FadeOut(0.6f);
            }
            // this lod scenenode is out of their visibility layer
            else
            {
                // if its alpha is above 3 ( visible ) fade it out to hide it
                if ( LOD->getTransparency() < 3 )
                    LOD->FadeIn(0.6f);
                vis = true;
            }

            ic++;
        }
    }

    // if its visible, register for rendering as transparent
    if ( vis )
    {
        // register always as transparent
        SceneManager->registerNodeForRendering(this, scene::ESNRP_TRANSPARENT);
    }
}

void CLODSceneNode::render()
{
    driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

    // for debug purposes only:
    if (DebugDataVisible)
    {
        video::SMaterial m;
        m.Lighting = false;
        driver->setMaterial( m );
        driver->draw3DBox(BBox, video::SColor(255,255,255,255));
    }

    CLODMesh* LOD;
    // for each lod
    for ( core::list<CLODMesh*>::Iterator child = LODs.begin(); child != LODs.end(); child++ )
    {
        LOD = (*(child));
        if ( ( LOD->getTransparency() > 3 ) && ( LOD->isBillBoard ) )
        {
            core::vector3df position = getAbsolutePosition();
            core::vector3df Target = SceneManager->getActiveCamera()->getAbsolutePosition() - position;

            Target = Target.getHorizontalAngle();
            Target.Y += 180;
            Target.Z = 0;
            Target.X = 0;
            setRotation( Target );
        }

        LOD->render();
    }
}

CLODSceneNode::CLODSceneNode( ISceneNode* parent,
                                    IrrlichtDevice* Device,
                                    s32 id,
                                    const core::vector3df& position,
                                    const core::vector3df& rotation,
                                    const core::vector3df& scale )
        : ISceneNode( parent, Device->getSceneManager(), id, position, rotation, scale )
{
    device = Device;
    currentLOD = 0;
    driver = device->getVideoDriver();

    setMaterialType( (video::E_MATERIAL_TYPE)myMaterialType );
    StandardMaterialType = myMaterialType;
    tmr = device->getTimer();
    lastTick = tmr->getTime();
}

