#ifndef __VIDEO_PLAYER_H__
#define __VIDEO_PLAYER_H__

// irrlicht includes
#include <irrlicht.h>

// ffmpeg includes
extern "C" {
#include <avcodec.h>
#include <avformat.h>
#include <swscale.h>
}

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;



class cVideoPlayer
{
private:

    irr::ITimer *Timer;
    irr::video::IVideoDriver *IrrVideoDriver;
    irr::video::IImage *CurrentImage;
    irr::video::ITexture *CurrentTexture;

    unsigned long lastTime;

    double SecondsPerFrame;

    AVFormatContext *FormatCtx;
    int             VideoStream;
    AVCodecContext  *CodecCtx;
    AVCodec         *Codec;
    AVFrame         *Frame;
    AVFrame         *FrameRGB;
    int             NumBytes;
    uint8_t         *Buffer;
    int          desiredH;
    int          desiredW;



    bool GetNextFrame(AVFormatContext *pFormatCtx,
                      AVCodecContext *pCodecCtx,
                      int videoStream,
                      AVFrame *pFrame);

    bool DumpFrame(AVFrame *pFrame, int width, int height, bool needResize);

    s32* p;
    s32* pimage;

    enum cState {playing, paused, stopped, manual};
    cState state;
    bool streamOpen;
    char* actualFilename;
    int actualFrame;
    bool loop;
    int replayCont;
    float framerate;
    int seekFactor;

public:
    // constructors
    cVideoPlayer(irr::video::IVideoDriver *irrVideoDriver,
                 irr::ITimer *timer);
    cVideoPlayer(irr::video::IVideoDriver *irrVideoDriver,
                 irr::ITimer *timer, char* filename, int desiredW, int desiredH);

    // destructor
    ~cVideoPlayer();

    void changeResolution(int w, int h);

    // functions to open, play, and stop the media
    bool open(char *filename);

    // refresh the video
    bool refresh();

    // play the video
    bool play();

    // stop the video
    bool stop();

    // pause the video
    bool pause();

    // get the video texture
    irr::video::ITexture* getVideoTexture();

    bool isPlaying();

    // draw the video texture
    void drawVideoTexture();

    // close the video stream
    bool restartStream();

    // set looped
    void setLoop(bool l);

    // go to selected second
    void goToSecond(int numSecond);

    // get selected frame
    bool goToFrame(int numFrame);

    // get the movie framerate
    float getFrameRate();

    // get the seek factor (15 by default)
    int getSeekFactor();

    // set the seek factor
    void setSeekFactor(int sf);
};

#endif
