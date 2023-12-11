#ifndef CLASS_VIDEO_MASTER_H_INC
#define CLASS_VIDEO_MASTER_H_INC

#include <irrlicht.h>
#include <TheoraPlayer.h>
#include "CAudioInterface_OpenAL.h"

enum E_SCALE_MODE
{
  // No scaling.
  ESM_NONE,
  // Scale to window size.
  ESM_WINDOW_FIT,
  // Scale width or height to window size and keep aspect ratio.
  // This doesn't always work correcty since Irrlicht might adjust texture dimensions internally
  // even if NPOT textures are enabled and supported by the hardware.
  ESM_WINDOW_RATIO,
  // Scale to nearest power-of-two, bigger than the original size.
  ESM_POT_UP,
  // Scale to nearest power-of-two, smaller than the original size.
  ESM_POT_DOWN
};

struct SIrrVideoClip
{
  TheoraVideoClip      *pTheoClip;
  irr::video::IImage   *pImg;
  irr::video::ITexture *pTex;
  irr::io::path        clipName;
};

class CVideoMaster
{
private :

  irr::IrrlichtDevice              *mDevice;
  irr::video::IVideoDriver         *mDriver;
  irr::ITimer                      *mTimer;
  TheoraVideoManager               *mVmgr;
  CAudioInterfaceFactory_OpenAL    *mAIF;
  irr::core::array<SIrrVideoClip*> mIrrClip;
  irr::f32                         mTimeThen;

  SIrrVideoClip* getIrrClipByName(const irr::io::path &clipname, irr::u32 &out_clipindex);

  void processFrames();

public :

  CVideoMaster(irr::IrrlichtDevice* pDevice, bool readAudio = true, irr::u32 workerThreads = 1);
  ~CVideoMaster();

  void update();
  const bool isPOT(const irr::core::dimension2du &size);
  const irr::core::dimension2du getNextPOT(const irr::core::dimension2du &size, bool up = false);

  SIrrVideoClip*        getIrrClipByName (const irr::io::path &clipname);
  TheoraVideoClip*      getTheoClipByName(const irr::io::path &clipname);
  irr::video::IImage*   getImageByName   (const irr::io::path &clipname);
  irr::video::ITexture* getTextureByName (const irr::io::path &clipname);

  irr::video::ITexture* addVideoClip(const irr::io::path &filename, const irr::io::path &clipname, E_SCALE_MODE scaleMode = ESM_NONE,          bool repeatPlayback = false, bool startPlayback = true, bool preloadIntoRAM = false);
  irr::video::ITexture* addVideoClip(const irr::io::path &filename, const irr::io::path &clipname, const irr::core::dimension2du &scaleToSize, bool repeatPlayback = false, bool startPlayback = true, bool preloadIntoRAM = false);

  bool removeVideoClip   (const irr::io::path &clipname, bool irrRemoveTexture = false, bool irrDropImage = true);
  void removeVideoClipAll(bool irrRemoveTexture = false, bool irrDropImage = true);
};

#endif
