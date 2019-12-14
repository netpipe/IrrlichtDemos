#include "alchemedia.h"
#include "alchemediaCanvas.h"
#include "alchemediaViewport.h"

alchemediaViewport::alchemediaViewport(alchemediaVerticalSpliter* const myParent)
	:QWidget(myParent), m_maximized(false)
{
	alchemedia* const root = alchemedia::GetRoot(myParent);

	setProperty("viewport", QVariant(true));

	QToolBar* const toolBar = new QToolBar(this); 

	QAction* const maximizeAction = toolBar->addAction("maximize"); 
	root->SetActionIcon ("maximize", maximizeAction);
	connect (maximizeAction, SIGNAL (triggered(bool)), this, SLOT (OnMaximizePane()));


//	toolBar->addAction("second");
	
	//QWidget* view = new QGLWidget(this);
	QWidget* view = new QTextEdit(this);

	QVBoxLayout* vbox = new QVBoxLayout(this);
	vbox->setContentsMargins(0,0,0,0);
	vbox->setSpacing(0);

	vbox->addWidget(toolBar);     
	vbox->addWidget(view);     

	setLayout(vbox); 
}

alchemediaViewport::~alchemediaViewport(void)
{
}


void alchemediaViewport::OnMaximizePane()
{
	alchemediaVerticalSpliter* const myParent = (alchemediaVerticalSpliter*) parent();
	alchemediaCanvas* const canvas = (alchemediaCanvas*) myParent->parent();
	if (m_maximized) {
		m_maximized = false;
		canvas->RestoreState();

	} else {
		m_maximized = true;
		canvas->SaveState();

		QString name (objectName());
		if (name == QString::fromUtf8("topViewport")) {
			myParent->MoveSpliter(myParent->height());
		} else {
			myParent->MoveSpliter(0);
		}
		
		name = myParent->objectName();
		if (name == QString::fromUtf8("leftSpliter")) {
			canvas->MoveSpliter(canvas->width());
		} else {
			canvas->MoveSpliter(0);
		}
	}
}
