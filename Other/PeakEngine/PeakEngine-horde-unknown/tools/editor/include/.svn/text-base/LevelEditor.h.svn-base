
/*
Copyright (C) 2008   Mathias Gottschlag

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in the
Software without restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef _LEVELEDITOR_H_
#define _LEVELEDITOR_H_

#include <gtkmm.h>
#include <libglademm/xml.h>

#include "Horde3DWidget.h"
#include "InputReceiver.h"

class LevelData;

class LevelEditor : public InputReceiver
{
	public:
		LevelEditor(Horde3DWidget *hordewidget, Glib::RefPtr<Gnome::Glade::Xml> ui);
		~LevelEditor();
		
		void show(void);
		void hide(void);
		
		bool openLevel(std::string levelname);
		bool saveLevel(std::string levelname = "");
		void newLevel(std::string levelname = "");
		bool closeLevel(void);
		
		virtual void onMouseMoved(int x, int y, int dx, int dy);
		virtual void onMouseButton(int x, int y, int button, int state);
		
		virtual void onOpenLevel(void);
		virtual void onOpenLevelOk(void);
		virtual void onOpenLevelCancel(void);
		virtual void onSaveLevel(void);
		virtual void onSaveLevelOk(void);
		virtual void onSaveLevelCancel(void);
		virtual void onNewLevel(void);
		
	private:
		Horde3DWidget *hordewidget;
		Glib::RefPtr<Gnome::Glade::Xml> ui;
		
		LevelData *data;
		std::string levelname;
		
		bool initialized;
		int camera;
		int pipeline;
		
		float camerapos[3];
		float camerarot[3];
};

#endif

