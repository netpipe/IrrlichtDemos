class Gameinfo
{
      public :
      // Global variables contained in this information object.

      float head,view;
      bool fin1,crouch,zoom;
      SIrrlichtCreationParameters param;


      Gameinfo();
      void Init();
      void recursiveFillMetaSelector(scene::ISceneNode* node, scene::IMetaTriangleSelector* meta );
      void loadscene();
      ISceneNode* GetRoot();
};

////////////////////////////////////////////////////////////////////////////////
// Put the initial value for creating the device
// Initialized when the Gameinfo class is used
////////////////////////////////////////////////////////////////////////////////
      Gameinfo::Gameinfo()
      // Automatic initialization of the class that setup theses variables
      {
           //To be replaced by a function that read a XML config file and config interface
           param.DriverType = video::EDT_OPENGL;
           //param.DriverType = video::EDT_OPENGL; // Testing OPENGL driver
           param.WindowSize = core::dimension2d<s32>(800,600);
           param.Bits = 32;
           param.Fullscreen = false;
           param.Stencilbuffer = true;
           param.Vsync = false;
           param.AntiAlias = true;

           head=34; // Height of the collision sphere translation
           fin1=false;
           view=45;
      }
////////////////////////////////////////////////////////////////////////////////
// Create the device, using the current parameters
//
////////////////////////////////////////////////////////////////////////////////
      void Gameinfo::Init()
      {
          //device = createDevice(video::EDT_DIRECT3D9);
          device = createDeviceEx(param);
      }

////////////////////////////////////////////////////////////////////////////////
// Load the an IRR defined scene
// May need some change to be more generic (parameters)
////////////////////////////////////////////////////////////////////////////////
      void Gameinfo::loadscene()
      {
      // Setup to use MIP mapping
      device->getVideoDriver()->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);
      // Setup the ZIP file "MyLevel.pk3" to be read directly
      device->getFileSystem()->addZipFileArchive("levels/Station.pk3");
      // Load the scene file from the archive
      device->getSceneManager()->loadScene("level.irr");
      }
////////////////////////////////////////////////////////////////////////////////
// Function to retrieve the root node
// Not really used
////////////////////////////////////////////////////////////////////////////////
      ISceneNode* Gameinfo::GetRoot()
      {return device->getSceneManager()->getRootSceneNode();}


