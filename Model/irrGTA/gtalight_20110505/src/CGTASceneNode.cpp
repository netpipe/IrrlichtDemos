// Copyright (C) 2007-2011 David Kment
// For license see license.txt

#include "CGTASceneNode.h"
#include "main.h"

#include "ISceneManager.h"
#include "IVideoDriver.h"
#include "IFileSystem.h"
#include "IReadFile.h"
#include "S3DVertex.h"

#include <sstream>

double myatof(char *input)
{
	stringstream ss(input);
	double d = 0;
	ss >> d;

	if (ss.fail())
	{
		return 0.0f;
	}

	return (d);
}

CGTASceneNode::CGTASceneNode(IrrlichtDevice *device, int gameId, ISceneNode* parent, ISceneManager* mgr,
		s32 id, const core::vector3df& position,
		const core::vector3df& rotation, const core::vector3df& scale)
	: ISceneNode(parent, mgr, id, position, rotation, scale)
{
    GameId = gameId;

    Device = device;

	Logger = Device->getLogger();

    IdeMode = 0;
    IplMode = 0;

    int maxObjectIds;

    if(GameId == GAMEID_GTA3) {maxObjectIds = MAXOBJECTIDS_GTA3;}
    if(GameId == GAMEID_GTAVC) {maxObjectIds = MAXOBJECTIDS_GTAVC;}
    if(GameId == GAMEID_GTASA) {maxObjectIds = MAXOBJECTIDS_GTASA;}

    Device->getSceneManager()->addEmptySceneNode();

    int i;
    ObjectDefinition *temp;
    for(i=0;i<maxObjectIds;i++)
    {
        temp = new ObjectDefinition(Device->getSceneManager());
        Definitions.push_back(*temp);
    }
}

CGTASceneNode::~CGTASceneNode()
{
	// if (Logger) {Logger->drop();}
}

void CGTASceneNode::render()
{
}

const core::aabbox3d<f32>& CGTASceneNode::getBoundingBox() const
{
    return Box;
}

void CGTASceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
	{
		SceneManager->registerNodeForRendering(this);
		ISceneNode::OnRegisterSceneNode();
	}
}

void CGTASceneNode::initialise()
{
    // every GTA from series 3 has a default.dat
    loadDAT("data/default.dat");

    stringc startFile = "";

    if(GameId == GAMEID_GTA3) {startFile = "data/gta3.dat";}
    if(GameId == GAMEID_GTAVC) {startFile = "data/gta_vc.dat";}
    if(GameId == GAMEID_GTASA) {startFile = "data/gta.dat";}

    // fixme: this is a pretty fragile way of getting the GTA root node
    ISceneNode *root = *SceneManager->getRootSceneNode()->getChildren().begin();

    root->setRotation(core::vector3df(-90.0f, 0.0f, 0.0f));

    loadDAT(startFile);

    // find LodObject (if available) for all instances

	for(unsigned i=0; i < Instances.size(); i++)
	{
		ObjectInstance *instance = &Instances[i];

		stringc lodName = instance->getName();

		lodName[0] = 'l';
		lodName[1] = 'o';
		lodName[2] = 'd';

		for(unsigned j=0; j < Instances.size(); j++)
		{
			ObjectInstance *instance2 = &Instances[j];

			if(instance2->getID() != instance->getID() && instance2->getName().equals_ignore_case(lodName))
			{
				instance->setLodObject(*instance2);

				continue;
			}
		}
	}
/*
	for(vector<ObjectInstance>::iterator it = Instances.begin(); it != Instances.end(); ++it)
	{
		stringc lodName = it->getName();

		lodName[0] = 'l';
		lodName[1] = 'o';
		lodName[2] = 'd';

		for(vector<ObjectInstance>::iterator it2 = Instances.begin(); it2 != Instances.end(); ++it2)
		{
			if(it2->getName().equals_ignore_case(lodName))
			{
				ObjectInstance *instance = it2->getInstance();

				it->setLodObject(*instance);
			}
		}
	}
*/
}

std::vector<CGTASceneNode::ObjectInstance> *CGTASceneNode::getObjectInstances()
{
    return &Instances;
}

void CGTASceneNode::loadDAT(stringc inFileName) {processFile(inFileName, FORMAT_DAT);}

void CGTASceneNode::loadIDE(stringc inFileName) {processFile(inFileName, FORMAT_IDE);}

void CGTASceneNode::loadIPL(stringc inFileName)
{
	stringc temp = inFileName;
	temp.make_lower();

	// skip overview.ipl
	if(temp.find("overview.ipl") != -1)
	{
		return;
	}

	processFile(inFileName, FORMAT_IPL);
}

void CGTASceneNode::processFile(stringc inFileName, int mode)
{
    inFileName.replace('\\', '/');

    Logger->log("Loading file", inFileName.c_str(), ELL_INFORMATION);

    io::IReadFile* file = Device->getFileSystem()->createAndOpenFile(inFileName.c_str());

    if(!file)
    {
        Logger->log("Cannot open", inFileName.c_str(), ELL_WARNING);
        return;
    }

    // here we read bytewise. this should be optimized.
    // if only irrlicht had a readLine function...

    stringc line = "";
    c8 tmp[2];
    tmp[1] = 0;
    int bytesRead = 0;

    do
    {
        bytesRead = file->read(tmp, 1);

        if(bytesRead > 0)
        {
            if(tmp[0] == '\r') {}
            else if(tmp[0] == '\n')
            {
                // line end
                if(!startsWith(&line, "#") && line.size() > 0)
                {
                    if(mode == FORMAT_DAT) {processDAT(line);}
                    if(mode == FORMAT_IPL) {processIPL(line);}
                    if(mode == FORMAT_IDE) {processIDE(line);}
                }

                line = "";
            }
            else
            {
                // append character to string
                line += tmp;
            }
        }
    } while(bytesRead > 0);

    file->drop();
}

void CGTASceneNode::processDAT(stringc inStr)
{
    if(startsWith(&inStr, "SPLASH ")) {/* SPLASH command is currently ignored */}
    else if(startsWith(&inStr, "COLFILE ")) {/* COLFILE command is currently ignored */}
    else if(startsWith(&inStr, "MAPZONE ")) {/* MAPZONE command is currently ignored */}
    else if(startsWith(&inStr, "TEXDICTION "))
    {
        Device->getFileSystem()->addFileArchive(inStr.subString(11, inStr.size() - 11), true, false); // " 11 = "TEXDICTION "
    }
    else if(startsWith(&inStr, "IDE "))
    {
        loadIDE(inStr.subString(4, inStr.size() - 4)); // " 4 = "IDE "
    }
    else if(startsWith(&inStr, "IPL "))
    {
        loadIPL(inStr.subString(4, inStr.size() - 4)); // " 4 = "IPL "
    }
    else if(startsWith(&inStr, "IMG "))
    {
		if(Device->getFileSystem()->addFileArchive(inStr.subString(4, inStr.size() - 4), true, false))
		{
			printf("error, cannot register archive models\\gta3.img\n");
		}
    }
    else
    {
        Logger->log("Unknown command", inStr.c_str(), ELL_WARNING);
    }
}

void CGTASceneNode::processIPL(stringc inStr)
{
    stringc iplTempName;
    int iplTempId = 0, iplTempInt = 0, iplTempLod = 0;
    double iplTempPosX = 0.0f, iplTempPosY = 0.0f, iplTempPosZ = 0.0f;
    double iplTempScaleX = 0.0f, iplTempScaleY = 0.0f, iplTempScaleZ = 0.0f;
    double iplTempRotX = 0.0f, iplTempRotY = 0.0f, iplTempRotZ = 0.0f, iplTempRotW = 0.0f;

    if(startsWith(&inStr, "inst")) {IplMode = IPL_MODE_INST;}
    else if(startsWith(&inStr, "cull")) {IplMode = IPL_MODE_CULL;}
    else if(startsWith(&inStr, "pick")) {IplMode = IPL_MODE_PICK;}
    else if(startsWith(&inStr, "end")) {IplMode = 0;}
    else
    {
        if(IplMode == IPL_MODE_INST)
        {
            int i = 0;
            char *tmp = (char *)inStr.c_str();
            char *tok = strtok(tmp, " ,");

            while(tok)
            {
                if(GameId == GAMEID_GTA3)
                {
                    if(i==0) {iplTempId = atoi(tok);}
                    if(i==1) {iplTempName = tok;}
                    if(i==2) {iplTempPosX = myatof(tok);}
                    if(i==3) {iplTempPosY = myatof(tok);}
                    if(i==4) {iplTempPosZ = myatof(tok);}
                    if(i==5) {iplTempScaleX = myatof(tok);}
                    if(i==6) {iplTempScaleY = myatof(tok);}
                    if(i==7) {iplTempScaleZ = myatof(tok);}
                    if(i==8) {iplTempRotX = myatof(tok);}
                    if(i==9) {iplTempRotY = myatof(tok);}
                    if(i==10) {iplTempRotZ = myatof(tok);}
                    if(i==11) {iplTempRotW = myatof(tok);}
                }
                else if(GameId == GAMEID_GTAVC)
                {
                    if(i==0) {iplTempId = atoi(tok);}
                    if(i==1) {iplTempName = tok;}
                    if(i==2) {iplTempInt = atoi(tok);}
                    if(i==3) {iplTempPosX = myatof(tok);}
                    if(i==4) {iplTempPosY = myatof(tok);}
                    if(i==5) {iplTempPosZ = myatof(tok);}
                    if(i==6) {iplTempScaleX = myatof(tok);}
                    if(i==7) {iplTempScaleY = myatof(tok);}
                    if(i==8) {iplTempScaleZ = myatof(tok);}
                    if(i==9) {iplTempRotX = myatof(tok);}
                    if(i==10) {iplTempRotY = myatof(tok);}
                    if(i==11) {iplTempRotZ = myatof(tok);}
                    if(i==12) {iplTempRotW = myatof(tok);}
                }
                else if(GameId == GAMEID_GTASA)
                {
                    if(i==0) {iplTempId = atoi(tok);}
                    if(i==1) {iplTempName = tok;}
                    if(i==2) {iplTempInt = atoi(tok);}
                    if(i==3) {iplTempPosX = myatof(tok);}
                    if(i==4) {iplTempPosY = myatof(tok);}
                    if(i==5) {iplTempPosZ = myatof(tok);}
                    if(i==6) {iplTempRotX = myatof(tok);}
                    if(i==7) {iplTempRotY = myatof(tok);}
                    if(i==8) {iplTempRotZ = myatof(tok);}
                    if(i==9) {iplTempRotW = myatof(tok);}
                    if(i==10) {iplTempLod = atoi(tok);}
                }

                i++;

                tok = strtok(NULL, " ,");
            }

			quaternion quat = quaternion(-iplTempRotX, iplTempRotY, iplTempRotZ, iplTempRotW);
            vector3d<f32> euler;
            quat.toEuler(euler);

			// convert radian to degrees
            vector3d<f32> objectRotation;
			objectRotation.X = radToDeg(euler.X);
			objectRotation.Y = radToDeg(euler.Y);
			objectRotation.Z = radToDeg(euler.Z);

			// printf("%s = %.10f, %.10f, %.10f\n", iplTempName.c_str(), objectRotation.X, objectRotation.Y, objectRotation.Z);

            ObjectInstance *instance = new ObjectInstance(Device->getSceneManager());

            instance->setName(iplTempName);
            instance->setTextureDir(Definitions[iplTempId].getTextureDir());
            instance->setDrawDistance(Definitions[iplTempId].getDrawDistance());
            instance->setID(iplTempId);

            instance->setPosition(core::vector3df(-iplTempPosX, iplTempPosY, iplTempPosZ));
            instance->setRotation(objectRotation);
            instance->setScale(core::vector3df(iplTempScaleX, iplTempScaleY, iplTempScaleZ));

			instance->init();

            Instances.push_back(*instance);
        }
    }
}

void CGTASceneNode::processIDE(stringc inStr)
{
    int ideTempId = 0;
    stringc ideTempName;
    stringc ideTempTextureDir;
    int ideTempObjectCount;
    float ideTempDrawDistance;

    if(startsWith(&inStr, "objs")) {IdeMode = IDE_MODE_OBJS;}
    else if(startsWith(&inStr, "tobj")) {IdeMode = IDE_MODE_TOBJ;}
    else if(startsWith(&inStr, "path")) {IdeMode = IDE_MODE_PATH;}
    else if(startsWith(&inStr, "2dfx")) {IdeMode = IDE_MODE_2DFX;}
    else if(startsWith(&inStr, "end")) {IdeMode = 0;}
    else
    {
        if(IdeMode == IDE_MODE_OBJS)
        {
            int i = 0;
            char *tmp = (char *)inStr.c_str();
            char *tok = strtok(tmp, " ,");

            while(tok)
            {
                if(i==0) {ideTempId = atoi(tok);}
                if(i==1) {ideTempName = tok;}
                if(i==2) {ideTempTextureDir = tok;}
                if(i==3) {ideTempObjectCount = atoi(tok);}
                if(i==4) {ideTempDrawDistance = myatof(tok);}

                i++;

                tok = strtok(NULL, " ,");
            }

            Definitions[ideTempId].setName(ideTempName);
            Definitions[ideTempId].setTextureDir(ideTempTextureDir);
            Definitions[ideTempId].setDrawDistance(ideTempDrawDistance);
        }
    }
}

CGTASceneNode::ObjectInstance::ObjectInstance(ISceneManager *sceneManager)
{
    SceneManager = sceneManager;

    Id = -1;

	Render = true;
    IsLODMesh = false;

    Mesh = NULL;
    Node = NULL;
    LodObject = NULL;

    Name = "";

    Position = core::vector3df(0.0f, 0.0f, 0.0f);
    Rotation = core::vector3df(0.0f, 0.0f, 0.0f);
    Scale = core::vector3df(0.0f, 0.0f, 0.0f);
}

CGTASceneNode::ObjectInstance::~ObjectInstance()
{
}

void CGTASceneNode::ObjectInstance::init()
{
	// fixme: this is a pretty fragile way of getting the GTA root node
	ISceneNode *root = *SceneManager->getRootSceneNode()->getChildren().begin();

	ISceneNode *tempNode = SceneManager->addEmptySceneNode(root);

	tempNode->setPosition(Position);
	tempNode->updateAbsolutePosition();

	AbsolutePosition = tempNode->getAbsolutePosition();

	tempNode->remove();
	tempNode = NULL;

    // fixme: this LOD detection works for GTA3/VC only

    stringc temp = Name.subString(0, 3);

    if(temp.equals_ignore_case("lod"))
    {
        IsLODMesh = true;
		// Render = false;
    }

	if(!DYNAMIC_LOD_MODE)
	{
		update();
	}
}

void CGTASceneNode::ObjectInstance::update()
{
    if(Render)
    {
        if(Node == NULL)
        {
            if(Mesh == NULL)
            {
                loadFromDFFFile();
            }

            if(Mesh)
            {
                // fixme: this is a pretty fragile way of getting the GTA root node
                ISceneNode *root = *SceneManager->getRootSceneNode()->getChildren().begin();

                Node = SceneManager->addMeshSceneNode(Mesh, root);

                Node->setPosition(Position);
                Node->setRotation(Rotation);
                Node->setScale(Scale);

				Node->updateAbsolutePosition();
            }
        }
    }
    else
    {
        if(Node)
        {
            Node->remove();
            Node = NULL;
        }

        if(Mesh)
        {
            Mesh = NULL;
        }
    }
}

void CGTASceneNode::ObjectInstance::loadFromDFFFile()
{
    stringc completeFileName = "";
    completeFileName += SceneManager->getParameters()->getAttributeAsString("GTA_MODELS_PATH");
    completeFileName += Name;
    completeFileName += ".dff";

    // only load mesh if corresponding TextureDir was set
    if(TextureDir.size() > 0)
    {
        stringc texturePath = "";
        texturePath += SceneManager->getParameters()->getAttributeAsString("GTA_TEXTURES_PATH");
        texturePath += TextureDir;
        texturePath += "/";

        SceneManager->getParameters()->setAttribute("DFF_TEXTURE_PATH", texturePath.c_str());

        SceneManager->getFileSystem()->addFileArchive(TextureDir + ".txd", true, false);

        Mesh = SceneManager->getMesh(completeFileName.c_str());

        if(!Mesh)
        {
            // error loading mesh
        }
    }
}

int CGTASceneNode::ObjectInstance::getID()
{
    return Id;
}

CGTASceneNode::ObjectInstance * CGTASceneNode::ObjectInstance::getInstance()
{
    return this;
}

float CGTASceneNode::ObjectInstance::getDrawDistance()
{
    return DrawDistance;
}

core::vector3df CGTASceneNode::ObjectInstance::getPosition()
{
    return Position;
}

core::vector3df CGTASceneNode::ObjectInstance::getAbsolutePosition()
{
	return AbsolutePosition;
}

stringc CGTASceneNode::ObjectInstance::getName()
{
    return Name;
}

bool CGTASceneNode::ObjectInstance::getRender()
{
    return Render;
}

bool CGTASceneNode::ObjectInstance::isLODMesh()
{
    return IsLODMesh;
}

void CGTASceneNode::ObjectInstance::setDrawDistance(float distance)
{
    DrawDistance = distance;
}

void CGTASceneNode::ObjectInstance::setName(stringc name)
{
    Name = name;
}

void CGTASceneNode::ObjectInstance::setTextureDir(stringc textureDir)
{
    TextureDir = textureDir;
}

void CGTASceneNode::ObjectInstance::setID(int id)
{
    Id = id;
}

void CGTASceneNode::ObjectInstance::setPosition(core::vector3df position)
{
    Position = position;
}

void CGTASceneNode::ObjectInstance::setRotation(core::vector3df rotation)
{
    Rotation = rotation;
}

void CGTASceneNode::ObjectInstance::setScale(core::vector3df scale)
{
    Scale = scale;
}

void CGTASceneNode::ObjectInstance::setLodObject(ObjectInstance &object)
{
    LodObject = &object;
}

void CGTASceneNode::ObjectInstance::setRender(bool render)
{
	if(Render == render) {return;}

    Render = render;

	if(LodObject != NULL)
	{
		LodObject->setRender(!render);
	}

    update();
}

CGTASceneNode::ObjectDefinition::ObjectDefinition(ISceneManager *sceneManager)
{
    SceneManager = sceneManager;

    Name = "";
    TextureDir = "";
}

CGTASceneNode::ObjectDefinition::~ObjectDefinition()
{

}

float CGTASceneNode::ObjectDefinition::getDrawDistance()
{
    return DrawDistance;
}

stringc CGTASceneNode::ObjectDefinition::getTextureDir()
{
    return TextureDir;
}

void CGTASceneNode::ObjectDefinition::setDrawDistance(float distance)
{
    DrawDistance = distance;
}

void CGTASceneNode::ObjectDefinition::setName(stringc name)
{
    Name = name;
}

void CGTASceneNode::ObjectDefinition::setTextureDir(stringc textureDir)
{
    TextureDir = textureDir;
}

bool startsWith(stringc *haystack, const char *needle)
{
    return haystack->trim().equalsn(needle, strlen(needle));
}
