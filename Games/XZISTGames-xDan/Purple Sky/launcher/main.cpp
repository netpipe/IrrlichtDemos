#include <windows.h>
#include "dialog.h"
#include <vector>
#include <string>
#include "stringaling.h"
#include "DeviceSettings.h"

std::string SETTINGS_FILEPATH;

using namespace std;


struct VideoMode
{
    int width,height,bits;
    std::string str;
};

std::vector <VideoMode> videoModes;

bool hasVideoMode(DEVMODE *mode)
{
    for (int i = 0; i < videoModes.size(); i ++)
    {
        if (videoModes[i].width == mode->dmPelsWidth && videoModes[i].height == mode->dmPelsHeight
            && videoModes[i].bits == mode->dmBitsPerPel)
        {
            return true;
        }
    }
    return false;
}

int autoVideoMode()
{
    int a = 0;
    for (int i = 0; i < videoModes.size(); i ++)
    {
        if (videoModes[i].width <= 1024 && videoModes[i].height <= 768 && videoModes[i].bits >=16
            && videoModes[a].width <= videoModes[i].width && videoModes[a].height <= videoModes[i].height
            && videoModes[a].bits <= videoModes[i].bits
            )
        {
            a = i;
        }
    }
    return a;
}

int findVideoMode(int width, int height, int bits)
{
    for (int i = 0; i < videoModes.size(); i ++)
    {
        if (videoModes[i].width == width && videoModes[i].height == height
            && videoModes[i].bits == bits)
        {
            return i;
        }
    }
    return autoVideoMode();
}

void getVideoModes()
{
	DWORD i = 0;
	DEVMODE mode;
	while (EnumDisplaySettings(NULL, i, &mode))
	{
        if (mode.dmBitsPerPel >= 16 && !hasVideoMode(&mode))
        {
            VideoMode videoMode = {mode.dmPelsWidth, mode.dmPelsHeight, mode.dmBitsPerPel};
            videoMode.str = stringify(videoMode.width)+"x"+stringify(videoMode.height)+"x"+stringify(videoMode.bits);
            videoModes.push_back(videoMode);
        }
        
		i ++;
    }
}

HWND hwndCombo_videoDriver;
HWND hwndCombo_screenMode;
HWND hwndCheck_fullScreen;

void saveSettings()
{
    int driver = SendMessage(hwndCombo_videoDriver, CB_GETCURSEL, 0, 0);
    if (driver == 0)
    {
        WritePrivateProfileString("screen", "driver", "direct3d8", SETTINGS_FILEPATH.c_str());
    }
    else if (driver == 1)
    {
        WritePrivateProfileString("screen", "driver", "opengl", SETTINGS_FILEPATH.c_str());
    }
    else
    {
        WritePrivateProfileString("screen", "driver", "software", SETTINGS_FILEPATH.c_str());
    }
    
    int i = SendMessage(hwndCombo_screenMode, CB_GETCURSEL, 0, 0);
    if (i >= 0 && i < videoModes.size())
    {
        WritePrivateProfileString("screen", "width", stringify(videoModes[i].width).c_str(), SETTINGS_FILEPATH.c_str());
        WritePrivateProfileString("screen", "height", stringify(videoModes[i].height).c_str(), SETTINGS_FILEPATH.c_str());
        WritePrivateProfileString("screen", "bits", stringify(videoModes[i].bits).c_str(), SETTINGS_FILEPATH.c_str());
    }
    
    if (SendMessage(hwndCheck_fullScreen, BM_GETSTATE, 0, 0) == BST_CHECKED)
    {
        WritePrivateProfileString("screen", "full", "1", SETTINGS_FILEPATH.c_str());
    }
    else
    {
        WritePrivateProfileString("screen", "full", "0", SETTINGS_FILEPATH.c_str());
    }
}


void loadSettings()
{
    DeviceSettings ds = getSettings();
    
    SendMessage(hwndCombo_videoDriver, CB_SETCURSEL, ds.driver, 0);
    SendMessage(hwndCombo_screenMode, CB_SETCURSEL, findVideoMode(ds.width,ds.height,ds.bits), 0);
    
    if (ds.fullscreen == 0)
    {
        SendMessage(hwndCheck_fullScreen, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
    }
    else
    {
        SendMessage(hwndCheck_fullScreen, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
    }
}

BOOL CALLBACK ControlDialogProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch(message)
    {
    case WM_INITDIALOG:
        // setup controls with default settings
        hwndCombo_videoDriver = GetDlgItem(hwnd, IDC_CBO_VIDEODRIVER);
        hwndCombo_screenMode = GetDlgItem(hwnd, IDC_CBO_SCREENMODE);
        hwndCheck_fullScreen = GetDlgItem(hwnd, IDC_CHK_FULLSCREEN);
        
        SendMessage(hwndCombo_videoDriver, CB_ADDSTRING, 0, (LPARAM)"Direct3D");
        SendMessage(hwndCombo_videoDriver, CB_ADDSTRING, 0, (LPARAM)"OpenGL");
        SendMessage(hwndCombo_videoDriver, CB_ADDSTRING, 0, (LPARAM)"Software Renderer");
        
        for (int i = 0; i < videoModes.size(); i ++)
        {
            SendMessage(hwndCombo_screenMode, CB_ADDSTRING, 0, (LPARAM)videoModes[i].str.c_str());
        }
        
        loadSettings();
        return true;
    
    case WM_COMMAND:
        switch(LOWORD(wparam))
        {
        case IDC_BTN_PLAY:
            {
                SendMessage(hwnd, WM_CLOSE, 0, 0);
                saveSettings();
                // do something
                if (!runApp("game.exe"))
                {
                    MessageBox(NULL, "Main executable could not be found! Please reinstall the game.",
                        "Error", MB_ICONERROR);
                }
                break;
            }
        case IDC_BTN_EXIT:
            SendMessage(hwnd, WM_CLOSE, 0, 0);
            saveSettings();
            break;
        }
        break;
    
    case WM_CLOSE:
        EndDialog(hwnd, false);
        break;
    
    default:
        return false;
    
    }
    
    return true;
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, LPSTR, INT)
{
    SETTINGS_FILEPATH = getSettingsFilePath();
    
    getVideoModes();
    
    if (videoModes.size() == 0)
    {
        MessageBox(NULL, "Your system doesn't appear to have a working display device.",
            "Error", MB_ICONERROR);
        return 0;
    }
    
    DialogBox(hinstance, MAKEINTRESOURCE(IDD_LAUNCHER), GetDesktopWindow(), ControlDialogProc);
    return 0;
}
