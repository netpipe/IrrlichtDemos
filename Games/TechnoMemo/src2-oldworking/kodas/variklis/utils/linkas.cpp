#include "linkas.hpp"


KLinkas::KLinkas(): lPav(NULL), fPav(NULL), apr(NULL), ikona(NULL), dir(NULL)
{
}


KLinkas::~KLinkas()
{
}


void KLinkas::Sukurti() throw(const char*)
{
	Sukurti(lPav, fPav, apr, ikona, dir);
}


void KLinkas::Sukurti(const char* _lPav, const char* _fPav, const char* _apr,
  const char* _ikona, const char* _dir) throw(const char*)
{
	HRESULT res;
	IShellLink *linkas;
	IPersistFile *ipf;
	
	if(!_lPav)
		throw("nenurodytas nuorodos pavadinimas");
	if(!_fPav)
		throw("nenurodytas failas");
	
	res = CoInitialize(NULL);
	if(!SUCCEEDED(res))
		throw("CoInitialize");
		
	res = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&linkas);
	if(!SUCCEEDED(res))
		throw("CoCreateInstance");
	
	if(_fPav)
		linkas->SetPath(_fPav);
	if(_apr)
		linkas->SetDescription(_apr);
	if(_ikona)
		linkas->SetIconLocation(_ikona, 0);
	if(_dir)
		linkas->SetWorkingDirectory(_dir);
	
	res = linkas->QueryInterface(IID_IPersistFile, (LPVOID*)&ipf);
	if(!SUCCEEDED(res))
		throw("QueryInterface");
	
	WCHAR pav[MAX_PATH];
	MultiByteToWideChar(CP_ACP, 0, _lPav, -1, pav, MAX_PATH);
	res = ipf->Save(pav, TRUE);
	ipf->Release();
	linkas->Release();
}
