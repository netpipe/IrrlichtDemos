#include <irrlicht.h>
using namespace irr;
using namespace core;
void convertString(stringc &irrstringc, stringw &irrstringw, int convertDirection)
{
	if(convertDirection==1){
		//wide to ascii
		const wchar_t* heightchar= irrstringw.c_str();
		int alloc_len=irrstringw.size()+1;
		char* chararray = new char[alloc_len];
		int result=wcstombs(chararray, irrstringw.c_str(), alloc_len);
		//irrstringc=new stringc();
		irrstringc.append(chararray);
	}else{
		//ascii to wide
		size_t alloc_len = irrstringc.size() + 1;
		wchar_t* wc_out = new wchar_t[alloc_len];
		size_t result = mbstowcs(wc_out, irrstringc.c_str(), alloc_len);
		//irrstringw=new stringw();
		irrstringw.append(wc_out);
	}
}