  #include "CLevelData.h"
  #include "CGame.h"
  #include "CMenu.h"
  #include "CMainMenu.h"

int main(int argc, char *argv[])
{
CGame      *theGame;
CMenu       cMenu;
int         iLevelTime=0,iState=0;

irr::IrrlichtDevice      *pDevice  =NULL;
irr::IEventReceiver      *pReceiver=NULL;
//irr::audio::ISoundEngine *pSound;

  if (cMenu.showMenu(iLevelTime)==4)
  {
    pDevice=irr::createDevice(cMenu.getDriver(), irr::core::dimension2du(cMenu.getResX(),cMenu.getResY()), 16,cMenu.getFullscreen(), true, true,NULL);
    pReceiver=pDevice->getEventReceiver();
//    pSound=irr::audio::createIrrKlangDevice();

    theGame=new CGame(pDevice);
    theGame->getDevice()->setWindowCaption(L"BulletByte Marbles");

    while ((iState=theGame->showMenu())!=6)
    {
      switch (iState)
      {
        case 2:
          iLevelTime=theGame->play(theGame->getSelectedFile(),0);
          break;

        case 3:
          iLevelTime=theGame->play(theGame->getSelectedFile(),1);
          break;
      }
    }
    free(theGame);

   // pSound->drop();
    pDevice->drop();
  }

	return 0;
}
