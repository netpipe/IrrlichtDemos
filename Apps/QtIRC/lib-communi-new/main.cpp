#include "mainwindow.h"
#include <QApplication>
#include "irrlichtwidget.h"
#include "irrapplication.h"

int main(int argc, char *argv[])
{
    IrrApplication a(argc, argv);
    IrrlichtWidget* widget = new IrrlichtWidget();
    a.setIrrWidget(widget);
    widget->setVisible(true);

    return a.exec();
}
