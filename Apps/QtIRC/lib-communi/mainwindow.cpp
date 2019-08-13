#include "mainwindow.h"
#include "ircclient.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setCentralWidget(new IrcClient());
}

MainWindow::~MainWindow()
{

}
