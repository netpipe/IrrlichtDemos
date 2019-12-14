/////////////////////////////////////////////////////////////////////////////
// Name:        alchemedia.cpp
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

#include "alchemedia.h"
#include "alchemediaCanvas.h"

typedef dPluginRecord** (CALLBACK* GetPluginArray)();

int alchemedia::m_physicsMemory = 0;

int main(int argc, char *argv[])
{
	// Enable run-time memory check for debug builds.
#ifdef _MSC_VER
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	QApplication a(argc, argv);
	alchemedia w;
	w.show();
	return a.exec();
}


alchemedia::alchemedia(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
	, m_canvas(NULL)
	, m_menubar(NULL)
	, m_statusbar(NULL)
	, m_toolBar(NULL)
	, m_undoLevels(16)
	, m_newton (NULL)
	, m_currentFileName()
	, m_allPlugins()
	, m_textureCache()
	, m_undoRedoWorlds()
	, m_currentWorld(NULL)
	, m_importPluginCount(0)
	, m_exportPluginsCount(0)
{
	// Set the memory allocation function before creation the world
	// this is the only function that can be called before the creation of the newton world.
	// it should be called once, and the the call is optional 
	NewtonSetMemorySystem (AllocMemory, FreeMemory);


	InitGui ();
	m_newton = NewtonCreate();

	// initialize the Scene for the first time
	OnNewScene();


#if 0
dScene xxxx(m_newton);
xxxx.Deserialize ("C:/Newton200/NewtonSDK/samples/bin/tetra_1.xml");
			   
//xxxx.Deserialize ("C:/Newton_200/NewtonSDK/samples/bin/box_0.xml");
//xxxx.Deserialize ("C:/Newton_200/NewtonSDK/samples/bin/box_1.xml");
//xxxx.Deserialize ("C:/Newton_200/NewtonSDK/samples/bin/box_2.xml");
//xxxx.Deserialize ("C:/Newton_200/NewtonSDK/samples/bin/box_3.xml");
//xxxx.Deserialize ("C:/Newton_200/NewtonSDK/samples/bin/prism_1.xml");
//xxxx.Deserialize ("C:/Newton_200/NewtonSDK/samples/bin/test_2.xml");
//xxxx.Deserialize ("C:/Newton_200/NewtonSDK/samples/bin/hedra_0.xml");
//xxxx.Deserialize ("C:/Newton_200/NewtonSDK/samples/bin/dear_1.xml");
//xxxx.Deserialize ("C:/Newton_200/NewtonSDK/samples/bin/dear_2.xml");
//xxxx.Deserialize ("C:/Newton_200/NewtonSDK/samples/bin/jeep_1.xml");
//xxxx.Deserialize ("C:/Newton_200/NewtonSDK/samples/bin/jeep_2.xml");
//xxxx.Deserialize ("C:/Newton_200/NewtonSDK/samples/bin/ogrehead.xml");
//xxxx.Deserialize ("C:/Newton_200/NewtonSDK/samples/bin/lshape_1.xml");
//xxxx.Deserialize ("C:/Newton_200/NewtonSDK/samples/bin/cshape_1.xml");

//xxxx.Deserialize ("C:/temp/CyrusDebugX86/DebugX86/xxxxx/MTL0001.xml");


dScene::dTreeNode* sceneNode = NULL;
for (void* ptr = xxxx.GetFirstChild (xxxx.GetRootNode()); ptr; ptr = xxxx.GetNextChild(xxxx.GetRootNode(), ptr)){
	dScene::dTreeNode* node = xxxx.GetNodeFromLink(ptr);
	dNodeInfo* const info = xxxx.GetInfoFromNode(node);
	if (info->GetTypeId() == dSceneNodeInfo::GetRttiType()) {
		sceneNode = node;
		break;
	}
}

dScene::dTreeNode* meshNode = NULL;
for (void* ptr = xxxx.GetFirstChild (sceneNode); ptr; ptr = xxxx.GetNextChild(sceneNode, ptr)){
	dScene::dTreeNode* node = xxxx.GetNodeFromLink(ptr);
	dNodeInfo* const info = xxxx.GetInfoFromNode(node);
	if (info->GetTypeId() == dMeshNodeInfo::GetRttiType()) {
		meshNode = node;
		break;
	}
}


/*
NewtonMesh* convexAproximation = NewtonMeshConvexApproximation (meshInfo->GetMesh(), 0.5f);
// create a feature mash
//dScene::dTreeNode* convexNode = xxxx.CreateSceneNode(xxxx.GetRootNode());
//dSceneNodeInfo* const convexInfo = (dSceneNodeInfo*) xxxx.GetInfoFromNode(convexNode);
//dSceneNodeInfo* const sceneInfo = (dSceneNodeInfo*) xxxx.GetInfoFromNode(sceneNode);
//convexInfo->SetTransform(sceneInfo->GetTransform());

//dScene::dTreeNode* convexMeshNode = xxxx.CreateMeshNode(convexNode);
//dMeshNodeInfo* const convexMesh = (dMeshNodeInfo*)xxxx.GetInfoFromNode(convexMeshNode);
//convexMesh->ReplaceMesh(convexAproximation);
//convexMesh->SetPivotMatrix ( meshInfo->GetPivotMatrix());
//xxxx.CreateMaterialNode(convexNode, 0);

// make convex pierces
int count = 0;
NewtonMesh* nextSegment = NULL;
for (NewtonMesh* segment = NewtonMeshCreateFirstSingleSegment (convexAproximation); segment; segment = nextSegment) {
	nextSegment = NewtonMeshCreateNextSingleSegment (convexAproximation, segment);

	NewtonCollision* convexHull = NewtonCreateConvexHullFromMesh (m_newton, segment, 0.01f, 0);
	if (convexHull) {
		NewtonMesh* convexHullMesh = NewtonMeshCreateFromCollision(convexHull);
		NewtonReleaseCollision(m_newton, convexHull);

		dScene::dTreeNode* convexNode = xxxx.CreateSceneNode(xxxx.GetRootNode());
		dSceneNodeInfo* const convexInfo = (dSceneNodeInfo*) xxxx.GetInfoFromNode(convexNode);
		dSceneNodeInfo* const sceneInfo = (dSceneNodeInfo*) xxxx.GetInfoFromNode(sceneNode);
		convexInfo->SetTransform(sceneInfo->GetTransform());

		dScene::dTreeNode* convexMeshNode = xxxx.CreateMeshNode(convexNode);
		dMeshNodeInfo* const convexMesh = (dMeshNodeInfo*)xxxx.GetInfoFromNode(convexMeshNode);
		convexMesh->ReplaceMesh(convexHullMesh);
		convexMesh->SetPivotMatrix ( meshInfo->GetPivotMatrix());
		xxxx.CreateMaterialNode(convexMeshNode, 0);
		count ++;
	}
	NewtonMeshDestroy(segment);
} 
NewtonMeshDestroy(convexAproximation);
*/


dMeshNodeInfo* const meshInfo = (dMeshNodeInfo*) xxxx.GetInfoFromNode(meshNode);
//NewtonMeshFixTJoints (meshInfo->GetMesh());
//NewtonCollision* collision = NewtonCreateCompoundCollisionFromMesh (m_newton, meshInfo->GetMesh(), 30, 0, 0);
NewtonCollision* collision = NewtonCreateConvexHullFromMesh (m_newton, meshInfo->GetMesh(), 0.01f, 0);
NewtonMesh* convexMesh = NewtonMeshCreateFromCollision(collision);
NewtonReleaseCollision(m_newton, collision);
dMeshNodeInfo* const convexMeshInfo = (dMeshNodeInfo*)xxxx.GetInfoFromNode(meshNode);
convexMeshInfo->ReplaceMesh(convexMesh);


xxxx.Serialize("C:/Newton_200/NewtonSDK/samples/bin/test_4.xml");
#endif


}

alchemedia::~alchemedia()
{
	CleanUp();

	NewtonDestroy(m_newton);
	_ASSERTE (m_physicsMemory == 0);

	while (m_allPlugins.GetFirst()) {
		FreeLibrary(m_allPlugins.GetFirst()->GetInfo());
		m_allPlugins.Remove(m_allPlugins.GetFirst());
	}
}

/*
bool alchemedia::event(QEvent *msg)
{
	switch (msg->type()) 
	{
		case QEvent::Resize:
		{
			//QResizeEvent* resize = (QResizeEvent*) msg;
			//QRect rect (frameGeometry());
			QRect rect (childrenRect ());
			if (m_toolBar)	{
				int x1;
				int y1;
				int w1;
				int h1;

				int w = m_toolBar->width();
				int h = m_toolBar->height();
				QRect toolBarRect (m_toolBar->rect());
				rect.getRect(&x1, &y1, &w1, &h1);
				
				y1 += h;   
				h1 -= h;
				rect.setRect(x1, y1, w1, h1);
			}
			if (m_menubar)	{
				int x1;
				int y1;
				int w1;
				int h1;

				int w = m_menubar->width();
				int h = m_menubar->height();
				QRect toolBarRect (m_toolBar->rect());
				rect.getRect(&x1, &y1, &w1, &h1);

				y1 += h;   
				h1 -= h;
				rect.setRect(x1, y1, w1, h1);
			}


			m_canvas->setGeometry(rect);
			break;
		}

	}

	return QMainWindow::event (msg);
}
*/

alchemedia* alchemedia::GetRoot (QObject* const me)
{
	for (QObject* ptr = me; ptr; ptr = ptr->parent()) {
		QString name (ptr->objectName());
		if (name == QString::fromUtf8("Alchemedia")) {
			return (alchemedia*) ptr;
		}
	}
	_ASSERTE (0);
	return NULL;
}

void alchemedia::InitGui ()
{
	setObjectName(QString::fromUtf8("Alchemedia"));
	resize(1024, 768);
	setWindowTitle (QApplication::translate("Alchemedia", "Newton Alchemedia Editor", 0, QApplication::UnicodeUTF8));

//	QAction* const mainAction = new QAction(this);
//	mainAction->setObjectName(QString::fromUtf8("mainAction"));

	// create all menu and toolbars	
	CreateMenus();

	// create the main render Canvas
	m_canvas = new alchemediaCanvas (this);
//	QHBoxLayout* horizontalLayout = new QHBoxLayout(m_canvas);
//	horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
	setCentralWidget(m_canvas);

	QMetaObject::connectSlotsByName(this);
}



void alchemedia::CreateMenus ()
{
	m_menubar = new QMenuBar(this);
	m_menubar->setObjectName(QString::fromUtf8("menubar"));
	m_menubar->setGeometry(QRect(0, 0, 1024, 26));
	setMenuBar(m_menubar);

	// set the status bar
	m_statusbar = new QStatusBar(this);
	m_statusbar->setObjectName(QString::fromUtf8("statusbar"));
	setStatusBar(m_statusbar);

	// add the tool bars
	m_toolBar = new QToolBar(this);
	m_toolBar->setObjectName(QString::fromUtf8("editToolBar"));
	addToolBar(Qt::TopToolBarArea, m_toolBar);
	//connect (m_toolBar, SIGNAL (movableChanged(bool)), this, SLOT (OnUndo()));
	//connect (m_toolBar, SIGNAL (actionTriggered(QAction*)), this, SLOT (OnUndo()));

	// adding the file menu
	{
		QMenu* const subMenu = new QMenu(m_menubar);
		subMenu->setObjectName(QString::fromUtf8("fileMenu"));
		subMenu->setTitle(QApplication::translate("Alchemedia", "File", 0, QApplication::UnicodeUTF8));
		m_menubar->addAction(subMenu->menuAction());

		// file new, open, save
		QAction* actionNew = new QAction(this);
		actionNew->setObjectName(QString::fromUtf8("actionNew"));
		actionNew->setStatusTip(tr("Erase current scene and reset editor"));
		actionNew->setText(QApplication::translate("Alchemedia", "New", 0, QApplication::UnicodeUTF8));
		subMenu->addAction(actionNew);
		connect (actionNew, SIGNAL (triggered(bool)), this, SLOT (OnNewScene()));

		QAction* actionOpen = new QAction(this);
		actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
		actionOpen->setStatusTip(tr("Delete current scene and Open a New one"));
		actionOpen->setText(QApplication::translate("Alchemedia", "Open", 0, QApplication::UnicodeUTF8));
		subMenu->addAction(actionOpen);
		connect (actionOpen, SIGNAL (triggered(bool)), this, SLOT (OnOpenScene()));

		QAction* actionMerge = new QAction(this);
		actionMerge->setObjectName(QString::fromUtf8("actionMerge"));
		actionMerge->setStatusTip(tr("Open a new Scene and merge with the current one"));
		actionMerge->setText(QApplication::translate("Alchemedia", "Merge", 0, QApplication::UnicodeUTF8));
		subMenu->addAction(actionMerge);
		connect (actionMerge, SIGNAL (triggered(bool)), this, SLOT (OnMergeScene()));

		QAction* actionSave = new QAction(this);
		actionSave->setObjectName(QString::fromUtf8("actionSave"));
		actionSave->setStatusTip(tr("Save current scene, override existing file"));
		actionSave->setText(QApplication::translate("Alchemedia", "Save", 0, QApplication::UnicodeUTF8));
		subMenu->addAction(actionSave);
		connect (actionSave, SIGNAL (triggered(bool)), this, SLOT (OnSaveScene()));


		QAction* actionSaveAs = new QAction(this);
		actionSaveAs->setObjectName(QString::fromUtf8("actionSaveAs"));
		actionSaveAs->setStatusTip(tr("Save current scene to a new file"));
		actionSaveAs->setText(QApplication::translate("Alchemedia", "Save As", 0, QApplication::UnicodeUTF8));
		subMenu->addAction(actionSaveAs);
		connect (actionSaveAs, SIGNAL (triggered(bool)), this, SLOT (OnSaveSceneAs()));

		// load plug ins
		subMenu->addSeparator();

		QMenu* const menuImport = new QMenu(subMenu);
		menuImport->setObjectName(QString::fromUtf8("menuImport"));
        menuImport->setTitle(QApplication::translate("Alchemedia", "Import ...", 0, QApplication::UnicodeUTF8));
		subMenu->addAction(menuImport->menuAction());


		QMenu* const menuExport = new QMenu(subMenu);
		menuExport->setObjectName(QString::fromUtf8("menuExport"));
		menuExport->setTitle(QApplication::translate("Alchemedia", "Export ...", 0, QApplication::UnicodeUTF8));
		subMenu->addAction(menuExport->menuAction());

		LoadPlugins(menuImport, menuExport);

		
		// exit menu
		subMenu->addSeparator();
		QAction* actionExit = new QAction(this);
		actionExit->setObjectName(QString::fromUtf8("actionExit"));
		actionExit->setStatusTip(tr("Exit application immediately"));
		actionExit->setText(QApplication::translate("Alchemedia", "Exit", 0, QApplication::UnicodeUTF8));
		subMenu->addAction(actionExit);
		connect (actionExit, SIGNAL (triggered(bool)), qApp, SLOT (quit()));
	}


	// Add edit Menu
	{
		QMenu* const subMenu = new QMenu(m_menubar);
		subMenu->setObjectName(QString::fromUtf8("editMenu"));
		subMenu->setTitle(QApplication::translate("Alchemedia", "Edit", 0, QApplication::UnicodeUTF8));
		m_menubar->addAction(subMenu->menuAction());

		// undo action
		QAction* const undo = new QAction(this);
		undo->setObjectName(QString::fromUtf8("actionUndo"));
		undo->setStatusTip(tr("Undo previous action"));
		undo->setText(QApplication::translate("Alchemedia", "Undo", 0, QApplication::UnicodeUTF8));
		SetActionIcon ("undo.bmp", undo);
		subMenu->addAction(undo);
		m_toolBar->addAction(undo);
		connect (undo, SIGNAL (triggered(bool)), this, SLOT (OnUndo()));

		// redo action
		QAction* const redo = new QAction(this);
		redo->setObjectName(QString::fromUtf8("actionRedo"));
		redo->setStatusTip(tr("Redo previous action"));
		redo->setText(QApplication::translate("Alchemedia", "Redo", 0, QApplication::UnicodeUTF8));
		SetActionIcon ("redo.bmp", redo);
		subMenu->addAction(redo);
		m_toolBar->addAction(redo);
		connect (redo, SIGNAL (triggered(bool)), this, SLOT (OnRedo()));

	}
}

void alchemedia::SetActionIcon (const char* const fileName, QAction* const action)
{
	QPixmap pixmap (QString::fromUtf8(fileName));
	pixmap.setMask(pixmap.createMaskFromColor(QColor(255,0,255)));
	QIcon icon(pixmap);
//	icon.addPixmap(pixmap, QIcon::Normal, QIcon::Off);
	action->setIcon(icon);
}


void alchemedia::LoadPlugins(QMenu* const importMenu, QMenu* const exportMenu)
{
	char plugInPath [2048];
	char rootPathInPath [2048];
	QString path (QApplication::applicationFilePath());
	QByteArray appPath (path.toAscii());

	sprintf (plugInPath, "%s", appPath.data());
	char* ptr = strrchr (plugInPath, '/');
	if (!ptr) {
		ptr = strrchr (plugInPath, '\\');
	}
	*ptr = 0;
	strcat (ptr, "/plugins");
	sprintf (rootPathInPath, "%s/*.dll", plugInPath);


	_finddata_t data;
	intptr_t handle = _findfirst (rootPathInPath, &data);
	if (handle != -1) {
		do {
			sprintf (rootPathInPath, "%s/%s", plugInPath, data.name);

			HMODULE module = LoadLibrary(rootPathInPath);
			if (module) {
				// get the interface function pointer to the Plug in classes
				GetPluginArray GetPluginsTable = (GetPluginArray) GetProcAddress (module, "GetPluginArray"); 
				if (GetPluginsTable) {
					m_allPlugins.Append(module);
				} else {
					FreeLibrary(module);
				}
			}
		} while (_findnext (handle, &data) == 0);
		_findclose (handle);
	}

	int id_importIndex = 0;
	int id_exportIndex = 0;
	for (alchemdiaPluginDlls::dListNode* dllNode = m_allPlugins.GetFirst(); dllNode; dllNode = dllNode->GetNext()) {
		HMODULE module = dllNode->GetInfo();
		GetPluginArray GetPluginsTable = (GetPluginArray) GetProcAddress (module, "GetPluginArray"); 
		dPluginRecord** table = GetPluginsTable();

		for (int i = 0; table[i]; i ++) {
			dPluginRecord* const plugin = table[i];

			switch (plugin->GetType())
			{
				case dPluginRecord::m_ImportPlugin:
				{
					char actionName[256];
					sprintf (actionName, "actionImportDll_%03d", id_importIndex) ;

					dImportPlugin* const importPlugin = (dImportPlugin*) plugin;
					const char* const menuName = importPlugin->GetMenuName ();
					const char* const description = importPlugin->GetFileDescription ();
					QAction* const action = new QAction(this);

					qlonglong value = qlonglong(importPlugin);
					action->setData(QVariant (value));
					action->setObjectName(QString::fromUtf8(actionName));
					action->setStatusTip(tr(description));
					action->setText(QApplication::translate("Alchemedia", menuName, 0, QApplication::UnicodeUTF8));
					importMenu->addAction(action);
					
					connect (action, SIGNAL (triggered(bool)), this, SLOT (OnImport()));

					id_importIndex ++;
					break;
				}

				case dPluginRecord::m_ExportPlugin:
				{
					char actionName[256];
					sprintf (actionName, "actionExportDll_%03d", id_exportIndex);

					dExportPlugin* const exportPlugin = (dExportPlugin*) plugin;
					const char* const menuName = exportPlugin->GetMenuName ();
					const char* const description = exportPlugin->GetFileDescription ();
					QAction* const action = new QAction(this);

					qlonglong value = qlonglong(exportPlugin);
					action->setData(QVariant (value));
					action->setObjectName(QString::fromUtf8(actionName));
					action->setStatusTip(tr(description));
					action->setText(QApplication::translate("Alchemedia", menuName, 0, QApplication::UnicodeUTF8));
					exportMenu->addAction(action);
					connect (action, SIGNAL (triggered(bool)), this, SLOT (OnExport()));

					id_exportIndex ++;


					break;
				}
			}
		}
	}

}



// memory allocation for the Newton library
void* alchemedia::AllocMemory(int sizeInBytes)
{
	m_physicsMemory += sizeInBytes;
	return new char[sizeInBytes];
}

// free memory use by Newton engine
void alchemedia::FreeMemory(void* const ptr, int sizeInBytes)
{
	m_physicsMemory -= sizeInBytes;
	delete[] (char*)ptr;
}







alchemediaScene* alchemedia::GetCurWorld(void) const
{
	return m_currentWorld ? &m_currentWorld->GetInfo() : NULL;
}





void alchemedia::CleanUp()
{
	// release all texture from all world
	m_undoRedoWorlds.RemoveAll();
}



alchemediaScene* alchemedia::NewWorldInstance ()
{
	while (m_currentWorld->GetNext()) {
		m_undoRedoWorlds.Remove (m_currentWorld->GetNext());
	}

	while (m_undoRedoWorlds.GetCount() >= m_undoLevels) {
		m_undoRedoWorlds.Remove (m_undoRedoWorlds.GetFirst());
	}

	//	neSceneExplorer* explorer = (neSceneExplorer*) ((neMainFrame*)wxGetApp().GetTopWindow())->GetCommandPanel()->FindWindow(_T("neSceneExplorer"));
	//	explorer->SaveState (GetCurWorld());

	m_currentWorld = m_undoRedoWorlds.Append(m_currentWorld->GetInfo());
	return GetCurWorld();
}



void alchemedia::OnNewScene()
{
	CleanUp();

//	_ASSERTE (m_explorer);
	m_currentFileName = QString();
	m_currentWorld = m_undoRedoWorlds.Append();
	m_currentWorld->GetInfo().SetTextCache(&m_textureCache);
	m_currentWorld->GetInfo().SetNewtonWorld (m_newton);
//	m_currentWorld->GetInfo().SetExplorer (m_explorer);
//	m_explorer->PopulateScene(&m_currentWorld->GetInfo());
}

void alchemedia::OnOpenScene()
{
	QString fileName (QFileDialog::getOpenFileName (this, tr("Load new Alchemedia scene"), "", tr("alchemedia (*.xml *.bin)")));
	if (fileName != "") {
		OnNewScene();
		m_currentFileName = fileName;
		
		alchemediaScene* const world = GetCurWorld();

		QByteArray name (fileName.toAscii());
		world->Deserialize(name.data());
		//m_explorer->PopulateScene(world);
		//m_mainFrame->Invalidate();
	}
}

void alchemedia::OnMergeScene()
{
	QString fileName (QFileDialog::getOpenFileName (this, tr("Merge Alchemedia scene"), "", tr("alchemedia (*.xml *.bin)")));
	if (fileName != "") {
		//alchemediaScene* world = GetCurWorld();
		alchemediaScene* const world = NewWorldInstance();
		alchemediaScene tmpScene (&m_textureCache);
		tmpScene.SetNewtonWorld (world->GetNewtonWorld());

		QByteArray name (fileName.toAscii());
		if (tmpScene.Deserialize(name.data())) {
			world->MergeScene (&tmpScene);
			world->MergeDuplicateTextures();

			//m_explorer->PopulateScene(world);
			world->UpdateAllOOBB ();
			//m_mainFrame->Invalidate();
		}
	}
}


void alchemedia::OnSaveScene()
{
	// TODO: Add your command handler code here
	if (m_currentFileName == "") {
		OnSaveSceneAs();
	} else {

		QByteArray tmp (m_currentFileName.toAscii());
		char name[2048];
		sprintf (name, "%s", tmp.data());

		const char* ptr = NULL;
		for (ptr = strrchr (name, '.'); ptr && (strchr (ptr, '/') || strchr (ptr, '\\')); ptr = strrchr (ptr, '.')) { 
			ptr ++;
		}
		if (ptr == NULL) {
			ptr = ALCHEMEDIA_FORMAT_ASCII_EXT;
			strcat (name, ALCHEMEDIA_FORMAT_ASCII_EXT);
		}

		if (!stricmp (ptr, ALCHEMEDIA_FORMAT_ASCII_EXT)) {
			GetCurWorld()->Serialize (name);
		} else if (!stricmp (ptr, ALCHEMEDIA_FORMAT_BINAY_EXT)) {
			_ASSERTE (0);
			//world->SerializeBinary (filename);
		}
	}
}

void alchemedia::OnSaveSceneAs()
{
	QString fileName (QFileDialog::getSaveFileName (this, tr("Save new Alchemedia scene"), "", tr("alchemedia (*.xml *.bin)")));
	if (fileName != "") {
		m_currentFileName = fileName;
		OnSaveScene();
	}
}

void alchemedia::OnExport()
{
	char tmp[1024];

	QAction* const action = (QAction*)sender();
	dExportPlugin* const exportPlugin = (dExportPlugin*) action->data().toLongLong();
	const char* const ext = exportPlugin->GetFileExtension ();
	const char* const description = exportPlugin->GetFileDescription();
	sprintf (tmp, "export filter (*.%s)", ext);

	QString fileName (QFileDialog::getSaveFileName (this, tr(description), "", tr(tmp)));
	if (fileName != "") {
		QByteArray fileNamePath (fileName.toAscii());
		exportPlugin->Export(fileNamePath.data(), GetCurWorld());
		//		Invalidate();
	}
}

void alchemedia::OnImport()
{
	char tmp[1024];

	QAction* const action = (QAction*)sender();
	dImportPlugin* const importPlugin = (dImportPlugin*) action->data().toLongLong();
	const char* const ext = importPlugin->GetFileExtension ();
	const char* const description = importPlugin->GetFileDescription();
	sprintf (tmp, "import filter (*.%s)", ext);

	QString fileName (QFileDialog::getOpenFileName (this, tr(description), "", tr(tmp)));
	if (fileName != "") {
		alchemediaScene* const world = NewWorldInstance();
		QByteArray fileNamePath (fileName.toAscii());
		if (importPlugin->Import(fileNamePath.data(), world)) {
			char texPath[256];
			dExtractPathFromFullName (fileNamePath.data(), texPath);

			world->MergeDuplicateTextures();
			world->LoadTextures (texPath);
			world->UpdateAllOOBB ();
			world->RemoveUnusedVertex();
			//m_document->m_explorer->PopulateScene(world);
			//Invalidate();
		}
	}
}

void alchemedia::OnRedo()
{

}

void alchemedia::OnUndo()
{

}