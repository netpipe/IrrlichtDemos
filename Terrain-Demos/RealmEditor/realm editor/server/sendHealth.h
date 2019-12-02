void sendHealth(int sock, int type, int id, int health)
{
	//send a new health to a client
	//type, 0 npc, 1 other player, 2 clients char
	//id not used if type is 2
	//sets health equal to given health
	stringc sendData;
	char sendChar[200];
	sprintf(sendChar, "35|%i|%i|%i|", type, id, health);
	sendData=sendChar;
	addMessageHeader(sendData);
	send(clientsock[sock], sendData.c_str(), sendData.size(),0);
}
