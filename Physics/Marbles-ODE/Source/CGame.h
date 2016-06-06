#ifndef _IGAME
  #define _IGAME

  #include <irrlicht.h>
//  #include <irrklang.h>
  #include "CLevelData.h"

class CGame : public irr::IEventReceiver
{
  private:
    irr::       IrrlichtDevice  *m_pDevice;
	  irr::video::IVideoDriver    *m_pDriver;
  	irr::scene::ISceneManager   *m_pSmgr;
	  irr::gui::  IGUIEnvironment *m_pGuienv;
    irr::gui::  ICursorControl  *m_pCursor;
    irr::       ITimer          *m_pTimer;
    irr::gui::  IGUIFont        *m_pFont,*m_pSmallFont;
    irr::gui::  IGUIListBox     *m_pHiScores;
    irr::gui::  IGUIEditBox     *m_pHiScoreName;
    irr::f32                     m_fCamDist;

    bool m_bLeftKey, m_bRghtKey;
    bool m_bRespawn;
    bool m_bPlaying;
    bool m_bEnterHiScore;

    int m_iResult,m_iSelectedLevel,m_iLevelTime;

    char m_sName[20];

    CLevels m_cLevels;

    void fillHiScoreTable();

  public:
	  CGame(irr::IrrlichtDevice *pDevice);

	  irr::       IrrlichtDevice  *getDevice() { return m_pDevice; }
	  irr::video::IVideoDriver    *getDriver() { return m_pDriver; }
	  irr::scene::ISceneManager   *getSmgr  () { return m_pSmgr  ; }
	  irr::gui  ::IGUIEnvironment *getGuiEnv() { return m_pGuienv; }
	  irr::gui  ::ICursorControl  *getCursor() { return m_pCursor; }
	  irr::       ITimer          *getTimer () { return m_pTimer ; }

//    irr::audio::ISoundEngine    *m_pSound;

	  void beginScene();

	  void hideCursor();
	  void showCursor();

    int getSelectedLevel() { return m_iSelectedLevel/10; }
	  int play(char *sLevelFileName, int iPlayMode);
	  int showMenu();

    char *getSelectedFile() { return m_cLevels.m_pLevels[getSelectedLevel()].sFile; }

	  virtual bool OnEvent(const irr::SEvent &event);
};

#endif
