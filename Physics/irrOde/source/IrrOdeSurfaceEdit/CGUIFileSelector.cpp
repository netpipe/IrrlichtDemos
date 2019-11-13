/*
  This file dialog is taken from the official Irrlicht forum as Irrlicht itself doesn't feature a "save file" dialog.
  The URL of the thread is http://irrlicht.sourceforge.net/phpBB2/viewtopic.php?t=30241. Thanks to JP for providing
  this nice piece of code.
*/
#include "CGUIFileSelector.h"


const s32 FOD_WIDTH = 350;
const s32 FOD_HEIGHT = 265;

s32 CGUIFileSelector::numFileSelectors = 0;

//! constructor
CGUIFileSelector::CGUIFileSelector(const wchar_t* title, IGUIEnvironment* environment, IGUIElement* parent, s32 id, E_FILESELECTOR_TYPE type)
: IGUIFileOpenDialog(environment, parent, id,
 core::rect<s32>((parent->getAbsolutePosition().getWidth()-FOD_WIDTH)/2,
               (parent->getAbsolutePosition().getHeight()-FOD_HEIGHT)/2,
               (parent->getAbsolutePosition().getWidth()-FOD_WIDTH)/2+FOD_WIDTH,
               (parent->getAbsolutePosition().getHeight()-FOD_HEIGHT)/2+FOD_HEIGHT)),
  Dragging(false), FileNameText(0), FileList(0), DialogType(type)
{
    #ifdef _DEBUG
     IGUIElement::setDebugName("CGUIFileSelector");
   #endif

    Text = title;
    IsDirectoryChoosable = false;

   IGUISkin* skin = Environment->getSkin();
   IGUISpriteBank* sprites = 0;
   video::SColor color(255,255,255,255);
   if (skin)
   {
      sprites = skin->getSpriteBank();
      color = skin->getColor(EGDC_WINDOW_SYMBOL);
   }

   s32 buttonw = Environment->getSkin()->getSize(EGDS_WINDOW_BUTTON_WIDTH);
   s32 posx = RelativeRect.getWidth() - buttonw - 4;

   CloseButton = Environment->addButton(core::rect<s32>(posx, 3, posx + buttonw, 3 + buttonw), this, -1,
      L"", L"Close");
   CloseButton->setSubElement(true);
   if (sprites)
   {
      CloseButton->setSpriteBank(sprites);
      CloseButton->setSprite(EGBS_BUTTON_UP, skin->getIcon(EGDI_WINDOW_CLOSE), color);
      CloseButton->setSprite(EGBS_BUTTON_DOWN, skin->getIcon(EGDI_WINDOW_CLOSE), color);
   }
   CloseButton->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
   CloseButton->grab();

   OKButton = Environment->addButton(
      core::rect<s32>(RelativeRect.getWidth()-80, 30, RelativeRect.getWidth()-10, 50),
      this, -1, (DialogType==EFST_OPEN_DIALOG?L"Open":L"Save"));
   OKButton->setSubElement(true);
   OKButton->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
   OKButton->grab();

   CancelButton = Environment->addButton(
      core::rect<s32>(RelativeRect.getWidth()-80, 55, RelativeRect.getWidth()-10, 75),
      this, -1, skin ? skin->getDefaultText(EGDT_MSG_BOX_CANCEL) : L"Cancel");
   CancelButton->setSubElement(true);
   CancelButton->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
   CancelButton->grab();

   FileBox = Environment->addListBox(core::rect<s32>(10, 55, RelativeRect.getWidth()-90, 230), this, -1, true);
   FileBox->setSubElement(true);
   FileBox->setAlignment(EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT);
   FileBox->grab();

   FileNameText = Environment->addEditBox(0, core::rect<s32>(10, 30, RelativeRect.getWidth()-90, 50), true, this, -1);
   FileNameText->setSubElement(true);
   FileNameText->setAlignment(EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
   FileNameText->setTextAlignment(EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
   FileNameText->grab();

   FilterComboBox = Environment->addComboBox(core::rect<s32>(10, RelativeRect.getHeight()-30, RelativeRect.getWidth()-90, RelativeRect.getHeight()-10), this, -1);
   FilterComboBox->setSubElement(true);
   FilterComboBox->setAlignment(EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
   FilterComboBox->grab();
    FilterComboBox->addItem(L"All Files");

    core::stringc str = "FileSelectorIcons";
    str += numFileSelectors++;
    SpriteBank = Environment->addEmptySpriteBank(str.c_str());
    if (SpriteBank) {
      SpriteBank->grab();
      FileBox->setSpriteBank(SpriteBank);
    }
    DirectoryIconIdx = -1;
    FileIconIdx = -1;

   FileSystem = Environment->getFileSystem();

   if (FileSystem) {
      FileSystem->grab();
       prev_working_dir = FileSystem->getWorkingDirectory();
      //printf("working directory saved: %s\n", prev_working_dir.c_str());
   }

   fillListBox();
}

//! destructor
CGUIFileSelector::~CGUIFileSelector()
{
   if (CloseButton)
      CloseButton->drop();

   if (OKButton)
      OKButton->drop();

   if (CancelButton)
      CancelButton->drop();

   if (FileBox)
      FileBox->drop();

   if (FileNameText)
      FileNameText->drop();

   if (FileSystem)
      FileSystem->drop();

   if (FileList)
      FileList->drop();

   if (FilterComboBox)
       FilterComboBox->drop();

    if (SpriteBank)
        SpriteBank->drop();

}


//! returns the filename of the selected file. Returns NULL, if no file was selected.
const wchar_t* CGUIFileSelector::getFileName() const
{
   return FileNameText->getText();
}



//! called if an event happened.
bool CGUIFileSelector::OnEvent(const SEvent& event)
{
   switch(event.EventType)
   {
    case EET_KEY_INPUT_EVENT:
         switch (event.KeyInput.Key) {
           case KEY_RETURN:
             if (FileSystem) {
            FileSystem->changeWorkingDirectoryTo(core::stringc(FileNameText->getText()).c_str());
            fillListBox();
            FileNameText->setText(core::stringw(FileSystem->getWorkingDirectory()).c_str());
            }
             return true;

            default:
              break;
         }
         break;
   case EET_GUI_EVENT:
      switch(event.GUIEvent.EventType)
      {
        case EGET_COMBO_BOX_CHANGED:
             fillListBox();
             break;
      case EGET_ELEMENT_FOCUS_LOST:
         Dragging = false;
         break;
      case EGET_BUTTON_CLICKED:
         if (event.GUIEvent.Caller == CloseButton ||
            event.GUIEvent.Caller == CancelButton)
         {
                if (FileSystem) {
              FileSystem->changeWorkingDirectoryTo(prev_working_dir.c_str());
              //printf("working directory reset to: %s\n", prev_working_dir.c_str());
            }
            sendCancelEvent();
            remove();
            return true;
         }
         else
         if (event.GUIEvent.Caller == OKButton && (IsDirectoryChoosable || matchesFileFilter(FileNameText->getText())))
         {
            if (FileSystem) {
              FileSystem->changeWorkingDirectoryTo(prev_working_dir.c_str());
              //printf("working directory reset to: %s\n", prev_working_dir.c_str());
            }
            sendSelectedEvent();
            remove();
            return true;
         }
         break;

      case EGET_LISTBOX_CHANGED:
         {
            s32 selected = FileBox->getSelected();
            if (FileList && FileSystem)
            {
               core::stringw strw;
                 strw = FileSystem->getWorkingDirectory();
                 if (strw[strw.size()-1] != '\\')
                 strw += "\\";
                strw += FileBox->getListItem(selected);
               FileNameText->setText(strw.c_str());
            }
         }
         break;

      case EGET_LISTBOX_SELECTED_AGAIN:
         {
            s32 selected = FileBox->getSelected();
            if (FileList && FileSystem)
            {
               if (FileList->isDirectory(selected))
               {
                  FileSystem->changeWorkingDirectoryTo(FileList->getFileName(selected));
                  fillListBox();
                  FileNameText->setText(core::stringw(FileSystem->getWorkingDirectory()).c_str());
               }
               else
               {
                  core::stringw strw;
                     strw = FileSystem->getWorkingDirectory();
                   if (strw[strw.size()-1] != '\\')
                     strw += "\\";
                    strw += FileBox->getListItem(selected);
                   FileNameText->setText(strw.c_str());
                  return true;
               }
            }
         }
         break;
        default:
         break;
      }
      break;
   case EET_MOUSE_INPUT_EVENT:
      switch(event.MouseInput.Event)
      {
      case EMIE_LMOUSE_PRESSED_DOWN:
         DragStart.X = event.MouseInput.X;
         DragStart.Y = event.MouseInput.Y;
         Dragging = true;
         Environment->setFocus(this);
         return true;
      case EMIE_LMOUSE_LEFT_UP:
         Dragging = false;
         Environment->removeFocus(this);
         return true;
      case EMIE_MOUSE_MOVED:
         if (Dragging)
         {
            // gui window should not be dragged outside its parent
            if (Parent)
               if (event.MouseInput.X < Parent->getAbsolutePosition().UpperLeftCorner.X +1 ||
                  event.MouseInput.Y < Parent->getAbsolutePosition().UpperLeftCorner.Y +1 ||
                  event.MouseInput.X > Parent->getAbsolutePosition().LowerRightCorner.X -1 ||
                  event.MouseInput.Y > Parent->getAbsolutePosition().LowerRightCorner.Y -1)

                  return true;

            move(core::position2d<s32>(event.MouseInput.X - DragStart.X, event.MouseInput.Y - DragStart.Y));
            DragStart.X = event.MouseInput.X;
            DragStart.Y = event.MouseInput.Y;
            return true;
         }
         break;
      default:
        break;
      }
    default:
      break;
   }

   return Parent ? Parent->OnEvent(event) : false;
}


//! draws the element and its children
void CGUIFileSelector::draw()
{
   if (!IsVisible)
      return;

   IGUISkin* skin = Environment->getSkin();

   core::rect<s32> rect = AbsoluteRect;

   rect = skin->draw3DWindowBackground(this, true, skin->getColor(EGDC_ACTIVE_BORDER),
      rect, &AbsoluteClippingRect);

   if (Text.size())
   {
      rect.UpperLeftCorner.X += 2;
      rect.LowerRightCorner.X -= skin->getSize(EGDS_WINDOW_BUTTON_WIDTH) + 5;

      IGUIFont* font = skin->getFont(EGDF_WINDOW);
      if (font)
         font->draw(Text.c_str(), rect, skin->getColor(EGDC_ACTIVE_CAPTION), false, true,
         &AbsoluteClippingRect);
   }

   IGUIElement::draw();
}

bool CGUIFileSelector::matchesFileFilter(core::stringw s) {

  if (FilterComboBox->getSelected() >= (s32)FileFilters.size()) return true; // 'All Files' selectable
  else {
    s32 pos = s.findLast('.'); // Find the last '.' so we can check the file extension
    return FileFilters[FilterComboBox->getSelected()].FileExtension.equals_ignore_case(core::stringw(&s.c_str()[pos+1]));
  }

}

bool CGUIFileSelector::matchesFileFilter(core::stringw s, core::stringw f) {

  s32 pos = s.findLast('.'); // Find the last '.' so we can check the file extension
  return f.equals_ignore_case(core::stringw(&s.c_str()[pos+1]));

}

//! fills the listbox with files.
void CGUIFileSelector::fillListBox()
{
   IGUISkin *skin = Environment->getSkin();

   if (!FileSystem || !FileBox || !skin)
      return;

   if (FileList)
      FileList->drop();

   FileBox->clear();

   FileList = FileSystem->createFileList();
   core::stringw s;

   for (u32 i=0; i<FileList->getFileCount(); ++i)
   {
      s = FileList->getFileName(i);
      // We just want a list of directories and those matching the file filter
      if (FileList->isDirectory(i))
          if (DirectoryIconIdx != -1) FileBox->addItem(s.c_str(), DirectoryIconIdx);
          else                        FileBox->addItem(s.c_str());
      else if (matchesFileFilter(s))
        if (FilterComboBox->getSelected() >= (s32)FileFilters.size())
            if (FileIconIdx != -1) {
              s32 iconIdx = FileIconIdx;
              for (u32 i = 0 ; i < FileFilters.size() ; i++)
                if (matchesFileFilter(s, FileFilters[i].FileExtension))
                  iconIdx = FileFilters[i].FileIconIdx;
              FileBox->addItem(s.c_str(), iconIdx);
            } else  FileBox->addItem(s.c_str());
        else FileBox->addItem(s.c_str(), FileFilters[FilterComboBox->getSelected()].FileIconIdx);

   }

   if (FileNameText)
   {
      s = FileSystem->getWorkingDirectory();
      FileNameText->setText(s.c_str());
   }
}


//! sends the event that the file has been selected.
void CGUIFileSelector::sendSelectedEvent()
{
   SEvent event;
   event.EventType = EET_GUI_EVENT;
   event.GUIEvent.Caller = this;
   event.GUIEvent.EventType = EGET_FILE_SELECTED;
   Parent->OnEvent(event);
}

//! sends the event that the file choose process has been canceld
void CGUIFileSelector::sendCancelEvent()
{
   SEvent event;
   event.EventType = EET_GUI_EVENT;
   event.GUIEvent.Caller = this;
   event.GUIEvent.EventType = EGET_FILE_CHOOSE_DIALOG_CANCELLED;
   Parent->OnEvent(event);
}

void CGUIFileSelector::addFileFilter(wchar_t* name, wchar_t* ext, video::ITexture* texture) {

  SFileFilter filter(name, ext, texture);

  filter.FileIconIdx = addIcon(texture);

  FileFilters.push_back(filter);

  FilterComboBox->clear();
  core::stringw strw;
  for (u32 i = 0 ; i < FileFilters.size() ; i++) {
    strw = FileFilters[i].FilterName;
    strw += " (*.";
    strw += FileFilters[i].FileExtension;
    strw += ")";
    FilterComboBox->addItem(strw.c_str());
  }
  FilterComboBox->addItem(L"All Files");

  fillListBox();

}

u32 CGUIFileSelector::addIcon(video::ITexture* texture) {

  if (!SpriteBank || !texture) return 0;

  // load and add the texture to the bank
         SpriteBank->addTexture(texture);
         u32 textureIndex = SpriteBank->getTextureCount() - 1;
         // now lets get the sprite bank's rectangles and add some for our animation
         core::array<core::rect<s32> >& rectangles = SpriteBank->getPositions();
         u32 firstRect = rectangles.size();
         // remember that rectangles are not in pixels, they enclose pixels!
         // to draw a rectangle around the pixel at 0,0, it would rect<s32>(0,0, 1,1)
         rectangles.push_back(core::rect<s32>(0,0, 16,16));


         // now we make a sprite..
         SGUISprite sprite;
         sprite.frameTime = 30;
         // add some frames of animation.
         SGUISpriteFrame frame;
         frame.rectNumber = firstRect;
         frame.textureNumber = textureIndex;

         // add this frame
         sprite.Frames.push_back(frame);
         // add the sprite
         //u32 spriteIndex = SpriteBank->getSprites().size();
         SpriteBank->getSprites().push_back(sprite);

         return textureIndex;

}
