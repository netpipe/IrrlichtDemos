#include "myEventReceiver.h"
/*
	bool OnEvent(const SEvent& event){
  switch(event.EventType){
    // Tastatureingabe
  	case EET_KEY_INPUT_EVENT:{
 	  	if(!event.KeyInput.PressedDown){
  	   	switch(event.KeyInput.Key){
          case KEY_ESCAPE:{
            Player.prgEnd = true;
            return true;
          }break;
          case KEY_F1:{
            Player.useEnemyCam = !Player.useEnemyCam;
            if(!Player.useEnemyCam){
              smgr->setActiveCamera(Player.cam);
              Enemies[Player.selEnemy].node->setVisible(true);
            }else{
              smgr->setActiveCamera(Enemies[Player.selEnemy].cam);
              Enemies[Player.selEnemy].node->setVisible(false);
            }
            return true;
          }break;
          case KEY_F2:{
            Player.showWaypoints = !Player.showWaypoints;
            return true;
          }break;
          case KEY_ADD:{
            Player.speedEnemies += 0.1;
            if(Player.speedEnemies > 10.0) Player.speedEnemies = 10.0;
            return true;
          }break;
          case KEY_KEY_0 :
          case KEY_KEY_1 :
          case KEY_KEY_2 :
          case KEY_KEY_3 :
          case KEY_KEY_4 :
          case KEY_KEY_5 :
          case KEY_KEY_6 :
          case KEY_KEY_7 :
          case KEY_KEY_8 :
          case KEY_KEY_9 :{
            Player.selEnemy = (int)event.KeyInput.Key - (int)KEY_KEY_0;
            return true;
          }break;
          case KEY_SUBTRACT:{
            Player.speedEnemies -= 0.1;
            if(Player.speedEnemies < 0.1) Player.speedEnemies = 0.1;
            return true;
          }break;
        }
	    }
    }break;
	}
  return false;
}
*/
void myEventReceiver::updateText(IGUIStaticText* txt){
  static int selPrev = -1;
  static double speedPrev = 0.0;
  if((speedPrev != Player.speedEnemies) || (selPrev != Player.selEnemy)){
    wchar_t q[100];
    swprintf(q, 100, L"selected: #%d\nspeed: %1.1f\n", Player.selEnemy, Player.speedEnemies);
    txt->setText(q);
    if(Enemies[Player.selEnemy].route.size() > 0){
      Player.bbDest->setPosition(Waypoints[Enemies[Player.selEnemy].route[Enemies[Player.selEnemy].route.size() - 1]].pos + vector3df(0,50,0));
    }
    swprintf(q, 100, L"enemy #%d" ,Player.selEnemy);
    Player.txtSel->setParent(Enemies[Player.selEnemy].node);
    Player.txtSel->setText(q);
    speedPrev = Player.speedEnemies;
    selPrev = Player.selEnemy;
  }
}
