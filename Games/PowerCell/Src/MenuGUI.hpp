#ifndef _MenuGUI_HPP_
#define _MenuGUI_HPP_

#include "Globals.hpp"

class MenuGUI
{
  private:

    ITexture *cell_menubg;
    IGUIButton *btn_stage1, *btn_stage2, *btn_stage3, *btn_stage4;
    IGUIStaticText *m_txtHighscore;

    IrrlichtDevice *irrDevice;

  public:

    static MenuGUI *Instance();

    void Init(void);

    __inline void setDevice(IrrlichtDevice *device)
    {
      irrDevice = device;
    }

  protected:

    static MenuGUI *m_menugui;
    static MenuGUI m_default_menugui_buffer;

};

#endif
