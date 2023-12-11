#include "castle.h"
#include <irrlicht.h>
#include <iostream>
#include <vector>
#include <cmath>
#include "Logger.h"
#include "bullethelper.h"
using namespace irr;
using namespace core;
using namespace std;
Castle::Castle(scene::ISceneManager* smgr, Physics* physics, IrrlichtDevice* device, video::IVideoDriver* driver, core::vector3df center){
    this->smgr = smgr;
    this->nodes.clear();
    this->driver = driver;
    this->device = device;
    this->physics = physics;
    this->position = center;
    this->COLOR_DARK = randomColor();
    this->COLOR_LIGHT = randomColor();
    this->position = center;
    if(this->buildCastle(this->position)){
        this->addToPhysicsWorld();
        this->setTreasure();
    }
}

bool Castle::buildCastle(core::vector3df center){

        irr::io::IXMLReader* xml = this->device->getFileSystem()->createXMLReader("media/castle/castle.xml");  //create xml reader
        if (!xml)
            return false;

        const stringw sideTag(L"side"); //we'll be looking for this tag in the xml
        core::stringw currentSection; //keep track of our current section
        const stringw blockTag(L"block"); //constant for blocktag
        core::stringc position, lastposition;
        float width, height, distance = 0.f;
        // core::vector3d<f32> * edges = new core::vector3d<f32>[8];
        f32 offsets[4]={0,0,0,0};
        u32 reading = -1 ;
        core::vector3df pos;
        core::aabbox3d<f32> box;
        core::matrix4 transformer;
        std::multimap<char, scene::IMeshSceneNode*>::iterator first, last;
        //while there is more to read
        log1("Reading XML castle file and building castle");
        while (xml->read())
        {
            //check the node type
            switch (xml->getNodeType()){
                //we found a new element
                case irr::io::EXN_ELEMENT:{

                    //we currently are in the empty or mygame section and find the side tag so we set our current block
                    if (currentSection.empty() && sideTag.equals_ignore_case(xml->getNodeName()))
                    {
                        currentSection = sideTag;
                        position = xml->getAttributeValueSafe(L"position");
                        distance = xml->getAttributeValueAsFloat(L"distance");
                        reading++;
                        // maybe a switch case is better

                        if(!this->nodes.empty() && position[0]){ // assert

                        if(reading == 1) offsets[reading] = BLOCKS_OFFSET+getMeshSize(first->second)[0]/2;
                        if(reading == 2 && last != this->nodes.end()) offsets[reading] = BLOCKS_OFFSET+getMeshSize(last->second)[0]/2;
                        if(reading == 3) offsets[reading] = BLOCKS_OFFSET+getMeshSize(first->second)[0]/2;
                        }

                    } else if (currentSection.equals_ignore_case(sideTag) && blockTag.equals_ignore_case(xml->getNodeName())){
                        //read in the key
                         core::vector3df scale = core::vector3df(1.f,1.f,1.f);
                        width = xml->getAttributeValueAsFloat(L"width");
                        height = xml->getAttributeValueAsFloat(L"height");
                        scale = core::vector3df(width,height,1);

                        switch(position[0]){
                            case 'f':
                                    pos = core::vector3df(
                                    center.X + offsets[0] +scale.X/2, scale.Y/2 , center.Z );
                                    this->createBlock(pos,scale, core::vector3df(0,0,0), 'f', this->COLOR_LIGHT);
                                    offsets[0] += (width+BLOCKS_OFFSET);
                                    break;
                            case 'l':
                                    pos = core::vector3df(
                                    center.X+scale.X/2, scale.Y/2 , scale.X/2+center.Z+offsets[1]);
                                    this->createBlock(pos,scale,core::vector3df(0,90,0), 'l', this->COLOR_DARK);
                                    offsets[1] += (width+BLOCKS_OFFSET);
                            break;
                            case 'b':
                                    pos = core::vector3df(
                                    center.X+scale.X/2+offsets[2], scale.Y/2,
                                    center.Z+distance+getMeshSize(this->node)[0]+BLOCKS_OFFSET*5
                                    );
                                    this->createBlock(pos,scale,core::vector3df(0,0,0), 'b', this->COLOR_LIGHT);
                                    offsets[2] += (width+BLOCKS_OFFSET);



                            break;
                            case 'r':
                                    pos = core::vector3df(
                                    center.X+distance-getMeshSize(this->node)[2], scale.Y/2 , scale.X/2+center.Z+offsets[3]);
                                    this->createBlock(pos,scale,core::vector3df(0,90,0), 'r', this->COLOR_DARK);
                                    offsets[3] += (width+BLOCKS_OFFSET);


                            break;
                            default:break;
                        }
                        if(!this->nodes.empty()){
                        first = this->nodes.begin();
                        last = this->nodes.end();
                        }

                    }

                }
                break;

                // found the end of an element(side)
                case irr::io::EXN_ELEMENT_END:
                    if(currentSection.equals_ignore_case(xml->getNodeName())) {position = ""; currentSection = ""; };
                break;

                default: break;
            }
        }

        // delete the xml reader
        xml->drop();
        return true;

}
//useless?
void Castle::addToPhysicsWorld(){


}
void Castle::createBlock(core::vector3df position, core::vector3df scale, core::vector3df rotation,  char side, video::SColor color){
      this->node = this->smgr->addCubeSceneNode(1,0,-1);
      this->node->setPosition(position);
      this->node->setScale(scale);
      this->node->setMaterialFlag(video::EMF_LIGHTING,false);
      this->smgr->getMeshManipulator()->setVertexColors(this->node->getMesh(),color);
      logVector(2,"Position castle block at", position);
        #if (DEBUG_OUTPUT_MASK & 2)
          this->node->setDebugDataVisible((scene::E_DEBUG_SCENE_TYPE)scene::EDS_BBOX_BUFFERS);
        #endif
      assert(this->node != 0);
      if(this->node){
        this->nodes.insert(std::make_pair(side, this->node));
        this->physics->createCastleBlock(this->node,rotation,scale,position);
      }
}
core::vector3df Castle::getSideSize(char side){
    core::vector3df size_side = core::vector3df(0,0,0) ;
    std::pair<
        std::multimap<char, scene::IMeshSceneNode*>::iterator,
        std::multimap<char, scene::IMeshSceneNode*>::iterator
        > blocks;
    blocks = this->nodes.equal_range(side);
    for(std::multimap<char, scene::IMeshSceneNode*>::iterator it = blocks.first; it != blocks.second; ++it){
        size_side.X+= it->second->getScale().X;
        size_side.Y+= it->second->getScale().Y;
        size_side.Z+= it->second->getScale().Z;
    }
    return size_side;
}
void Castle::setTreasure(){
//    core::vector3df position = this->calculateAbsoluteCenter();
    f32 x = this->getSideSize('f').X * getRand(100)/150;
    f32 z = this->getSideSize('l').X * getRand(100)/100;
    core::vector3df position = core::vector3df(0.5f+this->position.X + x,0,this->position.Z + z);
    this->target = new Target(position,this->smgr,this->driver,this->physics);
    this->initialTargetPos = position;
    log1("Treasure positioned");
}
core::vector3df Castle::calculateAbsoluteCenter(){
    core::vector3df relativeCenter = this->position;
    relativeCenter.X+= this->getSideSize('f').X * 0.5f;
    relativeCenter.Z+= this->getSideSize('l').X * 0.5f;
    return relativeCenter;
}
bool Castle::checkTarget(){
    core::vector3df diff = (this->target->getIrrNode()->getAbsolutePosition() - this->initialTargetPos);
    diff = core::vector3df(abs(diff.X),abs(diff.Y),abs(diff.Z));
    return (diff.X >= TARGET_OFFSET || diff.Z >= TARGET_OFFSET);
}
