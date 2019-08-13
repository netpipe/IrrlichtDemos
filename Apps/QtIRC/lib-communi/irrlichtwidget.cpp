#include "irrlichtwidget.h"
#include "irrlicht.h"
#include "irrutil.h"
#include "movemodelanimator.h"

#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>

using namespace irr;
using namespace irr::core;
using namespace irr::scene;
using namespace irr::video;

IrrlichtWidget::IrrlichtWidget(QWidget* parent)
    : QGLWidget(parent)
    , mModelNode(NULL)
    , mMoveModelAnimator(NULL)
    , mDevice(NULL)
    , mDriver(NULL) {

    setAttribute(Qt::WA_PaintOnScreen, true);
    setAttribute(Qt::WA_OpaquePaintEvent, true);
    setMouseTracking(true);
    setFocusPolicy(Qt::ClickFocus);
    setFocus(Qt::OtherFocusReason);
}

IrrlichtWidget::~IrrlichtWidget(){
    if(mMoveModelAnimator){
        mMoveModelAnimator->drop();
    }
    if(mDevice){
        mDevice->drop();
        mDevice = 0;
    }
}

void IrrlichtWidget::init(){
    createIrrlichtDevice();
}

void IrrlichtWidget::initializeGL()
{
}

void IrrlichtWidget::resizeGL( int w, int h )
{
    // Set the viewport to window dimensions
    glViewport( 0, 0, w, qMax( h, 1 ) );
}

void IrrlichtWidget::paintGL()
{
    // Clear the buffer with the current clearing color
   // glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Draw stuff
   // glDrawArrays( GL_TRIANGLES, 0, 3 );
}

void IrrlichtWidget::createIrrlichtDevice(){
    if(mDevice){
        return;
    }
    SIrrlichtCreationParameters params;
    params.DriverType = EDT_OPENGL;
    params.WindowId = (void*)winId();
    params.WindowSize =irr::core::dimension2d<irr::u32>(size().width()-100, size().height()-100);
    params.Stencilbuffer = true;

    mDevice = createDeviceEx(params);
    if(mDevice == 0){
        qDebug() << "failed to create irrlicht device";
    }
    mDriver = mDevice->getVideoDriver();
    mScene = mDevice->getSceneManager();

    buildIrrlichtScene();
}

void IrrlichtWidget::buildIrrlichtScene(){
    scene::ISceneManager* manager = mDevice->getSceneManager();
    mDevice->setWindowCaption(L"Hello World!");
    scene::ISceneNode* n = manager->addAnimatedMeshSceneNode(manager->getMesh("./media/sydney.md2"));
    if(n == nullptr){
        qDebug() << "Unable to load requested mesh\n";
        return;
    }
    n->setMaterialTexture(0, mDriver->getTexture("./media/sydney.bmp"));
    n->setMaterialFlag( video::EMF_LIGHTING, false );
    manager->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));

    mDevice->getFileSystem()->addFileArchive("./media/map-20kdm2.pk3");
    manager->addOctreeSceneNode(manager->getMesh("maps/20kdm2.bsp"));
}

void IrrlichtWidget::drawIrrlichtScene(){
    mDriver->beginScene(true, true, video::SColor(255, 100, 0, 255));
    mDevice->getSceneManager()->drawAll();
    mDriver->endScene();
}

void IrrlichtWidget::keyPressEvent(QKeyEvent* event){
    irrlichtKeyEvent(event, true);
    emit keyPressed(event);
}

void IrrlichtWidget::keyReleaseEvent(QKeyEvent *event){
    irrlichtKeyEvent(event, false);
}

void IrrlichtWidget::mouseMoveEvent(QMouseEvent* event){
    irrlichtMouseEvent(event, event->button() != Qt::NoButton);
}

void IrrlichtWidget::mousePressEvent(QMouseEvent *event){
    irrlichtMouseEvent(event, true);
}

void IrrlichtWidget::mouseReleaseEvent(QMouseEvent *event){
    irrlichtMouseEvent(event, false);
}

void IrrlichtWidget::irrlichtKeyEvent(QKeyEvent* event, bool pressed){
    irr::SEvent irrEvent;
    irrEvent.EventType = irr::EET_KEY_INPUT_EVENT;

    SIrrlichtKey irrKey = convertToIrrlichtKey(event->key());
    if(irrKey.code == 0){
        return;
    }
    irrEvent.KeyInput.Key = irrKey.code;
    irrEvent.KeyInput.Key = irrKey.code;
    irrEvent.KeyInput.Control = ((event->modifiers() & Qt::ControlModifier) != 0);
    irrEvent.KeyInput.Shift = ((event->modifiers() & Qt::ShiftModifier) != 0);
    irrEvent.KeyInput.Char = irrKey.ch;
    irrEvent.KeyInput.PressedDown = pressed;

    mDevice->postEventFromUser(irrEvent);

    repaint();
}

void IrrlichtWidget::irrlichtMouseEvent(QMouseEvent* event, bool keyPressed) {
    irr::SEvent irrEvent;

    irrEvent.EventType = irr::EET_MOUSE_INPUT_EVENT;

    switch ( event->button() ){
    case Qt::LeftButton:
        irrEvent.MouseInput.Event = keyPressed ? irr::EMIE_LMOUSE_PRESSED_DOWN:irr::EMIE_LMOUSE_LEFT_UP;
        break;

    case Qt::MidButton:
        irrEvent.MouseInput.Event = keyPressed ? irr::EMIE_MMOUSE_PRESSED_DOWN:irr::EMIE_MMOUSE_LEFT_UP;
        break;

    case Qt::RightButton:
        irrEvent.MouseInput.Event = keyPressed ? irr::EMIE_RMOUSE_PRESSED_DOWN:irr::EMIE_RMOUSE_LEFT_UP;
        break;

    default:
        irrEvent.MouseInput.Event = EMIE_MOUSE_MOVED;
        break;
    }

    irrEvent.MouseInput.X = event->x();
    irrEvent.MouseInput.Y = event->y();
    irrEvent.MouseInput.Wheel = 0.0f; // Zero is better than undefined

    mDevice->postEventFromUser( irrEvent );

    repaint();
}

void IrrlichtWidget::animatedMoveModelToPosition(irr::core::vector3df transition, irr::scene::ISceneNode* modelNode){
    if(modelNode != 0){
        return;
    }
    f32 distanceToNewPoint = transition.getDistanceFrom(mModelNode->getPosition());
    u32 timeForAnimation = u32(distanceToNewPoint);
    stopMoveAnimation();
    mMoveModelAnimator = new MoveModelAnimator(mModelNode->getPosition(), transition, timeForAnimation, mModelNode->getRotation());
    mModelNode->addAnimator(mMoveModelAnimator);
    mModelNode->setMD2Animation(EMAT_RUN);
}

void IrrlichtWidget::onCollisionDetected(){
    stopMoveAnimation();
}

void IrrlichtWidget::checkMoveAnimation(){
    if(mMoveModelAnimator && mMoveModelAnimator->hasFinished()){
        stopMoveAnimation();
    }
}

void IrrlichtWidget::stopMoveAnimation(){
    if(mMoveModelAnimator && mModelNode){
        mModelNode->removeAnimator(mMoveModelAnimator);
        mMoveModelAnimator->drop();
        mMoveModelAnimator = 0;
        mModelNode->setMD2Animation(EMAT_STAND);
    }
}
