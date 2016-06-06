#include "zone.h"

CZone::CZone(aabbox3d<f32> dim, int color, int type, int ID, ISceneManager* smgr, IVideoDriver* driver, CPlayer* Player, CSound* Sound)
{
    CZone::Player = Player;
//    CZone::Sound = Sound;
    CZone::ID = ID;
    CZone::driver = driver;
    CZone::smgr = smgr;
    CZone::Dim = new aabbox3d<f32>(dim);
    CZone::Dim->repair();
    if(DEBUG_ZONES)
        CZone::Visible = true;
    else
        CZone::Visible = false;
    CZone::Active = true;
    CZone::type = type;
    CZone::Sound_Playing = false;

    ITexture*  ZTex;

    //Create Node
    if(color == Z_RED)
        ZTex = driver->getTexture("./Media/Textures/red.png");
    if(color == Z_BLUE)
        ZTex = driver->getTexture("./Media/Textures/blue.png");
    if(color == Z_GREEN)
        ZTex = driver->getTexture("./Media/Textures/green.png");
    if(color != Z_RED && color != Z_BLUE && color != Z_GREEN)
        ZTex = driver->getTexture("./Media/Textures/black.png");

    CZone::Node = smgr->addCubeSceneNode(1, 0, -1, CZone::Dim->getCenter(), vector3df(0), CZone::Dim->MaxEdge-CZone::Dim->MinEdge);
    CZone::Node->setMaterialFlag(EMF_LIGHTING, false);
    CZone::Node->setMaterialTexture(0, ZTex);
    CZone::Node->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
    CZone::Node->setVisible(CZone::Visible);
}

void CZone::ActivateTriggers()
{
    if(!CZone::Triggers.empty())
        for(list<CTrigger *>::Iterator Iterator = CZone::Triggers.begin(); Iterator != CZone::Triggers.end(); Iterator++)
        {
            if((*Iterator)->Active)
            {
                if((*Iterator)->type==TR_SOUND)
                {
                    if(CZone::Sound_Playing == false && SOUNDS)
                    {
//                        CZone::Sound->getEngine()->stopAllSounds();
//                        CZone::Sound_Object = CZone::Sound->getEngine()->play2D((*Iterator)->Sound_filename.c_str(), (*Iterator)->Sound_loop, false, true);
//                        CZone::Sound_Playing = true;
                    }
                    if(CZone::Sound_Playing == true)
                    {
//                        if(CZone::Sound_Object->isFinished())
          //              {
          //                  if((*Iterator)->EOSA_type == EOSA_PCTRL)
           //                     CZone::Player->setActive((*Iterator)->EOSAction);
           //                 CZone::Sound_Playing = false;
           //                 (*Iterator)->Active = false;
           //             }
                    }
                }
                if((*Iterator)->type==TR_AMMO)
                {
                    if((*Iterator)->Ammo_action==1) //Add
                        CZone::Player->addAmmo((*Iterator)->Ammo_type, (*Iterator)->Ammo_amount);
                    if((*Iterator)->Ammo_action==2) //Subtract
                        CZone::Player->subAmmo((*Iterator)->Ammo_type, (*Iterator)->Ammo_amount);
                    (*Iterator)->Active = false;
                }
                if((*Iterator)->type==TR_CONTROL)
                {
                    if(!CC_PHYSICS)
                        CZone::Player->setActive((*Iterator)->Player_Control);
                    (*Iterator)->Active = false;
                }
                if((*Iterator)->type==TR_SHADER)
                {
                    if((*Iterator)->Shader_type==SHT_BLOOM)
                    {
                        if(DEBUG_CONSOLE)
                        {
                            if((*Iterator)->Shader_Action)
                                cout<<"Bloom Enabled"<<endl;
                            else
                                cout<<"Bloom Disabled"<<endl;
                        }
                        CZone::Player->BloomActive = (*Iterator)->Shader_Action;

                    }
                    if((*Iterator)->Shader_type==SHT_INVERT)
                    {
                        if(DEBUG_CONSOLE)
                        {
                            if((*Iterator)->Shader_Action)
                                cout<<"Invert Enabled"<<endl;
                            else
                                cout<<"Invert Disabled"<<endl;
                        }
                        CZone::Player->InvertActive = (*Iterator)->Shader_Action;
                    }
                    (*Iterator)->Active = false;
                }
            }
        }
}

int CZone::getID()
{
    return CZone::ID;
}

int CZone::getType()
{
    return CZone::type;
}

bool CZone::isActive()
{
    return CZone::Active;
}

void CZone::setActive(bool Active)
{
    CZone::Active = Active;
}

void CZone::setVisible(bool Visible)
{
    CZone::Visible = Visible;
}

bool CZone::isVisible()
{
    return CZone::Visible;
}

bool CZone::isPointInside(vector3df Point)
{
    return CZone::Dim->isPointInside(Point);
}

void CZone::remove()
{
    for(list<CTrigger *>::Iterator Iterator = CZone::Triggers.begin(); Iterator != CZone::Triggers.end();)
        Iterator = CZone::Triggers.erase(Iterator);

    if(DEBUG_CONSOLE)
        cout<<"Removed Zone"<<endl;
}
