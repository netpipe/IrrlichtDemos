#ifndef IRRLICHTWIDGET_H
#define IRRLICHTWIDGET_H

#include <QGLWidget>
#include <QVector3D>
#include <QTimer>
#include <vector3d.h>
#include <SKeyMap.h>

//#include <QOpenGLWidget>

#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QObject>
#include <QtOpenGL>

namespace irr {
    class IrrlichtDevice;
    namespace scene {
        class ISceneNode;
        class ISceneManager;
        class IAnimatedMeshSceneNode;
        class ISceneNodeAnimator;
        class IBillboardSceneNode;
    }

    namespace video {
        class IVideoDriver;
    }
}

class IrrlichtWidget : public QGLWidget {
    Q_OBJECT
public:
    explicit IrrlichtWidget(QWidget* parent = 0);
    ~IrrlichtWidget();

    void init();

    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);

signals:
    void keyPressed(QKeyEvent* event);

private slots:
    void onCollisionDetected();

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

    virtual void createIrrlichtDevice();
    virtual void buildIrrlichtScene();
    virtual void drawIrrlichtScene();

private:
    void irrlichtMouseEvent(QMouseEvent* event, bool keyPressed = true);
    void irrlichtKeyEvent(QKeyEvent* event, bool pressed);
    void animatedMoveModelToPosition(irr::core::vector3df transition, irr::scene::ISceneNode* modelNode = 0);
    irr::core::vector3df getCursoreIntersation();
    void checkMoveAnimation();
    void stopMoveAnimation();
    irr::SKeyMap* getCameraKeyMap();

private:
    irr::core::vector3df mMoveToVector;
    irr::IrrlichtDevice* mDevice;
    irr::scene::ISceneManager* mScene;
    irr::video::IVideoDriver* mDriver;
    irr::scene::IAnimatedMeshSceneNode* mModelNode;
    irr::scene::ISceneNodeAnimator* mMoveModelAnimator;
    irr::scene::IBillboardSceneNode* mCursor;
};

#endif // IRRLICHTWIDGET_H
