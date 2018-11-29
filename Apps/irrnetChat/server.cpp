#include <irrlicht.h>
#include <irrNet.h>
#include <iostream>

using namespace irr;
using namespace core;

net::INetManager* netManager;
class MyNetCallback : public net::INetCallback
{

public:

   virtual void handlePacket(net::SInPacket& packet)
   {

      packet.deCompressPacket();
      core::stringc str;
      packet >> str;
      // Print the values to the console.
      std::cout << "Message: " << str.c_str();
      std::cout << std::endl;

      if(netManager->getConnectionStatus() != net::EICS_FAILED)
      {
         net::SOutPacket outpacket;
         outpacket << str.c_str();
         outpacket.compressPacket();
         netManager->sendOutPacket(outpacket);
         netManager->update(100);
      }

   }
};

int main()
{
   // Create a server and pass in a new instance of our callback class. The default
   // port that clients can connect to is set to 45000.
   MyNetCallback* netCallback = new MyNetCallback();
   netManager = net::createIrrNetServer(netCallback);netManager->setVerbose(true);
   while(netManager->getConnectionStatus() != net::EICS_FAILED)
   {
      std::cout << "im still here";  //debugging
      std::cout << std::endl;
      netManager->update(1000);
   }
   // Don't forget to clean up!
   delete netManager;
   delete netCallback;
   return 0;
}
