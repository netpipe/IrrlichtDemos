
#include "CTheoraPlayer.h"
#include <irrlicht.h>
#include <iostream>
using namespace irr;
using namespace scene;
using namespace video;
using namespace core;
using namespace io;
using namespace gui;

CTheoraPlayer::CTheoraPlayer(irr::IrrlichtDevice* device) :
iIrrDevice(device), iIrrVideoDriver(device->getVideoDriver()), iTexture(0),
iImage(0), iVideoFile(0), iTheoraPacketsCount(0), iState(ETPS_IDLE)
{

      iIrrVideoDriver->getMaterial2D().TextureLayer[0].TextureWrapU = (E_TEXTURE_CLAMP)1;
iIrrVideoDriver->getMaterial2D().TextureLayer[0].TextureWrapV = (E_TEXTURE_CLAMP)1;
iIrrVideoDriver->getMaterial2D().TextureLayer[1].TextureWrapU = ETC_CLAMP_TO_EDGE;
iIrrVideoDriver->getMaterial2D().TextureLayer[1].TextureWrapV = ETC_CLAMP_TO_EDGE;
   // create blank texture
   iBlankTexture = iIrrVideoDriver->addTexture(irr::core::dimension2du(1, 1), "CTheoraPlayerBlankTexture");

   irr::u8* data = (irr::u8*) iBlankTexture->lock();
   if (data)
   {
      memset(data, 0xFF, iBlankTexture->getPitch()*iBlankTexture->getSize().Height);
      iBlankTexture->unlock();
   }


}

CTheoraPlayer::~CTheoraPlayer()
{
   stop();
}
void CTheoraPlayer::setDevice(irr::IrrlichtDevice* deviceptr){

iIrrDevice = deviceptr;

}
bool CTheoraPlayer::load(const char* const fileName, const bool loop)
{
   // stop previous video
   stop();

   iIsLooped = loop;

   // try to open file
   iVideoFile = iIrrDevice->getFileSystem()->createAndOpenFile(fileName);
   if (iVideoFile == NULL)
   {
      std::cerr << "Failed to open file: " << fileName << std::endl;
      return false;
   }

   // initialize ogg decoder
   if (!PrepareOgg())
      return false;

   // prepare texture and buffer
   if (!PrepareBuffers())
      return false;

   iState = ETPS_PLAYING;

   return true;
}

bool CTheoraPlayer::stop()
{
   if (iState == ETPS_IDLE)
      return true;

   if (iTheoraPacketsCount)
   {
      ogg_stream_clear(&iOggStreamState);
      theora_clear(&iTheoraDecoderState);
      theora_comment_clear(&iTheoraComment);
      theora_info_clear(&iTheoraInfo);
   }

   ogg_sync_clear(&iOggSyncState);

   if (iVideoFile != 0)
   {
      iVideoFile->drop();
      iVideoFile = 0;
   }

   if (iTexture != 0)
   {
      iTexture->drop();
      iTexture = 0;
   }

   if (iImage != 0)
   {
      iImage->drop();
      iImage = 0;
   }

   iState = ETPS_IDLE;

   return true;
}

bool CTheoraPlayer::PrepareOgg()
{
   // set start pos
   iVideoFile->seek(0);

   /*
   Ok, Ogg parsing. The idea here is we have a bitstream that is made up of Ogg pages. The libogg sync layer will
   find them for us. There may be pages from several logical streams interleaved;
   we find the first theora stream and ignore any others.

   Then we pass the pages for our stream to the libogg stream layer which assembles our original set of packets out of
   them. It's the packets that libtheora actually knows how to handle.
    */

   int stateflag = 0;

   /* start up Ogg stream synchronization layer */
   ogg_sync_init(&iOggSyncState);

   /* init supporting Theora structures needed in header parsing */
   theora_comment_init(&iTheoraComment);
   theora_info_init(&iTheoraInfo);

   iTheoraPacketsCount = 0;
   iCurrFrame = 0;
   iCurrTime = 0;


   //Ogg file open; parse the headers

   //Vorbis and Theora both depend on some initial header packets for decoder setup and initialization.
   //We retrieve these first before entering the main decode loop.

   //Only interested in Theora streams
   while (!stateflag)
   {
      const int ret = buffer_data();
      if (ret == 0)
         break;

      while (ogg_sync_pageout(&iOggSyncState,&iOggPage)>0)
      {
         ogg_stream_state test;

         //is this a mandated initial header? If not, stop parsing
         if (!ogg_page_bos(&iOggPage))
         {
            //don't leak the page; get it into the appropriate stream
            queue_page(&iOggPage);
            stateflag = 1;

            break;
         }

         ogg_stream_init(&test, ogg_page_serialno(&iOggPage));
         ogg_stream_pagein(&test, &iOggPage);
         ogg_stream_packetout(&test, &iOggPacket);

         //identify the codec: try theora
         if (!iTheoraPacketsCount && theora_decode_header(&iTheoraInfo, &iTheoraComment, &iOggPacket) >= 0)
         {
            //it is theora -- save this stream state
            memcpy(&iOggStreamState,&test,sizeof(test));
            iTheoraPacketsCount = 1;
         }
         else
         {
            //whatever it is, we don't care about it
            ogg_stream_clear(&test);
         }
      }
      //fall through to non-initial page parsing
   }

   //we're expecting more header packets
   while (iTheoraPacketsCount && iTheoraPacketsCount<3)
   {
      int ret;

      /* look for further theora headers */
      while (iTheoraPacketsCount && (iTheoraPacketsCount < 3))
      {
         ret = ogg_stream_packetout(&iOggStreamState, &iOggPacket);
         if (ret < 0)
         {
            std::cerr << "CTheoraPlayer: Error parsing Theora stream headers; corrupt stream?" << std::endl;
            return false;
         }

         if (theora_decode_header(&iTheoraInfo, &iTheoraComment, &iOggPacket))
         {
            std::cerr << "CTheoraPlayer: Error parsing Theora stream headers; corrupt stream?" << std::endl;
            return false;
         }

         ++iTheoraPacketsCount;
      }


      //The header pages/packets will arrive before anything else we care about, or the stream is not obeying spec
      if(ogg_sync_pageout(&iOggSyncState, &iOggPage)>0)
      {
         queue_page(&iOggPage); //demux into the stream state
      }
      else
      {
         int ret = buffer_data(); //need more data
         if (ret == 0)
         {
            std::cout << "CTheoraPlayer: End of file while searching for codec headers." << std::endl;
            return false;
         }
      }
   }

   //now we have all the required headers. initialize the decoder
   if (iTheoraPacketsCount)
   {
      theora_decode_init(&iTheoraDecoderState, &iTheoraInfo);
      static irr::c8 buffer[512] = {0};
      snprintf(buffer, 511, "Ogg logical stream %x is Theora %dx%d %.02f fps video\nEncoded frame content is %dx%d with %dx%d offset",
            (unsigned int)iOggStreamState.serialno,iTheoraInfo.width,iTheoraInfo.height,
            (double)iTheoraInfo.fps_numerator/iTheoraInfo.fps_denominator,
            iTheoraInfo.frame_width, iTheoraInfo.frame_height, iTheoraInfo.offset_x, iTheoraInfo.offset_y);

      std::cout << buffer << std::endl;
   }
   else
   {
      std::cerr << " initialize the decoder failed" << std::endl;

      //tear down the partial theora setup
      theora_info_clear(&iTheoraInfo);
      theora_comment_clear(&iTheoraComment);

      return false;
   }

   //queue any remaining pages from data we buffered but that did not contain headers
   while (ogg_sync_pageout(&iOggSyncState, &iOggPage) > 0)
   {
      queue_page(&iOggPage);
   }

   return true;
}

//! Create texture if needed and buffer for it.
//! If texture output is on we create buffer with the same parameters to use memcpy function in synchronization section
bool CTheoraPlayer::PrepareBuffers()
{
   // unset auto mipmaps flag
   bool oldMipmapFlag = iIrrVideoDriver->getTextureCreationFlag(irr::video::ETCF_CREATE_MIP_MAPS);
   iIrrDevice->getVideoDriver()->setTextureCreationFlag(irr::video::ETCF_CREATE_MIP_MAPS, false);

   // create texture
   const irr::core::dimension2du size(iTheoraInfo.frame_width, iTheoraInfo.frame_height);
   iTexture = iIrrVideoDriver->addTexture(size, "CTheoraTexture", irr::video::ECF_R8G8B8);

   // restore auto mipmaps flag
   iIrrVideoDriver->setTextureCreationFlag(irr::video::ETCF_CREATE_MIP_MAPS, oldMipmapFlag);

   if (iTexture == NULL)
   {
      std::cerr << "CTheoraPlayer: Failed to create texture" << std::endl;
      return false;
   }
   iTexture->grab();

   // we should care what buffer and texture have the same size so we can use memcpy operation create buffer image
   const irr::core::dimension2du& imageSize = iTexture->getOriginalSize();// was getSize();
   const irr::u32 bufferSize = iTexture->getPitch()*imageSize.Height;

   irr::u8* textureBuffer = new irr::u8[bufferSize];
   if (textureBuffer == NULL)
   {
      std::cerr << "CTheoraPlayer: Failed to allocate buffer memory" << std::endl;
      return false;
   }

   // create IImage, i couldn't get working parameters for createImageFromData()
   // to use textureBuffer as Data in CImage class and correctly delete it
   iImage = iIrrVideoDriver->createImageFromData(iTexture->getColorFormat(), imageSize, textureBuffer);
   delete[] textureBuffer;
   if (iImage == NULL)
   {
      std::cerr << "CTheoraPlayer: Failed to create image" << std::endl;
      return false;
   }

   return true;
}

bool CTheoraPlayer::update(const irr::u32 timeMs)
{
   if (iState != ETPS_PLAYING)
      return true;

   // calculate how many frames need to be decoded
   iCurrTime += timeMs;
   const irr::u32 neededFrame = (irr::u32)(1.0f*(iCurrTime/1000.0f)*iTheoraInfo.fps_numerator/iTheoraInfo.fps_denominator);
   const irr::u32 framesToDo = neededFrame - iCurrFrame;

   // now decode
   irr::u32 wasLastFrameDecoded = false;
   for (irr::u32 i = 0; i < framesToDo; ++i)
   {
      wasLastFrameDecoded = ProcessNextFrame();
      if (iState != ETPS_PLAYING)
         break;
   };

   if (wasLastFrameDecoded)
   {
      //dumpvideo frame
      UpdateBuffer();
      UpdateTexture();
   }

   return true;
}

bool CTheoraPlayer::ProcessNextFrame()
{
   /*
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
    */

   //single frame video buffering
   bool videobuf_ready = false;
   ogg_int64_t videobuf_granulepos = -1;
   double videobuf_time = 0; //holds the presentation time of the currently buffered video frame. We ignore this value.

   while (!videobuf_ready)
   {
      //theora is one in, one out...
      if (ogg_stream_packetout(&iOggStreamState, &iOggPacket) > 0)
      {
         theora_decode_packetin(&iTheoraDecoderState, &iOggPacket);

         videobuf_granulepos = iTheoraDecoderState.granulepos;
         videobuf_time = theora_granule_time(&iTheoraDecoderState, videobuf_granulepos);
         videobuf_ready = true;

         ++iCurrFrame;
      }

      // TODO: handle end of file
      if (!videobuf_ready && (iVideoFile->getPos() == iVideoFile->getSize()))
      {
         if (iIsLooped)
         {
            PrepareOgg();
         }
         else
         {
            stop();
            return false;
         }
      }

      if (!videobuf_ready)
      {
         //no data yet for somebody.  Grab another page
         buffer_data();

         while(ogg_sync_pageout(&iOggSyncState, &iOggPage) > 0)
         {
            queue_page(&iOggPage);
         }
      }
   }

   return videobuf_ready;
}

void CTheoraPlayer::UpdateTexture()
{
   if (!iImage)
      return;

   // try to lock texture
   irr::u8* const textureData = (irr::u8*) iTexture->lock();
   if (!textureData)
      return;

   const irr::core::dimension2du& textureSize = iTexture->getSize();

   iImage->copyToScaling(textureData, textureSize.Width, textureSize.Height, iTexture->getColorFormat(), iTexture->getPitch());

   iTexture->unlock();
}

void CTheoraPlayer::UpdateBuffer()
{
   if (iImage == NULL)
      return;

   yuv_buffer yuv;
   theora_decode_YUVout(&iTheoraDecoderState, &yuv);


//   const irr::core::dimension2du& imageSize = iImage->getDimension();
//   _IRR_DEBUG_BREAK_IF(imageSize.Height < yuv.y_height || imageSize.Width < yuv.y_width);

   irr::u8* const bufferData = (irr::u8*)iImage->lock();
//   irr::u8* yData = yuv.y;
//   irr::u8* uData = yuv.u;
//   irr::u8* vData = yuv.v;


   const irr::u32 rowSize = iImage->getPitch();

   switch (iImage->getColorFormat())
   {
   case irr::video::ECF_A8R8G8B8:
   {
      for (int y = 0; y < yuv.y_height; ++y)
      {
         const int xsize = yuv.y_width;
         const int uvy = (y/2)*yuv.uv_stride;
         const int yy = y*yuv.y_stride;
         const int by = y*rowSize;

         for (int x = 0; x < xsize; ++x)
         {
            const int Y = yuv.y[yy + x] - 16;
            const int U = yuv.u[uvy + (x/2)] - 128;
            const int V = yuv.v[uvy + (x/2)] - 128;

            int R = ((298*Y         + 409*V + 128)>>8);
            int G = ((298*Y - 100*U - 208*V + 128)>>8);
            int B = ((298*Y + 516*U         + 128)>>8);

            if (R < 0) R = 0; if (R > 255) R = 255;
            if (G < 0) G = 0; if (G > 255) G = 255;
            if (B < 0) B = 0; if (B > 255) B = 255;

            bufferData[by + x*4 + 0] = B;
            bufferData[by + x*4 + 1] = G;
            bufferData[by + x*4 + 2] = R;
            bufferData[by + x*4 + 3] = 0xFF;
         };
      };
   }
   break;

   case irr::video::ECF_R8G8B8:
   {
      for (int y = 0; y < yuv.y_height; ++y)
      {
         const int xsize = yuv.y_width;
         const int uvy = (y/2)*yuv.uv_stride;
         const int yy = y*yuv.y_stride;
         const int by = y*rowSize;

         for (int x = 0; x < xsize; ++x)
         {
            const int Y = yuv.y[yy + x] - 16;
            const int U = yuv.u[uvy + (x/2)] - 128;
            const int V = yuv.v[uvy + (x/2)] - 128;

            int R = ((298*Y         + 409*V + 128)>>8);
            int G = ((298*Y - 100*U - 208*V + 128)>>8);
            int B = ((298*Y + 516*U         + 128)>>8);

            if (R < 0) R = 0; if (R > 255) R = 255;
            if (G < 0) G = 0; if (G > 255) G = 255;
            if (B < 0) B = 0; if (B > 255) B = 255;

            bufferData[by + x*3 + 0] = B;
            bufferData[by + x*3 + 1] = G;
            bufferData[by + x*3 + 2] = R;
         };
      }
   }
   break;

   }
}

//! helper: push a page into the steam for packetization
void CTheoraPlayer::queue_page(ogg_page *page)
{
   if (iTheoraPacketsCount)
      ogg_stream_pagein(&iOggStreamState, &iOggPage);
}

//! Helper; just grab some more compressed bitstream and sync it for page extraction
int CTheoraPlayer::buffer_data()
{
   char* const buffer = ogg_sync_buffer(&iOggSyncState, 4096);
   const int bytes_read = iVideoFile->read(buffer, 4096);

   ogg_sync_wrote(&iOggSyncState, bytes_read);

   return bytes_read;
}
