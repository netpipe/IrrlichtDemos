#ifndef MAIN_H
#define MAIN_H

#define guiID_HSWin 666
#define guiID_HSTxt 667
#define guiID_HSBtn 668

//#include <windows.h>
#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

struct cubeDat{
  int Typ; // 1 - 7
  int Rot; // Rotation (0 - 3)
  position2d<int> Pos; // Position (0-10, 0-21)
  int Map[4][4];
};

struct hsDat{
  int Score;
  wchar_t Name[50];
};

class cGameIrrTris : public IEventReceiver{
  private:
  	IrrlichtDevice *device;
  	IVideoDriver* driver;
  	IGUIEnvironment* env;

  	ITexture* imgBack;

  	bool progDeath;
  	bool gameRunning;
  	bool showHighScores;

  	cubeDat cubeNow;
  	cubeDat cubeNext;
  	int Map[10][22];
    int Score;
  	IGUIStaticText* txtScore;
    long tmrInterval;

    void loadHighScores();
    void saveHighScores();
    void enableHS();
    void updateGame();
    void drawMap();
    void getNextCube();
    void getPlayCube();
    void setCubeToMap();
    void getCubeMap(cubeDat* cb);
    bool checkCubePos();
    void addToHighScores();
    void updateNewHSColor();

    array<hsDat> HighScores;
    int posNewScore;
    IGUIStaticText* hsView[25];

	public:
    void runGame();
  	virtual bool OnEvent(const SEvent& event);
	  int t;

};

#endif // MAIN_H
