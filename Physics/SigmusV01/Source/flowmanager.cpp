#include "flowmanager.h"

CFlowManager::CFlowManager(IVideoDriver* driver, ISceneManager* smgr, CPlayer* Player, CMap* Map, CSound* Sound)
{
    CFlowManager::driver = driver;
    CFlowManager::smgr = smgr;
    CFlowManager::Player = Player;
    CFlowManager::Map = Map;
    CFlowManager::Sound = Sound;
}

void CFlowManager::AddZone(aabbox3d<f32> Dim, int Color, int type, int ID)
{
    CZone* Zone = new CZone(Dim, Color, type, ID, CFlowManager::smgr, CFlowManager::driver, CFlowManager::Player, CFlowManager::Sound);
    CFlowManager::Zones.push_back(Zone);
}

void CFlowManager::CheckZones()
{
    for(list<CZone *>::Iterator Iterator = CFlowManager::Zones.begin(); Iterator != CFlowManager::Zones.end(); Iterator++)
        if((*Iterator)->isActive())
        {
            if((*Iterator)->isPointInside(CFlowManager::Player->getCamera()->getPosition()))
            {
                if((*Iterator)->getType()==Z_DEATH)
                {
                    (*Iterator)->setActive(false);
                    CFlowManager::Player->isDead = true;
                }
                if((*Iterator)->getType()==Z_END)
                {
                    CFlowManager::Player->FinishedLevel = true;
                }
                if((*Iterator)->getType()==Z_NORMAL)
                {
                    (*Iterator)->ActivateTriggers();
                }
            }
        }
}

void CFlowManager::LoadMapZones()
{
    IrrXMLReader* xml = createIrrXMLReader(CFlowManager::Map->fulldir.c_str());

    if(xml)
        if(DEBUG_CONSOLE)
            cout<<"Loading Map Zones for "<<CFlowManager::Map->fulldir.c_str()<<endl;

    while(xml && xml->read())
    {
        switch(xml->getNodeType())
        {
            case EXN_ELEMENT:
            {
				if(!strcmp("Zone", xml->getNodeName()))
				{
                    float minx, miny, minz, maxx, maxy, maxz;
                    std::string color, type;
                    int _color, _type, ID;

                    minx = xml->getAttributeValueAsFloat("MinX");
                    miny = xml->getAttributeValueAsFloat("MinY");
                    minz = xml->getAttributeValueAsFloat("MinZ");
                    maxx = xml->getAttributeValueAsFloat("MaxX");
                    maxy = xml->getAttributeValueAsFloat("MaxY");
                    maxz = xml->getAttributeValueAsFloat("MaxZ");
                    color = xml->getAttributeValue("Color");
                    type = xml->getAttributeValue("Type");

                    if(color == "Red")
                        _color = Z_RED;
                    if(color == "Green")
                        _color = Z_GREEN;
                    if(color == "Blue")
                        _color = Z_BLUE;
                    if(color != "Red" && color != "Green" && color != "Blue")
                        _color = Z_BLACK;

                    if(type == "Death")
                        _type = Z_DEATH;
                    if(type == "Normal")
                    {
                        _type = Z_NORMAL;
                        ID = xml->getAttributeValueAsFloat("ID");
                    }
                    if(type == "End")
                        _type = Z_END;

                    CFlowManager::AddZone(aabbox3d<f32>(minx, miny, minz, maxx, maxy, maxz), _color, _type, ID);
                }
            }
            break;
        }
    };
    delete xml;
}

void CFlowManager::LoadMapTriggers()
{
    IrrXMLReader* xml = createIrrXMLReader(CFlowManager::Map->fulldir.c_str());

    if(xml)
        if(DEBUG_CONSOLE)
            cout<<"Loading Map Triggers for "<<CFlowManager::Map->fulldir.c_str()<<endl;

    while(xml && xml->read())
    {
        switch(xml->getNodeType())
        {
            case EXN_ELEMENT:
            {
                if(!strcmp("Trigger", xml->getNodeName()))
                {
                    int zone = xml->getAttributeValueAsInt("Zone");

                    for(list<CZone *>::Iterator Iterator = CFlowManager::Zones.begin(); Iterator != CFlowManager::Zones.end(); Iterator++)
                        if((*Iterator)->getType()==Z_NORMAL)
                            if((*Iterator)->getID()==zone)
                            {
                                std::string type = xml->getAttributeValue("Type");
                                if(type=="Sound")
                                {
                                    CTrigger* Trigger = new CTrigger(TR_SOUND);
                                    std::string file = xml->getAttributeValue("File");
                                    int loop = xml->getAttributeValueAsInt("Loop");
                                    Trigger->setSoundValues(file, loop);

                                    std::string EOSA = xml->getAttributeValue("EOSA");

                                    if(EOSA != "None")
                                    {
                                        if(EOSA=="PlayerCTRL")
                                            Trigger->EOSA_type = EOSA_PCTRL;

                                        std::string action = xml->getAttributeValue("EOSAction");
                                        bool _action;
                                        if(action=="Enable" | action=="1" | action=="On")
                                            _action = true;
                                        if(action=="Disable" | action=="0" | action=="Off")
                                            _action = false;
                                        Trigger->EOSAction = _action;
                                    }

                                    (*Iterator)->Triggers.push_back(Trigger);
                                }
                                if(type=="Ammo")
                                {
                                    CTrigger* Trigger = new CTrigger(TR_AMMO);
                                    std::string type = xml->getAttributeValue("AmmoType");
                                    int _type;
                                    if(type=="Explosive")
                                        _type = WT_EXPLOSIVE;
                                    if(type=="Attractive")
                                        _type = WT_ATTRACTIVE;
                                    if(type=="Normal")
                                        _type = WT_NORMAL;
                                    if(type != "Explosive" && type != "Attractive" && type != "Normal")
                                        _type = WT_NORMAL;
                                    int amount = xml->getAttributeValueAsInt("Amount");
                                    std::string action = xml->getAttributeValue("Action");
                                    int _action;
                                    if(action=="Add")
                                        _action = 1;
                                    if(action=="Subtract")
                                        _action = 2;
                                    Trigger->setAmmoValues(_type, amount, _action);
                                    (*Iterator)->Triggers.push_back(Trigger);
                                }
                                if(type=="Control")
                                {
                                    std::string action = xml->getAttributeValue("Action");
                                    bool _action;
                                    if(action=="Enable" | action=="1" | action=="On")
                                        _action = true;
                                    if(action=="Disable" | action=="0" | action=="Off")
                                        _action = false;
                                    CTrigger* Trigger = new CTrigger(TR_CONTROL);
                                    Trigger->Player_Control = _action;
                                    (*Iterator)->Triggers.push_back(Trigger);
                                }
                                if(type=="Shader")
                                {
                                    std::string type = xml->getAttributeValue("Effect");
                                    int _type;
                                    if(type=="Bloom")
                                        _type = SHT_BLOOM;
                                    if(type=="Invert")
                                        _type = SHT_INVERT;

                                    std::string action = xml->getAttributeValue("Action");
                                    bool _action;
                                    if(action=="Enable" | action=="1" | action=="On")
                                        _action = true;
                                    if(action=="Disable" | action=="0" | action=="Off")
                                        _action = false;

                                    CTrigger* Trigger = new CTrigger(TR_SHADER);
                                    Trigger->Shader_Action = _action;
                                    Trigger->Shader_type = _type;
                                    (*Iterator)->Triggers.push_back(Trigger);
                                }
                            }
                }
            }
            break;
        }
    };
    delete xml;
}

void CFlowManager::remove()
{
    for(list<CZone *>::Iterator Iterator = CFlowManager::Zones.begin(); Iterator != CFlowManager::Zones.end();)
    {
        (*Iterator)->remove();
        Iterator = CFlowManager::Zones.erase(Iterator);
    }

    if(DEBUG_CONSOLE)
        cout<<"Cleaned Flow Manager"<<endl;

}
