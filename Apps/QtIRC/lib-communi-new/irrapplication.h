#ifndef IRRAPPLICATION_H
#define IRRAPPLICATION_H

#include <QApplication>
#include "irrlichtwidget.h"
#include <QDebug>

class IrrApplication : public QApplication
{
    Q_OBJECT
public:
    IrrApplication(int argc, char** argv);

    void setIrrWidget(IrrlichtWidget* widget){
        m_pWidget = widget;
    }

public slots:
    void closeWindows(){
        qDebug() << "Closing application";
        m_pWidget->closeIrrlicht();
    }

private:
    IrrlichtWidget* m_pWidget;
};

#endif // IRRAPPLICATION_H
