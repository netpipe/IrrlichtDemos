  #include <CHiScore.h>

/**
 * The constructor
 * @param Irrlicht device for the XML reader and writer
 */
HiScore::HiScore(IrrlichtDevice *pDevice) {
  m_pDevice=pDevice;
  IXMLReaderUTF8 *pXml=m_pDevice->getFileSystem()->createXMLReaderUTF8("../../data/highscore.xml");
  u32 iCnt=0;

  //If the XML was opened successfully we read the information from the file
  if (pXml) {
    while (pXml->read() && iCnt<10) {
      if (!strcmp(pXml->getNodeName(),"score")) {
        _ENTRY *theEntry=new _ENTRY();
        theEntry->iTime=atoi(pXml->getAttributeValue("time"));
        theEntry->sName=stringw(pXml->getAttributeValue("name"));
        m_lList.push_back(theEntry);
        iCnt++;
      }
    }
    pXml->drop();
  }
}

/**
 * The destructor
 */
HiScore::~HiScore() {
  list<_ENTRY *>::Iterator i;
  for (i=m_lList.begin(); i!=m_lList.end(); i++) delete(*i);
  m_lList.clear();
}

/**
 * This function is used to show the highscore table
 * @param a new time value to be entered to the list. If this param is 0 the highscore list will just be shown, no entry will be added
 */
void HiScore::show(u32 iTime) {
  m_pEdit=NULL;
  dimension2du screenSize=m_pDevice->getVideoDriver()->getScreenSize();
  m_iRet=0;
  u32 iPos=0;
  list<_ENTRY *>::Iterator i;
  IGUIEnvironment *gui=m_pDevice->getGUIEnvironment();
  //create a window to display the table
  rect<s32> winRect(screenSize.Width/2-249,130,screenSize.Width/2+248,screenSize.Height-25);
  IGUIWindow *pWin=gui->addWindow(winRect,true,L"Highscore");

  //should a new entry be added?
  if (iTime) {
    wchar_t ws[0xFF];
    swprintf(ws,0xFE,L"Highscore - Your Time: %.2f",((f32)iTime)/1000);
    pWin->setText(ws);
    m_pEntry=new _ENTRY();
    m_pEntry->iTime=iTime;
    m_pEntry->sName=stringw(L"");
    bool bDone=false;

    //search for the position of the new entry
    for (i=m_lList.begin(); i!=m_lList.end() && iPos<10; i++) {
      if ((*i)->iTime>iTime) {
        m_lList.insert_before(i,m_pEntry);
        bDone=true;
        break;
      }
      iPos++;
    }

    //if no position was found and iPos is zero we have an empty highscore list, so we add the entry as first place
    if (iPos==0 && !bDone)
      m_lList.push_front(m_pEntry);
    else
      //if no position was found and iPos is less than 10 we have a list with less than 10 entries so we
      //add the new entry at the end of the list
      if (iPos<10 && !bDone)
        m_lList.push_back(m_pEntry);
      else
        //new entry not in the highscore table? Then we delete it
        if (!bDone) {
          delete m_pEntry;
          m_pEntry=NULL;
        }

    s32 iBtnY=screenSize.Height-210;
    gui->addButton(rect<s32>(400,iBtnY,470,iBtnY+25),pWin,1,L"Close");
    m_pDevice->setEventReceiver(this);
  }
  else m_pEntry=NULL;

  wchar_t ws[0xFF];
  iPos=0;
  m_pDevice->getCursorControl()->setVisible(true);

  //Now we create static text fields with the entries of the list. If the player managed to get into
  //the highscore list and edit field will be added at that position and it will be focused
  for (i=m_lList.begin(); i!=m_lList.end() && iPos<10; i++) {
    if (*i==m_pEntry) {
      swprintf(ws,0xFE,L"%i",iPos+1);
      gui->addStaticText(ws,rect<s32>(25,25+25*iPos,50,40+25*iPos),false,true,pWin);
      m_pEdit=gui->addEditBox(L"",rect<s32>(55,23+25*iPos,200,42+25*iPos),true,pWin);
      gui->setFocus(m_pEdit);
      swprintf(ws,0xFE,L"%.2f sec",((f32)(*i)->iTime)/1000);
      gui->addStaticText(ws,rect<s32>(225,25+25*iPos,472,40+25*iPos),false,true,pWin);
    }
    else {
      const wchar_t *sName=(*i)->sName.c_str();
      swprintf(ws,0xFE,L"%i: %s (%.2f sec.)",iPos+1,sName,((f32)(*i)->iTime)/1000);
      gui->addStaticText(ws,rect<s32>(25,25+25*iPos,472,40+25*iPos),false,true,pWin);
    }
    iPos++;
  }
}

/**
 * This method saves the highscore table
 */
void HiScore::save() {
  IXMLWriter *pXml=m_pDevice->getFileSystem()->createXMLWriter("../../data/highscore.xml");
  if (pXml) {
    pXml->writeXMLHeader();
    pXml->writeElement(L"HiScore",false);
    pXml->writeLineBreak();
    u32 iCnt=0;
    list<_ENTRY *>::Iterator i;
    //Save the list of highscore entries, but only 10 entries will be saved
    for (i=m_lList.begin(); i!=m_lList.end() && iCnt<10; i++) {
      iCnt++;
      wchar_t s[0xFF];
      swprintf(s,0xFE,L"%i",(*i)->iTime);
      pXml->writeElement(L"score",true,L"time",s,L"name",(*i)->sName.c_str());
      pXml->writeLineBreak();
    }
    pXml->writeClosingTag(L"HiScore");
    pXml->drop();
  }
}

/**
 * This function is called every frame and it just returns a value not equal to "0" if a name was entered or
 * the "ESCAPE" key was pressed. In that case the highscore table will not be saved
 */
int HiScore::update() {
  return m_iRet;
}

/**
 * Event receiver for the Irrlicht event pipeline
 */
bool HiScore::OnEvent(const SEvent &event) {
  if (event.EventType==EET_GUI_EVENT) {
    if (event.GUIEvent.EventType==EGET_BUTTON_CLICKED || event.GUIEvent.EventType==EGET_EDITBOX_ENTER) {
      if (m_pEntry && m_pEdit) {
        m_pEntry->sName=stringw(m_pEdit->getText());
        save();
      }
      m_iRet=1;
    }
  }

  if (event.EventType==EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown && event.KeyInput.Key==KEY_ESCAPE) m_iRet=1;
  return false;
}
