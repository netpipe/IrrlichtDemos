/* Copyright (c) <2009> <Newton Game Dynamics>
* 
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely
*/

#ifndef __DEMO_ENTITY_MANAGER_H__
#define __DEMO_ENTITY_MANAGER_H__

#include "dRuntimeProfiler.h"
#include "dSimulationThread.h"
#include "dHeightResolutionTimer.h"



class DemoEntity;
class DemoCamera;

class DemoEntityManager: public QGLWidget,  public dList <DemoEntity*>, public dSimulationThread
{
	public:
	class EntityDictionary: public dTree<DemoEntity*, dScene::dTreeNode*>
	{
	};


	DemoEntityManager(QWidget* const parent);
	~DemoEntityManager(void);

//	GLContext& GetGL() {return m_glContext;}
	NewtonWorld* GetNewton() const;
	DemoCamera* GetCamera() const;


	void UpdatePhysics();

	void Cleanup ();
	void LoadScene (const char* const name);
	void SaveScene (const char* const name);
	dScene CreateAlchemediaFromPhysic(); 

	void SetAutoSleepState (bool state);

	private:
	void InterpolateMatrices ();
	void LoadVisualScene(dScene* const scene, EntityDictionary& dictionary);


	void initializeGL();
	void resizeGL(int w, int h);
	void paintEvent(QPaintEvent* ev);

	void Print (QPainter& painter, Qt::GlobalColor color, int x, int y, const char *fmt, ... ) const;

		//GLContext m_glContext;
	DemoCamera* m_camera;
	NewtonWorld* m_world;

	bool m_physicsUpdate;
	bool m_reEntrantUpdate;

	// some utility functionality
	unsigned64 m_microsecunds;
	dHeightResolutionTimer m_timer;

	int m_showProfiler[8]; 
	dRuntimeProfiler m_profiler;

	QFont m_font;
};


#endif