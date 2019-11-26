#ifndef _GameGUI_HPP_
#define _GameGUI_HPP_

#include "Globals.hpp"

class GameGUI
{
  private:

    IGUISkin *skin;
    IGUIFont *font;
    IGUIStaticText *powerCellCountTotal;
    IGUIStaticText *powerCellCountCurrent;
    IGUIStaticText *timeDisplay;
    IGUIStaticText *nameDisplay;
    IGUIStaticText *healthDisplay;

    ITexture *cell_hud;

    stringw str;

    IrrlichtDevice *irrDevice;

  public:

    static GameGUI *Instance();

    void Init(void);
    void updatePowerCellInfo(u32 aquired, u32 total);
    void updateTimeInfo(u32 time);
    void updateHealthInfo(u32 health);
    IGUIFont *GetFont(void);

    __inline void setDevice(IrrlichtDevice *device)
    {
      irrDevice = device;
    }

  protected:

    static GameGUI *m_gamegui;
    static GameGUI m_default_gamegui_buffer;


};

#endif
