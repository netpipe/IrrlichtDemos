#include "App.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace std;

App::App()
{
    device = createDevice(EDT_OPENGL, dimension2du(800, 600), 32,false, false, true, 0);

    receiver=new EventReceiver();

    device->setWindowCaption(L"Nocturne Thief");

    device->setEventReceiver(receiver);

    driver = device->getVideoDriver();

    smgr = device->getSceneManager();
    guienv = device->getGUIEnvironment();

    IGUIFont* font=guienv->getFont("../media/font14.png");
    guienv->getSkin()->setFont(font);

    smgr->addLightSceneNode(0,vector3df(0,100,0),SColorf( 0.27, 0.37, 0.52),150.0f);
    smgr->setAmbientLight(SColorf(0.3, 0.3, 0.5));


    player=new Player(smgr);
    player->setAnimation(PLAYER_ANIM_WAIT);

    IMesh* pisoMesh=smgr->getMesh("../media/piso.b3d");
    IMeshSceneNode* piso=smgr->addMeshSceneNode(pisoMesh);
    piso->setMaterialTexture(0,smgr->getVideoDriver()->getTexture("../media/piso.jpg"));
    piso->setScale(vector3df(50,1,50));
    piso->getMaterial(0).getTextureMatrix(0).setScale(vector3df(10,10,10));

    vidaImage=driver->getTexture("../media/vidas.png");
    ouroImage=driver->getTexture("../media/ouroHud.png");

    vidaImage_s=driver->getTexture("../media/vidas_s.png");
    ouroImage_s=driver->getTexture("../media/ouroHud_s.png");

    goal=smgr->addCubeSceneNode(1);
    goal->setScale(vector3df(2,0.1,2));
    goal->setMaterialFlag(EMF_LIGHTING,false);
    goal->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
    goal->setMaterialTexture(0,driver->getTexture("../media/goal.png"));

    allesblauImage=driver->getTexture("../media/xucrutzallesblau.png");
    fracassado=driver->getTexture("../media/fracassado.png");

//    soundEngine = createIrrKlangDevice();
}

App::~App()
{
    smgr->clear();
    device->drop();
}

void App::drawHud(int totalVidas, bool ouro)
{
    for(int i=0;i<3;i++)
    {
        driver->draw2DImage(vidaImage_s, position2d<s32>(20+i*30,20),
            myRect(0,0,25,30),
            0,
            SColor(255,255,255,255),
            true);
    }

    for(int i=0;i<totalVidas;i++)
    {
        driver->draw2DImage(vidaImage, position2d<s32>(20+i*30,20),
            myRect(0,0,25,30),
            0,
            SColor(255,255,255,255),
            true);
    }

    driver->draw2DImage(ouroImage_s, position2d<s32>(20,100),
        myRect(0,0,65,30),
        0,
        SColor(255,255,255,255),
        true);

    if(ouro)
    {
        driver->draw2DImage(ouroImage, position2d<s32>(20,100),
            myRect(0,0,65,30),
            0,
            SColor(255,255,255,255),
            true);
    }
}


void App::eventMouseClick(int mouse)
{
    switch(mouse)
    {
    case 0://click LMB
        break;
    case 1://click RMB
        break;
    case 3://release LMB
        break;
    case 6://move mouse
        break;
    }
}

COR App::getCor(SColor cor)
{
    if(cor.getRed() > 127 && cor.getGreen() > 127 && cor.getBlue() > 127) return BRANCO;
    if(cor.getRed() > 127 && cor.getGreen() > 127) return AMARELO;
    if(cor.getRed() > 127 ) return VERMELHO;
    if(cor.getGreen() > 127) return VERDE;
    if(cor.getBlue() > 127) return AZUL;
    return PRETO;
}

void App::loadMap(stringc mapFile)
{
    IImage* map=smgr->getVideoDriver()->createImageFromFile(mapFile.c_str());

    vector<ISceneNode*> waypointsTemp;

    vector<ITriangleSelector*> selectors;

    for(int z=map->getDimension().Height-1;z >=0 ;z--)
    {
        for(int x=0;x < map->getDimension().Width;x++)
        {
            if( getCor(map->getPixel(x,z)) == VERDE)
            {
                cofre=new Cofre(smgr);
                cofre->setPosition(vector3df(x,0,32-z));
                cofre->setEstado(COFRE_FECHADO);
                selectors.push_back(cofre->getTriangleSelector());
            }
            else if( getCor(map->getPixel(x,z)) == VERMELHO)
            {
                Camera* camera=new Camera(smgr);
                camera->setPosition(vector3df(x,0,32-z));
                cameras.push_back(camera);
            }
            else if( getCor(map->getPixel(x,z)) == AMARELO)
            {
                Guarda* guarda=new Guarda(smgr);
                guarda->setPosition(vector3df(x,0,32-z));
                guardas.push_back(guarda);
            }
            else if( getCor(map->getPixel(x,z)) == BRANCO)
            {
                IMesh* mesh=smgr->getMesh("../media/caixa.b3d");
                IMeshSceneNode* cube=smgr->addMeshSceneNode(mesh);
                cube->setPosition(vector3df(x,0,32-z));
                selectors.push_back(smgr->createTriangleSelectorFromBoundingBox(cube));
            }
            else if( getCor(map->getPixel(x,z)) == AZUL)
            {
                player->setPosition(vector3df(x,0,32-z));
                goal->setPosition(vector3df(x,0,32-z));
            }
        }
    }

    for(int i=0;i<selectors.size();i++)
    {
        ITriangleSelector* selector=selectors[i];
        player->getNode()->addAnimator(smgr->createCollisionResponseAnimator(selector,player->getNode(),vector3df(1,1,1),vector3df(0,0,0)));
    }

    for(int z=map->getDimension().Height-1;z >= 0 ;z-=2)
    {
        for(int x=0;x < map->getDimension().Width;x+=2)
        {
            if( getCor(map->getPixel(x,z)) == PRETO)
            {
                ISceneNode* temp=smgr->addEmptySceneNode();
                temp->setPosition(vector3df(x,0,32-z));
                waypointsTemp.push_back(temp);
            }
        }
    }

    vector<int> origem;
    vector<int> destino;
    f32 distance;

    for(int i=0;i<waypointsTemp.size();i++)
    {
        for(int j=0;j<waypointsTemp.size();j++)
        {
            distance = ((ISceneNode*)waypointsTemp[i])->getPosition().getDistanceFrom(((ISceneNode*)waypointsTemp[j])->getPosition());
            if( distance < 2.1)
            {
                bool arestaExiste=false;
                for(int x=0;x<origem.size();x++)///Verificar se a aresta ainda não foi criada
                {
                    if(origem[i] == i && destino[i] == j) arestaExiste=true;
                    if(origem[i] == j && destino[i] == i) arestaExiste=true;
                }
                if(!arestaExiste && i!=j)
                {
                    arestasStart.push_back(waypointsTemp[i]->getPosition());
                    arestasEnd.push_back(waypointsTemp[j]->getPosition());

                    origem.push_back(i);//adicionar arestas nos dois sentidos
                    destino.push_back(j);
                    origem.push_back(j);
                    destino.push_back(i);
                }
            }
        }
    }

    cout << "GRAFO:" << origem.size() << endl;

    graphWaypoints=new Graph();

    vector<int> tempNodes;

    for(int i=0;i<origem.size();i++)///Obter total de nós, adicionar estes nós ao grafo
    {
        if(!vetorContem(tempNodes,origem[i]))
        {
            tempNodes.push_back(origem[i]);
            graphWaypoints->addNode(origem[i],waypointsTemp[origem[i]]->getPosition());
        }
    }

    ///Criar arestas
    for(int i=0;i<origem.size();i++)
    {
        graphWaypoints->addArc(graphWaypoints->getNodeByID(origem[i]), graphWaypoints->getNodeByID(destino[i]));
    }

    for(int i=0;i<guardas.size();i++)
    {
        ((Guarda*)guardas[i])->setWaypoints(graphWaypoints);
    }
}

void* App::playSound(void* pthis)
{
    App* app=(App*)pthis;
    app->playAlarme();
}

void App::playAlarme()
{
    //while(alarme)
    {
        system("mplayer -loop 6 ../media/alarme.wav");
    }
    alarme=false;
}

void App::run()
{
    IGUIInOutFader* fader=guienv->addInOutFader();///TODO:move pra construtor

    tentativas=3;

    video::SMaterial material;
    material.Lighting = false;

    receiver->clearKeys();

    bool ouroNoSaco=false;
    bool gameplay=true;

    int counter=0;

    bool allesblau=false;

    while(device->run())
    {
        if(gameplay)
        {
            for(int i=0;i<cameras.size();i++)
            {
                if(((Camera*)cameras[i])->update(player->getPosition()))
                {
                    if(!alarme)
                    {
                        alarme=true;

//                        alarmeSound = soundEngine->play2D("../media/alarme.wav", true);

                        for(int i=0;i<guardas.size();i++) ((Guarda*)guardas[i])->alarm( player->getPosition() );
                    }
                }
            }


            for(int i=0;i<guardas.size();i++)
            {
                if( ((Guarda*)guardas[i])->update(player->getPosition()) )
                {
                    alarme=false;

//                    soundEngine->play2D("../media/mgsfound.wav",false);

                    fader->fadeOut(1000);
                    tentativas--;
                    player->setPosition(player->getPosition() + vector3df(0,20,0));
                    smgr->drawAll();
                    player->setPosition(goal->getPosition()+vector3df(0,20,0));
                    smgr->drawAll();
                    player->setPosition(goal->getPosition());
                    smgr->drawAll();
                    ouroNoSaco=false;
                    cofre->setEstado(COFRE_FECHADO);
                    fader->fadeIn(1000);
                }
            }

            if(alarme)
            {
                if(counter < 25)
                    smgr->setAmbientLight(SColorf(0.5,0.3,0.3,0.3));
                else
                    smgr->setAmbientLight(SColorf(0.3, 0.3, 0.5));

                counter++;
                if(counter > 50) counter =0;
            }
            else
            {
//                if(alarmeSound) alarmeSound->stop();
 ///               smgr->setAmbientLight(SColorf(0.3, 0.3, 0.5));
            }

            if(player->getPosition().getDistanceFrom(goal->getPosition()) < 2 && ouroNoSaco)
            {
                //guienv->addMessageBox(L"Voce venceu!",L"Parabens voce venceu, pressione ESC para sair.");
                gameplay=false;
                allesblau=true;
            }
            if(tentativas <= 0)
            {
                allesblau=false;
                //guienv->addMessageBox(L"Voce Perdeu!",L"Que vergonha, voce foi derrotado por agentes inteligentes.\nPressione ESC para sair.");
                gameplay=false;
            }

            if(player->getPosition().getDistanceFrom(cofre->getPosition()) < 2)
            {
                ouroNoSaco=true;
                cofre->setEstado(COFRE_ABERTO);
            }

            driver->beginScene(true, true, SColor(0,200,200,250));

            smgr->drawAll();
            guienv->drawAll();

            drawHud(tentativas,ouroNoSaco);

            /*for(int i=0;i<arestasStart.size();i++)
            {
                driver->setMaterial(material);
                driver->setTransform(video::ETS_WORLD, core::matrix4());
                driver->draw3DLine(arestasStart[i],arestasEnd[i],SColor(255,255,255,255));
            }*/



            if(receiver->getKeyState(KEY_LEFT))
                player->walk(PWALK_L,ouroNoSaco);
            else if(receiver->getKeyState(KEY_RIGHT))
                player->walk(PWALK_R,ouroNoSaco);
            else if(receiver->getKeyState(KEY_UP))
                player->walk(PWALK_U,ouroNoSaco);
            else if(receiver->getKeyState(KEY_DOWN))
                player->walk(PWALK_D,ouroNoSaco);
            else
                if(ouroNoSaco)
                    player->setAnimation(PLAYER_ANIM_WAIT_GOLD);
                else
                    player->setAnimation(PLAYER_ANIM_WAIT);

            driver->endScene();
        }
        else
        {
            alarme=false;
            driver->beginScene(true, true, SColor(0,200,200,250));
            //guienv->drawAll();
            if(allesblau)//Se ta alles blau!
            {
                driver->draw2DImage(allesblauImage, position2d<s32>(0,0),
                myRect(0,0,800,600),
                0,
                SColor(255,255,255,255),
                false);
            }
            else
            {
                driver->draw2DImage(fracassado, position2d<s32>(0,0),
                myRect(0,0,800,600),
                0,
                SColor(255,255,255,255),
                false);
            }

            driver->endScene();
        }

        if(receiver->getKeyState(KEY_ESCAPE)) device->closeDevice();
    }
}

bool App::vetorContem(vector<int> vec,int num)
{
    for(int i=0;i<vec.size();i++)
    {
        if(vec[i] == num) return true;
    }

    return false;
}

rect<s32> App::myRect(s32 x,s32 y, s32 w, s32 h)
{
    return rect<s32>(x,y,x+w,y+h);
}
