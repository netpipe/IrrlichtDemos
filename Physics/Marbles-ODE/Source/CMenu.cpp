//  #include <irrXml.h>

  #include "CMenu.h"

CMenu::CMenu()
{
  m_iStartedLevel=-1;
  m_iResult=0;
  m_bRefreshHiScore=false;
}

CMenu::~CMenu()
{
}

void CMenu::saveHiScore(CLevelInfo *pLvl)
{
FILE *f;
int   i;
char  s[100],*s2;

  strcpy(s,pLvl->sFile);
  s2=strrchr(s,'.');

  if (s2)
  {
    *s2='\0';

    if ((f=fopen(s,"wt"))!=NULL)
    {
      for (i=0; i<10; i++) fprintf(f,"%s=%i\n",pLvl->cHiScore[i].sName,pLvl->cHiScore[i].iTime);
      fclose(f);
    }
  }
}

void CMenu::saveSetup()
{
FILE *f;

  if ((f=fopen("data/setup.xml","wb"))!=NULL)
  {
    fprintf(f,"<marbles>\n");
    fprintf(f,"  <driver value=\"%i\" />\n",m_pVid->getSelected());
    fprintf(f,"  <resolution value=\"%i\" />\n",m_pRes->getSelected());
    fprintf(f,"  <fullscreen value=\"%i\" />\n",m_pFs->isChecked());
    fprintf(f,"</marbles>\n");
    fclose(f);
  }
}

void CMenu::loadSetup()
{
irr::io::IXMLReader *pXml=m_pDevice->getFileSystem()->createXMLReader("data/setup.xml");

  if (pXml==NULL) return;

  while (pXml->read())
  {
    if (!wcscmp(pXml->getNodeName(),L"driver"    )) m_pVid->setSelected(pXml->getAttributeValueAsInt(L"value"));
    if (!wcscmp(pXml->getNodeName(),L"resolution")) m_pRes->setSelected(pXml->getAttributeValueAsInt(L"value"));
    if (!wcscmp(pXml->getNodeName(),L"fullscreen")) m_pFs->setChecked((bool)(pXml->getAttributeValueAsInt(L"value")));
  }

  pXml->drop();
}

void CMenu::fillHiScore(CLevelTop10 *top10, irr::gui::IGUIListBox *pList)
{
int i;
wchar_t s[100];

  pList->clear();

  for (i=0; i<10; i++)
  {
//    swprintf(s,L"%s: %.3f",irr::core::stringw(top10[i].sName).c_str(),((float)top10[i].iTime)/1000);
    pList->addItem(s);
  }
}

int CMenu::showMenu(int iLevelTime)
{
 const wchar_t cHiScoreText[4][80]={
                                    L"Congratulations",
                                    L"You have made it into the HiScores of the Level",
                                    L"Your Position: %i",
                                    L"Please enter your name:"
                                  };
irr::gui::  IGUIEnvironment *pGui;
irr::gui::  IGUIFont        *pFont;
irr::gui::  IGUIButton      *pBtnOk,*pBtnCancel;
irr::gui::  IGUIImage       *pBb;
irr::video::IVideoDriver    *pDrv;

  m_iResult=0;

  m_pDevice=createDevice(irr::video::EDT_SOFTWARE,irr::core::dimension2du(320,200),16,false,false,false,this);
  pGui=m_pDevice->getGUIEnvironment();
  pDrv=m_pDevice->getVideoDriver();

  m_pDevice->setWindowCaption(L"BulletByte \"Marbles\" - Setup");

  pFont=pGui->getFont("data/fontcourier.bmp");
  pGui->getSkin()->setFont(pFont);

  pBb=pGui->addImage(pDrv->getTexture("data/logo_b2.bmp"),irr::core::position2d<irr::s32>(215,153));

  pBtnOk    =pGui->addButton(irr::core::rect<irr::s32>( 20,105,120,125),NULL,4,L"Play" );
  pBtnCancel=pGui->addButton(irr::core::rect<irr::s32>(125,105,205,125),NULL,5,L"Exit");

  pGui->addStaticText(L"3D Driver",irr::core::rect<irr::s32>(20,20,100,40),false,false,NULL);
  m_pVid=pGui->addComboBox(irr::core::rect<irr::s32>(110,20,210,40),NULL,1);

  m_pVid->addItem(L"Software");
  m_pVid->addItem(L"DirectX8");
  m_pVid->addItem(L"DirectX9");
  m_pVid->addItem(L"OpenGL");

  pGui->addStaticText(L"Resolution",irr::core::rect<irr::s32>(20,50,100,70),false,false,NULL);
  m_pRes=pGui->addComboBox(irr::core::rect<irr::s32>(110,50,210,70),NULL,2);
  m_pRes->addItem(L"640x480");
  m_pRes->addItem(L"800x600");
  m_pRes->addItem(L"1024x768");
  m_pRes->addItem(L"1280x1024");

  pGui->addStaticText(L"Fullscreen",irr::core::rect<irr::s32>(20,80,100,100),false,false,NULL);
  m_pFs=pGui->addCheckBox(false,irr::core::rect<irr::s32>(110,80,210,100),NULL,3);

  loadSetup();


  while (m_pDevice->run() && m_iResult!=4 && m_iResult!=5)
  {
    pDrv->beginScene(true, true, irr::video::SColor(0,192,192,192));

    pGui->drawAll();

    pDrv->endScene();
  }

  m_bFullscreen=m_pFs->isChecked();

  switch (m_pVid->getSelected())
  {
    case 0: m_iDriver=irr::video::EDT_BURNINGSVIDEO; break;
 //   case 1: m_iDriver=irr::video::EDT_DIRECT3D8    ; break;
    case 2: m_iDriver=irr::video::EDT_DIRECT3D9    ; break;
    case 3: m_iDriver=irr::video::EDT_OPENGL       ; break;
  }

  switch (m_pRes->getSelected())
  {
    case 0: m_iResX= 640; m_iResY= 480; break;
    case 1: m_iResX= 800; m_iResY= 600; break;
    case 2: m_iResX=1024; m_iResY= 768; break;
    case 3: m_iResX=1280; m_iResY=1024; break;
  }

  if (m_iResult==4) saveSetup();

  m_pDevice->closeDevice();
  return m_iResult;
}

bool CMenu::OnEvent(const irr::SEvent &event)
{
  if (event.EventType==irr::EET_GUI_EVENT)
  {
    if (event.GUIEvent.Caller)
    {
       irr::s32 id=event.GUIEvent.Caller->getID();

      if (event.GUIEvent.EventType==irr::gui::EGET_BUTTON_CLICKED) m_iResult=id;
      if (event.GUIEvent.EventType==irr::gui::EGET_LISTBOX_CHANGED && id==7) m_bRefreshHiScore=true;
    }
  }

  return false;
}
