#include "map.h"

CMap::CMap(core::stringc filename, ISceneManager* smgr, IVideoDriver* driver, CSound* Sound)
{
//    CMap::Sound = Sound;
    CMap::smgr = smgr;
    CMap::driver = driver;
    core::stringc directory="./Data/Maps/";
    core::stringc fulldir=directory+filename;
    std::string model,name;
    CMap::fulldir = fulldir;

    IrrXMLReader* xml = createIrrXMLReader(fulldir.c_str());

    if(xml==NULL)
        cout<<"Failed to open: "<<fulldir.c_str()<<endl;

	float scale,x,y,z,rx,ry,rz,mrx,mry,mrz;
	float lx,ly,lz,lr,lg,lb,la,lradius;
	float ar,ag,ab,aa;

    while(xml && xml->read())
    {
        switch(xml->getNodeType())
        {
            case EXN_ELEMENT:
            {
                if(!strcmp("Data", xml->getNodeName()))
                {
                    model = xml->getAttributeValue("Model");
                    name = xml->getAttributeValue("Name");
                    CMap::InternalName = xml->getAttributeValue("InternalName");
                }
				if(!strcmp("Rotation", xml->getNodeName()))
				{
					mrx = xml->getAttributeValueAsFloat("X");
					mry = xml->getAttributeValueAsFloat("Y");
					mrz = xml->getAttributeValueAsFloat("Z");
				}

                if(!strcmp("Size", xml->getNodeName()))
                    scale = xml->getAttributeValueAsFloat("Scale");
                if(!strcmp("Camera", xml->getNodeName()))
                {
                    x = xml->getAttributeValueAsFloat("PosX");
                    y = xml->getAttributeValueAsFloat("PosY");
                    z = xml->getAttributeValueAsFloat("PosZ");
                    rx = xml->getAttributeValueAsFloat("LookX");
                    ry = xml->getAttributeValueAsFloat("LookY");
                    rz = xml->getAttributeValueAsFloat("LookZ");
                }
				if(!strcmp("Ambience", xml->getNodeName()))
				{
					ar = xml->getAttributeValueAsFloat("R");
					ag = xml->getAttributeValueAsFloat("G");
					ab = xml->getAttributeValueAsFloat("B");
					aa = xml->getAttributeValueAsFloat("A");
				}
				if(!strcmp("Player", xml->getNodeName()))
				{
				    CMap::PlayerMoveSpeed = xml->getAttributeValueAsFloat("MoveSpeed");
				    CMap::PlayerRotSpeed = xml->getAttributeValueAsFloat("RotSpeed");
				    CMap::PlayerJumpSpeed = xml->getAttributeValueAsFloat("JumpSpeed");
				}
				if(!strcmp("Light", xml->getNodeName()))
				{
					lx = xml->getAttributeValueAsFloat("PosX");
                    ly = xml->getAttributeValueAsFloat("PosY");
                    lz = xml->getAttributeValueAsFloat("PosZ");
					lr = xml->getAttributeValueAsFloat("R");
                    lg = xml->getAttributeValueAsFloat("G");
                    lb = xml->getAttributeValueAsFloat("B");
					la = xml->getAttributeValueAsFloat("A");
					lradius = xml->getAttributeValueAsFloat("Radius");
					ILightSceneNode* light = smgr->addLightSceneNode(0,vector3df(lx,ly,lz),SColorf(lr,lg,lb,la),lradius);
					cout<<"PosZ- "<<lz<<endl;
					light->setVisible(true);

                    if(LIGHT_BB)
                    {
                        ISceneNode* node = smgr->addBillboardSceneNode(light, dimension2d<f32>(50, 50));
                        node->setMaterialFlag(EMF_LIGHTING, false);
                        node->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
                        node->setMaterialTexture(0, driver->getTexture("./Media/Billboards/particlewhite.bmp"));
                    }

					CMap::lights.push_back(light);
				}
				if(!strcmp("Sound", xml->getNodeName()))
				{
				    std::string type = xml->getAttributeValue("Type");
				    std::string filename = xml->getAttributeValue("File");
	//			    if(type=="Atmospheric")
//                        CMap::Atmospheric = CMap::Sound->getEngine()->play2D(filename.c_str(), true);
				}
            }
            break;
        }
    };
    delete xml;

    if(scale<=0)
        scale=1;

    CMap::CamPos = vector3df(x,y,z);
    CMap::CamTar = vector3df(rx,ry,rz);
    CMap::Position = vector3df(0,0,0);
    CMap::Rotation = vector3df(mrx, mry, mrz);
    CMap::Scale = vector3df(scale, scale, scale);
    CMap::name=name;
    CMap::model=model;
    std::string path="./Media/Maps/";
    path+=name;
    path+="/";
    path+=model;

	CMap::mesh = CMap::smgr->getMesh(path.c_str());

	if(CMap::mesh == NULL)
        cout<<"Failed to load: "<<path.c_str()<<endl;

	CMap::node = smgr->addOctreeSceneNode(CMap::mesh->getMesh(0));

    //Setup
    CMap::node->setMaterialFlag(EMF_LIGHTING,LIGHTING);
	CMap::node->setMaterialFlag(EMF_FOG_ENABLE,false);
    CMap::node->setScale(vector3df(scale,scale,scale));
	CMap::node->setMaterialFlag(EMF_NORMALIZE_NORMALS,true);
	smgr->getMeshManipulator()->recalculateNormals(CMap::mesh->getMesh(0),true);

	CMap::selector = smgr->createOctTreeTriangleSelector(CMap::mesh,CMap::node);
	CMap::node->setTriangleSelector(CMap::selector);

	path="./Media/Maps/";
	path+=name;
	path+="/Skybox/";
	std::string pathup=path;
	std::string pathdown=path;
	std::string pathleft=path;
	std::string pathright=path;
	std::string pathfront=path;
	std::string pathback=path;

	pathup+="up.bmp";
	pathdown+="down.bmp";
	pathleft+="left.bmp";
	pathright+="right.bmp";
	pathfront+="front.bmp";
	pathback+="back.bmp";

	ITexture* skytop = driver->getTexture(pathup.c_str());
	ITexture* skybottom = driver->getTexture(pathdown.c_str());
	ITexture* skyleft = driver->getTexture(pathleft.c_str());
	ITexture* skyright = driver->getTexture(pathright.c_str());
	ITexture* skyfront = driver->getTexture(pathfront.c_str());
	ITexture* skyback = driver->getTexture(pathback.c_str());

	CMap::smgr->setAmbientLight(SColorf(ar,ag,ab,aa));

	CMap::skybox = smgr->addSkyBoxSceneNode(skytop,skybottom,skyleft,skyright,skyfront,skyback);
	CMap::node->setRotation(vector3df(mrx,mry,mrz));
	CMap::node->setID(ID_IsMap);

	for(int i = 0; i <= CMap::node->getMaterialCount(); i++)
		CMap::node->getMaterial(i).NormalizeNormals = true;

    cout<<"Loaded map: "<<path.c_str()<<" successfully!"<<endl;
}

vector3df CMap::getPlayerInformation()
{
    vector3df Info = vector3df(CMap::PlayerMoveSpeed, CMap::PlayerRotSpeed, CMap::PlayerJumpSpeed);
    return Info;
}

vector3df CMap::getCamPosition()
{
    return CMap::CamPos;
}

vector3df CMap::getCamTarget()
{
    return CMap::CamTar;
}

ISceneNode* CMap::getNode()
{
    return CMap::node;
}

IAnimatedMesh* CMap::getMesh()
{
    return CMap::mesh;
}

vector3df CMap::getScale()
{
    return CMap::Scale;
}

ITriangleSelector* CMap::getSelector()
{
    return CMap::selector;
}

void CMap::remove()
{
//    CMap::Sound->getEngine()->stopAllSounds();

    if(CMap::skybox)
        CMap::skybox->remove();

    for(list<ILightSceneNode *>::Iterator Iterator = CMap::lights.begin(); Iterator != CMap::lights.end();)
    {
        (*Iterator)->remove();
        Iterator = CMap::lights.erase(Iterator);
    }

    if(DEBUG_CONSOLE)
        cout<<"Cleaned Map"<<endl;
}
