//utils.h - 

#pragma once


#define SAFE_DELETE(p)			{if(p) {delete (p);		(p)=0;	}}
#define SAFE_DELETE_ARRAY(p)	{if(p) {delete [] (p);	(p)=0;	}}

//useful define to get pointers to commonly used irrlicht objects
#define GET_SMGR scene::ISceneManager*		smgr = g_pIrrDevice->getSceneManager();
#define GET_GUIENV gui::IGUIEnvironment*	guienv = g_pIrrDevice->getGUIEnvironment();
#define GET_CURSORCTRL gui::ICursorControl* cursorctrl = g_pIrrDevice->getCursorControl();
#define GET_DRIVER video::IVideoDriver*		driver = g_pIrrDevice->getVideoDriver();
#define GET_SCENEROOT ISceneNode*			sceneroot = g_pIrrDevice->getSceneManager()->getRootSceneNode();
#define GET_FILESYSTEM io::IFileSystem*		filesystem = g_pIrrDevice->getFileSystem();

#ifndef GRAD_PI 
#define GRAD_PI 57.29577951f
#endif
#ifndef GRAD_PI2
#define GRAD_PI2 0.0174532925f
#endif
