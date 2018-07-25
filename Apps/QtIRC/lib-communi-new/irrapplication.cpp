#include "irrapplication.h"

IrrApplication::IrrApplication(int argc, char** argv)
    : QApplication(argc, argv){
    connect(this, SIGNAL(aboutToQuit()), this, SLOT(closeWindows()));
}
