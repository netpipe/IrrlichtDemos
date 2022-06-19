#include "MenuGUI.hpp"

MenuGUI *MenuGUI::m_menugui = NULL;
MenuGUI MenuGUI::m_default_menugui_buffer;

MenuGUI *MenuGUI::Instance()
{
  if(!m_menugui)
    m_menugui = &m_default_menugui_buffer;

  return m_menugui;
}

void MenuGUI::Init(void)
{

//		irrDevice->getFileSystem()->addFileArchive("../Src/media.zip");

  irrDevice->getCursorControl()->setVisible(true);
	IGUISkin* skin = irrDevice->getGUIEnvironment()->getSkin();

	CGUITexturedSkin * pNewSkin = new CGUITexturedSkin(irrDevice->getGUIEnvironment(), irrDevice->getFileSystem());

	pNewSkin->setSkin( "UI/guiskin.xml" );
	irrDevice->getGUIEnvironment()->setSkin(pNewSkin);
	pNewSkin->drop();

	IGUIFont* font = irrDevice->getGUIEnvironment()->getFont("fonts/cellfont1.png");

	if (font)
		pNewSkin->setFont(font, EGDF_DEFAULT);

  pNewSkin->setColor(EGDC_BUTTON_TEXT, SColor(255,6,30,67));

  irrDevice->getVideoDriver()->setTextureCreationFlag(ETCF_OPTIMIZED_FOR_QUALITY, true);
  irrDevice->getVideoDriver()->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, false);

  cell_menubg = irrDevice->getVideoDriver()->getTexture("UI/cell_menubg.jpg");
  irrDevice->getGUIEnvironment()->addImage(cell_menubg, position2d<s32>(0,0));

  irrDevice->getVideoDriver()->setTextureCreationFlag(ETCF_OPTIMIZED_FOR_QUALITY, false);
  irrDevice->getVideoDriver()->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, true);

  u32 x = 400;
  u32 y = 300;
  u32 h = 64;
  u32 w = 256;
  u32 s = 8;

  if(Stage::Instance()->GetUnlocked(0))
    btn_stage1 = irrDevice->getGUIEnvironment()->addButton(rect<s32>(x,y,x+w,y+h),0, BUTTON_MAIN_MENU_STAGE1,L"Stage 1");
  else
    btn_stage1 = irrDevice->getGUIEnvironment()->addButton(rect<s32>(x,y,x+w,y+h),0, -1,L"Locked");

  if(Stage::Instance()->GetUnlocked(1))
    btn_stage2 = irrDevice->getGUIEnvironment()->addButton(rect<s32>(x,y+s+h,x+w,y+s+(h*2)),0, BUTTON_MAIN_MENU_STAGE2,L"Stage 2");
  else
    btn_stage2 = irrDevice->getGUIEnvironment()->addButton(rect<s32>(x,y+s+h,x+w,y+s+(h*2)),0, -1,L"Locked");

  if(Stage::Instance()->GetUnlocked(2))
    btn_stage3 = irrDevice->getGUIEnvironment()->addButton(rect<s32>(x,y+(s*2)+(h*2),x+w,y+(s*2)+(h*3)),0, BUTTON_MAIN_MENU_STAGE3,L"Stage 3");
  else
    btn_stage3 = irrDevice->getGUIEnvironment()->addButton(rect<s32>(x,y+(s*2)+(h*2),x+w,y+(s*2)+(h*3)),0, -1,L"Locked");

  if(Stage::Instance()->GetUnlocked(3))
    btn_stage4 = irrDevice->getGUIEnvironment()->addButton(rect<s32>(x,y+(s*3)+(h*3),x+w,y+(s*3)+(h*4)),0, BUTTON_MAIN_MENU_STAGE4,L"Stage 4");
  else
    btn_stage4 = irrDevice->getGUIEnvironment()->addButton(rect<s32>(x,y+(s*3)+(h*3),x+w,y+(s*3)+(h*4)),0, -1,L"Locked");

	// Create highscore static text.
	stringc score = L"";
	score += Highscore::Instance()->GetTopHighScoreAsString(0);
	score += Highscore::Instance()->GetTopHighScoreAsString(1);
	score += Highscore::Instance()->GetTopHighScoreAsString(2);
	score += Highscore::Instance()->GetTopHighScoreAsString(3);
	score += L"\0";
 // scene::IAnimatedMesh* mesh = irrDevice->getSceneManager()->getMesh("IrrlichtLogo.b3d");
 // mesh->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
	// Convert to wide string.
	// FIXME (lightsoda#1#): Should be made into a function. String conversion class perhaps?	s32 len = score.size();
	wchar_t *buf = new wchar_t[len];
	mbstowcs(buf, score.c_str(), len);
	stringw wstr = buf;
	delete[] buf;

	m_txtHighscore = irrDevice->getGUIEnvironment()->addStaticText(wstr.c_str(), rect<s32>(244,150,800,260), false, true, 0, -1, true);
}
