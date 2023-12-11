
    /// ##########################################################################################
    /// ############# INTRO ######################################################################
    /// ##########################################################################################


    /// Using CODE::BLOCKS, edit these:
    /// PROJECT --> BUILD OPTIONS (Select the "main", at top left) --> LINKER SETTINGS TAB (edit there)
    /// PROJECT --> BUILD OPTIONS (Select the "main", at top left) --> SEARCH DIRECTORIES TAB --> LINKER TAB (edit there)

    /// You must edit the above ones, in my case they are:
    /// C:\Program Files (x86)\CodeBlocks\SDK\Irrlicht\lib
    /// C:\Program Files (x86)\CodeBlocks\SDK\irrBullet\lib
    /// C:\Program Files (x86)\CodeBlocks\SDK\Bullet\lib

    /// Engine (Irrlicht)
    #include <irrlicht.h>  // Declare Irrlicht
    // Declare Used Namespaces:
    using namespace irr;
    using namespace core;
    using namespace video;
    using namespace scene;
    using namespace io;
    using namespace gui;





    /// Events (For Irrlicht)
    #include "libraries/MainLibs/myEvents.h"    // MY EVENTS (KEYS + MOUSE + USER EVENTS)
    #include "libraries/MainLibs/myEffects.h"   // MY EFFECTS (motion blur, bloom)
    // #include "my3DAudio.h"                   // MY 3D AUDIO (Using Audiere)
    // #include "CEffects.h"                    // MORE EFFECTS (motion blur, bloom)
    // #include "readXML.h"

    #include "libraries/MainLibs/CWaterSurface.h"        // For ocean effects.
    #include "libraries/MainLibs/CWaterShader.h"         // For ocean effects.

    /// Ball reflexions:
    /// #include "libraries/MainLibs/CSphereMapRendererSceneNode.h"

    /// Configuration XML file reader (For Irrlicht)
    /// #include "libraries/SettingsReader/settings.h"

    /// Loading Bar (For Irrlicht)
    /// #include "libraries/MainLibs/CLoadingBar.h"

    /// Custom Animators (For Irrlicht)
    /// #include "libraries/MainLibs/CSceneNodeAnimatorFlyCircleExtended.h"

    /// Clouds for Irrlicht (3 Layers)
    #include "libraries/MainLibs/CloudSceneNode.h"

    /// Custom Animator Irrlicht ( http://irrlicht.sourceforge.net/forum/viewtopic.php?t=34606 )
    // #include "libraries/MainLibs/CFollowPathAnimator.h"
    // #include "libraries/MainLibs/CBSplineAnimator.h"

    /// XEffects: ( http://irrlicht.sourceforge.net/forum/viewtopic.php?f=6&t=30631 )
    #include "libraries/XEffects/Source/XEffects.h"

    /// Decal System / Manager: ( http://irrlicht.sourceforge.net/forum/viewtopic.php?f=6&t=44968 )
    /// #include "libraries/DecalSystem/DecalManager.h"

    /// Effects (For Irrlicht)
    // EFFECTS STUFF - Include the headers:
    /// #include "libraries/PostProcessor/CRendererPostProc.h"
    /// #include "libraries/PostProcessor/CEffectPostProc.h"
    // #include "libraries/PostProcessor/CTransitionPostProc.h"
    // #include "libraries/PostProcessor/CSplitPostProc.h"
    // #include "libraries/PostProcessor/CLensFlarePostProc.h"
    // #include "libraries/PostProcessor/CWaterPostProc.h"

    /// Bullet Physics (For Irrlicht)
    /// Colocar as Bullet libraries nas pastas:
    /// "K:\JORGE - TRABALHOS\3D - WORKS - Irrlicht\_TOOLS\irrlicht-1.7.1\include"
    /// "K:\JORGE - TRABALHOS\3D - WORKS - Irrlicht\_TOOLS\irrlicht-1.7.1\examples\000000000000000.Camara.Municipal.Marinha.Grande\libraries"
    /// #include "libraries/Bullet/src/btBulletCollisionCommon.h"
    /// #include "libraries/Bullet/src/btBulletDynamicsCommon.h"
    /// #include <BulletCollision\CollisionDispatch\btGhostObject.h>

    /// http://irrlicht.sourceforge.net/forum/viewtopic.php?f=9&t=39007
    // #include "libraries/MainLibs/CPhysics.h"
    // #include "libraries/MainLibs/DebugDraw.h"
    #include <irrBullet.h>

    /// http://irrlicht.sourceforge.net/forum/viewtopic.php?f=6&t=36861
    // #include "libraries/irrBullet/source/irrbullet.h"
    // #include "libraries/irrBullet/examples/framework/exampleframework.h"
    // #include "exampleframework.h"

    /// Play Video (For Irrlicht)
    // #include "libraries/OpenCV/include/opencv/cv.h"
    // #include "libraries/OpenCV/include/opencv/cxcore.h"
    // #include "libraries/OpenCV/include/opencv/highgui.h"

    /// K:\JORGE - TRABALHOS\3D - WORKS - Irrlicht\_TOOLS\irrlicht-1.7.1\source\Irrlicht
    // #include "libraries/MainLibs/View.h"
    // #include "libraries/MainLibs/CFollowPathAnimator.h"

    /// Loading TTF Fonts (For Irrlicht)
    // #include "libraries/Internationalization/irrUString.h"
    // #include "libraries/Internationalization/CGUITTFont.h"

    /// Tools (For Irrlicht)
    // #include "libraries/Tools/GameCameraPathEditor.h"

    /// Lights Manager (For Irrlicht)
    // #include "libraries/MainLibs/myLights_Manager.h"


    /// Events (For Irrlicht)
    // #include "libraries/MainLibs/myEvents.h"

    /// Lens Flare (For Irrlicht)
    // #include "libraries/MainLibs/CLensFlareSceneNode.h"

    /// irrXML is a xml parser (For Irrlicht)
    // #include "libraries/IrrXML/irrXML.h"

    /// Effects (For Irrlicht)
    // EFFECTS STUFF - Include the headers:
    // #include "libraries/PostProcess/src/PostProcessManager.h"

    /// Water
    // #include "libraries\MainLibs\CWaterSurface.h" // For ocean effects.
    // #include "libraries\MainLibs\CWaterShader.h"  // For ocean effects.
    // #include "CBloodEffect.h"
    // #include "CBloodShader.h"

    /// #include "audiere.h" // For Audio. ALSO must add "audiere" in the Project->Build Options->Linker !!!
    // #include "audiere_irrlicht.h"

    /// Decal System / Manager: ( http://irrlicht.sourceforge.net/forum/viewtopic.php?f=6&t=44968 )
    // #include "libraries/DecalSystem/DecalManager.h"



    /// System
    #include <fstream>
    #include <iostream>
    #include <sstream>
    #include <vector>
    #include <string>
    #include <cstring>
    #include <stdio.h>
    #include <dirent.h>
    #include <algorithm>
    #include <ctype.h>
    #include <ctime>         // For time()
    #include <cstdlib>       // For srand() and rand()
    // #include <windows.h>  // Used for Sleep()
    // Declare Namespaces:
    using namespace std;


    /// Other stuff...
    #ifdef _IRR_WINDOWS_
    #pragma comment(lib, "Irrlicht.lib")
    #endif


