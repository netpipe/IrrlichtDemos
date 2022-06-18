#include "GameGUI.hpp"

GameGUI *GameGUI::m_gamegui = NULL;
GameGUI GameGUI::m_default_gamegui_buffer;

GameGUI *GameGUI::Instance()
{
  if(!m_gamegui)
    m_gamegui = &m_default_gamegui_buffer;

  return m_gamegui;
}

void GameGUI::Init(void)
{
  skin = irrDevice->getGUIEnvironment()->getSkin();
  skin->setColor(EGDC_BUTTON_TEXT, SColor(255,240,250,0));

  font = irrDevice->getGUIEnvironment()->getFont("fonts/cellfont1.png");
  if (font)
    skin->setFont(font);

  //guienv->addImage(driver->getTexture("cellicon.tga"),	position2d<int>(600-128,30));

  cell_hud = irrDevice->getVideoDriver()->getTexture("UI/cell_hud.tga");

  s32 x = 400 - (cell_hud->getOriginalSize().Width / 2);
  s32 y = 600 - (cell_hud->getOriginalSize().Height);

  bool showFrame = true;

  irrDevice->getGUIEnvironment()->addImage(cell_hud, position2d<s32>(x,y));

  u32 top = 560;
  u32 height = 25;

  timeDisplay = irrDevice->getGUIEnvironment()->addStaticText(L"Time",
    rect<s32>(168,top,310,top+height), showFrame);

  healthDisplay = irrDevice->getGUIEnvironment()->addStaticText(L"",
    rect<s32>(495,top,630,top+height), showFrame);

  powerCellCountCurrent = irrDevice->getGUIEnvironment()->addStaticText(L"",
    rect<s32>(316,top,395,top+height), showFrame);

  powerCellCountTotal = irrDevice->getGUIEnvironment()->addStaticText(L"",
    rect<s32>(406,top,485,top+height), showFrame);
}

void GameGUI::updatePowerCellInfo(u32 aquired, u32 total)
{
	str = L"";
  str += aquired;
  powerCellCountCurrent->setText(str.c_str());
  str = L"";
  str += total;
  powerCellCountTotal->setText(str.c_str());
}

void GameGUI::updateTimeInfo(u32 time)
{
 // Output::Instance()->w("Updating time info\n");
 //Output::Instance()->w("Time: %i\n", time);
  str = L"Time: ";
  str += time;

  timeDisplay->setText(str.c_str());
}

void GameGUI::updateHealthInfo(u32 health)
{
  str = L"Health: ";
  str += health;

  healthDisplay->setText(str.c_str());
}

IGUIFont *GameGUI::GetFont(void)
{
  return font;
}
