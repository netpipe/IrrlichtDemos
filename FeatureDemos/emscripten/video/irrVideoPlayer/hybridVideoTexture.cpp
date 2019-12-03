#include "irrlicht.h"
#include "ITimer.h"
#include <avifile-0.7/avifile.h>

// Irrlicht Namespaces
using namespace irr;
using namespace video;

class TMovie
{
     public:   
	TMovie(ITimer* timer) : MediaFile(0), MediaStream(0), timer(timer),
				timePerFrame(40), oldtime(0),loop(false)
	{}

	~TMovie()
	{
		delete MediaStream;
                delete MediaFile;
	}
           
        void LoadMovie(char* filename)
        {
		MediaFile=avm::CreateReadFile(filename);
		MediaStream=MediaFile->GetStream(0,avm::IReadStream::Video);
		if (MediaStream)
		{
			MediaStream->SetDirection(true);
			MediaStream->StartStreaming();
			MediaStream->Seek(1);
			pSurface=MediaStream->GetFrame(true);
			MediaStreamInfo=MediaStream->GetStreamInfo();
		}
        }

	bool NextMovieFrame()
	{
		if(timer->getTime()-oldtime < timePerFrame)
		{
			return false;
		}
		oldtime = timer->getTime();
		pSurface=MediaStream->GetFrame(true);
		// Loop after movie end
		if (!pSurface)
		{
			if (!loop)
				return false;
			MediaStream->Seek(1);
			pSurface=MediaStream->GetFrame(true);
		}
		return true;
	}
 
	u32 getMovieWidth()
	{
		return (MediaStreamInfo->GetVideoWidth());
	}
 
	u32 getMovieHeight()
	{
		return (MediaStreamInfo->GetVideoHeight());
	}
           
	void DrawMovie(int x,int y,ITexture* texture)
	{
                const u32 pitch = texture->getPitch(); 
                u8* pTex = (u8*)texture->lock()+(y*pitch);
		CImage *image;
		u8 bpp;

		// Create image of correct depth from movie frame
		if (texture->getColorFormat()==ECF_A1R5G5B5)
		{
			image = new avm::CImage(pSurface, 15);
			bpp=2;
		}
		else
		{
			image = new avm::CImage(pSurface, 32);
			bpp=4;
		}

		uint8_t* pPixel=image->At(0,0);
		// Check for area to be drawn. Avoid access out of bound.
                u32 minWidth=(pitch<image->Bpl())?pitch:image->Bpl();
		u32 minHeight=(image->Height()<texture->getSize().Height)?image->Height():texture->getSize().Height;

		// copy movie frame to texture
                for(int h=0; h<minHeight; ++h)
		{
                	memcpy(pTex+x*bpp,pPixel,minWidth);
			// jump to next scanline
			pTex+=pitch;
			pPixel+=image->Bpl();
		}

		delete image;
                texture->unlock();
          } 
           
          void SetMovieFPS(double fps)
          {
                timePerFrame = 1000.0/fps;
          }
         
          void Stop()
          {
          }
         
          void Run()
          {
          }
              
          void MovieLock()
          {
          } 
           
          void MovieUnlock()
          { 
          }             
	private:
	avm::IReadFile*	MediaFile;
	avm::IReadStream*	MediaStream;
	avm::StreamInfo*	MediaStreamInfo;
	avm::CImage*	pSurface;
	ITimer*		timer;
	double		timePerFrame;
	u32		oldtime;
	bool loop;
};

using namespace core;
using namespace scene;

 class CSampleSceneNode : public ISceneNode
 {
       aabbox3d<f32> Box;
       S3DVertex Vertices[4];
       SMaterial Material;
       
    public:
       CSampleSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id): ISceneNode(parent, mgr, id)
        { 
            float u = 320.0/512.0;
            float v = 240.0/256.0;
            Material.Wireframe = false;
            Material.Lighting = false;
            Vertices[0] = S3DVertex(-10,-10,0, 0,0,0,SColor(255,255,255,255),0,v);
            Vertices[1] = S3DVertex( 10,-10,0, 0,0,0,SColor(255,255,255,255),u,v);
            Vertices[2] = S3DVertex( 10, 10,0, 0,0,0,SColor(255,255,255,255),u,0);
            Vertices[3] = S3DVertex(-10, 10,0, 0,0,0,SColor(255,255,255,255),0,0);
           
            Box.reset(Vertices[0].Pos);
            for (u32 i=1; i<4; ++i)
		    Box.addInternalPoint(Vertices[i].Pos);
        }
     
      virtual void OnRegisterSceneNode()
       {
           if (IsVisible)
		   SceneManager->registerNodeForRendering(this);
           ISceneNode::OnRegisterSceneNode();
       }
       
      virtual void render()
       { 
           u16 indices[] = { 0,1,3, 3,1,2, 1,0,2, 2,0,3 };
           IVideoDriver* driver = SceneManager->getVideoDriver();
           driver->setMaterial(Material);
           driver->setTransform(ETS_WORLD, AbsoluteTransformation);
           driver->drawIndexedTriangleList(&Vertices[0], 4, &indices[0], 4);
       }
 
      virtual const aabbox3d<f32>& getBoundingBox() const
       {
           return Box;
       }
 
      virtual u32 getMaterialCount()
       {
           return 1;
       }
 
      virtual SMaterial& getMaterial(u32 i)
       {
           return Material;
       }
 };
 
 int main()
 {
    IrrlichtDevice* irrDevice   = createDevice(EDT_OPENGL,dimension2d<s32>(800,600),32,false,false,false,0);
    IVideoDriver*   irrVideo    = irrDevice->getVideoDriver();
    ISceneManager*  irrSceneMgr = irrDevice->getSceneManager();
    TMovie* movie = new TMovie(irrDevice->getTimer());
    movie->LoadMovie("Mymovie.avi");
    irrVideo->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, false);
    u32 w = movie->getMovieWidth();
    u32 h = movie->getMovieHeight();
    ITexture* movTxtr = irrVideo->addTexture(dimension2d<s32>(w<513?512:1024,h<513?512:1024),"imovie"); 
    irrSceneMgr->addCameraSceneNode(0, vector3df(0,0,-20), vector3df(0,0,0));
    CSampleSceneNode* myNode = new CSampleSceneNode(irrSceneMgr->getRootSceneNode(), irrSceneMgr, 666);
    myNode->setMaterialTexture( 0, movTxtr);
    myNode->drop();
    ISceneNodeAnimator* anim = irrSceneMgr->createRotationAnimator(vector3df(0,0.1f,0));
    myNode->addAnimator(anim);
    anim->drop();
 
    while(irrDevice->run())
     {
        irrVideo->beginScene(true, true, SColor(0,200,200,200));
                  if (movie->NextMovieFrame())
                    movie->DrawMovie(0,0,movTxtr);
                  irrSceneMgr->drawAll();
        irrVideo->endScene();
     }       
    irrDevice->drop();

    return 0;
}

