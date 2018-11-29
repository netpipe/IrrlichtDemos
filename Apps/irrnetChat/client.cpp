#include <stdio.h>
#include <irrlicht.h>
#include <irrNet.h>
#include <wchar.h>
#include <stdlib.h>
//#include <enet/enet.h>
//#include <ws2tcpip.h>
//#include <winsock2.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

enum
{
   EDIT_ID,
   CHAT_ID
};

gui::IGUIElement* root;
wchar_t * buff;
wchar_t * buffB;
stringc test1 ;
stringw test2 ;
int keyInput;
bool chat;


IrrlichtDevice *device ;
IVideoDriver* driver ;
ISceneManager* smgr ;
IGUIEnvironment* guienv ;
IGUIEditBox* editbox;
IGUIListBox* chatbox;
net::INetManager* netManager;

// The client callback.
class MyNetCallback : public net::INetCallback
{
   stringc str;
public:
   // Our handlePacket function.
   virtual void handlePacket(net::SInPacket& packet)
   {
      packet.deCompressPacket();
      packet >> str;
      stringw mess = str.c_str();
      chatbox->addItem(mess.c_str());
   }
};

class MyEventReceiver : public IEventReceiver
{
public:
   virtual bool OnEvent(const SEvent& event)
   {
      if (event.KeyInput.PressedDown == true && event.KeyInput.Key == KEY_RETURN)
      {
         if(guienv->hasFocus(editbox) == true)
         {
            chat = false;
         }
         else if(guienv->hasFocus(editbox) == false)
         {
            chat = true;
         }

         if (chat == true) guienv->setFocus(editbox);

         if (chat == false)
         {
            if(netManager->getConnectionStatus() != net::EICS_FAILED)
            {
               net::SOutPacket packet;
               test1 = editbox->getText();
               packet << test1.c_str();
               packet.compressPacket();
               netManager->sendOutPacket(packet);
               editbox->setText(L"");
               guienv->removeFocus(editbox);
               netManager->update(10);
            }
         }
      }
      return false;
   }

};

int main(int argc, char **argv)
{
   MyEventReceiver receiver;

   device = createDevice( video::EDT_SOFTWARE, dimension2d<s32>(800, 600), 16,false, false, false, &receiver);
   device->setWindowCaption(L"Irrlicht MMORPG");
   driver = device->getVideoDriver();
   smgr = device->getSceneManager();
   guienv = device->getGUIEnvironment();
   //guienv->addStaticText(L"Irrlicht MMORPG",rect<s32>(10,10,260,22), true);
   smgr->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));
   chatbox = guienv->addListBox(rect<s32>(0,500,400,580),0,CHAT_ID,false);
   editbox = guienv->addEditBox(L"", rect<s32>(0, 580, 400, 600), true, 0, EDIT_ID);
   editbox->setMultiLine(false);
   root = guienv->getRootGUIElement();

   MyNetCallback* netCallback = new MyNetCallback();
   netManager = net::createIrrNetClient(netCallback, "127.0.0.1");
   netManager->setVerbose(true);

   if (!device)
      return 1;

   while(device->run())
   {
      driver->beginScene(true, true, SColor(255,100,101,140));
      smgr->drawAll();
      guienv->drawAll();

      if(netManager->getConnectionStatus() != net::EICS_FAILED)
         netManager->update(10);

      driver->endScene();

   }
   device->drop();
   delete netManager;
   delete netCallback;
   return 0;
}
