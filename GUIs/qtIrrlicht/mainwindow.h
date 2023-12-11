
//mainwindow.h

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "irrcorerenderer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
bool eventFilter( QObject *o, QEvent *e );
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    IrrCoreRenderer* irr0;
    IrrCoreRenderer* irr1;

private:
    Ui::MainWindow *ui;
    void initIrrInstance0(QWidget *irrRenderTarget);
    void initIrrInstance1(QWidget *irrRenderTarget);

protected:
    virtual void resizeEvent(QResizeEvent *event);
};

#endif // MAINWINDOW_H
