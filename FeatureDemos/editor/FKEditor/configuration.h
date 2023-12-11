void loadXMLConfig()
{

   /*
      The next step is to read the configuration file. It is stored in the xml
      format and looks a little bit like this:

      <?xml version="1.0"?>
      <config>
         <startUpModel file="some filename" />
         <messageText caption="Irrlicht Engine Mesh Viewer">
            Hello!
         </messageText>
      </config>

      We need the data stored in there to be written into the global variables
      StartUpModelFile, MessageText and Caption. This is now done using the
      Irrlicht Engine integrated XML parser:
   */
   // read configuration from xml file
   printf("Entering the loadind of configuration\n");
   io::IXMLReader* xml = Device->getFileSystem()->createXMLReader(
      "media/config.xml");
   core::stringc DisplayMode=L"Not found";
   core::stringc full="";
   core::stringc dev="";
   if (!xml) printf("File not found!\n");
   while(xml && xml->read())
   {
      switch(xml->getNodeType())
      {
      case io::EXN_TEXT:
         // in this xml file, the only text which occurs is the messageText
         MessageText = xml->getNodeData();
         break;
      case io::EXN_ELEMENT:
         {
            if (core::stringw("startUpModel") == xml->getNodeName())
               {StartUpModelFile = xml->getAttributeValue(L"file");
               printf("Found the model infos\n");}
            else
            if (core::stringw("messageText") == xml->getNodeName())
               {Caption = xml->getAttributeValue(L"caption");
               printf ("Found the message text\n");}
            else
            if (core::stringw("chosenDisplay") == xml->getNodeName())
               {DisplayMode = xml->getAttributeValue(L"mode");
                printf("Here is the displaymode %s\n",DisplayMode.c_str());
                if (DisplayMode=="DirectX") driverType=video::EDT_DIRECT3D9;
                if (DisplayMode=="OpenGl") driverType=video::EDT_OPENGL;
                if (DisplayMode=="Software") driverType=video::EDT_BURNINGSVIDEO;
                }
            else
            if (core::stringw("fullscreen") == xml->getNodeName())
               {full = xml->getAttributeValue(L"mode");
                screensize.Height = xml->getAttributeValueAsInt(L"x");
                screensize.Width  = xml->getAttributeValueAsInt(L"y");
                dev=xml->getAttributeValue(L"use");
                video::IVideoModeList* Liste = Device->getVideoModeList();
                core::dimension2du Res = Liste->getDesktopResolution();
                if (dev=="desktop") {screensize=Res;}
                printf("Here is the fullscreen value %s\n",full.c_str());
                if (full=="false") fullscreen=false; else if (full=="true") fullscreen=true;
               }
         }
         break;
      }
   }

   if (xml)
      xml->drop(); // don't forget to delete the xml reader
}
