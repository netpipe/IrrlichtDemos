/////////////////////////////////////////////////////////////////////////////
// Name:        alchemediaCanvas.cpp
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
#include "alchemediaViewport.h"

class alchemediaSplietHandle: public QSplitterHandle
{
	public:
	alchemediaSplietHandle(Qt::Orientation o, QSplitter *parent)
		:QSplitterHandle (o, parent)
	{

	}

	void mouseMoveEvent(QMouseEvent *ev)
	{
		QWidget* const myParent = (QSplitter*) parent();	

		int stack = 1;
		QObject* pool[32];

		pool[0] = (QSplitter*) parent();	
		while (stack) {
			stack --;
			QObject* const myParent = pool[stack];	

			QVariant prop (myParent->property("viewport"));
			if (!prop.isNull()) {
				alchemediaViewport* const viewport = (alchemediaViewport*) myParent;
				if (viewport->m_maximized) {
					return; 
				}
			}
			const QObjectList &children = myParent->children();
			for (int i = 0; i < children.size(); i ++) {
				QObject* const child = children[i];
				pool[stack] = child;
				stack ++;
			}
		}

		QSplitterHandle::mouseMoveEvent(ev);
	}
};



alchemediaVerticalSpliter::alchemediaVerticalSpliter(QWidget* const parent)
	:QSplitter (Qt::Vertical, parent), m_updating(false)
{
	alchemediaViewport* const topViewport = new alchemediaViewport(this);
	alchemediaViewport* const bottomViewport = new alchemediaViewport(this);

	addWidget(topViewport);
	addWidget(bottomViewport);

	topViewport->setObjectName(QString::fromUtf8("topViewport")); 
	bottomViewport->setObjectName(QString::fromUtf8("bottomViewport")); 
}

void alchemediaVerticalSpliter::OnSpliterMove(int pos, int index)
{
	if (!m_updating) {
		m_updating = true;
		moveSplitter(pos, index);
		m_updating = false;
	}
}


void alchemediaVerticalSpliter::MoveSpliter(int position)
{
	moveSplitter(position, 1);
}

void alchemediaVerticalSpliter::SaveState()
{
	m_maximizedState = saveState();
}

void alchemediaVerticalSpliter::RestoreState()
{
	restoreState(m_maximizedState);	
}

QSplitterHandle *alchemediaVerticalSpliter::createHandle()
{
	return new alchemediaSplietHandle (Qt::Vertical, this);
}


alchemediaCanvas::alchemediaCanvas(QWidget* const parent)
	:QSplitter (Qt::Horizontal, parent), m_leftPane(NULL), m_rightPane(NULL)
{
	setObjectName(QString::fromUtf8("horizontalSpliter"));

	m_leftPane = new alchemediaVerticalSpliter (this);
	m_rightPane = new alchemediaVerticalSpliter (this);

	m_leftPane->setObjectName(QString::fromUtf8("leftSpliter")); 
	m_rightPane->setObjectName(QString::fromUtf8("rightSpliter")); 

	addWidget(m_leftPane);
	addWidget(m_rightPane);

	connect (m_leftPane, SIGNAL (splitterMoved(int, int)), m_rightPane, SLOT (OnSpliterMove(int, int)));
	connect (m_rightPane, SIGNAL (splitterMoved(int, int)), m_leftPane, SLOT (OnSpliterMove(int, int)));
}


alchemediaCanvas::~alchemediaCanvas(void)
{
}

QSplitterHandle *alchemediaCanvas::createHandle()
{
	return new alchemediaSplietHandle (Qt::Horizontal, this);
}

void alchemediaCanvas::MoveSpliter(int position)
{
	moveSplitter(position, 1);
}



void alchemediaCanvas::SaveState()
{
	m_maximizedState = saveState();
	m_leftPane->SaveState();
	m_rightPane->SaveState();
}

void alchemediaCanvas::RestoreState()
{
	m_leftPane->RestoreState();
	m_rightPane->RestoreState();
	restoreState(m_maximizedState);	
}
