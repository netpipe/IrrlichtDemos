//
////#include <windows.h>
//#include <string>
//#include "OS.h"
//
//namespace os
//{
//
//
//#if defined(WIN32) || defined(WIN64)
//
//bool runApp(char *appName)
//{
//    STARTUPINFO sinfo;
//    FillMemory(&sinfo, sizeof(STARTUPINFO), 0);
//    sinfo.cb = sizeof(STARTUPINFO);
//
//    PROCESS_INFORMATION pinfo;
//
//    if (CreateProcess(NULL, appName, NULL, NULL, FALSE,
//        NORMAL_PRIORITY_CLASS, NULL,
//        NULL, // current dir
//        &sinfo,
//        &pinfo)
//        )
//    {
//        CloseHandle(pinfo.hProcess);
//        CloseHandle(pinfo.hThread);
//        return true;
//    }
//    return false;
//}
//
//void openWebpage(char *page)
//{
//    // modified from http://www.codeproject.com/internet/urlnewwindow.asp
//
//    std::string strBrowser;
//
//    HKEY hKey = NULL;
//    if (RegOpenKeyEx(HKEY_CLASSES_ROOT, "http\\shell\\open\\command", 0, KEY_READ, &hKey) == ERROR_SUCCESS)
//    {
//        DWORD cbData = 0;
//        if (RegQueryValueEx(hKey, NULL, NULL, NULL, NULL, &cbData) == ERROR_SUCCESS && cbData > 0)
//        {
//            TCHAR *psz = new TCHAR[cbData];
//            if (psz != NULL)
//            {
//                if (RegQueryValueEx(hKey, NULL, NULL, NULL, (LPBYTE)psz, &cbData) == ERROR_SUCCESS)
//                {
//                    strBrowser = psz;
//                }
//                delete [] psz;
//            }
//        }
//        RegCloseKey(hKey);
//    }
//
//    if (strBrowser.length() > 0)
//    {
//        std::string::size_type nStart = strBrowser.find('"', 0);
//
//        // if first is a quote, then exe path is in quotes, so find the second quote
//        if (nStart == 0)
//        {
//            std::string::size_type nEnd = strBrowser.find('"', 1);
//            if (nStart != nEnd && nEnd != std::string::npos)
//            {
//                strBrowser = strBrowser.substr(nStart+1, nEnd-nStart-1);
//            }
//        }
//        // otherwise if no quotes, 2nd point is the first space after the last backslashes
//        else
//        {
//            std::string::size_type nIndex = strBrowser.rfind('\\', strBrowser.length()-1);
//            if (nIndex != std::string::npos)
//            {
//                std::string::size_type nSpace = strBrowser.find(' ', nIndex);
//                if (nSpace != std::string::npos)
//                {
//                    strBrowser = strBrowser.substr(0, nSpace);
//                }
//            }
//        }
//
//        std::string command = "\"";
//        command += strBrowser;
//        command += "\" \"";
//        command += page;
//        command += "\"";
//
//        if (runApp((char *)command.c_str()))
//        {
//            return;
//        }
//    }
//
//    ShellExecute(NULL, "open", page, NULL, NULL, SW_SHOWNORMAL);
//}
//
//#else
//
//#define BROWSER_SCRIPT "open_browser.sh"
//
//void openWebpage(char *page)
//{
//    int pid;
//    switch ( pid=fork())
//    {
//        case 0: // child
//            execl(BROWSER_SCRIPT, BROWSER_SCRIPT, page, (char*)0);
//
//            // should never be reached
////            perror("execl failed");
////            exit(2);
//        case -1:
////            perror("fork failed");
////            exit();
//        default:
//            // just continue
//            break;
//    }
//}
//
//#endif
//
//
//} // End namespace OS
