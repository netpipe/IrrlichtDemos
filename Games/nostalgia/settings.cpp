/*				log_str2.append(L"move  ",7);
            	       log_str2.append(core::stringw(movement.Z),5);
				log_str2.append(L"lapse  ",15);
				log_str2.append(core::stringw(lapse),5);
				log_str2.append(L"yet  ",17);
				log_str2.append(core::stringw(yet),15);
				log_str2.append(L"now  ",17);
				log_str2.append(core::stringw(now),15);
				log_str2.append(L"last  ",17);
				log_str2.append(core::stringw(last_time),15);
                logger->log(log_str2.c_str());
                log_str2 =("   ");
log_str2.append(L"rotation X  ",17);
				log_str2.append(core::stringw(rotate.X),15);
				log_str2.append(L"    offset Y  ",17);
				log_str2.append(core::stringw(offset.Y),15);
                logger->log(log_str2.c_str());
                log_str2 =("   ");*/
///loads and save video settings
///loads and save also sound parameters and transparency
#include "settings.h"
#include <irrXML.h>

using namespace irr;
using namespace core;
using namespace gui;
using namespace io;
SIrrlichtCreationParameters paramIrr;
gui::IGUIComboBox* cmbScreenResolution;
IGUIWindow* guiWindow;
extern irr::ILogger* logger;
extern core::stringw log_str2;
extern bool audio_reverse_normal;
extern u32 master_volume;
extern u32 music_volume ;
extern u32 transparency;
extern IrrlichtDevice *Device;

///loads video settings at start, before the device is running
void load_settings()
{
     u32 screenWidth =1024, screenHeight = 768;
     io::IrrXMLReader* xml = io::createIrrXMLReader("xml_files//settings.xml");
while(xml && xml->read())
   {
     switch(xml->getNodeType())
      {
     case EXN_ELEMENT:
         {
            if (!strcmp("settings", xml->getNodeName()))
                {
                screenWidth          = xml->getAttributeValueAsInt("width");
                screenHeight         = xml->getAttributeValueAsInt("height");
                paramIrr.Bits = xml->getAttributeValueAsInt("bits");
                paramIrr.Fullscreen    = xml->getAttributeValueAsInt("fullscreen");
               }
               else if (!strcmp("options", xml->getNodeName()))
               {paramIrr.Vsync          = xml->getAttributeValueAsInt("vsync");
               paramIrr.AntiAlias = xml->getAttributeValueAsInt("antialias");
               paramIrr.Stencilbuffer = xml->getAttributeValueAsInt("stencilbuffer");
               }
         }
      }//switch
   }//while
   delete xml;
//transfer parameters
   paramIrr.WindowSize = dimension2d<s32>(screenWidth, screenHeight);
   paramIrr.DriverType = video::EDT_OPENGL;
return;
}
//===================================================================================
//===================================================================================
///interface to change settings
void change_settings()
{
///display current settings
core::stringw current_settings;
current_settings.append(L"surrent settings =>");
current_settings.append(L"  Width: ");
current_settings.append(core::stringw(paramIrr.WindowSize.Width),5);
current_settings.append(L"  Height: ");
current_settings.append(core::stringw(paramIrr.WindowSize.Height),5);
current_settings.append(L"  Bits: ");
current_settings.append(core::stringw(paramIrr.Bits),5);

    video::IVideoModeList *VideoModeList;
/// create handle to 2D user interface
    IGUIEnvironment* GUIenv = Device->getGUIEnvironment();

/// create an empty gui window
    guiWindow = GUIenv->addWindow(
        core::rect<s32>(200,100,650,420),
        true,
        L"CHANGE GAME VIDEO SETTINGS",
        0,
        200);

/// add fullscreen check box
    IGUICheckBox* cbFullScreen = GUIenv->addCheckBox(
        false,
        core::rect<s32>(10,35,100,45),
        guiWindow,
        221,
        L"Fullscreen");
        if (paramIrr.Fullscreen) cbFullScreen->setChecked(true);

/// add stencil buffer check box
    IGUICheckBox* cbStencilBuffer = GUIenv->addCheckBox(
        false,
        core::rect<s32>(120,35,210,45),
        guiWindow,
        222,
        L"Stencilbuffer");
        if (paramIrr.Stencilbuffer) cbStencilBuffer->setChecked(true);

///warning about turning off stencilbuffer = loose shadows
     IGUIStaticText* lbStencilBuffer = GUIenv->addStaticText(
        L"Needed for shadows",
        core::rect<s32>(120,50,220,70),
        false,
        false,
        guiWindow,
        206,
        false);
///antialias checkbox
    IGUICheckBox* cbAntialias = GUIenv->addCheckBox(
        false,
        core::rect<s32>(240,35,330,45),
        guiWindow,
        223,
        L"AntiAlias");
        if (paramIrr.AntiAlias) cbAntialias->setChecked(true);

/// add vsync check box
   IGUICheckBox* cbVsync = GUIenv->addCheckBox(
        false,
        core::rect<s32>(360,35,450,45),
        guiWindow,
        224,
        L"Vsync");
        if (paramIrr.Vsync) cbVsync->setChecked(true);

/// add label for display driver combobox
    IGUIStaticText* lbDisplayDriver = GUIenv->addStaticText(
        L"Display Driver",
        core::rect<s32>(5,85,95,100),
        false,
        false,
        guiWindow,
       204,
        false);

/// add display driver combobox
    IGUIComboBox* cmbDisplayDriver = GUIenv->addComboBox(
        core::rect<s32>(100,85,300,100),
        guiWindow,
        205);
    IGUIStaticText* lbDriverSupported = GUIenv->addStaticText(
        L"Currently support only OpenGL",
        core::rect<s32>(100,110,300,125),
        false,
        false,
        guiWindow,
        206,
        false);

///currenltly support only opengl, keep the others for future
    // populate display driver combobox
    //cmbDisplayDriver->addItem(L"Direct3D 9 Renderer");
    //cmbDisplayDriver->addItem(L"Direct3D 8 Renderer");
    cmbDisplayDriver->addItem(L"OpenGL Renderer");
    //cmbDisplayDriver->addItem(L"Software Renderer");
    //cmbDisplayDriver->addItem(L"Apfelbaum Software Renderer");
    //cmbDisplayDriver->addItem(L"Null Device");

    // add label for screen resolution combobox
    IGUIStaticText* lbScreenResolution = GUIenv->addStaticText(
        L"Screen Resolution",
        core::rect<s32>(5,150,95,175),
        false,
        false,
        guiWindow,
        208,
        false);

    IGUIStaticText* lbCurrentScreenSettings = GUIenv->addStaticText(
        current_settings.c_str(),
        core::rect<s32>(5,190,300,215),
        false,
        false,
        guiWindow,
        209,
        false);

/// add screen resolution combobox
    cmbScreenResolution = GUIenv->addComboBox(
        core::rect<s32>(100,150,300,175),
        guiWindow,
        207);

/// query the Device to obtain the available video mode
    VideoModeList = Device->getVideoModeList();

/// define array to hold a formatted listing of the availabe modes
    wchar_t formated_listing[64] = L" ";

/// go through all available video mode for graphic adapter
    for(int i = 0; i < VideoModeList->getVideoModeCount(); ++i)
    {
        // format available modes into desired listing
        swprintf(
            formated_listing,
            64,
            L"%i x %i pixels @ %i-bit colors",
            VideoModeList->getVideoModeResolution(i).Width,
            VideoModeList->getVideoModeResolution(i).Height,
            VideoModeList->getVideoModeDepth(i));

        // add formatted mode to combo list box
        cmbScreenResolution->addItem(formated_listing);
    }



    // add apply button
   IGUIButton* bApply = GUIenv->addButton(
        core::rect<s32>(105,230,165,245),
        guiWindow,
        211,
        L"Apply", L"Writes settings to config file");

    // add cancel button
  IGUIButton* bCancel = GUIenv->addButton(
        core::rect<s32>(175,230,235,245),
        guiWindow,
        212,
        L"Close",L"Close this window");

IGUIStaticText* lbwarning = GUIenv->addStaticText(
        L"Restart game after changing parameters",
        core::rect<s32>(70,250,290,270),
        false,
        false,
        guiWindow,
        209,
        false);

/// don't let user resize the window
   Device->setResizeAble(false);

/// display app title and version information
   Device->setWindowCaption(L"Change video mode settings");
/// exit
    return;
}

///prepare settings parameter for save
void modify_settings(s32 i)//i = index in videolist box
{   IGUIElement *root = Device->getGUIEnvironment()->getRootGUIElement();
    gui::IGUIElement *list = root->getElementFromId( 207, true);
    paramIrr.WindowSize.Width = Device->getVideoModeList()->getVideoModeResolution(i).Width;
    paramIrr.WindowSize.Height = Device->getVideoModeList()->getVideoModeResolution(i).Height;
    paramIrr.Bits = Device->getVideoModeList()->getVideoModeDepth(i);
    }

///writing out teh settings to xml file
void save_settings(SIrrlichtCreationParameters paramIrr)
{
wchar_t  bits_str[32], videoheigth[32], videowidth[32],fullscreen[32],
vsync[32],antialias[32],stencilbuffer[32];

//formats the strings for XMLwriter

swprintf(videowidth, 5, L"%d", paramIrr.WindowSize.Width);
swprintf(videoheigth, 5, L"%d", paramIrr.WindowSize.Height);
swprintf(bits_str, 4, L"%d", paramIrr.Bits);
swprintf(fullscreen, 2, L"%d", paramIrr.Fullscreen);
swprintf(vsync, 2, L"%d", paramIrr.Vsync);
swprintf(antialias, 2, L"%d", paramIrr.AntiAlias);
swprintf(stencilbuffer, 2, L"%d", paramIrr.Stencilbuffer);
// define xml writer
io::IXMLWriter * xml_write_file = Device->getFileSystem()->createXMLWriter("xml_files//settings.xml");

//must write header of file to xml file
     xml_write_file->writeXMLHeader();
     xml_write_file->writeComment(L"This is a config file for arcade nostalgia");
     xml_write_file->writeLineBreak();
     xml_write_file->writeElement(L"config");
     xml_write_file->writeLineBreak();
//writing maximum 5 elements
     xml_write_file->writeElement(L"settings",false
          ,L"width",videowidth
          ,L"height",videoheigth
          ,L"bits",bits_str
          ,L"fullscreen",fullscreen
          ,L"driver",L"EDT_OPENGL");
//must write closing tag
     xml_write_file->writeClosingTag(L"settings");
     xml_write_file->writeLineBreak();

     //writing optionnal parameters
     xml_write_file->writeElement(L"options",false
          ,L"vsync",vsync
          ,L"antialias",antialias
          ,L"stencilbuffer",stencilbuffer);
     xml_write_file->writeLineBreak();
     xml_write_file->writeClosingTag(L"options");
     xml_write_file->writeLineBreak();
//must write closing tag
     xml_write_file->writeClosingTag(L"config");
     // cleanup and write buffer to disk
     xml_write_file->drop();
}


///write audio and transparency parameters to xml file
void save_parameters()
{wchar_t  formated_audio_reverse_normal[16]=L"00000000000000"
, formated_music_volume[16]=L"0000000000000"
,formated_master_volume[16]=L"0000000000000"
,formated_transparency[16]=L"00000000000000";
swprintf(formated_transparency, 5, L"%d", transparency);
swprintf(formated_audio_reverse_normal, 2, L"%d", audio_reverse_normal);
swprintf(formated_music_volume, 5, L"%d", music_volume);
swprintf(formated_master_volume, 5, L"%d", master_volume);


// define xml writer
            io::IXMLWriter * xml_write_file = Device->getFileSystem()->createXMLWriter("xml_files//parameters.xml");

                 // writing settings to xml file
                 xml_write_file->writeXMLHeader();
                 xml_write_file->writeComment(L"This is a config file for arcade nostalgia");
                 xml_write_file->writeLineBreak();
                 xml_write_file->writeElement(L"config");
                 xml_write_file->writeLineBreak();
                 xml_write_file->writeElement(L"parameters",false
                 ,L"audio_reverse_normal",formated_audio_reverse_normal
                 ,L"music_volume",formated_music_volume
                 ,L"master_volume",formated_master_volume
                 ,L"transparency",formated_transparency);
                  xml_write_file->writeClosingTag(L"parameters");
                  xml_write_file->writeLineBreak();
                 xml_write_file->writeClosingTag(L"config");
                 // cleanup and write buffer to disk
                 xml_write_file->drop();

}


///load audio and transparency parameters to xml file
void load_parameters()
{

    io::IrrXMLReader* xml = io::createIrrXMLReader("xml_files//parameters.xml");
    while(xml && xml->read())
   {
      switch(xml->getNodeType())
      {

         case EXN_ELEMENT:
         {
            if (!strcmp("parameters", xml->getNodeName()))
                {
                audio_reverse_normal= xml->getAttributeValueAsInt("audio_reverse_normal");
                music_volume = xml->getAttributeValueAsInt("music_volume");
                master_volume = xml->getAttributeValueAsInt("master_volume");
                transparency = xml->getAttributeValueAsInt("transparency");
               }
         }

      }//switch

   }//while
   delete xml;
}
