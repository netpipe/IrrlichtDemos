/////////////////////////////////////////////////////////////////////////////
// Name:        alchemedia.h
// Purpose:     
// Author:      Julio Jerez
// Modified by: 
// Created:     22/05/2010 08:02:08
// RCS-ID:      
// Copyright:   Copyright (c) <2010> <Newton Game Dynamics>
// License:     
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
// 
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely
/////////////////////////////////////////////////////////////////////////////


#ifndef ALCHEMEDIA_H
#define ALCHEMEDIA_H

#include <io.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <Newton.h>

#include <tinyxml.h>
#include <dStdAfxMath.h>
#include <dContainersStdAfx.h>


#include <dMathDefines.h>
#include <dVector.h>
#include <dMatrix.h>
#include <dQuaternion.h>

#include <dCRC.h>
#include <dRtti.h>
#include <dHeap.h>
#include <dList.h>
#include <dTree.h>
#include <dClassInfo.h>
#include <dRefCounter.h>
#include <dBaseHierarchy.h>

#include "dPluginStdafx.h"
#include "dScene.h"
#include "dNodeInfo.h"
#include "dDrawUtils.h"
#include "dRootNodeInfo.h"
#include "dMeshNodeInfo.h"
#include "dBoneNodeInfo.h"
#include "dSceneNodeInfo.h"
#include "dCameraNodeInfo.h"
#include "dTextureNodeInfo.h"
#include "dMaterialNodeInfo.h"
#include "dGeometryNodeInfo.h"
#include "dGeometryNodeSkinModifierInfo.h"

#include "dPluginRecord.h"
#include "dImportPlugin.h"
#include "dExportPlugin.h"

#include "alchemediaScene.h"
#include "alchemediaTexCache.h"

#include <QtCore/QList>
#include <QtGui/QApplication>
#include <QtGui/QMainWindow>
#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QFileDialog>
#include <QtCore/QVariant>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>
#include <QtGui/QIcon>
#include <QtGui/QPixmap>
#include <QtGui/QBitmap>
#include <QtGui/QTextEdit>
#include <QtGui/QResizeEvent>
#include <QtGui/QHBoxLayout>
#include <QtOpenGL/QGLWidget>

#define	ALCHEMEDIA_FORMAT_ASCII_EXT		".xml" 	
#define	ALCHEMEDIA_FORMAT_BINAY_EXT		".new" 	


class alchemediaCanvas;

class alchemedia : public QMainWindow
{
	Q_OBJECT

	class alchemediaUndoRedoWorlds: public dList<alchemediaScene>
	{
	};

	class alchemdiaPluginDlls: public dList <HMODULE>
	{
	};

	
	public:
	alchemedia(QWidget *parent = 0, Qt::WFlags flags = 0);
	~alchemedia();

	static alchemedia* GetRoot (QObject* const me);

	void InitGui ();
	void CreateMenus();
	void SetActionIcon (const char* const fileName, QAction* const action);
	void LoadPlugins(QMenu* const importMenu, QMenu* const exportMenu);

	alchemediaScene* GetCurWorld(void) const;

	public slots:
	void OnNewScene(); 
	void OnOpenScene();
	void OnMergeScene();
	void OnSaveScene(); 
	void OnSaveSceneAs(); 

	void OnImport(); 
	void OnExport(); 

	void OnUndo(); 
	void OnRedo(); 

	private:
	// memory allocation for the Newton library
	static void* AllocMemory(int sizeInBytes);
	static void FreeMemory(void* const ptr, int sizeInBytes);

//	virtual bool event(QEvent *msg);

	void CleanUp();
	alchemediaScene* NewWorldInstance ();

	static int m_physicsMemory;

	alchemediaCanvas *m_canvas;
	QMenuBar* m_menubar;
	QStatusBar* m_statusbar;
	QToolBar* m_toolBar;
	

	int m_undoLevels;
	NewtonWorld* m_newton;	
	QString m_currentFileName;
	alchemdiaPluginDlls m_allPlugins;
	alchemediaTextureCache m_textureCache;
	alchemediaUndoRedoWorlds m_undoRedoWorlds;
	alchemediaUndoRedoWorlds::dListNode* m_currentWorld;  

	int m_importPluginCount;
	int m_exportPluginsCount;
	dImportPlugin** m_importPlugins;
	dExportPlugin** m_exportPlugins;


//	Ui::Alchemedia m_ui;
};

#endif // ALCHEMEDIA_H
