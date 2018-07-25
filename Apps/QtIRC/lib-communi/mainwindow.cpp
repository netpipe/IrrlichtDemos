#include "mainwindow.h"
#include "ircclient.h"
#include "irrwidget.h"
#include "irrlichtwidget.h"
#include <QDialog>
#include <QWindow>
#include <QGLWidget>

#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    IrrWidget* widget = new IrrWidget();
    setCentralWidget(new IrcClient()); //widget
  //  setCentralWidget(widget); //widget
    showMaximized();
    widget->autoRepaint();

    m_button = new QPushButton("My Button", this);
    // set size and location of the button
    m_button->setGeometry(QRect(QPoint(100, 100),
    QSize(200, 50)));

    // Connect button signal to appropriate slot
    connect(m_button, SIGNAL (released()), this, SLOT (handleButton()));


    QWindow* dlg = new QWindow();
    //dlg->setModal(false);
    dlg->show();
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
