#include <irrlicht.h>
//#include <IReferenceCounted.h>  //?? no need for this. irrlicht.h already includes it

using namespace irr;

class CHUD : public virtual IReferenceCounted
{
public:
    someClass(io::IFileSystem* fs);
    virtual ~someClass();
private:
    io::IFileSystem* fs;
};

CHUD::CHUD(io::IFileSystem* fileSystem)
: fs(fileSystem)
{
    // We add a ref count to the fileSystem
    if (fileSystem)
    	fileSystem->grab();
}

CHUD::~CHUD()
{
    // drop the filesystem reference
    if (fs)
        fs->drop();
}

void CHUD::loadHud(const c8* filename)
{
    // create a IReadFile from the filename supplied, then go to readHud();


}

void CHUD::loadHud(io::IReadFile* file)
{
    // check the file is valid, then go to readHud();


}


void CHUD::readHud(io::IXMLReader* reader)
{
    // Load XML elements here
        io::IFileSystem* FileSystem;
    // Get the file system to open the file for reading first
    io::IReadFile* temp = FileSystem->createAndOpenFile(filename);
    // Create the xml reader
    io::IXMLReader* xml = FileSystem->createXMLReader(temp);

	CHUD* HUD = new CHUD();

	while(xml && xml->read())
	{
		switch(xml->getNodeType())
		{
			case io::EXN_ELEMENT:
			{
				if (core::stringw("element") == xml->getNodeName())
				{
				
                HUD->object = xml->getAttributeValue(L"picture")
                HUD->object = xml->getAttributeValue(L"text")
				}
			}
		}
	}
	//HUDelements.push_back(HUD);
	xml->drop();

}

