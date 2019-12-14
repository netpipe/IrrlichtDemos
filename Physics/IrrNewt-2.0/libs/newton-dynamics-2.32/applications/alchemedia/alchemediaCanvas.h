/////////////////////////////////////////////////////////////////////////////
// Name:        alchemediaCanvas.h
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

#ifndef _ALCHEMEDIA_CANVAS_H_
#define _ALCHEMEDIA_CANVAS_H_




class alchemediaVerticalSpliter: public QSplitter
{
	Q_OBJECT
	public:

	alchemediaVerticalSpliter(QWidget* const parent);


	void SaveState();
	void RestoreState();
	void MoveSpliter(int position);

	QSplitterHandle *createHandle();

	public slots:
	void OnSpliterMove(int pos, int index); 

	private:
	bool m_updating;
	QByteArray m_maximizedState;
	
};


class alchemediaCanvas: public QSplitter 
{
	public:
	alchemediaCanvas(QWidget* const parent);
	~alchemediaCanvas(void);

	void SaveState();
	void RestoreState();
	void MoveSpliter(int position);

	QSplitterHandle *createHandle();

	alchemediaVerticalSpliter* m_leftPane;
	alchemediaVerticalSpliter* m_rightPane;
	QByteArray m_maximizedState;
};


#endif