#ifndef JUKEBOX_H
#define JUKEBOX_H

#include <string>
#include <sstream>
#include <vector>
#include <dirent.h>
#include <ctime>

#include <GL/glew.h>
#ifndef __APPLE__
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif

#include "../GUI/Button.h"
#include "../GUI/Scrollbar.h"
#include "../GUI/MouseScrollArea.h"
#include "../Input/Input.h"
#include "../Fonts/Font.h"
#include "../Texture/TextureFactory.h"
#include "../Sound/SoundFactory.h"
#include "../GUI/Window.h"
#include "../XML/tinyxml.h"
#include "../Config/properties.h"
#include "../Config/Helpers/types.h"

class SoundFactory;

class JukeBox : public GUI::Window
{
    public:
        void init(const std::string &workingDir);
        void shutdown();
        void setPlayType(unsigned short);
        void update();
        void tick(Input& input, const float &camPosX, const float &camPosY, const float &camPosZ);
        void draw(const int &sw, const int &sh);
        inline void toggleVisibility() { visible = !visible; }
    private:
        void loadTemplate(const std::string &workingDir);
        void loadConfig();
        void saveConfig();

        SoundFactory *sound_factory;
        Font *text_engine;
        TextureFactory *texture_factory;

        int PlayListX, PlayListY;
        int PlayListW, PlayListH;
        int PlayInfoX, PlayInfoY;
        int PlayInfoW, PlayInfoH;
        std::string PlayInfoFont;
        std::string PlayListFont;
        unsigned int PlayInfoFontPoint;
        unsigned int PlayListFontPoint;

        GUI::Scrollbar volumeSlider;
        int VolumeX, VolumeY;
        int VolumeW, VolumeH;
        std::string VolumeBarTex, VolumeSlideTex;
        int VolumeDirection;
        float volume;

        GUI::Scrollbar songScroller;
        int SongScrollX, SongScrollY;
        int SongScrollW, SongScrollH;
        std::string SongScrollBarTex, SongScrollSlideTex;
        int SongScrollDirection;

        unsigned int MaximumVisibleSongs;
        unsigned int visibleOffset;

        std::vector<std::string> files;
        std::string workingDir;
        std::string songDirectory;
        DIR *dir;

        std::vector<GUI::Button> selectBox;
        GUI::MouseScrollArea scrollBox;

        std::string playtime;
        std::string totaltime;

        time_t startTime;
        unsigned long int elapsedTime;

        std::string currentTrack;
        unsigned int currentTrackNum;

        unsigned short playtype;
        bool firstStart;
        GLint songlist;
        long jukeSource;

        bool recompileSongList;
};

#endif
