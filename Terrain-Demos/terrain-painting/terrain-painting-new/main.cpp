    #include <irrlicht.h>

    using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

    video::IImage* heightmap;
    video::IImage* brush;

    scene::ITerrainSceneNode* terrain;
video::IVideoDriver* driver;
IrrlichtDevice* device;
    /*==============================================================================
      Receiver class
    ==============================================================================*/
    class MyEventReceiver : public IEventReceiver {
        bool KeyIsDown[KEY_KEY_CODES_COUNT];
        bool LEFTBUTTONCLICKED;
        bool RIGHTBUTTONCLICKED;

        public:
            virtual bool OnEvent(const SEvent& event){
                if (event.EventType == irr::EET_KEY_INPUT_EVENT)
                    KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

	if ( event.EventType == EET_GUI_EVENT )
	{
		s32 cid = event.GUIEvent.Caller->getID();

		switch ( event.GUIEvent.EventType )
		{
			case EGET_BUTTON_CLICKED:
				switch ( cid )
				{
					case 101:

					brush = driver->createImageFromFile("brush4.png");
					device->getCursorControl()->setVisible(false);
						return true;
					case 102:
					//	login = true;
						return true;
				}
		}
    };
                if(event.EventType == EET_MOUSE_INPUT_EVENT){
                    if(event.MouseInput.Event==EMIE_LMOUSE_PRESSED_DOWN) LEFTBUTTONCLICKED = true;
                    else if(event.MouseInput.Event==EMIE_LMOUSE_LEFT_UP) LEFTBUTTONCLICKED = false;
                    else if(event.MouseInput.Event==EMIE_RMOUSE_PRESSED_DOWN) RIGHTBUTTONCLICKED = true;
                    else if(event.MouseInput.Event==EMIE_RMOUSE_LEFT_UP) RIGHTBUTTONCLICKED = false;
                }

                return false;
            }

            virtual bool IsKeyDown(EKEY_CODE keyCode) const { return KeyIsDown[keyCode]; }
            virtual bool IsLMBDown() const { return LEFTBUTTONCLICKED; }
            virtual bool IsRMBDown() const { return RIGHTBUTTONCLICKED; }

            MyEventReceiver(){
                for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
                    KeyIsDown[i] = false;

                LEFTBUTTONCLICKED = RIGHTBUTTONCLICKED = false;
            }
    };

    /*==============================================================================
      Raise or lower terrain (selected vertice)
    ==============================================================================*/
    void RaiseTerrainVertex(scene::IMesh* pMesh,s32 index, f32 step, bool up){
       // scene::IMesh* pMesh = terrain->getMesh();

        s32 heightmapWidth = heightmap->getDimension().Width;
        s32 heightmapHeight = heightmap->getDimension().Height;

        s32 b;
        for (b=0; b<pMesh->getMeshBufferCount(); ++b){
            scene::IMeshBuffer* pMeshBuffer = pMesh->getMeshBuffer(b);
            // skip mesh buffers that are not the right type
            if (pMeshBuffer->getVertexType() != video::EVT_2TCOORDS) continue;

            video::S3DVertex2TCoords* pVertices = (video::S3DVertex2TCoords*)pMeshBuffer->getVertices();

            s32 brushWidth = brush->getDimension().Width;
            s32 brushHeight = brush->getDimension().Height;

            for(int y = 0; y < brushHeight; y++){
                for(int x = 0; x < brushWidth; x++){
                    video::SColor brushPixel = brush->getPixel(x, y);

                    if((index-(brushWidth/2)-((brushWidth/2)*heightmapWidth) + (x+(y*heightmapWidth))) >= 0){
                        f32 hy = pVertices[index-(brushWidth/2)-((brushWidth/2)*heightmapWidth) + (x+(y*heightmapWidth))].Pos.Y;
                        f32 bp = brushPixel.getRed()/255.0*step;
                        bp = (up)?bp:-bp;

                        if(bp > 0 && hy+bp <= 255)
                            pVertices[index-(brushWidth/2)-((brushWidth/2)*heightmapWidth) + (x+(y*heightmapWidth))].Pos.Y = hy+bp;
                    }
                }
            }
        }

        // force terrain render buffer to reload
        terrain->setPosition(terrain->getPosition());
    }

    /*==============================================================================
      Save file
    ==============================================================================*/
    void save (video::IVideoDriver* driver){
        s32 heightmapWidth = heightmap->getDimension().Width;
        s32 heightmapHeight = heightmap->getDimension().Height;

        const core::dimension2d<u32> dim (heightmapWidth, heightmapHeight);
        video::IImage *img = driver->createImage(video::ECF_A8R8G8B8, dim);

        video::S3DVertex2TCoords* verts = (video::S3DVertex2TCoords*)terrain->getMesh()->getMeshBuffer(0)->getVertices();

        for (u32 y= 0, i = 0; y < heightmapHeight; y++){
            for(u32 x = 0; x < heightmapWidth; i++, x++){
                u8 py = (u8)verts[i].Pos.Y;
                img->setPixel((heightmapHeight-1)-y, x, video::SColor(0, py, py, py));
            }
        }

        driver->writeImageToFile(img, "heightmap.bmp", 0);
        img->drop();
    }


    int main(){
        MyEventReceiver receiver;
//vector2d resolution[1];
unsigned int resolution[2];
resolution[0] = 800;
resolution[1] =600;
        device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(resolution[0], resolution[1]), 32, false, true, false, &receiver);

        driver = device->getVideoDriver();
        scene::ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();
      //  device->getCursorControl()->setVisible(false);

        io::path heightmapFile = "heightmap.bmp";
        heightmap = driver->createImageFromFile(heightmapFile);
        brush = driver->createImageFromFile("brush.png");

        terrain = smgr->addTerrainSceneNode(heightmapFile, 0, -1, core::vector3df(0, 0, 0));
        terrain->setScale(core::vector3df(32, 5, 32));
        terrain->setMaterialFlag(video::EMF_LIGHTING, false);
        	terrain->setMaterialTexture(0, driver->getTexture("./media/Grass.jpg"));

        terrain->setPosition(terrain->getPosition());

        scene::ITriangleSelector* selector = smgr->createTerrainTriangleSelector(terrain, 0);

        // Arrow
        scene::ISceneNode* arrow = smgr->addAnimatedMeshSceneNode(smgr->addArrowMesh("arrow", video::SColor(255, 255, 0, 0), video::SColor(255, 0, 255, 0)), NULL);
        arrow->setMaterialFlag(video::EMF_LIGHTING, false);
        arrow->setScale(core::vector3df(10, 10, 10));
        arrow->setRotation(core::vector3df(0,0,180));

        scene::ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS(0, 100.0f, 5.1f);
        cam->setPosition(core::vector3df(-100,500,100));
       cam->setTarget(core::vector3df(0,0,0));

        ITimer* irrTimer = device->getTimer();
        u32 then = 0, then30 = 0;
        char c[24];
        f32 step = 2.f;
        bool wireframe = false;

	ITexture *menuButtonNormal = driver->getTexture ( "brush4.png" );
	ITexture *menuButtonDepressed = driver->getTexture ( "brush4.png" );


	int buttonPosX = ( resolution[0] / 2 ) - 60;
	int buttonPosY = ( resolution[1] / 100 ) * 80;

	IGUIButton *exitButton = guienv->addButton ( rect<s32> ( buttonPosX, buttonPosY + 50, buttonPosX + 120, buttonPosY + 70 ), 0, 101, L"Exit" );
	exitButton->setImage ( menuButtonNormal );
	exitButton->setPressedImage ( menuButtonDepressed );

        while(device->run()){
            if(device->isWindowActive()){
                u32 now = irrTimer->getTime();

                if (then30 < now){
                    if(receiver.IsKeyDown(irr::KEY_ESCAPE)) break;

                    if (receiver.IsKeyDown(irr::KEY_KEY_W) && then < now){
                        wireframe = !wireframe;
                        terrain->setMaterialFlag(video::EMF_WIREFRAME, wireframe);

                        then = now + 300;
                    }

                    if (receiver.IsKeyDown(irr::KEY_F4) && then < now){
                        step += 1.f;

                        then = now + 100;
                    } else if (receiver.IsKeyDown(irr::KEY_F5) && then < now && step > 0){
                        step -= 1.f;

                        then = now + 100;
                    }
else if (receiver.IsKeyDown(irr::KEY_SPACE)){
    if (cam->isInputReceiverEnabled()){
        cam->setInputReceiverEnabled(false);
        device->getCursorControl()->setVisible(true);
} else {    cam->setInputReceiverEnabled(true);
					device->getCursorControl()->setVisible(false);
					}
                }


                    if(receiver.IsKeyDown(irr::KEY_KEY_S))
                        save (driver);

                    // move the arrow to the nearest vertex ...
                    //400, 300 si la résolution utilisée est 800x600
                    const core::position2di clickPosition = core::position2di(400, 300);
                    const core::line3d<float> ray = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(clickPosition, cam);
                    core::vector3df pos;
                    core::triangle3df Tri;

                     scene::ISceneNode* out;
                    if (smgr->getSceneCollisionManager()->getCollisionPoint(ray, selector, pos, Tri, out)){
                        //arrow->setPosition(pos);
                        static const s32 scale = 32; // terrain is scaled 32X
                        static const s32 size = 256; // heightmap is 512x512 pixels
                        s32 x = (s32)(pos.X / scale);
                        s32 z = (s32)(pos.Z / scale);
                        s32 index = x * size + z;

                        // ... Move it if clicked
                        if(cam->isInputReceiverEnabled() &&receiver.IsLMBDown() || receiver.IsRMBDown() && then < now){
                            RaiseTerrainVertex(terrain->getMesh(),index, step, receiver.IsLMBDown());
                            then = now + 100;
                        }

                        x *= scale;
                        z *= scale;

                        arrow->setPosition(core::vector3df(x, terrain->getHeight(x, z) + 20, z));
                    }
                    device->sleep(2);
                    driver->beginScene(true, true, video::SColor(255, 255, 255, 255));
                    smgr->drawAll();
                    guienv->drawAll();
                    driver->endScene();

                    then30 = now + 30;
                }
            }
        }

        heightmap->drop();
        brush->drop();

        device->closeDevice();
        device->drop();

        return 0;
    }
