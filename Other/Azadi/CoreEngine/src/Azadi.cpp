#include "Azadi.h"
#include "ModelFactory/ModelFactory.h"
#include "GUI/Drawing.h"
#include "frustum.h"
#include "matrix.h"

#include <SDL/SDL.h>
#include <unistd.h>

#ifndef __APPLE__
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include <GL/glew.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#endif

Input input;
GUI::Console console;
Geometry::Frustum Geometry::vFrustum;

float xx=0;
float zz=0;

// These matrices are needed to unproject the mouse
GLint viewport_matrix[4] =
{ 0, 0, 0, 0 };

GLdouble projection_matrix[16]=
{1, 0, 0, 0,
 0, 1, 0, 0,
 0, 0, 1, 0,
 0, 0, 0, 1};

 GLdouble modelview_matrix[16]=
{1, 0, 0, 0,
 0, 1, 0, 0,
 0, 0, 1, 0,
 0, 0, 0, 1};

Azadi* Azadi::instance = NULL;

Azadi::Azadi(int argc, char** argv){
	screen[0]=1024, screen[1]=768;
	fullScreen = false;
	withAudio = true;
	useExtensions = true;
	postProcessInitialized = false;
	antiAliasSamples = 1;
	postProcessStyle = 0;
	shuttingDown = false;
    workingDirectory = "data";

	parseParameters(argc, argv);

	window = NULL;

	drawing = NULL;
	model_factory = NULL;
	script_engine = NULL;
	texture_factory = NULL;
	text_engine = NULL;
	particle_factory = NULL;

	constrainedAxis[0] = false;
	constrainedAxis[1] = false;
	constrainedAxis[2] = false;

	movementAxis[0] = 0;
	movementAxis[1] = 1;
	movementAxis[2] = 2;

	CamPos[0] = 0;
	CamPos[1] = 0;
	CamPos[2] = 0;

	movespeed = 20;

	ix = iy = 0;

	showJukeBox = false;

	instance = this;

	terrainEnabled = true;
}

Azadi::~Azadi(){
    rootNode->prepRemove();
    delete rootNode;

    if (drawing != NULL)
        delete drawing;
    if (model_factory != NULL)
        delete model_factory;
    if (script_engine != NULL)
        delete script_engine;
    if (texture_factory != NULL)
        delete texture_factory;
    if (text_engine != NULL)
        delete text_engine;
    if (particle_factory != NULL)
        delete particle_factory;

    SDL_Quit();
}

void Azadi::parseParameters(int argc, char** argv){
    for (int i = 0; i< argc; i++){
        if (strcmp(argv[i], "-fullscreen") == 0)
            fullScreen = true;
        else if (strcmp(argv[i], "-window") == 0)
            fullScreen = false;
        else if (strcmp(argv[i], "-nosound") == 0)
            withAudio = false;
        else if (strcmp(argv[i], "-withsound") == 0)
            withAudio = true;
        else if (strcmp(argv[i], "+game") == 0 && i+1 < argc)
        {
            workingDirectory = argv[i+1];
            i++;
        }
	}
}

bool Azadi::changeVideoMode(const unsigned int &width, const unsigned int &height, const unsigned short &bpp, const bool &fs)
{
    if (window)
    {
        console << GUI::Console::medium << GUI::Console::normal << "Shutting down Screen" << GUI::Console::endl();
        SDL_FreeSurface(window);
    }

	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, antiAliasSamples);

    console << GUI::Console::medium << GUI::Console::normal << "Initialising Screen" << GUI::Console::endl();

    // Set the need to redraw the display lists as true
    createdList = false;

    if (fs)
    {
        window = SDL_SetVideoMode(width, height, bpp, SDL_OPENGL | SDL_FULLSCREEN);
        if (window)
        {
            glViewport(0, 0, (GLsizei) width, (GLsizei) height);
            viewport_matrix[0] = viewport_matrix[1] = 0;
            viewport_matrix[2] = width; viewport_matrix[3] = height;
            aspect = float(width) / height;
            sceneFrustumNode->updateFrustum(-fov * aspect, fov * aspect, -fov, fov, 1.0f, 1500.0f);
            sceneOrthoNode->updateOrtho(0.0f, width, height, 0.0f, -1.0f, 1500.0f);
            if (drawing != NULL)
            {
                drawing->setScreenResolution(width, height);
                drawing->setAspect(aspect);
                if (postProcessAvailable && postProcessInitialized)
                {
                    glBindTexture(GL_TEXTURE_2D, renderBufferTexture);
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
                }
            }

            console.setDimensions(0, 0, width, height/3);
            return true;
        }
        return false;
    }
    else
    {
        window = SDL_SetVideoMode(width, height, bpp, SDL_OPENGL);
        if (window)
        {
            glViewport(0, 0, (GLsizei) width, (GLsizei) height);
            viewport_matrix[0] = viewport_matrix[1] = 0;
            viewport_matrix[2] = width; viewport_matrix[3] = height;
            aspect = float(width) / height;
            sceneFrustumNode->updateFrustum(-fov * aspect, fov * aspect, -fov, fov, 1.0f, 1500.0f);
            sceneOrthoNode->updateOrtho(0.0f, width, height, 0.0f, -1.0f, 1500.0f);
            if (drawing != NULL)
            {
                drawing->setScreenResolution(width, height);
                drawing->setAspect(aspect);
                if (postProcessAvailable && postProcessInitialized)
                {
                    glBindTexture(GL_TEXTURE_2D, renderBufferTexture);
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
                }
            }

            console.setDimensions(0, 0, width, height/3);
            return true;
        }
        return false;
    }
}

void Azadi::setFov(const double &fv)
{
    fov = tan((fv/2)*piover180);
    sceneFrustumNode->updateFrustum(-fov * aspect, fov * aspect, -fov, fov, 1.0f, 1500.0f);
    drawing->setFov(fov);
}

void Azadi::outputConsole(const std::string &text)
{
    console << GUI::Console::normal << GUI::Console::medium << "Scripting engine: " << text << GUI::Console::endl();
}

void Azadi::checkExtensions()
{
    if (!postProcessAvailable)
    {
        console << GUI::Console::log << GUI::Console::medium << "Post processing has been disabled" << GUI::Console::endl();
        return;
    }

    if (GLEW_EXT_framebuffer_object)
    {
        glGenFramebuffersEXT(1, &renderBuffer);
        glGenRenderbuffersEXT(1, &depthBuffer);
        glGenRenderbuffersEXT(1, &stencilBuffer);
        glGenTextures(1, &renderBufferTexture);
        glBindTexture(GL_TEXTURE_2D, renderBufferTexture);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screen[0], screen[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, renderBuffer);
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, renderBufferTexture, 0);

        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthBuffer);
        glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, screen[0], screen[1]);
        glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthBuffer);

        if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) != GL_FRAMEBUFFER_COMPLETE_EXT)
        {
            console << GUI::Console::log << GUI::Console::medium << "An error occured while creating a depthbuffer object. Falling back to no post processing" << GUI::Console::endl();
            postProcessAvailable = false;
        }

        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    }
    else
        postProcessAvailable = false;
}

void Azadi::toggleAntiAlias(const unsigned short &samples)
{
    antiAliasSamples = samples;
    if (antiAliasSamples < 1)
        antiAliasSamples = 1;
    if (!changeVideoMode(screen[0], screen[1], 32, fullScreen))
    {
        toggleAntiAlias(1);
    }
    if (antiAliasSamples == 1)
        antiAliasEnabled = false;
    else
        antiAliasEnabled = true;
}

bool Azadi::Init(){

	console << GUI::Console::log << GUI::Console::medium << "Initialising Azadi Engine" << " " << AZADIVERSION << " " << BUILD << GUI::Console::endl();

    Config *config = new Config();
    if (config->load() < 0)
        console << "Could not load the configuration file" << GUI::Console::endl();
    delete config;

    printf("Using mod directory: %s\n", workingDirectory.c_str());

	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
		printf( "Unable to init SDL: %s\n", SDL_GetError() );
		return false;
	}

    // Create the root nodes of the scene graph
    rootNode = new SceneGraphNodes::RootNode();
    sceneOrthoNode = new SceneGraphNodes::OrthoViewNode();
    sceneFrustumNode = new SceneGraphNodes::FrustumViewNode();
    sceneTranslateNode = new SceneGraphNodes::TranslateNode();
    sceneRotateNode = new SceneGraphNodes::RotateNode();
    rootNode->addChild(sceneFrustumNode);
    rootNode->addChild(sceneOrthoNode);
    sceneFrustumNode->addChild(sceneRotateNode);
    sceneRotateNode->addChild(sceneTranslateNode);


	SDL_WM_SetCaption("Azadi", "azadi.bmp");
	SDL_EnableUNICODE(1);
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
	//SDL_SetModState(KMOD_NUM,false);

    if (!changeVideoMode(screen[0], screen[1], 32, fullScreen))
    {
        printf("Unable to set %ix%i video: %s\n", screen[0], screen[1], SDL_GetError());
        return false;
    }

    SDL_ShowCursor(SDL_DISABLE);

    console << GUI::Console::log << GUI::Console::low << "=================================================" << GUI::Console::endl();
    console << GUI::Console::log << GUI::Console::low << std::string((const char*)glGetString(GL_VENDOR)) << GUI::Console::endl();
    console << GUI::Console::log << GUI::Console::low << std::string((const char*)glGetString(GL_RENDERER)) << GUI::Console::endl();
    console << GUI::Console::log << GUI::Console::low << "OpenGL version " << std::string((const char*)glGetString(GL_VERSION)) << GUI::Console::endl();
    console << GUI::Console::log << GUI::Console::low << "=================================================" << GUI::Console::endl();

    if (useExtensions == false || glewInit() != GLEW_OK)
    {
        console << GUI::Console::log << GUI::Console::low << "Could not initialize the GL extension handler" << GUI::Console::endl();
        useExtensions = false;
    }

    // Clears the garbage from the screen straight away
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SDL_GL_SwapBuffers();

    // Nice Colorflow
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    // Enable texturing
    glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

    //------------------------------------------------------------ Culling
    shaders = new Shaders(workingDirectory);
    if (shaders->usingGLSL())
        console << GUI::Console::log << GUI::Console::medium << "Using GLSL shaders" << GUI::Console::endl();
    else
        postProcessAvailable = false;

    sound_factory = new SoundFactory(workingDirectory);
    texture_factory = new TextureFactory(workingDirectory);
    model_factory = new ModelFactory(workingDirectory);
    text_engine = new Font(workingDirectory);
    drawing = new Drawing();
    physics_engine = new Physics();

    particle_factory = new ParticleFactory();
    script_io = new ScriptEngineInputs();
    script_engine = new ScriptEngine(workingDirectory);

    drawing->setScreenResolution(screen[0], screen[1]);

    console.initTextures();

    checkExtensions();

    // Start the factory threads
    pthread_create(&soundThread, NULL, startSoundFactoryThread, NULL);

    // Set the default sensitivity
	msensitivity=5;
	view2d=false;

    TP = new Terrain();
//	TP->Initialise();

    center[0]=screen[0]/2;
	center[1]=screen[1]/2;

	CamPos[0]=0.0f;
	CamPos[1]=0.0f;
	CamPos[2]=0.0f;

	MoveKey[0]=false;
	MoveKey[1]=false;
	MoveKey[2]=false;
	MoveKey[3]=false;

    // Generate the session id
    #ifndef WIN32
	usleep(30000);
    #else
	Sleep(300);
    #endif
	srand(SDL_GetTicks());
	sessionId = rand();

    setFov(75);

    fps = 1; // To take care of any divisions by zero
    fpsCounter = new FPSCounter(0, 0);

    // Instantiate the developer's menu
    devMenu = new DevMenu();

    return true;
}

void Azadi::storeMatrices()
{
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview_matrix);
    glGetDoublev(GL_PROJECTION_MATRIX, projection_matrix);
}

int Azadi::Run()
{
    if(!Init()){
		console << GUI::Console::error << GUI::Console::high << "Azadi engine initialisation failed" << GUI::Console::endl();
		return 1;
	}

    console << GUI::Console::log << GUI::Console::highish << "====================================" << GUI::Console::endl();
	console << GUI::Console::log << GUI::Console::highish << "Azadi engine initialisation finished" << GUI::Console::endl();
	console << GUI::Console::log << GUI::Console::highish << "====================================" << GUI::Console::endl();

    // Loads the main azadi script file
	script_engine->runfile("main.azs");

    // The following code is necessary otherwise the distance of wherever the mouse was
    // to the center of the screen gets accounted for in the rotations later on.
    // Which means sometimes we end up up-side-down
	SDL_WarpMouse(center[0], center[1]);

	script_engine->dostring("intro()");	// Scripted intro
    script_engine->returnedInt();

    int ret;
    bool done = false;
    while (!done)
    {
        ret = Lobby();
        switch (ret)
        {
            case 1:
                done = true;
                break;
            case 0:
                Mainloop();
                continue;
        }
        if (shuttingDown)
            break;
    }

    console << GUI::Console::low << GUI::Console::normal << "Closing engine" << GUI::Console::endl();

    delete TP;
    if (postProcessAvailable)
    {
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
        glDeleteFramebuffersEXT(1, &renderBuffer);
        glDeleteRenderbuffersEXT(1, &depthBuffer);
        glDeleteRenderbuffersEXT(1, &stencilBuffer);
        glDeleteTextures(1, &renderBufferTexture);
    }

    console << "Saving current configuration" << GUI::Console::endl();
    Config *conf = new Config();
    conf->save();
    delete conf;

    return 0;
}

int Azadi::Mainloop()
{
    SDL_ShowCursor(SDL_DISABLE);

    sound_factory->flush();

    // Reset the camera position.
    CamPos[0] = 0.0f;
    CamPos[1] = 0.0f;
    CamPos[2] = 0.0f;

    //! Initialize things using the scripting engine
    script_engine->dostring("init()");
    script_engine->returnedInt();

    jukebox = new JukeBox;
    showJukeBox = false;

    //! Initialize the juke box
    jukebox->init(workingDirectory);

    //! Updates the file list for the jukebox
    jukebox->update();

    unsigned int lastupdate=0;
    unsigned int intervall=30;
    unsigned int fpslastval=0;
    unsigned int fpslimit=19;
    unsigned long int gameframes=0;
    unsigned long int framecount=0;
    unsigned long int currentTicks=0;

    // Move the mouse to the center one more time
    SDL_WarpMouse(center[0], center[1]);

      // flush the input queue
    input.get();

    console << GUI::Console::endl();
    console << GUI::Console::low << GUI::Console::log << "==== Entering main loop ====" << GUI::Console::endl();

    if (delayEnabled)
    {
        while (!shuttingDown)
        {
            currentTicks = SDL_GetTicks();
            activeNode = sceneTranslateNode;
            devMenu->parseInput(input);
            physics_engine->tick();
            script_engine->dostring("main()");
            ProcessMessages();
            alListener3f(AL_POSITION, CamPos[0], CamPos[1], CamPos[2]);
            jukebox->tick(input, CamPos[0], CamPos[1], CamPos[2]);
            model_factory->Update();
            particle_factory->tick();
            ++framecount;
            delay(1);
            if (!view2d && !showJukeBox)
                SDL_WarpMouse(center[0], center[1]);
            if ((fpslastval + fpslimit) < currentTicks)
            {
                fpslastval = currentTicks;
                Draw();
            }
            if (framecount > 10)
            {
                fps = 10000.0f / (currentTicks - gameframes);
                gameframes = currentTicks;
                framecount = 0;
            }
        }
    }
    else
    {
        while (!shuttingDown)
        {
            currentTicks = SDL_GetTicks();
            activeNode = sceneTranslateNode;
            devMenu->parseInput(input);
            physics_engine->tick();
            script_engine->dostring("main()");
            ProcessMessages();
            alListener3f(AL_POSITION, CamPos[0], CamPos[1], CamPos[2]);
            jukebox->tick(input, CamPos[0], CamPos[1], CamPos[2]);
            model_factory->Update();
            particle_factory->tick();
	//TP.tick();
            ++framecount;
            if (!view2d && !showJukeBox && (currentTicks - lastupdate >= intervall))
            {
                    SDL_WarpMouse(center[0], center[1]);
                    lastupdate=currentTicks;
            }
            if ((fpslastval + fpslimit) < currentTicks)
            {
                fpslastval = currentTicks;
                Draw();
            }
            if (framecount > 10)
            {
                fps = 10000.0f / (currentTicks - gameframes);
                gameframes = currentTicks;
                framecount = 0;
            }
        }
    }

    console << GUI::Console::low << GUI::Console::log << "==== Exiting main loop ====" << GUI::Console::endl();
    console << GUI::Console::endl();

    sound_factory->flush();
    delete jukebox;

    input.get();
    input.flush();

    return 0;
}

/*============================================================================

    All graphical related items go under here as well as any game-specific
    routines. Try to keep the core functions above clean.

=============================================================================*/

void Azadi::ProcessMessages()
{
    input.get();

    // The script engine gets first preference (as long as nothing registers the keys)
    script_io->parseInput(input, viewport_matrix, projection_matrix, modelview_matrix, CamPos);

    console.parseInput();

    if (input.getKeyPress(SDLK_PAGEUP))
        console.increaseVerbosityLevel();

    if (input.getKeyPress(SDLK_PAGEDOWN))
        console.decreaseVerbosityLevel();

    if (input.getKeyPress(SDLK_ESCAPE))
        shuttingDown = true;

    if (input.getKeyPress(SDLK_TAB))
        console.toggleShow();

    if (input.getKeyDown(SDLK_w))
        MoveKey[FORWARD] = true;
    else
        MoveKey[FORWARD] = false;

    if (input.getKeyDown(SDLK_s))
        MoveKey[BACKWARD] = true;
    else
        MoveKey[BACKWARD] = false;

    if (input.getKeyDown(SDLK_a))
        MoveKey[LEFT] = true;
    else
        MoveKey[LEFT] = false;

    if (input.getKeyDown(SDLK_d))
        MoveKey[RIGHT] = true;
    else
        MoveKey[RIGHT] = false;

    if (input.getKeyPress(SDLK_m))
    {
        showJukeBox = !showJukeBox;
        SDL_ShowCursor((showJukeBox)?SDL_ENABLE:SDL_DISABLE);
        jukebox->toggleVisibility();
    }

    if (input.getMouseDown(3))
        devMenu->setVisible(true);
    else
        devMenu->setVisible(false);

    mouse[0] = input.getMouseX();
    mouse[1] = input.getMouseY();

    if (input.anyKeysPressed())
    {
        Math::Vector3<GLfloat> forward;
        Math::Vector3<GLfloat> side;

        float Xangle, Yangle;

        Xangle = iy*piover180;
        Yangle = ix*piover180;

        forward.y = -sin(Xangle);
        forward.x = cos(Xangle)*sin(Yangle);
        forward.z = cos(Xangle)*-cos(Yangle);

        forward.normalize();

        side=forward.cross(Math::Vector3<GLfloat> (0.0f, 1.0f, 0.0f));

        if (view2d){
            forward = forward.cross(side * -1);
        }

        if(MoveKey[FORWARD]){
            CamPos[movementAxis[0]]-=(forward.x*(constrainedAxis[0]?0:movespeed))/fps;
            CamPos[movementAxis[1]]-=(forward.y*(constrainedAxis[1]?0:movespeed))/fps;
            CamPos[movementAxis[2]]-=(forward.z*(constrainedAxis[2]?0:movespeed))/fps;
        }
        if(MoveKey[BACKWARD]){
            CamPos[movementAxis[0]]+=(forward.x*(constrainedAxis[0]?0:movespeed))/fps;
            CamPos[movementAxis[1]]+=(forward.y*(constrainedAxis[1]?0:movespeed))/fps;
            CamPos[movementAxis[2]]+=(forward.z*(constrainedAxis[2]?0:movespeed))/fps;
        }
        if(MoveKey[LEFT]){
            CamPos[movementAxis[0]]+=(side.x*(constrainedAxis[0]?0:movespeed))/fps;
            CamPos[movementAxis[1]]+=(side.y*(constrainedAxis[1]?0:movespeed))/fps;
            CamPos[movementAxis[2]]+=(side.z*(constrainedAxis[2]?0:movespeed))/fps;
        }
        if(MoveKey[RIGHT]){
            CamPos[movementAxis[0]]-=(side.x*(constrainedAxis[0]?0:movespeed))/fps;
            CamPos[movementAxis[1]]-=(side.y*(constrainedAxis[1]?0:movespeed))/fps;
            CamPos[movementAxis[2]]-=(side.z*(constrainedAxis[2]?0:movespeed))/fps;
        }
    }

    // Set it so that if the space button is being held, the screen doesn't rotate
	if (!view2d && !showJukeBox){
        iy-=((center[1]-mouse[1])*msensitivity ) /fps;   // The mouse movement speed though hasn't been changed
        ix-=((center[0]-mouse[0])*msensitivity ) /fps;
	}

    //Keep the angles small
    if(iy>90) iy=90;
    if(iy<-90) iy=-90;
    if(ix>359) ix-=359;
    if(ix<0) ix+=359;

    sceneTranslateNode->updateTranslation(CamPos[0], CamPos[1], CamPos[2]);
	sceneRotateNode->updateRotation(iy, ix, 0.0f);
}

int Azadi::Lobby()
{
    SDL_ShowCursor(SDL_ENABLE);

    MainMenu mMenu;
    mMenu.init(screen[0] - 350, (screen[1] / 2) - 200, 300, 400);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    unsigned long int lastframe = 0;
    unsigned long int frameinterval = 19;

    int ret = -1;
    while (ret == -1)
    {
        delay(1);
        input.flush();
        input.get();

        mMenu.parseInput(input);
        ret = mMenu.clickedButton();

        if ((lastframe + frameinterval) > SDL_GetTicks())
            continue;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mMenu.draw(screen[0], screen[1]);
        Drawing::getInstance()->flipBuffers();
        lastframe = SDL_GetTicks();
    }
    SDL_ShowCursor(SDL_DISABLE);
	return ret;
}

bool Azadi::Draw()
{
    if (view2d)
        SDL_ShowCursor(SDL_ENABLE);

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glViewport(0, 0, screen[0], screen[1]);

    if (postProcessAvailable)
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, renderBuffer);

    if (antiAliasEnabled)
    {
        glEnable(GL_MULTISAMPLE);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    }

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // traverses the scene's frustum node
    sceneFrustumNode->execute();
    // Store the final project/modelview matrices
    storeMatrices();

    // Disable all anti aliasing
    glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
    glDisable(GL_MULTISAMPLE);

    sceneOrthoNode->execute();

    if (postProcessAvailable)
    {
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

        if (!createdList)
        {
            glDeleteLists(postProcessList,1);
            glNewList(postProcessList, GL_COMPILE_AND_EXECUTE);
            if (postProcessStyle == 1)
                shaders->applyShader("Bloom");
            else if (postProcessStyle == 2)
                shaders->applyShader("HDR");
            else
                shaders->applyShader("Default");

            glClear(GL_COLOR_BUFFER_BIT);
            glBindTexture(GL_TEXTURE_2D, renderBufferTexture);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0.0f, screen[0], screen[1], 0.0f, -1.0f, 2000.0f);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glDisable(GL_CULL_FACE);

            glColor3f(1.0f, 1.0f, 1.0f);
            glBegin(GL_QUADS);
            {
                glTexCoord2f(0.0f, 1.0f);
                glVertex2i(0, 0);
                glTexCoord2f(1.0f, 1.0f);
                glVertex2i(screen[0], 0);
                glTexCoord2f(1.0f, 0.0f);
                glVertex2i(screen[0], screen[1]);
                glTexCoord2f(0.0f, 0.0f);
                glVertex2i(0, screen[1]);
            }
            glEnd();

            shaders->applyShader("Default");
            glEndList();

            createdList = true;
        }
        else
            glCallList(postProcessList);
    }

    // Place all the VGUI related things in here.
    devMenu->draw(mouse[0],mouse[1]);
    // draw the fps counter
    fpsCounter->draw();
    // draw the jukebox
    jukebox->draw(screen[0], screen[1]);
    // draw the console
    console.draw();

    // Flip the double buffered display and count the FPS
    Drawing::getInstance()->flipBuffers();
    return true;
}
