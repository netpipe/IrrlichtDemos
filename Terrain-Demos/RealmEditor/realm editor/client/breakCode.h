void breakCode(stringc stringtobreak, stringc brokenString[], int numbreaks)
{
	s32 strpos=0, strpos2=0;
	//cout << endl << stringtobreak.c_str() << endl;
		strpos2=stringtobreak.findNext('|', 0)+1;
	for(int x=0;x<numbreaks;x++){
		strpos=strpos2;
		strpos2=stringtobreak.findNext('|', strpos);
		brokenString[x]=stringtobreak.subString(strpos, strpos2-strpos);
		//cout << "strpos " << strpos << " strpos2 " << strpos2 << " string " << brokenString[x].c_str() << endl;
		strpos2+=1;
	}
}

void breakCode(stringc stringtobreak, stringc brokenString[], int numbreaks, bool removeSize)
{
	s32 strpos=0, strpos2=0;
	//cout << endl << stringtobreak.c_str() << endl;
	if(removeSize==true)
	{
		strpos2=stringtobreak.findNext('|', 0)+1;
	}
	for(int x=0;x<numbreaks;x++){
		strpos=strpos2;
		strpos2=stringtobreak.findNext('|', strpos);
		brokenString[x]=stringtobreak.subString(strpos, strpos2-strpos);
		//cout << "strpos " << strpos << " strpos2 " << strpos2 << " string " << brokenString[x].c_str() << endl;
		strpos2+=1;
	}
}

