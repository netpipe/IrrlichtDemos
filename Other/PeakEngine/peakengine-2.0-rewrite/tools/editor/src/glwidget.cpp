#include "../include/glwidget.h"
#include <iostream>

NodeHandle model = 0, cam = 0;

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent)
{
  resize(sizeHint());
}

GLWidget::~GLWidget()
{
  Horde3D::release();
}


 QSize GLWidget::minimumSizeHint() const
{
  return QSize(50,50);
}

QSize GLWidget::sizeHint() const{
  
  return QSize(800,600);
  
}

void GLWidget::initializeGL(){
  
  if(!Horde3D::init()){
	
	Horde3DUtils::dumpMessages();
	
	return;
	
  }
  
  
  
  Horde3DUtils::setResourcePath( ResourceTypes::SceneGraph, "models" );
  
  Horde3DUtils::setResourcePath( ResourceTypes::Geometry, "models" );
  
  Horde3DUtils::setResourcePath( ResourceTypes::Animation, "models" );
  
  Horde3DUtils::setResourcePath( ResourceTypes::Material, "materials" );
  
  Horde3DUtils::setResourcePath( ResourceTypes::Code, "shaders" );
  
  Horde3DUtils::setResourcePath( ResourceTypes::Shader, "shaders" );
  
  Horde3DUtils::setResourcePath( ResourceTypes::Texture2D, "textures" );
  
  Horde3DUtils::setResourcePath( ResourceTypes::TextureCube, "textures" );
  
  Horde3DUtils::setResourcePath( ResourceTypes::Effect, "effects" );
  
  Horde3DUtils::setResourcePath( ResourceTypes::Pipeline, "pipelines" );
  
  ResHandle pipeRes = Horde3D::addResource( ResourceTypes::Pipeline, "deferred.pipeline.xml",0);
  
  ResHandle modelRes = Horde3D::addResource(ResourceTypes::SceneGraph,"charecter.scene.xml",0);
  
  ResHandle animRes = Horde3D::addResource(ResourceTypes::Animation,"walk.anim.xml",0);
  
  ResHandle fontRes = Horde3D::addResource(ResourceTypes::Material,"font.material.xml",0);
  
  logoRes = Horde3D::addResource(ResourceTypes::Material,"logo.material.xml",0);
  
  Horde3DUtils::loadResourcesFromDisk("media");
  
  Horde3D::setOption( EngineOptions::LoadTextures, 1 );
  
  Horde3D::setOption( EngineOptions::TexCompression, 0 );
  
  Horde3D::setOption( EngineOptions::FastAnimation, 0 );
  
  Horde3D::setOption( EngineOptions::AnisotropyFactor, 8 );
  
  Horde3D::setOption( EngineOptions::ShadowMapSize, 2048 );
  
  Horde3D::setOption( EngineOptions::DebugViewMode,0.0f);
  
  model = Horde3D::addNodes(RootNode,modelRes);
  
  //Horde3D::setupModelAnimStage(model,0,animRes,"",true);
  
  NodeHandle light = Horde3D::addLightNode(RootNode,"Light1", 0,"LIGHTING","SHADOWMAP");
  
  Horde3D::setNodeTransform(light,0,2,0,0,00,0,1,1,1);
  
  Horde3D::setNodeParamf( light, LightNodeParams::Radius, 50 );
  
  Horde3D::setNodeParamf( light, LightNodeParams::FOV, 90 );
  
  Horde3D::setNodeParami( light, LightNodeParams::ShadowMapCount, 3 );
  
  Horde3D::setNodeParamf( light, LightNodeParams::ShadowSplitLambda, 0.9f );
  
  Horde3D::setNodeParamf( light, LightNodeParams::ShadowMapBias, 0.001f );
  
  Horde3D::setNodeParamf( light, LightNodeParams::Col_R, 0.9f );
  Horde3D::setNodeParamf( light, LightNodeParams::Col_G, 0.7f );
  Horde3D::setNodeParamf( light, LightNodeParams::Col_B, 0.75f );
  
  cam = Horde3D::addCameraNode(RootNode,"Camera",pipeRes);
  
}

void GLWidget::paintGL(){
  
  float curFps = 60;
  
  curFps = curFps + 10.0f * ( 1/curFps);
  
  Horde3D::setNodeTransform(model,curFps*10,0,0,0,0,0,1,1,1);
  
  Horde3D::showOverlay(0.75f,0,0,0,1,0,1,0,1,0.2f,1,1,0.75f,0.2f,0,1,7,logoRes);
  
  Horde3D::render(cam);
  
  Horde3D::clearOverlays();
  
  Horde3DUtils::dumpMessages();
  
}

void GLWidget::resizeGL(int width,int height){
  
  Horde3D::resize(0,0,800,600);
  
  Horde3D::setupCameraView(cam,45.0f,(float)width /height,0.1f,1000.0f);
  
}

void GLWidget::mousePressEvent(QMouseEvent *event){
  
  if (event->buttons() & Qt::LeftButton)
	
  {
	
	std::cout<<"left mouse button pressed";
	
  }
  
}

void GLWidget::mouseMoveEvent(QMouseEvent *event){
  
  QPoint pnt = event->pos();
  
  
  
  if ( pnt.x() == 0 )
	
	std::cout<<"pointers xvalue equals zero";
  
  
  if ( pnt.y() == 0 )
	
	std::cout<<"pointers yvalue equals zero";
  
}

void GLWidget::keyPressEvent(QKeyEvent *event){
  
  switch( event->key() )
	
  {
	
	case Qt::Key_Up:
	  
	  std::cout<<"up pressed";
	  
	  
	  break;
	  
	case Qt::Key_Down:
	  
	  std::cout<<"down pressed";
	  
	  break;
	default:
	  
	  GLWidget::keyPressEvent( event );
	  
  }
  
}


