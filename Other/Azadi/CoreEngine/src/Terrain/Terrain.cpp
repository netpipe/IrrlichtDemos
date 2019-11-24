#define BUFFER_OFFSET(bytes)    ((GLubyte*) NULL + (bytes))

#include "Terrain.h"
#include <math.h>
#include <cstdlib>
#include <iostream>
#include "../vector.h"
#include "./simplexnoise1234.h"
#include "../tilegrid.h"

tilegrid<int> TGrid(3, 3,0);
#define LEAF_TYPE 0
#define NODE_TYPE 1
#define TILETHRESHOLD 10.0f

float 	     *fVerts;
unsigned int *uiList;

    GLuint vertexBufferHandle;
    GLuint elementBufferHandle;
GLfloat *vertexData;

int Terrain::setter=5;
NODE* pNodeList;

int grass;
int sphereDL;



Terrain::~Terrain()
{
	delete frustum; // Don't forget to delete it after! - Aren
//	free(pNodeList);
	free(uiList);
//	free(pNodeList);

    glDeleteBuffers(1, &vertexBufferHandle);
    glDeleteBuffers(1, &elementBufferHandle);
    glBindBuffer(GL_VERTEX_ARRAY, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Terrain::Initialise()
{

}

float Terrain::getHeight(float x, float z){

    int octaves=setter;//8
    float persistance =2;
    int scale=7;  // noise scaler, not the same as patch scaler
    float total = 0;

    //amplify?
    int ax=0;
    int az=0;

    for(int i=4; i < octaves; i++){
        float frequency = pow(2.0f, i);
        float amplitude = pow(persistance, i);
        total += SimplexNoise1234::noise(((x+ax)/scale)/ frequency, ((z+az)/scale)/ frequency) * amplitude;
    }
 return (total);
};

void Terrain::Sphere ( int dtheta, int dphi ){

	int n;
	int theta, phi;
	Math::Vector3<float> Vertex[4];
//   sphereDL = glGenLists(1);
//   glNewList(sphereDL, GL_COMPILE);
	for ( theta=-90;theta<=90-dtheta;theta+=dtheta )
	{
		for ( phi=0;phi<=360-dphi;phi+=dphi )
		{
			n = 0;
			Vertex[n].x = cos ( theta*piover180 ) * cos ( phi*piover180 );
			Vertex[n].y = cos ( theta*piover180 ) * sin ( phi*piover180 );
			Vertex[n].z = sin ( theta*piover180 );
		n++;
			Vertex[n].x = cos ( ( theta+dtheta ) *piover180 ) * cos ( phi*piover180 );
			Vertex[n].y = cos ( ( theta+dtheta ) *piover180 ) * sin ( phi*piover180 );
			Vertex[n].z = sin ( ( theta+dtheta ) *piover180 );
		n++;
			Vertex[n].x = cos ( ( theta+dtheta ) *piover180 ) * cos ( ( phi+dphi ) *piover180 );
			Vertex[n].y = cos ( ( theta+dtheta ) *piover180 ) * sin ( ( phi+dphi ) *piover180 );
			Vertex[n].z = sin ( ( theta+dtheta ) *piover180 );
		n++;

			if ( theta > -90 && theta < 90 )
			{
				Vertex[n].x = cos ( theta*piover180 ) * cos ( ( phi+dphi ) *piover180 );
				Vertex[n].y = cos ( theta*piover180 ) * sin ( ( phi+dphi ) *piover180 );
				Vertex[n].z = sin ( theta*piover180 );
			n++;
			}

			glPushMatrix();
			//glTranslatef(222,20,222);
			glScalef ( 5, 5, 5 );
			glPointSize ( 5 );
			glBegin ( GL_POINTS );

			for ( int i=0; i < 3;i++ )
			{
				glVertex3f ( Vertex[i].x, Vertex[i].y, Vertex[i].z );
			}

			glEnd();
			glPopMatrix();

		}
	}
}

void Terrain::CreateNode(unsigned int Bounding[4],unsigned int ParentID,unsigned int NodeID) {

	static unsigned int TotalTreeID = 0;
	unsigned int uiNodeType;
	float uiWidth,uiHeight;

	uiWidth = fVerts[(Bounding[1]*3)] - fVerts[(Bounding[0]*3)];
	uiHeight = fVerts[(Bounding[2]*3)+2] - fVerts[(Bounding[0]*3)+2];

	if(0.5*uiWidth==2) {uiNodeType = LEAF_TYPE;}
	else {uiNodeType = NODE_TYPE;}


	NODE *pNode = &pNodeList[NodeID];

	pNode->uiID = NodeID;
	pNode->uiParentID = ParentID;

	for (int i=0; i < 4; i++) { 		//parrent NODE uiBounding coords
		pNode->vBoundingCoordinates[i].x = fVerts[(Bounding[i]*3)];
		pNode->vBoundingCoordinates[i].y = fVerts[(Bounding[i]*3)+1];
		pNode->vBoundingCoordinates[i].z = fVerts[(Bounding[i]*3)+2];
	}

	pNode->bType = uiNodeType;

	if(uiNodeType == LEAF_TYPE){return;}
	else{
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

void Terrain::generateLod(unsigned int LOD, NODE* pNode, const int TN ,const int XT, const int TZ){
 glvertex.clear();  // Clear out the vertices
 elements.clear();  // Clear out the vertices
 unsigned int count=1;
	unsigned int element=0;

 if (LOD==0){
/*		uiBoundingCoordinates[0]	uiBoundingCoordinates[1]
		*---------------------------|--------------------------*
		*---------------------------|--------------------------*
		uiBoundingCoordinates[2]	uiBoundingCoordinates[3]
*/

	for (unsigned int x=0; x < 4 ; x++) { //get the bounding coords


		unsigned int z=pNode[0].uiBranches[x];

			glvertex.push_back(pNode[z].vBoundingCoordinates[0].x+XT);
			glvertex.push_back(getHeight(pNode[z].vBoundingCoordinates[0].x+XT,
					   pNode[z].vBoundingCoordinates[0].z+TZ));
			glvertex.push_back(pNode[z].vBoundingCoordinates[0].z+TZ);
			glvertex.push_back (1.0);glvertex.push_back (0.0);
			elements.push_back(element);
			element++;

			glvertex.push_back(pNode[z].vBoundingCoordinates[2].x+XT);
			glvertex.push_back(getHeight(pNode[z].vBoundingCoordinates[2].x+XT,
					   pNode[z].vBoundingCoordinates[2].z+TZ));
			glvertex.push_back(pNode[z].vBoundingCoordinates[2].z+TZ);
			glvertex.push_back (0.0);glvertex.push_back (1.0);
			elements.push_back(element);
			element++;

			glvertex.push_back(pNode[z].vBoundingCoordinates[3].x+XT);
			glvertex.push_back(getHeight(pNode[z].vBoundingCoordinates[3].x+XT,
					   pNode[z].vBoundingCoordinates[3].z+TZ));
			glvertex.push_back(pNode[z].vBoundingCoordinates[3].z+TZ);
			glvertex.push_back (0.0);glvertex.push_back (0.0);
			elements.push_back(element);
			element++;

			glvertex.push_back(pNode[z].vBoundingCoordinates[1].x+XT);
			glvertex.push_back(getHeight(pNode[z].vBoundingCoordinates[1].x+XT,
					   pNode[z].vBoundingCoordinates[1].z+TZ));
			glvertex.push_back(pNode[z].vBoundingCoordinates[1].z+TZ);

			glvertex.push_back (1.0);glvertex.push_back (1.0);
			elements.push_back(element);
			element++;

	}
    return;
 }else if (LOD==1){
	count=1;

		for (unsigned int n=0; n < 4; n++){ // Unit this is dangerous, N is uninitialized

		 for (unsigned int x=0; x < 4; x++) {
			unsigned int z=pNode[count].uiBranches[x];

			glvertex.push_back(pNode[z].vBoundingCoordinates[0].x+XT);
			glvertex.push_back(getHeight(pNode[z].vBoundingCoordinates[0].x+XT,
					  pNode[z].vBoundingCoordinates[0].z+TZ));
			glvertex.push_back(pNode[z].vBoundingCoordinates[0].z+TZ);
			glvertex.push_back (1.0);glvertex.push_back (0.0);
			elements.push_back(element);
			element++;

			glvertex.push_back(pNode[z].vBoundingCoordinates[2].x+XT);
			glvertex.push_back(getHeight(pNode[z].vBoundingCoordinates[2].x+XT,
					  pNode[z].vBoundingCoordinates[2].z+TZ));
			glvertex.push_back(pNode[z].vBoundingCoordinates[2].z+TZ);
			glvertex.push_back (0.0);glvertex.push_back (1.0);
			elements.push_back(element);
			element++;

			glvertex.push_back(pNode[z].vBoundingCoordinates[3].x+XT);
			glvertex.push_back(getHeight(pNode[z].vBoundingCoordinates[3].x+XT,
						pNode[z].vBoundingCoordinates[3].z+TZ));
			glvertex.push_back(pNode[z].vBoundingCoordinates[3].z+TZ);
			glvertex.push_back (0.0);glvertex.push_back (0.0);
			elements.push_back(element);
			element++;

			glvertex.push_back(pNode[z].vBoundingCoordinates[1].x+XT);
			glvertex.push_back(getHeight(pNode[z].vBoundingCoordinates[1].x+XT,
						pNode[z].vBoundingCoordinates[1].z+TZ));
			glvertex.push_back(pNode[z].vBoundingCoordinates[1].z+TZ);

			glvertex.push_back (1.0);glvertex.push_back (1.0);
        		    elements.push_back(element);
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
			glvertex.push_back(pNode[z].vBoundingCoordinates[0].x+XT);
			glvertex.push_back(getHeight(pNode[z].vBoundingCoordinates[0].x+XT,
					   pNode[z].vBoundingCoordinates[0].z+TZ));
			glvertex.push_back(pNode[z].vBoundingCoordinates[0].z+TZ);

			glvertex.push_back (1.0);glvertex.push_back (0.0);
			elements.push_back(element);
			element++;
			glvertex.push_back(pNode[z].vBoundingCoordinates[2].x+XT);
			glvertex.push_back(getHeight(pNode[z].vBoundingCoordinates[2].x+XT,
					   pNode[z].vBoundingCoordinates[2].z+TZ));
			glvertex.push_back(pNode[z].vBoundingCoordinates[2].z+TZ);

			glvertex.push_back (0.0);glvertex.push_back (1.0);
			elements.push_back(element);
			element++;
			glvertex.push_back(pNode[z].vBoundingCoordinates[3].x+XT);
			glvertex.push_back(getHeight(pNode[z].vBoundingCoordinates[3].x+XT,
					   pNode[z].vBoundingCoordinates[3].z+TZ));
			glvertex.push_back(pNode[z].vBoundingCoordinates[3].z+TZ);

			glvertex.push_back (0.0);glvertex.push_back (0.0);
			elements.push_back(element);
			element++;
			glvertex.push_back(pNode[z].vBoundingCoordinates[1].x+XT);
			glvertex.push_back(getHeight(pNode[z].vBoundingCoordinates[1].x+XT,
					   pNode[z].vBoundingCoordinates[1].z+TZ));
			glvertex.push_back(pNode[z].vBoundingCoordinates[1].z+TZ);

			glvertex.push_back (1.0);glvertex.push_back (1.0);
			elements.push_back(element);
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

			glvertex.push_back(pNode[z].vBoundingCoordinates[0].x+XT);
			glvertex.push_back(getHeight(pNode[z].vBoundingCoordinates[0].x+XT,
					   pNode[z].vBoundingCoordinates[0].z+TZ));
			glvertex.push_back(pNode[z].vBoundingCoordinates[0].z+TZ);
			glvertex.push_back (1.0);glvertex.push_back (0.0);
           		elements.push_back(element);
			element++;

			glvertex.push_back(pNode[z].vBoundingCoordinates[2].x+XT);
			glvertex.push_back(getHeight(pNode[z].vBoundingCoordinates[2].x+XT,
					   pNode[z].vBoundingCoordinates[2].z+TZ));
			glvertex.push_back(pNode[z].vBoundingCoordinates[2].z+TZ);
			glvertex.push_back (0.0);glvertex.push_back (1.0);
			elements.push_back(element);
			element++;

			glvertex.push_back(pNode[z].vBoundingCoordinates[3].x+XT);
			glvertex.push_back(getHeight(pNode[z].vBoundingCoordinates[3].x+XT,
					   pNode[z].vBoundingCoordinates[3].z+TZ));
			glvertex.push_back(pNode[z].vBoundingCoordinates[3].z+TZ);
			glvertex.push_back (0.0);glvertex.push_back (0.0);
			elements.push_back(element);
			element++;

			glvertex.push_back(pNode[z].vBoundingCoordinates[1].x+XT);
			glvertex.push_back(getHeight(pNode[z].vBoundingCoordinates[1].x+XT,
					   pNode[z].vBoundingCoordinates[1].z+TZ));
			glvertex.push_back(pNode[z].vBoundingCoordinates[1].z+TZ);
			glvertex.push_back (1.0);glvertex.push_back (1.0);
			elements.push_back(element);
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
    // Since we are using GL_STATIC_DRAW, we have to delete the old buffer first. (Since the buffer is static and unmodifiable (without screwing things)
    glDeleteBuffers ( 1, &vertexBufferHandle );
    glDeleteBuffers ( 1, &elementBufferHandle );

    // Generate the LOD
    generateLod ( LOD, pNode, TN, XT, TZ );

    // Does the vertex data already exist? if so, we delete it
    if ( vertexData != NULL );
    delete [] vertexData;
    // Allocate enough space for all the vertex data.
    vertexData = new GLfloat[glvertex.size() ];

    // How many GLfloats exist in our glvertex vector?
    sizeVerts = glvertex.size();

    for ( unsigned int i = 0; i < sizeVerts; ++i ){
        // Push the actual glvertex[] data into the temporary.
        vertexData[i] = glvertex[i];
    }

    // Does the vertElements already contain values? if so we delete it
    if ( vertElements != NULL )
        delete [] vertElements;

    // We allocate enough space for all the element indices
    vertElements = new GLuint[elements.size() ];

    sizeElements = elements.size();

    // Then fill them here, using the above gathered loop (elements.push_back(i))
    for ( unsigned int i = 0; i < sizeElements; ++i )
        vertElements[i] = elements[i];

    // Generate fresh buffers for our data
    glGenBuffers ( 1, &vertexBufferHandle );
    glGenBuffers ( 1, &elementBufferHandle );

    glBindBuffer ( GL_ARRAY_BUFFER, vertexBufferHandle );
    // Supply the buffer with the required data
    glBufferData ( GL_ARRAY_BUFFER, sizeof ( GLfloat ) * sizeVerts, vertexData, GL_STATIC_DRAW );

    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, elementBufferHandle );
    // Supply the buffer with the indices
    glBufferData ( GL_ELEMENT_ARRAY_BUFFER, sizeof ( GLfloat ) * sizeElements, vertElements, GL_STATIC_DRAW );
}

void Terrain::CreateGrid(unsigned int w,unsigned int h) {
	unsigned int total=0;

	unsigned int width = w;
	unsigned int height = h;

	fVerts = (float*)malloc((sizeof(float))*width*height*3); /* Allocate space for 											 vertices with an x,y and z*/

	for(unsigned int ctr=0;ctr<height;ctr++)
	{
		for(unsigned int ctr2=0;ctr2<width;ctr2++)
		{				// Vertex number is total
			fVerts[total] =  ctr2;	// Vertex X component is ctr2
			fVerts[total+1] = 0;	// Vertex Y component is height field (0)
			fVerts[total+2] =  ctr;  // Vertex Z component is ctr
			total+=3;		// Increase total to be next vertex number
		}
	}
	total=0;

}

unsigned int Terrain::CalcNodeNum(unsigned int max,unsigned int min) {

	int ctr=0;
	int var = 0;
	while(var==0) {
		ctr+=max;
		max=max/min;

		if(max==1)
		{var=1;}
	}
	ctr++;
	return ctr;
}

Terrain::Terrain()
{
	texture_factory = TextureFactory::getInstance();
	frustum = new Frustum;  //Instantiate the frustum class
	//	Initialise();

	CreateGrid ( 65,65 ); // width and height + 1
	unsigned int Temp = ( 64/4 ) * ( 64/4 ); // devided into 4 sections ??
	unsigned int Temp2 = CalcNodeNum ( Temp,4 ); // max , min

	//	{0,GridWidth,(GridHeight*(GridWidth+1)),((GridHeight)*(GridWidth+1))+GridWidth};
	unsigned int uiBoundingCoordinates[] = {0,64, ( 64* ( 65 ) ), ( ( 64 ) * ( 65 ) ) +64};

	TextureFactory::getInstance()->loadTexture ( "rust1.png" );

	// 		pNodeList = new NODE[Temp2];
	pNodeList = ( NODE* ) malloc ( sizeof ( NODE ) *Temp2 );

	CreateNode ( uiBoundingCoordinates,0,0 );

	vertexData = NULL; // Nullify the pointers first
	vertElements = NULL;

	free ( fVerts );
};

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


