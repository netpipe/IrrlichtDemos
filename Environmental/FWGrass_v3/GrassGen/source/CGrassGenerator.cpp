#include "CGrassGenerator.h"

#include <iostream>
#include <pthread.h>
#include <sys/time.h>

#ifdef WINDOWS
#   include <windows.h>
#endif

//we can “use” a namespace now because it will not carry on
//into more source files (unlike from headers)
using namespace irr;

//Again we need to enclose everything in a namespace
namespace GrassGenerator
{

CGrassGenerator::CGrassGenerator(irr::IrrlichtDevice* Irrlicht)
    : irrlicht(Irrlicht)
{
    //check for invalid irrlicht
    if (irrlicht) {
        fileSystem = irrlicht->getFileSystem();
        video = irrlicht->getVideoDriver();
        smgr = irrlicht->getSceneManager();

        pTerrainNode = NULL;
        pVegetationMap = NULL;
        pTerrainModel = NULL;
    }
}

CGrassGenerator::~CGrassGenerator()
{
    removeChannelTextures();
    clearChannelArrays();
    allVTs.clear();

    if (pVegetationMap != NULL) {
        pVegetationMap->drop();
    }
    if (pTerrainModel != NULL) {
        pTerrainModel->drop();
    }

    mb2tVec.clear();
    mbtanVec.clear();
}

void CGrassGenerator::removeChannelTextures()
{
    for (u32 i = 0; i < 3; i++) {
        for (u32 j = 0; j < channelSortedVTs[i].size(); j++) {
            video::ITexture* tex = video->getTexture(channelSortedVTs[i][j].texturePath);
            if (tex) {
                video->removeTexture(tex);
            }
        }
    }
}

void CGrassGenerator::clearChannelArrays()
{
    channelSortedVTs[0].clear();
    channelSortedVTs[1].clear();
    channelSortedVTs[2].clear();

    channelSortedVTsLimits[0].clear();
    channelSortedVTsLimits[1].clear();
    channelSortedVTsLimits[2].clear();

    channelSortedVTsRatioSums[0] = 0;
    channelSortedVTsRatioSums[1] = 0;
    channelSortedVTsRatioSums[2] = 0;
}

//This functions loads the input parameters from an XML file
bool CGrassGenerator::load(const irr::io::path& xmlFilename)
{
    density = 1.f;
    up_axis = 1;

    //If we cannot use the fileSystem pointer then the function
    //cannot continue, therefore it fails (success=false) and
    //quits
    if (!fileSystem) {
        std::cout << "GRASS GENERATOR ERROR: The IFileSystem pointer is NULL." << std::endl;
        return false;
    }

    io::IXMLReader* xmlReader = fileSystem->createXMLReader(xmlFilename);
    //if the file doesnt exist or the irrlicht function fails, it
    //returns a NULL pointer which is unsafe to use and the loading
    //cannot continue
    if (!xmlReader) {
        std::cout << "GRASS GENERATOR ERROR: The IXMLReader pointer is NULL, file most probably doesn't exist." << std::endl;
        return false;
    }

    core::stringc tmp = xmlFilename;
    u32 length = xmlFilename.size()-core::deletePathFromFilename(tmp).size();
    fileSystem->addFolderFileArchive( xmlFilename.subString(0,length).c_str() );

    core::stringc terrainModelFilename;
    core::stringc vegetationMapFilename;
    u8 no_of_VTs = 0;

    removeChannelTextures();
    clearChannelArrays();

    smgr->getMeshCache()->clearUnusedMeshes();

    // returns false if end of file is reached
    while (xmlReader->read()) {
        if (xmlReader->getNodeType()!=io::EXN_COMMENT) {
            core::stringw nodeName = xmlReader->getNodeName();

            if (nodeName==L"terrainModel") {
                terrainModelFilename = xmlReader->getAttributeValueSafe(L"file");
            } else if (nodeName==L"vegetationMap") {
                vegetationMapFilename = xmlReader->getAttributeValueSafe(L"file");
            } else if (nodeName==L"globals") {
                density = xmlReader->getAttributeValueAsFloat(L"density");
                if (density<=0.f) {
                    std::cout << "GRASS GENERATOR ERROR: The density parameter is invalid." << std::endl;
                    xmlReader->drop();
                    return false;
                }

                core::stringc upx = xmlReader->getAttributeValueSafe(L"up_axis");
                if (upx.size()!=1) {
                    std::cout << "GRASS GENERATOR ERROR: The up axis is not declared." << std::endl;
                    xmlReader->drop();
                    return false;
                }

                //wont get changed if up_axis doesnt equal x,X,y,Y,z or Z
                up_axis = 127;

                if (upx[0]=='X'||upx[0]=='x') {
                    up_axis = 0;
                } else if (upx[0]=='Y'||upx[0]=='y') {
                    up_axis = 1;
                } else if (upx[0]=='Z'||upx[0]=='z') {
                    up_axis = 2;
                }

                if (up_axis==127) {
                    std::cout << "GRASS GENERATOR ERROR: The up axis is not declared." << std::endl;
                    xmlReader->drop();
                    return false;
                }
            } else if (nodeName==L"vegetationType"&&no_of_VTs<=32) {

                scene::IMesh* model = smgr->getMesh(xmlReader->getAttributeValueSafe(L"model"));
                if (!model) {
                    continue;
                }

                core::stringc texture;
                if (!model->getMeshBuffer(0)->getMaterial().getTexture(0)) {
                    texture = xmlReader->getAttributeValueSafe(L"texture");
                    model->getMeshBuffer(0)->getMaterial().setTexture(0,video->getTexture(texture));
                } else {
                    video::ITexture* tex = model->getMeshBuffer(0)->getMaterial().getTexture(0);
                    if (tex) {
                        texture = tex->getName();
                    }
                }

                core::stringc colorMask = xmlReader->getAttributeValueSafe(L"colorMask");
                if (colorMask.size()==0||colorMask.size()>4) {
                    if (model->getMeshBuffer(0)->getMaterial().getTexture(0)) {
                        video->removeTexture(model->getMeshBuffer(0)->getMaterial().getTexture(0));
                    }
                    continue;
                }

                u32 cMask = 0x00000000;
                for (u32 i=0; i<colorMask.size(); i++) {
                    if (colorMask[i]=='r'||colorMask[i]=='R') {
                        cMask |= 0x00000001;
                    } else if (colorMask[i]=='g'||colorMask[i]=='G') {
                        cMask |= 0x00000002;
                    } else if (colorMask[i]=='b'||colorMask[i]=='B') {
                        cMask |= 0x00000004;
                    } else if (colorMask[i]==' '||colorMask[i]=='\0') {
                        continue;
                    } else {
                        cMask = 0x00000000;
                        break;
                    }
                }

                //no bits set
                if (!cMask) {
                    if (model->getMeshBuffer(0)->getMaterial().getTexture(0)) {
                        video->removeTexture(model->getMeshBuffer(0)->getMaterial().getTexture(0));
                    }
                    continue;
                }

                VegetationTypeParams temp;
                temp.ratio = xmlReader->getAttributeValueAsInt(L"ratio");

                if (temp.ratio <= 0) {
                    if (model->getMeshBuffer(0)->getMaterial().getTexture(0)) {
                        video->removeTexture(model->getMeshBuffer(0)->getMaterial().getTexture(0));
                    }
                    continue;
                }

                temp.scale = xmlReader->getAttributeValueAsFloat(L"scale");
                if (core::iszero(temp.scale,core::ROUNDING_ERROR_f32)) {
                    if (model->getMeshBuffer(0)->getMaterial().getTexture(0)) {
                        video->removeTexture(model->getMeshBuffer(0)->getMaterial().getTexture(0));
                    }
                    continue;
                }

                temp.position_var = xmlReader->getAttributeValueAsFloat(L"position_var");
                temp.scale_var = xmlReader->getAttributeValueAsFloat(L"scale_var");
                temp.colorMaks = cMask;
                temp.pMesh = model;
                temp.texturePath = texture;
                no_of_VTs++;

                if ((cMask&0x00000001)==0x00000001) {
                    channelSortedVTs[0].push_back(temp);
                    channelSortedVTsRatioSums[0] += temp.ratio;
                    channelSortedVTsLimits[0].push_back(channelSortedVTsRatioSums[0]);
                }
                if ((cMask&0x00000002)==0x00000002) {
                    channelSortedVTs[1].push_back(temp);
                    channelSortedVTsRatioSums[1] += temp.ratio;
                    channelSortedVTsLimits[1].push_back(channelSortedVTsRatioSums[1]);
                }
                if ((cMask&0x00000004)==0x00000004) {
                    channelSortedVTs[2].push_back(temp);
                    channelSortedVTsRatioSums[2] += temp.ratio;
                    channelSortedVTsLimits[2].push_back(channelSortedVTsRatioSums[2]);
                }
                allVTs.push_back(temp);
            }
        }
    }

    channelSortedVTsRatioSums[0] += 1;
    channelSortedVTsRatioSums[1] += 1;
    channelSortedVTsRatioSums[2] += 1;
    xmlReader->drop();

    //Load the terrain model
    scene::IMesh* mesh = smgr->getMesh(terrainModelFilename);
    if (!mesh) {
        std::cout << "GRASS GENERATOR ERROR: The terrain IMesh pointer is NULL, the file most probably doesn't exist." << std::endl;
        return false;
    }
    pTerrainNode = smgr->addMeshSceneNode(mesh);
    pTerrainModel = smgr->createOctreeTriangleSelector(mesh,0,32);

    //Load the Vegetation Map
    pVegetationMap = video->createImageFromFile(vegetationMapFilename);
    if (!pVegetationMap) {
        std::cout << "GRASS GENERATOR ERROR: The vegetationMap IImage pointer is NULL, the file most probably doesn't exist." << std::endl;
        return false;
    }

    return true;
}

//invisible functions and data types (just in this .cpp file)
struct imageMarchThreadInput
{
    core::vector2df noOfGrids;
    video::IImage* vegMap;
    u8* posColours;
    u32* cSVTsRS;
    irr::core::array<irr::u32>* cSVTsL;
    core::rectf threadoffset;
    irr::core::array<irr::u32> plantCounters;
};

void *InterpolatePositions(void *iMTI)
{
    if (!iMTI) {
        pthread_exit(NULL);
    }

    imageMarchThreadInput td;
    td = *((imageMarchThreadInput*)iMTI);
    core::vector2df numberOfGrids = td.noOfGrids;
    video::IImage* pVegetationMap = td.vegMap;
    u8* positionColours = td.posColours;

    for (u32 i=0; i<(td.cSVTsL[0].size()+td.cSVTsL[1].size()+td.cSVTsL[2].size()); i++) {
        ((imageMarchThreadInput*)iMTI)->plantCounters.push_back(0);
    }

    u32 arrayOffset = numberOfGrids.X+1;
    core::vector2df sizeConstant(pVegetationMap->getDimension().Width,pVegetationMap->getDimension().Height);
    sizeConstant /= (numberOfGrids+1.f);

    u32* channelSortedVTsRatioSums = td.cSVTsRS;
    irr::core::array<irr::u32>* channelSortedVTsLimits = td.cSVTsL;
    u32 cSVTsRS2 = channelSortedVTsLimits[1].size()+channelSortedVTsLimits[0].size();
    u32 lastPrintedMessage=0;
    core::dimension2du clamps(pVegetationMap->getDimension().Width-1,pVegetationMap->getDimension().Height-1);
    long tick;   // A point in time

    for (f32 y=td.threadoffset.UpperLeftCorner.Y; y<=td.threadoffset.LowerRightCorner.Y; y++) {
        float my = y*sizeConstant.Y;
        float ypixel = floorf(my);
        u32 arrayIndex = u32(y*numberOfGrids.X-arrayOffset);

#       ifdef WINDOWS
        QueryPerformanceCounter((LARGE_INTEGER*)&tick);
#       else
        timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        tick = ts.tv_nsec;
#       endif

        srand(((u32*)&tick)[0]);
        if (lastPrintedMessage<arrayIndex/204857) {
            std::cout << "Discarding plant positions and picking Vegetation Type (thread "
                      << td.threadoffset.UpperLeftCorner.X << ","
                      << td.threadoffset.UpperLeftCorner.Y << "): "
                      << 100.f*f32(arrayIndex)/(numberOfGrids.Y*numberOfGrids.X) << std::endl;
            lastPrintedMessage = arrayIndex/204857;
        }

        for (f32 x=td.threadoffset.UpperLeftCorner.X; x<=td.threadoffset.LowerRightCorner.X; x++) {
            float m = x*sizeConstant.X;
            u32 xpixel = floorf(m);
            float mu = core::fract(m);
            float mu2 = mu*mu;
            float mu3 = mu*mu2;
            video::SColorf temp[4];

            for (s32 i=-1; i<3; i++) {
                u32 ypixeli = core::clamp(u32(ypixel+i),u32(0),clamps.Height);
                video::SColorf y1 = video::SColorf(pVegetationMap->getPixel(xpixel,ypixeli));
                video::SColorf y0 = video::SColorf(pVegetationMap->getPixel(core::max_(xpixel-1,u32(0)),ypixeli));
                video::SColorf y2 = video::SColorf(pVegetationMap->getPixel(core::min_(xpixel+1,clamps.Width),ypixeli));
                video::SColorf y3 = video::SColorf(pVegetationMap->getPixel(core::min_(xpixel+2,clamps.Width),ypixeli));
                y3.r -= y2.r + y0.r - y1.r;
                y3.g -= y2.g + y0.g - y1.g;
                y3.b -= y2.b + y0.b - y1.b;
                y3.a -= y2.a + y0.a - y1.a;
                y2.r -= y0.r;
                y2.g -= y0.g;
                y2.b -= y0.b;
                y2.a -= y0.a;
                y0.r -= y1.r + y3.r;
                y0.g -= y1.g + y3.g;
                y0.b -= y1.b + y3.b;
                y0.a -= y1.a + y3.a;
                temp[i+1].set(y3.a*mu3+y0.a*mu2+y2.a*mu+y1.a,y3.r*mu3+y0.r*mu2+y2.r*mu+y1.r,y3.g*mu3+y0.g*mu2+y2.g*mu+y1.g,y3.b*mu3+y0.b*mu2+y2.b*mu+y1.b);
            }

            mu = core::fract(my);
            mu2 = mu*mu;
            mu3 = mu*mu2;
            temp[3].a -= temp[2].a + temp[0].a - temp[1].a;
            temp[2].a -= temp[0].a;
            temp[0].a -= temp[1].a + temp[3].a;
            u8 alpha = core::clamp(255.f*(temp[3].a*mu3+temp[0].a*mu2+temp[2].a*mu+temp[1].a),0.f,255.f);

            if (alpha>=rand()%256) {
                temp[3].r -= temp[2].r + temp[0].r - temp[1].r;
                temp[3].g -= temp[2].g + temp[0].g - temp[1].g;
                temp[3].b -= temp[2].b + temp[0].b - temp[1].b;
                temp[2].r -= temp[0].r;
                temp[2].g -= temp[0].g;
                temp[2].b -= temp[0].b;
                temp[0].r -= temp[1].r + temp[3].r;
                temp[0].g -= temp[1].g + temp[3].g;
                temp[0].b -= temp[1].b + temp[3].b;
                u8 red = core::clamp(255.f*(temp[3].r*mu3+temp[0].r*mu2+temp[2].r*mu+temp[1].r),0.f,255.f);
                u8 green = core::clamp(255.f*(temp[3].g*mu3+temp[0].g*mu2+temp[2].g*mu+temp[1].g),0.f,255.f);
                u8 blue = core::clamp(255.f*(temp[3].b*mu3+temp[0].b*mu2+temp[2].b*mu+temp[1].b),0.f,255.f);
                u32 sum = red+green+blue+1;

                //pixel opaque but black
                if (sum == 1) {
                    positionColours[arrayIndex+u32(x)] = 255; // error code
                    continue;
                }
                u32 thresholds[2] = {red,red+green};
                u32 random = rand()%sum;
#ifdef WINDOWS
                QueryPerformanceCounter((LARGE_INTEGER*)&tick);
#else
                timespec ts;
                clock_gettime(CLOCK_REALTIME, &ts);
                tick = ts.tv_nsec;
#endif
                if (random<=thresholds[0]) {
                    srand(((u32*)&tick)[0]*7);
                    random = rand()%channelSortedVTsRatioSums[0];

                    for (u32 I=0; I<channelSortedVTsLimits[0].size(); I++) {
                        if (random<=channelSortedVTsLimits[0][I]) {
                            positionColours[arrayIndex+u32(x)] = I;
                            break;
                        }
                    }
                } else if (random<=thresholds[1]) {
                    srand(((u32*)&tick)[0]*13);
                    random = rand()%channelSortedVTsRatioSums[1];

                    for (u32 I=0; I<channelSortedVTsLimits[1].size(); I++) {
                        if (random<=channelSortedVTsLimits[1][I]) {
                            positionColours[arrayIndex+u32(x)] = I+channelSortedVTsLimits[0].size();
                            break;
                        }
                    }
                } else {
                    srand(((u32*)&tick)[0]*17);
                    random = rand()%channelSortedVTsRatioSums[2];

                    for (u32 I=0; I<channelSortedVTsLimits[2].size(); I++) {
                        if (random<=channelSortedVTsLimits[2][I]) {
                            positionColours[arrayIndex+u32(x)] = I+cSVTsRS2;
                            break;
                        }
                    }
                }
                ((imageMarchThreadInput*)iMTI)->plantCounters[positionColours[arrayIndex+u32(x)]]++;
            }
            else
                positionColours[arrayIndex+u32(x)] = 255; // error code
        }
    }

   pthread_exit(NULL);
}

struct raycastThreadData
{
    scene::ITriangleSelector* terrain;
    core::array<u8>* posColours;
    core::matrix4* plantMatrices;
    irr::core::array<VegetationTypeParams>* cSVTs;
    core::vector2df noOfGrids;
    core::aabbox3df bbox;
    core::vector2df scale;
    core::vector2df translation;
    scene::ISceneCollisionManager* collMan;

    //The size of the plantMatrices Buffer
    u32 oldPlantEstimate;

    //The amount of plants after ones on slopes and in holes have been discarded
    u32* newPlantEstimate;

    //Notifies Mesh Buffer worker thread of completion
    bool* done;

    c8 upAxis;
};

core::line3df getRayAlongX(core::aabbox3df &bbox, core::vector2df &planarCoord)
{
    return core::line3df(core::vector3df(bbox.MaxEdge.X,planarCoord.Y,planarCoord.X),core::vector3df(bbox.MinEdge.X,planarCoord.Y,planarCoord.X));
}

core::line3df getRayAlongY(core::aabbox3df &bbox, core::vector2df &planarCoord)
{
    return core::line3df(core::vector3df(planarCoord.X,bbox.MaxEdge.Y,planarCoord.Y),core::vector3df(planarCoord.X,bbox.MinEdge.Y,planarCoord.Y));
}

core::line3df getRayAlongZ(core::aabbox3df &bbox, core::vector2df &planarCoord)
{
    return core::line3df(core::vector3df(planarCoord.X,planarCoord.Y,bbox.MaxEdge.Z),core::vector3df(planarCoord.X,planarCoord.Y,bbox.MinEdge.Z));
}

void *RaycastPositions(void *rTD)
{
    if (!rTD) {
        pthread_exit(NULL);
    }

    raycastThreadData td = *((raycastThreadData*)rTD);
    scene::ITriangleSelector* terrain = td.terrain;
    u8* positionColours = (td.posColours->pointer());
    core::matrix4* matrices = td.plantMatrices;
    irr::core::array<VegetationTypeParams>* channelSortedVTs = td.cSVTs;
    core::vector2df numberOfGrids = td.noOfGrids;
    core::aabbox3df bbox = td.bbox;
    core::vector2df scale = td.scale;
    core::vector2df translation = td.translation;
    scene::ISceneCollisionManager* collMan = td.collMan;

    //need to offset the positions of the plants by half the distance between plants (minor issue)
    core::vector2df terrainOffset = core::vector2df(1.f,1.f)/(scale*numberOfGrids)+(translation)/scale;
    //coordinate on the plane of the other two axes to the up axis
    core::vector2df planarCoord;

    //gets multiplied to transform image-space (pixel positions) to worldspace positions
    scale *= numberOfGrids;

    //Function pointer saves us running the if statements for every pixel
    //But the code is incredibly hard to get right (function pointers are messy)
    core::line3df (*rayFunc)(core::aabbox3df&,core::vector2df&);
    if (td.upAxis==0)
        rayFunc = &getRayAlongX;
    else if (td.upAxis==1)
        rayFunc = &getRayAlongY;
    else
        rayFunc = &getRayAlongZ;
    core::triangle3df hitTriangle; //stores the triangle hit by the ray
    core::vector3df intersection; //stores the plant position
    //success of the raycast, no intersection means no surface for the plant to sit on.
    bool nohole=false;
    u32 cSVTsRS2 = channelSortedVTs[1].size()+channelSortedVTs[0].size();
    u32 cSVTsSum = channelSortedVTs[2].size()+cSVTsRS2;
    long tick;   // A point in time
    //counter of plants that passed the raycasts and slope tests
    u32 counter=0;
    //variable to store the increments to when to print another message updating on progress
    u32 lastPrintedMessage=0;

    for (f32 y=0.f; y<numberOfGrids.Y; y++)
    {
        u32 arrayIndex = u32(y*numberOfGrids.X);
        //whether to print a message notifying of progress
        if (lastPrintedMessage<arrayIndex/204857)
        {
            std::cout << "Finding plant positions via raycasting against the terrain geometry: "
                      << 100*f32(arrayIndex)/(numberOfGrids.Y*numberOfGrids.X) << std::endl;
            lastPrintedMessage = arrayIndex/204857;
        }
        for (f32 x=0.f; x<numberOfGrids.X; x++)
        {
            //This deals with retrieving the right Vegetation Type parameters
            //from the pixel 8-byte index
            u32 VTindex = positionColours[arrayIndex+u32(x)];
            f32 scalef,scale_var,pos_var;
            if (VTindex>=cSVTsSum) // no plant here
                continue;
            if (VTindex>=cSVTsRS2)
            {
                VTindex -= cSVTsRS2;
                pos_var = channelSortedVTs[2][VTindex].position_var;
                scalef = (channelSortedVTs[2][VTindex].scale);
                scale_var = (channelSortedVTs[2][VTindex].scale_var);
            }
            else if (VTindex>=channelSortedVTs[0].size())
            {
                VTindex -= channelSortedVTs[0].size();
                pos_var = channelSortedVTs[1][VTindex].position_var;
                scalef = (channelSortedVTs[1][VTindex].scale);
                scale_var = (channelSortedVTs[1][VTindex].scale_var);
            }
            else
            {
                pos_var = channelSortedVTs[0][VTindex].position_var;
                scalef = (channelSortedVTs[0][VTindex].scale);
                scale_var = (channelSortedVTs[0][VTindex].scale_var);
            }
            // Inverting the Y a.k.a. V coord of the texture corrects OpenGL Uv texturing
            planarCoord = core::vector2df(x,numberOfGrids.Y-y)/scale+terrainOffset;
            //Getting the CPU clock ticks or nanoseconds
#ifdef WINDOWS
            QueryPerformanceCounter((LARGE_INTEGER*)&tick);
#else
            timespec ts;
            clock_gettime(CLOCK_REALTIME, &ts);
            tick = ts.tv_nsec;
#endif
            srand(((u32*)&tick)[0]);
            //2039 and 2053 are just big primes to give "random" numbers with high fidelty
            //I divde by half of each number to get a value in the 0.f to 2.f range
            //which I transform into the -1.f to 1.f range
            planarCoord.X += f32(rand()%2039)/1019.5*pos_var-pos_var;
            //multiply to get a slightly different division remainder
            srand(((u32*)&tick)[0]*7);
            planarCoord.Y += f32(rand()%2053)/1026.5*pos_var-pos_var;
            //I'm not doing anything with this variable, i just need to feed this to the raycast function
            scene::ISceneNode* selectedSceneNode;
            nohole = collMan->getCollisionPoint((const core::line3d<float>&)rayFunc(bbox,planarCoord),(scene::ITriangleSelector*)terrain,intersection,hitTriangle,selectedSceneNode);
            if (!nohole) //handles gaps in terrain like holes/wells
                continue;
            // The maths behind this simplification is complicated, but I can assure that the dot product
            //with a vector along an axis simplifies to just the same coefficient of the other vector
            core::vector3df normal(hitTriangle.getNormal().normalize());
            if (normal.Y<0.42262) // reject if triangle has a slope of more than cos(65)
                continue;
            //we can safely overwrite this as counter<arrayIndex+x always,
            //and the previous values have already been processed
            positionColours[counter] = positionColours[arrayIndex+u32(x)];
            //build a rotation so the plant is perpendicular to the surface of the terrain
            matrices[counter].buildRotateFromTo(core::vector3df(0.f,1.f,0.f),normal);
            matrices[counter].setTranslation(intersection);
#ifdef WINDOWS
            QueryPerformanceCounter((LARGE_INTEGER*)&tick);
#else
            clock_gettime(CLOCK_REALTIME, &ts);
            tick = ts.tv_nsec;
#endif
            srand(((u32*)&tick)[0]*13);
            scalef += f32(rand()%1023)/511.5*scale_var-scale_var;
            srand(((u32*)&tick)[0]*17);
            //simplified the scaling of a matrix, less of copying memory around and less multiplications
            matrices[counter][0] *= scalef;
            matrices[counter][1] *= scalef;
            matrices[counter][2] *= scalef;
            matrices[counter][3] *= scalef;
            matrices[counter][4] *= scalef;
            matrices[counter][5] *= scalef;
            matrices[counter][6] *= scalef;
            matrices[counter][7] *= scalef;
            matrices[counter][8] *= scalef;
            matrices[counter][9] *= scalef;
            matrices[counter][10] *= scalef;
            matrices[counter][11] *= scalef;
            f32 mat4x1[4];
            //just re-using a variable, 0.00998917 == 2*PI/629, 629 is yet another high prime
            scalef = f32(rand()%629)*0.00998917f;
            //introducing random rotation around the plant's stem
            //matrix multiplication simplified out
            //I assume that all plants have their up axis at Y
            const f32 cp = cos( scalef );
            const f32 sp = sin( scalef );
            mat4x1[0] = matrices[counter][0]*cp - matrices[counter][8]*sp;
            mat4x1[1] = matrices[counter][1]*cp - matrices[counter][9]*sp;
            mat4x1[2] = matrices[counter][2]*cp - matrices[counter][10]*sp;
            mat4x1[3] = matrices[counter][3]*cp - matrices[counter][11]*sp;
            matrices[counter][8] = matrices[counter][0]*sp + matrices[counter][8]*cp;
            matrices[counter][9] = matrices[counter][1]*sp + matrices[counter][9]*cp;
            matrices[counter][10] = matrices[counter][2]*sp + matrices[counter][10]*cp;
            matrices[counter][11] = matrices[counter][3]*sp + matrices[counter][11]*cp;
            matrices[counter][0] = mat4x1[0];
            matrices[counter][1] = mat4x1[1];
            matrices[counter][2] = mat4x1[2];
            matrices[counter][3] = mat4x1[3];
            //one more plant processed
            counter++;
            *(((raycastThreadData*)rTD)->newPlantEstimate) = counter;
        }
    }

    //notify the threads using the output from this one that execution is complete
    *(((raycastThreadData*)rTD)->done) = true;
    //we dont need the triangle selector anymore (no more raycasts)
    //we will need extra memory, in case of complicated terrain, this is crucial
    if (terrain)
        terrain->drop();

   td.posColours->erase(td.oldPlantEstimate,td.posColours->size()-td.oldPlantEstimate);
   //some plants could have failed the raycasts
   //other threads are working with the old estimate
   //so we fill left over colours with error codes
   for (u32 i=counter; i<td.oldPlantEstimate; i++)
        positionColours[i] = 255;

   pthread_exit(NULL);
}

struct meshCopyThreadData
{
    //constructor to set the constant pointers
    meshCopyThreadData(const bool* completion, const u32* plantsdone) : done(completion), plantsDone(plantsdone) {}
    //I'm using constant pointers so the worker
    //threads can only access and not write the
    //counter, which could be catastrophic
    const bool* done;
    const u32* plantsDone;
    u32 oldPlantEstimate;
    u32 start;
    u8* posColours;
    core::matrix4* plantMatrices;
    irr::core::array<VegetationTypeParams>* cSVTs;
    core::aabbox3df bbox;
    f32* mb2tQuadXCount;
    f32* mb2tQuadScale;
    scene::ISceneManager* smgr;
    //scene::SMeshBufferLightMap** mb2t;
    irr::core::array<irr::scene::SMeshBufferLightMap*> mb2tVec;
    pthread_mutex_t* mutexes;
    u32* allVTLookUpTable;
};

void *CopyMeshesIntoQuadsGrass(void *mCTD)
{
    if (!mCTD)
        pthread_exit(NULL);

    meshCopyThreadData td = *((meshCopyThreadData*)mCTD);
    u8* positionColours = td.posColours;
    core::matrix4* plantMatrices = td.plantMatrices;
    irr::core::array<VegetationTypeParams>* channelSortedVTs = td.cSVTs;
    u32 cSVTsRS2 = channelSortedVTs[1].size()+channelSortedVTs[0].size();
    f32* mb2tQuadXCount = td.mb2tQuadXCount;
    f32* mb2tQuadScale = td.mb2tQuadScale;
    scene::ISceneManager* smgr = td.smgr;
    //scene::SMeshBufferLightMap** mb2t = td.mb2t;
    irr::core::array<irr::scene::SMeshBufferLightMap*> mb2tVec = td.mb2tVec;
    pthread_mutex_t* mutexes = td.mutexes;
    u32 lastMessage = 0;

    for (u32 i=td.start; i<td.oldPlantEstimate; i+=3)
    {
        //Here we prevent the thread from "jumping ahead" of the thread which feeds in
        //plant positions, we use different ways of making the thread wait and give up
        //CPU core for other threads (different for each OS).
        if (i>=(*(td.plantsDone)))
        {
            if (*(td.done))
                break;
#ifdef WINDOWS
            SwitchToThread();
#else
            pthread_yield();
#endif
            //make the counter stay the same
            i-=3;
            continue;
        }
        scene::IMesh* mesh;
        u32 VTindex;
        //error code ouputted if plant failed some stages
        if (positionColours[i]==255)
            continue;
        else if (positionColours[i]>=channelSortedVTs[0].size()&&positionColours[i]<cSVTsRS2)
        {
            mesh = channelSortedVTs[1][positionColours[i]-channelSortedVTs[0].size()].pMesh;
            VTindex = td.allVTLookUpTable[positionColours[i]];
        }
        else
            continue;
        //No plant quads are supposed to be here
        //(not ever supposed to happen but check for it anyway)
        if (mb2tQuadXCount[VTindex]<1.f)
            continue;
        if (i/16384>lastMessage) {
            std::cout << "Creating Plant (Grass) Geometry: " << 100*i/td.oldPlantEstimate << std::endl;
            lastMessage = i/16384;
        }
        core::vector3df center = plantMatrices[i].getTranslation();
        core::vector3df up;
        //irrlicht's rotate is not perfect it applies scaling
        core::vector3df correctScale = plantMatrices[i].getScale();
        //we make the plant rotate so its Y axis lines up with the
        //normal/is perpendicular to the terrain
        plantMatrices[i].rotateVect(up,core::vector3df(0.f,1.f,0.f));
        //so we divide by the scale to even it out
        up /= correctScale;
        //create a working copy of the VT mesh
        mesh = smgr->getMeshManipulator()->createMeshWith2TCoords(mesh);
        //grab a pointer to the vertices
        video::S3DVertex2TCoords* verts = (video::S3DVertex2TCoords*)mesh->getMeshBuffer(0)->getVertices();
        //Transform/move all vertices and correct normals
        for (u32 j=0; j<mesh->getMeshBuffer(0)->getVertexCount(); j++) {
            plantMatrices[i].transformVect(verts[j].Pos);
            //normal equals the up axis to achieve proper lighting
            verts[j].Normal = up;
            verts[j].Color = video::SColor(255,255,255,255);
            //commented out because I am not using a proper shader yet
            //verts[j].Color.setAlpha(verts[j].TCoords.Y*255.f);
            //verts[j].TCoords.Y = center.X;
            verts[j].TCoords2.set(center.Y,center.Z);
        }
        //reconstruct x,y coordinates in the xy grid of quads
        u32 index = mb2tQuadXCount[VTindex]*floorf(mb2tQuadScale[VTindex]*(center.Z-td.bbox.MinEdge.Z))+floorf(mb2tQuadScale[VTindex]*(center.X-td.bbox.MinEdge.X));
        pthread_mutex_lock(mutexes+VTindex);
        //append to the main VT's quad's buffer
        if (mb2tVec.allocated_size() != 0) {
            mb2tVec[VTindex][index].append(verts,mesh->getMeshBuffer(0)->getVertexCount(),mesh->getMeshBuffer(0)->getIndices(),mesh->getMeshBuffer(0)->getIndexCount());
        }
        pthread_mutex_unlock( mutexes+VTindex );
        //dispose of the working copy of the mesh
        mesh->drop();

    }

   pthread_exit(NULL);
}

struct meshCopyThreadDataBush
{
    meshCopyThreadDataBush(const core::aabbox3df& box) : BoundingBox(box) {}
    u32 oldPlantEstimate;
    u8* posColours;
    core::matrix4* plantMatrices;
    irr::core::array<VegetationTypeParams>* cSVTs;
    core::aabbox3df BoundingBox;
    f32* mbtanQuadXCount;
    f32* mbtanQuadScale;
    scene::ISceneManager* smgr;
    //scene::SMeshBufferTangents** mbtan;
    irr::core::array<irr::scene::SMeshBufferTangents*> mbtanVec;
    u32* allVTLookUpTable;
    u32* mbtanLookUpTable;
    u32 start;
    pthread_mutex_t* mutexes;
};

void *CopyMeshesIntoQuads(void *mCTDB)
{
    if (!mCTDB)
        return NULL;

    meshCopyThreadDataBush td = *((meshCopyThreadDataBush*)mCTDB);
    u8* positionColours = td.posColours;
    core::matrix4* plantMatrices = td.plantMatrices;
    core::array<VegetationTypeParams>* channelSortedVTs = td.cSVTs;
    u32 cSVTsRS2 = channelSortedVTs[1].size()+channelSortedVTs[0].size();
    f32* mbtanQuadXCount = td.mbtanQuadXCount;
    f32* mbtanQuadScale = td.mbtanQuadScale;
    scene::ISceneManager* smgr = td.smgr;
    core::array<irr::scene::SMeshBufferTangents*> mbtanVec = td.mbtanVec;
    pthread_mutex_t* mutexes = td.mutexes;
    u32 lastMessage = 0;

    for (u32 i=td.start; i<td.oldPlantEstimate; i+=4)
    {
        scene::IMesh* mesh;
        u32 VTindex;
        if (positionColours[i]==255)
            continue;
        else if (positionColours[i]>=cSVTsRS2)
        {
            mesh = channelSortedVTs[2][positionColours[i]-cSVTsRS2].pMesh;
            VTindex = td.mbtanLookUpTable[td.allVTLookUpTable[positionColours[i]]];
        }
        else if (positionColours[i]<channelSortedVTs[0].size())
        {
            mesh = channelSortedVTs[0][positionColours[i]].pMesh;
            VTindex = td.mbtanLookUpTable[td.allVTLookUpTable[positionColours[i]]];
        }
        else
            continue;
        if (mbtanQuadXCount[VTindex]<1.f||VTindex==255)
            continue;
        if (i/16384>lastMessage) {
            std::cout << "Creating Plant Geometry: " << 100*i/td.oldPlantEstimate << std::endl;
            lastMessage = i/16384;
        }
        core::vector3df center = plantMatrices[i].getTranslation();
        core::vector3df up;
        core::vector3df correctScale = plantMatrices[i].getScale();
        plantMatrices[i].rotateVect(up,core::vector3df(0.f,1.f,0.f));
        up /= correctScale;
        mesh = smgr->getMeshManipulator()->createMeshWithTangents(mesh,false,false,false,false);
        video::S3DVertexTangents* verts = (video::S3DVertexTangents*)mesh->getMeshBuffer(0)->getVertices();
        for (u32 j=0; j<mesh->getMeshBuffer(0)->getVertexCount(); j++)
        {
            plantMatrices[i].transformVect(verts[j].Pos);
            //here we actually rotate the normal
            plantMatrices[i].rotateVect(verts[j].Normal);
            verts[j].Normal /= correctScale;
            verts[j].Tangent = center;
            verts[j].Binormal = up;
        }
        u32 index = mbtanQuadXCount[VTindex]*floorf(mbtanQuadScale[VTindex]*(center.Z-td.BoundingBox.MinEdge.Z))+floorf(mbtanQuadScale[VTindex]*(center.X-td.BoundingBox.MinEdge.X));
        pthread_mutex_lock(mutexes+VTindex);
        if (mbtanVec.allocated_size() != 0) {
            mbtanVec[VTindex][index].append(verts,mesh->getMeshBuffer(0)->getVertexCount(),mesh->getMeshBuffer(0)->getIndices(),mesh->getMeshBuffer(0)->getIndexCount());
        }
        pthread_mutex_unlock(mutexes+VTindex);
        mesh->drop();
    }

    return NULL;
}

//The processing function
void CGrassGenerator::process()
{
    u32 timeStart = irrlicht->getTimer()->getRealTime();
    core::aabbox3df bbox = pTerrainNode->getBoundingBox();
    core::vector2df translation;
    if (up_axis==0)
    {
        scale.Y = 1.f/(bbox.MaxEdge.Y-bbox.MinEdge.Y);
        scale.X = 1.f/(bbox.MaxEdge.Z-bbox.MinEdge.Z);
        translation.Y = (bbox.MinEdge.Y)*scale.Y;
        translation.X = (bbox.MinEdge.Z)*scale.X;
    }
    else if (up_axis==1)
    {
        scale.Y = 1.f/(bbox.MaxEdge.Z-bbox.MinEdge.Z);
        scale.X = 1.f/(bbox.MaxEdge.X-bbox.MinEdge.X);
        translation.Y = (bbox.MinEdge.Z)*scale.Y;
        translation.X = (bbox.MinEdge.X)*scale.X;
    }
    else
    {
        scale.Y = 1.f/(bbox.MaxEdge.Y-bbox.MinEdge.Y);
        scale.X = 1.f/(bbox.MaxEdge.X-bbox.MinEdge.X);
        translation.Y = (bbox.MinEdge.Y)*scale.Y;
        translation.X = (bbox.MinEdge.X)*scale.X;
    }
    core::vector2df numberOfGrids(core::vector2df(1.f)/(scale*density));
    numberOfGrids.X = floor(numberOfGrids.X);
    numberOfGrids.Y = floor(numberOfGrids.Y);
    numberOfGrids -= 1.f;

    if (numberOfGrids.X<1||numberOfGrids.Y<1) {
        std::cout << "GRASS GENERATOR ERROR: The density parameter is too large." << std::endl;
        return;
    }

    //the value corresponds to the index of the vegetation type chosen (max 32)
    core::array<u8> positionColours(u32(numberOfGrids.X*numberOfGrids.Y));

    //Launching threads to interpolate colours for each plant position and pick a Vegetation Type and discard plant positions
    pthread_t threads[4];
    int rc;
    bool join[] = {true,true,true,true};
    imageMarchThreadInput td[4];
    td[0].threadoffset = core::rectf(1,1,floorf(numberOfGrids.X/2),floorf(numberOfGrids.Y/2));
    td[1].threadoffset = core::rectf(floorf(numberOfGrids.X/2)+1,1,numberOfGrids.X,floorf(numberOfGrids.Y/2));
    td[2].threadoffset = core::rectf(1,floorf(numberOfGrids.Y/2)+1,floorf(numberOfGrids.X/2),numberOfGrids.Y);
    td[3].threadoffset = core::rectf(floorf(numberOfGrids.X/2)+1,floorf(numberOfGrids.Y/2)+1,numberOfGrids.X,numberOfGrids.Y);
    for(u32 i=0; i<4; i++){
        td[i].noOfGrids = numberOfGrids;
        td[i].posColours = positionColours.pointer();
        td[i].vegMap = pVegetationMap;
        td[i].cSVTsL = channelSortedVTsLimits;
        td[i].cSVTsRS = channelSortedVTsRatioSums;
        rc = pthread_create(&threads[i], NULL, InterpolatePositions, (void *)(td+i));
        if (rc) {
            std::cout << "ERROR; return code from pthread_create() is " << rc << std::endl;
            join[i] = false;
        }
    }

    //Join threads - wait for them if necessary
    for(u32 i=0; i<4; i++)
    {
        if (join[i])
            (void) pthread_join(threads[i],NULL);
        join[i] = true;
    }

    //dispose of the vegetation map, we dont need it anymore
    if (pVegetationMap != NULL) {
        pVegetationMap->drop();
    }

    //set the pointer to NULL so it doesnt pass another "if (vegetationMap)" somewhere and get deleted again == crash
    pVegetationMap = NULL;

    //the total number of plants gets added from each thread and Vegetation Type
    u32 numberOfPlants = 0;

    //number of plants per VT
    core::array<u32> numberOfPlantsVT(allVTs.size());

    //this is an array into which you can put the 8 bit index from positionColours[] to obtain an array index corresponding to the
    //exact same Vegetation Type in the allVTs array or the numberOfPlantsVT array
    core::array<u32> allVTLookUpTable(channelSortedVTsLimits[0].size()+channelSortedVTsLimits[1].size()+channelSortedVTsLimits[2].size());

    //Initialize counters (may end up with random starting values)
    for (u32 j=0; j<allVTs.size(); j++) {
        numberOfPlantsVT[j] = 0;
    }
    for (u32 i=0; i<channelSortedVTsLimits[0].size(); i++) {
        VegetationTypeParams temp = channelSortedVTs[0][i];

        for (u32 j=0; j<allVTs.size(); j++) {
            if (temp.equals(allVTs[j])) {
                numberOfPlantsVT[j] += (td[0].plantCounters[i]+td[1].plantCounters[i]+td[2].plantCounters[i]+td[3].plantCounters[i]);
                allVTLookUpTable[i] = j;
                break;
            }
        }
    }
    for (u32 i=0; i<channelSortedVTsLimits[1].size(); i++) {
        VegetationTypeParams temp = channelSortedVTs[1][i];
        i += channelSortedVTsLimits[0].size();

        for (u32 j=0; j<allVTs.size(); j++) {
            if (temp.equals(allVTs[j])) {
                numberOfPlantsVT[j] += (td[0].plantCounters[i]+td[1].plantCounters[i]+td[2].plantCounters[i]+td[3].plantCounters[i]);
                allVTLookUpTable[i] = j;
                break;
            }
        }
        i -= channelSortedVTsLimits[0].size();
    }
    for (u32 i=0; i<channelSortedVTsLimits[2].size(); i++) {
        VegetationTypeParams temp = channelSortedVTs[2][i];
        i += channelSortedVTsLimits[0].size()+channelSortedVTsLimits[1].size();

        for (u32 j=0; j<allVTs.size(); j++) {
            if (temp.equals(allVTs[j])) {
                numberOfPlantsVT[j] += (td[0].plantCounters[i]+td[1].plantCounters[i]+td[2].plantCounters[i]+td[3].plantCounters[i]);
                allVTLookUpTable[i] = j;
                break;
            }
        }
        i -= channelSortedVTsLimits[0].size()+channelSortedVTsLimits[1].size();
    }
    for (u32 j=0; j<allVTs.size(); j++) {
        numberOfPlants += numberOfPlantsVT[j]; //total sum of all plants
        u32 quadTemp = allVTs[j].pMesh->getMeshBuffer(0)->getIndexCount()*numberOfPlantsVT[j];
        //divide by 3*theVertexCacheTrianglesAllowed and make the amount of leaves one more
        //because integer division rounds down (better to have one more quad)
        quadTemp = quadTemp/((allVTs[j].colorMaks&0x00000002)==0x00000002 ? 24576:21504)+(quadTemp>0 ? 1:0);

        //Some Debug Information
        std::cout << "Plant Type No: " << j
                  << " Count: " << numberOfPlantsVT[j]
                  << ", Quad Leaves: " << quadTemp
                  << std::endl;

        //now stores no. of quad leaves instead of per VT plant count
        numberOfPlantsVT[j] = quadTemp;
    }

    core::array<core::matrix4> plantMatrices(numberOfPlants);
    std::cout << "Possible number of plants: " << numberOfPlants << std::endl;

    //need to find the first grass VT in the allVT array
    u32 allVTsGrassStartIndex = 255;

    for (u32 i=0; i<allVTs.size(); i++) {
        if ((allVTs[i].colorMaks&0x00000002)==0x00000002) {
            allVTsGrassStartIndex = i;
            break;
        }
    }

    //X quad grid size
    mb2tQuadXCount = new f32[channelSortedVTs[1].size()];

    //How big is the individual grid
    mb2tQuadScale = new f32[channelSortedVTs[1].size()];

    //it is likely many threads will try to append vertices and indices to the same quad mesh buffer
    //mutexes pause threads because only one can lock it while the others wait
    pthread_mutex_t mb2tMutexes[channelSortedVTs[1].size()];

    //if no grass VTs were defined we shouldnt bother with computing them
    if (allVTsGrassStartIndex!=255) {

        //i is iterating through allVTs and j is iterating through grass VTs
        for (u32 i=allVTsGrassStartIndex, j=0; i<allVTs.size()&&j<channelSortedVTs[1].size(); i++) {
            if ((allVTs[i].colorMaks&0x00000002)==0x00000002) {
                pthread_mutex_init(mb2tMutexes+j, 0);

                //we dont want to add a mesh buffer for an empty/duplicate VT entry
                if (numberOfPlantsVT[i]) {
                    mb2tQuadScale[j] = sqrtf(f32(numberOfPlantsVT[i])*scale.X*scale.Y);

                    //better be on the safe side, integer division floors so +1
                    mb2tQuadXCount[j] = floorf(mb2tQuadScale[j]/scale.X)+1.f;
                    u32 yCount = floorf(mb2tQuadScale[j]/scale.Y)+1;
                    mb2tVec.push_back(new scene::SMeshBufferLightMap[u32(mb2tQuadXCount[j]*yCount)]);

                    //debug information
                    std::cout << "Plant Type No: " << i
                              << " Quad Leaves: " << numberOfPlantsVT[i]
                              << ", Layout: " << mb2tQuadXCount[j]
                              << "x " << yCount
                              << std::endl;
                }
                else
                {
                    //error codes, get caught out in if statements
                    mb2tQuadXCount[j] = 0.f;
                    mb2tVec.push_back(NULL);
                }
                j++;
            }
        }
    }

    //all VTs than grass are bushes
    mbtanSize = allVTs.size()-channelSortedVTs[1].size();

    //rest of the procedure is the same as the grass quads
    mbtanQuadXCount = new f32[mbtanSize];
    mbtanQuadScale = new f32[mbtanSize];
    u32 mbtanLookUpTable[allVTs.size()];
    pthread_mutex_t mbtanMutexes[mbtanSize];

    if (mbtanSize) {
        for (u32 i=0,j=0; i<allVTs.size()&&j<mbtanSize; i++) {
            if ((allVTs[i].colorMaks&0x00000002)!=0x00000002) {
                pthread_mutex_init(mbtanMutexes+j, 0);

                if (numberOfPlantsVT[i]) {
                    mbtanQuadScale[j] = sqrtf(f32(numberOfPlantsVT[i])*scale.X*scale.Y);
                    mbtanQuadXCount[j] = floorf(mbtanQuadScale[j]/scale.X)+1.f;
                    u32 yCount = floorf(mbtanQuadScale[j]/scale.Y)+1;
                    mbtanVec.push_back(new scene::SMeshBufferTangents[u32(mbtanQuadXCount[j]*yCount)]);

                    //debug information
                    std::cout << "Plant Type No: " << i
                              << " Quad Leaves: " << numberOfPlantsVT[i]
                              << ", Layout: " << mbtanQuadXCount[j]
                              << "x " << yCount
                              << std::endl;
                } else {
                    mbtanQuadXCount[j] = 0.f;
                    mbtanVec.push_back(NULL);
                }
                mbtanLookUpTable[i] = j;
                j++;
            } else {
                mbtanLookUpTable[i] = 255;
            }
        }
    }

    raycastThreadData td2;
    u32 newPlantEstimate=0;
    bool plantsCasted = false;
    td2.noOfGrids = numberOfGrids;
    td2.terrain = pTerrainModel;
    td2.posColours = &positionColours;
    td2.plantMatrices = plantMatrices.pointer();
    td2.cSVTs = channelSortedVTs;
    td2.bbox = bbox;
    td2.scale = scale;
    td2.translation = translation;
    td2.collMan = smgr->getSceneCollisionManager();
    td2.done = &plantsCasted;
    td2.newPlantEstimate = &newPlantEstimate;
    td2.oldPlantEstimate = numberOfPlants;
    td2.upAxis = up_axis;
    rc = pthread_create(threads, NULL, RaycastPositions, (void *)&td2);

    if (rc) {
        std::cout << "ERROR; return code from pthread_create() is " << rc << std::endl;
        join[0] = false;
    }

    //dont bother launching thread if no grass VTs
    if (allVTsGrassStartIndex!=255) {
        //passing pointers because the thread needs to acess variables out of
        //it's scope
        meshCopyThreadData td3(&plantsCasted,&newPlantEstimate);
        td3.oldPlantEstimate = numberOfPlants;
        td3.posColours = positionColours.pointer();
        td3.plantMatrices = plantMatrices.pointer();
        td3.cSVTs = channelSortedVTs;
        td3.bbox = bbox;
        td3.mb2tQuadXCount = mb2tQuadXCount;
        td3.mb2tQuadScale = mb2tQuadScale;
        td3.smgr = smgr;
        td3.mb2tVec = mb2tVec;

        //offset to start off from for each thread,every thread
        //does every 3rd processed plant position
        td3.start = 0;
        td3.mutexes = mb2tMutexes;
        td3.allVTLookUpTable = allVTLookUpTable.pointer();
        rc = pthread_create(threads+1, NULL, CopyMeshesIntoQuadsGrass, (void *)&td3);

        if (rc) {
            std::cout << "ERROR; return code from pthread_create() is " << rc << std::endl;
            join[1] = false;
        }

        meshCopyThreadData td4(&plantsCasted,&newPlantEstimate);
        td4.oldPlantEstimate = numberOfPlants;
        td4.posColours = positionColours.pointer();
        td4.plantMatrices = plantMatrices.pointer();
        td4.cSVTs = channelSortedVTs;
        td4.bbox = bbox;
        td4.mb2tQuadXCount = mb2tQuadXCount;
        td4.mb2tQuadScale = mb2tQuadScale;
        td4.smgr = smgr;
        td4.mb2tVec = mb2tVec;
        td4.start = 1;
        td4.mutexes = mb2tMutexes;
        td4.allVTLookUpTable = allVTLookUpTable.pointer();
        rc = pthread_create(threads+2, NULL, CopyMeshesIntoQuadsGrass, (void *)&td4);

        if (rc) {
            std::cout << "ERROR; return code from pthread_create() is " << rc << std::endl;
            join[2] = false;
        }

        meshCopyThreadData td5(&plantsCasted,&newPlantEstimate);
        td5.oldPlantEstimate = numberOfPlants;
        td5.posColours = positionColours.pointer();
        td5.plantMatrices = plantMatrices.pointer();
        td5.cSVTs = channelSortedVTs;
        td5.bbox = bbox;
        td5.mb2tQuadXCount = mb2tQuadXCount;
        td5.mb2tQuadScale = mb2tQuadScale;
        td5.smgr = smgr;
        td5.mb2tVec = mb2tVec;
        td5.start = 2;
        td5.mutexes = mb2tMutexes;
        td5.allVTLookUpTable = allVTLookUpTable.pointer();
        rc = pthread_create(threads+3, NULL, CopyMeshesIntoQuadsGrass, (void *)&td5);

        if (rc) {
            std::cout << "ERROR; return code from pthread_create() is " << rc << std::endl;
            join[3] = false;
        }
    } else {
        join[1] = false;
        join[2] = false;
        join[3] = false;
    }

    for(u32 i=0; i<4; i++) {
        if (join[i])
            (void) pthread_join(threads[i],NULL);
        join[i] = true;
    }

    //the OctTree Triangle selector has been dropped in the raycasting thread
    pTerrainModel = NULL;
    if (mbtanSize>0) {
        meshCopyThreadDataBush td5[] = {meshCopyThreadDataBush(bbox),meshCopyThreadDataBush(bbox),meshCopyThreadDataBush(bbox),meshCopyThreadDataBush(bbox)};
        for (u32 i=0; i<4; i++) {
            td5[i].oldPlantEstimate = numberOfPlants;
            td5[i].posColours = positionColours.pointer();
            td5[i].plantMatrices = plantMatrices.pointer();
            td5[i].cSVTs = channelSortedVTs;
            td5[i].mbtanQuadXCount = mbtanQuadXCount;
            td5[i].mbtanQuadScale = mbtanQuadScale;
            td5[i].smgr = smgr;
            td5[i].mbtanVec = mbtanVec;
            td5[i].allVTLookUpTable = allVTLookUpTable.pointer();
            td5[i].mbtanLookUpTable = mbtanLookUpTable;
            td5[i].mutexes = mbtanMutexes;
            td5[i].start = i;
            rc = pthread_create(threads+i, NULL, CopyMeshesIntoQuads, (void *)(td5+i));
            if (rc) {
                std::cout << "ERROR; return code from pthread_create() is " << rc << std::endl;
                join[i] = false;
            }
        }
        for(u32 i=0; i<4; i++) {
            if (join[i]) {
                (void) pthread_join(threads[i],NULL);
            }
            join[i] = true;
        }
    }

    plantMatrices.clear();
    positionColours.clear();
    std::cout << "Time Taken : " << irrlicht->getTimer()->getRealTime() - timeStart << std::endl;

    return;
}


#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "zlib.h"

#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif

#define CHUNK 262144

/* Compress from file source to file dest until EOF on source.
   def() returns Z_OK on success, Z_MEM_ERROR if memory could not be
   allocated for processing, Z_STREAM_ERROR if an invalid compression
   level is supplied, Z_VERSION_ERROR if the version of zlib.h and the
   version of the library linked do not match, or Z_ERRNO if there is
   an error reading or writing the files. */
int def(FILE *source, FILE *dest, int level)
{
    int ret, flush;
    unsigned have;
    z_stream strm;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];

    /* allocate deflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit(&strm, level);
    if (ret != Z_OK)
        return ret;

    /* compress until end of file */
    do {
        strm.avail_in = fread(in, 1, CHUNK, source);
        if (ferror(source)) {
            (void)deflateEnd(&strm);
            return Z_ERRNO;
        }
        flush = feof(source) ? Z_FINISH : Z_NO_FLUSH;
        strm.next_in = in;

        /* run deflate() on input until output buffer not full, finish
           compression if all of source has been read in */
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = deflate(&strm, flush);    /* no bad return value */
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            have = CHUNK - strm.avail_out;
            if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
                (void)deflateEnd(&strm);
                return Z_ERRNO;
            }
        } while (strm.avail_out == 0);
        assert(strm.avail_in == 0);     /* all input will be used */

        /* done when last data in file processed */
    } while (flush != Z_FINISH);
    assert(ret == Z_STREAM_END);        /* stream will be complete */

    /* clean up and return */
    (void)deflateEnd(&strm);
    return Z_OK;
}

/* report a zlib or i/o error */
void zerr(int ret)
{
    fputs("zpipe: ", stderr);

    switch (ret) {
        case Z_ERRNO:
            if (ferror(stdin)) {
                fputs("error reading stdin\n", stderr);
            }
            if (ferror(stdout)) {
                fputs("error writing stdout\n", stderr);
            }
            break;
        case Z_STREAM_ERROR:
            fputs("invalid compression level\n", stderr);
            break;
        case Z_DATA_ERROR:
            fputs("invalid or incomplete deflate data\n", stderr);
            break;
        case Z_MEM_ERROR:
            fputs("out of memory\n", stderr);
            break;
        case Z_VERSION_ERROR:
            fputs("zlib version mismatch!\n", stderr);
        default:
            break;
    }
}

bool CGrassGenerator::writeOut(const irr::io::path& Filename)
{
    if (!(mb2tQuadXCount&&mb2tQuadScale&&mb2tVec.allocated_size() != 0&&pTerrainNode)) {
        std::cout << "GRASS GENERATOR ERROR: Did you run CGrassGenerator::load(..) and CGrassGenerator::process()" << std::endl;
        return false;
    }

    io::IWriteFile* file = fileSystem->createAndWriteFile("./output2398913.grs",false);
    if (!file) {
        std::cout << "GRASS GENERATOR ERROR: Could not write out data to file (is disk full?)" << std::endl;
        return false;
    }

    file->write("FrontWarriorGrassFile\n",23);
    u32 endianCheck = 0xfdb97531;
    file->write(&endianCheck,4);
    core::aabbox3df bbox = pTerrainNode->getBoundingBox();
    pTerrainNode->remove();
    pTerrainNode = NULL;
    file->write(&bbox.MaxEdge.X,12);
    file->write(&bbox.MinEdge.X,12);
    file->write(&scale.X,8);
    file->write(&up_axis,1);

    u8 temp=channelSortedVTs[1].size();
    file->write(&temp,1);
    for (u32 i=0; i<channelSortedVTs[1].size(); i++) {

        if (mb2tQuadXCount[i]<1.f||(mb2tVec[i] == 0)) {
            temp='\0';
            file->write(&temp,1);
            continue;
        }

        io::path texture = fileSystem->getFileBasename(channelSortedVTs[1][i].texturePath);
        file->write(texture.c_str(),texture.size());
        temp='\0';
        file->write(&temp,1);
        file->write(mb2tQuadScale+i,4);
        u32 tempu32;
        u32 indexCount = channelSortedVTs[1][i].pMesh->getMeshBuffer(0)->getIndexCount();
        file->write(&indexCount,4);

        for (u32 j=0; j<(mb2tQuadXCount[i]*floorf(mb2tQuadScale[i]/scale.Y)+mb2tQuadXCount[i]); j++) {
            tempu32 = mb2tVec[i][j].getVertexCount();
            file->write(&tempu32,4);
            tempu32 = mb2tVec[i][j].getIndexCount();
            file->write(&tempu32,4);
            u32 plantCount = mb2tVec[i][j].getIndexCount()/channelSortedVTs[1][i].pMesh->getMeshBuffer(0)->getIndexCount();

            if (plantCount==0) {
                continue;
            }

            file->write(mb2tVec[i][j].getVertices(),sizeof(video::S3DVertex2TCoords)*mb2tVec[i][j].getVertexCount());
            file->write(mb2tVec[i][j].getIndices(),2*mb2tVec[i][j].getIndexCount());
            plantCount /= 2;
            if (plantCount==0) {
                continue;
            }

            u16* indicesLOD = new u16[indexCount*plantCount];
            for (u32 k=0; k<plantCount; k++) {
                for (u32 m=0; m<indexCount; m++) {
                    indicesLOD[k*indexCount+m] = mb2tVec[i][j].getIndices()[k*indexCount*2+m];
                }
            }
            file->write(indicesLOD,2*plantCount*indexCount);
            delete [] indicesLOD;
            plantCount /= 2;
            if (plantCount==0) {
                continue;
            }

            indicesLOD = new u16[indexCount*plantCount];
            for (u32 k=0; k<plantCount; k++) {
                for (u32 m=0; m<indexCount; m++) {
                    indicesLOD[k*indexCount+m] = mb2tVec[i][j].getIndices()[k*indexCount*4+m];
                }
            }
            file->write(indicesLOD,2*plantCount*indexCount);
            delete [] indicesLOD;
            plantCount /= 2;
            if (plantCount==0) {
                continue;
            }

            indicesLOD = new u16[indexCount*plantCount];
            for (u32 k=0; k<plantCount; k++) {
                for (u32 m=0; m<indexCount; m++) {
                    indicesLOD[k*indexCount+m] = mb2tVec[i][j].getIndices()[k*indexCount*8+m];
                }
            }
            file->write(indicesLOD,2*plantCount*indexCount);
            delete [] indicesLOD;
            plantCount /= 2;
            if (plantCount==0) {
                continue;
            }

            indicesLOD = new u16[indexCount*plantCount];
            for (u32 k=0; k<plantCount; k++) {
                for (u32 m=0; m<indexCount; m++) {
                    indicesLOD[k*indexCount+m] = mb2tVec[i][j].getIndices()[k*indexCount*16+m];
                }
            }
            file->write(indicesLOD,2*plantCount*indexCount);
            delete [] indicesLOD;
            plantCount /= 2;
            if (plantCount==0) {
                continue;
            }

            indicesLOD = new u16[indexCount*plantCount];
            for (u32 k=0; k<plantCount; k++) {
                for (u32 m=0; m<indexCount; m++) {
                    indicesLOD[k*indexCount+m] = mb2tVec[i][j].getIndices()[k*indexCount*32+m];
                }
            }
            file->write(indicesLOD,2*plantCount*indexCount);
            delete [] indicesLOD;
        }
    }

    temp=mbtanSize;
    file->write(&temp,1);

    for (u32 i=0,j=0; i<allVTs.size()&&j<mbtanSize; i++) {
        if ((allVTs[i].colorMaks&0x00000002)!=0x00000002) {
            if (mbtanQuadXCount[j]<1.f) {
                temp='\0';
                file->write(&temp,1);
                continue;
            }

            io::path texture = fileSystem->getFileBasename(allVTs[i].texturePath);
            file->write(texture.c_str(),texture.size());
            temp='\0';
            file->write(&temp,1);
            file->write(mbtanQuadScale+j,4);
            u32 tempu32;
            u32 indexCount = allVTs[i].pMesh->getMeshBuffer(0)->getIndexCount();
            file->write(&indexCount,4);

            for (u32 l=0; l<(mbtanQuadXCount[j]*floorf(mbtanQuadScale[j]/scale.Y)+mbtanQuadXCount[j]); l++) {
                mbtanVec[j][l].recalculateBoundingBox();
                tempu32 = mbtanVec[j][l].getVertexCount();
                file->write(&tempu32,4);
                tempu32 = mbtanVec[j][l].getIndexCount();
                file->write(&tempu32,4);
                u32 plantCount = mbtanVec[j][l].getIndexCount()/allVTs[i].pMesh->getMeshBuffer(0)->getIndexCount();
                if (plantCount==0) {
                    continue;
                }

                file->write(mbtanVec[j][l].getVertices(),sizeof(video::S3DVertexTangents)*mbtanVec[j][l].getVertexCount());
                file->write(mbtanVec[j][l].getIndices(),2*mbtanVec[j][l].getIndexCount());
                plantCount /= 2;
                if (plantCount==0) {
                    continue;
                }

                u16* indicesLOD = new u16[indexCount*plantCount];
                for (u32 k=0; k<plantCount; k++)
                {
                    for (u32 m=0; m<indexCount; m++)
                        indicesLOD[k*indexCount+m] = mbtanVec[j][l].getIndices()[k*indexCount*2+m];
                }
                file->write(indicesLOD,2*plantCount*indexCount);
                delete [] indicesLOD;
                plantCount /= 2;
                if (plantCount==0)
                    continue;
                indicesLOD = new u16[indexCount*plantCount];
                for (u32 k=0; k<plantCount; k++)
                {
                    for (u32 m=0; m<indexCount; m++)
                        indicesLOD[k*indexCount+m] = mbtanVec[j][l].getIndices()[k*indexCount*4+m];
                }
                file->write(indicesLOD,2*plantCount*indexCount);
                delete [] indicesLOD;
                plantCount /= 2;
                if (plantCount==0)
                    continue;
                indicesLOD = new u16[indexCount*plantCount];
                for (u32 k=0; k<plantCount; k++)
                {
                    for (u32 m=0; m<indexCount; m++)
                        indicesLOD[k*indexCount+m] = mbtanVec[j][l].getIndices()[k*indexCount*8+m];
                }
                file->write(indicesLOD,2*plantCount*indexCount);
                delete [] indicesLOD;
                plantCount /= 2;
                if (plantCount==0)
                    continue;
                indicesLOD = new u16[indexCount*plantCount];
                for (u32 k=0; k<plantCount; k++)
                {
                    for (u32 m=0; m<indexCount; m++)
                        indicesLOD[k*indexCount+m] = mbtanVec[j][l].getIndices()[k*indexCount*16+m];
                }
                file->write(indicesLOD,2*plantCount*indexCount);
                delete [] indicesLOD;
                plantCount /= 2;
                if (plantCount==0)
                    continue;
                indicesLOD = new u16[indexCount*plantCount];
                for (u32 k=0; k<plantCount; k++)
                {
                    for (u32 m=0; m<indexCount; m++)
                        indicesLOD[k*indexCount+m] = mbtanVec[j][l].getIndices()[k*indexCount*32+m];
                }
                file->write(indicesLOD,2*plantCount*indexCount);
                delete [] indicesLOD;
            }
            j++;
        }
    }

    for (int i = 0; i < (int)mbtanVec.size(); i++) {
        if (mbtanVec[i] != NULL) {
            delete [] mbtanVec[i];
            mbtanVec[i] = NULL;
        }
    }

    if (mbtanQuadScale) {
        delete [] mbtanQuadScale;
        mbtanQuadScale = NULL;
    }

    if (mbtanQuadXCount) {
        delete [] mbtanQuadXCount;
        mbtanQuadXCount = NULL;
    }

    for (int i = 0; i < (int)mb2tVec.size(); i++) {
        if (mb2tVec[i] != NULL) {
            delete [] mb2tVec[i];
            mb2tVec[i] = NULL;
        }
    }

    if (mb2tQuadScale) {
        delete [] mb2tQuadScale;
        mb2tQuadScale = NULL;
    }

    if (mb2tQuadXCount) {
        delete [] mb2tQuadXCount;
        mb2tQuadXCount = NULL;
    }

    file->drop();
    FILE* fileRead = fopen("./output2398913.grs","r");
    FILE* outFile = fopen(Filename.c_str(),"w");
    SET_BINARY_MODE(fileRead);
    SET_BINARY_MODE(outFile);
    def(fileRead,outFile,5);
    fclose(fileRead);
    fclose(outFile);
    remove("./output2398913.grs");

    return true;
}

} // namespace GrassGenerator
