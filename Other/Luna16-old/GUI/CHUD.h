#include <irrlicht.h>
#include "Font/CGUITTFont.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class CHUD
{
public:
    CHUD();
    ~CHUD();
    io::IFileSystem* FileSystem;
    void LoadHUD(char* filename);
    void DisplayHUD();
    void RegisterDevice(IrrlichtDevice *device1);
    IrrlichtDevice *device;
    irr::gui::IGUIEnvironment *gui;
    gui::IGUIFont* font;
    IVideoDriver* driver;
    io::IAttributes * attrs;
    io::IXMLReader* xml;

    int pictureAttributes ;
    int textAttributes ;
    core::array<core::rect<s32> > textRects;
    core::array<core::stringw > texts;
    core::array<core::rect<s32> > imageRects;
    core::array<video::ITexture*> images;

    CGUITTFont *font2;
    CGUITTFace face;
};


CHUD::CHUD (){};

CHUD::~CHUD (){

};


void CHUD::LoadHUD(char* filename)
{
    font = device->getGUIEnvironment()->getBuiltInFont();

    face.load("./data/fonts/coolvetica.ttf");

    io::IReadFile* temp = FileSystem->createAndOpenFile(filename);
    xml = FileSystem->createXMLReader(temp);

	while(xml && xml->read())
	{
		switch(xml->getNodeType())
		{
			case io::EXN_ELEMENT:
			{
				attrs = FileSystem->createEmptyAttributes();
				attrs->read(xml);
				pictureAttributes = attrs->getAttributeAsInt(0);
				textAttributes = attrs->getAttributeAsInt(1);
			}
		}
	}
    xml->drop();    // Unload XML

    for (int i=0;  i < pictureAttributes ; i++)
    {
        stringc NodeName = "picture"; NodeName += i;  // add iteration to picture.
        stringc NodeItemAmmount = "";                 // initialise
        NodeItemAmmount += attrs->getAttributeAsInt(NodeName.c_str()); //picture element count.

        int attPosition = attrs->findAttribute(NodeName.c_str());     //start from this number

        stringc pictureFile = attrs->getAttributeAsString((attPosition + 1));

        imageRects.push_back(core::rect<s32>(
                    attrs->getAttributeAsInt((attPosition + 2)),
                    attrs->getAttributeAsInt((attPosition + 3)),
                    attrs->getAttributeAsInt((attPosition + 4)),
                    attrs->getAttributeAsInt((attPosition + 5))  ) );

        images.push_back(driver->getTexture(pictureFile.c_str()));

        IGUIImage* a = gui->addImage(imageRects[i]);
              a->setImage(images[i]);
              a->setScaleImage(true);
            a->setColor(video::SColor(100,23,23,23));
    }

    font2 = new CGUITTFont(gui);
    font2->attach(&face,24); // scale this number with the screen
    font2->AntiAlias=1;

	//skin = env->getSkin();
	//skin->setFont(font2);

    for (int i=0;  i < textAttributes ; i++)
    {
        stringc NodeName = "text"; NodeName += i;

        //2 test lines for figuring out how many attributes belong to this
        stringc NodeItemAmmount = "";
        //picture element count.
        NodeItemAmmount += attrs->getAttributeAsInt(NodeName.c_str());


        int attPosition = attrs->findAttribute(NodeName.c_str());
        texts.push_back(attrs->getAttributeAsString((attPosition + 1)));

        textRects.push_back(core::rect<s32>(
                    attrs->getAttributeAsInt((attPosition + 2)),
                    attrs->getAttributeAsInt((attPosition + 3)),
                    attrs->getAttributeAsInt((attPosition + 4)),
                    attrs->getAttributeAsInt((attPosition + 5))  ) );

    }
  //      driver->draw2DImage(images[i], stuff[i]);
}

void CHUD::DisplayHUD()
{
    for (int i=0;  i < textAttributes ; i++)
    {
        font2->draw(texts[i].c_str(),textRects[i],SColor(255,255,64,64),true);


                //!old font code from irrlicht
				//font->draw(texts[i].c_str(),
				//	textRects[i], video::SColor(255,255,255,255));
    }

}

void CHUD::RegisterDevice(IrrlichtDevice *device1)
{
    device = device1;
    driver = device->getVideoDriver();
    FileSystem = device->getFileSystem();
    gui = device->getGUIEnvironment();
}
