
#include "system.h"

//�A�C�R���̐ݒ�
void sys_setIcon(HINSTANCE hInst)
{
	#ifndef WIN32
		return;
	#else
		//�A�C�R���ύX�iEXE�̓��\�[�X�ǉ�������OK�j
		SExposedVideoData exposedData = driver->getExposedVideoData();
		HWND hWnd = reinterpret_cast<HWND>(exposedData.OpenGLWin32.HWnd);	//OpenGL
		//HWND hWnd = FindWindow(NULL,"IrrLicht");
		HICON hIcon = (HICON)LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));//���\�[�X����
		SendMessage(hWnd,WM_SETICON,ICON_SMALL,(LPARAM)hIcon);
	#endif
}

//�J�����̐ݒ�
void sys_setCamera(IVideoDriver	*driver, ISceneManager *manager, bool bOrtho)
{
	matrix4 matrix;
	dimension2d<u32> Screensize = driver->getScreenSize();
	ICameraSceneNode *CameraNode;

	CameraNode = manager->addCameraSceneNode(0,vector3df(0,0,-400),vector3df(0,0,0),-1);

	//����ŕ��s���e�ݒ�
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

//FPS�̐ݒ�
//true :�`��OK
//false:Sleep(1)�őҋ@
bool sys_checkFps(s32 frame)
{
	u32 frameBase = 3000/frame;	//�X�V����(�v�Z���₷���悤��3�{)
	static u32 lastTime  = 0;	//�O��̎���
	static u32 errorTime = 0;	//�덷�l
	u32 currentTime;			//���݂̎���
	u32 pastTime;				//�o�ߎ���

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

