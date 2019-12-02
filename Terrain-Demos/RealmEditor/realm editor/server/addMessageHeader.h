// add custom message header to outgoing messages
void addMessageHeader(stringc &netMessage)
{
	//add size to front
	//get the message size
	int startSize=netMessage.size();
	bool sizeChanged=false;
	//based off of message size + size of header determine new start size and attach it to the front of the message
	if(startSize+2<10&&!sizeChanged)
	{
	startSize+=2;
	sizeChanged=true;
	}else if(startSize+3<100&&!sizeChanged)
	{
	startSize+=3;
	sizeChanged=true;
	}else if(startSize+4<1000&&!sizeChanged)
	{
	startSize+=4;
	sizeChanged=true;
	}else if(startSize+5<10000&&!sizeChanged)
	{
	startSize+=5;
	sizeChanged=true;
	}
	//attach to front of message
	char sizeAttach[10];
	sprintf(sizeAttach,"%i|",startSize);
	stringc sizeAttachment=sizeAttach;
	//append message to end of sizeAttachment
	sizeAttachment.append(netMessage);
	//give them the new message with size attached
	netMessage=sizeAttachment;
}
