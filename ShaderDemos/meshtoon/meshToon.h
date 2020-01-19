#ifndef TOONOUTLINE_H
#define TOONOUTLINE_H
#include <irrlicht.h>

struct indexRedirect {
   int oldIndex;
   int newIndex;
};
struct vertCombine {
   irr::core::vector3df totalPos;
   irr::core::vector3df totalNormal;
   int count;
};

// Adds a toon outline to a mesh. Works by copying the mesh, and expanding by the normals. It then flips the faces.
irr::scene::IMesh* createToonOutlineMesh(irr::scene::ISceneManager* smgr, irr::scene::IMesh* mesh, float size, irr::video::SColor color) {

   // Copy the mesh buffers
   irr::scene::SMesh* toonMesh = smgr->getMeshManipulator()->createMeshCopy(mesh);


   for (int i = 0; i < toonMesh->getMeshBufferCount(); i++) {

      // Find duplicate vertices, then add an indexRedirect so indices can be updated and the vertices can later be collapsed
      irr::video::S3DVertex* sourceVertices = (irr::video::S3DVertex*)toonMesh->getMeshBuffer(i)->getVertices();
      irr::u16* sourceIndices = toonMesh->getMeshBuffer(i)->getIndices();
      vertCombine* combiners = new vertCombine[toonMesh->getMeshBuffer(i)->getVertexCount()];
      irr::core::array<indexRedirect> redirects;
      bool *check = new bool[toonMesh->getMeshBuffer(i)->getVertexCount()];
      for (int j = 0; j < toonMesh->getMeshBuffer(i)->getVertexCount(); j++) {
         check[j] = true;
         combiners[j].totalPos = sourceVertices[j].Pos;
         combiners[j].totalNormal = sourceVertices[j].Normal;
         combiners[j].count = 1;
      }
      for (int j = 0; j < toonMesh->getMeshBuffer(i)->getVertexCount(); j++) {
         if (check[j]) {
            // Check for vertices that are very close
            for (int k = j; k < toonMesh->getMeshBuffer(i)->getVertexCount(); k++) {
               if (check[k]) {
                  if (sourceVertices[j].Pos.getDistanceFromSQ(sourceVertices[k].Pos) <= irr::core::ROUNDING_ERROR_f32*irr::core::ROUNDING_ERROR_f32) {
                     check[k] = false;
                     indexRedirect redir;
                     redir.newIndex = j;
                     redir.oldIndex = k;
                     redirects.push_back(redir);
                  }
               }
            }
         }
      }
      delete [] check;

      // Combine vertices
      for (int j = 0; j < redirects.size(); j++) {
         combiners[redirects[j].newIndex].totalPos += sourceVertices[redirects[j].oldIndex].Pos;
         combiners[redirects[j].newIndex].totalNormal += sourceVertices[redirects[j].oldIndex].Normal;
         combiners[redirects[j].newIndex].count++;
      }
      for (int j = 0; j < redirects.size(); j++) {
         sourceVertices[redirects[j].newIndex].Pos = combiners[redirects[j].newIndex].totalPos / combiners[redirects[j].newIndex].count;
         sourceVertices[redirects[j].newIndex].Normal = combiners[redirects[j].newIndex].totalNormal / combiners[redirects[j].newIndex].count;
         sourceVertices[redirects[j].newIndex].Normal.normalize();
      }
      // After combining the vertices, fix indices so they no longer reference dead vertices
      for (int j = 0; j < redirects.size(); j++) {
         for (int k = 0; k < toonMesh->getMeshBuffer(i)->getIndexCount(); k++) {
            if (sourceIndices[k] == redirects[j].oldIndex) {
               sourceIndices[k] = redirects[j].newIndex;
            }
         }
      }
      delete [] combiners;

      // Now expand the mesh, moving the vertices along their normals
      smgr->getMeshManipulator()->setVertexColors(toonMesh,color);
      for (int j = 0; j < toonMesh->getMeshBuffer(i)->getVertexCount(); j++) {
         sourceVertices[j].Color = color;
         sourceVertices[j].Pos += sourceVertices[j].Normal*size;
      }

      toonMesh->getMeshBuffer(i)->recalculateBoundingBox();

      // Now set the material
      toonMesh->getMeshBuffer(i)->getMaterial().Lighting = false;
      toonMesh->getMeshBuffer(i)->getMaterial().BackfaceCulling = false;
      toonMesh->getMeshBuffer(i)->getMaterial().FrontfaceCulling = true;
      toonMesh->getMeshBuffer(i)->getMaterial().DiffuseColor = color;
      toonMesh->getMeshBuffer(i)->getMaterial().ColorMaterial = irr::video::ECM_DIFFUSE_AND_AMBIENT;
      toonMesh->getMeshBuffer(i)->getMaterial().TextureLayer[0].Texture = 0;

   }

   toonMesh->recalculateBoundingBox();
   return (irr::scene::IMesh*)toonMesh;
}

#endif
