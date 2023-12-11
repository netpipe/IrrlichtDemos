
#include "videoPlayer.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;




//-----------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------
// Original code by: juliusctw
// Modified by Hansel
// Additional changes by stepan1117

// compile with:
// windows:
// g++ -o player cVideoPlayer.cpp -lavformat -lavcodec -lavutil -lz -I/c/code/iamar/irrlicht-1.2/include/ -L/c/code/iamar/irrlicht-1.2/bin/Win32-gcc/ -lIrrlicht
//
// linux:
// g++ -o player cVideoPlayer.cpp -lavformat -lavcodec -lavutil -lz -I../irrlicht/include/ -L../irrlicht -lIrrlicht -lGL -lGLU -lXxf86vm -lXext -lX11


//-----------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------

cVideoPlayer::cVideoPlayer(irr::video::IVideoDriver *irrVideoDriver, irr::ITimer *timer)
        : IrrVideoDriver(irrVideoDriver), Timer(timer)
{
    IrrVideoDriver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, false);
    IrrVideoDriver->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, true);
    // Register all formats and codecs
    av_register_all();

    state = stopped;
    streamOpen = false;
    actualFrame = 0;
    loop = false;
    replayCont = 0;
    seekFactor = 15;




}


//-----------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------

cVideoPlayer::cVideoPlayer(irr::video::IVideoDriver *irrVideoDriver, irr::ITimer *timer, char* filename, int desiredW, int desiredH)
        : IrrVideoDriver(irrVideoDriver), Timer(timer)
{
    IrrVideoDriver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, false);
    IrrVideoDriver->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, true);
    // Register all formats and codecs
    av_register_all();

    this->desiredH = desiredH;
    this->desiredW = desiredW;

    if (! open(filename))
    {
        printf("problem opening movie");
        exit(1);
    }


    state = stopped;
    streamOpen = false;
    actualFrame = 0;
    loop = false;
    replayCont = 0;
    seekFactor = 15;


}


//-----------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------


cVideoPlayer::~cVideoPlayer()
{
    // Free the RGB image
    if (Buffer != NULL)
        delete [] Buffer;

    if (FrameRGB != NULL)
        av_free(FrameRGB);

    // Free the YUV frame
    if (Frame != NULL)
        av_free(Frame);

    // Close the codec
    if (CodecCtx != NULL)
        avcodec_close(CodecCtx);

    // Close the video file
    if (FormatCtx != NULL)
        av_close_input_file(FormatCtx);
}


//-----------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------

void cVideoPlayer::changeResolution(int w, int h){

   if (desiredW != w || desiredH != h){

      std::cout << "Changing resolution from ["<< desiredW << "x" << desiredH << "] to [" << w << "x" << h << "]" << std::endl;

      desiredW = w;
      desiredH = h;

      delete [] Buffer;
      //av_free((AVPicture *)FrameRGB);

      NumBytes=avpicture_get_size(PIX_FMT_RGB32,
                              desiredW,
                              desiredH);

      Buffer=new uint8_t[NumBytes];

         // Assign appropriate parts of buffer to image planes in pFrameRGB
      avpicture_fill((AVPicture *)FrameRGB, Buffer, PIX_FMT_RGB32,
               desiredW, desiredH);
   }
}

bool cVideoPlayer::open(char *filename)
{
    actualFilename = filename;

    // Open video file
    if (av_open_input_file(&FormatCtx, filename, NULL, 0, NULL)!=0)
        return false; // Couldn't open file

    // Retrieve stream information
    if (av_find_stream_info(FormatCtx)<0)
        return false; // Couldn't find stream information

    // Dump information about file onto standard error
    dump_format(FormatCtx, 0, filename, false);

    // Find the first video stream
    VideoStream=-1;
    for (int i=0; i<FormatCtx->nb_streams; i++)
        if (FormatCtx->streams[i]->codec->codec_type==CODEC_TYPE_VIDEO)
        {
            VideoStream=i;
            break;
        }
    if (VideoStream==-1)
        return false; // Didn't find a video stream



    // Get a pointer to the codec context for the video stream
    CodecCtx=FormatCtx->streams[VideoStream]->codec;

    // Get the seconds/frame of this video stream
    SecondsPerFrame = (double)FormatCtx->streams[VideoStream]->r_frame_rate.den / FormatCtx->streams[VideoStream]->r_frame_rate.num;
    printf("Duration:  %f", (double)FormatCtx->duration);

    printf("  seconds per frame: %f\n", SecondsPerFrame);

    // Find the decoder for the video stream
    Codec=avcodec_find_decoder(CodecCtx->codec_id);
    if (Codec==NULL)
        return false; // Codec not found

    // Inform the codec that we can handle truncated bitstreams -- i.e.,
    // bitstreams where frame boundaries can fall in the middle of packets
    if (Codec->capabilities & CODEC_CAP_TRUNCATED)
        CodecCtx->flags|=CODEC_FLAG_TRUNCATED;

    // Open codec
    if (avcodec_open(CodecCtx, Codec)<0)
        return false; // Could not open codec

    // Allocate video frame
    Frame=avcodec_alloc_frame();

    // Allocate an AVFrame structure
    FrameRGB=avcodec_alloc_frame();
    if (FrameRGB==NULL)
        return false;

    // Determine required buffer size and allocate buffer

//    desiredH = CodecCtx->height+100;
//    desiredW = CodecCtx->width+100;

    NumBytes=avpicture_get_size(PIX_FMT_RGB32,
                                desiredW,
                                desiredH);
    Buffer=new uint8_t[NumBytes];

    // Assign appropriate parts of buffer to image planes in pFrameRGB
    avpicture_fill((AVPicture *)FrameRGB, Buffer, PIX_FMT_RGB32,
          desiredW, desiredH);


    // get the movie framerate
    framerate = FormatCtx->streams[VideoStream]->r_frame_rate.num;



    return true;
}



//-----------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------


bool cVideoPlayer::refresh()
{
   static struct SwsContext *img_convert_ctx;
   static int currentX = 0;
   static int currentY = 0;
   bool needResize = false;

   if (state == playing)
    {

       if (Timer->getRealTime() - lastTime > (SecondsPerFrame*1000))
        {

           if (GetNextFrame(FormatCtx, CodecCtx, VideoStream, Frame))
            {




              if(img_convert_ctx == NULL) {
                 currentX = desiredW;
                 currentY = desiredH;

                 int w = CodecCtx->width;
                    int h = CodecCtx->height;

                         img_convert_ctx = sws_getContext(w, h,
                                 CodecCtx->pix_fmt,
                                           desiredW, desiredH, PIX_FMT_RGB32, SWS_FAST_BILINEAR | SWS_CPU_CAPS_MMX2,
                                           NULL, NULL, NULL);
                         if(img_convert_ctx == NULL) {
                            fprintf(stderr, "Cannot initialize the conversion context!\n");
                            exit(1);
                         }
                } else if (currentX != desiredW || currentY != desiredH){
                   needResize = true;
                   currentX = desiredW;
                   currentY = desiredH;

                   int w = CodecCtx->width;
                int h = CodecCtx->height;

                sws_freeContext(img_convert_ctx);
                img_convert_ctx = NULL;

                img_convert_ctx = sws_getContext(w, h,
                              CodecCtx->pix_fmt,
                              desiredW, desiredH, PIX_FMT_RGB32, SWS_FAST_BILINEAR | SWS_CPU_CAPS_MMX2,
                              NULL, NULL, NULL);
                if(img_convert_ctx == NULL) {
                    fprintf(stderr, "Cannot re-initialize the conversion context!\n");
                    exit(1);
                }
                }



              sws_scale(img_convert_ctx, Frame->data,
                                 Frame->linesize, 0,
                                 CodecCtx->height,
                                 FrameRGB->data, FrameRGB->linesize);

                //printf("Replay Num.: %d  ::  Dumping Frame: %d  ::  FrameRate: %f\n", replayCont, actualFrame, framerate);

                // Dump the frame
                DumpFrame(FrameRGB, desiredW, desiredH, needResize);

                actualFrame++;
            }
            else
            {
                state = stopped;
                printf("->End\n");
            }

            lastTime = Timer->getRealTime();
        }
    }

    if (state == stopped)
    {
        actualFrame = 0;

        restartStream();

        // if looped then replay the movie
        if (loop)
        {
            replayCont ++;
            state = playing;
            streamOpen = true;
        }
    }
}


//-----------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------

bool cVideoPlayer::isPlaying(){
   return (state == playing);
}

bool cVideoPlayer::play()
{
    if (state == stopped) replayCont++;
    state = playing;
    streamOpen = true;

    return true;
}


//-----------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------


bool cVideoPlayer::stop()
{
    state = stopped;

    return true;
};


//-----------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------

bool cVideoPlayer::pause()
{
    state = paused;

    return true;
};


//-----------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------


bool cVideoPlayer::GetNextFrame(AVFormatContext *pFormatCtx,
                                AVCodecContext *pCodecCtx,
                                int videoStream,
                                AVFrame *pFrame)
{
    static AVPacket packet;
    static int      bytesRemaining=0;
    static uint8_t  *rawData;
    static bool     fFirstTime=true;
    int             bytesDecoded;
    int             frameFinished;



    // First time we're called, set packet.data to NULL to indicate it
    // doesn't have to be freed
    if (fFirstTime)
    {
        fFirstTime=false;
        packet.data=NULL;
    }

    // Decode packets until we have decoded a complete frame
    while (true)
    {
        // Work on the current packet until we have decoded all of it
        while (bytesRemaining > 0)
        {
            // Decode the next chunk of data
            bytesDecoded=avcodec_decode_video(pCodecCtx, pFrame,
                                              &frameFinished, rawData, bytesRemaining);

            // Was there an error?
            if (bytesDecoded < 0)
            {
                fprintf(stderr, "Error while decoding frame\n");
                return false;
            }

            bytesRemaining-=bytesDecoded;
            rawData+=bytesDecoded;

            // Did we finish the current frame? Then we can return
            if (frameFinished)
                return true;
        }

        // Read the next packet, skipping all packets that aren't for this
        // stream
        do
        {
            // Free old packet
            if (packet.data!=NULL)
                av_free_packet(&packet);

            // Read new packet
            if (av_read_packet(pFormatCtx, &packet)<0)
                goto loop_exit;
        }
        while (packet.stream_index!=videoStream);

        bytesRemaining=packet.size;
        rawData=packet.data;
    }

loop_exit:

    // Decode the rest of the last frame
    bytesDecoded=avcodec_decode_video(pCodecCtx, pFrame, &frameFinished,
                                      rawData, bytesRemaining);

    // Free last packet
    if (packet.data!=NULL)
        av_free_packet(&packet);

    return frameFinished!=0;
}



//-----------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------



bool cVideoPlayer::DumpFrame(AVFrame *pFrame,
                             int width,
                             int height, bool needResize)
{
    static char first_time = 1;

    if (first_time)
    {
       CurrentImage = IrrVideoDriver->createImageFromData(irr::video::ECF_A8R8G8B8,
                       irr::core::dimension2d<irr::s32>(width, height),
                       pFrame->data[0],
                       true);
        first_time = 0;
        CurrentTexture = IrrVideoDriver->addTexture("movie", CurrentImage);

    }

    if (needResize){
       IrrVideoDriver->removeTexture(CurrentTexture);

       CurrentImage = IrrVideoDriver->createImageFromData(irr::video::ECF_A8R8G8B8,
                              irr::core::dimension2d<irr::s32>(width, height),
                              pFrame->data[0],
                              true);
        CurrentTexture = IrrVideoDriver->addTexture("movie", CurrentImage);
    }

    p = (s32*)CurrentTexture->lock ();
    pimage = (s32*)CurrentImage->lock ();


    for (int i = 0; i < width*height; i++) p[i] = pimage[i];

    // unlock de texture and the image
    CurrentTexture->unlock();
    CurrentImage->unlock();



    return true;

}


//-----------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------

irr::video::ITexture* cVideoPlayer::getVideoTexture()
{
    return CurrentTexture;
}



//-----------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------



void cVideoPlayer::drawVideoTexture()
{
    // draw the texture
    if (state == playing)
       IrrVideoDriver->draw2DImage(CurrentTexture, irr::core::position2d<irr::s32>(0,0),
                                irr::core::rect<irr::s32>(0,0,desiredW,desiredH), 0, irr::video::SColor(255,255,255,255), false);
}

//-----------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------


bool cVideoPlayer::restartStream()
{
    //Close stream
   // printf("GetNextFrame()\n");
   if ( FormatCtx && streamOpen)
    {
        streamOpen = false;

        if (av_open_input_file(&FormatCtx, actualFilename, NULL, 0, NULL)!=0) return false; // Couldn't open file
    }
}


//-----------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------


void cVideoPlayer::setLoop(bool l)
{
    loop = l;
}


//-----------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------


void cVideoPlayer::goToSecond(int numSecond)
{
    restartStream();

    state = manual;


    goToFrame((int)framerate*numSecond);
}


//-----------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------




bool cVideoPlayer::goToFrame(int numFrame)
{
    // seek the movie
    if (numFrame < seekFactor) av_seek_frame(FormatCtx, VideoStream, numFrame, AVSEEK_FLAG_ANY);
    else
    {
        av_seek_frame(FormatCtx, VideoStream, numFrame-seekFactor, AVSEEK_FLAG_ANY);

        // advance to the real selected frame
        for (int i = 0; i < seekFactor; i++) GetNextFrame(FormatCtx, CodecCtx, VideoStream, Frame);
    }

    actualFrame = numFrame;

    printf("Replay Num.: %d  ::  Dumping Frame: %d  ::  FrameRate: %f\n", replayCont, actualFrame, framerate);

    return true;
}



//-----------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------


float cVideoPlayer::getFrameRate()
{
    return framerate;
}


//-----------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------

