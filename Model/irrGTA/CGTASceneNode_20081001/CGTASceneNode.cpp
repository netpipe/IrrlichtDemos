// Copyright (C) 2008 David Kment
// For license see CGTASceneNode.h

#include "CGTASceneNode.h"

#include "ISceneManager.h"
#include "IVideoDriver.h"
#include "IFileSystem.h"
#include "IReadFile.h"
#include "S3DVertex.h"

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
	if (Logger) {Logger->drop();}
}

void CGTASceneNode::render()
{
    for(unsigned int i=0;i < Instances.size();i++)
    {
        Instances[i].render();
    }
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
    stringc startFile = "";

    if(GameId == GAMEID_GTA3) {startFile = "data/gta3.dat";}
    if(GameId == GAMEID_GTAVC) {startFile = "data/gta_vc.dat";}
    if(GameId == GAMEID_GTASA) {startFile = "data/gta.dat";}

    loadDAT(startFile);

    // copy mesh pointer from object definition to object instance
    unsigned int i;
    for(i=0; i < Instances.size(); i++) {

        if(Instances[i].getID() != -1) {

            Instances[i].setMesh(Definitions[Instances[i].getID()].getMesh());
        }
    }
}

void CGTASceneNode::loadDAT(stringc inFileName) {processFile(inFileName, FORMAT_DAT);}

void CGTASceneNode::loadIDE(stringc inFileName) {processFile(inFileName, FORMAT_IDE);}

void CGTASceneNode::loadIPL(stringc inFileName) {processFile(inFileName, FORMAT_IPL);}

void CGTASceneNode::processFile(stringc inFileName, int mode)
{
    if(SceneManager->getParameters()->getAttributeAsBool("GTA_WINDOWS_PATH_FORMAT"))
    {
        inFileName.replace('/', '\\');
    }
    else
    {
        inFileName.replace('\\', '/');
    }

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
    else if(startsWith(&inStr, "IDE ")) {loadIDE(inStr.subString(4, inStr.size() - 4));} // " 4 = "IDE "
    else if(startsWith(&inStr, "IPL ")) {loadIPL(inStr.subString(4, inStr.size() - 4));} // " 4 = "IPL "
    else
    {
        Logger->log("Unknown command", inStr.c_str(), ELL_WARNING);
    }
}

void CGTASceneNode::processIPL(stringc inStr)
{
    stringc ipl_temp_name;
    int ipl_temp_id = 0;
    float ipl_temp_pos_x = 0.0f, ipl_temp_pos_y = 0.0f, ipl_temp_pos_z = 0.0f;
    float ipl_temp_scale_x = 0.0f, ipl_temp_scale_y = 0.0f, ipl_temp_scale_z = 0.0f;
    float ipl_temp_rot_x = 0.0f, ipl_temp_rot_y = 0.0f, ipl_temp_rot_z = 0.0f, ipl_temp_rot_w = 0.0f;

    float q2a_out_x = 0.0f, q2a_out_y = 0.0f, q2a_out_z = 0.0f;

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
                if(i==0) {ipl_temp_id = atoi(tok);}
                if(i==1) {ipl_temp_name = tok;}
                if(i==2) {ipl_temp_pos_x = atof(tok);}
                if(i==3) {ipl_temp_pos_y = atof(tok);}
                if(i==4) {ipl_temp_pos_z = atof(tok);}
                if(i==5) {ipl_temp_scale_x = atof(tok);}
                if(i==6) {ipl_temp_scale_y = atof(tok);}
                if(i==7) {ipl_temp_scale_z = atof(tok);}
                if(i==8) {ipl_temp_rot_x = atof(tok);}
                if(i==9) {ipl_temp_rot_y = atof(tok);}
                if(i==10) {ipl_temp_rot_z = atof(tok);}
                if(i==11) {ipl_temp_rot_w = atof(tok);}

                i++;

                tok = strtok(NULL, " ,");
            }

            quaternionsToAngles(ipl_temp_rot_x, ipl_temp_rot_y, ipl_temp_rot_z, ipl_temp_rot_w, q2a_out_x, q2a_out_y, q2a_out_z);

            ObjectInstance *instance = new ObjectInstance(Device->getSceneManager());

            instance->setName(ipl_temp_name);
            instance->setID(ipl_temp_id);
            instance->setPosition(ipl_temp_pos_x, ipl_temp_pos_y, ipl_temp_pos_z);
            instance->setRotation(q2a_out_x, q2a_out_y, q2a_out_z);
            instance->setScale(ipl_temp_scale_x, ipl_temp_scale_y, ipl_temp_scale_z);

            Instances.push_back(*instance);
        }
    }
}

void CGTASceneNode::processIDE(stringc inStr)
{
    int ide_temp_id = 0;
    stringc ide_temp_name;
    stringc ide_temp_texturedir;

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
                if(i==0) {ide_temp_id = atoi(tok);}
                if(i==1) {ide_temp_name = tok;}
                if(i==2) {ide_temp_texturedir = tok;}

                i++;

                tok = strtok(NULL, " ,");
            }

            Definitions[ide_temp_id].setName(ide_temp_name);
            Definitions[ide_temp_id].setTextureDir(ide_temp_texturedir);
            Definitions[ide_temp_id].loadFromDFFFile();
        }
    }
}

CGTASceneNode::ObjectInstance::ObjectInstance(ISceneManager *sceneManager)
{
    SceneManager = sceneManager;

    Id = -1;

    Mesh = NULL;

    Name = "";

    PositionX = 0.0f;
    PositionY = 0.0f;
    PositionZ = 0.0f;

    RotationX = 0.0f;
    RotationY = 0.0f;
    RotationZ = 0.0f;

    ScaleX = 0.0f;
    ScaleY = 0.0f;
    ScaleZ = 0.0f;

    State = 0;
}

CGTASceneNode::ObjectInstance::~ObjectInstance()
{

}

void CGTASceneNode::ObjectInstance::setName(stringc name)
{
    Name = name;
}

void CGTASceneNode::ObjectInstance::setID(int id)
{
    Id = id;
}

int CGTASceneNode::ObjectInstance::getID()
{
    return Id;
}

void CGTASceneNode::ObjectInstance::setPosition(float x, float y, float z)
{
    PositionX = x;
    PositionY = y;
    PositionZ = z;
}

void CGTASceneNode::ObjectInstance::setRotation(float x, float y, float z)
{
    RotationX = x;
    RotationY = y;
    RotationZ = z;
}

void CGTASceneNode::ObjectInstance::setScale(float x, float y, float z)
{
    ScaleX = x;
    ScaleY = y;
    ScaleZ = z;
}

void CGTASceneNode::ObjectInstance::setMesh(IAnimatedMesh *mesh)
{
    Mesh = mesh;
}

void CGTASceneNode::ObjectInstance::init()
{
    if (Mesh != NULL) {

        Node = SceneManager->addMeshSceneNode(Mesh);
        // Node = SceneManager->addOctTreeSceneNode(Mesh->getMesh(0));

        Node->setPosition(core::vector3df(PositionX, PositionY, PositionZ));
        Node->setRotation(core::vector3df(RotationX, RotationY, RotationZ));

        State = 1;
    }
}

void CGTASceneNode::ObjectInstance::render()
{
    if(!State) {init();}
}

CGTASceneNode::ObjectDefinition::ObjectDefinition(ISceneManager *sceneManager)
{
    SceneManager = sceneManager;

    Mesh = NULL;

    Name = "";
    TextureDir = "";
}

CGTASceneNode::ObjectDefinition::~ObjectDefinition()
{

}

void CGTASceneNode::ObjectDefinition::setName(stringc name)
{
    Name = name;
}

void CGTASceneNode::ObjectDefinition::setTextureDir(stringc textureDir)
{
    TextureDir = textureDir;
}

IAnimatedMesh* CGTASceneNode::ObjectDefinition::getMesh()
{
    return Mesh;
}

void CGTASceneNode::ObjectDefinition::loadFromDFFFile()
{
    stringc completeFileName = "";
    completeFileName += SceneManager->getParameters()->getAttributeAsString("GTA_MODELS_PATH");
    completeFileName += Name;
    completeFileName += ".dff";

    bool lodMode = SceneManager->getParameters()->getAttributeAsBool("GTA_LOD_MESHES_ONLY");
    bool isLodMesh = (Name.find("LOD") != -1);

    if((lodMode && isLodMesh) || (!lodMode && !isLodMesh))
    {
        stringc texturePath = "";
        texturePath += SceneManager->getParameters()->getAttributeAsString("GTA_TEXTURES_PATH");
        texturePath += TextureDir;
        texturePath += "/";

        SceneManager->getParameters()->setAttribute("DFF_TEXTURE_PATH", texturePath.c_str());

        Mesh = SceneManager->getMesh(completeFileName.c_str());

        if(!Mesh)
        {
            // error loading mesh
        }
    }
}

void quaternionsToAngles(float in_x, float in_y, float in_z, float in_w, float out_x, float out_y, float out_z)
{
    float pi = 3.141592654;

    float temp_x;
    float temp_y;
    float temp_z;

    float sqx = pow(in_x, 2);
    float sqy = pow(in_y, 2);
    float sqz = pow(in_z, 2);
    float sqw = pow(in_w, 2);
    float unit = sqx + sqy + sqz + sqw;
    float test = in_x*in_y+in_z*in_w;

    if(test > 0.4999999999999 * unit) {

        temp_x = 2 * atan2(in_x,in_w);
        temp_z = pi/2;
        temp_y = 0;
    }
    else if(test < -0.4999999999999 * unit){
        temp_x = -2 * atan2(in_x,in_w);
        temp_z = -pi/2;
        temp_y = 0;
    }
    else {
        float tmp = (2*in_y*in_w)-(2*in_x*in_z);
        temp_x = atan2(tmp,sqx-sqy-sqz+sqw);
        temp_z = asin(2 * test/unit);
        tmp = (2*in_x*in_w)-(2*in_y*in_z);
        temp_y = atan2(tmp,-sqx+sqy-sqy+sqw);
    }

    out_x = temp_x;
    out_y = temp_y;
    out_z = temp_z;
}

bool startsWith(stringc *haystack, c8 *needle)
{
    return haystack->trim().equalsn(needle, strlen(needle));
}
