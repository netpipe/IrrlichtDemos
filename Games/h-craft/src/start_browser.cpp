// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "start_browser.h"
#include <cstdlib>

#ifdef __GNUWIN32__
#include <windows.h>

#include <windows.h>
#include <string>

bool RunWindowsApp(char *appName)
{
    STARTUPINFO sinfo;
    FillMemory(&sinfo, sizeof(STARTUPINFO), 0);
    sinfo.cb = sizeof(STARTUPINFO);

    PROCESS_INFORMATION pinfo;

    if (CreateProcess(NULL, appName, NULL, NULL, FALSE,
        NORMAL_PRIORITY_CLASS, NULL,
        NULL, // current dir
        &sinfo,
        &pinfo)
        )
    {
        CloseHandle(pinfo.hProcess);
        CloseHandle(pinfo.hThread);
        return true;
    }
    return false;
}

void OpenUrl(const std::string &url_)
{
    // modified from http://www.codeproject.com/internet/urlnewwindow.asp

    std::string strBrowser;

    HKEY hKey = NULL;
    if (RegOpenKeyEx(HKEY_CLASSES_ROOT, "http\\shell\\open\\command", 0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
        DWORD cbData = 0;
        if (RegQueryValueEx(hKey, NULL, NULL, NULL, NULL, &cbData) == ERROR_SUCCESS && cbData > 0)
        {
            TCHAR *psz = new TCHAR[cbData];
            if (psz != NULL)
            {
                if (RegQueryValueEx(hKey, NULL, NULL, NULL, (LPBYTE)psz, &cbData) == ERROR_SUCCESS)
                {
                    strBrowser = psz;
                }
                delete [] psz;
            }
        }
        RegCloseKey(hKey);
    }

    if (strBrowser.length() > 0)
    {
        std::string::size_type nStart = strBrowser.find('"', 0);

        // if first is a quote, then exe path is in quotes, so find the second quote
        if (nStart == 0)
        {
            std::string::size_type nEnd = strBrowser.find('"', 1);
            if (nStart != nEnd && nEnd != std::string::npos)
            {
                strBrowser = strBrowser.substr(nStart+1, nEnd-nStart-1);
            }
        }
        // otherwise if no quotes, 2nd point is the first space after the last backslash
        else
        {
            std::string::size_type nIndex = strBrowser.rfind('\\', strBrowser.length()-1);
            if (nIndex != std::string::npos)
            {
                std::string::size_type nSpace = strBrowser.find(' ', nIndex);
                if (nSpace != std::string::npos)
                {
                    strBrowser = strBrowser.substr(0, nSpace);
                }
            }
        }

        std::string command = "\"";
        command += strBrowser;
        command += "\" \"";
        command += url_;
        command += "\"";

        if (RunWindowsApp((char *)command.c_str()))
        {
            return;
        }
    }

    ShellExecute(NULL, "open", url_.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

// that solution will sometimes do strange things to the browsers (like opening them in a extreme small window)
//void OpenUrl(const std::string &url_)
//{
//    ShellExecute(NULL, "open", url_.c_str(), NULL, NULL, SW_SHOWNORMAL);
//}

#else   // linux

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

// The linux version depends on the script given here
// That script will have to start a browser with the given webpage
#define BROWSER_SCRIPT "open_browser.sh"

void OpenUrl(const std::string &url_)
{
    int pid;
    switch ( pid=fork())
    {
        case 0: // child
            execl(BROWSER_SCRIPT, BROWSER_SCRIPT, url_.c_str(), (char*)0);

            // should never be reached
            perror("execl failed");
            exit(2);
        case -1:
            perror("fork failed");
            exit(2);
        default:
            // just continue
            break;
    }
}

#endif
