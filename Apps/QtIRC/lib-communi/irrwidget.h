#ifndef IRRWIDGET_H
#define IRRWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QObject>


#include "irrlicht.h"

using namespace irr;
using namespace irr::video;
using namespace irr::core;
using namespace irr::scene;

class IrrWidget : public QWidget {
    Q_OBJECT
public:
    IrrWidget(bool frameless = false);

    virtual void paintEvent(QPaintEvent* event);
    virtual void closeEvent(QCloseEvent* event);
    virtual void resizeEvent(QResizeEvent* event);

    virtual QPaintEngine* paintEngine() const;

    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);

    void autoRepaint(int delay = 15);

    bool isVerbose(){
        return verboseOutput;
    }

    ISceneManager* getSceneManager();
    IVideoDriver* getVideoDriver();
    IrrlichtDevice* getDevice();

protected:
    IrrlichtDevice* m_pDevice;
    ISceneManager* m_pScene;
    IVideoDriver* m_pDriver;

private:
    void drawIrrlichtScene();
    void createIrrlichtDevice();

    bool verboseOutput;
    bool autoRefresh;
    QTimer* timer;
    int time;
    int lastFps;

public slots:
    void quit();
    void updateIrrlicht();

    void centerCursor();
    void setCursorInvisible();
    void setCursorVisible();
    void resetActiveCameraAspectRatio();

signals:
    void refreshed();
    void simulate();
    void keyPressed(int);
    void keyReleased(int);
    void mousePressed(QPoint, Qt::MouseButton);
    void mouseReleased(QPoint, Qt::MouseButton);
    void mouseMoved(QPoint);
};



#endif // IRRWIDGET_H
