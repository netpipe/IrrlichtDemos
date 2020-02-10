
#include <windows.h>
#include "DeviceSettings.h"
#include "stringaling.h"

#define SETTINGS_FILENAME "launcher.ini"

bool runApp(char *appName)
{
    STARTUPINFO sinfo;
    FillMemory(&sinfo, sizeof(STARTUPINFO), 0);
    sinfo.cb = sizeof(STARTUPINFO);

    PROCESS_INFORMATION pinfo;

    if (CreateProcess(appName, NULL, NULL, NULL, FALSE,
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

void deviceSettingsErrorMsg(char *msg)
{
    MessageBox(NULL, msg, "Error", MB_ICONERROR);
}

void deviceSettingsOpenWebpage(char *page)
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
        // otherwise if no quotes, 2nd point is the first space after the last \\
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
        command += page;
        command += "\"";
        
        if (runApp((char *)command.c_str()))
        {
            return;
        }
    }
    
    ShellExecute(NULL, "open", page, NULL, NULL, SW_SHOWNORMAL);
}

std::string getSettingsFilePath()
{
    std::string SETTINGS_FILEPATH = "";
    
    TCHAR cwd[MAX_PATH] = "";
    
    if(!GetCurrentDirectory(sizeof(cwd) - 1, cwd))
    {
        MessageBox(NULL, "Installation directory exceeds MAX_PATH.", "Error", MB_ICONERROR);
        return 0;
    }
    
    SETTINGS_FILEPATH = cwd;
    SETTINGS_FILEPATH += "\\";
    SETTINGS_FILEPATH += SETTINGS_FILENAME;
    
    return SETTINGS_FILEPATH;
}

DeviceSettings getSettings()
{
    std::string SETTINGS_FILEPATH = getSettingsFilePath();
    
    DeviceSettings ds;
    
    // if settings file does not exist or is corrupted, sets defaults as specified in GetPrivateProfileString.
    char buf[256] = {0};
    int bufSize = 256;
    
    // driver, opengl or software
    GetPrivateProfileString("screen", "driver", "direct3d8", buf,bufSize, SETTINGS_FILEPATH.c_str());
    if (strcmp(buf, "direct3d8") == 0)
    {
        ds.driver = 0;
    }
    else if (strcmp(buf, "opengl") == 0)
    {
        ds.driver = 1;
    }
    else
    {
        ds.driver = 2;
    }
    
    GetPrivateProfileString("screen", "width", "1024", buf,bufSize, SETTINGS_FILEPATH.c_str());
    ds.width = fromString<int>(buf);
    
    GetPrivateProfileString("screen", "height", "768", buf,bufSize, SETTINGS_FILEPATH.c_str());
    ds.height = fromString<int>(buf);
    
    GetPrivateProfileString("screen", "bits", "16", buf,bufSize, SETTINGS_FILEPATH.c_str());
    ds.bits = fromString<int>(buf);
    
    GetPrivateProfileString("screen", "full", "1", buf,bufSize, SETTINGS_FILEPATH.c_str());
    ds.fullscreen = fromString<int>(buf);
    
    return ds;
}
