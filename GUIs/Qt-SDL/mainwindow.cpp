#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ptr_gfx = ui->gfx;
    ui->gfx->setUpdatesEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}
