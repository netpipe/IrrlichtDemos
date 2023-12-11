#include "CTheoraPlayer.h"


using namespace irr;

/* Helper; just grab some more compressed bitstream and sync it for
page extraction */
int buffer_data(FILE *in, ogg_sync_state *oy)
{
  char *buffer=ogg_sync_buffer(oy,4096);
  int bytes=fread(buffer,1,4096,in);
  ogg_sync_wrote(oy,bytes);
  return (bytes);
}

//------------------------------------------------------------------------------
//! Ctor
//! 
CTheoraPlayer::CTheoraPlayer() :
  iTexture(NULL), iInputFile(NULL), iTheoraPacketsCount(0), iState(CTPS_IDLE)
{
}

//------------------------------------------------------------------------------
//! Dtor
//! 
CTheoraPlayer::~CTheoraPlayer()
{

}

//------------------------------------------------------------------------------
//! CleanUp
//! 
void CTheoraPlayer::CleanUp()
{
  if (iTexture != NULL) { iTexture->drop(); iTexture == NULL; }
}

//------------------------------------------------------------------------------
//! Create
//! 
bool CTheoraPlayer::Create(irr::video::IVideoDriver* driver)
{
  iIrrVideodriver = driver;

  return true;
}

//------------------------------------------------------------------------------
//! OnUpdate
//! 
bool CTheoraPlayer::OnUpdate(irr::u32 timeMs)
{
  /* Finally the main decode loop. 

  It's one Theora packet per frame, so this is pretty 
  straightforward if we're not trying to maintain sync
  with other multiplexed streams.

  the videobuf_ready flag is used to maintain the input
  buffer in the libogg stream state. If there's no output
  frame available at the end of the decode step, we must
  need more input data. We could simplify this by just 
  using the return code on ogg_page_packetout(), but the
  flag system extends easily to the case were you care
  about more than one multiplexed stream (like with audio
  playback). In that case, just maintain a flag for each
  decoder you care about, and pull data when any one of
  them stalls.

  videobuf_time holds the presentation time of the currently
  buffered video frame. We ignore this value.
  */

  //if(fps_only){
  //  gettimeofday(&start,NULL);
  //  gettimeofday(&last,NULL);
  //}

  if (iState != CTPS_PLAYING)
    return true;

  /* single frame video buffering */
  int          videobuf_ready=0;
  ogg_int64_t  videobuf_granulepos=-1;
  double       videobuf_time=0;
  int          raw = 0;
  int          fps_only=0;

  while (iTheoraPacketsCount && !videobuf_ready)
  {
    /* theora is one in, one out... */
    if (ogg_stream_packetout(&iOggStreamState, &iOggPacket)>0)
    {
      theora_decode_packetin(&iTheoraDecoderState, &iOggPacket);
      videobuf_granulepos=iTheoraDecoderState.granulepos;
      videobuf_time=theora_granule_time(&iTheoraDecoderState, videobuf_granulepos);
      videobuf_ready=1;
      iFramesCount++;
    }
    else
      break;
  }


  if (!videobuf_ready && feof(iInputFile))
    return false;

  if (!videobuf_ready)
  {
    /* no data yet for somebody.  Grab another page */
    buffer_data(iInputFile, &iOggSyncState);
    while(ogg_sync_pageout(&iOggSyncState, &iOggPage)>0){
      queue_page(&iOggPage);
    }
  }
  /* dumpvideo frame, and get new one */
  else 
    video_write();

  videobuf_ready=0;

  return true;
}

//------------------------------------------------------------------------------
//! Play
//! 
bool CTheoraPlayer::Play(const irr::c8* fileName)
{
  iInputFile = fopen(fileName,"rb");
  if (iInputFile == NULL)
  {
    return false;
  }

  int stateflag=0;

  /*
  Ok, Ogg parsing. The idea here is we have a bitstream
  that is made up of Ogg pages. The libogg sync layer will
  find them for us. There may be pages from several logical
  streams interleaved; we find the first theora stream and
  ignore any others.

  Then we pass the pages for our stream to the libogg stream
  layer which assembles our original set of packets out of
  them. It's the packets that libtheora actually knows how
  to handle.
  */

  /* start up Ogg stream synchronization layer */
  ogg_sync_init(&iOggSyncState);

  /* init supporting Theora structures needed in header parsing */
  theora_comment_init(&iTheoraComment);
  theora_info_init(&iTheoraInfo);

  /* Ogg file open; parse the headers */

  /* Vorbis and Theora both depend on some initial header packets
  for decoder setup and initialization. We retrieve these first
  before entering the main decode loop. */

  /* Only interested in Theora streams */
  while (!stateflag)
  {
    int ret = buffer_data(iInputFile,&iOggSyncState);
    if (ret == 0)
      break;

    while (ogg_sync_pageout(&iOggSyncState,&iOggPage)>0)
    {
      ogg_stream_state test;

      /* is this a mandated initial header? If not, stop parsing */
      if (!ogg_page_bos(&iOggPage))
      {
        /* don't leak the page; get it into the appropriate stream */
        queue_page(&iOggPage);
        stateflag=1;
        break;
      }

      ogg_stream_init(&test,ogg_page_serialno(&iOggPage));
      ogg_stream_pagein(&test,&iOggPage);
      ogg_stream_packetout(&test,&iOggPacket);

      /* identify the codec: try theora */
      if (!iTheoraPacketsCount && theora_decode_header(&iTheoraInfo, &iTheoraComment, &iOggPacket)>=0)
      {
        /* it is theora -- save this stream state */
        memcpy(&iOggStreamState,&test,sizeof(test));
        iTheoraPacketsCount=1;
      }
      else
      {
        /* whatever it is, we don't care about it */
        ogg_stream_clear(&test);
      }
    }
    /* fall through to non-initial page parsing */
  }

  /* we're expecting more header packets. */
  while (iTheoraPacketsCount && iTheoraPacketsCount<3)
  {
    int ret;

    /* look for further theora headers */
    while (iTheoraPacketsCount && 
      (iTheoraPacketsCount<3) && 
      (ret=ogg_stream_packetout(&iOggStreamState, &iOggPacket))
      )
    {
      if (ret<0) 
      {
        fprintf(stderr,"Error parsing Theora stream headers; corrupt stream?\n");
        return false;
      }

      if (theora_decode_header(&iTheoraInfo, &iTheoraComment, &iOggPacket))
      {
        printf("Error parsing Theora stream headers; corrupt stream?\n");
        return false;
      }
      iTheoraPacketsCount++;
      if (iTheoraPacketsCount==3)
        break;
    }


    /* The header pages/packets will arrive before anything else we
    care about, or the stream is not obeying spec */
    if(ogg_sync_pageout(&iOggSyncState, &iOggPage)>0)
    {
      queue_page(&iOggPage); /* demux into the stream state */
    }
    else
    {
      int ret=buffer_data(iInputFile, &iOggSyncState); /* need more data */
      if(ret==0)
      {
        fprintf(stderr,"End of file while searching for codec headers.\n");
        return false;
      }
    }
  }

  /* Now we have all the required headers. initialize the decoder. */
  if (iTheoraPacketsCount)
  {
    theora_decode_init(&iTheoraDecoderState, &iTheoraInfo);
    fprintf(stderr,"Ogg logical stream %x is Theora %dx%d %.02f fps video\nEncoded frame content is %dx%d with %dx%d offset\n",
      (unsigned int)iOggStreamState.serialno,iTheoraInfo.width,iTheoraInfo.height, 
      (double)iTheoraInfo.fps_numerator/iTheoraInfo.fps_denominator,
      iTheoraInfo.frame_width, iTheoraInfo.frame_height, iTheoraInfo.offset_x, iTheoraInfo.offset_y);
  }
  else
  {
    /* tear down the partial theora setup */
    theora_info_clear(&iTheoraInfo);
    theora_comment_clear(&iTheoraComment);
  }

  /* queue any remaining pages from data we buffered but that did not
  contain headers */
  while (ogg_sync_pageout(&iOggSyncState, &iOggPage) > 0)
  {
    queue_page(&iOggPage);
  }

  iFramesCount = 0;


  // prepare texture
  if (iTexture != NULL) { iTexture->drop(); iTexture == NULL; }

  core::dimension2d<s32> size(iTheoraInfo.frame_width, iTheoraInfo.frame_height);
  const c8* name = "CTheoraTexture";
  video::ECOLOR_FORMAT format = video::ECF_A8R8G8B8;
  iTexture = iIrrVideodriver->addTexture(size, name, format);




  iState = CTPS_PLAYING;

  return true;
}

//------------------------------------------------------------------------------
//! Stop
//! 
bool CTheoraPlayer::Stop()
{
  if (iTheoraPacketsCount)
  {
    ogg_stream_clear(&iOggStreamState);
    theora_clear(&iTheoraDecoderState);
    theora_comment_clear(&iTheoraComment);
    theora_info_clear(&iTheoraInfo);
  }
  ogg_sync_clear(&iOggSyncState);

  if (iInputFile != NULL)
    fclose(iInputFile);

  iState = CTPS_IDLE;

  return true;
}

//------------------------------------------------------------------------------
//! Stop
//! 
irr::video::ITexture* CTheoraPlayer::GetTexture()
{
  return iTexture;
}

//------------------------------------------------------------------------------
//! queue_page
//! helper: push a page into the steam for packetization
int CTheoraPlayer::queue_page(ogg_page *page)
{
  if (iTheoraPacketsCount)
    ogg_stream_pagein(&iOggStreamState, &iOggPage);
  return 0;
}

//------------------------------------------------------------------------------
//! video_write
//! write out the planar YUV frame, uncropped
void CTheoraPlayer::video_write()
{
  int i, j;

  yuv_buffer yuv;
  theora_decode_YUVout(&iTheoraDecoderState, &yuv);

  u8* textureData = (u8*) iTexture->lock();
  u32 texturePitch = iTexture->getPitch();

  if (textureData)
  {
    for (i=0; i < yuv.y_height;i++)
    {
      u32* textureRowData = (u32*)textureData;
      u8*  yData = yuv.y+yuv.y_stride*i;
      for (j=0; j < yuv.y_width; j++)
      {
        u8 y = *yData;
        *textureRowData = (0x88 << 24) | (y << 16) | (y << 8) | y;

        textureRowData++; yData++;
      }
      
      textureData += texturePitch;
    }

    //for(i=0;i<yuv.y_height;i++)
    //  fwrite(yuv.y+yuv.y_stride*i, 1, yuv.y_width, outfile);
    //for(i=0;i<yuv.uv_height;i++)
    //  fwrite(yuv.u+yuv.uv_stride*i, 1, yuv.uv_width, outfile);
    //for(i=0;i<yuv.uv_height;i++)
    //  fwrite(yuv.v+yuv.uv_stride*i, 1, yuv.uv_width, outfile);
  }

  iTexture->unlock();
}
