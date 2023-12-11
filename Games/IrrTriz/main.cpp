
////////////////////////////////////////////////////////////////////
//                           "IrrTris"                            //
////////////////////////////////////////////////////////////////////
// DevCpp Version 2005-2006 by A. Buschhüter (abusoft@freenet.de) //
////////////////////////////////////////////////////////////////////

#include "main.h"
#include "Resourcen.h"
#include "sys/times.h"
int main(){
  cGameIrrTris Spiel;
  Spiel.runGame();
	return 0;
}

bool cGameIrrTris::OnEvent(const SEvent &event){
  if(event.EventType == EET_GUI_EVENT){
    s32 id = event.GUIEvent.Caller->getID();
    switch(id){
      case guiID_HSBtn:{
        wchar_t nName[100];
        hsDat hs;
        swprintf(hs.Name, 100, L"%d - %s", Score, env->getRootGUIElement()->getElementFromId(guiID_HSTxt, true)->getText());
        hs.Score = Score;
        for(posNewScore = 0; HighScores[posNewScore].Score >= Score; posNewScore++);
        HighScores.insert(hs, posNewScore);
        HighScores.erase(25);
       	saveHighScores();
       	env->getRootGUIElement()->getElementFromId(guiID_HSWin, true)->remove();
        showHighScores = true;
        device->getCursorControl()->setVisible(false);
      	device->getCursorControl()->setPosition(400, 300);
        return true;
      }break;
    }
  }else if((event.EventType == EET_KEY_INPUT_EVENT) && !event.KeyInput.PressedDown){
    switch(event.KeyInput.Key){
      case KEY_ESCAPE:{
        if(gameRunning){
          gameRunning = false;
        }else{
          progDeath = true;
        }
        return true;
      }break;
      case KEY_RETURN:{
        if(env->getRootGUIElement()->getElementFromId(guiID_HSTxt, true)){
          wchar_t nName[100];
          hsDat hs;
          swprintf(hs.Name, 100, L"%d - %s", Score, env->getRootGUIElement()->getElementFromId(guiID_HSTxt, true)->getText());
          hs.Score = Score;
          for(posNewScore = 0; HighScores[posNewScore].Score >= Score; posNewScore++);
          HighScores.insert(hs, posNewScore);
          HighScores.erase(25);
         	saveHighScores();
         	env->getRootGUIElement()->getElementFromId(guiID_HSWin, true)->remove();
          showHighScores = true;
          device->getCursorControl()->setVisible(false);
        	device->getCursorControl()->setPosition(400, 300);
        }
      }break;
      case KEY_LEFT:{
        if(gameRunning){
          // left
          cubeNow.Pos.X--;
          if(!checkCubePos()) cubeNow.Pos.X++;
        }
        return true;
      }break;
      case KEY_RIGHT:{
        if(gameRunning){
          // right
          cubeNow.Pos.X++;
          if(!checkCubePos()) cubeNow.Pos.X--;
        }
        return true;
      }break;
      case KEY_DOWN:{
        if(gameRunning){
          // drop
          while(checkCubePos()) cubeNow.Pos.Y++;
          cubeNow.Pos.Y--;
          setCubeToMap();
        }
        return true;
      }break;
      case KEY_SPACE:{
        if(gameRunning){
          // rotate
          cubeNow.Rot = (cubeNow.Rot + 1) % 4;
          getCubeMap(&cubeNow);
          if(!checkCubePos()){
            cubeNow.Rot--;
            if(cubeNow.Rot < 0) cubeNow.Rot = 3;
            getCubeMap(&cubeNow);
          }
        }
        return true;
      }break;
      case KEY_F1:{
        if(env->getRootGUIElement()->getElementFromId(guiID_HSWin, true)) return false;
        if(!gameRunning) showHighScores = !showHighScores;
        return true;
      }break;
      case KEY_F2:{
        if(env->getRootGUIElement()->getElementFromId(guiID_HSWin, true)) return false;
        if(!gameRunning){
          showHighScores = false;
          posNewScore = -1;
          Score = 0;
          // clear map
          for(int x = 0; x < 10; x++)
            for(int y = 0; y < 22; y++)
              Map[x][y] = 0;
          // get next cube
          srand(device->getTimer()->getTime());
          getNextCube();
          getPlayCube();
          tmrInterval = 1000; // 1 sec.
          gameRunning = true;
        }
        return true;
      }break;
    }
  }
  return false;
}

void cGameIrrTris::getCubeMap(cubeDat* cb){
   switch(cb->Typ){
    case 0:{
      if((cb->Rot == 0) || (cb->Rot == 2)){ // 0°
        // row 1
        cb->Map[0][0] = 0;
        cb->Map[1][0] = 0;
        cb->Map[2][0] = 0;
        cb->Map[3][0] = 0;
        // row 2
        cb->Map[0][1] = cb->Typ + 1;
        cb->Map[1][1] = cb->Typ + 1;
        cb->Map[2][1] = cb->Typ + 1;
        cb->Map[3][1] = cb->Typ + 1;
        // row 3
        cb->Map[0][2] = 0;
        cb->Map[1][2] = 0;
        cb->Map[2][2] = 0;
        cb->Map[3][2] = 0;
        // row 4
        cb->Map[0][3] = 0;
        cb->Map[1][3] = 0;
        cb->Map[2][3] = 0;
        cb->Map[3][3] = 0;
      }else if((cb->Rot == 1) || (cb->Rot == 3)){ // 90°
        cb->Map[0][0] = 0;
        cb->Map[1][0] = cb->Typ + 1;
        cb->Map[2][0] = 0;
        cb->Map[3][0] = 0;
        // row 2
        cb->Map[0][1] = 0;
        cb->Map[1][1] = cb->Typ + 1;
        cb->Map[2][1] = 0;
        cb->Map[3][1] = 0;
        // row 3
        cb->Map[0][2] = 0;
        cb->Map[1][2] = cb->Typ + 1;
        cb->Map[2][2] = 0;
        cb->Map[3][2] = 0;
        // row 4
        cb->Map[0][3] = 0;
        cb->Map[1][3] = cb->Typ + 1;
        cb->Map[2][3] = 0;
        cb->Map[3][3] = 0;
      }
    }break;
    case 1:{
      cb->Map[0][0] = 0;
      cb->Map[1][0] = cb->Typ + 1;
      cb->Map[2][0] = cb->Typ + 1;
      cb->Map[3][0] = 0;
      // row 2
      cb->Map[0][1] = 0;
      cb->Map[1][1] = cb->Typ + 1;
      cb->Map[2][1] = cb->Typ + 1;
      cb->Map[3][1] = 0;
      // row 3
      cb->Map[0][2] = 0;
      cb->Map[1][2] = 0;
      cb->Map[2][2] = 0;
      cb->Map[3][2] = 0;
      // row 4
      cb->Map[0][3] = 0;
      cb->Map[1][3] = 0;
      cb->Map[2][3] = 0;
      cb->Map[3][3] = 0;
    }break;
    case 2:{
      if(cb->Rot == 0){ // 0°
        cb->Map[0][0] = 0;
        cb->Map[1][0] = 0;
        cb->Map[2][0] = 0;
        cb->Map[3][0] = 0;
        // row 2
        cb->Map[0][1] = cb->Typ + 1;
        cb->Map[1][1] = cb->Typ + 1;
        cb->Map[2][1] = cb->Typ + 1;
        cb->Map[3][1] = 0;
        // row 3
        cb->Map[0][2] = 0;
        cb->Map[1][2] = cb->Typ + 1;
        cb->Map[2][2] = 0;
        cb->Map[3][2] = 0;
        // row 4
        cb->Map[0][3] = 0;
        cb->Map[1][3] = 0;
        cb->Map[2][3] = 0;
        cb->Map[3][3] = 0;
      }else if(cb->Rot == 1){ // 90°
        cb->Map[0][0] = 0;
        cb->Map[1][0] = cb->Typ + 1;
        cb->Map[2][0] = 0;
        cb->Map[3][0] = 0;
        // row 2
        cb->Map[0][1] = cb->Typ + 1;
        cb->Map[1][1] = cb->Typ + 1;
        cb->Map[2][1] = 0;
        cb->Map[3][1] = 0;
        // row 3
        cb->Map[0][2] = 0;
        cb->Map[1][2] = cb->Typ + 1;
        cb->Map[2][2] = 0;
        cb->Map[3][2] = 0;
        // row 4
        cb->Map[0][3] = 0;
        cb->Map[1][3] = 0;
        cb->Map[2][3] = 0;
        cb->Map[3][3] = 0;
      }else if(cb->Rot == 2){ // 180°
        cb->Map[0][0] = 0;
        cb->Map[1][0] = cb->Typ + 1;
        cb->Map[2][0] = 0;
        cb->Map[3][0] = 0;
        // row 2
        cb->Map[0][1] = cb->Typ + 1;
        cb->Map[1][1] = cb->Typ + 1;
        cb->Map[2][1] = cb->Typ + 1;
        cb->Map[3][1] = 0;
        // row 3
        cb->Map[0][2] = 0;
        cb->Map[1][2] = 0;
        cb->Map[2][2] = 0;
        cb->Map[3][2] = 0;
        // row 4
        cb->Map[0][3] = 0;
        cb->Map[1][3] = 0;
        cb->Map[2][3] = 0;
        cb->Map[3][3] = 0;
      }else if(cb->Rot == 3){ // 270°
        cb->Map[0][0] = 0;
        cb->Map[1][0] = cb->Typ + 1;
        cb->Map[2][0] = 0;
        cb->Map[3][0] = 0;
        // row 2
        cb->Map[0][1] = 0;
        cb->Map[1][1] = cb->Typ + 1;
        cb->Map[2][1] = cb->Typ + 1;
        cb->Map[3][1] = 0;
        // row 3
        cb->Map[0][2] = 0;
        cb->Map[1][2] = cb->Typ + 1;
        cb->Map[2][2] = 0;
        cb->Map[3][2] = 0;
        // row 4
        cb->Map[0][3] = 0;
        cb->Map[1][3] = 0;
        cb->Map[2][3] = 0;
        cb->Map[3][3] = 0;
      }
    }break;
    case 3:{
      if((cb->Rot == 0) || (cb->Rot == 2)){ // 0°
        cb->Map[0][0] = 0;
        cb->Map[1][0] = 0;
        cb->Map[2][0] = 0;
        cb->Map[3][0] = 0;
        // row 2
        cb->Map[0][1] = cb->Typ + 1;
        cb->Map[1][1] = cb->Typ + 1;
        cb->Map[2][1] = 0;
        cb->Map[3][1] = 0;
        // row 3
        cb->Map[0][2] = 0;
        cb->Map[1][2] = cb->Typ + 1;
        cb->Map[2][2] = cb->Typ + 1;
        cb->Map[3][2] = 0;
        // row 4
        cb->Map[0][3] = 0;
        cb->Map[1][3] = 0;
        cb->Map[2][3] = 0;
        cb->Map[3][3] = 0;
      }else if((cb->Rot == 1) || (cb->Rot == 3)){ // 90°
        cb->Map[0][0] = 0;
        cb->Map[1][0] = cb->Typ + 1;
        cb->Map[2][0] = 0;
        cb->Map[3][0] = 0;
        // row 2
        cb->Map[0][1] = cb->Typ + 1;
        cb->Map[1][1] = cb->Typ + 1;
        cb->Map[2][1] = 0;
        cb->Map[3][1] = 0;
        // row 3
        cb->Map[0][2] = cb->Typ + 1;
        cb->Map[1][2] = 0;
        cb->Map[2][2] = 0;
        cb->Map[3][2] = 0;
        // row 4
        cb->Map[0][3] = 0;
        cb->Map[1][3] = 0;
        cb->Map[2][3] = 0;
        cb->Map[3][3] = 0;
      }
    }break;
    case 4:{
      if((cb->Rot == 0) || (cb->Rot == 2)){ // 0°
        cb->Map[0][0] = 0;
        cb->Map[1][0] = 0;
        cb->Map[2][0] = 0;
        cb->Map[3][0] = 0;
        // row 2
        cb->Map[0][1] = 0;
        cb->Map[1][1] = cb->Typ + 1;
        cb->Map[2][1] = cb->Typ + 1;
        cb->Map[3][1] = 0;
        // row 3
        cb->Map[0][2] = cb->Typ + 1;
        cb->Map[1][2] = cb->Typ + 1;
        cb->Map[2][2] = 0;
        cb->Map[3][2] = 0;
        // row 4
        cb->Map[0][3] = 0;
        cb->Map[1][3] = 0;
        cb->Map[2][3] = 0;
        cb->Map[3][3] = 0;
      }else if((cb->Rot == 1) || (cb->Rot == 3)){ // 90°
        cb->Map[0][0] = cb->Typ + 1;
        cb->Map[1][0] = 0;
        cb->Map[2][0] = 0;
        cb->Map[3][0] = 0;
        // row 2
        cb->Map[0][1] = cb->Typ + 1;
        cb->Map[1][1] = cb->Typ + 1;
        cb->Map[2][1] = 0;
        cb->Map[3][1] = 0;
        // row 3
        cb->Map[0][2] = 0;
        cb->Map[1][2] = cb->Typ + 1;
        cb->Map[2][2] = 0;
        cb->Map[3][2] = 0;
        // row 4
        cb->Map[0][3] = 0;
        cb->Map[1][3] = 0;
        cb->Map[2][3] = 0;
        cb->Map[3][3] = 0;
      }
    }break;
    case 5:{
      if(cb->Rot == 0){ // 0°
        cb->Map[0][0] = 0;
        cb->Map[1][0] = 0;
        cb->Map[2][0] = 0;
        cb->Map[3][0] = 0;
        // row 2
        cb->Map[0][1] = cb->Typ + 1;
        cb->Map[1][1] = cb->Typ + 1;
        cb->Map[2][1] = cb->Typ + 1;
        cb->Map[3][1] = 0;
        // row 3
        cb->Map[0][2] = 0;
        cb->Map[1][2] = 0;
        cb->Map[2][2] = cb->Typ + 1;
        cb->Map[3][2] = 0;
        // row 4
        cb->Map[0][3] = 0;
        cb->Map[1][3] = 0;
        cb->Map[2][3] = 0;
        cb->Map[3][3] = 0;
      }else if(cb->Rot == 1){ // 90°
        cb->Map[0][0] = 0;
        cb->Map[1][0] = cb->Typ + 1;
        cb->Map[2][0] = 0;
        cb->Map[3][0] = 0;
        // row 2
        cb->Map[0][1] = 0;
        cb->Map[1][1] = cb->Typ + 1;
        cb->Map[2][1] = 0;
        cb->Map[3][1] = 0;
        // row 3
        cb->Map[0][2] = cb->Typ + 1;
        cb->Map[1][2] = cb->Typ + 1;
        cb->Map[2][2] = 0;
        cb->Map[3][2] = 0;
        // row 4
        cb->Map[0][3] = 0;
        cb->Map[1][3] = 0;
        cb->Map[2][3] = 0;
        cb->Map[3][3] = 0;
      }else if(cb->Rot == 2){ // 180°
        cb->Map[0][0] = cb->Typ + 1;
        cb->Map[1][0] = 0;
        cb->Map[2][0] = 0;
        cb->Map[3][0] = 0;
        // row 2
        cb->Map[0][1] = cb->Typ + 1;
        cb->Map[1][1] = cb->Typ + 1;
        cb->Map[2][1] = cb->Typ + 1;
        cb->Map[3][1] = 0;
        // row 3
        cb->Map[0][2] = 0;
        cb->Map[1][2] = 0;
        cb->Map[2][2] = 0;
        cb->Map[3][2] = 0;
        // row 4
        cb->Map[0][3] = 0;
        cb->Map[1][3] = 0;
        cb->Map[2][3] = 0;
        cb->Map[3][3] = 0;
      }else if(cb->Rot == 3){ // 270°
        cb->Map[0][0] = 0;
        cb->Map[1][0] = cb->Typ + 1;
        cb->Map[2][0] = cb->Typ + 1;
        cb->Map[3][0] = 0;
        // row 2
        cb->Map[0][1] = 0;
        cb->Map[1][1] = cb->Typ + 1;
        cb->Map[2][1] = 0;
        cb->Map[3][1] = 0;
        // row 3
        cb->Map[0][2] = 0;
        cb->Map[1][2] = cb->Typ + 1;
        cb->Map[2][2] = 0;
        cb->Map[3][2] = 0;
        // row 4
        cb->Map[0][3] = 0;
        cb->Map[1][3] = 0;
        cb->Map[2][3] = 0;
        cb->Map[3][3] = 0;
      }
    }break;
    case 6:{
      if(cb->Rot == 0){ // 0°
        cb->Map[0][0] = 0;
        cb->Map[1][0] = 0;
        cb->Map[2][0] = cb->Typ + 1;
        cb->Map[3][0] = 0;
        // row 2
        cb->Map[0][1] = cb->Typ + 1;
        cb->Map[1][1] = cb->Typ + 1;
        cb->Map[2][1] = cb->Typ + 1;
        cb->Map[3][1] = 0;
        // row 3
        cb->Map[0][2] = 0;
        cb->Map[1][2] = 0;
        cb->Map[2][2] = 0;
        cb->Map[3][2] = 0;
        // row 4
        cb->Map[0][3] = 0;
        cb->Map[1][3] = 0;
        cb->Map[2][3] = 0;
        cb->Map[3][3] = 0;
      }else if(cb->Rot == 1){ // 90°
        cb->Map[0][0] = 0;
        cb->Map[1][0] = cb->Typ + 1;
        cb->Map[2][0] = 0;
        cb->Map[3][0] = 0;
        // row 2
        cb->Map[0][1] = 0;
        cb->Map[1][1] = cb->Typ + 1;
        cb->Map[2][1] = 0;
        cb->Map[3][1] = 0;
        // row 3
        cb->Map[0][2] = 0;
        cb->Map[1][2] = cb->Typ + 1;
        cb->Map[2][2] = cb->Typ + 1;
        cb->Map[3][2] = 0;
        // row 4
        cb->Map[0][3] = 0;
        cb->Map[1][3] = 0;
        cb->Map[2][3] = 0;
        cb->Map[3][3] = 0;
      }else if(cb->Rot == 2){ // 180°
        cb->Map[0][0] = 0;
        cb->Map[1][0] = 0;
        cb->Map[2][0] = 0;
        cb->Map[3][0] = 0;
        // row 2
        cb->Map[0][1] = cb->Typ + 1;
        cb->Map[1][1] = cb->Typ + 1;
        cb->Map[2][1] = cb->Typ + 1;
        cb->Map[3][1] = 0;
        // row 3
        cb->Map[0][2] = cb->Typ + 1;
        cb->Map[1][2] = 0;
        cb->Map[2][2] = 0;
        cb->Map[3][2] = 0;
        // row 4
        cb->Map[0][3] = 0;
        cb->Map[1][3] = 0;
        cb->Map[2][3] = 0;
        cb->Map[3][3] = 0;
      }else if(cb->Rot == 3){ // 270°
        cb->Map[0][0] = cb->Typ + 1;
        cb->Map[1][0] = cb->Typ + 1;
        cb->Map[2][0] = 0;
        cb->Map[3][0] = 0;
        // row 2
        cb->Map[0][1] = 0;
        cb->Map[1][1] = cb->Typ + 1;
        cb->Map[2][1] = 0;
        cb->Map[3][1] = 0;
        // row 3
        cb->Map[0][2] = 0;
        cb->Map[1][2] = cb->Typ + 1;
        cb->Map[2][2] = 0;
        cb->Map[3][2] = 0;
        // row 4
        cb->Map[0][3] = 0;
        cb->Map[1][3] = 0;
        cb->Map[2][3] = 0;
        cb->Map[3][3] = 0;
      }
    }break;
  }
}

void cGameIrrTris::runGame(){
  progDeath = false;
  gameRunning = false;
  showHighScores = true;
  posNewScore = -1;

  SIrrlichtCreationParameters paramIrr;
	paramIrr.Fullscreen = true;
	paramIrr.AntiAlias = false;
	paramIrr.Bits = 32;
	paramIrr.DriverType = EDT_OPENGL;
	paramIrr.EventReceiver = this;
	paramIrr.Stencilbuffer = false;
	paramIrr.Vsync = true;
	paramIrr.WindowSize = dimension2d<s32>(800, 600);

  device = createDeviceEx(paramIrr);
	driver = device->getVideoDriver();
	device->setWindowCaption(L"IrrTriz");
  device->getCursorControl()->setVisible(false);

	driver->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, true);
	driver->setTextureCreationFlag(ETCF_ALWAYS_16_BIT, false);
  driver->setTextureCreationFlag(ETCF_OPTIMIZED_FOR_QUALITY, true);
	driver->setTextureCreationFlag(ETCF_OPTIMIZED_FOR_SPEED, true);
	driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, false);

  device->getFileSystem()->addZipFileArchive("game.dat");
  loadHighScores();

  imgBack = driver->getTexture("map.bmp");
	env = device->getGUIEnvironment();
	IGUIFont* font = env->getFont("font.bmp");
  txtScore = env->addStaticText(L"0", rect<s32>(650, 275, 799, 320), false, false);
  txtScore->setOverrideFont(font);
  txtScore->setOverrideColor(SColor(255, 0, 190, 190));

	IGUIFont* fontS = env->getFont("font-sh-small.bmp");
	IGUIFont* fontB = env->getFont("font-sh-big.bmp");
	hsView[0] = env->addStaticText(L"", rect<s32>(290, 10, 510, 60), false, false);
  hsView[0]->setOverrideColor(SColor(255, 0, 150, 250));
  hsView[0]->setOverrideFont(fontB);
  for(int t = 1; t < 25; t++){
    hsView[t] = env->addStaticText(L"", rect<s32>(290, t * 20 + 40, 510, t * 20 + 60), false, false);
    hsView[t]->setOverrideColor(SColor(255, 255, 255, 0));
    hsView[t]->setOverrideFont(fontS);
  }

  // make GUI not transparent
  for(s32 i=0; i<EGDC_COUNT ; ++i){
		SColor col = env->getSkin()->getColor((EGUI_DEFAULT_COLOR)i);
		col.setAlpha(255);
		env->getSkin()->setColor((EGUI_DEFAULT_COLOR)i, col);
	}

  // clear map
  for(int x = 0; x < 10; x++)
    for(int y = 0; y < 22; y++)
      Map[x][y] = 0;

	while(device->run()){
		driver->beginScene(true, true, SColor(255,0,110,0));
    driver->draw2DImage(imgBack, rect<s32>(0, 0, 800, 601), rect<s32>(0, 0, 800, 601), 0, 0, false);
    if(gameRunning) updateGame();
    drawMap();
    env->drawAll();
    driver->endScene();
    updateNewHSColor();
		if(progDeath)	device->closeDevice();
		device->sleep(1,0);
	}
	device->drop();
}

void cGameIrrTris::getNextCube(){
  cubeNext.Typ = (rand() % 7);
  cubeNext.Rot = 0;
  cubeNext.Pos = position2d<int>(3, 0);
  getCubeMap(&cubeNext);
}

void cGameIrrTris::getPlayCube(){
  cubeNow = cubeNext;
  getNextCube();
  if((cubeNow.Typ != 1) && (cubeNow.Typ != 6)) cubeNow.Pos.Y = -1;
  if(!checkCubePos()){
    gameRunning = false;
    if(Score > HighScores[24].Score) addToHighScores();
  }
}

void cGameIrrTris::setCubeToMap(){
  static int cubeX, cubeY;
  static wchar_t wcScore[10];

  for(cubeY = 0; cubeY < 4; cubeY++){
    for(cubeX = 0; cubeX < 4; cubeX++){
      if(cubeNow.Map[cubeX][cubeY] != 0){
        Map[cubeX + cubeNow.Pos.X][cubeY + cubeNow.Pos.Y] = cubeNow.Map[cubeX][cubeY];
      }
    }
  }
  // check for complete rows
  static bool rFull;
  for(int y = 21; y >= 0; y--){
    rFull = true;
    for(int x = 0; x < 10; x++){
      if(Map[x][y] == 0){
        rFull = false;
        break;
      }
    }
    if(rFull){
      Score += 100;
      tmrInterval = 1000 - Score / 10;
      if(tmrInterval < 1) tmrInterval = 1;
      swprintf(wcScore, 10, L"%d", Score);
      txtScore->setText(wcScore);
      // delete row
      for(int y2 = y; y2 > 0; y2--){
        for(int x = 0; x < 10; x++){
          Map[x][y2] = Map[x][y2 - 1];
        }
      }
      for(int x = 0; x < 10; x++){
        Map[x][0] = 0;
      }
      y++;
    }
  }
  // get next as play cube
  getPlayCube();
}

void cGameIrrTris::drawMap(){
  static int cubeX, cubeY, ghostX, ghostY;
  // draw next cube
  if(gameRunning){
    for(int y = 0; y < 4; y++){
      for(int x = 0; x < 4; x++){
        if(cubeNext.Map[x][y] != 0){
          driver->draw2DImage(imgBack,
            rect<s32>(629 + x * 26, 77 + y * 26, 654 + x * 26, 102  + y * 26),
            rect<s32>(cubeNext.Typ * 30, 700, cubeNext.Typ * 30 + 26, 726), 0, 0, false);
        }
      }
    }
    // get ghost position
    cubeY = cubeNow.Pos.Y;
    while(checkCubePos()) cubeNow.Pos.Y++;
    ghostY = (cubeNow.Pos.Y - 1) * 26;
    cubeNow.Pos.Y = cubeY;
    // draw current cube and hist ghost
    cubeX = ghostX = 270 + cubeNow.Pos.X * 26;
    cubeY = cubeNow.Pos.Y * 26;
    for(int y = 0; y < 4; y++){
      for(int x = 0; x < 4; x++){
        if(cubeNow.Map[x][y] != 0){
          // ghost stone
          driver->draw2DImage(imgBack,
            rect<s32>(ghostX + x * 26, ghostY + y * 26, ghostX + x * 26 + 25, ghostY + y * 26 + 25),
            rect<s32>(0, 730, 25, 755), 0, 0, false);
          // normal stone
          driver->draw2DImage(imgBack,
            rect<s32>(cubeX + x * 26, cubeY + y * 26, cubeX + x * 26 + 25, cubeY + y * 26 + 25),
            rect<s32>(cubeNow.Typ * 30, 700, cubeNow.Typ * 30 + 26, 726), 0, 0, false);
        }
      }
    }
    // hide "press F2"
    driver->draw2DImage(imgBack, rect<s32>(40, 280, 190, 350), rect<s32>(60, 230, 170, 280), 0, 0, false);
  }
  if(!showHighScores){
    // draw map
    cubeX = 270;
    for(int y = 0; y < 22; y++){
      for(int x = 0; x < 10; x++){
        if(Map[x][y] != 0){
          driver->draw2DImage(imgBack,
            rect<s32>(cubeX + x * 26, y * 26, cubeX + x * 26 + 25, y * 26 + 25),
            rect<s32>((Map[x][y] - 1) * 30, 700, (Map[x][y] - 1) * 30 + 26, 726), 0, 0, false);
        }
      }
    }
  }
  enableHS();
}

bool cGameIrrTris::checkCubePos(){
  static int cubeX, cubeY, mapX, mapY;
  for(cubeY = 0; cubeY < 4; cubeY++){
    for(cubeX = 0; cubeX < 4; cubeX++){
      if(cubeNow.Map[cubeX][cubeY] != 0){
        mapX = cubeX + cubeNow.Pos.X;
        mapY = cubeY + cubeNow.Pos.Y;
        if((mapX < 0) || (mapY < 0)) return false;
        if((mapX > 9) || (mapY > 21)) return false;
        if(Map[mapX][mapY] != 0) return false;
      }
    }
  }
  return true;
}

long getTickCount()
{
    tms tm;
    return times(&tm);
}

void cGameIrrTris::updateGame(){
  static long tmrLast = 0, tmrNow;
 // tmrNow = GetTickCount();
  tmrNow = getTickCount();
  if(tmrNow > (tmrLast + tmrInterval)){
    cubeNow.Pos.Y++;
    if(!checkCubePos()){
      cubeNow.Pos.Y--;
      setCubeToMap();
    }
    tmrLast = tmrNow;
  }
}

void cGameIrrTris::enableHS(){
  static bool hsOld = false;
  if(hsOld == showHighScores) return;
  hsOld = showHighScores;
  for(int t = 0; t < 25; t++){
    hsView[t]->setText(HighScores[t].Name);
    hsView[t]->setVisible(showHighScores);
  }
}

void cGameIrrTris::saveHighScores(){
  char key[10], val[100];
  for(int t = 0; t < 25; t++){
    sprintf(key, "HS%d", t);
    for(int r = 0; HighScores[t].Name[r] != '\0'; r++){
      val[r] = HighScores[t].Name[r];
      val[r + 1] = '\0';
    }
//    WritePrivateProfileString(key, "Name", val, "IrrTriz.hs");
    sprintf(val, "%d", HighScores[t].Score);
 //   WritePrivateProfileString(key, "Score", val, "IrrTriz.hs");
  }
}

void cGameIrrTris::loadHighScores(){
  char retVal[255], key[10], vor[100];
  hsDat hsNew;
  HighScores.clear();
  for(int t = 0; t < 25; t++){
    sprintf(key, "HS%d", t);
    sprintf(vor, "%d - Acki", (25 - t) * 100);
//    GetPrivateProfileString(key, "Name", vor, retVal, 255, "IrrTriz.hs");
 //   for(int r = 0; retVal[r] != '\0'; r++){
 //     hsNew.Name[r] = (unsigned char)retVal[r];
 //     hsNew.Name[r + 1] = '\0';
 //   }
    sprintf(vor, "%d", (25 - t) * 100);
//    GetPrivateProfileString(key, "Score", vor, retVal, 255, "IrrTriz.hs");
    hsNew.Score = atoi(retVal);
    HighScores.push_back(hsNew);
  }
}

void cGameIrrTris::addToHighScores(){
  IGUIWindow* wnd = env->addWindow(rect<s32>(225, 100, 575, 315), true, L"Highscore - New Entry", 0, guiID_HSWin);
  wnd->getElementFromPoint(position2d<s32>(570, 105))->setVisible(false);
  IGUIFont* fontB = env->getFont("font-sh-big.bmp");
  IGUIStaticText* txt = env->addStaticText(
    L"             !!! Congratulations !!!\n"
    L"You have entered the Highscore-List!\n"
    L"          Please enter your name.",
    rect<s32>(10,30,345,110), false, true, wnd);
  txt->setOverrideFont(fontB);
  txt->setOverrideColor(SColor(255, 0, 50, 200));
  IGUIEditBox* box = env->addEditBox(L"Player", rect<s32>(50, 120, 300, 150), true, wnd, guiID_HSTxt);
  box->setOverrideFont(fontB);
  box->setOverrideColor(SColor(255, 0, 50, 100));
  box->setMax(25);
  IGUIButton* btn = env->addButton(rect<s32>(125, 165, 225, 195), wnd, guiID_HSBtn, L"OK");
  btn->setOverrideFont(fontB);
	device->getCursorControl()->setVisible(true);
	device->getCursorControl()->setPosition(400, 300);
  env->setFocus(box);
}

void cGameIrrTris::updateNewHSColor(){
  static s32 r = 0, g = 100, b = 200;
  static s32 step1 = 10, step2 = 20, step3 = 30;
  static SColor cNewHS(255, 0, 0, 255);
  // add a funny color cycle to the new HighScore entry
  if(!showHighScores || (posNewScore < 0)) return;
  r += step1;
  if((r > 255) || (r < 0)){
    step1 = -step1;
    r += step1;
  }
  g += step2;
  if((g > 255) || (g < 0)){
    step2 = -step2;
    g += step2;
  }
  b += step3;
  if((b > 255) || (b < 0)){
    step3 = -step3;
    b += step3;
  }
  cNewHS.setRed(r);
  cNewHS.setGreen(g);
  hsView[posNewScore]->setOverrideColor(cNewHS);
}

