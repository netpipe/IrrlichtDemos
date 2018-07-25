#include "mainwindow.h"
#include "ircclient.h"
#include "irrlichtwidget.h"
#include "irrwidget.h"
#include <QDialog>
#include <QWindow>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    IrrlichtWidget* widget = new IrrlichtWidget();
    widget->makeCurrent();
    setCentralWidget(widget);

    //showMaximized();
    //widget->autoRepaint();

}

MainWindow::~MainWindow()
{

}
