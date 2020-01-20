/** Example Spherical Harmonics && Global Illumination

This example bakes an entire level into a spherical harmonic 32 bit float map
We will use a 128bit per pixel Render target
2nd order SH needs 9 coefficients, each with 3 channels
working out to be 27*32bits
With the RTT at 16 bytes and SH coeffs at 108 bytes we need 6.75 pixels per spherical harmonic
This demo first shows a travelling sphere for which the SH are interpolated trilinearly

The aim would be to produce a deferred renderer which gathers all the SH in camera frustum
resampled and LoDed from a OctTree like structure into a texture which is used to globally
illuminate every pixel

*/
#include <irrlicht.h>
#include <math.h>

float sinc(float x) {               /* Supporting sinc function */
  if (fabs(x) < 1.0e-4) return 1.0 ;
  else return(sin(x)/x) ;
}

using namespace irr;

f32 SH[27];
f32 sunAngle, sunHeight;
core::vector3df Wldoff,WldSz;
core::vector3df LightVec(0.707,0.707,0.0);


class MyEventReceiver : public IEventReceiver
{
public:
	// This is the one method that we have to implement
	virtual bool OnEvent(const SEvent& event)
	{
		// Remember whether each key is down or up
		if (event.EventType == irr::EET_KEY_INPUT_EVENT)
		{
			if (event.KeyInput.Key==irr::KEY_KEY_D)
                sunAngle+=0.012;
			else if (event.KeyInput.Key==irr::KEY_KEY_A)
                sunAngle-=0.012;
			else if (event.KeyInput.Key==irr::KEY_KEY_W)
                sunHeight+=0.012;
			else if (event.KeyInput.Key==irr::KEY_KEY_S)
                sunHeight-=0.012;
            else
                return false;
            LightVec.X = cosf(sunAngle);
            LightVec.Z = sinf(sunAngle);
            LightVec.Y = sunHeight;
            printf("SUN  X: %f  Y: %f  Z: %f \n", LightVec.X,LightVec.Y,LightVec.Z);
            LightVec.normalize();
		}

		return false;
	}

	MyEventReceiver()
	{
	}
};

class MyShaderCallBack : public video::IShaderConstantSetCallBack
{
public:

	virtual void OnSetConstants(video::IMaterialRendererServices* services,
			s32 userData)
	{
	    core::matrix4 mat = services->getVideoDriver()->getTransform(video::ETS_WORLD);
	    services->setPixelShaderConstant("Wmat",mat.pointer(),16);
	    mat.setTranslation(core::vector3df(0.f));
	    mat.setScale(core::vector3df(1.f));
	    services->setPixelShaderConstant("SH",SH,27);
	    services->setPixelShaderConstant("WNmat",mat.pointer(),16);
	    services->setPixelShaderConstant("Wldoff",&Wldoff.X,3);
	    services->setPixelShaderConstant("WldSz",&WldSz.X,3);
	    services->setPixelShaderConstant("LightVec",&LightVec.X,3);
	    int i=1;
	    services->setPixelShaderConstant("SHatlas2",(float*)&i,1);
	    i=2;
	    services->setPixelShaderConstant("SHatlas3",(float*)&i,1);
	    i=3;
	    services->setPixelShaderConstant("SHatlas4",(float*)&i,1);
	}
};


//! A SH will be created every N world units
#define SH_APART_DIST 48.0
//! How big the cubemaps will be for the spherical harmonics
#define SH_CUBEMAP_SZ 40
//! PreBAking?
//#define BAKE

//! dir, 0=front, 1=right, 2=back, 3=left, 4=up and 5=down
void addCoeff(float* coeff, video::ITexture* rtt, c8 dir, float &fwtSum) {
    u32* pixel = (u32*)rtt->lock(true);
    u32 temp;
    for (u32 i=0; i<SH_CUBEMAP_SZ; i++) {
        for (u32 j=0; j<SH_CUBEMAP_SZ; j++) {
            temp = pixel[i*SH_CUBEMAP_SZ+j]; // first across then up, BGRA order (most probs because of little endianness)
            core::vector3df col = core::vector3df(*(((u8*)(&temp))+2),*(((u8*)(&temp))+1),*(((u8*)(&temp))+0))/255.f;

            float u,v,x,y,z,domega,c,fwt;
            u = (j-SH_CUBEMAP_SZ/2.0)/(SH_CUBEMAP_SZ/2.0)-1.f/SH_CUBEMAP_SZ;    /* u ranges from -1 to 1 */
            v = (SH_CUBEMAP_SZ/2.0-i)/(SH_CUBEMAP_SZ/2.0)-1.f/SH_CUBEMAP_SZ;  /* v ranges from -1 to 1 */
            fwt = u*u+v*v+1.f;
            c = sqrt(fwt);
            if (dir==0) {
                x=u;
                y=v;
                z=1.f;
            }
            else if (dir==1) {
                x=1.f;
                y=v;
                z=-u;
            }
            else if (dir==2) {
                x=-u;
                y=v;
                z=-1.f;
            }
            else if (dir==3) {
                x=-1.f;
                y=v;
                z=u;
            }
            else if (dir==4) {
                x=u;
                y=1.f;
                z=v;
            }
            else if (dir==5) {
                x=-u;
                y=-1.f;
                z=-v;
            }
            x /= c;
            y /= c;
            z /= c;

            c = 1.f/(fwt*c);
            fwtSum += c;
            domega = c;

            c = 0.282095*domega;
            coeff[0] += col.X*c;
            coeff[1] += col.Y*c;
            coeff[2] += col.Z*c;

            c = 0.488603*domega;
            coeff[3] += col.X*c*y;
            coeff[4] += col.Y*c*y;
            coeff[5] += col.Z*c*y;
            coeff[6] += col.X*c*z;
            coeff[7] += col.Y*c*z;
            coeff[8] += col.Z*c*z;
            coeff[9] += col.X*c*x;
            coeff[10] += col.Y*c*x;
            coeff[11] += col.Z*c*x;

            c = 1.092548*domega;
            coeff[12] += col.X*c*x*y;
            coeff[13] += col.Y*c*x*y;
            coeff[14] += col.Z*c*x*y;
            coeff[15] += col.X*c*y*z;
            coeff[16] += col.Y*c*y*z;
            coeff[17] += col.Z*c*y*z;
            coeff[21] += col.X*c*x*z;
            coeff[22] += col.Y*c*x*z;
            coeff[23] += col.Z*c*x*z;

            c = 1.092548*domega*(3.f*z*z-1.f);
            coeff[18] += col.X*c;
            coeff[19] += col.Y*c;
            coeff[20] += col.Z*c;

            c = 0.546274*domega*(x*x-y*y);
            coeff[24] += col.X*c;
            coeff[25] += col.Y*c;
            coeff[26] += col.Z*c;
        }
    }
    rtt->unlock();
}

void SHintoTexture(f32* TextureSHBuffer, video::IVideoDriver* driver, video::ITexture** SHatlas, u32* spherical_distance, core::dimension2du atlasSz, video::S3DVertex* vertices, u32* indices, video::SMaterial atlasMat)
{
            driver->beginScene(true, true, video::SColor(0,0,0,0));
            driver->setRenderTarget(SHatlas[0],true, true, video::SColor(255,200,200,200));
            driver->setViewPort(core::recti(0,0,atlasSz.Width*0.5,atlasSz.Height*0.5));
            driver->setMaterial(atlasMat);
            driver->drawVertexPrimitiveList(vertices, (spherical_distance[0]+1)*(spherical_distance[2]+1), indices, spherical_distance[0]*spherical_distance[2]*2*(spherical_distance[1]+1), video::EVT_STANDARD, scene::EPT_TRIANGLES, video::EIT_32BIT);
            driver->setViewPort(core::recti(atlasSz.Width*0.5,0,atlasSz.Width,atlasSz.Height*0.5));
            for (u32 i=0; i<=spherical_distance[1]; i++) { //y
                for (u32 j=0; j<=spherical_distance[2]; j++) {//z
                    for (u32 k=0; k<=spherical_distance[0]; k++) {//x
                        u32 index = k+j*(spherical_distance[0]+1)+i*(spherical_distance[0]+1)*(spherical_distance[2]+1);
                        u32 luidx =(j+i*(spherical_distance[2]+1)+k*(spherical_distance[1]+1)*(spherical_distance[2]+1))*27;
                        vertices[index].Pos.X = TextureSHBuffer[luidx+4];
                        vertices[index].Pos.Y = TextureSHBuffer[luidx+5];
                        vertices[index].Pos.Z = TextureSHBuffer[luidx+6];
                        vertices[index].Normal.Z = TextureSHBuffer[luidx+7];
                    }
                }
            }
            driver->drawVertexPrimitiveList(vertices, (spherical_distance[0]+1)*(spherical_distance[2]+1), indices, spherical_distance[0]*spherical_distance[2]*2*(spherical_distance[1]+1), video::EVT_STANDARD, scene::EPT_TRIANGLES, video::EIT_32BIT);
            driver->setViewPort(core::recti(0,atlasSz.Height*0.5,atlasSz.Width*0.5,atlasSz.Height));
            for (u32 i=0; i<=spherical_distance[1]; i++) { //y
                for (u32 j=0; j<=spherical_distance[2]; j++) {//z
                    for (u32 k=0; k<=spherical_distance[0]; k++) {//x
                        u32 index = k+j*(spherical_distance[0]+1)+i*(spherical_distance[0]+1)*(spherical_distance[2]+1);
                        u32 luidx =(j+i*(spherical_distance[2]+1)+k*(spherical_distance[1]+1)*(spherical_distance[2]+1))*27;
                        vertices[index].Pos.X = TextureSHBuffer[luidx+8];
                        vertices[index].Pos.Y = TextureSHBuffer[luidx+9];
                        vertices[index].Pos.Z = TextureSHBuffer[luidx+10];
                        vertices[index].Normal.Z = TextureSHBuffer[luidx+11];
                    }
                }
            }
            driver->drawVertexPrimitiveList(vertices, (spherical_distance[0]+1)*(spherical_distance[2]+1), indices, spherical_distance[0]*spherical_distance[2]*2*(spherical_distance[1]+1), video::EVT_STANDARD, scene::EPT_TRIANGLES, video::EIT_32BIT);
            driver->setViewPort(core::recti(atlasSz.Width*0.5,atlasSz.Height*0.5,atlasSz.Width,atlasSz.Height));
            for (u32 i=0; i<=spherical_distance[1]; i++) { //y
                for (u32 j=0; j<=spherical_distance[2]; j++) {//z
                    for (u32 k=0; k<=spherical_distance[0]; k++) {//x
                        u32 index = k+j*(spherical_distance[0]+1)+i*(spherical_distance[0]+1)*(spherical_distance[2]+1);
                        u32 luidx =(j+i*(spherical_distance[2]+1)+k*(spherical_distance[1]+1)*(spherical_distance[2]+1))*27;
                        vertices[index].Pos.X = TextureSHBuffer[luidx+12];
                        vertices[index].Pos.Y = TextureSHBuffer[luidx+13];
                        vertices[index].Pos.Z = TextureSHBuffer[luidx+14];
                        vertices[index].Normal.Z = TextureSHBuffer[luidx+15];
                    }
                }
            }
            driver->drawVertexPrimitiveList(vertices, (spherical_distance[0]+1)*(spherical_distance[2]+1), indices, spherical_distance[0]*spherical_distance[2]*2*(spherical_distance[1]+1), video::EVT_STANDARD, scene::EPT_TRIANGLES, video::EIT_32BIT);
            driver->setRenderTarget(SHatlas[1],true, true, video::SColor(255,200,200,200));
            driver->setViewPort(core::recti(0,0,atlasSz.Width*0.5,atlasSz.Height*0.5));
            for (u32 i=0; i<=spherical_distance[1]; i++) { //y
                for (u32 j=0; j<=spherical_distance[2]; j++) {//z
                    for (u32 k=0; k<=spherical_distance[0]; k++) {//x
                        u32 index = k+j*(spherical_distance[0]+1)+i*(spherical_distance[0]+1)*(spherical_distance[2]+1);
                        u32 luidx =(j+i*(spherical_distance[2]+1)+k*(spherical_distance[1]+1)*(spherical_distance[2]+1))*27;
                        vertices[index].Pos.X = TextureSHBuffer[luidx+16];
                        vertices[index].Pos.Y = TextureSHBuffer[luidx+17];
                        vertices[index].Pos.Z = TextureSHBuffer[luidx+18];
                        vertices[index].Normal.Z = TextureSHBuffer[luidx+19];
                    }
                }
            }
            driver->drawVertexPrimitiveList(vertices, (spherical_distance[0]+1)*(spherical_distance[2]+1), indices, spherical_distance[0]*spherical_distance[2]*2*(spherical_distance[1]+1), video::EVT_STANDARD, scene::EPT_TRIANGLES, video::EIT_32BIT);
            driver->setViewPort(core::recti(atlasSz.Width*0.5,0,atlasSz.Width,atlasSz.Height*0.5));
            for (u32 i=0; i<=spherical_distance[1]; i++) { //y
                for (u32 j=0; j<=spherical_distance[2]; j++) {//z
                    for (u32 k=0; k<=spherical_distance[0]; k++) {//x
                        u32 index = k+j*(spherical_distance[0]+1)+i*(spherical_distance[0]+1)*(spherical_distance[2]+1);
                        u32 luidx =(j+i*(spherical_distance[2]+1)+k*(spherical_distance[1]+1)*(spherical_distance[2]+1))*27;
                        vertices[index].Pos.X = TextureSHBuffer[luidx+20];
                        vertices[index].Pos.Y = TextureSHBuffer[luidx+21];
                        vertices[index].Pos.Z = TextureSHBuffer[luidx+22];
                        vertices[index].Normal.Z = TextureSHBuffer[luidx+23];
                    }
                }
            }
            driver->drawVertexPrimitiveList(vertices, (spherical_distance[0]+1)*(spherical_distance[2]+1), indices, spherical_distance[0]*spherical_distance[2]*2*(spherical_distance[1]+1), video::EVT_STANDARD, scene::EPT_TRIANGLES, video::EIT_32BIT);
            driver->setViewPort(core::recti(0,atlasSz.Height*0.5,atlasSz.Width*0.5,atlasSz.Height));
            for (u32 i=0; i<=spherical_distance[1]; i++) { //y
                for (u32 j=0; j<=spherical_distance[2]; j++) {//z
                    for (u32 k=0; k<=spherical_distance[0]; k++) {//x
                        u32 index = k+j*(spherical_distance[0]+1)+i*(spherical_distance[0]+1)*(spherical_distance[2]+1);
                        u32 luidx =(j+i*(spherical_distance[2]+1)+k*(spherical_distance[1]+1)*(spherical_distance[2]+1))*27;
                        vertices[index].Pos.X = TextureSHBuffer[luidx+24];
                        vertices[index].Pos.Y = TextureSHBuffer[luidx+25];
                        vertices[index].Pos.Z = TextureSHBuffer[luidx+26];
                        vertices[index].Normal.Z = 1.f;
                    }
                }
            }
            driver->drawVertexPrimitiveList(vertices, (spherical_distance[0]+1)*(spherical_distance[2]+1), indices, spherical_distance[0]*spherical_distance[2]*2*(spherical_distance[1]+1), video::EVT_STANDARD, scene::EPT_TRIANGLES, video::EIT_32BIT);
            driver->setRenderTarget(0,true, true, video::SColor(0,0,0,0));
            driver->endScene();
}

int main()
{

	// create device and exit if creation failed

	IrrlichtDevice *device =
		createDevice(video::EDT_OPENGL, core::dimension2d<u32>(1280, 960));

	if (device == 0)
		return 1; // could not create selected driver.

    MyEventReceiver* rec = new MyEventReceiver();
    device->setEventReceiver(rec);
	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();


	device->getFileSystem()->addZipFileArchive("./map-20kdm2.pk3");

	scene::IAnimatedMesh* mesh = smgr->getMesh("20kdm2.bsp");
	scene::ISceneNode* node = 0;
	scene::ISceneNode* node2 = 0;
	s32 newMaterialType = 0;

	if (mesh) {
		node = smgr->addOctreeSceneNode(mesh->getMesh(0), 0, -1, 1024);
        node2 = smgr->addMeshSceneNode(smgr->getMesh("./bunny.obj"),0,-1,core::vector3df(SH_APART_DIST)+core::vector3df(-1300,-144,-1249));
		MyShaderCallBack* mc = new MyShaderCallBack();
        newMaterialType = driver->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles(
				"./shader.vert", "vertexMain", video::EVST_VS_2_0,
				"./shader.frag", "pixelMain", video::EPST_PS_2_0,
				mc, video::EMT_SOLID);
        node2->setMaterialType((video::E_MATERIAL_TYPE)newMaterialType);
        mc->drop();
	}


	if (node)
		node->setPosition(core::vector3df(-1300,-144,-1249));

    //! Now get The Bounding box so we can subdivide it into SH
    node->updateAbsolutePosition();
    core::aabbox3df bbox = node->getTransformedBoundingBox();
    Wldoff = bbox.MinEdge;
    WldSz = bbox.getExtent();
    u32 spherical_distance[3] = {floorf((bbox.MaxEdge.X-bbox.MinEdge.X)/SH_APART_DIST)-1,floorf((bbox.MaxEdge.Y-bbox.MinEdge.Y)/SH_APART_DIST)-1,floorf((bbox.MaxEdge.Z-bbox.MinEdge.Z)/SH_APART_DIST)-1};
    float pixel_cnt = float((spherical_distance[0]+1)*(spherical_distance[1]+1)*(spherical_distance[2]+1))*6.75;
    if (floorf(pixel_cnt)<pixel_cnt)
        pixel_cnt = floorf(pixel_cnt)+1;
    f32* TextureSHBuffer = new f32[u32(pixel_cnt*4)];
    f32* TextureSHNormalBuffer = new f32[u32(pixel_cnt*4)];

    printf("SHs X: %i Y: %i Z: %i \n Total:  %i   Pixels: %f\n", spherical_distance[0], spherical_distance[1], spherical_distance[2],
                                                                spherical_distance[0]*spherical_distance[1]*spherical_distance[2],
                                                                pixel_cnt);

#ifdef BAKE
    node->setMaterialFlag(video::EMF_BACK_FACE_CULLING,false);
	scene::ICameraSceneNode* cam = smgr->addCameraSceneNode();
	cam->setFOV(core::PI/2.f);
	video::ITexture* cubemap = driver->addRenderTargetTexture(core::dimension2du(SH_CUBEMAP_SZ,SH_CUBEMAP_SZ),"cubemap",video::ECF_A8R8G8B8);
	f32 SHcoeff[27];

    u32 lastTime = device->getTimer()->getRealTime();
    for (u32 i = 0; i<=spherical_distance[0]; i++) {
        u32 time = device->getTimer()->getRealTime();
        for (u32 j = 0; j<=spherical_distance[1]; j++) {
            for (u32 k = 0; k<=spherical_distance[2]; k++) {
                cam->setPosition(core::vector3df(i+1,j+1,k+1)*SH_APART_DIST+bbox.MinEdge);
                cam->updateAbsolutePosition();
                //! zero SHcoeffs
                for (u32 l=0; l<27; l++)
                    SHcoeff[l] = 0.f;
                float fwtSum = 0;

                cam->setTarget(cam->getPosition()+core::vector3df(0.f,0.f,1.f));
                driver->beginScene(true, true, video::SColor(255,0,0,0));
                driver->setRenderTarget(cubemap,true, true, video::SColor(255,200,200,200));
                smgr->drawAll();
                driver->endScene();
                addCoeff(SHcoeff,cubemap,0,fwtSum);

                cam->setTarget(cam->getPosition()+core::vector3df(1.f,0.f,0.f));
                driver->beginScene(true, true, video::SColor(255,0,0,0));
                driver->setRenderTarget(cubemap,true, true, video::SColor(255,200,200,200));
                smgr->drawAll();
                driver->endScene();
                addCoeff(SHcoeff,cubemap,1,fwtSum);

                cam->setTarget(cam->getPosition()+core::vector3df(0.f,0.f,-1.f));
                driver->beginScene(true, true, video::SColor(255,0,0,0));
                driver->setRenderTarget(cubemap,true, true, video::SColor(255,200,200,200));
                smgr->drawAll();
                driver->endScene();
                addCoeff(SHcoeff,cubemap,2,fwtSum);

                cam->setTarget(cam->getPosition()+core::vector3df(-1.f,0.f,0.f));
                driver->beginScene(true, true, video::SColor(255,0,0,0));
                driver->setRenderTarget(cubemap,true, true, video::SColor(255,200,200,200));
                smgr->drawAll();
                driver->endScene();
                addCoeff(SHcoeff,cubemap,3,fwtSum);

                cam->setTarget(cam->getPosition()+core::vector3df(0.f,1.f,0.f));
                driver->beginScene(true, true, video::SColor(255,0,0,0));
                driver->setRenderTarget(cubemap,true, true, video::SColor(255,200,200,200));
                smgr->drawAll();
                driver->endScene();
                addCoeff(SHcoeff,cubemap,4,fwtSum);

                cam->setTarget(cam->getPosition()+core::vector3df(0.f,-1.f,0.f));
                driver->beginScene(true, true, video::SColor(255,0,0,0));
                driver->setRenderTarget(cubemap,true, true, video::SColor(255,200,200,200));
                smgr->drawAll();
                driver->endScene();
                addCoeff(SHcoeff,cubemap,5,fwtSum);

                fwtSum = 4.f*core::PI/(fwtSum*3.f);
                for (u32 p=0; p<27; p++)
                    SHcoeff[p] *= fwtSum;

                memcpy((void*)&TextureSHBuffer[(k+j*(spherical_distance[2]+1)+i*(spherical_distance[1]+1)*(spherical_distance[2]+1))*27],(void*)SHcoeff,27*4);
            }
            if ((time-lastTime)>2000) {
                printf("%f prcnt complete \n", float(i)/float(spherical_distance[0])*100.f);
                lastTime=time;
            }
        }
    }

    driver->beginScene(true, true, video::SColor(255,0,0,0));
    driver->setRenderTarget(0,true, true, video::SColor(255,200,200,200));
    driver->endScene();
    io::IWriteFile* file = device->getFileSystem()->createAndWriteFile("./baked.shi");
    file->write(TextureSHBuffer,u32(pixel_cnt*16));
    file->drop();
#else
    io::IReadFile* file = device->getFileSystem()->createAndOpenFile("./baked.shi");
    file->read(TextureSHBuffer,u32(pixel_cnt*16));
    for (u32 i=0; i<((pixel_cnt*4)/27); i++) {
        TextureSHBuffer[i*27] *= 0.886227;
        TextureSHBuffer[i*27+1] *= 0.886227;
        TextureSHBuffer[i*27+2] *= 0.886227;
        TextureSHBuffer[i*27+3] *= 0.511664*2.0;
        TextureSHBuffer[i*27+4] *= 0.511664*2.0;
        TextureSHBuffer[i*27+5] *= 0.511664*2.0;
        TextureSHBuffer[i*27+6] *= 0.511664*2.0;
        TextureSHBuffer[i*27+7] *= 0.511664*2.0;
        TextureSHBuffer[i*27+8] *= 0.511664*2.0;
        TextureSHBuffer[i*27+9] *= 0.511664*2.0;
        TextureSHBuffer[i*27+10] *= 0.511664*2.0;
        TextureSHBuffer[i*27+11] *= 0.511664*2.0;
        TextureSHBuffer[i*27+12] *= 0.429043*2.0;
        TextureSHBuffer[i*27+13] *= 0.429043*2.0;
        TextureSHBuffer[i*27+14] *= 0.429043*2.0;
        TextureSHBuffer[i*27+15] *= 0.429043*2.0;
        TextureSHBuffer[i*27+16] *= 0.429043*2.0;
        TextureSHBuffer[i*27+17] *= 0.429043*2.0;
        TextureSHBuffer[i*27+21] *= 0.429043*2.0;
        TextureSHBuffer[i*27+22] *= 0.429043*2.0;
        TextureSHBuffer[i*27+23] *= 0.429043*2.0;
        TextureSHBuffer[i*27+24] *= 0.429043;
        TextureSHBuffer[i*27+25] *= 0.429043;
        TextureSHBuffer[i*27+26] *= 0.429043;
    }
    file->drop();
    file = device->getFileSystem()->createAndOpenFile("./normalsbaked.shi");
    file->read(TextureSHNormalBuffer,u32(pixel_cnt*16));
    for (u32 i=0; i<((pixel_cnt*4)/27); i++) {
        TextureSHNormalBuffer[i*27] *= 0.886227;
        TextureSHNormalBuffer[i*27+1] *= 0.886227;
        TextureSHNormalBuffer[i*27+2] *= 0.886227;
        TextureSHNormalBuffer[i*27+3] *= 0.511664*2.0;
        TextureSHNormalBuffer[i*27+4] *= 0.511664*2.0;
        TextureSHNormalBuffer[i*27+5] *= 0.511664*2.0;
        TextureSHNormalBuffer[i*27+6] *= 0.511664*2.0;
        TextureSHNormalBuffer[i*27+7] *= 0.511664*2.0;
        TextureSHNormalBuffer[i*27+8] *= 0.511664*2.0;
        TextureSHNormalBuffer[i*27+9] *= 0.511664*2.0;
        TextureSHNormalBuffer[i*27+10] *= 0.511664*2.0;
        TextureSHNormalBuffer[i*27+11] *= 0.511664*2.0;
        TextureSHNormalBuffer[i*27+12] *= 0.429043*2.0;
        TextureSHNormalBuffer[i*27+13] *= 0.429043*2.0;
        TextureSHNormalBuffer[i*27+14] *= 0.429043*2.0;
        TextureSHNormalBuffer[i*27+15] *= 0.429043*2.0;
        TextureSHNormalBuffer[i*27+16] *= 0.429043*2.0;
        TextureSHNormalBuffer[i*27+17] *= 0.429043*2.0;
        TextureSHNormalBuffer[i*27+21] *= 0.429043*2.0;
        TextureSHNormalBuffer[i*27+22] *= 0.429043*2.0;
        TextureSHNormalBuffer[i*27+23] *= 0.429043*2.0;
        TextureSHNormalBuffer[i*27+24] *= 0.429043;
        TextureSHNormalBuffer[i*27+25] *= 0.429043;
        TextureSHNormalBuffer[i*27+26] *= 0.429043;
    }
    file->drop();
#endif

    float tilefactorX = sqrtf(4*(spherical_distance[0]+1)*(spherical_distance[1]+1)*(spherical_distance[2]+1));
    float tilefactorY = ceilf(tilefactorX/float(2.f*spherical_distance[2]+2.f));
    tilefactorX = ceilf(tilefactorX/float(2.f*spherical_distance[0]+2.f));
    core::dimension2du atlasSz = core::dimension2du((spherical_distance[0]+1)*tilefactorX,(spherical_distance[2]+1)*tilefactorY);
    core::dimension2df atlPixelSz = core::dimension2df(1.f/atlasSz.Width,1.f/atlasSz.Height);
    video::ITexture* SHatlas[4];
    atlasSz *= 2;
    SHatlas[0]  = driver->addRenderTargetTexture(atlasSz,"SHatlas0",video::ECF_A32B32G32R32F);
    SHatlas[1]  = driver->addRenderTargetTexture(atlasSz,"SHatlas1",video::ECF_A32B32G32R32F);
    SHatlas[2]  = driver->addRenderTargetTexture(atlasSz,"SHatlas2",video::ECF_A32B32G32R32F);
    SHatlas[3]  = driver->addRenderTargetTexture(atlasSz,"SHatlas3",video::ECF_A32B32G32R32F);

    {
        u32 vertex_cnt = (spherical_distance[0]+1)*(spherical_distance[1]+1)*(spherical_distance[2]+1)*7;
        video::S3DVertex* vertices = new video::S3DVertex[vertex_cnt];
        video::SMaterial atlasMat;
        atlasMat.setFlag(video::EMF_LIGHTING,false);
        atlasMat.setFlag(video::EMF_BACK_FACE_CULLING,false);
        MyShaderCallBack* mc = new MyShaderCallBack();
        u32 newMaterialType2 = driver->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles(
                    "./shaderAtl.vert", "vertexMain", video::EVST_VS_2_0,
                    "./shaderAtl.frag", "pixelMain", video::EPST_PS_2_0,
                    mc, video::EMT_SOLID);
        atlasMat.MaterialType = (video::E_MATERIAL_TYPE)newMaterialType2;
        mc->drop();
        tilefactorX = 1.f/tilefactorX;
        tilefactorY = 1.f/tilefactorY;
        for (u32 i=0; i<=spherical_distance[1]; i++) { //y
            float offsetX = (1.f+i%int(1.f/tilefactorX))*tilefactorX-tilefactorX;
            float offsetY = (1.f+floorf(float(i)*tilefactorY))*tilefactorY-tilefactorY;
            for (u32 j=0; j<=spherical_distance[2]; j++) {//z
                float zAKAy = (float(j)/float(spherical_distance[2])*tilefactorY+offsetY)*2.f-1.f; //float(1+i%u32(tilefactorX))
                for (u32 k=0; k<=spherical_distance[0]; k++) {//x
                    u32 index = k+j*(spherical_distance[0]+1)+i*(spherical_distance[0]+1)*(spherical_distance[2]+1);
                    vertices[index].Normal.X = (float(k)/float(spherical_distance[0])*tilefactorX+offsetX)*2.f-1.f;
                    vertices[index].Normal.Y = zAKAy;
                    u32 luidx =(j+i*(spherical_distance[2]+1)+k*(spherical_distance[1]+1)*(spherical_distance[2]+1))*27;
                    vertices[index].Pos.X = TextureSHBuffer[luidx];
                    vertices[index].Pos.Y = TextureSHBuffer[luidx+1];
                    vertices[index].Pos.Z = TextureSHBuffer[luidx+2];
                    vertices[index].Normal.Z = TextureSHBuffer[luidx+3];
                }
            }
        }
        u32* indices = new u32[(spherical_distance[1]+1)*spherical_distance[0]*spherical_distance[2]*6];
        for (u32 k=0; k<=spherical_distance[1]; k++) {
            for (u32 j=0; j<spherical_distance[2]; j++) {
                for (u32 i=0; i<spherical_distance[0]; i++) {
                    u32 indexPart2 = spherical_distance[0]*j+i+spherical_distance[0]*spherical_distance[2]*k;
                    u32 indexPart = indexPart2+j+k+k*(spherical_distance[0]+spherical_distance[2]);
                    indices[indexPart2*6] = indexPart;
                    indices[indexPart2*6+1] = indexPart+1;
                    indices[indexPart2*6+2] = (spherical_distance[0]+1)+indexPart;
                    indices[indexPart2*6+3] = (spherical_distance[0]+1)+indexPart+1;
                    indices[indexPart2*6+4] = (spherical_distance[0]+1)+indexPart;
                    indices[indexPart2*6+5] = indexPart+1;
                }
            }
        }
        SHintoTexture(TextureSHBuffer,driver,SHatlas,spherical_distance,atlasSz,vertices,indices,atlasMat);
        for (u32 i=0; i<=spherical_distance[1]; i++) { //y
            float offsetX = (1.f+i%int(1.f/tilefactorX))*tilefactorX-tilefactorX;
            float offsetY = (1.f+floorf(float(i)*tilefactorY))*tilefactorY-tilefactorY;
            for (u32 j=0; j<=spherical_distance[2]; j++) {//z
                float zAKAy = (float(j)/float(spherical_distance[2])*tilefactorY+offsetY)*2.f-1.f; //float(1+i%u32(tilefactorX))
                for (u32 k=0; k<=spherical_distance[0]; k++) {//x
                    u32 index = k+j*(spherical_distance[0]+1)+i*(spherical_distance[0]+1)*(spherical_distance[2]+1);
                    vertices[index].Normal.X = (float(k)/float(spherical_distance[0])*tilefactorX+offsetX)*2.f-1.f;
                    vertices[index].Normal.Y = zAKAy;
                    u32 luidx =(j+i*(spherical_distance[2]+1)+k*(spherical_distance[1]+1)*(spherical_distance[2]+1))*27;
                    vertices[index].Pos.X = TextureSHNormalBuffer[luidx];
                    vertices[index].Pos.Y = TextureSHNormalBuffer[luidx+1];
                    vertices[index].Pos.Z = TextureSHNormalBuffer[luidx+2];
                    vertices[index].Normal.Z = TextureSHNormalBuffer[luidx+3];
                }
            }
        }
        SHintoTexture(TextureSHNormalBuffer,driver,SHatlas+2,spherical_distance,atlasSz,vertices,indices,atlasMat);
    }

	node2->setParent(smgr->addCameraSceneNodeFPS());
	node2->setPosition(core::vector3df(0.5,-0.5,2.f));
	node2->setScale(core::vector3df(4.f));
	device->getCursorControl()->setVisible(false);
	int lastFPS = -1;
	node2->setMaterialTexture(0,SHatlas[0]);
	node2->setMaterialTexture(1,SHatlas[1]);
	node2->setMaterialTexture(2,SHatlas[2]);
	node2->setMaterialTexture(3,SHatlas[3]);
	//node->setMaterialType(video::EMT_SOLID); //if you dont want lightmaps
	video::SMaterial oldMaterials[node->getMaterialCount()];
    for (u32 j=0; j<node->getMaterialCount(); j++)
        oldMaterials[j] = node->getMaterial(j);
    video::ITexture* screen = driver->addRenderTargetTexture(core::dimension2d<u32>(1280, 960),"screen");
    video::ITexture* screen2 = driver->addRenderTargetTexture(core::dimension2d<u32>(1280, 960),"screen2");
    video::SMaterial quadMat;
    quadMat.setTexture(0,screen);
    quadMat.setTexture(1,screen2);
    MyShaderCallBack* mc = new MyShaderCallBack();
    u32 newMaterialType2 = driver->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles(
                "./shaderQuad.vert", "vertexMain", video::EVST_VS_2_0,
                "./shaderQuad.frag", "pixelMain", video::EPST_PS_2_0,
                mc, video::EMT_SOLID);
    quadMat.MaterialType = (video::E_MATERIAL_TYPE)newMaterialType2;
    mc->drop();
    video::S3DVertex vertices[4];
    vertices[0] = video::S3DVertex(
                  -1.0f,-1.0f,0.5f,
                  0.0f,0.0f,-1.0f,
                  video::SColor(255,255,255,255),
                  0,0);
    vertices[1] = video::S3DVertex(
                  1.0f,-1.0,0.5f,
                  0.0f,0.0f,-1.0f,
                  video::SColor(255,255,255,255),
                  1,0);
    vertices[2] = video::S3DVertex(
                  -1.0f,1.0,0.5f,
                  0.0f,0.0f,-1.0f,
                  video::SColor(255,255,255,255),
                  0,1);
    vertices[3] = video::S3DVertex(
                  1.0f,1.0f,0.0f,
                  0.0f,0.0f,-1.0f,
                  video::SColor(255,255,255,255),
                  1,1);

	while(device->run())
	{
		if (device->isWindowActive())
		{
			driver->beginScene(true, true, video::SColor(255,200,200,200));

			driver->setRenderTarget(screen,true,true, video::SColor(255,255,255,255));
			node2->setMaterialType((video::E_MATERIAL_TYPE)newMaterialType);
            node2->setMaterialTexture(0,SHatlas[0]);
            node2->setMaterialTexture(1,SHatlas[1]);
            node2->setMaterialTexture(2,SHatlas[2]);
            node2->setMaterialTexture(3,SHatlas[3]);
            node->setMaterialType((video::E_MATERIAL_TYPE)newMaterialType);
            node->setMaterialTexture(0,SHatlas[0]);
            node->setMaterialTexture(1,SHatlas[1]);
            node->setMaterialTexture(2,SHatlas[2]);
            node->setMaterialTexture(3,SHatlas[3]);
			smgr->drawAll();
			driver->setRenderTarget(screen2,true,true, video::SColor(255,200,200,200));
			node2->setMaterialType(video::EMT_SOLID);
			node2->setMaterialFlag(video::EMF_LIGHTING,false);
            node2->setMaterialTexture(0,driver->getTexture("./white.bmp"));
            for (u32 j=0; j<node->getMaterialCount(); j++)
                node->getMaterial(j) = oldMaterials[j];
			smgr->drawAll();
			driver->setRenderTarget(0,true,true, video::SColor(255,200,200,200));
            u16 indices[] = {2,1,0,3,1,2};
            driver->setMaterial(quadMat);
            driver->drawIndexedTriangleList(&vertices[0],4,&indices[0],2);
			driver->endScene();

			int fps = driver->getFPS();

			if (lastFPS != fps)
			{
				core::stringw str = L"SH [";
				str += driver->getName();
				str += "] FPS:";
				str += fps;

				device->setWindowCaption(str.c_str());
				lastFPS = fps;
			}
		}
		else
			device->yield();
	}

	device->drop();
	delete rec;
	delete []TextureSHBuffer;

	return 0;
}
