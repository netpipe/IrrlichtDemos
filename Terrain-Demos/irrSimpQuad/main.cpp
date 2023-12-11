//#TODO
// add multiple terrain tiles into the cube mesh buffer somehow.



#include "irrlicht.h"
#include <math.h>
#include <sstream>
using namespace irr;
using namespace scene;
using namespace core;
#include "simplexnoise1234.h"
#define LEAF_TYPE 0
#define NODE_TYPE 1
#define TILETHRESHOLD 10.0f

SMeshBuffer * pNode1 = new SMeshBuffer();
SMeshBuffer * glvertex = new SMeshBuffer();
float 	     *fVerts;
unsigned int *uiList;

class NODE
{
public:
    int          bType;
    core::vector3df	     vBoundingCoordinates[4];
    unsigned int uiBranches[4];
    unsigned int uiID;
    unsigned int uiParentID;
    /*
    	unsigned int uiVertexStrip1[8]; // 4 vertex per quad
    	unsigned int uiVertexStrip2[8];
    	unsigned int uiVertexStrip3[8];
    	unsigned int uiVertexStrip4[8];
    */
};

NODE* pNodeList;

float getHeight(float x, float z)
{

    int octaves=4;
    float persistance =2;
    int scale=7;  // noise scaler, not the same as patch scaler
    float total = 0;

    //amplify?
    int ax=0;
    int az=0;

    for (int i=3; i < octaves; i++)
    {
        float frequency = pow(2.0f, i);
        float amplitude = pow(persistance, i);
        total += SimplexNoise1234::noise(((x+ax)/scale)/ frequency, ((z+az)/scale)/ frequency) * amplitude;
    }

    return (total);
};

void CreateNode(unsigned int Bounding[4],unsigned int ParentID,unsigned int NodeID)
{

    static unsigned int TotalTreeID = 0;
    unsigned int uiNodeType;
    float uiWidth,uiHeight;

    uiWidth = fVerts[(Bounding[1]*3)] - fVerts[(Bounding[0]*3)];
    uiHeight = fVerts[(Bounding[2]*3)+2] - fVerts[(Bounding[0]*3)+2];

    if ( 0.5*uiWidth==2 ){ uiNodeType = LEAF_TYPE; }else{ uiNodeType = NODE_TYPE; }

    NODE *pNode = &pNodeList[NodeID];
    pNode->uiID = NodeID;
    pNode->uiParentID = ParentID;

    for (int i=0; i < 4; i++)   		//!parrent NODE uiBounding aka texture coords
    {
        pNode->vBoundingCoordinates[i].X = fVerts[(Bounding[i]*3)];
        pNode->vBoundingCoordinates[i].Y = fVerts[(Bounding[i]*3)+1];
        pNode->vBoundingCoordinates[i].Z = fVerts[(Bounding[i]*3)+2];
    }

    pNode->bType = uiNodeType;
    if (uiNodeType == LEAF_TYPE){return;}
    else {
        unsigned int BoundingBox[4];
        TotalTreeID++;
        pNode->uiBranches[0] = TotalTreeID;
        BoundingBox[0] = Bounding[0]; //Top-Left i.e. b[0]
        BoundingBox[1] = Bounding[0]+((Bounding[1]-Bounding[0])/2); //Between b[0] and b[1]
        BoundingBox[2] = Bounding[0]+((Bounding[2]-Bounding[0])/2); //[between b[0] and b[2]
        BoundingBox[3] = Bounding[0]+((Bounding[2]-Bounding[0])/2)+((Bounding[1]-Bounding[0])/2); //middle of node
        CreateNode(BoundingBox,NodeID,TotalTreeID);
//************************************************************************************************************
        TotalTreeID++;
        pNode->uiBranches[1] = TotalTreeID;
        BoundingBox[0] = Bounding[0]+((Bounding[1]-Bounding[0])/2); // Between b[0] and b[1]
        BoundingBox[1] = Bounding[1]; //Top-Right i.e.  b[1]
        BoundingBox[2] = Bounding[0]+((Bounding[2]-Bounding[0])/2)+((Bounding[1]-Bounding[0])/2); //middle of node
        BoundingBox[3] = Bounding[0]+((Bounding[2]-Bounding[0])/2)+((Bounding[1]-Bounding[0])); //between b[1] & b[3]
        CreateNode(BoundingBox,NodeID,TotalTreeID);
//************************************************************************************************************
        TotalTreeID++;
        pNode->uiBranches[2] = TotalTreeID;
        BoundingBox[0] = Bounding[0]+((Bounding[2]-Bounding[0])/2); //between b[0] and b[2]
        BoundingBox[1] = Bounding[0]+((Bounding[2]-Bounding[0])/2)+((Bounding[1]-Bounding[0])/2); //middle of node
        BoundingBox[2] = Bounding[2]; //Bottom-Left i.e. b[2]
        BoundingBox[3] = Bounding[2]+((Bounding[3]-Bounding[2])/2); //between b[2] and b[3]
        CreateNode(BoundingBox,NodeID,TotalTreeID);
//************************************************************************************************************
        TotalTreeID++;
        pNode->uiBranches[3] = TotalTreeID;
        BoundingBox[0] = Bounding[0]+((Bounding[2]-Bounding[0])/2)+((Bounding[1]-Bounding[0])/2); //middle of node
        BoundingBox[1] = Bounding[0]+((Bounding[2]-Bounding[0])/2) + int(uiWidth); //between b[1] and b[3]
        BoundingBox[2] = Bounding[2]+((Bounding[3]-Bounding[2])/2); //between b[2] and b[3]
        BoundingBox[3] = Bounding[3]; //Bottom-Right i.e. b[3]
        CreateNode(BoundingBox,NodeID,TotalTreeID);
    }
}

void generateLod(unsigned int LOD, NODE* pNode, const int TN ,const int XT, const int TZ)
{

    unsigned int count=1;
    unsigned int element=0;
   // LOD=3;
 if (LOD==0){
    for (unsigned int x=0; x < 4 ; x++)
    {
        //get the bounding coords
        unsigned int z=pNode[0].uiBranches[x];
        glvertex->Vertices.push_back(video::S3DVertex(
                                         pNode[z].vBoundingCoordinates[3].X+XT,
                                         getHeight(pNode[z].vBoundingCoordinates[3].X+XT,
                                                   pNode[z].vBoundingCoordinates[3].Z+TZ),
                                         pNode[z].vBoundingCoordinates[3].Z+TZ,
                                         0.0f, 1.0f, 0.0f, video::SColor(0xffffffff),
                                         0.0f, 0.0f));
        glvertex->Indices.push_back(element);
        element++;
        glvertex->Vertices.push_back(video::S3DVertex(
                                         pNode[z].vBoundingCoordinates[1].X+XT,
                                         getHeight(pNode[z].vBoundingCoordinates[1].X+XT,
                                                   pNode[z].vBoundingCoordinates[1].Z+TZ),
                                         pNode[z].vBoundingCoordinates[1].Z+TZ,
                                         0.0f, 1.0f, 0.0f, video::SColor(0xffffffff),
                                         1.0f, 1.0f));
        glvertex->Indices.push_back(element);
        element++;
        glvertex->Vertices.push_back(video::S3DVertex(
                                         pNode[z].vBoundingCoordinates[0].X+XT,
                                         getHeight(pNode[z].vBoundingCoordinates[0].X+XT,
                                                   pNode[z].vBoundingCoordinates[0].Z+TZ),
                                         pNode[z].vBoundingCoordinates[0].Z+TZ,
                                         0.0f, 1.0f, 0.0f, video::SColor(0xffffffff),
                                         1.0f, 0.0f));
        glvertex->Indices.push_back(element);
        element++;
        glvertex->Vertices.push_back(video::S3DVertex(
                                         pNode[z].vBoundingCoordinates[2].X+XT,
                                         getHeight(pNode[z].vBoundingCoordinates[2].X+XT,
                                                   pNode[z].vBoundingCoordinates[2].Z+TZ),
                                         pNode[z].vBoundingCoordinates[2].Z+TZ,
                                         0.0f, 1.0f, 0.0f, video::SColor(0xffffffff),
                                         0.0f, 1.0f));
        glvertex->Indices.push_back(element);
        element++;
        glvertex->Vertices.push_back(video::S3DVertex(
                                         pNode[z].vBoundingCoordinates[3].X+XT,
                                         getHeight(pNode[z].vBoundingCoordinates[3].X+XT,
                                                   pNode[z].vBoundingCoordinates[3].Z+TZ),
                                         pNode[z].vBoundingCoordinates[3].Z+TZ,
                                         0.0f, 1.0f, 0.0f, video::SColor(0xffffffff),
                                         0.0f, 0.0f));
        glvertex->Indices.push_back(element);
        element++;
        glvertex->Vertices.push_back(video::S3DVertex(
                                         pNode[z].vBoundingCoordinates[0].X+XT,
                                         getHeight(pNode[z].vBoundingCoordinates[0].X+XT,
                                                   pNode[z].vBoundingCoordinates[0].Z+TZ),
                                         pNode[z].vBoundingCoordinates[0].Z+TZ,
                                         0.0f, 1.0f, 0.0f, video::SColor(0xffffffff),
                                         1.0f, 0.0f));
        glvertex->Indices.push_back(element);
        element++;
    }
       return;
 }else if (LOD==1){
	count=1;

		for (unsigned int n=0; n < 4; n++){ // Unit this is dangerous, N is uninitialized

		 for (unsigned int x=0; x < 4; x++) {
			unsigned int z=pNode[count].uiBranches[x];

        glvertex->Vertices.push_back(video::S3DVertex(
                                         pNode[z].vBoundingCoordinates[3].X+XT,
                                         getHeight(pNode[z].vBoundingCoordinates[3].X+XT,
                                                   pNode[z].vBoundingCoordinates[3].Z+TZ),
                                         pNode[z].vBoundingCoordinates[3].Z+TZ,
                                         0.0f, 1.0f, 0.0f, video::SColor(0xffffffff),
                                         0.0f, 0.0f));
        glvertex->Indices.push_back(element);
        element++;
        glvertex->Vertices.push_back(video::S3DVertex(
                                         pNode[z].vBoundingCoordinates[1].X+XT,
                                         getHeight(pNode[z].vBoundingCoordinates[1].X+XT,
                                                   pNode[z].vBoundingCoordinates[1].Z+TZ),
                                         pNode[z].vBoundingCoordinates[1].Z+TZ,
                                         0.0f, 1.0f, 0.0f, video::SColor(0xffffffff),
                                         1.0f, 1.0f));
        glvertex->Indices.push_back(element);
        element++;
        glvertex->Vertices.push_back(video::S3DVertex(
                                         pNode[z].vBoundingCoordinates[0].X+XT,
                                         getHeight(pNode[z].vBoundingCoordinates[0].X+XT,
                                                   pNode[z].vBoundingCoordinates[0].Z+TZ),
                                         pNode[z].vBoundingCoordinates[0].Z+TZ,
                                         0.0f, 1.0f, 0.0f, video::SColor(0xffffffff),
                                         1.0f, 0.0f));
        glvertex->Indices.push_back(element);
        element++;
        glvertex->Vertices.push_back(video::S3DVertex(
                                         pNode[z].vBoundingCoordinates[2].X+XT,
                                         getHeight(pNode[z].vBoundingCoordinates[2].X+XT,
                                                   pNode[z].vBoundingCoordinates[2].Z+TZ),
                                         pNode[z].vBoundingCoordinates[2].Z+TZ,
                                         0.0f, 1.0f, 0.0f, video::SColor(0xffffffff),
                                         0.0f, 1.0f));
        glvertex->Indices.push_back(element);
        element++;
        glvertex->Vertices.push_back(video::S3DVertex(
                                         pNode[z].vBoundingCoordinates[3].X+XT,
                                         getHeight(pNode[z].vBoundingCoordinates[3].X+XT,
                                                   pNode[z].vBoundingCoordinates[3].Z+TZ),
                                         pNode[z].vBoundingCoordinates[3].Z+TZ,
                                         0.0f, 1.0f, 0.0f, video::SColor(0xffffffff),
                                         0.0f, 0.0f));
        glvertex->Indices.push_back(element);
        element++;
        glvertex->Vertices.push_back(video::S3DVertex(
                                         pNode[z].vBoundingCoordinates[0].X+XT,
                                         getHeight(pNode[z].vBoundingCoordinates[0].X+XT,
                                                   pNode[z].vBoundingCoordinates[0].Z+TZ),
                                         pNode[z].vBoundingCoordinates[0].Z+TZ,
                                         0.0f, 1.0f, 0.0f, video::SColor(0xffffffff),
                                         1.0f, 0.0f));
        glvertex->Indices.push_back(element);
        element++;

		 }
		 count+=85;
		}
	return;
 }else if (LOD==2){
		count=2;
		for (unsigned int n=0; n < 4; n++){
		 for (unsigned int g=0; g < 4; g++){
		  for (unsigned int x=0; x < 4 ; x++) {
			unsigned int z=pNode[(count+n)].uiBranches[x];
	        glvertex->Vertices.push_back(video::S3DVertex(
                                         pNode[z].vBoundingCoordinates[3].X+XT,
                                         getHeight(pNode[z].vBoundingCoordinates[3].X+XT,
                                                   pNode[z].vBoundingCoordinates[3].Z+TZ),
                                         pNode[z].vBoundingCoordinates[3].Z+TZ,
                                         0.0f, 1.0f, 0.0f, video::SColor(0xffffffff),
                                         0.0f, 0.0f));
        glvertex->Indices.push_back(element);
        element++;
        glvertex->Vertices.push_back(video::S3DVertex(
                                         pNode[z].vBoundingCoordinates[1].X+XT,
                                         getHeight(pNode[z].vBoundingCoordinates[1].X+XT,
                                                   pNode[z].vBoundingCoordinates[1].Z+TZ),
                                         pNode[z].vBoundingCoordinates[1].Z+TZ,
                                         0.0f, 1.0f, 0.0f, video::SColor(0xffffffff),
                                         1.0f, 1.0f));
        glvertex->Indices.push_back(element);
        element++;
        glvertex->Vertices.push_back(video::S3DVertex(
                                         pNode[z].vBoundingCoordinates[0].X+XT,
                                         getHeight(pNode[z].vBoundingCoordinates[0].X+XT,
                                                   pNode[z].vBoundingCoordinates[0].Z+TZ),
                                         pNode[z].vBoundingCoordinates[0].Z+TZ,
                                         0.0f, 1.0f, 0.0f, video::SColor(255,33,33,255),
                                         1.0f, 0.0f));
        glvertex->Indices.push_back(element);
        element++;
        glvertex->Vertices.push_back(video::S3DVertex(
                                         pNode[z].vBoundingCoordinates[2].X+XT,
                                         getHeight(pNode[z].vBoundingCoordinates[2].X+XT,
                                                   pNode[z].vBoundingCoordinates[2].Z+TZ),
                                         pNode[z].vBoundingCoordinates[2].Z+TZ,
                                         0.0f, 1.0f, 0.0f, video::SColor(0xffffffff),
                                         0.0f, 1.0f));
        glvertex->Indices.push_back(element);
        element++;
        glvertex->Vertices.push_back(video::S3DVertex(
                                         pNode[z].vBoundingCoordinates[3].X+XT,
                                         getHeight(pNode[z].vBoundingCoordinates[3].X+XT,
                                                   pNode[z].vBoundingCoordinates[3].Z+TZ),
                                         pNode[z].vBoundingCoordinates[3].Z+TZ,
                                         0.0f, 1.0f, 0.0f
                                         , video::SColor(0xffffffff),

                                         0.0f, 0.0f));
        glvertex->Indices.push_back(element);
        element++;
        glvertex->Vertices.push_back(video::S3DVertex(
                                         pNode[z].vBoundingCoordinates[0].X+XT,
                                         getHeight(pNode[z].vBoundingCoordinates[0].X+XT,
                                                   pNode[z].vBoundingCoordinates[0].Z+TZ),
                                         pNode[z].vBoundingCoordinates[0].Z+TZ,
                                         0.0f, 1.0f, 0.0f, video::SColor(0xffffffff),
                                         1.0f, 0.0f));
        glvertex->Indices.push_back(element);
        element++;
		   }
		   count+=21;
		  }
		}
	return;
 }else if(LOD==3){
		count=3;
		for (unsigned int n=0; n < 4; n++){
		 for (unsigned int d=0; d < 4; d++){
		  for (unsigned int g=0; g < 4; g++){
		   for (unsigned int x=0; x < 4 ; x++){
			unsigned int z=pNode[(count+d)].uiBranches[x];

	        glvertex->Vertices.push_back(video::S3DVertex(
                                         pNode[z].vBoundingCoordinates[3].X+XT,
                                         getHeight(pNode[z].vBoundingCoordinates[3].X+XT,
                                                   pNode[z].vBoundingCoordinates[3].Z+TZ),
                                         pNode[z].vBoundingCoordinates[3].Z+TZ,
                                         0.0f, 1.0f, 0.0f, video::SColor(0xffffffff),
                                         0.0f, 0.0f));
        glvertex->Indices.push_back(element);
        element++;
        glvertex->Vertices.push_back(video::S3DVertex(
                                         pNode[z].vBoundingCoordinates[1].X+XT,
                                         getHeight(pNode[z].vBoundingCoordinates[1].X+XT,
                                                   pNode[z].vBoundingCoordinates[1].Z+TZ),
                                         pNode[z].vBoundingCoordinates[1].Z+TZ,
                                         0.0f, 1.0f, 0.0f, video::SColor(255,33,33,255),
                                         1.0f, 1.0f));
        glvertex->Indices.push_back(element);
        element++;
        glvertex->Vertices.push_back(video::S3DVertex(
                                         pNode[z].vBoundingCoordinates[0].X+XT,
                                         getHeight(pNode[z].vBoundingCoordinates[0].X+XT,
                                                   pNode[z].vBoundingCoordinates[0].Z+TZ),
                                         pNode[z].vBoundingCoordinates[0].Z+TZ,
                                         0.0f, 1.0f, 0.0f, video::SColor(0xffffffff),
                                         1.0f, 0.0f));
        glvertex->Indices.push_back(element);
        element++;
        glvertex->Vertices.push_back(video::S3DVertex(
                                         pNode[z].vBoundingCoordinates[2].X+XT,
                                         getHeight(pNode[z].vBoundingCoordinates[2].X+XT,
                                                   pNode[z].vBoundingCoordinates[2].Z+TZ),
                                         pNode[z].vBoundingCoordinates[2].Z+TZ,
                                         0.0f, 1.0f, 0.0f, video::SColor(0xffffffff),
                                         0.0f, 1.0f));
        glvertex->Indices.push_back(element);
        element++;
        glvertex->Vertices.push_back(video::S3DVertex(
                                         pNode[z].vBoundingCoordinates[3].X+XT,
                                         getHeight(pNode[z].vBoundingCoordinates[3].X+XT,
                                                   pNode[z].vBoundingCoordinates[3].Z+TZ),
                                         pNode[z].vBoundingCoordinates[3].Z+TZ,
                                         0.0f, 1.0f, 0.0f, video::SColor(0xffffffff),
                                         0.0f, 0.0f));
        glvertex->Indices.push_back(element);
        element++;
        glvertex->Vertices.push_back(video::S3DVertex(
                                         pNode[z].vBoundingCoordinates[0].X+XT,
                                         getHeight(pNode[z].vBoundingCoordinates[0].X+XT,
                                                   pNode[z].vBoundingCoordinates[0].Z+TZ),
                                         pNode[z].vBoundingCoordinates[0].Z+TZ,
                                         0.0f, 1.0f, 0.0f, video::SColor(0xffffffff),
                                         1.0f, 0.0f));
        glvertex->Indices.push_back(element);
        element++;

		    };
		    count+=5;
		   };
		  };
		 count+=5;
		};

 }; // end of if
}

void GenerateNode ( NODE *pNode, const int &TN,const int &XT,const int &TZ, const unsigned int &LOD )
{
    generateLod ( 1, pNode, TN, XT, TZ );
}


void CreateGrid(unsigned int w,unsigned int h)
{
    unsigned int total=0;

    unsigned int width = w;
    unsigned int height = h;

    fVerts = (float*)malloc((sizeof(float))*width*height*3); /* Allocate space for 											 vertices with an x,y and z*/

    for (unsigned int ctr=0;ctr<height;ctr++)
    {
        for (unsigned int ctr2=0;ctr2<width;ctr2++)
        {
            // Vertex number is total
            fVerts[total] =  ctr2;	// Vertex X component is ctr2
            fVerts[total+1] = 0;	// Vertex Y component is height field (0)
            fVerts[total+2] =  ctr;  // Vertex Z component is ctr
            total+=3;		// Increase total to be next vertex number
        }
    }
    total=0;

}

unsigned int CalcNodeNum(unsigned int max,unsigned int min)
{

    int ctr=0;
    int var = 0;
    while (var==0)
    {
        ctr+=max;
        max=max/min;

        if (max==1)
        {
            var=1;
        }
    }
    ctr++;
    return ctr;
}

int main()
{

IrrlichtDevice *device =createDevice(video::EDT_OPENGL, core::dimension2d<s32>(640, 480), 16, false);
    device->setWindowCaption(L"QuadTree + SIMPLEX - Irrlicht Engine Demo");
    video::IVideoDriver* driver = device->getVideoDriver();
    scene::ISceneManager* smgr = device->getSceneManager();
    //smgr->addCameraSceneNodeMaya();
    smgr->addCameraSceneNodeFPS();

    scene::ICameraSceneNode* camera = smgr->getActiveCamera();

    camera->setFarValue(22000);
    camera->setPosition(core::vector3df(-500,100,-500));
		smgr->addLightSceneNode(NULL, core::vector3df(0,50,0), video::SColorf(255.0f, 255.0f, 255.0f), 500, -1);

//!Start of Terrain Init
    CreateGrid ( 65,65 ); // width and height + 1
    unsigned int Temp = ( 64/4 ) * ( 64/4 ); // devided into 4 sections ??
    unsigned int Temp2 = CalcNodeNum ( Temp,4 ); // max , min
    //!	{0,GridWidth,(GridHeight*(GridWidth+1)),((GridHeight)*(GridWidth+1))+GridWidth};
    unsigned int uiBoundingCoordinates[] = {0,64, ( 64* ( 65 ) ), ( ( 64 ) * ( 65 ) ) +64};

    // 		pNodeList = new NODE[Temp2];
    pNodeList = ( NODE* ) malloc ( sizeof ( NODE ) *Temp2 );
    CreateNode ( uiBoundingCoordinates,0,0 );
    free ( fVerts );


    generateLod ( 3, pNodeList , 1, 1, 1 );
    //only one is shown.
    generateLod ( 3, pNodeList , 1, 61, 61 );
    f32 cubeSize = 50.f;
    SMesh * cubeMesh = new SMesh();

   // video::SColor cubeColour(255,255,255,255);
    glvertex->BoundingBox.reset(0,0,0);

    for (int i=0; i<glvertex->getVertexCount(); ++i)
    {
        glvertex->Vertices[i].Pos -= core::vector3df(0.5f, 0.5f, 0.5f);
        glvertex->Vertices[i].Pos *= cubeSize;
        glvertex->BoundingBox.addInternalPoint(glvertex->Vertices[i].Pos);
    }


    cubeMesh->addMeshBuffer(glvertex);
    cubeMesh->recalculateBoundingBox();
    cubeMesh->setHardwareMappingHint(EHM_STATIC);  // VBO

    ISceneNode * cubeSceneNode = smgr->addMeshSceneNode(cubeMesh);

    cubeSceneNode->setMaterialTexture(0, driver->getTexture("terrain.jpg"));
  //  cubeSceneNode->setScale(irr::core::vector3df(10,5,10));
	u32 frames = 0;

    while (device->run())
    {
        driver->beginScene(true, true, video::SColor(0,100,100,100));
        smgr->drawAll();
        driver->endScene();

        if (++frames==100)
		{
			core::stringw str = L"Irrlicht Engine [";
			str += driver->getName();
			str += L"] FPS: ";
			str += (s32)driver->getFPS();

			device->setWindowCaption(str.c_str());
			frames=0;
		}
     //     device->sleep(10,0);
    }
    free(uiList);
    device->drop();
    return 0;
}
