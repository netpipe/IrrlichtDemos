#include "irrwidget.h"

#include <QResizeEvent>
#include <QTimer>
#include <QApplication>
#include <QKeyEvent>
#include "irreventreciever.h"

IrrWidget::IrrWidget(bool frameless)
    : m_pDevice(NULL)
    , verboseOutput(false)
    , autoRefresh(false)
    , time(0)
    , lastFps(0){
    if(frameless){
        setWindowFlags(Qt::FramelessWindowHint);
    }
    timer = new QTimer(this);

    this->setAttribute(Qt::WA_PaintOnScreen, true);
    this->setAttribute(Qt::WA_OpaquePaintEvent, true);

    createIrrlichtDevice();
}

IrrWidget::~IrrWidget(){
    if(m_pDevice != NULL){
        m_pDevice->drop();
    }
}
void IrrWidget::createIrrlichtDevice(){
    dimension2d<u32> windowSize(this->geometry().width(), this->geometry().height());

    SIrrlichtCreationParameters createParams;
    createParams.WindowId = (void*)this->winId();
    createParams.WindowSize = windowSize;
    createParams.DriverType = EDT_OPENGL;
    createParams.Stencilbuffer = true;
    createParams.Vsync = false;

    m_pDevice = createDeviceEx(createParams);
    if(m_pDevice == NULL){
        qDebug() << "failed to create irrlicht device";
    }
    m_pDevice->setResizable(true);
    m_pScene = m_pDevice->getSceneManager();
    m_pDriver = m_pDevice->getVideoDriver();
    m_pGuiEnv = m_pDevice->getGUIEnvironment();

    m_pBufferList = m_pGuiEnv->addListBox(core::rect<int>(5,5,200, 400), 0, true, true);
        //gui::IGUITab* optTab = tabctrl->addTab(L"Demo");
        //gui::IGUITab* aboutTab = tabctrl->addTab(L"About");

        // add list box

        //gui::IGUIListBox* box = m_pGuiEnv->addListBox(core::rect<int>(10,10,220,120), optTab, 1);
        m_pBufferList->addItem(L"OpenGL 1.5");
        m_pBufferList->addItem(L"Direct3D 8.1");
        m_pBufferList->addItem(L"Direct3D 9.0c");
        m_pBufferList->addItem(L"Burning's Video 0.47");
        m_pBufferList->addItem(L"Irrlicht Software Renderer 1.0");
        m_pBufferList->setSelected(true);

    scene::IAnimatedMesh* mesh = m_pScene->getMesh("./media/sydney.md2");
    if(mesh == NULL){
        return;
    }
    scene::IAnimatedMeshSceneNode* node = m_pScene->addAnimatedMeshSceneNode(mesh);
    if(node !=NULL){
        node->setMaterialFlag(EMF_LIGHTING, false);
        node->setMD2Animation(scene::EMAT_STAND);
        node->setMaterialTexture(0, m_pDriver->getTexture("./media/sydney.bmp"));
    }
    m_pScene->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));
    //m_pDevice->setEventReceiver(new IrrEventReciever());
}

void IrrWidget::paintEvent(QPaintEvent* event){
    if(verboseOutput){
        qDebug() << "IrrWidget::paintEvent()";
    }
    drawIrrlichtScene();
}

void IrrWidget::resizeEvent(QResizeEvent* event){
    m_pDevice->getVideoDriver()->OnResize(dimension2d<irr::u32>(event->size().width(), event->size().height()));
    if(m_pScene->getActiveCamera() != NULL){
        m_pScene->getActiveCamera()->setAspectRatio((f32)event->size().width()/(f32)event->size().height());
    }
}

QPaintEngine* IrrWidget::paintEngine() const {
    return 0;
}

void IrrWidget::drawIrrlichtScene() {
    qDebug() << "draw all";
    if(this->isActiveWindow() || !autoRefresh){
        emit IrrWidget::simulate();
        if(verboseOutput){
            qDebug() << "IrrWidget::drawIrrlichtScene();";
        }
        m_pDevice->run();
        m_pDriver->beginScene(true, true, SColor(255, 128, 128, 128));
        //m_pScene->drawAll();
       // m_pScene->getGUIEnvironment()->drawAll();
        //if(m_pDriver->getFPS() != lastFps){
          //  qDebug() << "FPS = " << m_pDriver->getFPS();
            //lastFps = m_pDriver->getFPS();
        //}
        m_pGuiEnv->drawAll();
        m_pDriver->endScene();
        emit IrrWidget::refreshed();
    }else {
        m_pDevice->sleep(time + 1, true);
    }
}

void IrrWidget::closeEvent(QCloseEvent *event)
{
  //if you click on the cross on the top of the window
  if (verboseOutput)
  qDebug() << "QIrrlichtWidget::closeEvent()" << event->isAccepted();

  this->quit();
}


void IrrWidget::autoRepaint(int delay)
{
  //set the autorefresh timer.
  connect(timer, SIGNAL(timeout()), this, SLOT(updateIrrlicht()));

  //too low delay = too high framerates, I don't want to permit it for now.
  if(delay >= 5)
     time = delay;
  else
    time = 5;
  autoRefresh = true;
  timer->start(time);
}


//GETTERS

//just returning coresponding pointers
ISceneManager * IrrWidget::getSceneManager()
{
  return m_pScene;
}

IVideoDriver * IrrWidget::getVideoDriver()
{
  return m_pDriver;
}

IrrlichtDevice * IrrWidget::getDevice()
{
  return m_pDevice;
}



//SLOTS

void IrrWidget::resetActiveCameraAspectRatio()
{
  if(m_pScene->getActiveCamera() == NULL)
    return;
  m_pScene->getActiveCamera()->setAspectRatio(
        (f32) this->geometry().width()/ (f32) this->geometry().height());
}

//closing the window properly
void IrrWidget::quit()
{
  if (verboseOutput)
    qDebug() << "QIrrlichtWidget::quit()";
  timer->stop();
  m_pDevice->drop(); //Avoid memory leak : drop Irrlicht Device
  this->close();
}


void IrrWidget::updateIrrlicht()
{
  //this slot is for calling this private method from the outside
  //more stuffs will be done here to improuve the rendering loop performences
  drawIrrlichtScene();
}

void IrrWidget::centerCursor()
{
   QCursor::setPos(this->mapToGlobal(QPoint(this->geometry().width()/2,this->geometry().height()/2)));
}

void IrrWidget::setCursorInvisible()
{
  QApplication::setOverrideCursor(QCursor(Qt::BlankCursor));
}

void IrrWidget::setCursorVisible()
{
  QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
}

//////////////// emit keyboard signals
void IrrWidget::keyPressEvent(QKeyEvent *event)
{
  if(!event->isAutoRepeat())
    emit keyPressed(event->key());
}

void IrrWidget::keyReleaseEvent(QKeyEvent *event)
{
  if(!event->isAutoRepeat())
    emit keyReleased(event->key());
}

///////////////// emit mouse signals
void IrrWidget::mouseMoveEvent(QMouseEvent *event)
{
  emit mouseMoved(event->pos());
}

void IrrWidget::mousePressEvent(QMouseEvent *event)
{
  if (verboseOutput)
    qDebug() << "mousePressEvent from widget";
  emit mousePressed(event->pos(),event->button());
}

void IrrWidget::mouseReleaseEvent(QMouseEvent *event)
{
  emit mouseReleased(event->pos(),event->button());
}
