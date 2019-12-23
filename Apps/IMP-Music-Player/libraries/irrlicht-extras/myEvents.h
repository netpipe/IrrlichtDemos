#include <irrlicht.h>
#include <iostream>

// Declare Used Namespaces:
using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


/// My events for the GUI Buttons:
bool Selected_GUIButton   = false;
int  Selected_GUIButtonID = 0;

/// My events for the GUI ListBox: (on item selected)
bool ItemSelected_GUIListBox = false;

/// My events for the GUI FileDialog: (on OK clicked)
bool DirSelected_GUIFileOpenDialog = false;
std::string fileopendialog_path  = "music/";
// gui::IGUIFileOpenDialog* dialog;
bool canCreateNewFOD = true;


bool userDragging_positionScroolbar = false; // 0 = no, 1 = yes
s32 ScroolbarPosition               = 0;

bool userDragging_volumeScroolbar   = false; // 0 = no, 1 = yes
s32 ScroolbarVolume                 = 0;

bool userDragging_pitchScroolbar    = false; // 0 = no, 1 = yes
s32 ScroolbarPitch                  = 0;

std::string userMouseWheel = "";


/// IDs for our scrollbars:
enum {
ID_SCROLLBAR_POSITION,
ID_SCROLLBAR_VOLUME,
ID_SCROLLBAR_PITCH,
ID_FILEDIALOG_N1
};








/// ##########################################################################################
/// EVENTS - ALL IN ONE (KEYBOARD, MOUSE, ETC): ##############################################
/// ##########################################################################################



   class MyEventReceiver : public irr::IEventReceiver {

   public:

   // Enumeration for UP, DOWN, PRESSED and RELEASED key states. Also used for mouse button states.
   enum keyStatesENUM {UP, DOWN, PRESSED, RELEASED};

   // Keyboard key states.
   keyStatesENUM keyState[KEY_KEY_CODES_COUNT];


   // Mouse data.
   struct mouseData {
   int X;
   int Y;
   bool LButtonDown;
   bool RButtonDown;
   bool MButtonDown;
   float Wheel;
   float WheelDelta;
   };


   struct mouseData m_Mouse;
   int MouseX()            { return m_Mouse.X;          };
   int MouseY()            { return m_Mouse.Y;          };
   float MouseWheel()      { return m_Mouse.Wheel;      };
   float MouseWheelDelta() { return m_Mouse.WheelDelta; };


   MyEventReceiver(void) {
   for(int i=0; i<KEY_KEY_CODES_COUNT; i++){ keyState[i] = RELEASED; };
   m_Mouse.X = 0;
   m_Mouse.Y = 0;
   m_Mouse.Wheel = 0;
   m_Mouse.WheelDelta = 0;
   m_Mouse.LButtonDown = false;
   m_Mouse.RButtonDown = false;
   m_Mouse.MButtonDown = false;
   };


   virtual ~MyEventReceiver(void){};

   // Keyboard events:
   virtual bool OnKeyInputEvent(const SEvent& e)        { return false; };
   virtual bool IsKeyPressed(unsigned char keycode)     { return (keyState[keycode] == PRESSED); };
   virtual bool IsKeyDown(unsigned char keycode)        { return (keyState[keycode] == DOWN || keyState[keycode] == PRESSED);  };
   virtual bool IsKeyUp(unsigned char keycode)          { return (keyState[keycode] == UP   || keyState[keycode] == RELEASED); };
   virtual bool IsKeyReleased(unsigned char keycode)    { return (keyState[keycode] == RELEASED); };

   // Guievents:
   virtual bool OnButtonClicked(const SEvent& e)        { return false; }; // printf("event: Button Clicked \n");
   // or you can override the: virtual bool OnButtonClicked(const SEvent& e){ return false; };
   virtual bool OnScrollBarChanged(const SEvent& e)     { return false; };
   virtual bool OnCheckBoxChanged(const SEvent& e)      { return false; };
   virtual bool OnListBoxChanged(const SEvent& e)       { return false; };
   virtual bool OnListBoxSelectedAgain(const SEvent& e) { return false; };
   virtual bool OnDirectorySelected(const SEvent& e)    { return false; };
   virtual bool OnFileSelected(const SEvent& e)         { return false; };
   virtual bool OnFileChooseDialogCanceled(const SEvent& e) { return false; };
   virtual bool OnMessageBoxYes(const SEvent& e)        { return false; };
   virtual bool OnMessageBoxNo(const SEvent& e)         { return false; };
   virtual bool OnMessageBoxOk(const SEvent& e)         { return false; };
   virtual bool OnMessageBoxCancel(const SEvent& e)     { return false; };
   virtual bool OnEditBoxEnter(const SEvent& e)         { return false; };
   virtual bool OnTabChanged(const SEvent& e)           { return false; };
   virtual bool OnComboBoxChanged(const SEvent& e)      { return false; };
   virtual bool OnSpinBoxChanged(const SEvent& e)       { return false; };

   // Mouse events:
   virtual bool OnLMousePressedDown(const SEvent& e)    { return false; }; // printf("event: LMouse down \n");
   virtual bool OnRMousePressedDown(const SEvent& e)    { return false; }; // printf("event: RMouse down \n");
   virtual bool OnMMousePressedDown(const SEvent& e)    { return false; }; // printf("event: MMouse down \n");
   virtual bool OnLMouseLeftUp(const SEvent& e)         { return false; }; // printf("event: LMouse up \n");
   virtual bool OnRMouseLeftUp(const SEvent& e)         { return false; }; // printf("event: RMouse up \n");
   virtual bool OnMMouseLeftUp(const SEvent& e)         { return false; }; // printf("event: MMouse up \n");
   virtual bool OnMouseMoved(const SEvent& e)           { return false; }; // printf("event: MOUSE moved \n");
   virtual bool OnMouseWheel(const SEvent& e)           { return false; }; // printf("event: Mouse wheel \n");

   // User events:
   virtual bool OnUserEvent(const SEvent& e)            { printf("event: USER event \n"); return false; };





   virtual bool OnEvent(const SEvent& e){



      switch(e.EventType){

         case EET_KEY_INPUT_EVENT :
            {
               if(e.KeyInput.PressedDown == true){
                  if(keyState[e.KeyInput.Key] != DOWN){ keyState[e.KeyInput.Key] = PRESSED; } else { keyState[e.KeyInput.Key] = DOWN; };

               } else {

                  if(keyState[e.KeyInput.Key] != UP){ keyState[e.KeyInput.Key] = RELEASED; };
                  if(e.KeyInput.PressedDown == true){ return OnKeyInputEvent(e); };
               };

            };
         break;

         case EET_GUI_EVENT :
            {


			s32 id = e.GUIEvent.Caller->getID();
			// IGUIEnvironment* env = Context.device->getGUIEnvironment();



            if (e.GUIEvent.EventType == EGET_BUTTON_CLICKED){
            /// std::cout << "(GUIEvent) Clicked Button ID: " << id << std::endl;
            };


            // WORKING! Clicked ListBox Item:
            // int id = e.GUIEvent.Caller->getID();
            // if (e.GUIEvent.EventType == EGET_LISTBOX_SELECTED_AGAIN){
            /// GUI ListBox:
            if (e.GUIEvent.EventType == EGET_LISTBOX_CHANGED){
            ItemSelected_GUIListBox = true;
            };



            // WORKING! Clicked FileOpenDialog OK button:
            // https://github.com/ousttrue/onibi/blob/master/irrlicht/examples/D.PmdAndVmd/main.cpp
            /// GUI FileOpenDialog:
            if (e.GUIEvent.EventType == gui::EGET_FILE_SELECTED){ /// <-- Means that the user clicked OK button
            canCreateNewFOD = true;
            };


            /// GUI FileOpenDialog:
            if (e.GUIEvent.EventType == EGET_DIRECTORY_SELECTED){ /// <-- Means that the user clicked OK button
            gui::IGUIFileOpenDialog* dialog = (gui::IGUIFileOpenDialog*)e.GUIEvent.Caller;
            if(dialog->getDirectoryName() != 0){ /// FIXED: Make sure that we selected a directory not a file !!! (Fixes FileOpenDialog Bug)
            fileopendialog_path = dialog->getDirectoryName().c_str();
            dialog->setVisible(false); /// forces the dialog to "exit", since it fails sometimes....
            DirSelected_GUIFileOpenDialog = true;
            canCreateNewFOD = true;
            };
            };

            /// GUI FileOpenDialog:
            if (e.GUIEvent.EventType == EGET_FILE_CHOOSE_DIALOG_CANCELLED){ // <-- Means that the user clicked OK button
            // std::cout << "EGET_FILE_CHOOSE_DIALOG_CANCELLED" << std::endl;
            canCreateNewFOD = true;
            };






            if (e.GUIEvent.EventType == gui::EGET_SCROLL_BAR_CHANGED){

            /// GUI Position ScrollBar:
            if(id==ID_SCROLLBAR_POSITION){
            ScroolbarPosition = ((IGUIScrollBar*)e.GUIEvent.Caller)->getPos();
            userDragging_positionScroolbar = true;
            };

            /// GUI Volume ScrollBar:
            if(id==ID_SCROLLBAR_VOLUME){
            ScroolbarVolume = ((IGUIScrollBar*)e.GUIEvent.Caller)->getPos();
            userDragging_volumeScroolbar   = true;
            };

            /// GUI Pitch ScrollBar:
            if(id==ID_SCROLLBAR_PITCH){
            ScroolbarPitch = ((IGUIScrollBar*)e.GUIEvent.Caller)->getPos();
            userDragging_pitchScroolbar    = true;
            };

            };






               switch (e.GUIEvent.EventType) {

                  case EGET_BUTTON_CLICKED         : return OnButtonClicked(e);
                  case EGET_SCROLL_BAR_CHANGED     : return OnScrollBarChanged(e);
                  case EGET_CHECKBOX_CHANGED       : return OnCheckBoxChanged(e);
                  case EGET_LISTBOX_CHANGED        : return OnListBoxChanged(e);
                  case EGET_LISTBOX_SELECTED_AGAIN : return OnListBoxSelectedAgain(e);
                  case EGET_FILE_CHOOSE_DIALOG_CANCELLED : return OnFileChooseDialogCanceled(e);
                  case EGET_DIRECTORY_SELECTED     : return OnDirectorySelected(e);
                  case EGET_FILE_SELECTED          : return OnFileSelected(e);
                  case EGET_MESSAGEBOX_YES         : return OnMessageBoxYes(e);
                  case EGET_MESSAGEBOX_NO          : return OnMessageBoxNo(e);
                  case EGET_MESSAGEBOX_OK          : return OnMessageBoxOk(e);
                  case EGET_MESSAGEBOX_CANCEL      : return OnMessageBoxCancel(e);
                  case EGET_EDITBOX_ENTER          : return OnEditBoxEnter(e);
                  case EGET_TAB_CHANGED            : return OnTabChanged(e);
                  case EGET_COMBO_BOX_CHANGED      : return OnComboBoxChanged(e);
                  case EGET_SPINBOX_CHANGED        : return OnSpinBoxChanged(e);
                  default : return false;
               };
            };
         break;

         case EET_MOUSE_INPUT_EVENT :
            {
               m_Mouse.X = e.MouseInput.X;
               m_Mouse.Y = e.MouseInput.Y;

               switch (e.MouseInput.Event) {

                  case EMIE_LMOUSE_PRESSED_DOWN : m_Mouse.LButtonDown = true;  return OnLMousePressedDown(e);
                  case EMIE_RMOUSE_PRESSED_DOWN : m_Mouse.RButtonDown = true;  return OnRMousePressedDown(e);
                  case EMIE_MMOUSE_PRESSED_DOWN : m_Mouse.MButtonDown = true;  return OnMMousePressedDown(e);
                  case EMIE_LMOUSE_LEFT_UP      : m_Mouse.LButtonDown = false; return OnLMouseLeftUp(e);
                  case EMIE_RMOUSE_LEFT_UP      : m_Mouse.RButtonDown = false; return OnRMouseLeftUp(e);
                  case EMIE_MMOUSE_LEFT_UP      : m_Mouse.MButtonDown = false; return OnMMouseLeftUp(e);
                  case EMIE_MOUSE_MOVED         : return OnMouseMoved(e);
                  case EMIE_MOUSE_WHEEL         :
                     {
                        m_Mouse.WheelDelta = m_Mouse.Wheel - e.MouseInput.Wheel;
                        m_Mouse.Wheel += e.MouseInput.Wheel;

                        /// My code: (good for the wheel 2 states only, in other situations must be removed)
                        if(m_Mouse.Wheel < 0){ m_Mouse.WheelDelta = 0; m_Mouse.Wheel = 0; userMouseWheel = "down"; };
                        if(m_Mouse.Wheel > 0){ m_Mouse.WheelDelta = 0; m_Mouse.Wheel = 0; userMouseWheel = "up";   };

                        return OnMouseWheel(e);
                     };
                  default : return false;
               };
            };
         break;

         case EET_USER_EVENT :
            {
               return OnUserEvent(e);
               default : return false;
            };
         break;
      };
      return false;
    };
    };




    /*
    // Guievents - Should work this way (?)
    // Should be called from "while(device->run())" at "main.cpp" someway...

    class app : public CSEventHandler
    {
    blah blah blah.......
    virtual bool OnButtonClicked(const SEvent& e)
        {
            switch (e.whatever)
            {
            case ID_SOMETHING : doit(); return true;
            case ID_SOMETHING2 : doit2(); return true;
            case ID_SOMETHING3 : doit3(); return true;
            }
            return false;
        }
    }
    */



