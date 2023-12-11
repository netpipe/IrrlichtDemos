#include "irrlichtwidget.h"
#include "irrlicht.h"
#include "irrutil.h"
#include "movemodelanimator.h"
#include "ircclient.h"

#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QTimer>
using namespace irr;
using namespace irr::core;
using namespace irr::scene;
using namespace irr::video;

using namespace io;
using namespace gui;

using namespace gui;

IrrlichtWidget::IrrlichtWidget(QWidget* parent)
    : QGLWidget(parent)
    , mModelNode(NULL)
    , mDevice(NULL)
    , mDriver(NULL) {

    setAttribute(Qt::WA_PaintOnScreen, true);
    setAttribute(Qt::WA_OpaquePaintEvent, true);
    setMouseTracking(true);
    setFocusPolicy(Qt::ClickFocus);
    setFocus(Qt::OtherFocusReason);
    IrcClient* client = new IrcClient();
    //setMinimumSize(640, 480);
}

IrrlichtWidget::~IrrlichtWidget(){
    qDebug() << "Called destructor";

    if(mDevice){
        mDevice->drop();
        mDevice = 0;
    }
}

void IrrlichtWidget::initializeGL(){
    init();
}

void IrrlichtWidget::closeIrrlicht(){
    mDevice->drop();
}

void IrrlichtWidget::resizeGL(int width, int height){
    if(mDevice != 0){
        dimension2d<uint> size;
        size.Width = width;
        size.Height = height;

        mDriver->OnResize(size);
    }
}

void IrrlichtWidget::paintGL(){
    this->makeCurrent();
    if(mDevice == 0)
        return;
    drawIrrlichtScene();
}

void IrrlichtWidget::init(){
    createIrrlichtDevice();
}

void IrrlichtWidget::createIrrlichtDevice(){
    if(mDevice){
        return;
    }
    SIrrlichtCreationParameters params;
    params.DriverType = EDT_OPENGL;
    params.WindowId = (void*)winId();
    params.WindowSize =irr::core::dimension2d<irr::u32>(640, 480);
    params.Stencilbuffer = false;

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

    scene::IAnimatedMesh* mesh = manager->getMesh("./media/sydney.md2");
    if(mesh == NULL){
        return;
    }
    scene::IAnimatedMeshSceneNode* node = manager->addAnimatedMeshSceneNode(mesh);
    if(node !=NULL){
        node->setMaterialFlag(EMF_LIGHTING, false);
        node->setMD2Animation(scene::EMAT_STAND);
        node->setMaterialTexture(0, mDriver->getTexture("./media/sydney.bmp"));
    }
    manager->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));

    IGUIEnvironment* guienv = mDevice->getGUIEnvironment();
    gui::IGUIListBox* box = guienv->addListBox(core::rect<int>(10,10,220,120));
    box->addItem(L"OpenGL 1.5");
    box->addItem(L"Direct3D 8.1");
    box->addItem(L"Direct3D 9.0c");
    box->addItem(L"Burning's Video 0.47");
    box->addItem(L"Irrlicht Software Renderer 1.0");
    box->setSelected(true);


    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(drawIrrlichtScene()));
    m_pTimer->start(5);

}

void IrrlichtWidget::drawIrrlichtScene(){
    mDriver->beginScene(true, true, video::SColor(255, 100, 101, 140));
    mDevice->run();
    mDevice->getSceneManager()->drawAll();
    mDevice->getGUIEnvironment()->drawAll();
    mDriver->endScene();
}

void IrrlichtWidget::keyPressEvent(QKeyEvent* event){
    irrlichtKeyEvent(event, true);
    //emit keyPressed(event);
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
}
