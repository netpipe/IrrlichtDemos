#include "mainwindow.h"
#include "ircclient.h"
#include "irrwidget.h"
#include "irrlichtwidget.h"
#include <QDialog>
#include <QWindow>
#include <QGLWidget>

#include <QCoreApplication>

//#include "ui_dialog.h"

class Dialog1 : public QDialog
{
public:
    Dialog()
    {
        QDialog *subDialog = new QDialog;
        subDialog->setWindowTitle("Sub Dialog");
        QPushButton *button = new QPushButton("Push to open new dialog", this);
        connect(button, SIGNAL(clicked()), subDialog, SLOT(show()));
    }
};



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

  //  MainWindow w;
  //  this->setWindowTitle("Main Window");

//    Dialog1 *newDlg = new Dialog1();
//    this->hide();
//    int result = newDlg->exec();
//    this->show();
//    delete newDlg;

  //  IrrWidget* widget = new IrrWidget();

    setCentralWidget(new IrcClient()); //widget

//    setCentralWidget(widget); //widget
//    showMaximized();
//    widget->autoRepaint();

   // m_button = new QPushButton("My Button", this);
    // set size and location of the button
   // m_button->setGeometry(QRect(QPoint(100, 100),
   // QSize(200, 50)));
   // m_button->setAttribute(Qt::WA_TranslucentBackground);

    // Connect button signal to appropriate slot
  //  connect(m_button, SIGNAL (released()), this, SLOT (handleButton()));


   // QWindow* dlg = new QWindow();
   // dlg->setModal(false);
   // dlg->show();
}

MainWindow::~MainWindow()
{

}

 void MainWindow::handleButton()
 {
    // change the text
    m_button->setText("Example");
    // resize button
    m_button->resize(100,100);
 }
