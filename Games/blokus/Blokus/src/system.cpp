
#include "system.h"

//アイコンの設定
void sys_setIcon(HINSTANCE hInst)
{
	#ifndef WIN32
		return;
	#else
		//アイコン変更（EXEはリソース追加だけでOK）
		SExposedVideoData exposedData = driver->getExposedVideoData();
		HWND hWnd = reinterpret_cast<HWND>(exposedData.OpenGLWin32.HWnd);	//OpenGL
		//HWND hWnd = FindWindow(NULL,"IrrLicht");
		HICON hIcon = (HICON)LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));//リソースから
		SendMessage(hWnd,WM_SETICON,ICON_SMALL,(LPARAM)hIcon);
	#endif
}

//カメラの設定
void sys_setCamera(IVideoDriver	*driver, ISceneManager *manager, bool bOrtho)
{
	matrix4 matrix;
	dimension2d<u32> Screensize = driver->getScreenSize();
	ICameraSceneNode *CameraNode;

	CameraNode = manager->addCameraSceneNode(0,vector3df(0,0,-400),vector3df(0,0,0),-1);

	//これで並行投影設定
	if(bOrtho)
	{
		matrix.buildProjectionMatrixOrthoLH(Screensize.Width,Screensize.Height,-1000,1000); //w,h,near,far
		CameraNode->setProjectionMatrix(matrix);
	}
}

#include <sys/time.h>

unsigned int timeGetTime()
{
struct timeval now;
gettimeofday(&now, NULL);
return now.tv_usec/1000;
}

//FPSの設定
//true :描画OK
//false:Sleep(1)で待機
bool sys_checkFps(s32 frame)
{
	u32 frameBase = 3000/frame;	//更新時間(計算しやすいように3倍)
	static u32 lastTime  = 0;	//前回の時間
	static u32 errorTime = 0;	//誤差値
	u32 currentTime;			//現在の時間
	u32 pastTime;				//経過時間

	currentTime  = timeGetTime();
	pastTime     = (currentTime - lastTime) * 3 + errorTime;
	if(pastTime >= frameBase)
	{
		lastTime  = currentTime;
		errorTime = pastTime % frameBase;
		return true;
	}
	return false;
}

