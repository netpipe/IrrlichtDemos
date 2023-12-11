
#ifndef THEORAPLAYER_H_
#define THEORAPLAYER_H_

#include <theora/theora.h>
#include <irrTypes.h>

namespace irr
{
   class IrrlichtDevice;
   namespace video
   {
      class IVideoDriver;
      class ITexture;
      class IImage;
   }

   namespace io
   {
      class IReadFile;
   }
}

class CTheoraPlayer
{

public:

   CTheoraPlayer(irr::IrrlichtDevice* device);

   ~CTheoraPlayer();


   //! Start playing
   bool load(const char* const fileName, const bool loop = false);

   //! Get next frame and update texture
   bool update(const irr::u32 timeMs);

   bool stop();


   enum E_THEORA_PLAYER_STATE
   {
      ETPS_IDLE = 0,
      ETPS_PLAYING,
   };

   //! Get playing state
   inline const E_THEORA_PLAYER_STATE& getState() const { return iState; }

   //! Get texture interface
   inline irr::video::ITexture* const getTexture() const
   {
      if (iTexture)
         return iTexture;

      return iBlankTexture;
   }

   //! Get image interface
   inline irr::video::IImage* const getImage() const { return iImage; }


      // Irrlicht classes
   irr::IrrlichtDevice* iIrrDevice;
   irr::video::IVideoDriver* iIrrVideoDriver;

   void setDevice(irr::IrrlichtDevice*);
private:

   //! Decode next frame from ogg input stream.
   //\return true if frame was successfully decoded.
   bool  ProcessNextFrame();

   //! Write out the planar YUV frame, uncropped.
   void  UpdateBuffer();

   ///Copies the buffer data to the Irrlicht texture.
   void  UpdateTexture();

   bool  PrepareOgg();
   bool  PrepareBuffers();

   //! Helper Theora functions
   void queue_page(ogg_page* page);
   int buffer_data();



   irr::video::ITexture* iBlankTexture;
   irr::video::ITexture* iTexture;
   irr::video::IImage* iImage;
   irr::io::IReadFile* iVideoFile;



   //Ogg and codec state for demux/decode
   ogg_sync_state iOggSyncState;

   ///Used to encapsulate the data in one Ogg bitstream page.
   ogg_page iOggPage;

   ogg_stream_state iOggStreamState;

   theora_info iTheoraInfo;
   theora_comment iTheoraComment;
   theora_state iTheoraDecoderState;
   int iTheoraPacketsCount;
   ogg_packet iOggPacket;



   bool iIsLooped;
   irr::u32 iCurrFrame;
   irr::u32 iCurrTime;


   E_THEORA_PLAYER_STATE iState;
};

#endif /* THEORAPLAYER_H_ */
