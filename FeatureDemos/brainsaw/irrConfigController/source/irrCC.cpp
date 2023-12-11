  #include <irrCC.h>

CIrrCC::CIrrCCItem::CIrrCCItem(stringw sName, IrrlichtDevice *pDevice, u32 iSet) {
  m_pDevice=pDevice;
  m_sName=sName;
  m_iSet=iSet;
  m_iType=CC_TYPE_NUL;
  m_eKey=KEY_KEY_CODES_COUNT;
  m_eType=eCtrlAxis;

  m_pGuiEnv=m_pDevice->getGUIEnvironment();
  m_bConflict=false;
  m_fValue=0.0f;
  m_pCrsCtrl=pDevice->getCursorControl();
  m_fMouseSensitivity=10.0f;
  m_pCtrlText=NULL;
  m_pBrother=NULL;

  m_iFaderStep=0;
  m_iFaderLastStep=0;
  m_fFaderDiff=0.0f;
  m_fFaderValue=0.0f;
}

CIrrCC::CIrrCCItem::~CIrrCCItem() {
}

bool CIrrCC::CIrrCCItem::isButton() {
  return m_eType==CIrrCC::eCtrlButton || m_eType==CIrrCC::eCtrlToggleButton;
}

//should use  map and case instead to boost speed instead of if
stringw CIrrCC::CIrrCCItem::keyCodeToString(EKEY_CODE e) {
  if (e == KEY_LBUTTON)    return stringw(L"KEY_LBUTTON");
  if (e == KEY_RBUTTON)    return stringw(L"KEY_RBUTTON");
  if (e == KEY_CANCEL)     return stringw(L"KEY_CANCEL");
  if (e == KEY_MBUTTON)    return stringw(L"KEY_MBUTTON");
  if (e == KEY_XBUTTON1)   return stringw(L"KEY_XBUTTON1");
  if (e == KEY_XBUTTON2)   return stringw(L"KEY_XBUTTON2");
  if (e == KEY_BACK)       return stringw(L"KEY_BACK");
  if (e == KEY_TAB)        return stringw(L"KEY_TAB");
  if (e == KEY_CLEAR)      return stringw(L"KEY_CLEAR");
  if (e == KEY_RETURN)     return stringw(L"KEY_RETURN");
  if (e == KEY_SHIFT)      return stringw(L"KEY_SHIFT");
  if (e == KEY_CONTROL)    return stringw(L"KEY_CONTROL");
  if (e == KEY_MENU)       return stringw(L"KEY_MENU");
  if (e == KEY_PAUSE)      return stringw(L"KEY_PAUSE");
  if (e == KEY_CAPITAL)    return stringw(L"KEY_CAPITAL");
  if (e == KEY_KANA)       return stringw(L"KEY_KANA");
  if (e == KEY_HANGUEL)    return stringw(L"KEY_HANGUEL");
  if (e == KEY_HANGUL)     return stringw(L"KEY_HANGUL");
  if (e == KEY_JUNJA)      return stringw(L"KEY_JUNJA");
  if (e == KEY_FINAL)      return stringw(L"KEY_FINAL");
  if (e == KEY_HANJA)      return stringw(L"KEY_HANJA");
  if (e == KEY_KANJI)      return stringw(L"KEY_KANJI");
  if (e == KEY_ESCAPE)     return stringw(L"KEY_ESCAPE");
  if (e == KEY_CONVERT)    return stringw(L"KEY_CONVERT");
  if (e == KEY_NONCONVERT) return stringw(L"KEY_NONCONVERT");
  if (e == KEY_ACCEPT)     return stringw(L"KEY_ACCEPT");
  if (e == KEY_MODECHANGE) return stringw(L"KEY_MODECHANGE");
  if (e == KEY_SPACE)      return stringw(L"KEY_SPACE");
  if (e == KEY_PRIOR)      return stringw(L"KEY_PRIOR");
  if (e == KEY_NEXT)       return stringw(L"KEY_NEXT");
  if (e == KEY_END)        return stringw(L"KEY_END");
  if (e == KEY_HOME)       return stringw(L"KEY_HOME");
  if (e == KEY_LEFT)       return stringw(L"KEY_LEFT");
  if (e == KEY_UP)         return stringw(L"KEY_UP");
  if (e == KEY_RIGHT)      return stringw(L"KEY_RIGHT");
  if (e == KEY_DOWN)       return stringw(L"KEY_DOWN");
  if (e == KEY_SELECT)     return stringw(L"KEY_SELECT");
  if (e == KEY_PRINT)      return stringw(L"KEY_PRINT");
  if (e == KEY_EXECUT)     return stringw(L"KEY_EXECUT");
  if (e == KEY_SNAPSHOT)   return stringw(L"KEY_SNAPSHOT");
  if (e == KEY_INSERT)     return stringw(L"KEY_INSERT");
  if (e == KEY_DELETE)     return stringw(L"KEY_DELETE");
  if (e == KEY_HELP)       return stringw(L"KEY_HELP");
  if (e == KEY_KEY_0)      return stringw(L"KEY_KEY_0");
  if (e == KEY_KEY_1)      return stringw(L"KEY_KEY_1");
  if (e == KEY_KEY_2)      return stringw(L"KEY_KEY_2");
  if (e == KEY_KEY_3)      return stringw(L"KEY_KEY_3");
  if (e == KEY_KEY_4)      return stringw(L"KEY_KEY_4");
  if (e == KEY_KEY_5)      return stringw(L"KEY_KEY_5");
  if (e == KEY_KEY_6)      return stringw(L"KEY_KEY_6");
  if (e == KEY_KEY_7)      return stringw(L"KEY_KEY_7");
  if (e == KEY_KEY_8)      return stringw(L"KEY_KEY_8");
  if (e == KEY_KEY_9)      return stringw(L"KEY_KEY_9");
  if (e == KEY_KEY_A)      return stringw(L"KEY_KEY_A");
  if (e == KEY_KEY_B)      return stringw(L"KEY_KEY_B");
  if (e == KEY_KEY_C)      return stringw(L"KEY_KEY_C");
  if (e == KEY_KEY_D)      return stringw(L"KEY_KEY_D");
  if (e == KEY_KEY_E)      return stringw(L"KEY_KEY_E");
  if (e == KEY_KEY_F)      return stringw(L"KEY_KEY_F");
  if (e == KEY_KEY_G)      return stringw(L"KEY_KEY_G");
  if (e == KEY_KEY_H)      return stringw(L"KEY_KEY_H");
  if (e == KEY_KEY_I)      return stringw(L"KEY_KEY_I");
  if (e == KEY_KEY_J)      return stringw(L"KEY_KEY_J");
  if (e == KEY_KEY_K)      return stringw(L"KEY_KEY_K");
  if (e == KEY_KEY_L)      return stringw(L"KEY_KEY_L");
  if (e == KEY_KEY_M)      return stringw(L"KEY_KEY_M");
  if (e == KEY_KEY_N)      return stringw(L"KEY_KEY_N");
  if (e == KEY_KEY_O)      return stringw(L"KEY_KEY_O");
  if (e == KEY_KEY_P)      return stringw(L"KEY_KEY_P");
  if (e == KEY_KEY_Q)      return stringw(L"KEY_KEY_Q");
  if (e == KEY_KEY_R)      return stringw(L"KEY_KEY_R");
  if (e == KEY_KEY_S)      return stringw(L"KEY_KEY_S");
  if (e == KEY_KEY_T)      return stringw(L"KEY_KEY_T");
  if (e == KEY_KEY_U)      return stringw(L"KEY_KEY_U");
  if (e == KEY_KEY_V)      return stringw(L"KEY_KEY_V");
  if (e == KEY_KEY_W)      return stringw(L"KEY_KEY_W");
  if (e == KEY_KEY_X)      return stringw(L"KEY_KEY_X");
  if (e == KEY_KEY_Y)      return stringw(L"KEY_KEY_Y");
  if (e == KEY_KEY_Z)      return stringw(L"KEY_KEY_Z");
  if (e == KEY_LWIN)       return stringw(L"KEY_LWIN");
  if (e == KEY_RWIN)       return stringw(L"KEY_RWIN");
  if (e == KEY_APPS)       return stringw(L"KEY_APPS");
  if (e == KEY_SLEEP)      return stringw(L"KEY_SLEEP");
  if (e == KEY_NUMPAD0)    return stringw(L"KEY_NUMPAD0");
  if (e == KEY_NUMPAD1)    return stringw(L"KEY_NUMPAD1");
  if (e == KEY_NUMPAD2)    return stringw(L"KEY_NUMPAD2");
  if (e == KEY_NUMPAD3)    return stringw(L"KEY_NUMPAD3");
  if (e == KEY_NUMPAD4)    return stringw(L"KEY_NUMPAD4");
  if (e == KEY_NUMPAD5)    return stringw(L"KEY_NUMPAD5");
  if (e == KEY_NUMPAD6)    return stringw(L"KEY_NUMPAD6");
  if (e == KEY_NUMPAD7)    return stringw(L"KEY_NUMPAD7");
  if (e == KEY_NUMPAD8)    return stringw(L"KEY_NUMPAD8");
  if (e == KEY_NUMPAD9)    return stringw(L"KEY_NUMPAD9");
  if (e == KEY_MULTIPLY)   return stringw(L"KEY_MULTIPLY");
  if (e == KEY_ADD)        return stringw(L"KEY_ADD");
  if (e == KEY_SEPARATOR)  return stringw(L"KEY_SEPARATOR");
  if (e == KEY_SUBTRACT)   return stringw(L"KEY_SUBTRACT");
  if (e == KEY_DECIMAL)    return stringw(L"KEY_DECIMAL");
  if (e == KEY_DIVIDE)     return stringw(L"KEY_DIVIDE");
  if (e == KEY_F1)         return stringw(L"KEY_F1");
  if (e == KEY_F2)         return stringw(L"KEY_F2");
  if (e == KEY_F3)         return stringw(L"KEY_F3");
  if (e == KEY_F4)         return stringw(L"KEY_F4");
  if (e == KEY_F5)         return stringw(L"KEY_F5");
  if (e == KEY_F6)         return stringw(L"KEY_F6");
  if (e == KEY_F7)         return stringw(L"KEY_F7");
  if (e == KEY_F8)         return stringw(L"KEY_F8");
  if (e == KEY_F9)         return stringw(L"KEY_F9");
  if (e == KEY_F10)        return stringw(L"KEY_F10");
  if (e == KEY_F11)        return stringw(L"KEY_F11");
  if (e == KEY_F12)        return stringw(L"KEY_F12");
  if (e == KEY_F13)        return stringw(L"KEY_F13");
  if (e == KEY_F14)        return stringw(L"KEY_F14");
  if (e == KEY_F15)        return stringw(L"KEY_F15");
  if (e == KEY_F16)        return stringw(L"KEY_F16");
  if (e == KEY_F17)        return stringw(L"KEY_F17");
  if (e == KEY_F18)        return stringw(L"KEY_F18");
  if (e == KEY_F19)        return stringw(L"KEY_F19");
  if (e == KEY_F20)        return stringw(L"KEY_F20");
  if (e == KEY_F21)        return stringw(L"KEY_F21");
  if (e == KEY_F22)        return stringw(L"KEY_F22");
  if (e == KEY_F23)        return stringw(L"KEY_F23");
  if (e == KEY_F24)        return stringw(L"KEY_F24");
  if (e == KEY_NUMLOCK)    return stringw(L"KEY_NUMLOCK");
  if (e == KEY_SCROLL)     return stringw(L"KEY_SCROLL");
  if (e == KEY_LSHIFT)     return stringw(L"KEY_LSHIFT");
  if (e == KEY_RSHIFT)     return stringw(L"KEY_RSHIFT");
  if (e == KEY_LCONTROL)   return stringw(L"KEY_LCONTROL");
  if (e == KEY_RCONTROL)   return stringw(L"KEY_RCONTROL");
  if (e == KEY_LMENU)      return stringw(L"KEY_LMENU");
  if (e == KEY_RMENU)      return stringw(L"KEY_RMENU");
  if (e == KEY_PLUS)       return stringw(L"KEY_PLUS");
  if (e == KEY_COMMA)      return stringw(L"KEY_COMMA");
  if (e == KEY_MINUS)      return stringw(L"KEY_MINUS");
  if (e == KEY_PERIOD)     return stringw(L"KEY_PERIOD");
  if (e == KEY_ATTN)       return stringw(L"KEY_ATTN");
  if (e == KEY_CRSEL)      return stringw(L"KEY_CRSEL");
  if (e == KEY_EXSEL)      return stringw(L"KEY_EXSEL");
  if (e == KEY_EREOF)      return stringw(L"KEY_EREOF");
  if (e == KEY_PLAY)       return stringw(L"KEY_PLAY");
  if (e == KEY_ZOOM)       return stringw(L"KEY_ZOOM");
  if (e == KEY_PA1)        return stringw(L"KEY_PA1");
  if (e == KEY_OEM_CLEAR)  return stringw(L"KEY_OEM_CLEAR");
  return stringw(L"UNKNOWN");
}

IGUITab *CIrrCC::CIrrCCItem::getGuiElement(IGUIElement *pParent, position2di cPos, position2di cEditPos) {
  IGUIFont *pFont=m_pGuiEnv->getSkin()->getFont();
  dimension2du dim=pFont->getDimension(m_sName.c_str());

  position2di ulc(cPos.X,cPos.Y),lrc(cPos.Y+dim.Width+cEditPos.X+250,cPos.Y+dim.Height+10);

  m_pGuiElement=m_pGuiEnv->addTab(rect<s32>(ulc.X,ulc.Y,lrc.X,lrc.Y),pParent,-1);
  m_pGuiEnv->addStaticText(m_sName.c_str(),rect<s32>(0,0,dim.Width+10,dim.Height+10),false,true,m_pGuiElement);
  m_pCtrlText=m_pGuiEnv->addStaticText(L"Not set",rect<s32>(cEditPos.X,cEditPos.Y,cEditPos.X+200,cEditPos.Y+dim.Height+2),true,true,m_pGuiElement,65536+m_iIdx);
  this->updateCtrlText();
  return m_pGuiElement;
}

void CIrrCC::CIrrCCItem::setKey(EKEY_CODE e) {
  m_eKey=e;
  m_iType=CC_TYPE_KEY;
}

void CIrrCC::CIrrCCItem::setMouseMove(u32 iAxis, s32 iDirection) {
  m_iMouseAxis=iAxis;
  m_iMouseDirection=iDirection;
  m_iType=CC_TYPE_MOU;
}

void CIrrCC::CIrrCCItem::setMouseButton(u32 iBtn) {
  m_iMouseBtn=iBtn;
  m_iType=CC_TYPE_MBT;
}

void CIrrCC::CIrrCCItem::setJoyAxis(u8 id, u32 iAxis, s32 iDirection) {
  m_iJoyId=id;
  m_iJoyAxis=iAxis;
  m_iJoyDirection=iDirection;
  m_iType=CC_TYPE_JOY;
}

void CIrrCC::CIrrCCItem::setJoyButton(u8 id, u32 iBtn) {
  m_iJoyId=id;
  m_iJoyButton=iBtn;
  m_iType=CC_TYPE_JBT;
}

void CIrrCC::CIrrCCItem::setPov(u8 id, u16 iPov) {
  m_iPov=iPov;
  m_iJoyId=id;
  m_iType=CC_TYPE_POV;
}

IGUIStaticText *CIrrCC::CIrrCCItem::getCtrlText() {
  return m_pCtrlText;
}

bool CIrrCC::CIrrCCItem::hasConflict() {
  return m_bConflict;
}

void CIrrCC::CIrrCCItem::setConflict(bool b) {
  m_bConflict=b;
}

void CIrrCC::CIrrCCItem::updateCtrlText() {
  if (!m_pCtrlText) return;
  wchar_t s[0xFF];

  switch (m_iType) {
    case CC_TYPE_KEY:
      m_pCtrlText->setText(keyCodeToString(m_eKey).c_str());
      break;

    case CC_TYPE_MOU:
      swprintf(s,0xFF,L"Mouse %s %s",m_iMouseAxis==0?L"X":L"Y",m_iMouseDirection>0?L"+":L"-");
      m_pCtrlText->setText(s);
      break;

    case CC_TYPE_MBT:
      swprintf(s,0xFF,L"Mouse Button %i",m_iMouseBtn);
      m_pCtrlText->setText(s);
      break;

    case CC_TYPE_JOY:
      swprintf(s,0xFF,L"Joy %i Axis %i %s",m_iJoyId,m_iJoyAxis,m_iJoyDirection>0?L"+":L"-");
      m_pCtrlText->setText(s);
      break;

    case CC_TYPE_JBT:
      swprintf(s,0xFF,L"Joy %i Button %i",m_iJoyId,m_iJoyButton);
      m_pCtrlText->setText(s);
      break;

    case CC_TYPE_POV:
      swprintf(s,0xFF,L"Joy %i POV %u",m_iJoyId,m_iPov/100);
      m_pCtrlText->setText(s);
      break;

    default:
      break;
  }
}

bool CIrrCC::CIrrCCItem::conflicts(CIrrCCItem *pOther) {
  bool bRet=m_iType==pOther->m_iType;

  if (bRet) {
    switch (m_iType) {
      case CC_TYPE_KEY:
        bRet=m_eKey==pOther->m_eKey;
        break;

      case CC_TYPE_MOU:
        bRet=m_iMouseAxis==pOther->m_iMouseAxis && m_iMouseDirection==pOther->m_iMouseDirection;
        break;

      case CC_TYPE_MBT:
        bRet=m_iMouseBtn==pOther->m_iMouseBtn;
        break;

      case CC_TYPE_JOY:
        bRet=m_iJoyId==pOther->m_iJoyId && m_iJoyAxis==pOther->m_iJoyAxis && m_iJoyDirection==pOther->m_iJoyDirection;
        break;

      case CC_TYPE_JBT:
        bRet=m_iJoyId==pOther->m_iJoyId && m_iJoyButton==pOther->m_iJoyButton;
        break;

      case CC_TYPE_POV:
        bRet=m_iJoyId==pOther->m_iJoyId && m_iPov==pOther->m_iPov;
        break;

      default:
        break;
    }
  }

  return bRet;
}

void CIrrCC::CIrrCCItem::setMouseSensitivity(f32 f) {
  m_fMouseSensitivity=f;
}

void CIrrCC::CIrrCCItem::updateMouse() {
  m_fValue=0.0f;
  position2di pos=m_pCrsCtrl->getPosition();
  if (m_iMouseAxis==0) {
    if (m_iMouseDirection>0 && pos.X>320) {
      m_fValue=((f32)pos.X-320)/m_fMouseSensitivity;
      m_fDetectedValue=m_fValue;
      pos.X=320;
    }
    if (m_iMouseDirection<0 && pos.X<320) {
      m_fValue=((f32)320-pos.X)/m_fMouseSensitivity;
      m_fDetectedValue=m_fValue;
      pos.X=320;
    }
  }

  if (m_iMouseAxis==1) {
    if (m_iMouseDirection>0 && pos.Y>200) {
      m_fValue=((f32)pos.Y-200)/m_fMouseSensitivity;
      m_fDetectedValue=m_fValue;
      pos.Y=200;
    }
    if (m_iMouseDirection<0 && pos.Y<200) {
      m_fValue=((f32)200-pos.Y)/m_fMouseSensitivity;
      m_fDetectedValue=m_fValue;
      pos.Y=200;
    }
  }

  if (m_fValue) m_pCrsCtrl->setPosition(pos);
}

f32 CIrrCC::CIrrCCItem::get() {
  if (m_iType==CC_TYPE_MOU) updateMouse();
  return m_pBrother==NULL?m_fValue:m_fValue-m_pBrother->getRaw();
}

f32 CIrrCC::CIrrCCItem::getRaw() {
  if (m_iType==CC_TYPE_MOU) updateMouse();
  return m_fValue;
}

void CIrrCC::CIrrCCItem::set(f32 f) {
  m_fValue=f;
}

void CIrrCC::CIrrCCItem::setWithDetected(f32 f) {
  if (m_eType==eCtrlToggleButton && f==m_fDetectedValue) return;
  m_fValue=f;
  m_fDetectedValue=f;
}

CIrrCC::CIrrCC(IrrlichtDevice *pDevice) {
  m_pDevice=pDevice;
  m_iGuiItemHeight=pDevice->getGUIEnvironment()->getSkin()->getFont()->getDimension(L"Hello World").Height;
  m_pFocused=NULL;
  m_iFocusedIndex=-1;
  m_pDevice->activateJoysticks(m_aJoysticks);

  for (u32 i=0; i<m_aJoysticks.size(); i++) {
    CJoyState *p=new CJoyState();
    m_aJoyState.push_back(p);
  }

  m_bAllowMouse=true;
  m_bAllowFKeys=true;
  m_bAllowJoystick=true;
  m_bLoaded=false;
  m_pCrsCtrl=m_pDevice->getCursorControl();
  m_fMouseSensitivity=10.0f;
}

CIrrCC::~CIrrCC() {
  for (u32 i=0; i<m_aItems.size(); i++) delete m_aItems[i];
  m_aItems.clear();
  m_bSetsCanConflict=true;
}

u32 CIrrCC::addItem(u32 iSet,stringw sName, EKEY_CODE eDefault, eControllerType eType) {
  u32 iRet=0;

  if (m_aMaxLenText.size()<=iSet)
    m_aMaxLenText.push_back(sName);
  else
    if (m_aMaxLenText[iSet].size()<sName.size())
      m_aMaxLenText[iSet]=sName;

  CIrrCCItem *item=new CIrrCCItem(sName,m_pDevice,iSet);
  item->setKey(eDefault);
  item->m_eType=eType;
  item->setMouseSensitivity(m_fMouseSensitivity);
  m_aItems.push_back(item);

  iRet=m_aItems.size()-1;
	item->m_iIdx=iRet;

  return iRet;
}

void CIrrCC::addGui(u32 iSet, IGUIElement *pParent, position2di cPos) {
  u32 editX=m_pDevice->getGUIEnvironment()->getSkin()->getFont()->getDimension(m_aMaxLenText[iSet].c_str()).Width+10;

  for (u32 i=0; i<m_aItems.size(); i++)
    if (m_aItems[i]->m_iSet==iSet) {
      m_aItems[i]->getGuiElement(pParent,cPos,position2di(editX,0));
      cPos.Y+=(s32)(2.5f*m_iGuiItemHeight);
    }
}

bool CIrrCC::ConfigEvent (const SEvent &event) {
  bool bRet=false;
  stringw s;
  u32 iItem=m_iFocusedIndex-65536;

  if (event.EventType==EET_GUI_EVENT && event.GUIEvent.EventType==EGET_ELEMENT_FOCUSED) {
    m_cMousePos=m_pDevice->getCursorControl()->getPosition();
		if (event.GUIEvent.Caller->getID()>=65536) {
      if (m_pFocused) m_pFocused->setBackgroundColor(SColor(128,192,192,192));
      s=stringw(event.GUIEvent.Caller->getText());
      m_pFocused=(IGUIStaticText *)event.GUIEvent.Caller;
      m_pFocused->setBackgroundColor(SColor(128,0,255,0));
      m_iFocusedIndex=m_pFocused->getID();
    }
  }

  if (m_pFocused) {
    CIrrCCItem *pItem=iItem<m_aItems.size()?m_aItems[iItem]:NULL,
               *pBrother=pItem!=NULL?pItem->m_pBrother:NULL;

    if (event.EventType==EET_KEY_INPUT_EVENT) {

			if ((m_bAllowFKeys || event.KeyInput.Key<KEY_F1 || event.KeyInput.Key>KEY_F24) && event.KeyInput.Key!=KEY_ESCAPE) {
        pItem->setKey(event.KeyInput.Key);
				bRet=true;
			}

      for (u32 i=0; i<m_aItems.size(); i++) {
        m_aItems[i]->setConflict(false);
        m_aItems[i]->getCtrlText()->setBackgroundColor(SColor(128,192,192,192));
      }
    }

    if (event.EventType==EET_MOUSE_INPUT_EVENT && m_bAllowMouse) {
      position2di mPos=m_pDevice->getCursorControl()->getPosition();
      if ((mPos.X>m_cMousePos.X+50 || mPos.X<m_cMousePos.X-50) && !pItem->isButton()) {
        s32 iDirection=mPos.X>m_cMousePos.X+50?1:-1;
        pItem->setMouseMove(0,iDirection);
        if (pBrother!=NULL) {
          if (pBrother->m_iMouseAxis!=pItem->m_iMouseAxis && pBrother->m_iMouseDirection!=-pItem->m_iMouseDirection) {
            pBrother->setMouseMove(0,-iDirection);
          }
        }
        bRet=true;
      }

      if ((mPos.Y>m_cMousePos.Y+50 || mPos.Y<m_cMousePos.Y-50) && !pItem->isButton()) {
        s32 iDirection=mPos.Y>m_cMousePos.Y+50?1:-1;
        pItem->setMouseMove(1,iDirection);
        if (pBrother->m_iMouseAxis!=pItem->m_iMouseAxis && pBrother->m_iMouseDirection!=-pItem->m_iMouseDirection) {
          pBrother->setMouseMove(1,-iDirection);
        }
        bRet=true;
      }

      if (event.MouseInput.Event==EMIE_LMOUSE_PRESSED_DOWN) {
        pItem->setMouseButton(0);
        bRet=true;
      }

      if (event.MouseInput.Event==EMIE_RMOUSE_PRESSED_DOWN) {
        pItem->setMouseButton(1);
        bRet=true;
      }

      if (event.MouseInput.Event==EMIE_MMOUSE_PRESSED_DOWN) {
        pItem->setMouseButton(2);
        bRet=true;
      }
    }

    if (event.EventType==EET_JOYSTICK_INPUT_EVENT && m_bAllowJoystick) {
      u8 joyId=event.JoystickEvent.Joystick;
      for (u32 i=0; i<6; i++) {
        s16 pos1=(m_aJoyState[joyId]->iAxis[i]),
            pos2=event.JoystickEvent.Axis[i];

				if (pos2<16000 && pos2>-16000) pos2=0;
        if (pos1!=pos2 && !pItem->isButton()) {
          s32 iDirection=pos1>pos2?1:-1;
          pItem->setJoyAxis(joyId,i,iDirection);
          if (pBrother!=NULL) {
            if (pItem->m_iJoyId!=pBrother->m_iJoyId || pItem->m_iJoyAxis!=pBrother->m_iJoyAxis)
              pBrother->setJoyAxis(joyId,i,-iDirection);
          }
          bRet=true;
        }

        if (m_aJoyState[joyId]->iPov!=event.JoystickEvent.POV) {
          pItem->setPov(joyId,event.JoystickEvent.POV);
          bRet=true;
        }

        if (m_aJoyState[joyId]->iButtons!=event.JoystickEvent.ButtonStates)
          for (u32 i=0; i<32; i++)
            if (m_aJoyState[joyId]->buttonPressed(i)!=event.JoystickEvent.IsButtonPressed(i)) {
              pItem->setJoyButton(joyId,i);
              bRet=true;
            }
      }
    }

    if (bRet) {
      pItem->updateCtrlText();
      pItem->setConflict(false);

      if (pBrother!=NULL) {
        pBrother->updateCtrlText();
        pBrother->setConflict(false);
        pBrother->getCtrlText()->setBackgroundColor(SColor(128,192,192,192));
      }

      m_pFocused->setBackgroundColor(SColor(128,192,192,192));
      m_pFocused=NULL;
      m_iFocusedIndex=-1;
      m_pDevice->getGUIEnvironment()->setFocus(NULL);
    }
  }

  for (u32 i=0; i<m_aItems.size(); i++) {
    for (u32 j=i+1; j<m_aItems.size(); j++) {
      if (m_aItems[i]->conflicts(m_aItems[j]) && (m_aItems[i]->m_iSet==m_aItems[j]->m_iSet || m_bSetsCanConflict)) {
        m_aItems[j]->getCtrlText()->setBackgroundColor(SColor(128,255,0,0)); m_aItems[j]->setConflict(true);
        m_aItems[i]->getCtrlText()->setBackgroundColor(SColor(128,255,0,0)); m_aItems[i]->setConflict(true);
      }
    }
  }

  if (!m_pFocused) {
    if (event.EventType==EET_JOYSTICK_INPUT_EVENT) {
      for (u32 i=0; i<6; i++) {
        s16 pos=event.JoystickEvent.Axis[i];
				if (pos<16000 && pos>-16000) pos=0;
        m_aJoyState[event.JoystickEvent.Joystick]->iAxis[i]=pos;
      }

      m_aJoyState[event.JoystickEvent.Joystick]->iPov=event.JoystickEvent.POV;
    }
  }

  return bRet;
}

void CIrrCC::setAllowMouse(bool b) {
  m_bAllowMouse=b;
}

void CIrrCC::setAllowFKeys(bool b) {
  m_bAllowFKeys=b;
}

void CIrrCC::setAllowJoystick(bool b) {
  m_bAllowJoystick=b;
}

f32 CIrrCC::get(u32 idx) {
  //if the queried item is a fader we have to do something special.
  if (m_aItems[idx]->m_eType==eCtrlFader && m_aItems[idx]->m_pBrother!=NULL) {
    //get the two items that make the fader
    CIrrCCItem *p1=m_aItems[idx],*p2=m_aItems[idx]->m_pBrother;
    u32 iTime=m_pDevice->getTimer()->getTime();

    //if the fader is a joy or mouse axis we simply return it's value
    if (p1->m_iType==CC_TYPE_JOY || p1->m_iType==CC_TYPE_MOU) {
      p1->m_fFaderValue=p1->get();
      p2->m_fFaderValue=p1->m_fFaderValue;
    }
    else {
      //if if's a button or a key we need to calculate it's current
      //value from using it's time step and it's diff per step value

      //in case the value is queried for the first time we need to initialize the timer stuff...
      if (p1->m_iFaderLastStep==0) {
        p1->m_iFaderLastStep=iTime;
        p2->m_iFaderLastStep=iTime;
      }
      else {
        //otherwise we calculate and return it's value
        f32 fValue=p1->get();
        while (p1->m_iFaderLastStep<iTime) {
          p1->m_fFaderValue+=fValue*p1->m_fFaderDiff;
          p1->m_iFaderLastStep+=p1->m_iFaderStep;
        }
        if (p1->m_fFaderValue> 1.0f) p1->m_fFaderValue= 1.0f;
        if (p1->m_fFaderValue<-1.0f) p1->m_fFaderValue=-1.0f;
        p2->m_fFaderValue=p1->m_fFaderValue;
      }
    }

    return p1->m_fFaderValue;
  }

  //if it's not a fader we simply return it's value
  return m_aItems[idx]->get();
}

void CIrrCC::set(u32 idx, f32 f) {
  m_aItems[idx]->set(f);
}

bool CIrrCC::OnEvent (const SEvent &event) {
  bool bRet=false;
  u32 i,j;

  if (event.EventType==EET_KEY_INPUT_EVENT)
    for (i=0; i<m_aItems.size(); i++) {
      CIrrCCItem *p=m_aItems[i];
      if (p->m_iType==CC_TYPE_KEY && p->m_eKey==event.KeyInput.Key) {
        p->setWithDetected(event.KeyInput.PressedDown?1.0f:0.0f);
        bRet=true;
      }
    }

  if (event.EventType==EET_JOYSTICK_INPUT_EVENT)
    for (i=0; i<m_aItems.size(); i++) {
      CIrrCCItem *p=m_aItems[i];
      switch (p->m_iType) {
        case CC_TYPE_JOY:
          if (p->m_iJoyId==event.JoystickEvent.Joystick)
            for (j=0; j<6; j++)
              if (p->m_iJoyAxis==j) {
                f32 pos=((f32)event.JoystickEvent.Axis[j])/256.0f;
                if (pos>10.0f) {
                  pos/=127;
                  if (p->m_iJoyDirection<0) p->setWithDetected(pos);
                }
                else
								  if (pos<-10.0f) {
                    pos/=128;
                    if (p->m_iJoyDirection>0) p->setWithDetected(-pos);
                  }
									else p->setWithDetected(0.0f);

                bRet=true;
              }
          break;

        case CC_TYPE_JBT:
          if (p->m_iJoyId==event.JoystickEvent.Joystick)
            p->setWithDetected(event.JoystickEvent.IsButtonPressed(p->m_iJoyButton)?1.0f:0.0f);
          break;

        case CC_TYPE_POV:
          if (p->m_iJoyId==event.JoystickEvent.Joystick) {
            p->setWithDetected(event.JoystickEvent.POV==p->m_iPov?1.0f:0.0f);
            bRet=true;
          }
          break;

        default:
          break;
      }
    }

  if (event.EventType==EET_MOUSE_INPUT_EVENT)
    for (i=0; i<m_aItems.size(); i++) {
      CIrrCCItem *p=m_aItems[i];
      if (p->m_iType==CC_TYPE_MBT) {
        if (p->m_iMouseBtn==0) {
          if (event.MouseInput.Event==EMIE_LMOUSE_PRESSED_DOWN) { p->setWithDetected(1.0f); bRet=true; }
          if (event.MouseInput.Event==EMIE_LMOUSE_LEFT_UP     ) { p->setWithDetected(0.0f); bRet=true; }
        }
        if (p->m_iMouseBtn==1) {
          if (event.MouseInput.Event==EMIE_RMOUSE_PRESSED_DOWN) { p->setWithDetected(1.0f); bRet=true; }
          if (event.MouseInput.Event==EMIE_RMOUSE_LEFT_UP     ) { p->setWithDetected(0.0f); bRet=true; }
        }
        if (p->m_iMouseBtn==2) {
          if (event.MouseInput.Event==EMIE_MMOUSE_PRESSED_DOWN) { p->setWithDetected(1.0f); bRet=true; }
          if (event.MouseInput.Event==EMIE_MMOUSE_LEFT_UP     ) { p->setWithDetected(0.0f); bRet=true; }
        }
      }
    }

  return bRet;
}

void CIrrCC::setMouseSensitivity(f32 f) {
  m_fMouseSensitivity=f;
  for (u32 i=0; i<m_aItems.size(); i++) m_aItems[i]->setMouseSensitivity(f);
}

void CIrrCC::reset() {
  for (u32 i=0; i<m_aItems.size(); i++) {
    m_aItems[i]->m_fValue=0.0f;
    m_aItems[i]->m_fDetectedValue=0.0f;
  }
}

void CIrrCC::createAxis(u32 idx1, u32 idx2) {
  m_aItems[idx1]->m_pBrother=m_aItems[idx2];
  m_aItems[idx2]->m_pBrother=m_aItems[idx1];
}

void CIrrCC::createFader(u32 idx1, u32 idx2, u32 iStep, f32 fInc) {
  CIrrCCItem *item1=idx1<m_aItems.size()?m_aItems[idx1]:NULL,
             *item2=idx2<m_aItems.size()?m_aItems[idx2]:NULL;

  if (item1==NULL || item2==NULL) return;

  item1->m_pBrother=item2;
  item2->m_pBrother=item1;
  item1->m_iFaderStep=iStep;
  item2->m_iFaderStep=iStep;
  item1->m_fFaderDiff= fInc;
  item2->m_fFaderDiff=-fInc;
  item1->m_fFaderValue=0.0f;
  item2->m_fFaderValue=0.0f;
}

void CIrrCC::save(c8 *sFileName) {
  IXMLWriter *pXml=m_pDevice->getFileSystem()->createXMLWriter(sFileName);

  if (pXml) {
    pXml->writeXMLHeader();
    pXml->writeElement(L"IrrConfigControl");
    pXml->writeLineBreak();

    for (u32 i=0; i<m_aItems.size(); i++) {
      CIrrCCItem *p=m_aItems[i];

      array<stringw> names,values;
      wchar_t s[0xFF];

                                         names.push_back(stringw(L"name")); values.push_back(p->m_sName.c_str());
      swprintf(s,0xFF,L"%u",p->m_iSet ); names.push_back(stringw(L"set" )); values.push_back(s                 );
      swprintf(s,0xFF,L"%u",p->m_iType); names.push_back(stringw(L"type")); values.push_back(s                 );

      switch (p->m_iType) {
        case CC_TYPE_KEY:
          names.push_back(stringw(L"key")); swprintf(s,0xFF,L"%u",(u32)p->m_eKey); values.push_back(s);
          break;

        case CC_TYPE_MOU:
          names.push_back(stringw(L"m_axis")); swprintf(s,0xFF,L"%u",p->m_iMouseAxis     ); values.push_back(s);
          names.push_back(stringw(L"m_dir" )); swprintf(s,0xFF,L"%i",p->m_iMouseDirection); values.push_back(s);
          break;

        case CC_TYPE_MBT:
          names.push_back(stringw(L"m_btn")); swprintf(s,0xFF,L"%u",p->m_iMouseBtn); values.push_back(s);
          break;

        case CC_TYPE_JOY:
          names.push_back(stringw(L"joy"   )); swprintf(s,0xFF,L"%u",p->m_iJoyId       ); values.push_back(s);
          names.push_back(stringw(L"j_axis")); swprintf(s,0xFF,L"%u",p->m_iJoyAxis     ); values.push_back(s);
          names.push_back(stringw(L"j_dir" )); swprintf(s,0xFF,L"%i",p->m_iJoyDirection); values.push_back(s);
          break;

        case CC_TYPE_JBT:
          names.push_back(stringw(L"joy"  )); swprintf(s,0xFF,L"%u",p->m_iJoyId    ); values.push_back(s);
          names.push_back(stringw(L"j_btn")); swprintf(s,0xFF,L"%u",p->m_iJoyButton); values.push_back(s);
          break;

        case CC_TYPE_POV:
          names.push_back(stringw(L"joy")); swprintf(s,0xFF,L"%u",p->m_iJoyId); values.push_back(s);
          names.push_back(stringw(L"pov")); swprintf(s,0xFF,L"%u",p->m_iPov  ); values.push_back(s);
          break;

        default:
          break;
      }

      pXml->writeElement(L"control",true,names,values);
      pXml->writeLineBreak();
    }

    pXml->writeClosingTag(L"IrrConfigControl");
    pXml->writeLineBreak();
    pXml->drop();
  }
}

void CIrrCC::load(c8 *sFileName) {
  if (m_bLoaded) return;
  IXMLReaderUTF8 *pXml=m_pDevice->getFileSystem()->createXMLReaderUTF8(sFileName);
  u32 iNum=0;

  if (pXml)
    while (pXml->read() && iNum<m_aItems.size()) {
      if (!strcmp(pXml->getNodeName(),"control")) {
        CIrrCCItem *p=m_aItems[iNum];
        p->m_iType=(IrrConfigControlTypes)(atoi(pXml->getAttributeValue("type")));
        p->m_iSet =atoi(pXml->getAttributeValue("set"));

        switch (p->m_iType) {
          case CC_TYPE_KEY:
            p->setKey((EKEY_CODE)atoi(pXml->getAttributeValue("key")));
            break;

          case CC_TYPE_MOU:
            p->setMouseMove(atoi(pXml->getAttributeValue("m_axis")),atoi(pXml->getAttributeValue("m_dir")));
            break;

          case CC_TYPE_MBT:
            p->setMouseButton(atoi(pXml->getAttributeValue("m_btn")));
            break;

          case CC_TYPE_JOY:
            p->setJoyAxis(atoi(pXml->getAttributeValue("joy")),atoi(pXml->getAttributeValue("j_axis")),atoi(pXml->getAttributeValue("j_dir")));
            break;

          case CC_TYPE_JBT:
            p->setJoyButton(atoi(pXml->getAttributeValue("joy")),atoi(pXml->getAttributeValue("j_btn")));
            break;

          case CC_TYPE_POV:
            p->setPov(atoi(pXml->getAttributeValue("joy")),atoi(pXml->getAttributeValue("pov")));
            break;

          default:
            break;
        }

        p->updateCtrlText();
        iNum++;
      }
    }

  m_bLoaded=true;
}
