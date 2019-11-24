#ifndef AZADI_H
#define AZADI_H

#include <pthread.h>
#include <unistd.h>

#include "Config/config.h"
#include "Scripting/Headers/Azadi.h"
#include "Input/Input.h"
#include "Particle/ParticleFactory.h"
#include "Shaders/Shaders.h"
#include "Texture/TextureFactory.h"
#include "Sound/SoundFactory.h"
#include "Scripting/ScriptEngine.h"
#include "Scripting/ScriptEngineInputs.h"
#include "Net/Client.h"
#include "GUI/Menu.h"
#include "matrix.h"
#include "quaternion.h"
#include "tilegrid.h"
#include "Fonts/Font.h"
#include "Sound/JukeBox.h"
#include "CustomFunctions.h"
#include "Terrain/Terrain.h"
#include "VGUI/MainMenu.h"
#include "VGUI/FPSCounter.h"
#include "VGUI/DevMenu.h"
#include "Scenegraph/Nodes.h"
#include "Physics/Physics.h"

#ifndef NDEBUG
#define BUILD "Debug"
#else
#define BUILD "Release"
#endif

#define AZADIVERSION "0.1 pre-alpha"

//#define PI 3.1415926535897932384626433832795f
#define PI 3.14159
const float piover180 = 0.0174532925f;

class ModelFactory ;
class Drawing;
class Textures;
class SDL_Surface;
class JukeBox;
class SoundFactory;
class Terrain;

enum mov
{
    FORWARD=0,
    BACKWARD,
    LEFT,
    RIGHT
};

//! The main Azadi class that handles the main game loop
class Azadi : public azCore
{
    public:
        Azadi(int argc, char** argv);
		~Azadi(); // Cleans up the engine
        int Run();

        // the player's name
        std::string playerName;

        inline void setView2D(const bool &flag)
        {
            view2d = flag;
        }

        inline void setConstrainedAxis(const unsigned short &axis)
        {
            constrainedAxis[axis] = true;
        }

        inline void setCameraPosition(const float &x, const float &y, const float &z)
        {
            CamPos[0] = x;
            CamPos[1] = y;
            CamPos[2] = z;
        }

        void remapMovementAxis(const unsigned short &a1, const unsigned short &a2, const unsigned short &a3)
        {
            movementAxis[0] = a1;
            movementAxis[1] = a2;
            movementAxis[2] = a3;
        }

        void setMovementSpeed(const float &moveSpeed)
        {
            movespeed = moveSpeed;
        }

        void setFov(const double &fov);

        inline void delay(const unsigned int &delayAmount)
        {
            #ifdef WIN32
            timeBeginPeriod(1);
            Sleep(delayAmount);
            timeEndPeriod(1);
            #else
            usleep(1000 * delayAmount);
            #endif
        }

        void captureCursor()
        {
            SDL_ShowCursor(SDL_DISABLE);
        }

        void releaseCursor()
        {
            SDL_ShowCursor(SDL_ENABLE);
        }

        void toggleAntiAlias(const unsigned short &samples);

        void outputConsole(const std::string &text);

        inline unsigned long int getTicks()
        {
            return SDL_GetTicks();
        }

        // Configuration related functions
        inline void setWorkingDirectory(const std::string &workingDir) { workingDirectory = workingDir; }
        inline void setPostProcessing(const bool &flag) { postProcessAvailable = flag; }
        inline void setAntiAliasing(const bool &flag) { antiAliasEnabled = flag; }
        inline void setDelay(const bool &flag) { delayEnabled = flag; }
        inline void setScreenWidth(const unsigned int &w) { screen[0] = w; }
        inline void setScreenHeight(const unsigned int &h) { screen[1] = h; }
        inline void setFullScreen(const bool &fs) { fullScreen = fs; }
        inline void setTerrain(const bool &ter) { terrainEnabled = ter; }
        inline void setShaders(const bool &flag) { useShaders = flag; }
        inline void setExtensions(const bool &flag) { useExtensions = flag; }

        inline std::string getWorkingDirectory() { return workingDirectory; }
        inline bool isPostProcessing() { return postProcessAvailable; }
        inline bool isAntiAlias() { return antiAliasEnabled; }
        inline bool isDelay() { return delayEnabled; }
        inline unsigned int getScreenWidth() { return screen[0]; }
        inline unsigned int getScreenHeight() { return screen[1]; }
        inline bool isFullScreen() { return fullScreen; }
        inline bool isTerrainEnabled() { return terrainEnabled; }
        inline bool usingShaders() { return useShaders; }
        inline bool usingExtensions() { return useExtensions; }

        inline void shutdown() { shuttingDown = true; }
        inline SceneGraphNodes::BaseNode* getActiveNode() { return activeNode; }

		Client client;

		SceneGraphNodes::BaseNode *activeNode;

		static Azadi* getInstance() { return instance; }
    private:
        static Azadi* instance;

        void parseParameters(int argc, char** argv);
        void registerKeys();
        bool Init();
        bool changeVideoMode(const unsigned int &width, const unsigned int &height, const unsigned short &bpp, const bool &fullscreen);
        void checkExtensions();

        int Lobby();
        int Mainloop();

        void ProcessMessages();
        bool Draw();

        void storeMatrices();

        // Factory threads
        pthread_t soundThread;

        bool constrainedAxis[3];
        bool view2d;
        float ix;
        float iy;
        float movespeed;
        int movementAxis[3];
        GLfloat CamPos[3];
        float fps;

        bool shuttingDown;

        //! Screen size
        int screen[2];
        //! Mouse position
        int mouse[2];
        //! Koordinates of the center
        int center[2];

        bool showJukeBox;

        bool MoveKey[4];

        float aspect;
        double fov;

        bool soundDeviceWorking;
        bool fullScreen;
        bool withAudio;
        bool useShaders;
        bool useExtensions;

        bool antiAliasEnabled;
        unsigned short antiAliasSamples;
        // For post processing
        bool postProcessAvailable;
        bool postProcessInitialized;
        unsigned short postProcessStyle;
        GLuint renderBuffer;
        GLuint renderBufferTexture;
        GLuint depthBuffer;
        GLuint stencilBuffer;
        GLuint *renderBufferData;
        bool createdList;

        // For the terrain
        bool terrainEnabled;

        // FPS Counter
        FPSCounter *fpsCounter;

        // Should we have a delay or not?
        bool delayEnabled;

        float msensitivity;   // Mouse sensitivity

        unsigned int sessionId;
        unsigned long lastTick;

        std::string workingDirectory;

        // Factories
        SoundFactory *sound_factory;
        JukeBox *jukebox;
        Shaders *shaders;
        TextureFactory *texture_factory;
        ModelFactory *model_factory;
        Font *text_engine;
        ScriptEngine *script_engine;
        ParticleFactory *particle_factory;
        Terrain *TP;
        Drawing* drawing;
        Physics *physics_engine;
        ScriptEngineInputs *script_io;

        // Developer menu
        DevMenu *devMenu;

        //! Window SDL-Handler
        SDL_Surface* window;

        // OpenGL display lists the Azadi uses
        GLint postProcessList;

        unsigned long int tempModel;

        SceneGraphNodes::RootNode *rootNode;
        SceneGraphNodes::TranslateNode *sceneTranslateNode;
        SceneGraphNodes::RotateNode *sceneRotateNode;
        SceneGraphNodes::OrthoViewNode *sceneOrthoNode;
        SceneGraphNodes::FrustumViewNode *sceneFrustumNode;
};

/*! \mainpage Azadi RTS engine

	Place some descriptive text here you guys, at the bottom of Azadi.h <br><br>

	Even after how many months we have been working, we're still so far from our goals. But we're slowly getting there day by day

*/

#endif
