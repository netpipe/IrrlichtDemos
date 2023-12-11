
//mainwindow.cpp

#include "mainwindow.h"
#include "ui_mainwindow.h"

#ifdef PYTHON
#include <qtpython.h>
#endif

//#define irr2

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Init an Irrlicht instance and give it a target widget into where Irrlicht should show its output - here I named it "irrRenderWidget0" and "irrRenderWidget1".
    this->initIrrInstance0(ui->centralWidget->findChild<QWidget *>("irrRenderWidget0"));
#ifdef irr2
    this->initIrrInstance1(ui->centralWidget->findChild<QWidget *>("irrRenderWidget1"));
#endif
  this->installEventFilter(this);
    this->irr0->grabKeyboard();
    //this->irr0->grabMouse();
}


bool MainWindow::eventFilter( QObject *o, QEvent *e )
{
    if ( e->type() == QEvent::KeyPress ) {
        // special processing for key press
        QKeyEvent *k = (QKeyEvent *)e;
         this->irr0->keyPressEvent(k);


      //  qDebug( "Ate key press %d", k->key() );
        return 1; // eat event
    }else if ( e->type() == QEvent::KeyRelease) {
         QKeyEvent *k = (QKeyEvent *)e;
        this->irr0->keyReleaseEvent(k);

      return 1;
    } else if ( e->type() == QEvent::MouseButtonPress) {
        QMouseEvent* ee =(QMouseEvent*)e;
        this->irr0->mousePressEvent(ee);
            return 1;
     } else if ( e->type() == QEvent::MouseButtonRelease) {
           QMouseEvent* ee =(QMouseEvent*)e;
           this->irr0->mouseReleaseEvent(ee);
               return 1;
        }  else if ( e->type() == QEvent::MouseMove) {
               QMouseEvent* ee =(QMouseEvent*)e;
               this->irr0->mouseMoveEvent(ee);
                   return 1;
            }
    else {
        // standard event processing
        return 0;
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initIrrInstance0(QWidget *irrRenderTarget)
{
    this->irr0 = new IrrCoreRenderer(irrRenderTarget, true);

    this->irr0->init();
}

void MainWindow::initIrrInstance1(QWidget *irrRenderTarget)
{
    #ifdef irr2
    this->irr1 = new IrrCoreRenderer(irrRenderTarget, false);

    this->irr1->init();
#endif
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
   this->irr0->resizeIrrWidget(0, 0, this->size().width()/2, this->size().height());
    #ifdef irr2
   this->irr1->resizeIrrWidget(this->size().width()/2, 0, this->size().width()/2, this->size().height());
#endif
}

void MainWindow::on_pyrun_clicked()
{
    qtPython();
    settext();

}
