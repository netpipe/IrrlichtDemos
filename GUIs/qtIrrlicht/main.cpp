//http://irrlicht.sourceforge.net/forum/viewtopic.php?f=5&t=50533
//main.cpp


#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
