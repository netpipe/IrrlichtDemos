
#include <CTileNode.h>

namespace irr { namespace scene {

#include <debug.h>

CTileNode::CTileNode(ISceneNode *parent, scene::ISceneManager *smgr, s32 id, f32 tileWidth, u32 width, vector3df pos,
    CTileNode **tiles, CTileNodeInfo info) : ISceneNode(parent, smgr, id)
{
    // globals
    this->info = info;
    this->smgr = smgr;

	// user options
	scale = 1.0/width * tileWidth;
	//material.Wireframe = true;

	// for correct tiling + 1 as extra vertex needed (as not really a grid)
	width ++;

	sides[0] = new CTileNodeSidePoint[width];
	sides[1] = new CTileNodeSidePoint[width];
	sides[2] = new CTileNodeSidePoint[width];
	sides[3] = new CTileNodeSidePoint[width];

	if (tiles[0]) memcpy(sides[0], tiles[0]->sides[2], sizeof(CTileNodeSidePoint)*width);
	if (tiles[1]) memcpy(sides[1], tiles[1]->sides[3], sizeof(CTileNodeSidePoint)*width);
	if (tiles[2]) memcpy(sides[2], tiles[2]->sides[0], sizeof(CTileNodeSidePoint)*width);
	if (tiles[3]) memcpy(sides[3], tiles[3]->sides[1], sizeof(CTileNodeSidePoint)*width);

	/* create triangles */
	vcount = width*width;
	vertices = new S3DVertex[vcount];

	tricount = (width-1)*(width-1)*2;
	indices = new u16[tricount*3];

	// ODE
	dVector3 *dVertices = new dVector3[vcount];
	int *dIndices = new int[tricount*3];

	u32 x,y,i=0;
	f32 height = 0;

	/* heightmap calculation */
	f32 *heightArray = new f32[vcount];
	f32 *heightArrayFinal = new f32[vcount];

    // set sides from adjacent tile buffer, excluding corners
    if (tiles[0]) for (y = 1; y < width-1; y ++) heightArray[y*width] = sides[0][y].height;
    if (tiles[1]) for (x = 1; x < width-1; x ++) heightArray[(width-1)*width+x] = sides[1][x].height;
    if (tiles[2]) for (y = 1; y < width-1; y ++) heightArray[y*width+width-1] = sides[2][y].height;
    if (tiles[3]) for (x = 1; x < width-1; x ++) heightArray[x] = sides[3][x].height;

    // now set corners

    // bottom left
    if (tiles[0]) {
        heightArray[0] = sides[0][0].height;
    } else if (tiles[3]) {
        heightArray[0] = sides[3][0].height;
    }
    // top left
    if (tiles[0]) {
        heightArray[width*(width-1)] = sides[0][width-1].height;
    } else if (tiles[1]) {
        heightArray[width*(width-1)] = sides[1][0].height;
    }
    // top right
    if (tiles[1]) {
        heightArray[width*width-1] = sides[1][width-1].height;
    } else if (tiles[2]) {
        heightArray[width*width-1] = sides[2][width-1].height;
    }
    // bottom right
    if (tiles[2]) {
        heightArray[width-1] = sides[2][0].height;
    } else if (tiles[3]) {
        heightArray[width-1] = sides[3][width-1].height;
    }

    // NOW _FULL LENGTH_ OF ALL EDGES WITH ADJACENT TILES SET.
    // STILL MAY BE SOME EDGES WITH UNSET 1 TO WITH-2 OR EVEN 0 TO WIDTH-1.

    // generate random heights except for preset edges
    for (y = 0; y < width; y ++)
    {
        for (x = 0; x < width; x ++)
        {
            if (
                (tiles[0] && x == 0) ||
                (tiles[1] && y == width-1) ||
                (tiles[2] && x == width-1) ||
                (tiles[3] && y == 0)
                )
            {
                // preset so do nothing
            }
            else
            {
                if (rand()%100 < 4) {
                    heightArray[y*width+x] = (rand()%100000)/10000.0;
                } else {
                    heightArray[y*width+x] = 0;//sin(x);
                }
                //heightArray[y*width+x] = sin(x*rand())*0.7;
                //heightArray[y*width+x] = sin(x/10.0);
            }
            heightArrayFinal[y*width+x] = heightArray[y*width+x];
        }
    }

    // smoothing loop
    s32 a,b,it;
    f32 *array = &heightArray[0], *array2 = &heightArrayFinal[0], *la;
    for (it = 0; it < 10; it ++)
    {
    for (y = 0; y < width; y ++) {
        for (x = 0; x < width; x ++) {
            if (
                (tiles[0] && x == 0) ||
                (tiles[1] && y == width-1) ||
                (tiles[2] && x == width-1) ||
                (tiles[3] && y == 0)
            ) {
            }else{
            f32 val = 0, val2 = 0;
            f32 count = 0;
            for (a = -1; a < 2; a ++)
            {
                if (y+a<0) a = 0;
                if (y+a>=width) break;
                for (b = -1; b < 2; b ++)
                {
                    if (x+b<0) b = 0;
                    if (x+b>=width) break;
                    if (a == 0 && b == 0) {
                        val += array[(y+a)*width+x+b]*0.75;///2.0;
                    } else {
                        val2 += array[(y+a)*width+x+b];///9.0;
                    }
                    count += 1.0;
                }
            }

            if (count == 0) continue; // bug: sometimes count == 0 ??? WHY?

            val += 0.6 * ( val2 / (count) );
            array2[y*width+x] = val;
            }
        }
    }

        la = array;
        array = array2;
        array2 = la;
    }

    /*
    // smoothing loop
    s32 a,b,it;
    f32 *array = &heightArray[0], *array2 = &heightArrayFinal[0], *la;
    for (it = 0; it < 10; it ++) {
    for (y = 1; y < width-1; y ++) {
        for (x = 1; x < width-1; x ++) {
            f32 val = 0;
            for (a = -1; a < 2; a ++) {
                for (b = -1; b < 2; b ++) {
                    if (a == 0 & b == 0) {
                        val += array[(y+a)*width+x+b]/2.0;
                    } else {
                        val += array[(y+a)*width+x+b]/10.0;
                    }
                }
            }
            array2[y*width+x] = val;///9.0;
        }
    }
        la = array;
        array = array2;
        array2 = la;
    }
    */

    // set side buffers again (as some may be new...)
    if (!tiles[0]) for (y = 0; y < width; y ++) sides[0][y].height = heightArrayFinal[y*width];
    if (!tiles[1]) for (x = 0; x < width; x ++) sides[1][x].height = heightArrayFinal[(width-1)*width+x];
    if (!tiles[2]) for (y = 0; y < width; y ++) sides[2][y].height = heightArrayFinal[y*width+width-1];
    if (!tiles[3]) for (x = 0; x < width; x ++) sides[3][x].height = heightArrayFinal[x];

	/* mesh generation */
	for (y = 0; y < width; y ++)
    {
        for (x = 0; x < width; x ++)
        {
            height = heightArrayFinal[y*width+x];

            // S3DVertex: x,y,z, nx,ny,nz, colour, u,v
            vertices[y*width+x] = S3DVertex(x*scale, height*scale, y*scale,
                0,1,0,
                SColor(255,255,255,255),
                (float)x/(float)(width-1)*3, (float)y/(float)(width-1)*3);

            if (x < width-1 && y < width-1)
            {
                // two tris for each vert apart from tile edge ones
                indices[i] = y*width+x;
                indices[i+1] = (y+1)*width+x+1;
                indices[i+2] = y*width+x+1;
                i+=3;
                indices[i] = y*width+x;
                indices[i+1] = (y+1)*width+x;
                indices[i+2] = (y+1)*width+x+1;
                i+=3;
            }
        }
    }

    delete[] heightArray;
    delete[] heightArrayFinal;

	//setAutomaticCulling(false);

	box.reset(vertices[0].Pos);
	for (x = 0; x < vcount; x ++)
	{
        // could optimise by adding only corners and max/min heights...
        box.addInternalPoint(vertices[x].Pos);
    }


    if (parent) parent->addChild(this);

    // rest of function is for ODE

    // set ODE vertices
    for (x = 0; x < vcount; x ++)
    {
        dVertices[x][0] = vertices[x].Pos.X;
        dVertices[x][1] = vertices[x].Pos.Y;
        dVertices[x][2] = vertices[x].Pos.Z;
        dVertices[x][3] = 0;
    }
    // set ODE indices
    for (x = 0; x < tricount*3; x ++)
    {
        dIndices[x] = indices[x];
    }

    // create ODE geom
    dTriMeshDataID ode_trimesh = dGeomTriMeshDataCreate();

    dGeomTriMeshDataBuildSingle(ode_trimesh,
    (const dReal *)dVertices, 4*sizeof(dReal), vcount,
    dIndices, tricount*3, 3*sizeof(int));

    dGeomID ode_geom = dCreateTriMesh(info.dSpace, ode_trimesh, 0, 0, 0);
    dGeomSetBody(ode_geom, 0);

    this->dGeom.push_back(ode_geom);
    this->dGeomRelPosition.push_back(vector3df(0,0,0));
    this->dVertices.push_back(dVertices);
    this->dIndices.push_back(dIndices);
}

CTileNode::~CTileNode()
{
    delete[] indices;
    delete[] vertices;
    delete[] sides[0];
    delete[] sides[1];
    delete[] sides[2];
    delete[] sides[3];

    // remove ODE geoms and stuff
    for (i = 0; i < dTriMeshData.size(); i ++)
    {
        if (dTriMeshData[i]) dGeomTriMeshDataDestroy(dTriMeshData[i]);
    }
    for (i = 0; i < dGeom.size(); i ++)
    {
        if (dGeom[i]) dGeomDestroy(dGeom[i]);
    }
    for (i = 0; i < dVertices.size(); i ++)
    {
        if (dVertices[i]) delete [] dVertices[i];
    }
    for (i = 0; i < dIndices.size(); i ++)
    {
        if (dIndices[i]) delete [] dIndices[i];
    }

    // remove all references to this
    removeAll();
}

f32 CTileNode::getHeight(f32 x, f32 y)
{
    // this function needs improving!
    u32 i, chosen;
    f32 lastDist = 1000000000;
    for (i = 0; i < vcount; i ++)
    {
        vector3df pos = vertices[i].Pos;
        // note: y is used instead of z as we are in 2D
        f32 dx = pos.X-x, dy = pos.Z-y;
        f32 dist = sqrt(dx*dx + dy*dy);
        if (dist < lastDist)
        {
            lastDist = dist;
            chosen = i;
        }
    }
    return vertices[chosen].Pos.Y;
}

void QuaternionToEuler(const dQuaternion quaternion, vector3df &euler)
{
    // thanks to Thomas Suter's ODE tutorial for this function
    // http://thomas.webtracker.ch/jahia/Jahia/pid/481
    dReal w,x,y,z;
    w=quaternion[0];
    x=quaternion[1];
    y=quaternion[2];
    z=quaternion[3];
    double sqw = w*w;
    double sqx = x*x;
    double sqy = y*y;
    double sqz = z*z;
    euler.Z = (irr::f32) (atan2(2.0 * (x*y + z*w),(sqx - sqy - sqz + sqw))*irr::core::RADTODEG);
    euler.X = (irr::f32) (atan2(2.0 * (y*z + x*w),(-sqx - sqy + sqz + sqw))*irr::core::RADTODEG);
    euler.Y = (irr::f32) (asin(-2.0 * (x*z - y*w))*irr::core::RADTODEG);
}

// function to add extra geometry to tile. uses ODE. NEED TO IMPLEMENT *proper* ROTATION FOR ODE.
void CTileNode::addGeom(s32 type, vector3df pos)
{
    IAnimatedMesh *amesh = smgr->getMesh("mesh/twizzler.x");
    IAnimatedMeshSceneNode *n = smgr->addAnimatedMeshSceneNode(amesh);
    n->setParent(this);
    n->setPosition(pos);
    /*
    dQuaternion q;
    q[0]=0;
    q[1]=0;
    q[2]=1;
    q[3]=0;
    vector3df rot;
    QuaternionToEuler(q, rot);
    */

    f32 frot = rand();
    vector3df rot = vector3df(0, frot, 0);
    n->setRotation(rot);

    frot *= 0.017453292519943295769236907684886;

    // randomly scale our object. must multiply ODE units by this later!
    f32 scale = 6 + rand()%1000 / 100.0;
    if (rand()%100 < 1)
    {
        scale = 40;
    }
    n->setScale(vector3df(scale,scale,scale));
    //n->addShadowVolumeSceneNode(-1, false);

    n->updateAbsolutePosition();

    IMesh *mesh = amesh->getMesh(0, 255, -1, -1);

    for (i = 0; i < mesh->getMeshBufferCount(); i ++)
    {
        IMeshBuffer *mb = mesh->getMeshBuffer(i);

        u32 vertexCount = mb->getVertexCount();
        u32 indexCount = mb->getIndexCount();

        // get pointers from irrlicht
        S3DVertex *iVertices = (S3DVertex *)mb->getVertices();
        u16 *iIndices = mb->getIndices();

        // create ODE data. must remember these pointers and free them later (in deconstructor)
        dVector3 *dVertices = new dVector3[vertexCount];
        int *dIndices = new int[indexCount];

        for (j = 0; j < mb->getVertexCount(); j ++)
        {
            dVertices[j][0] = iVertices[j].Pos.X*scale;
            dVertices[j][1] = iVertices[j].Pos.Y*scale;
            dVertices[j][2] = iVertices[j].Pos.Z*scale;
            dVertices[j][3] = 0;

            // ugly hack. use sine and cos to rotate about y axis.
            // so rotation will only work around y
            // need to write code to convert irrlicht angles to ODE matrix.
            // then set in each rotation change like is done for position.
            f32 x1 = dVertices[j][0];
            f32 z1 = dVertices[j][2];
            dVertices[j][0] = z1*sin(frot)+x1*cos(frot);
            dVertices[j][2] = z1*cos(frot)-x1*sin(frot);

        }

        for (j = 0; j < mb->getIndexCount(); j ++)
        {
            dIndices[j] = iIndices[j];
        }

        dTriMeshDataID dTriMeshData = dGeomTriMeshDataCreate();
        dGeomTriMeshDataBuildSingle(dTriMeshData,
        (const dReal *)dVertices, 4*sizeof(dReal), vertexCount,
        dIndices, indexCount, 3*sizeof(int));
        dGeomID dGeom = dCreateTriMesh(info.dSpace, dTriMeshData, 0, 0, 0);
        dGeomSetBody(dGeom, 0);

        //dMatrix3 rota;
        //dRFromEulerAngles(rota, 0, rot.Y*0.017453292519943295769236907684886, 0);
        //dGeomSetRotation(dGeom, rota);
        /*
        quaternion qu(rot.X, rot.Y, rot.Z);
        dQuaternion q;
        q[0] = qu.W;
        q[1] = qu.X;
        q[2] = qu.Y;
        q[3] = qu.Z;

        //dGeomSetQuaternion(dGeom, q);
        dMatrix3 mm;
        mm[0]=0;
        mm[1]=rand();
        mm[2]=0;
        dGeomSetRotation(dGeom, mm);
        // get rotation
        dQuaternion result;
        dGeomGetQuaternion(dGeom, result);
        //vector3df rot;
        QuaternionToEuler(result, rot);
        */

        // add to arrays so freed in deconstructor
        this->dTriMeshData.push_back(dTriMeshData);
        this->dGeom.push_back(dGeom);
        this->dGeomRelPosition.push_back(pos);
        this->dVertices.push_back(dVertices);
        this->dIndices.push_back(dIndices);
    }
}

};
};
