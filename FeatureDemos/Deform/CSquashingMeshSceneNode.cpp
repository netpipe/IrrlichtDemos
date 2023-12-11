
#include "CSquashingMeshSceneNode.h"

namespace irr
{
namespace scene
{

//! constructor
CSquashingMeshSceneNode::CSquashingMeshSceneNode(
                IMesh* mesh, ISceneNode* parent, ISceneManager* mgr, s32 id,
                const core::vector3df& position, const core::vector3df& rotation,
                const core::vector3df& scale):

                CMeshSceneNode1(mesh, /* make sure the parent is not NULL */
                               parent ? parent : mgr->getRootSceneNode(),
                               mgr, id, position, rotation, scale),

                OriginalMesh(0),
                lasttime(0)
{
        #ifdef _DEBUG
        setDebugName("CSquashingMeshSceneNode");
        #endif

        /*
          We have just been passed an IMesh pointer
          We use the mesh manipulator to copy it to a SMesh and keep a pointer
          to the original IMesh just in case we need it.
        */

        if (!mesh)
                return;

        IMesh* clone = SceneManager->getMeshManipulator()->createMeshCopy(mesh);
        OriginalMesh = Mesh;
        Mesh = clone;
}



//! destructor
CSquashingMeshSceneNode::~CSquashingMeshSceneNode()
{
    // in a scene node's destructor we drop everything we have grabbed
    // calling CMeshSceneNode1(mesh) in our constructor causes a Mesh->grab()
    // and later on we rename it to OriginalMesh. we'll drop that here
    if (OriginalMesh)
       OriginalMesh->drop();
}



//! frame
void CSquashingMeshSceneNode::OnPreRender()
{
     if (IsVisible)
        SceneManager->registerNodeForRendering(this);

  ///   ISceneNode::OnPreRender();
}

//! frame
void CSquashingMeshSceneNode::OnPostRender(u32 t)
{
     // we deform the mesh depending on how much time has passed
     deformMesh(t);

///     ISceneNode::OnPostRender(t);
}


void CSquashingMeshSceneNode::deformMesh(u32 t)
{
    // we return if there is no mesh
    if (!Mesh)
            return;

    /*
      We need to know how much time has passed since
    */

    // we work out how much time has passed since the last frame
    if (lasttime == 0)
       lasttime = t-1;

    // 'time' is number of milisecs passed as a float
    f32 time = t-lasttime;
    // and we remember the current time for the next loop
    lasttime =t;

    // One IMesh can contain many mesh buffers,
    // we'll find out how many there are and loop through them all
    s32 meshBufferCount = Mesh->getMeshBufferCount();

    for (s32 b=0; b<meshBufferCount; ++b)
    {

        /*
          each mesh buffer is made of: (among other things)

           a list of vertices:
             each vertex is a point in space,
             they hold position, texture coordinates
             there are different types of vertices

           a list of indices:
             this is a list of numbers of vertices to make triangles,
             forward facing triangles are clockwise
             for example, "0,1,2 0,2,1" would make a double sided triangle from
             vertices[0] vertices[1] and vertices[2]
        */
        /*
           since we dont change how the mesh is joined together, we just want to
           move the vertices round. we'll ignore indices.
           we get he number of vertices
        */

        s32 vtxCnt = Mesh->getMeshBuffer(b)->getVertexCount();

        /*
          we need to know where the bottom of the mesh is so we can move each
          vertex down to it.
        */
        f32 miny = Mesh->getBoundingBox().MinEdge.Y;
        /*
          because there are different types of vertices, we need separate code
          for each one.
          http://irrlicht.sourceforge.net/docu/namespaceirr_1_1video.html#a111
          We'll just do two types of vertices and ignore the ones with tangents
        */
        switch(Mesh->getMeshBuffer(b)->getVertexType())
        {
            case video::EVT_STANDARD:
                {
                    /*
                      we start by get a pointer to the start of the vertex array.
                    */
                    video::S3DVertex* v =
                            (video::S3DVertex*)Mesh->getMeshBuffer(b)->getVertices();
                    /*
                      Remember, we have the original mesh too. Useful for non-permanent
                      deformations.
                      video::S3DVertex* v2 =
                            (video::S3DVertex*)OriginalMesh->getMeshBuffer(b)->getVertices();
                    */
                    // loop through every vertex in the list
                    for (s32 i=0; i<vtxCnt; ++i)
                    {
                        /*
                          if this vertex is not squashed flat then we move it down a bit.
                          we multiply by the time so that it moves the same speed
                          no matter how many fps there are.
                        */
                        if (v[i].Pos.Y > miny)
                           v[i].Pos.Y -= 0.01 * time;
                    }
                }
                break;

            case video::EVT_2TCOORDS:
                {
                    // this is exactly the same code but for vertices with 2 tcoords
                    video::S3DVertex2TCoords* v =
                            (video::S3DVertex2TCoords*)Mesh->getMeshBuffer(b)->getVertices();

                    for (s32 i=0; i<vtxCnt; ++i)
                    {
                        if (v[i].Pos.Y > miny)
                           v[i].Pos.Y -= 0.01 * time;
                    }
                }
                break;

                // we don't deal with these yet
                case video::EVT_TANGENTS:
                {
                    video::S3DVertexTangents* v =
                            (video::S3DVertexTangents*)Mesh->getMeshBuffer(b)->getVertices();
                }
                break;
        }// end switch
    }// end for all mesh buffers
        /*


        */
        SceneManager->getMeshManipulator()->recalculateNormals(Mesh);
}

} // end namespace scene
} // end namespace irr

