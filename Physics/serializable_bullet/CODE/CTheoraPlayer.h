// Theora lib wrapper class

#ifndef __C_THEORA_PALYER_H_INCLUDED__
#define __C_THEORA_PALYER_H_INCLUDED__


//! Irrlicht
#include "irrlicht.h"

#include "theora/theora.h"

enum CTheoraPlayerStates
{
  CTPS_IDLE = 0,
  CTPS_PLAYING,
};

class CTheoraPlayer 
{
public:

  CTheoraPlayer();
  ~CTheoraPlayer();

  bool  Create(irr::video::IVideoDriver* driver);
  bool  OnUpdate(irr::u32 timeMs);

  irr::video::ITexture* GetTexture();
  bool  Play(const irr::c8* fileName);
  bool  Stop();

private:

  void  CleanUp();

  void video_write();
  int queue_page(ogg_page *page);

  irr::video::IVideoDriver* iIrrVideodriver;

  irr::video::ITexture* iTexture;
  FILE*                 iInputFile;

  /* never forget that globals are a one-way ticket to Hell */
  /* Ogg and codec state for demux/decode */
  ogg_sync_state        iOggSyncState;
  ogg_page              iOggPage;
  ogg_stream_state      iOggStreamState;
  theora_info           iTheoraInfo;
  theora_comment        iTheoraComment;
  theora_state          iTheoraDecoderState;
  int                   iTheoraPacketsCount;
  int                   iFramesCount;
  ogg_packet            iOggPacket;


  CTheoraPlayerStates   iState;

};

#endif //__C_THEORA_PALYER_H_INCLUDED__
