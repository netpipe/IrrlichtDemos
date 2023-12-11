#include <CVideoMaster.h>
#include <TheoraDataSource.h>

#ifdef _MSC_VER
  // goto warning, everything is safe though
  #pragma warning (disable : 4533)
#endif

using namespace irr;
using namespace video;
using namespace core;
using namespace io;

CVideoMaster::CVideoMaster(irr::IrrlichtDevice* pDevice, bool readAudio, irr::u32 workerThreads) :
              mDevice(pDevice), mDriver(0), mTimer(0), mVmgr(0), mAIF(0), mTimeThen(0)
{
  mDriver   = mDevice->getVideoDriver();
  mTimer    = mDevice->getTimer();

  if (!workerThreads) workerThreads = 1;

  mVmgr = new TheoraVideoManager(workerThreads);

  if (readAudio)
  {
    mAIF = new CAudioInterfaceFactory_OpenAL();
    mVmgr->setAudioInterfaceFactory(mAIF);
  }

  mTimeThen = (f32)mTimer->getRealTime();
}

CVideoMaster::~CVideoMaster()
{
  removeVideoClipAll();

  if (mVmgr) delete mVmgr;
  if (mAIF)  delete mAIF;
}

void CVideoMaster::processFrames()
{
  if (mIrrClip.empty()) return;

  for (u32 i = 0; i < mIrrClip.size(); ++i)
  {
    TheoraVideoFrame* frame = mIrrClip[i]->pTheoClip->getNextFrame();

    if (frame)
    {
      IImage* pImg  = mIrrClip[i]->pImg;
      void* imgData = pImg->lock();
      u8* frameData = frame->getBuffer();

      if (imgData && frameData)
      {
        memcpy(imgData, frameData, pImg->getImageDataSizeInBytes());
      }
      if (imgData) pImg->unlock();


      ITexture* pTex = mIrrClip[i]->pTex;
      void* texData  = pTex->lock();

      if(texData)
      {
        if (pTex->getSize() == pImg->getDimension())
        {
          memcpy(texData, frameData, pImg->getImageDataSizeInBytes());
        }
        else
        {
          pImg->copyToScaling(texData, pTex->getSize().Width, pTex->getSize().Height, ECF_A8R8G8B8, pTex->getPitch());
        }
        
        pTex->unlock();
      }

      mIrrClip[i]->pTheoClip->popFrame();
    }
  }
}

void CVideoMaster::update()
{
  processFrames();

  f32 timeNow   = (f32)mTimer->getRealTime();
  f32 timeDelta = (timeNow - mTimeThen) / 1000.0f;

  if (timeDelta > 0.25f) timeDelta = 0.05f;

  mVmgr->update(timeDelta);

  mTimeThen = timeNow;
}

SIrrVideoClip* CVideoMaster::getIrrClipByName(const irr::io::path &clipname, irr::u32 &out_clipindex)
{
  if (mIrrClip.empty()) return NULL;

  for (u32 i = 0; i < mIrrClip.size(); ++i)
  {
    if (mIrrClip[i]->clipName == clipname)
    {
      out_clipindex = i;
      return mIrrClip[i];
    }
  }

  return NULL;
}

SIrrVideoClip* CVideoMaster::getIrrClipByName(const irr::io::path &clipname)
{
  if (mIrrClip.empty()) return NULL;

  for (u32 i = 0; i < mIrrClip.size(); ++i)
  {
    if (mIrrClip[i]->clipName == clipname)
    {
      return mIrrClip[i];
    }
  }

  return NULL;
}

TheoraVideoClip* CVideoMaster::getTheoClipByName(const irr::io::path &clipname)
{
  if (mIrrClip.empty()) return NULL;

  for (u32 i = 0; i < mIrrClip.size(); ++i)
  {
    if (mIrrClip[i]->clipName == clipname)
    {
      return mIrrClip[i]->pTheoClip;
    }
  }

  return NULL;
}

irr::video::IImage* CVideoMaster::getImageByName(const irr::io::path &clipname)
{
  if (mIrrClip.empty()) return NULL;

  for (u32 i = 0; i < mIrrClip.size(); ++i)
  {
    if (mIrrClip[i]->clipName == clipname)
    {
      return mIrrClip[i]->pImg;
    }
  }

  return NULL;
}

irr::video::ITexture* CVideoMaster::getTextureByName(const irr::io::path &clipname)
{
  if (mIrrClip.empty()) return NULL;

  for (u32 i = 0; i < mIrrClip.size(); ++i)
  {
    if (mIrrClip[i]->clipName == clipname)
    {
      return mIrrClip[i]->pTex;
    }
  }

  return NULL;
}

const bool CVideoMaster::isPOT(const irr::core::dimension2du &size)
{
  u32 x = size.Width;
  u32 y = size.Height;

  bool xTrue = ((x != 0) && !(x & (x - 1)));
  bool yTrue = ((y != 0) && !(y & (y - 1)));

  return (xTrue && yTrue);
}

const irr::core::dimension2du CVideoMaster::getNextPOT(const irr::core::dimension2du &size, bool up)
{
  dimension2du temp(1, 1);

  if (isPOT(size))
  {
    temp = size;
    return temp;
  }

  while (temp.Width  < size.Width)  { temp.Width  *= 2; }
  while (temp.Height < size.Height) { temp.Height *= 2; }

  if (up)
  {
    return temp;
  }
  else
  {
    temp.Width  /= 2;
    temp.Height /= 2;
    return temp;
  }
}

irr::video::ITexture* CVideoMaster::addVideoClip(const irr::io::path &filename,
                                                 const irr::io::path &clipname,
                                                 E_SCALE_MODE        scaleMode,
                                                 bool                repeatPlayback,
                                                 bool                startPlayback,
                                                 bool                preloadIntoRAM)
{
  const bool oldFlag = mDriver->getTextureCreationFlag(ETCF_CREATE_MIP_MAPS);
  mDriver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, false);

  if(mDriver->queryFeature(EVDF_TEXTURE_NPOT))
    mDriver->setTextureCreationFlag(ETCF_ALLOW_NON_POWER_2, true);

  TheoraVideoClip *pClip = NULL;
  IImage          *pImg  = NULL;
  ITexture        *pTex  = NULL;

  if (filename == "" || clipname == "") goto FAIL_CLEANUP;

  if (preloadIntoRAM) pClip = mVmgr->createVideoClip(new TheoraMemoryFileDataSource(std::string(filename.c_str())), TH_BGRA);
  else                pClip = mVmgr->createVideoClip(std::string(filename.c_str()), TH_BGRA);

  if (!pClip) goto FAIL_CLEANUP;

  if (startPlayback) pClip->play();
  else               pClip->stop();

  pClip->setAutoRestart(repeatPlayback);

  const dimension2du sizeClip(pClip->getWidth(), pClip->getHeight());
  const dimension2du sizeScreen(mDriver->getScreenSize());

  pImg = mDriver->createImage(ECF_A8R8G8B8, sizeClip);

  if (!pImg) goto FAIL_CLEANUP;

  void* imgData = pImg->lock();
  if (imgData)
  {
    memset(imgData, 0xFF, pImg->getImageDataSizeInBytes());
    pImg->unlock();
  }

  switch (scaleMode)
  {
    case ESM_NONE : 
    {
      pTex = mDriver->addTexture(sizeClip, clipname, ECF_A8R8G8B8);
    } break;

    case ESM_WINDOW_FIT :
    {
      pTex = mDriver->addTexture(sizeScreen, clipname, ECF_A8R8G8B8);
    } break;

    case ESM_WINDOW_RATIO :
    {
      f32 ratioX = (f32)sizeScreen.Width  / (f32)sizeClip.Width;
      f32 ratioY = (f32)sizeScreen.Height / (f32)sizeClip.Height;

      if (ratioX < ratioY)
      {
        dimension2du dim(sizeScreen.Width, sizeClip.Height * (u32)ratioX);
        pTex = mDriver->addTexture(dim, clipname, ECF_A8R8G8B8);
      }
      else if (ratioX > ratioY)
      {
        dimension2du dim(sizeClip.Width * (u32)ratioY, sizeScreen.Height);
        pTex = mDriver->addTexture(dim, clipname, ECF_A8R8G8B8);
      }
      else
      {
        pTex = mDriver->addTexture(sizeScreen, clipname, ECF_A8R8G8B8);
      }
    } break;

    case ESM_POT_UP :
    {
      pTex = mDriver->addTexture(getNextPOT(sizeClip, true), clipname, ECF_A8R8G8B8);
    } break;

    case ESM_POT_DOWN :
    {
      pTex = mDriver->addTexture(getNextPOT(sizeClip, false), clipname, ECF_A8R8G8B8);
    } break;
  }

  if (!pTex) goto FAIL_CLEANUP;

  void* texData = pTex->lock();
  if(texData)
  {
    pImg->copyToScaling(texData, pTex->getSize().Width, pTex->getSize().Height, ECF_A8R8G8B8, pTex->getPitch());
    pTex->unlock();
  }

  SIrrVideoClip *temp = new SIrrVideoClip();
  temp->pTheoClip = pClip;
  temp->pImg      = pImg;
  temp->pTex      = pTex;
  temp->clipName  = clipname;

  mIrrClip.push_back(temp);
  mDriver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, oldFlag);
  return pTex;


  FAIL_CLEANUP :

    if (pClip) mVmgr->destroyVideoClip(pClip);
    if (pImg)  pImg->drop();
    if (pTex)  mDriver->removeTexture(pTex);
    mDriver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, oldFlag);
    return NULL;
}

irr::video::ITexture* CVideoMaster::addVideoClip(const irr::io::path           &filename,
                                                 const irr::io::path           &clipname,
                                                 const irr::core::dimension2du &scaleToSize,
                                                 bool                          repeatPlayback,
                                                 bool                          startPlayback,
                                                 bool                          preloadIntoRAM)
{
  const bool oldFlag = mDriver->getTextureCreationFlag(ETCF_CREATE_MIP_MAPS);
  mDriver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, false);

  if(mDriver->queryFeature(EVDF_TEXTURE_NPOT))
    mDriver->setTextureCreationFlag(ETCF_ALLOW_NON_POWER_2, true);

  TheoraVideoClip *pClip = NULL;
  IImage          *pImg  = NULL;
  ITexture        *pTex  = NULL;

  if (filename == "" || clipname == "") goto FAIL_CLEANUP;

  if (preloadIntoRAM) pClip = mVmgr->createVideoClip(new TheoraMemoryFileDataSource(std::string(filename.c_str())), TH_BGRA);
  else                pClip = mVmgr->createVideoClip(std::string(filename.c_str()), TH_BGRA);

  if (!pClip) goto FAIL_CLEANUP;

  if (startPlayback) pClip->play();
  else               pClip->stop();

  pClip->setAutoRestart(repeatPlayback);

  const dimension2du sizeClip(pClip->getWidth(), pClip->getHeight());
  const dimension2du sizeScreen(mDriver->getScreenSize());

  pImg = mDriver->createImage(ECF_A8R8G8B8, sizeClip);

  if (!pImg) goto FAIL_CLEANUP;

  void* imgData = pImg->lock();
  if (imgData)
  {
    memset(imgData, 0xFF, pImg->getImageDataSizeInBytes());
    pImg->unlock();
  }

  pTex = mDriver->addTexture(scaleToSize, clipname, ECF_A8R8G8B8);

  if (!pTex) goto FAIL_CLEANUP;

  void* texData = pTex->lock();
  if(texData)
  {
    pImg->copyToScaling(texData, pTex->getSize().Width, pTex->getSize().Height, ECF_A8R8G8B8, pTex->getPitch());
    pTex->unlock();
  }

  SIrrVideoClip *temp = new SIrrVideoClip();
  temp->pTheoClip = pClip;
  temp->pImg      = pImg;
  temp->pTex      = pTex;
  temp->clipName  = clipname;

  mIrrClip.push_back(temp);
  mDriver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, oldFlag);
  return pTex;

  FAIL_CLEANUP :

    if (pClip) mVmgr->destroyVideoClip(pClip);
    if (pImg)  pImg->drop();
    if (pTex)  mDriver->removeTexture(pTex);
    mDriver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, oldFlag);
    return NULL;
}

bool CVideoMaster::removeVideoClip(const irr::io::path &clipname, bool irrRemoveTexture, bool irrDropImage)
{
  u32 clipIndex;
  SIrrVideoClip* pIrrClip = getIrrClipByName(clipname, clipIndex);

  if (!pIrrClip) return false;

  if (pIrrClip->pTheoClip)                mVmgr->destroyVideoClip(pIrrClip->pTheoClip);
  if (pIrrClip->pImg && irrDropImage)     pIrrClip->pImg->drop();
  if (pIrrClip->pTex && irrRemoveTexture) mDriver->removeTexture(pIrrClip->pTex);

  delete pIrrClip;
  mIrrClip.erase(clipIndex);

  return true;
}

void CVideoMaster::removeVideoClipAll(bool irrRemoveTexture, bool irrDropImage)
{
  while (!mIrrClip.empty())
  {
    SIrrVideoClip* pIrrClip = mIrrClip.getLast();

    if (pIrrClip)
    {
      if (pIrrClip->pTheoClip)                mVmgr->destroyVideoClip(pIrrClip->pTheoClip);
      if (pIrrClip->pImg && irrDropImage)     pIrrClip->pImg->drop();
      if (pIrrClip->pTex && irrRemoveTexture) mDriver->removeTexture(pIrrClip->pTex);
      delete pIrrClip;
    }

    mIrrClip.erase(mIrrClip.size() - 1);
  }
}