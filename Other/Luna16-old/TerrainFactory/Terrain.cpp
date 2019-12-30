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
#include "Terrain.h"

#include "ProceduralTrees/CTreeGenerator.h"
#include "ProceduralTrees/CBillboardGroupSceneNode.h"
#include "ProceduralTrees/CTreeSceneNode.h"
#include <vector>
using namespace std;

   SMeshBuffer* pNode1 = new SMeshBuffer();
   SMeshBuffer* glvertex = new SMeshBuffer();

Terrain::Terrain(){};

void Terrain::Init(){

     driver = m_irrDevice->getVideoDriver();
    smgr = m_irrDevice->getSceneManager();
    scene::ICameraSceneNode* camera = smgr->getActiveCamera();

};

Terrain::~Terrain(){
    //   for ( s32 i=0; i<NUM_TREE_DESIGNS; i++ )
    // {
    //   treeDesigns[i].Generator->drop();
    //}
 };

float Terrain::getHeight(float x, float z)
{

    int octaves=5;
    float persistance =1;
    int scale=3;  // noise scaler, not the same as patch scaler
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




int Terrain::Render( vector3df terrainPosition)
{

    //INIT VARS
    vector3df terrainScale = vector3df(.1,.1,.1);
   // vector3df terrainPosition = vector3df(0,-1.0f,0);


 //       camera->setFarValue(5200);
        //camera->setPosition(core::vector3df(-500,100,-500));
 //   smgr->addLightSceneNode(NULL, core::vector3df(0,50,0), video::SColorf(255.0f, 255.0f, 255.0f), 500, -1);


    CreateGrid ( 65,65 ); // width and height + 1
    unsigned int Temp = ( 64/4 ) * ( 64/4 ); // devided into 4 sections ??
    unsigned int Temp2 = CalcNodeNum ( Temp,4 ); // max , min
    //!	{0,GridWidth,(GridHeight*(GridWidth+1)),((GridHeight)*(GridWidth+1))+GridWidth};
    unsigned int uiBoundingCoordinates[] = {0,64, ( 64* ( 65 ) ), ( ( 64 ) * ( 65 ) ) +64};

    // 		pNodeList = new NODE[Temp2];
    pNodeList = ( NODE* ) malloc ( sizeof ( NODE ) *Temp2 );
    CreateNode ( uiBoundingCoordinates,0,0 );
    free ( fVerts );
    generateLod ( 3, pNodeList , 1, -10, -10 );
    f32 cubeSize = 50.f;

    // video::SColor cubeColour(255,255,255,255);
    glvertex->BoundingBox.reset(0,0,0);

    for (int i=0; i<glvertex->getVertexCount(); ++i)
    {
        glvertex->Vertices[i].Pos -= core::vector3df(0.5f, 0.5f, 0.5f);
        glvertex->Vertices[i].Pos *= cubeSize;
        glvertex->BoundingBox.addInternalPoint(glvertex->Vertices[i].Pos);
    }

    cubeMesh = new SMesh();
    cubeMesh->addMeshBuffer(glvertex);
    cubeMesh->recalculateBoundingBox();
    cubeSceneNode = smgr->addMeshSceneNode(cubeMesh);
    cubeSceneNode->setMaterialTexture(0, driver->getTexture("data/textures/detailmaps/rock.png"));

  //  free(uiList);  // causes crashing

    tr.setIdentity();
    tr.setOrigin(btVector3( terrainPosition.X,  terrainPosition.Y,  terrainPosition.Z));

    cubeSceneNode->setMaterialType(EMT_DETAIL_MAP);
    cubeSceneNode->setMaterialFlag(EMF_LIGHTING, true);
    //! POSITION
    cubeSceneNode->setScale(terrainScale);
    cubeSceneNode->setPosition(terrainPosition);
    //m_cScene->setGenericMaterial(cubeSceneNode, 0);

    IMeshBuffer *meshBuffer = cubeMesh->getMeshBuffer(0);
    btTriangleMesh *collisionMesh = new btTriangleMesh();
    m_cPhysics->convertIrrMeshBufferBtTriangleMesh(meshBuffer, collisionMesh, terrainScale);
    btBvhTriangleMeshShape *trackShape = new btBvhTriangleMeshShape(collisionMesh, true);

    localCreateRigidBody(0, tr, trackShape, cubeSceneNode);

    return 0;
}


void Terrain::CreateNode(unsigned int Bounding[4],unsigned int ParentID,unsigned int NodeID)
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

void Terrain::generateLod(unsigned int LOD, NODE* pNode, const int TN ,const int XT, const int TZ)
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


void Terrain::GenerateNode ( NODE *pNode, const int &TN,const int &XT,const int &TZ, const unsigned int &LOD )
{
    generateLod ( 1, pNode, TN, XT, TZ );
}


void Terrain::CreateGrid(unsigned int w,unsigned int h)
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


unsigned int Terrain::CalcNodeNum(unsigned int max,unsigned int min)
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





btRigidBody*	Terrain::localCreateRigidBody(float mass, const btTransform& startTransform,btCollisionShape* shape, ISceneNode *node)
{
    bool isDynamic = (mass != 0.f);

    btVector3 localInertia(0,0,0);
    btRigidBody* body = NULL;

    if (isDynamic)
    {
        shape->calculateLocalInertia(mass,localInertia);
        btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
        body = new btRigidBody(mass, myMotionState, shape, localInertia);
    }
    else
    {
        body = new btRigidBody(mass,0,shape,localInertia);
    }

    body->setWorldTransform(startTransform);
    body->setUserPointer((void *)(node));

    m_cPhysics->getDynamicsWorld()->addRigidBody(body);
    m_cPhysics->push_back(body);
    return body;
}


void Terrain::registerIrrDevice(IrrlichtDevice &device)
{
    m_irrDevice = &device;
}
void Terrain::registerPhysics(Physics &physics)
{
    m_cPhysics = &physics;
}
void Terrain::registerScene(Scene &scene)
{
    m_cScene = &scene;
}


/*
void Terrain::tick(const float &CamX, const float &CamY, const float &CamZ)
{
    SceneGraphNodes::BaseNode *activeNode = Azadi::getInstance()->getActiveNode();
    if (CamX > (extentX - TILETHRESHOLD))
    {
        for (std::deque<std::deque<SceneGraphNodes::TerrainNode*> >::iterator it = tNodes.begin(); it != tNodes.end(); ++it)
        {
            delete it->front();
            it->pop_front();
            // Generate a new Node relative to what's already here
            // SceneGraphNodes::TerrainNode *newNode = new SceneGraphNodes::TerrainNode(VBO, EBO, size);
            // Register the node with the scene graph
            // activeNode->addChild(newNode);
            // Push the new node into the back of the matrix
            // it->push_back(newNode);
        }
    }
    else if (CamX < (nextentX + TILETHRESHOLD))
    {
        for (std::deque<std::deque<SceneGraphNodes::TerrainNode*> >::iterator it = tNodes.begin(); it != tNodes.end(); ++it)
        {
            delete it->back();
            it->pop_back();
            // Generate a new Node relative to what's already here
            // SceneGraphNodes::TerrainNode *newNode = new SceneGraphNodes::TerrainNode(VBO, EBO, size);
            // Register the node with the scene graph
            // activeNode->addChild(newNode);
            // Push the new node into the front of the matrix
            // it->push_front(newNode);
        }
    }

    if (CamY > (extentY - TILETHRESHOLD))
    {
        std::deque<std::deque<SceneGraphNodes::TerrainNode*> >::iterator it = tNodes.begin();
        for (std::deque<SceneGraphNodes::TerrainNode*>::iterator ti = it->begin(); ti != it->end(); ++ti)
        {
            delete *ti;
        }
        tNodes.pop_front();
        std::deque<SceneGraphNodes::TerrainNode*> temp;
        for (unsigned long int i = 0; i < 4; ++i)
        {
            // Generate a new Node relative to what's already here
            // SceneGraphNodes::TerrainNode *newNode = new SceneGraphNodes::TerrainNode(VBO, EBO, size);
            // Register the node with the scene graph
            // activeNode->addChild(newNode);
            // Push the new node into the back of the matrix
            // temp.push_back(newNode);
        }
        tNodes.push_back(temp);
    }
    else if (CamY < (nextentY + TILETHRESHOLD))
    {
        std::deque<std::deque<SceneGraphNodes::TerrainNode*> >::iterator it = tNodes.end();
        --it;
        for (std::deque<SceneGraphNodes::TerrainNode*>::iterator ti = it->begin(); ti != it->end(); ++ti)
        {
            delete *ti;
        }
        tNodes.pop_back();
        std::deque<SceneGraphNodes::TerrainNode*> temp;
        for (unsigned long int i = 0; i < 4; ++i)
        {
            // Generate a new Node relative to what's already here
            // SceneGraphNodes::TerrainNode *newNode = new SceneGraphNodes::TerrainNode(VBO, EBO, size);
            // Register the node with the scene graph
            // activeNode->addChild(newNode);
            // Push the new node into the back of the matrix
            // temp.push_back(newNode);
        }
        tNodes.push_front(temp);
    }
}
*/


int Terrain::MakeTrees(){
CTreeGenerator* generator = 0;
    vector <CTreeSceneNode*> tree2;


    video::ITexture* billTexture = 0;
video::ITexture* leafTexture = 0;

struct STreeDesignFiles
{
    const c8* DesignFile;
    const c8* TreeTextureFile;
    const c8* LeafTextureFile;
    const c8* BillTextureFile;
};

struct STreeDesign
{
    CTreeGenerator* Generator;
    video::ITexture* TreeTexture;
    video::ITexture* LeafTexture;
    video::ITexture* BillTexture;
};

    const s32 NUM_TREE_DESIGNS = 4;

const STreeDesignFiles treeDesignFiles[NUM_TREE_DESIGNS] = {
        { "./data/trees/Oak.xml", "./data/textures/trees/OakBark.png", "./data/textures/trees/OakLeaf.png", "./data/textures/trees/OakBillboard.png" },
        { "./data/trees/Aspen.xml", "./data/textures/trees/AspenBark.bmp", "./data/textures/trees/AspenLeaf.png", "./data/textures/trees/AspenBillboard.png" },
        { "./data/trees/Pine.xml", "./data/textures/trees/PineBark.png", "./data/textures/trees/PineLeaf.png", "./data/textures/trees/PineBillboard.png" },
        { "./data/trees/Willow.xml", "./data/textures/trees/WillowBark.png", "./data/textures/trees/WillowLeaf.png", "./data/textures/trees/WillowBillboard.png" }
    };


STreeDesign treeDesigns [NUM_TREE_DESIGNS];
s32 currentTreeDesign = 0;
core::vector3df lightDir=vector3df(100.0f,110.0f,0.0f);
s32 seed = 0;



        for ( s32 i=0; i<NUM_TREE_DESIGNS; i++ )
    {
        treeDesigns[i].Generator = new CTreeGenerator( smgr );
        io::IXMLReader* xml = m_irrDevice->getFileSystem()->createXMLReader( treeDesignFiles[i].DesignFile );
        treeDesigns[i].Generator->loadFromXML( xml );
        xml->drop();
        treeDesigns[i].TreeTexture = driver->getTexture( treeDesignFiles[i].TreeTextureFile );
        treeDesigns[i].LeafTexture = driver->getTexture( treeDesignFiles[i].LeafTextureFile );
        treeDesigns[i].BillTexture = driver->getTexture( treeDesignFiles[i].BillTextureFile );
    }
    video::E_MATERIAL_TYPE leafMaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
    leafMaterialType = (video::E_MATERIAL_TYPE) driver->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles(
        "shaders/trees/leaves.vert",
        "main",
        EVST_VS_2_0,
        "shaders/trees/leaves.frag",
        "main", EPST_PS_2_0 , 0 , EMT_TRANSPARENT_ALPHA_CHANNEL , 0 );

        int treeType = 1;
    for(int i=0; i < 12; i++){
        tree2.push_back(new CTreeSceneNode( smgr->getRootSceneNode(), smgr ));
        seed = rand()%4;
        tree2[i]->setup( treeDesigns[seed].Generator, seed, treeDesigns[seed].BillTexture );
        tree2[i]->getLeafNode()->setMaterialTexture( 0, treeDesigns[seed].LeafTexture );
        tree2[i]->setMaterialTexture( 0, treeDesigns[seed].TreeTexture );

bool lightsEnabled = 0;
        if ( lightsEnabled )
            tree2[i]->getLeafNode()->applyVertexShadows( lightDir, 1.0f, 0.25f );

        tree2[i]->getLeafNode()->setMaterialType( leafMaterialType );
        tree2[i]->setMaterialFlag( video::EMF_LIGHTING, 0 );
        tree2[i]->setScale(core::vector3df(0.5f,0.5f,0.5f));
        tree2[i]->setPosition(core::vector3df(0,9,210*i));
        tree2[i]->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
    //     tree2[i]->drop();
    }
}
