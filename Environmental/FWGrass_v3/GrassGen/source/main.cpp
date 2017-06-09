#include <iostream>

#include <irrlicht.h>
#include "CGrassGenerator.h"

using namespace irr;
using namespace GrassGenerator;

//the modified main() function which allows to input parameters like the path to the input XML
int main(int argc,char** argv)
{
    //Latest Change, the null device provides all but rendering functionality
    //making the program smaller and reducing its memory footprint
    IrrlichtDevice* device = createDevice(video::EDT_NULL,core::dimension2du(800,600));

    //Here we create a DYNAMICALLY allocated instance of the grass generator
    CGrassGenerator* grassGenInstance = new CGrassGenerator(device);

    //Here the program checks the input arguments from the command line, if they are not valid
    //it will use the default path to input file "./input.xml"
    //The validation rules are: presence check, length check, file existence check
    core::stringc inputXMLPath = "./input.xml";
    core::stringc outputFilePath = "./output.grs";

    if (argc>1) {//If there are any arguments
        core::stringc temp = core::stringc(argv[1]);
        if (temp.size()>4) { //has a file extension
            if (device->getFileSystem()->existFile(temp)) {
                inputXMLPath=temp;
            }
        }

    } else {
        std::cout << "Please specify relative path to the input file as an argument." << std::endl;
    }
    //Here we load the parameters
    grassGenInstance->load(inputXMLPath);
    grassGenInstance->process();
    std::cout << "PROCESSED" << std::endl;

    grassGenInstance->writeOut(outputFilePath);
    std::cout << "DONE" << std::endl;

    //must dispose of the dynamic instance or memory leak
    delete grassGenInstance;

    device->closeDevice();
    device->drop();

    std::cout << "Still Alive" << std::endl;

    return 0;
}
