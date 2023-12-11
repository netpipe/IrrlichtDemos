
#include <irrlicht.h>

using namespace std;

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


int main(int argc, char *argv[])
{
   IrrlichtDevice *device = createDevice(video::EDT_OPENGL, dimension2d<s32>(800,600), 32, false, false, false);
   if ( device == 0 ) return 1;

   IVideoDriver* driver = device->getVideoDriver();
   ISceneManager* smgr = device->getSceneManager();
   IGUIEnvironment* guienv = device->getGUIEnvironment();

 //  driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);

   IGUIStaticText* fpstext = guienv->addStaticText(L"", rect<s32>(0,0,700,15), true, false, 0, -1, true);
   IGUIFont* font = guienv->getFont("fonthaettenschweiler.bmp");
   IGUISkin* skin = guienv->getSkin();
   if (font) skin->setFont(font);

    scene::ICameraSceneNode* MMOCam = smgr->addCameraSceneNodeFPS();
   MMOCam->setFarValue(1000000.0f);
   MMOCam->setNearValue(0.1f);
   MMOCam->setPosition(core::vector3df(0,120,0));
   MMOCam->setTarget(core::vector3df(0,100,100));


#define PERFACE
    bool bWIREFRAME = 0;

    scene::IAnimatedMesh* terrmesh1 = smgr->getMesh("sphere.obj");

    CMeshBuffer<S3DVertex>* buffer=(CMeshBuffer<S3DVertex>*)terrmesh1->getMeshBuffer(0);
    printf("Vertex Count: %u\n",buffer->getVertexCount());
    printf("Index Count: %u\n\n",buffer->getIndexCount());
    printf("Total Triangles Count: %u\n\n",buffer->getIndexCount()/3);

    u16* indices = buffer->getIndices();
    void* vertices = buffer->getVertices();
    S3DVertex* vertex = (S3DVertex *) vertices;
    s32 indexc = buffer->getIndexCount();
    u16* indicesc = (u16* )malloc(sizeof(short int)*buffer->getIndexCount());
    vector3df* normals = (vector3df *)malloc(sizeof(vector3df)*buffer->getIndexCount()/3);
    vector3df* vertpnt = (vector3df *)malloc(sizeof(vector3df)*buffer->getIndexCount()/3);
    triangle3df poly;

    //precalculated faces normals and precalculated 1 point of intersection: normals/vertpnt
    memcpy(indicesc, indices, sizeof(short int)*buffer->getIndexCount());
    for(s32 i=0;i<indexc;i+=3) {
          poly.pointA = vertex[indicesc[i]].Pos;
          poly.pointB = vertex[indicesc[i+1]].Pos;
          poly.pointC = vertex[indicesc[i+2]].Pos;
          normals[i/3] = poly.getNormal().normalize();
          vertpnt[i/3] = vertex[indicesc[i]].Pos;
          //printf("%d",indicesc[i]);
          //printf(" %d",indicesc[i+1]);
          //printf(" %d\n",indicesc[i+2]);
    }

    video::SMaterial material;
    material.Lighting=false;
    material.Wireframe=bWIREFRAME;
    material.BackfaceCulling=true;
    ITexture* texture = driver->getTexture("worldtopo-5400x2700a.jpg");
    material.setTexture(0, texture);

    // scene::ILightSceneNode* light1 = smgr->addLightSceneNode(0,vector3df(0,500,0),video::SColorf(1.0f,1.0f,1.0f,0.0f),10000,-1);

    int m=0,k=0;

    u32 t1,t2,dt=0;

    #ifndef PERFACE
    //uncomment this and comment out PerFaceCulling to test performance
    scene::ISceneNode* terrnode1 = smgr->addAnimatedMeshSceneNode(terrmesh1);
    scene::IMesh *mesh = terrmesh1->getMesh(0);
    mesh->getMeshBuffer(0)->setHardwareMappingHint(scene::EHM_STATIC);

    //terrnode1 = smgr->addMeshSceneNode(mesh);  //some reason this is only half the mesh

    terrnode1->setMaterialTexture(0, driver->getTexture("worldtopo-5400x2700a.jpg"));
    terrnode1->setMaterialFlag(video::EMF_WIREFRAME, bWIREFRAME);
    terrnode1->setMaterialFlag(video::EMF_BACK_FACE_CULLING, true);
    terrnode1->setMaterialFlag(video::EMF_LIGHTING, false);
    terrnode1->setPosition(core::vector3df(0,0,0));
    terrnode1->setVisible(true);
    #endif

   device->getCursorControl()->setVisible(false);
   vector3df campos, b;
   u32 c1,pos;
   stringw str;
   c1=0;
   pos=0;
   s32 i;
   indexc/=3;

   while(device->run())
   {
      str = L"FPS: ";str += driver->getFPS();
      str += " TRI:";str += driver->getPrimitiveCountDrawn();
      //comment out the following lines for better performance--start commenting
      //str += " Total Triangles: ";str += m;
      //str += " Visible Triangles: ";str += k;
      //str += " Not Visible Triangles: ";str += m-k;
      //str += " Index Count: ";str += k*3;
      //str += " dt:";str += dt;
      //--end commenting
      fpstext->setText(str.c_str());
      //m = k = 0;

      campos = MMOCam->getPosition();
      driver->setMaterial(material);
      driver->setTransform(video::ETS_WORLD, core::matrix4());

      driver->beginScene(true, true, 0);

#ifdef PERFACE
// PER FACE CULLING - START
            driver->drawMeshBuffer(buffer);

            //t1 = device->getTimer()->getRealTime();
            buffer->Indices.erase(0, buffer->getIndexCount());
            //buffer->Indices.set_used(indexc);
            for(i=0;i<indexc;i++) {
               //m++;
               if ( normals[i].dotProduct(vertpnt[i] - campos ) < 0 ) {
                  buffer->Indices.push_back(indicesc[i*3]);
                  buffer->Indices.push_back(indicesc[i*3+1]);
                  buffer->Indices.push_back(indicesc[i*3+2]);
                  k++;
                  c1++;
               }
               else {
                  if (c1!=0) {
                     c1*=3;
                     pos+=c1;
                     buffer->Indices.set_used(pos);
                     memcpy(&indices[pos-c1], &indicesc[i*3-c1], c1*2);
                     c1=0;
                  }
               }
            }
            buffer->Indices.set_used(pos);
            pos=0;

            //t2 = device->getTimer()->getRealTime();
            //dt=t2-t1;
            //printf("%d\n",dt);
// PER FACE CULLING - END
#endif
         smgr->drawAll();
         guienv->drawAll();
      driver->endScene();
   }
   device->drop();
//   return 0;
}
