/*
bool works;
bool login=0;
net::SOutPacket rotationPacket;
vector3df tmpvect;



class ClientNetCallback : public net::INetCallback
{
public:

	 //	u16 playerId = packet.getPlayerId();
	virtual void handlePacket(net::SInPacket& packet)
	{
		//packet.decryptPacket("hushthisissecret");
		//packet.deCompressPacket();
		core::stringc str;
		packet >> str;

		core::vector3df vec;
		packet >> vec;

        //	f32 height;
        //	packet >> height;
        //	rotationPacket << str;

        if (!login){
            if (str == "handshake"){
			login=1;
            std::cout << "Authorized" << str.c_str() << std::endl;
            }
        }
       // }else{}
//        resetV=true;
        // m_cVehicle2->resetVehicle();//( core::vector3df(vec[0],vec[1],vec[2]));
        tmpvect=vector3df(vec);
        std::cout << "vehicle   coords xyz=" << vec.X << vec.Y << vec.Z << std::endl;
        //login=false;
	}
};

*/
