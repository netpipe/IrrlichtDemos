#ifndef __APP__
#define __APP__

class App;

#include <irrlicht.h>
#include "Player.h"
#include "Cofre.h"
#include "Camera.h"
#include <vector>
#include "Guarda.h"

#include "aStar/AStar.h"
#include "aStar/Graph.h"

#include "Input.h"

//#include "irrklang/irrKlang.h"

//using namespace irrklang;

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace std;

enum COR
{
    BRANCO, AZUL, AMARELO, VERDE, VERMELHO, PRETO
};

class App
{
public:
    App();
    ~App();

    void eventMouseClick(int mouse);

    void loadMap(stringc mapFile);

    void run();
private:
    IrrlichtDevice *device;
    IVideoDriver* driver;
    ISceneManager* smgr;
    IGUIEnvironment* guienv;

    bool alarme;
    static void* playSound(void*);
    void playAlarme();

    vector<Camera*> cameras;
    vector<Guarda*> guardas;

    COR getCor(SColor cor);

    Graph* graphWaypoints;

    bool vetorContem(vector<int> vec,int num);

    vector<vector3df> arestasStart;
    vector<vector3df> arestasEnd;

    Player* player;
    EventReceiver* receiver;

    ITexture* vidaImage;
    ITexture* ouroImage;
    ITexture* vidaImage_s;
    ITexture* ouroImage_s;

    void drawHud(int totalVidas, bool ouro);
    rect<s32> myRect(s32 x,s32 y, s32 w, s32 h);

    IMeshSceneNode* goal;
    Cofre* cofre;

    int tentativas;

    ITexture* allesblauImage;
    ITexture* fracassado;

//    ISoundEngine* soundEngine;
//    ISound* alarmeSound;
};

#endif
