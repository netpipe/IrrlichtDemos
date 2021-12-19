#ifndef ADR_WX_H
#define ADR_WX_H


#ifdef _MSC_VER
  #pragma warning(disable : 4786)
  #include <windows.h>
  #ifdef CreateDialog
    #undef CreateDialog
  #endif
#endif

#include <wx/wx.h>

#if defined(wxUSE_UNICODE) && wxUSE_UNICODE
	#define CStr2wxString(x) wxString((const char *)x, wxConvUTF8)
	#define wxString2CStr(x) x.mb_str(wxConvUTF8).data()
#else
	#define CStr2wxString(x) wxString((const char *)x)
	#define wxString2CStr(x) x
#endif


#endif
