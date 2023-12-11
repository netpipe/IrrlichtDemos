#include <irrlicht.h>
#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

using namespace irr;
using namespace core;
using namespace gui;
using namespace video;
using namespace scene;

IrrlichtDevice *Device = 0;
video::E_DRIVER_TYPE driverType;
core::stringc StartUpModelFile ;
core::stringw MessageText;
core::stringw Caption;

unsigned int cursortype = 0; // define the shape of the cursor (used inside the editor)

core::dimension2d<s32> screensize;

IGUIImage *img=0;

IGUISpriteBank * icons=0;
IGUISpriteBank* sprites;

IGUIButton * moveMDL=0;
IGUIButton * rotaMDL=0;
IGUIButton * scalMDL=0;
IGUIButton * strcMDL=0;
IGUIStaticText* coords = 0;
IGUIStaticText* fpstext = 0;
IGUIStaticText* animframes = 0;
IGUIStaticText* animseq = 0;
IGUIStaticText* vptopright = 0;
IGUIStaticText* vptopleft = 0;
IGUIStaticText* vpbottomright = 0;
IGUIStaticText* vpbottomleft = 0;
IGUIStaticText* seltitle = 0;                              // display coordinates
IGUITabControl* tab = 0;
IGUITabControl* tab1 = 0;
s32 ThePoly=0;
s32 fakecounter=0;
bool mouserightpress = false;

bool fullscreen=false;
bool loading=false;
bool refresh=true;
bool refreshanimwin = false;
bool animated=false;
bool atcamtarget = true;
int startAnim = 0;
int endAnim = 0;
int viewrightspace=180; // Right space at the end of the viewport;                // contain the number of polygons in the Model
stringc appPath = "";


//scene::IAnimatedMesh * ModelMesh[1025] = {0,0};    // old mesh pointers (remove)
scene::E_ANIMATED_MESH_TYPE animateType;
scene::IAnimatedMesh * ModelMs = 0;                         // new mesh pointer
scene::IAnimatedMeshSceneNode* Model = 0;                       // Node pointer
IGUIScrollBar* AnimScroll = 0;                  // Scrollbar for animation frames
int selNode = 0, lastNode=0;      // Current selected node, define the last node
gui::IGUIListBox * selList = 0;                          // Item selection list
core::stringw selName[1025];                                // name of the mesh
//scene::ICameraSceneNode* Camera[5] = { 0, 0};
scene::ILightSceneNode  *light1;
scene::ILightSceneNode  *light2;
scene::ILightSceneNode  *light3;
//#include "gui/CGridSceneNode.h"
//CGridSceneNode* grid;
// Work on scene nodes

#include "viewports.h"                               // viewports functions
viewprt viewp; // use the viewport class inside this program.
#include "FileLoader.h"
files fileRequest; // use the filerequest class inside this program.


#include "GuiSelectList.h"                     // selection list management
#include "SceneManagement.h"                    // Toolbox Gui creation and updates
#include "ToolBox.h"
#include "GuiCreation.h"          // Gui item creations (menu, buttons, etc)
#include "cursors.h"                 // cursor shape function
#include "animation.h"                        // Model animation functions
#include "lighting.h"                    // Lighting related functions
#include "ModelFunctions.h"                              // Loading, export etc.
#include "ItemCreation.h"                                // default editor items
#include "configuration.h"                      // loading, saving editor config

#include "event_receiver.h"  // treatment of the events

int Render(){

    Device->run();

        if (Device->isWindowActive())
        {
            if (refresh)
            {
                setCursor();
                if (viewp.currentview==1) viewp.drawViewPorts(viewp.viewscale2,viewp.viewscale3,viewp.viewscale4,0);
                else viewp.drawSingle(viewp.currentview);
            }
    Device->sleep(30,0);
            if (refreshanimwin) refreshAnimWin();
            //printf ("Currentview: %i\n",currentview);
            core::stringw result="";
            core::stringw str(L"FPS: ");
//            str.append(core::stringw(Device->getFPS()));
            if (Model && (Model->getType() ==   ESNT_OCTREE))
            {
                io::IAttributes* attribs = Device->getFileSystem()->createEmptyAttributes();
                if (attribs)
                {
                    // get the mesh name out
                    Model->serializeAttributes(attribs);
                    core::stringc name = attribs->getAttributeAsString("Mesh");
                    attribs->drop();
                    // get the animated mesh for the object
                    scene::IAnimatedMesh* mesh = Device->getSceneManager()->getMesh(name.c_str());
                    ModelMs=mesh;
                }
                if (ModelMs)
                {
                    str += L" | Tris: ";
                    if (ModelPoly(ModelMs)>0) str.append(core::stringw(ModelPoly(ModelMs)));
                    else  str+="Unknown |";
                    if (ModelMs) str.append(checkModel(ModelMs));
                    str+="| OCTTREE";
                }

            }
            if (Model)
            {
                if ((Model->getType() ==   ESNT_ANIMATED_MESH))
                {
                    str += L" | Tris: ";
                    if (ModelPoly(ModelMs)>0)
                    {
                        str.append(core::stringw(ModelPoly(ModelMs)));
                        str+=" |";
                    }
                    else  str+="Unknown |";
                    if (ModelMs) str.append(checkModel(ModelMs));
                    int currentFRM = (int)Model->getFrameNr();
                    if (ModelMs->getFrameCount()>0)
                    {
                        str+=L"| Frame:";
                        str.append((core::stringw)currentFRM);
                        str+="/";
                        str.append((core::stringw) (int) (ModelMs->getFrameCount()-1));
                    }
                    //if (ModelMesh[selNode]) str.append((core::stringw)(int)(ModelMesh[selNode]->getFrameCount()-1));
                }
            }
            fpstext->setText(str.c_str());
            //fpstext->setText(s.c_str());
        }
        else
            Device->sleep(500,0);
  //  }

    }

    void main_loop(){
    Render();


    }
int main()
{
#ifdef __EMSCRIPTEN__
    driverType = video::EDT_OGLES2;
    #else
      driverType = video::EDT_OPENGL;
    #endif
    MyEventReceiver receiver;

    Device = createDevice(driverType, dimension2du(1024, 768), 32,
                          false, false, false, &receiver);

    loadXMLConfig();
    //printf ("Here is the current screen size: %i X %i \n",screensize.Width,screensize.Height);

    if (Device == 0)
        return 1; // could not create selected driver.

    Device->setResizable(true);
    Device->setWindowCaption(L"Irrlicht Engine - Loading...");
    video::IVideoDriver* driver = Device->getVideoDriver();
    IGUIEnvironment* env = Device->getGUIEnvironment();
    scene::ISceneManager* smgr = Device->getSceneManager();
    smgr->getParameters()->setAttribute(scene::COLLADA_CREATE_SCENE_INSTANCES, true);
    driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);
    // add our media directory as "search path"
   // Device->getFileSystem()->addFolderFileArchive ( "media/" );
    icons=env->addEmptySpriteBank("list_bank");
    selNode=0; // No selected node;
    // Init the GUI
    createSkin();
    CreateMenus();
    createToolbar();
    createLights();
    createLogo();
    createSelect();
    // Init the Viewports items
    //use the class
    viewp.init(Device,viewrightspace);
    viewp.createSkyBox();
    viewp.createBillboard();
    viewp.createGround();
    viewp.createGrid();
    viewp.createCameras();
    viewp.grid->setVisible(true);

    // set window caption
    Caption += " - [";
    Caption += driver->getName();
    Caption += "]";
    Device->setWindowCaption(Caption.c_str());
    // show about message box and load default model
    //showAboutText();
    loadModel(StartUpModelFile.c_str());
    // draw everything
    // Default fog value for certain scenes
    driver->setFog(SColor(0,128,128,128),EFT_FOG_LINEAR, true, 500.0f, 2500.0f, 0.01f, true );
    //Device->closeDevice(); // close the loader
    // Clear all and reset the scene
    selList->clear();
    selNode=0;
    lastNode=0;
    fakecounter=0;
    SceneAnalys(Device->getSceneManager()->getRootSceneNode());
    viewp.gridvisibility=true;
    viewp.groundvisibility=true;
    viewp.gridvispers=true;
    createIconBank();
    //viewp.loading=true;
    fileRequest.init(Device);
    //fileRequest.draw();
    appPath = Device->getFileSystem()->getWorkingDirectory();
    //appPath+="\\ ";
    Device->getSceneManager()->getRootSceneNode()->setName("root"); // Name the root node (Matching IRREdit)
#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(main_loop,0,1);
#else
//    while (1)
//    {
//    Render();
//    }
    while (Device->run() && driver)
    {
        if (Device->isWindowActive())
        {
            if (refresh)
            {
                setCursor();
                if (viewp.currentview==1) viewp.drawViewPorts(viewp.viewscale2,viewp.viewscale3,viewp.viewscale4,0);
                else viewp.drawSingle(viewp.currentview);
            }
    Device->sleep(30,0);
            if (refreshanimwin) refreshAnimWin();
            //printf ("Currentview: %i\n",currentview);
            core::stringw result="";
            core::stringw str(L"FPS: ");
            str.append(core::stringw(driver->getFPS()));
            if (Model && (Model->getType() ==   ESNT_OCTREE))
            {
                io::IAttributes* attribs = Device->getFileSystem()->createEmptyAttributes();
                if (attribs)
                {
                    // get the mesh name out
                    Model->serializeAttributes(attribs);
                    core::stringc name = attribs->getAttributeAsString("Mesh");
                    attribs->drop();
                    // get the animated mesh for the object
                    scene::IAnimatedMesh* mesh = Device->getSceneManager()->getMesh(name.c_str());
                    ModelMs=mesh;
                }
                if (ModelMs)
                {
                    str += L" | Tris: ";
                    if (ModelPoly(ModelMs)>0) str.append(core::stringw(ModelPoly(ModelMs)));
                    else  str+="Unknown |";
                    if (ModelMs) str.append(checkModel(ModelMs));
                    str+="| OCTTREE";
                }

            }
            if (Model)
            {
                if ((Model->getType() ==   ESNT_ANIMATED_MESH))
                {
                    str += L" | Tris: ";
                    if (ModelPoly(ModelMs)>0)
                    {
                        str.append(core::stringw(ModelPoly(ModelMs)));
                        str+=" |";
                    }
                    else  str+="Unknown |";
                    if (ModelMs) str.append(checkModel(ModelMs));
                    int currentFRM = (int)Model->getFrameNr();
                    if (ModelMs->getFrameCount()>0)
                    {
                        str+=L"| Frame:";
                        str.append((core::stringw)currentFRM);
                        str+="/";
                        str.append((core::stringw) (int) (ModelMs->getFrameCount()-1));
                    }
                    //if (ModelMesh[selNode]) str.append((core::stringw)(int)(ModelMesh[selNode]->getFrameCount()-1));
                }
            }
            fpstext->setText(str.c_str());
            //fpstext->setText(s.c_str());
        }
        else
            Device->sleep(500,0);
    }
#endif
    Device->drop();
    return 0;
}
