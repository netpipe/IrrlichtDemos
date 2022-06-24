//============================================================================//
//
// date: xx.xx.xxxx
//============================================================================//
#include <irrlicht.h>
#include <iostream>
#pragma comment(lib,"Irrlicht.lib")

#include "M8M/CM8MMeshLoader.h"
#include "MDL7/CMDL7MeshLoader.h"
#include "MDL3/CMDL3MeshLoader.h"
#include "ASCII/CASCIIMeshConverter.h"
//============================================================================//
#define M8MEXP_VERSION "0.0.4"
#define __DATE__ "22.12.2008"
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
//==================================funcs=====================================//
typedef core::vector3d<f32> v3f;typedef core::vector2d<f32> v2f;
typedef struct rands{private:s32 seedx;public:
    virtual s32 rand(){const s32 m=2147483381,a=40692,q=m/a,r=m%a;if(seedx<5483381){
      seedx=5483381;}seedx=(a*(seedx%q)-r*(seedx/q));seedx+=(seedx<0)*m;
     return seedx;}};rands randsx;
s32 random(f32 n){s32 v0=randsx.rand()%(!n?1:abs((s32)n+1));return v0;}
f32 max(f32 v1,f32 v2){v1=v1>=v2?v1:v2;return v1;}
f32 min(f32 v1,f32 v2){v1=v1<=v2?v1:v2;return v1;}
f32 vec_dis(v3f v1,v3f v2){v1=v3f(v1.X-v2.X,v1.Y-v2.Y,v1.Z-v2.Z);
    f32 v0=sqrt((f64)(v1.X*v1.X+v1.Y*v1.Y+v1.Z*v1.Z));return v0;}
void vec_rot(v3f &v1,v3f v2){const f64 a=3.14159265359f/180.0f;f32 cs=0,sn=0;
    v2.Y*=a;cs=cos(v2.Y);sn=sin(v2.Y);v1=v3f(v1.X*cs-v1.Z*sn,v1.Y,v1.X*sn+v1.Z*cs);
    v2.X*=a;cs=cos(v2.X);sn=sin(v2.X);v1=v3f(v1.X*cs-v1.Y*sn,v1.X*sn+v1.Y*cs,v1.Z);
    v2.Z*=a;cs=cos(v2.Z);sn=sin(v2.Z);v1=v3f(v1.X,v1.Y*cs-v1.Z*sn,v1.Y*sn+v1.Z*cs);}
void vec_ang(f32 &ag,v3f v2){const f64 a=180/3.14159265359f;
    if(!v2.Z){ag=v2.X<0?180:0;ag-=180;return;}if(!v2.X){ag=v2.Z<0?-90:90;return;}
    f64 t=sqrt(f64(v2.X*v2.X+v2.Z*v2.Z));t=v2.Z/(t+!t);ag=atan(sqrt(1-t*t)/(t+!t))*a;
    if(v2.X>0&&v2.Z>0){ag=s32(ag+90+(ag+90<0?-1:1)*0.5f);return;}
    if(v2.X>0&&v2.Z<0){ag=s32(ag-90+(ag-90<0?-1:1)*0.5f);return;}
    if(v2.X<0&&v2.Z<0){ag=s32(-ag-90+(-ag-90<0?-1:1)*0.5f);return;}
    if(v2.X<0&&v2.Z>0){ag=s32(-ag+90+(-ag+90<0?-1:1)*0.5f);return;}
    ag=s32(ag-180+(ag-180<0?-1:1)*0.5f);}
struct DLinex{private:
    struct CDLine:public ISceneNode{public:
        CDLine(ISceneNode*parent,ISceneManager*smgr,v3f sx,v3f ex,SColor cx,
         SMaterial material=SMaterial(),s32 id=-1):ISceneNode(parent,smgr,id),start(sx),
         end(ex),color(cx){material.Wireframe=false;material.Lighting=false;
          material.Thickness=1;material.FogEnable=false;box=aabbox3df(1,1,1,-1,-1,-1);
          AutomaticCullingState=EAC_OFF;}~CDLine(){}
        virtual void OnRegisterSceneNode(){
         if(IsVisible){SceneManager->registerNodeForRendering(this);}
         ISceneNode::OnRegisterSceneNode();}
        virtual void render(){
         IVideoDriver*driver=SceneManager->getVideoDriver();
         driver->setMaterial(material);
         driver->setTransform(ETS_WORLD,AbsoluteTransformation);
         driver->draw3DLine(start,end,color);this->remove();}
        virtual const aabbox3d<f32>&getBoundingBox()const{return box;}
      private:
        aabbox3df box;SMaterial material;v3f start;v3f end;SColor color;};public:
    void Draw(ISceneManager*smgr,v3f s,v3f e,SColor c){
        CDLine*DLine0=new CDLine(smgr->getRootSceneNode(),smgr,s,e,c);
        DLine0->drop();DLine0=0;}
};DLinex*DLine;
//=================================einput=====================================//
u8 eKEY_CODE;bool eKEY_BKSP,eKEY_TAB,eKEY_ENTER,eKEY_PAUSE,eKEY_SCROLL,eKEY_ESC,eKEY_HOME,
eKEY_END,eKEY_UP,eKEY_DOWN,eKEY_LEFT,eKEY_RIGHT,eKEY_PGUP,eKEY_PGDN,eKEY_INS,eKEY_DEL,
eKEY_SHIFT,eKEY_CTRL,eKEY_ALT,eKEY_CAPS,eKEY_PLUS,eKEY_MINUS,eKEY_COMMA,eKEY_PERIOD,
eKEY_SPACE,eKEY_F1,eKEY_F2,eKEY_F3,eKEY_F4,eKEY_F5,eKEY_F6,eKEY_F7,eKEY_F8,eKEY_F9,eKEY_F10,
eKEY_F11,eKEY_F12,eKEY_1,eKEY_2,eKEY_3,eKEY_4,eKEY_5,eKEY_6,eKEY_7,eKEY_8,eKEY_9,eKEY_0,
eKEY_A,eKEY_B,eKEY_C,eKEY_D,eKEY_E,eKEY_F,eKEY_G,eKEY_H,eKEY_I,eKEY_J,eKEY_K,eKEY_L,eKEY_M,
eKEY_N,eKEY_O,eKEY_P,eKEY_Q,eKEY_R,eKEY_S,eKEY_T,eKEY_U,eKEY_V,eKEY_W,eKEY_X,eKEY_Y,eKEY_Z,
eMOUSE_RIGHT,eMOUSE_LEFT;s32 eMOUSE_X,eMOUSE_Y;bool OnEventx(const SEvent& ev);
class einput:public IEventReceiver{public:
    virtual bool OnEvent(const SEvent& ev){
       if(ev.EventType==EET_KEY_INPUT_EVENT){
        s32 kk=ev.KeyInput.Key;s32 kd=ev.KeyInput.PressedDown;eKEY_CODE=(u8)kk;
        eKEY_BKSP=(!eKEY_BKSP?kk==0x08:1)-(kk==0x08&&!kd);
        eKEY_TAB=(!eKEY_TAB?kk==0x09:1)-(kk==0x09&&!kd);
        eKEY_ENTER=(!eKEY_ENTER?kk==0x0D:1)-(kk==0x0D&&!kd);
        eKEY_PAUSE=(!eKEY_PAUSE?kk==0x13:1)-(kk==0x13&&!kd);
        eKEY_SCROLL=(!eKEY_SCROLL?kk==0x91:1)-(kk==0x91&&!kd);
        eKEY_ESC=(!eKEY_ESC?kk==0x1B:1)-(kk==0x1B&&!kd);
        eKEY_HOME=(!eKEY_HOME?kk==0x24:1)-(kk==0x24&&!kd);
        eKEY_END=(!eKEY_END?kk==0x23:1)-(kk==0x23&&!kd);
        eKEY_UP=(!eKEY_UP?kk==0x26:1)-(kk==0x26&&!kd);
        eKEY_DOWN=(!eKEY_DOWN?kk==0x28:1)-(kk==0x28&&!kd);
        eKEY_LEFT=(!eKEY_LEFT?kk==0x25:1)-(kk==0x25&&!kd);
        eKEY_RIGHT=(!eKEY_RIGHT?kk==0x27:1)-(kk==0x27&&!kd);
        eKEY_PGUP=(!eKEY_PGUP?kk==0x21:1)-(kk==0x21&&!kd);
        eKEY_PGDN=(!eKEY_PGDN?kk==0x22:1)-(kk==0x22&&!kd);
        eKEY_INS=(!eKEY_INS?kk==0x2D:1)-(kk==0x2D&&!kd);
        eKEY_DEL=(!eKEY_DEL?kk==0x2E:1)-(kk==0x2E&&!kd);
        eKEY_SHIFT=(!eKEY_SHIFT?kk==0x10:1)-(kk==0x10&&!kd);
        eKEY_CTRL=(!eKEY_CTRL?kk==0x11:1)-(kk==0x11&&!kd);
        eKEY_ALT=(!eKEY_ALT?kk==0x12:1)-(kk==0x12&&!kd);
        eKEY_CAPS=(!eKEY_CAPS?kk==0x14:1)-(kk==0x14&&!kd);
        eKEY_PLUS=(!eKEY_PLUS?kk==0xBB:1)-(kk==0xBB&&!kd);
        eKEY_MINUS=(!eKEY_MINUS?kk==0xBD:1)-(kk==0xBD&&!kd);
        eKEY_COMMA=(!eKEY_COMMA?kk==0xBC:1)-(kk==0xBC&&!kd);
        eKEY_PERIOD=(!eKEY_PERIOD?kk==0xBE:1)-(kk==0xBE&&!kd);
        eKEY_SPACE=(!eKEY_SPACE?kk==0x20:1)-(kk==0x20&&!kd);
        eKEY_F1=(!eKEY_F1?kk==0x70:1)-(kk==0x70&&!kd);
        eKEY_F2=(!eKEY_F2?kk==0x71:1)-(kk==0x71&&!kd);
        eKEY_F3=(!eKEY_F3?kk==0x72:1)-(kk==0x72&&!kd);
        eKEY_F4=(!eKEY_F4?kk==0x73:1)-(kk==0x73&&!kd);
        eKEY_F5=(!eKEY_F5?kk==0x74:1)-(kk==0x74&&!kd);
        eKEY_F6=(!eKEY_F6?kk==0x75:1)-(kk==0x75&&!kd);
        eKEY_F7=(!eKEY_F7?kk==0x76:1)-(kk==0x76&&!kd);
        eKEY_F8=(!eKEY_F8?kk==0x77:1)-(kk==0x77&&!kd);
        eKEY_F9=(!eKEY_F9?kk==0x78:1)-(kk==0x78&&!kd);
        eKEY_F10=(!eKEY_F10?kk==0x79:1)-(kk==0x79&&!kd);
        eKEY_F11=(!eKEY_F11?kk==0x7A:1)-(kk==0x7A&&!kd);
        eKEY_F12=(!eKEY_F12?kk==0x7B:1)-(kk==0x7B&&!kd);
        eKEY_1=(!eKEY_1?kk==0x31:1)-(kk==0x31&&!kd);
        eKEY_2=(!eKEY_2?kk==0x32:1)-(kk==0x32&&!kd);
        eKEY_3=(!eKEY_3?kk==0x33:1)-(kk==0x33&&!kd);
        eKEY_4=(!eKEY_4?kk==0x34:1)-(kk==0x34&&!kd);
        eKEY_5=(!eKEY_5?kk==0x35:1)-(kk==0x35&&!kd);
        eKEY_6=(!eKEY_6?kk==0x36:1)-(kk==0x36&&!kd);
        eKEY_7=(!eKEY_7?kk==0x37:1)-(kk==0x37&&!kd);
        eKEY_8=(!eKEY_8?kk==0x38:1)-(kk==0x38&&!kd);
        eKEY_9=(!eKEY_9?kk==0x39:1)-(kk==0x39&&!kd);
        eKEY_0=(!eKEY_0?kk==0x30:1)-(kk==0x30&&!kd);
        eKEY_A=(!eKEY_A?kk==0x41:1)-(kk==0x41&&!kd);
        eKEY_B=(!eKEY_B?kk==0x42:1)-(kk==0x42&&!kd);
        eKEY_C=(!eKEY_C?kk==0x43:1)-(kk==0x43&&!kd);
        eKEY_D=(!eKEY_D?kk==0x44:1)-(kk==0x44&&!kd);
        eKEY_E=(!eKEY_E?kk==0x45:1)-(kk==0x45&&!kd);
        eKEY_F=(!eKEY_F?kk==0x46:1)-(kk==0x46&&!kd);
        eKEY_G=(!eKEY_G?kk==0x47:1)-(kk==0x47&&!kd);
        eKEY_H=(!eKEY_H?kk==0x48:1)-(kk==0x48&&!kd);
        eKEY_I=(!eKEY_I?kk==0x49:1)-(kk==0x49&&!kd);
        eKEY_J=(!eKEY_J?kk==0x4A:1)-(kk==0x4A&&!kd);
        eKEY_K=(!eKEY_K?kk==0x4B:1)-(kk==0x4B&&!kd);
        eKEY_L=(!eKEY_L?kk==0x4C:1)-(kk==0x4C&&!kd);
        eKEY_M=(!eKEY_M?kk==0x4D:1)-(kk==0x4D&&!kd);
        eKEY_N=(!eKEY_N?kk==0x4E:1)-(kk==0x4E&&!kd);
        eKEY_O=(!eKEY_O?kk==0x4F:1)-(kk==0x4F&&!kd);
        eKEY_P=(!eKEY_P?kk==0x50:1)-(kk==0x50&&!kd);
        eKEY_Q=(!eKEY_Q?kk==0x51:1)-(kk==0x51&&!kd);
        eKEY_R=(!eKEY_R?kk==0x52:1)-(kk==0x52&&!kd);
        eKEY_S=(!eKEY_S?kk==0x53:1)-(kk==0x53&&!kd);
        eKEY_T=(!eKEY_T?kk==0x54:1)-(kk==0x54&&!kd);
        eKEY_U=(!eKEY_U?kk==0x55:1)-(kk==0x55&&!kd);
        eKEY_V=(!eKEY_V?kk==0x56:1)-(kk==0x56&&!kd);
        eKEY_W=(!eKEY_W?kk==0x57:1)-(kk==0x57&&!kd);
        eKEY_X=(!eKEY_X?kk==0x58:1)-(kk==0x58&&!kd);
        eKEY_Y=(!eKEY_Y?kk==0x59:1)-(kk==0x59&&!kd);
        eKEY_Z=(!eKEY_Z?kk==0x5A:1)-(kk==0x5A&&!kd);}
      if(ev.EventType==EET_MOUSE_INPUT_EVENT){
       eMOUSE_X=ev.MouseInput.X;eMOUSE_Y=ev.MouseInput.Y;s32 kk=ev.MouseInput.Event;
       eMOUSE_LEFT=kk==EMIE_LMOUSE_PRESSED_DOWN?1:kk==EMIE_LMOUSE_LEFT_UP?0:eMOUSE_LEFT;
       eMOUSE_RIGHT=kk==EMIE_RMOUSE_PRESSED_DOWN?1:kk==EMIE_RMOUSE_LEFT_UP?0:eMOUSE_RIGHT;}
      OnEventx(ev);}};
//==================================device====================================//
einput ev;IrrlichtDevice*device=0;s32 scrnx=0,scrny=0;
//===================================type=====================================//
ICameraSceneNode*camerax=0;
/////////////edir
core::stringc exedir="";stringw ls="";
void edir(){if(device){device->getFileSystem()->changeWorkingDirectoryTo(exedir.c_str());}}
/////////////var
s32 frampos=0,fracnt=0,stex=0,gwact=0,vquit=0,sknum=0;bool alpx=0,ltex=0;
f32 fv=1.0f;aabbox3d<f32> ebox;v3f v0,v1,vx;
/////////////var
core::stringw MessageText;
core::stringw Caption;
IAnimatedMeshSceneNode*Model=0,*rvm=0;
IAnimatedMeshSceneNode*m=0;
/////////////func
bool writem(ISceneManager*smgr=0,IMesh*msh=0,s32 tp=0);
bool writexframe(ISceneManager*smgr=0,IMesh*msh=0,s32 tp=0);
video::ITexture*image2tex(video::IImage*im,video::IVideoDriver*driver,const c8*nm="x"){
    video::ITexture*tex=0;if(im>0&&driver){tex=driver->addTexture(nm,im);
     tex->grab();}return tex;}
video::IImage*tex2image(video::ITexture*tex,video::IVideoDriver*driver){
    video::IImage*im=0;if(tex>0&&driver){im=driver->createImageFromData(tex->getColorFormat(),
     tex->getSize(),tex->lock(),0,1);tex->unlock();}return im;}
/////////////grid{
class CGSceneNode:public ISceneNode{public:typedef core::vector3df v3f;
    CGSceneNode(ISceneNode*parent_rootSceneNode,ISceneManager*smgr,s32 id=-1,
     u32 spacing=16,u32 size=160,SColor gridcolor=SColor(128,110,110,110))
     :ISceneNode(parent_rootSceneNode,smgr,id),
     m_spacing(spacing),m_size(size),
     m_gridcolor(gridcolor),m_XLineColor(SColor(128,255,0,0)),
     m_ZLineColor(SColor(128,0,0,255)){
      Material.Wireframe=0;
      Material.Lighting=0;
      Material.Thickness=1;
      Material.FogEnable=0;
      m_size/=2;Box=aabbox3df(1,1,1,-1,-1,-1);
      AutomaticCullingState=EAC_OFF;}
    //! Pre-Register stuff
    virtual void OnRegisterSceneNode(){
     if(IsVisible){SceneManager->registerNodeForRendering(this);}
     ISceneNode::OnRegisterSceneNode();}
    //! Render our grid using 3D lines
    virtual void render(){
     IVideoDriver*driver=SceneManager->getVideoDriver();
     v3f leftMost=v3f(0,0,0);leftMost.X-=m_size;leftMost.Z-=m_size;
     v3f rightMost=v3f(0,0,0);rightMost.X+=m_size;rightMost.Z+=m_size;
     driver->setMaterial(Material);
     driver->setTransform(ETS_WORLD,AbsoluteTransformation);
     s32 x=0;while(x<=m_size*2){
      v3f start=leftMost;start.X+=x;
      v3f end=rightMost;end.X=start.X;
      if(start.X&&end.X){driver->draw3DLine(start,end,m_gridcolor);}
      x+=m_spacing;}
     x=0;while(x<=m_size*2){
      v3f start=leftMost;start.Z+=x;
      v3f end=rightMost;end.Z=start.Z;
      if(start.Z&&end.Z){driver->draw3DLine(start,end,m_gridcolor);}
      x+=m_spacing;}
     driver->draw3DLine(v3f((f32)m_size,0,0),v3f(-(f32)m_size,0,0),m_XLineColor);
     driver->draw3DLine(v3f(0,0,(f32)m_size),v3f(0,0,-(f32)m_size),m_ZLineColor);
    }
    virtual const aabbox3d<f32>& getBoundingBox()const{return Box;}
    virtual u32 getMaterialCount(){return 1;}
    virtual SMaterial& getMaterial(u32 i){return Material;}
    u32 GetSpacing(){return m_spacing;}
    u32 GetSize(){return m_size;}
    void SetSpacing(u32 newspacing){m_spacing=newspacing;}
    void SetSize(u32 newsize){m_size=newsize;}
    void SetGridColor(SColor newcolor){m_gridcolor=newcolor;}
    void SetAxisLineXColor(SColor XLine){m_XLineColor=XLine;}
    void SetAxisLineZColor(SColor ZLine){m_ZLineColor=ZLine;}
    void SetMaterial(SMaterial newMaterial){Material=newMaterial;}
   private:
    aabbox3df Box;
    SMaterial Material;
    u32 m_spacing;
    u32 m_size;
    SColor m_gridcolor;
    SColor m_XLineColor;
    SColor m_ZLineColor;
};CGSceneNode*grid;
void createGrid(scene::ISceneManager*smgr){
    grid=new CGSceneNode(smgr->getRootSceneNode(),smgr);
    grid->SetGridColor(SColor(0,100,100,100));
    grid->SetSize(160);grid->SetSpacing(16);
    grid->drop();
}
/////////////}grid
IBillboardSceneNode*spr=0,*sprx=0;
IGUIButton*sprb0=0,*sprb1=0,*sprb2=0,*sprb3=0,*sprb4=0,*sprb5=0,
    *sprtn=0,*sprs0=0,*sprs1=0,*sprcr=0,*sprad=0,*sprde=0;
u32 TEXBFR[4096*4096];video::ITexture*emptytex=0;
bool viewports=0;
scene::ICameraSceneNode*Camera[4]={0,0,0,0};

void setActiveCamera(scene::ICameraSceneNode*newActive){
    if(!device){return;}newActive->setInputReceiverEnabled(true);
    device->getSceneManager()->setActiveCamera(newActive);}

void showAboutText(){
    MessageText=" M8Mexp Version ";MessageText+=M8MEXP_VERSION;MessageText+="\n Date ";
    MessageText+=__DATE__;MessageText+="\n";
    device->getGUIEnvironment()->addMessageBox(Caption.c_str(),MessageText.c_str());gwact=1;}

video::IImage*skbn;
bool loadTexture(const c8*fn,bool alp=0){
    core::stringc filename(fn);if(filename.size()<2){return 0;}
    core::stringc extension,fileNm;
    core::getFileNameExtension(extension,filename);extension.make_lower();
    core::cutFilenameExtension(fileNm,filename);

    stringc workdir=device->getFileSystem()->getWorkingDirectory();
    stringc absolute=device->getFileSystem()->getAbsolutePath(filename);
    device->getFileSystem()->changeWorkingDirectoryTo(workdir.c_str());
    if(extension==".jpg"||
     extension==".pcx"||
     extension==".png"||
     extension==".ppm"||
     extension==".pgm"||
     extension==".pbm"||
     extension==".psd"||
     extension==".tga"||
     extension==".bmp"){
     video::ITexture*tmptex=Model->getMaterial(0).getTexture(sknum);
     video::ITexture*texture=device->getVideoDriver()->getTexture(filename.c_str());
     texture->grab();
     if(Model){
     device->getVideoDriver()->removeTexture(texture);
     texture=device->getVideoDriver()->getTexture(filename.c_str());
     if(!texture){return 0;}texture->grab();
//
     if(alp){
     video::IVideoDriver*driver=device->getVideoDriver();
     if(driver){ITexture*tex=Model->getMaterial(0).getTexture(0);
      skbn=tex2image(tex,driver);video::IImage*skbnz=tex2image(texture,driver);
      device->getVideoDriver()->removeTexture(texture);
      if(skbn){video::SColor color,acolor;
       s32 xi=0,yi=0,skw=tex->getSize().Width,skh=tex->getSize().Height;
       while(yi<skh){s32 o=0;color=skbn->getPixel(xi,yi);acolor=skbnz->getPixel(xi,yi).color;
        o=(acolor.getRed()+acolor.getGreen()+acolor.getBlue())/3;
        color.setAlpha(o);skbn->setPixel(xi,yi,color);xi=xi+1<skw?xi+1:0;yi+=!xi;}
       texture=image2tex(skbn,driver);
       if(texture){Model->setMaterialTexture(sknum,texture);
        E_ANIMATED_MESH_TYPE mtp=Model->getMesh()->getMeshType();
        if(mtp==(E_ANIMATED_MESH_TYPE)88){IM8MMesh*mes2=(IM8MMesh*)Model->getMesh();
         mes2->setSkin(sknum,skbn);}
        if(mtp==(E_ANIMATED_MESH_TYPE)89){IMDL7Mesh*mes2=(IMDL7Mesh*)Model->getMesh();
         mes2->setSkin(sknum,skbn);}
        if(mtp==(E_ANIMATED_MESH_TYPE)90){IMDL3Mesh*mes2=(IMDL3Mesh*)Model->getMesh();
         mes2->setSkin(sknum,skbn);}
        if(mtp==(E_ANIMATED_MESH_TYPE)91){IASCIIMesh*mes2=(IASCIIMesh*)Model->getMesh();
         mes2->setSkin(sknum,skbn);}
        device->getVideoDriver()->removeTexture(tmptex);}
       Model->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL_REF);
       delete skbn;}if(skbnz){delete skbnz;}}else{printf("No driver!!");}
      return 1;}
//
     Model->setMaterialTexture(sknum,texture);
     device->getFileSystem()->changeWorkingDirectoryTo(workdir.c_str());
     video::IVideoDriver*driver=device->getVideoDriver();
     skbn=tex2image(texture,driver);
     if(skbn){E_ANIMATED_MESH_TYPE mtp=Model->getMesh()->getMeshType();
      if(mtp==(E_ANIMATED_MESH_TYPE)88){IM8MMesh*mes2=(IM8MMesh*)Model->getMesh();
       mes2->setSkin(sknum,skbn);}
      if(mtp==(E_ANIMATED_MESH_TYPE)89){IMDL7Mesh*mes2=(IMDL7Mesh*)Model->getMesh();
       mes2->setSkin(sknum,skbn);}
      if(mtp==(E_ANIMATED_MESH_TYPE)90){IMDL3Mesh*mes2=(IMDL3Mesh*)Model->getMesh();
       mes2->setSkin(sknum,skbn);}
      if(mtp==(E_ANIMATED_MESH_TYPE)91){IASCIIMesh*mes2=(IASCIIMesh*)Model->getMesh();
       mes2->setSkin(sknum,skbn);}delete skbn;}
     device->getVideoDriver()->removeTexture(tmptex);}
    }return 0;
}
core::stringc lfilename;
bool loadModel(const c8*fn){
    core::stringc filename(fn);if(filename.size()<2){return 0;}
    //if(filename==lfilename&&Model){return 1;}
    io::IFileSystem*fs=device->getFileSystem();if(!fs->existFile(fn)){return 0;}
    core::stringc extension,fileNm;
    core::getFileNameExtension(extension,filename);extension.make_lower();
    core::cutFilenameExtension(fileNm,filename);

    stringc workdir=device->getFileSystem()->getWorkingDirectory();
    stringc absolute=device->getFileSystem()->getAbsolutePath(filename);
    device->getFileSystem()->changeWorkingDirectoryTo(workdir.c_str());
    if(extension==".jpg"||
     extension==".pcx"||
     extension==".png"||
     extension==".ppm"||
     extension==".pgm"||
     extension==".pbm"||
     extension==".psd"||
     extension==".tga"||
     extension==".bmp"){return 0;}

    // if a archive is loaded add it to the FileSystems..
    if(extension==".pk3"||extension==".zip"){
     device->getFileSystem()->addZipFileArchive(filename.c_str());return 0;}

    // load a model into the engine
    //if(filename==lfilename&&Model){return 1;}
    lfilename=filename;if(Model){rvm=Model;}
    ITimer*timerx=device->getTimer();f32 tm=timerx->getRealTime();
    scene::IAnimatedMesh*im=device->getSceneManager()->getMesh(filename.c_str());
    printf("\n->>Import time(%f)..\n\n",(timerx->getRealTime()-tm)/1000);
    if(!im){
     device->getGUIEnvironment()->addMessageBox(
     Caption.c_str(), L"The model could not be loaded. "\
     L"Maybe it is not a supported file format.");gwact=1;
     device->getFileSystem()->changeWorkingDirectoryTo(workdir.c_str());
     return 0;
    }

    // set default material properties
    scene::ISceneNode*sceneRoot=device->getSceneManager()->getRootSceneNode();
    IAnimatedMeshSceneNode*ims=0;
    //z{
    if(im&&sceneRoot){ims=device->getSceneManager()->addAnimatedMeshSceneNode(im,sceneRoot);
     if(!ims){return 0;}Model=ims;
     //Model->setMaterialFlag(EMF_LIGHTING,false);//t=black
     Model->setMaterialFlag(EMF_ZWRITE_ENABLE,true);
     Model->setMaterialFlag(EMF_WIREFRAME,false);
     Model->setMaterialFlag(EMF_BACK_FACE_CULLING,true);
     //Model->setMaterialFlag(EMF_GOURAUD_SHADING,true);
     Model->setMaterialFlag(EMF_BILINEAR_FILTER,true);
     Model->setMaterialFlag(EMF_TRILINEAR_FILTER,false);
     Model->setMaterialFlag(EMF_ANISOTROPIC_FILTER,false);
     Model->setMaterialFlag(EMF_FOG_ENABLE,true);
     Model->setMaterialFlag(EMF_NORMALIZE_NORMALS,true);//
     Model->setMaterialFlag(EMF_TEXTURE_WRAP,true);//
     Model->setMaterialFlag(EMF_ZBUFFER,true);//f
     Model->setMaterialFlag(EMF_POINTCLOUD,false);
     //Model->addShadowVolumeSceneNode(false);
     Model->setMaterialFlag(video::EMF_BACK_FACE_CULLING,true);
     Model->setDebugDataVisible(scene::EDS_OFF);
     Model->setAnimationSpeed(0);

////////////
     printf("\ntex? %d",!!Model->getMaterial(0).getTexture(0));
     if(Model&&extension!=".m8m"&&extension!=".mdl"&&
      (!Model->getMaterial(0).getTexture(0)||extension==".txt")){
      core::stringc fileNmx,fileNmz,fileNmt=" ";
      core::cutFilenameExtension(fileNmx,filename);
      fileNmz=fileNmx;fileNmz+=".jpg";if(fs->existFile(fileNmz.c_str())){fileNmt=fileNmz;}
      fileNmz=fileNmx;fileNmz+=".pcx";if(fs->existFile(fileNmz.c_str())){fileNmt=fileNmz;}
      fileNmz=fileNmx;fileNmz+=".png";if(fs->existFile(fileNmz.c_str())){fileNmt=fileNmz;}
      fileNmz=fileNmx;fileNmz+=".ppm";if(fs->existFile(fileNmz.c_str())){fileNmt=fileNmz;}
      fileNmz=fileNmx;fileNmz+=".pgm";if(fs->existFile(fileNmz.c_str())){fileNmt=fileNmz;}
      fileNmz=fileNmx;fileNmz+=".pbm";if(fs->existFile(fileNmz.c_str())){fileNmt=fileNmz;}
      fileNmz=fileNmx;fileNmz+=".psd";if(fs->existFile(fileNmz.c_str())){fileNmt=fileNmz;}
      fileNmz=fileNmx;fileNmz+=".tga";if(fs->existFile(fileNmz.c_str())){fileNmt=fileNmz;}
      fileNmz=fileNmx;fileNmz+=".bmp";if(fs->existFile(fileNmz.c_str())){fileNmt=fileNmz;}
      if(fileNmt==" "){
       fileNmz="skin.jpg";if(fs->existFile(fileNmz.c_str())){fileNmt=fileNmz;}
       fileNmz="skin.pcx";if(fs->existFile(fileNmz.c_str())){fileNmt=fileNmz;}
       fileNmz="skin.png";if(fs->existFile(fileNmz.c_str())){fileNmt=fileNmz;}
       fileNmz="skin.ppm";if(fs->existFile(fileNmz.c_str())){fileNmt=fileNmz;}
       fileNmz="skin.pgm";if(fs->existFile(fileNmz.c_str())){fileNmt=fileNmz;}
       fileNmz="skin.pbm";if(fs->existFile(fileNmz.c_str())){fileNmt=fileNmz;}
       fileNmz="skin.psd";if(fs->existFile(fileNmz.c_str())){fileNmt=fileNmz;}
       fileNmz="skin.tga";if(fs->existFile(fileNmz.c_str())){fileNmt=fileNmz;}
       fileNmz="skin.bmp";if(fs->existFile(fileNmz.c_str())){fileNmt=fileNmz;}}
     stringc workdir=device->getFileSystem()->getWorkingDirectory();
     stringc absolute=device->getFileSystem()->getAbsolutePath(filename);
     device->getFileSystem()->changeWorkingDirectoryTo(workdir.c_str());

     video::ITexture*texture=device->getVideoDriver()->getTexture(fileNmt.c_str());
     printf("\n.(%s) tex(%s);",extension.c_str(),fileNmt.c_str());
     if(texture){
      device->getVideoDriver()->removeTexture(texture);
      texture=device->getVideoDriver()->getTexture(fileNmt.c_str());
      Model->setMaterialTexture(0,texture);
      video::IVideoDriver*driver=device->getVideoDriver();
      skbn=tex2image(texture,driver);
      if(skbn){E_ANIMATED_MESH_TYPE mtp=Model->getMesh()->getMeshType();
       if(mtp==(E_ANIMATED_MESH_TYPE)91){IASCIIMesh*mes2=(IASCIIMesh*)Model->getMesh();
        mes2->setSkin(0,skbn);}delete skbn;}
     }}
////////////
    }return 1;//z}
}

void createToolBox(){
    // remove tool box if already there
    IGUIEnvironment*env=device->getGUIEnvironment();
    IGUIElement*root=env->getRootGUIElement();
    IGUIElement*e=root->getElementFromId(5000,true);
    if(e){e->remove();}

    // create the toolbox window
    IGUIWindow*wnd=env->addWindow(core::rect<s32>(10,40,230,520),false,L"Toolset",0,5000);
    // create tab control and tabs
    IGUITabControl*tab=env->addTabControl(core::rect<s32>(2,20,800-602,480-7),wnd,false,false);

    IGUITab*t1=tab->addTab(L"Scale");
    // add some edit boxes and a button to tab one
    env->addEditBox(L"1.0",core::rect<s32>(20,50,80,70),true,t1,901);
    env->addEditBox(L"1.0",core::rect<s32>(20,80,80,100),true,t1,902);
    env->addEditBox(L"1.0",core::rect<s32>(20,110,80,130),true,t1,903);
    vx=v0-v1;
    ls="";ls+=vx.X;env->addStaticText(ls.c_str(),core::rect<s32>(85,50,145,70),true,true,t1,30901);
    ls="";ls+=vx.Y;env->addStaticText(ls.c_str(),core::rect<s32>(85,80,145,100),true,true,t1,30902);
    ls="";ls+=vx.Z;env->addStaticText(ls.c_str(),core::rect<s32>(85,110,145,130),true,true,t1,30903);
    env->addButton(core::rect<s32>(10,150,100,190),t1,1101,L"set");
    // add senseless checkbox
    env->addCheckBox(true,core::rect<s32>(10,220,200,240),t1,-1,L"Senseless Checkbox");
    // add undocumentated transparent control
    env->addStaticText(L"Transparent Control:",core::rect<s32>(10,240,150,260),true,false,t1);
    IGUIScrollBar*scrollbar=env->addScrollBar(true,core::rect<s32>(10,260,150,275),t1,104);

    IGUITab*t2=tab->addTab(L"Translate");
    // add some edit boxes and a button to tab one
    env->addEditBox(L"1.0",core::rect<s32>(20,50,80,70),true,t2,904);
    env->addEditBox(L"1.0",core::rect<s32>(20,80,80,100),true,t2,905);
    env->addEditBox(L"1.0",core::rect<s32>(20,110,80,130),true,t2,906);
    vx=v3f(0,0,0);if(Model){vx=Model->getPosition();}
    ls="";ls+=vx.X;env->addStaticText(ls.c_str(),core::rect<s32>(85,50,145,70),true,true,t2,30904);
    ls="";ls+=vx.Y;env->addStaticText(ls.c_str(),core::rect<s32>(85,80,145,100),true,true,t2,30905);
    ls="";ls+=vx.Z;env->addStaticText(ls.c_str(),core::rect<s32>(85,110,145,130),true,true,t2,30906);
    env->addButton(core::rect<s32>(10,150,100,190),t2,11010,L"set");

    IGUITab*t3=tab->addTab(L"Rotate");
    // add some edit boxes and a button to tab one
    env->addEditBox(L"1.0",core::rect<s32>(20,50,80,70),true,t3,907);
    env->addEditBox(L"1.0",core::rect<s32>(20,80,80,100),true,t3,908);
    env->addEditBox(L"1.0",core::rect<s32>(20,110,80,130),true,t3,909);
    vx=v3f(0,0,0);if(Model){vx=Model->getRotation();}
    ls="";ls+=vx.X;env->addStaticText(ls.c_str(),core::rect<s32>(85,50,145,70),true,true,t3,30907);
    ls="";ls+=vx.Y;env->addStaticText(ls.c_str(),core::rect<s32>(85,80,145,100),true,true,t3,30908);
    ls="";ls+=vx.Z;env->addStaticText(ls.c_str(),core::rect<s32>(85,110,145,130),true,true,t3,30909);
    env->addButton(core::rect<s32>(10,150,100,190),t3,11011,L"set");
}

bool OnEventx(const SEvent& ev){
   if(ev.EventType==EET_GUI_EVENT){s32 kk=ev.GUIEvent.EventType;
    if(kk==EGET_FILE_SELECTED||kk==EGET_FILE_CHOOSE_DIALOG_CANCELLED||
     kk==EGET_MESSAGEBOX_YES||kk==EGET_MESSAGEBOX_NO||kk==EGET_MESSAGEBOX_OK||
     kk==EGET_MESSAGEBOX_CANCEL||kk==EGET_ELEMENT_CLOSED){gwact=0;}}
     eMOUSE_LEFT*=!gwact;eMOUSE_RIGHT*=!gwact;
   if(Camera[0]&&ev.EventType==irr::EET_KEY_INPUT_EVENT&&
     ev.KeyInput.PressedDown){
     if(eKEY_SHIFT&&m){v3f ps0=m->getPosition();
      switch(ev.KeyInput.Key){
       case KEY_UP:{ps0.X+=2.0f;}break;
       case KEY_DOWN:{ps0.X+=-2.0f;}break;
       case KEY_LEFT:{ps0.Z+=-2.0f;}break;
       case KEY_RIGHT:{ps0.Z+=2.0f;}break;}
      m->setPosition(ps0);}
   }

   //Escape swaps Camera Input
   if(ev.EventType==EET_KEY_INPUT_EVENT&&ev.KeyInput.Key==irr::KEY_ESCAPE&&
     ev.KeyInput.PressedDown==false){
     if(device){
      scene::ICameraSceneNode*camera=device->getSceneManager()->getActiveCamera();
      if(camera){
       camera->setInputReceiverEnabled(!camera->isInputReceiverEnabled());
      }return true;
     }
   }

   if(ev.EventType==EET_GUI_EVENT){
     s32 id=ev.GUIEvent.Caller->getID();
     IGUIEnvironment*env=device->getGUIEnvironment();
     switch(ev.GUIEvent.EventType){
     case EGET_MENU_ITEM_SELECTED:{
        // a menu item was clicked
        IGUIContextMenu*menu=(IGUIContextMenu*)ev.GUIEvent.Caller;
        s32 id=menu->getItemCommandId(menu->getSelectedItem());
        switch(id){
        case 101: // load the model file
         env->addFileOpenDialog(L"Please select file");gwact=1;
         break;
        case 102: // clear the current model
          if(Model){rvm=Model;}
        break;
        case 9999:
         if(Model){
          IGUIFileOpenDialog*d=env->addFileOpenDialog(L"Please select texture file");ltex=1;gwact=1;
         }
         break;
        case 9998:
         if(Model){
          IGUIFileOpenDialog*d=env->addFileOpenDialog(L"Please select texture file");alpx=1;gwact=1;
         }
         break;
        case 10000:
         if(Model){ITimer*timerx=device->getTimer();f32 tm=timerx->getRealTime();
          if(writem(device->getSceneManager(),Model->getMesh(),0)){
           printf("\n->>Exported OK. time(%f)..\n\n",
           (timerx->getRealTime()-tm)/1000);}else{printf("\n->>Error...");}
          }
         break;
        case 10001:
         if(Model){ITimer*timerx=device->getTimer();f32 tm=timerx->getRealTime();
          if(writem(device->getSceneManager(),Model->getMesh(),1)){
           printf("\n->>Exported OK. time(%f)..\n\n",
           (timerx->getRealTime()-tm)/1000);}else{printf("\n->>Error...");}
          }
         break;
        case 10002:
         if(Model){ITimer*timerx=device->getTimer();f32 tm=timerx->getRealTime();
          if(writem(device->getSceneManager(),Model->getMesh(),2)){
           printf("\n->>Exported OK. time(%f)..\n\n",
           (timerx->getRealTime()-tm)/1000);}else{printf("\n->>Error...");}
          }
         break;
        case 10003:
         if(Model){ITimer*timerx=device->getTimer();f32 tm=timerx->getRealTime();
          if(writem(device->getSceneManager(),Model->getMesh(),3)){
           printf("\n->>Exported OK. time(%f)..\n\n",
           (timerx->getRealTime()-tm)/1000);}else{printf("\n->>Error...");}
          }
         break;
        case 10102:
         if(Model){ITimer*timerx=device->getTimer();f32 tm=timerx->getRealTime();edir();
          if(writexframe(device->getSceneManager(),Model->getMesh(),0)){
           printf("\n->>Exported OK. time(%f)..\n\n",
           (timerx->getRealTime()-tm)/1000);}else{printf("\n->>Error...");}
          }
         break;
        case 10103:
         if(Model){ITimer*timerx=device->getTimer();f32 tm=timerx->getRealTime();edir();
          if(writexframe(device->getSceneManager(),Model->getMesh(),1)){
           printf("\n->>Exported OK. time(%f)..\n\n",
           (timerx->getRealTime()-tm)/1000);}else{printf("\n->>Error...");}
          }
         break;
        case 10104:
         if(Model){ITimer*timerx=device->getTimer();f32 tm=timerx->getRealTime();edir();
          if(writexframe(device->getSceneManager(),Model->getMesh(),2)){
           printf("\n->>Exported OK. time(%f)..\n\n",
           (timerx->getRealTime()-tm)/1000);}else{printf("\n->>Error...");}
          }
         break;
        case 10010://exp texture BMP
         if(Model){video::IVideoDriver*driver=device->getVideoDriver();
           if(driver){ITexture*tex=Model->getMaterial(0).getTexture(0);
            skbn=tex2image(tex,driver);
            if(skbn){driver->writeImageToFile(skbn,"Exp_Texture.bmp");
             printf("Ok");delete skbn;}}else{printf("No driver!!");}
           }
         break;
        case 10011://exp texture TGA
         if(Model){video::IVideoDriver*driver=device->getVideoDriver();
           if(driver){ITexture*tex=Model->getMaterial(0).getTexture(0);
            skbn=tex2image(tex,driver);
            if(skbn){driver->writeImageToFile(skbn,"Exp_Texture.tga");
             printf("Ok");delete skbn;}}else{printf("No driver!!");}
           }
         break;
        case 10012://exp alpha
         if(Model){video::IVideoDriver*driver=device->getVideoDriver();
           if(driver){ITexture*tex=Model->getMaterial(0).getTexture(0);
            skbn=tex2image(tex,driver);
            if(skbn){video::SColor color;
             s32 c=0,xi=0,yi=0,skw=tex->getSize().Width,skh=tex->getSize().Height;
             while(yi<skh){s32 o=0;color=skbn->getPixel(xi,yi);o=color.getAlpha();
              TEXBFR[3+c*4]=255;TEXBFR[2+c*4]=o;TEXBFR[1+c*4]=o;TEXBFR[0+c*4]=o;
              xi=xi+1<skw?xi+1:0;yi+=!xi;c++;}
             c=0;while(c<skw*skh){color=video::SColor(255,TEXBFR[2+c*4],TEXBFR[1+c*4],
              TEXBFR[0+c*4]);TEXBFR[c]=color.color;c++;}
             video::IImage*skbnz=driver->createImageFromData(tex->getColorFormat(),
              tex->getSize(),TEXBFR,0,1);
             if(skbnz){driver->writeImageToFile(skbnz,"Exp_Alpha.bmp");
             printf("Ok");delete skbnz;}delete skbn;}}else{printf("No driver!!");}
           }
         break;
        case 200: // File -> Quit
         device->closeDevice();vquit=1;
         break;
        case 301: // View -> Grid
         grid->setVisible(!grid->isVisible());
         break;
        case 302: // View -> skin
         if(spr){spr->setVisible(!spr->isVisible());
          if(sprx){sprx->setVisible(spr->isVisible());}}
        break;
        case 400:
         if(Model){Model->setDebugDataVisible(scene::EDS_OFF);}
         break;
        case 410:
         if(Model){
          Model->setDebugDataVisible((scene::E_DEBUG_SCENE_TYPE)(
           Model->isDebugDataVisible()^scene::EDS_BBOX));}
         break;
        case 420:
         if(Model){
          Model->setDebugDataVisible((scene::E_DEBUG_SCENE_TYPE)(
           Model->isDebugDataVisible()^scene::EDS_NORMALS));}
         break;
        case 430:
         if(Model){
          Model->setDebugDataVisible((scene::E_DEBUG_SCENE_TYPE)(
           Model->isDebugDataVisible()^scene::EDS_SKELETON));}
         break;
        case 440:
         if(Model){
          Model->setDebugDataVisible((scene::E_DEBUG_SCENE_TYPE)(
           Model->isDebugDataVisible()^scene::EDS_MESH_WIRE_OVERLAY));}
         break;
        case 450:
         if(Model){
          Model->setDebugDataVisible((scene::E_DEBUG_SCENE_TYPE)(
           Model->isDebugDataVisible()^scene::EDS_HALF_TRANSPARENCY));}
         break;
        case 460:
         if(Model){
          Model->setDebugDataVisible((scene::E_DEBUG_SCENE_TYPE)(
           Model->isDebugDataVisible()^scene::EDS_BBOX_BUFFERS));}
         break;
        case 499:
         if(Model){Model->setDebugDataVisible(scene::EDS_FULL);}
         break;
        case 500: // Help->About
         showAboutText();break;
        case 610: // View -> Material -> Solid
         if(Model){Model->setMaterialType(video::EMT_SOLID);}
         break;
        case 620: // View -> Material -> Transparent
         if(Model){Model->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);}
         break;
        case 630: // View -> Material -> Reflection
         if(Model){Model->setMaterialType(video::EMT_SPHERE_MAP);}
         break;

        case 2000: // enable single view
         viewports=false;
         break;
        case 2010: // enable quad view
         viewports=true;
         break;
        case 2100:
          if(Model->getType()==ESNT_ANIMATED_MESH){
           s32 s=(s32)Model->getFrameNr()-32,e=(s32)Model->getFrameNr()+32,
            c=(s32)Model->getMesh()->getFrameCount();s=s>0?s:0;e<c?e:c;
            Model->setFrameLoop(s,e);Model->setAnimationSpeed(0);}
          break;
        case 2101:
          if(Model->getType()==ESNT_ANIMATED_MESH){
           s32 s=(s32)Model->getFrameNr()-32,e=(s32)Model->getFrameNr()+32,
            c=(s32)Model->getMesh()->getFrameCount();s=s>0?s:0;e<c?e:c;
            Model->setFrameLoop(s,e);Model->setAnimationSpeed(5);}
          break;
        case 2102:
          if(Model->getType()==ESNT_ANIMATED_MESH){
           s32 s=(s32)Model->getFrameNr()-32,e=(s32)Model->getFrameNr()+32,
            c=(s32)Model->getMesh()->getFrameCount();s=s>0?s:0;e<c?e:c;
            Model->setFrameLoop(s,e);Model->setAnimationSpeed(15);}
          break;
        case 2103:
          if(Model->getType()==ESNT_ANIMATED_MESH){
           s32 s=(s32)Model->getFrameNr()-32,e=(s32)Model->getFrameNr()+32,
            c=(s32)Model->getMesh()->getFrameCount();s=s>0?s:0;e<c?e:c;
            Model->setFrameLoop(s,e);Model->setAnimationSpeed(30);}
          break;
        case 2104:
          if(Model->getType()==ESNT_ANIMATED_MESH){
           s32 s=(s32)Model->getFrameNr()-32,e=(s32)Model->getFrameNr()+32,
            c=(s32)Model->getMesh()->getFrameCount();s=s>0?s:0;e<c?e:c;
            Model->setFrameLoop(s,e);Model->setAnimationSpeed(-5);}
          break;
        case 2105:
          if(Model->getType()==ESNT_ANIMATED_MESH){
           s32 s=(s32)Model->getFrameNr()-32,e=(s32)Model->getFrameNr()+32,
            c=(s32)Model->getMesh()->getFrameCount();s=s>0?s:0;e<c?e:c;
            Model->setFrameLoop(s,e);Model->setAnimationSpeed(-15);}
          break;
        case 2106:
          if(Model->getType()==ESNT_ANIMATED_MESH){
           s32 s=(s32)Model->getFrameNr()-32,e=(s32)Model->getFrameNr()+32,
            c=(s32)Model->getMesh()->getFrameCount();s=s>0?s:0;e<c?e:c;
            Model->setFrameLoop(s,e);Model->setAnimationSpeed(-30);}
          break;
        case 6001:
         createToolBox();
         break;
        }
      break;}

     case EGET_FILE_SELECTED:{
        // load the model file, selected in the file open dialog
        IGUIFileOpenDialog*dialog=(IGUIFileOpenDialog*)ev.GUIEvent.Caller;
///////////////////////////////////////////////////////////////////////////////////
        core::stringc filename(dialog->getFileName());

        core::stringc extension,fileNm;
        core::getFileNameExtension(extension,filename);
        extension.make_lower();
        core::cutFilenameExtension(fileNm,filename);

        stringc workdir=device->getFileSystem()->getWorkingDirectory();
        stringc absolute=device->getFileSystem()->getAbsolutePath(filename);
        device->getFileSystem()->changeWorkingDirectoryTo(workdir.c_str());
        if(extension==".jpg"||
         extension==".pcx"||
         extension==".png"||
         extension==".ppm"||
         extension==".pgm"||
         extension==".pbm"||
         extension==".psd"||
         extension==".tga"||
         extension==".bmp"){
          loadTexture(core::stringc(dialog->getFileName()).c_str(),alpx);}
         else{if(!alpx&&!ltex){loadModel(core::stringc(dialog->getFileName()).c_str());}}
         alpx=0;ltex=0;
///////////////////////////////////////////////////////////////////////////////////
      }
     case EGET_SCROLL_BAR_CHANGED:
      // control skin transparency
      if(id==104){
        s32 pos=((IGUIScrollBar*)ev.GUIEvent.Caller)->getPos();
        s32 i=0;while(i<irr::gui::EGDC_COUNT){
         video::SColor col=env->getSkin()->getColor((EGUI_DEFAULT_COLOR)i);
         if(!pos){pos=255;}col.setAlpha(pos);
         env->getSkin()->setColor((EGUI_DEFAULT_COLOR)i,col);
         i++;}
      }
      if(id==11001&&Model){
        if(Model->getType()==ESNT_ANIMATED_MESH){
         fracnt=(s32)Model->getMesh()->getFrameCount();
         fracnt=fracnt/4>0&&fracnt/4<5000?fracnt:1;
         ((IGUIScrollBar*)ev.GUIEvent.Caller)->setMax(fracnt/4-1);
         ((IGUIScrollBar*)ev.GUIEvent.Caller)->setSmallStep(1);
         ((IGUIScrollBar*)ev.GUIEvent.Caller)->setLargeStep(2);
         frampos=((IGUIScrollBar*)ev.GUIEvent.Caller)->getPos()*4;
         frampos=frampos/4>0&&frampos/4<5000?frampos:0;
         Model->setFrameLoop(frampos,frampos);
         if(Model){Model->setAnimationSpeed(0.00001f);
          Model->setCurrentFrame(frampos);}}
      }break;

     case EGET_COMBO_BOX_CHANGED:
      // control anti-aliasing/filtering
      if(id==108){
        s32 pos=((IGUIComboBox*)ev.GUIEvent.Caller)->getSelected();
        switch(pos){
         case 0:
         if(Model){
          Model->setMaterialFlag(video::EMF_BILINEAR_FILTER,false);
          Model->setMaterialFlag(video::EMF_TRILINEAR_FILTER,false);
          Model->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER,false);
         }
         break;
         case 1:
         if(Model){
          Model->setMaterialFlag(video::EMF_BILINEAR_FILTER,true);
          Model->setMaterialFlag(video::EMF_TRILINEAR_FILTER,false);
         }
         break;
         case 2:
         if(Model){
          Model->setMaterialFlag(video::EMF_BILINEAR_FILTER,false);
          Model->setMaterialFlag(video::EMF_TRILINEAR_FILTER,true);
         }
         break;
         case 3:
         if(Model){
          Model->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER,true);
         }
         break;
         case 4:
         if(Model){
          Model->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER,false);
         }
         break;
        }
      }
////////////////
     case EGET_LISTBOX_CHANGED:
      switch(id){
      case 890:
          break;
      }
     break;
     case EGET_LISTBOX_SELECTED_AGAIN:
      switch(id){
      case 890:
          break;
      }
     break;
/////////////////////
     case EGET_BUTTON_CLICKED:
      switch(id){
/////////////////
      case 1200:
         break;
      case 1201:
         break;
      case 1202:
         printf("The windows as been closed!!!");
         break;
/////////////////
      case 1101:
        {
         // set scale
         gui::IGUIElement*root=env->getRootGUIElement();
         core::vector3df scale;core::stringc s;
         s=root->getElementFromId(901,true)->getText();
         scale.X=(f32)atof(s.c_str());
         s=root->getElementFromId(902,true)->getText();
         scale.Y=(f32)atof(s.c_str());
         s=root->getElementFromId(903,true)->getText();
         scale.Z=(f32)atof(s.c_str());
         if(Model){Model->setScale(scale);
          vx=v0-v1;
          ls="";ls+=vx.X;root->getElementFromId(30901,true)->setText(ls.c_str());
          ls="";ls+=vx.Y;root->getElementFromId(30902,true)->setText(ls.c_str());
          ls="";ls+=vx.Z;root->getElementFromId(30903,true)->setText(ls.c_str());
         }
        }
        break;
      case 11010:
        {
         // set translate
         gui::IGUIElement*root=env->getRootGUIElement();
         core::vector3df translate;core::stringc s;
         s=root->getElementFromId(904,true)->getText();
         translate.X=(f32)atof(s.c_str());
         s=root->getElementFromId(905,true)->getText();
         translate.Y=(f32)atof(s.c_str());
         s=root->getElementFromId(906,true)->getText();
         translate.Z=(f32)atof(s.c_str());
         if(Model){Model->setPosition(translate);
          vx=Model->getPosition();
          ls="";ls+=vx.X;root->getElementFromId(30904,true)->setText(ls.c_str());
          ls="";ls+=vx.Y;root->getElementFromId(30905,true)->setText(ls.c_str());
          ls="";ls+=vx.Z;root->getElementFromId(30906,true)->setText(ls.c_str());
         }
        }
        break;
      case 11011:
        {
         // set rotate
         gui::IGUIElement*root=env->getRootGUIElement();
         core::vector3df rotatn;core::stringc s;
         s=root->getElementFromId(907,true)->getText();
         rotatn.X=(f32)atof(s.c_str());
         s=root->getElementFromId(908,true)->getText();
         rotatn.Y=(f32)atof(s.c_str());
         s=root->getElementFromId(909,true)->getText();
         rotatn.Z=(f32)atof(s.c_str());
         if(Model){Model->setRotation(rotatn);
          vx=Model->getRotation();
          ls="";ls+=vx.X;root->getElementFromId(30907,true)->setText(ls.c_str());
          ls="";ls+=vx.Y;root->getElementFromId(30908,true)->setText(ls.c_str());
          ls="";ls+=vx.Z;root->getElementFromId(30909,true)->setText(ls.c_str());
         }
        }
        break;
      case 1102:
        env->addFileOpenDialog(L"Please select a model file to open");gwact=1;
        break;
      case 1103:
        showAboutText();
        break;
      case 1104:
        createToolBox();
        break;
      }
      break;
     }
     return false;
   }
   if(ev.EventType==EET_MOUSE_INPUT_EVENT){
     scene::ICameraSceneNode*cam=device->getSceneManager()->getActiveCamera();
     v3f p=v3f(ev.MouseInput.X,ev.MouseInput.Y,Camera[0]->getPosition().Z);
     switch(ev.MouseInput.Event){
      case EMIE_MOUSE_WHEEL:
        {
        }
        break;
      case EMIE_RMOUSE_PRESSED_DOWN:
        {
        }
        break;
      case EMIE_LMOUSE_PRESSED_DOWN:
        {
        }
        break;
     }
   }
   return false;
}

void drawViewPorts(){
    // Left top view
    video::IVideoDriver*driver=device->getVideoDriver();
    IGUIEnvironment*env=device->getGUIEnvironment();
    scene::ISceneManager*smgr=device->getSceneManager();
    s32 scrnx=driver->getScreenSize().Width;
    s32 scrny=driver->getScreenSize().Height;
    f32 aspect=(f32)scrnx/(f32)scrny;

    f32 distance=Camera[0]->getPosition().getDistanceFrom(core::vector3d<f32>(0.0,0.0,0.0));
    Camera[1]->setPosition(core::vector3df(0,distance,0));
    Camera[1]->setTarget(core::vector3df(0,0,0));
    Camera[2]->setPosition(core::vector3df(0,0,distance));
    Camera[2]->setTarget(core::vector3df(0,0,0));
    Camera[3]->setPosition(core::vector3df(distance,0,0));
    Camera[3]->setTarget(core::vector3df(0,0,0));

    // left top view - TOP
    Camera[1]->setAspectRatio(aspect);
    device->getSceneManager()->setActiveCamera(Camera[1]);
    driver->setViewPort(core::rect<s32>(0,20,scrnx/2,(scrny/2)-10));
    smgr->drawAll();
    // Right top view - perspective
    device->getSceneManager()->setActiveCamera(Camera[0]);
    Camera[0]->setAspectRatio(aspect);
    driver->setViewPort(core::rect<s32>((scrnx/2),20,scrnx,(scrny/2)-10));
    smgr->drawAll();
    // Bottom left view - front
    device->getSceneManager()->setActiveCamera(Camera[2]);
    Camera[3]->setAspectRatio(aspect);
    driver->setViewPort(core::rect<s32>(0,(scrny/2)+2,scrnx/2,scrny));
    smgr->drawAll();
    // Bottom right view - side
    device->getSceneManager()->setActiveCamera(Camera[3]);
    Camera[3]->setAspectRatio(aspect);
    driver->setViewPort(core::rect<s32>((scrnx/2)+2,(scrny/2)+2,scrnx,scrny));
    smgr->drawAll();

    // restore the view for GUI drawing
    driver->setViewPort(core::rect<s32>(0,0,scrnx,scrny));
    // draw the viewport boundary
    smgr->setActiveCamera(Camera[0]);
    driver->draw2DRectangle(video::SColor(100,255,255,255),
     core::rect<s32>(0,scrny/2-2,scrnx,scrny/2+2));
    driver->draw2DRectangle(video::SColor(100,255,255,255),
     core::rect<s32>(scrnx/2-2,20,scrnx/2+2,scrny));
}
//////////////////////////////////////XRWrite
struct CXRWFile:public IWriteFile{
     public:
      CXRWFile(const c8*fileName,bool append):File(0),FileSize(0){
       Filename=fileName;openFile(append);}virtual ~CXRWFile(){drop();}
      virtual s32 read(void*bfr,u32 size){if(File){
       return (s32)fread(bfr,1,size,File);}return 0;}
      virtual s32 write(const void*bfr,u32 size){if(File){
       return fwrite(bfr,1,size,File);}return 0;}
      virtual bool seek(long final,bool relative=0){if(File){
       return !fseek(File,final,relative?SEEK_CUR:SEEK_SET);}return 0;}
      virtual long getSize()const{return FileSize;}
      virtual bool isOpen()const{return !!File;}
      virtual long getPos()const{return ftell(File);}
      virtual const c8*getFileName()const{return Filename.c_str();}
      virtual void drop(){if(File){fclose(File);}}
      CXRWFile*createXRWFile(const c8*fileName,bool append){
       CXRWFile*file=new CXRWFile(fileName,append);if(file->isOpen()){
        return file;}delete file;return 0;}
     private:
      virtual void openFile(bool append){if(!Filename.size()){File=0;return;}
       File=fopen(Filename.c_str(),"ab");append*=!!File;if(File){fclose(File);}
       File=fopen(Filename.c_str(),append?"r+b":"w+b");
       if(!File){File=fopen(Filename.c_str(),"rb");}
       if(File){fseek(File,0,SEEK_END);FileSize=getPos();fseek(File,0,SEEK_SET);}}
      core::stringc Filename;FILE*File;long FileSize;
};IWriteFile*createXRWFile(const c8*fileName,bool append){
     IWriteFile*file=new CXRWFile(fileName,append);return file;}
bool writem(ISceneManager*smgr,IMesh*msh,s32 tp){
    if(!smgr||!msh||tp<0||!Model){return 0;}IWriteFile*file;
     E_ANIMATED_MESH_TYPE mtp=Model->getMesh()->getMeshType();
     if(tp<2){//write m8m/mdl5
      if(mtp==(E_ANIMATED_MESH_TYPE)88){edir();IM8MMesh*imm=(IM8MMesh*)Model->getMesh();
       return !!imm->WriteM8M("M8Mexp",tp);}
      //
      if(mtp==EAMT_MD2){edir();IAnimatedMesh*imm=Model->getMesh();
       s32 i=0;IAnimatedMesh*msh=new IASCIIMesh(smgr);if(msh){IASCIIMesh*imz=(IASCIIMesh*)msh;
        if(imz->createMesh(imm)){
         video::IVideoDriver*driver=device->getVideoDriver();
         skbn=tex2image(Model->getMaterial(0).getTexture(0),driver);
         if(skbn){imz->setSkin(0,skbn);delete skbn;}
         i=imz->WriteM8M("M8Mexp",tp);}msh->drop();}return i;}
      if(mtp==(E_ANIMATED_MESH_TYPE)89){edir();IMDL7Mesh*imm=(IMDL7Mesh*)Model->getMesh();
       return !!imm->WriteM8M("M8Mexp",tp);}
      if(mtp==(E_ANIMATED_MESH_TYPE)90){edir();IMDL3Mesh*imm=(IMDL3Mesh*)Model->getMesh();
       return !!imm->WriteM8M("M8Mexp",tp);}
      if(mtp==(E_ANIMATED_MESH_TYPE)91){edir();IASCIIMesh*imm=(IASCIIMesh*)Model->getMesh();
       return !!imm->WriteM8M("M8Mexp",tp);}}
     if(tp==2||tp==3){//write md2/ascii
      if(mtp==(E_ANIMATED_MESH_TYPE)88){edir();IAnimatedMesh*imm=Model->getMesh();
       s32 i=0;IAnimatedMesh*msh=new IASCIIMesh(smgr);if(msh){IASCIIMesh*imz=(IASCIIMesh*)msh;
        if(imz->createMesh(imm)){if(tp==2){i=imz->WriteMD2("ASCIIMD2");}
         else{i=imz->WriteASCII("ASCIIM8Mexp");}}msh->drop();}return i;}
      //
      if(mtp==EAMT_MD2){edir();IAnimatedMesh*imm=Model->getMesh();
       s32 i=0;IAnimatedMesh*msh=new IASCIIMesh(smgr);if(msh){IASCIIMesh*imz=(IASCIIMesh*)msh;
        if(imz->createMesh(imm)){
         video::IVideoDriver*driver=device->getVideoDriver();
         skbn=tex2image(Model->getMaterial(0).getTexture(0),driver);
         if(skbn){imz->setSkin(0,skbn);delete skbn;}if(tp==2){i=imz->WriteMD2("ASCIIMD2");}
         else{i=imz->WriteASCII("ASCIIM8Mexp");}}msh->drop();}return i;}
      if(mtp==(E_ANIMATED_MESH_TYPE)91){edir();IASCIIMesh*imm=(IASCIIMesh*)Model->getMesh();
       s32 i=0;if(tp==2){i=imm->WriteMD2("ASCIIMD2");}else{i=imm->WriteASCII("ASCIIM8Mexp");}
      return i;}}
     return 0;}
bool writexframe(ISceneManager*smgr,IMesh*msh,s32 tp){
    if(!smgr||!msh||tp<0||tp>2){return 0;}IMeshWriter*mwr;IWriteFile*file;
     if(!tp){mwr=smgr->createMeshWriter(EMWT_IRR_MESH);
      file=createXRWFile("frame.irrmesh",0);
      mwr->writeMesh(file,m->getMesh(),EMWF_NONE);file->drop();mwr->drop();}
     if(tp==1){mwr=smgr->createMeshWriter(EMWT_COLLADA);
      file=createXRWFile("frame.dae",0);
      mwr->writeMesh(file,m->getMesh(),EMWF_NONE);file->drop();mwr->drop();}
     if(tp==2){mwr=smgr->createMeshWriter(EMWT_STL);
      file=createXRWFile("frame.stl",0);
      mwr->writeMesh(file,m->getMesh(),EMWF_NONE);file->drop();mwr->drop();}
     return 1;}
void buildx(){//BuiltIn
    CXRWFile*filex=filex->createXRWFile("build.bmp",1);
    if(filex){s32 bnum=filex->getSize();c8*U_BFR=new c8[bnum+2];
     s32 o0=0;while(o0<bnum){filex->read(&U_BFR[o0],1);o0++;}filex->drop();
     if(filex){filex=filex->createXRWFile("BuiltIn.txt",0);stringc str="";
      char hex[1];o0=0;while(o0<bnum){str="    ";
        s32 o1=o0+15;while(o0<o1&&o0<bnum){u8 o2=U_BFR[o0];_itoa(o2,hex,16);str+="0x";
        if(o2<16){str+='0';}str+=hex;o0++;if(o0<bnum){str+=",";if(o0>=o1){str+="\n";}}}
       filex->write(str.c_str(),str.size());if(o0<o1){break;}}
      filex->drop();}delete U_BFR;}
}
//////////////////////////////////////Anim
    const c8*fname,*fnamen;
    s32     frame,fpct,frames,ts,tx;
    s32     Anim(IAnimatedMeshSceneNode*m,const c8*name){if(!m){return 0;}
             ITimer*timerx=device->getTimer();m->setAnimationSpeed(0.00001f);
             if((s32)timerx->getRealTime()>=ts){ts=(s32)timerx->getRealTime()+20;
              tx=(tx+1)%8;if(!tx){fname=fnamen;}
              if(m->getMesh()->getMeshType()==(E_ANIMATED_MESH_TYPE)88){//M8M
               IM8MMesh*mes2=(IM8MMesh*)m->getMesh();s32 s,e,f,t;
               mes2->getFrameLoop(fname,s,e,f);fpct=(fpct+1)%10;t=e-s>0?e-s:0;
               frame=(e>=s&&e<=frames)*(s+((fpct*(t-1))/10)%(t-2*!!t+!t));
               m->setFrameLoop(frame,frame+4);m->setCurrentFrame(frame);
               frames=mes2->getFrameCount();}
              if(m->getMesh()->getMeshType()==EAMT_MD2){//MD2
               IAnimatedMeshMD2*mes2=(IAnimatedMeshMD2*)m->getMesh();s32 s,e,f,t;
               mes2->getFrameLoop(fname,s,e,f);fpct=(fpct+1)%10;t=e-s>0?e-s:0;
               frame=(e>=s&&e<=frames)*(s+((fpct*(t-1))/10)%(t-2*!!t+!t));
               m->setFrameLoop(frame,frame+4);m->setCurrentFrame(frame);
               frames=mes2->getFrameCount();}
              if(m->getMesh()->getMeshType()==(E_ANIMATED_MESH_TYPE)89){//MDL7
               IMDL7Mesh*mes2=(IMDL7Mesh*)m->getMesh();s32 s,e,f,t;
               mes2->getFrameLoop(fname,s,e,f);fpct=(fpct+1)%10;t=e-s>0?e-s:0;
               frame=(e>=s&&e<=frames)*(s+((fpct*(t-1))/10)%(t-2*!!t+!t));
               m->setFrameLoop(frame,frame+4);m->setCurrentFrame(frame);
               frames=mes2->getFrameCount();}
              if(m->getMesh()->getMeshType()==(E_ANIMATED_MESH_TYPE)90){//MDL3
               IMDL3Mesh*mes2=(IMDL3Mesh*)m->getMesh();s32 s,e,f,t;
               mes2->getFrameLoop(fname,s,e,f);fpct=(fpct+1)%10;t=e-s>0?e-s:0;
               frame=(e>=s&&e<=frames)*(s+((fpct*(t-1))/10)%(t-2*!!t+!t));
               m->setFrameLoop(frame,frame+4);m->setCurrentFrame(frame);
               frames=mes2->getFrameCount();}
              if(m->getMesh()->getMeshType()==(E_ANIMATED_MESH_TYPE)91){//ASCII
               IASCIIMesh*mes2=(IASCIIMesh*)m->getMesh();s32 s,e,f,t;
               mes2->getFrameLoop(fname,s,e,f);fpct=(fpct+1)%10;t=e-s>0?e-s:0;
               frame=(e>=s&&e<=frames)*(s+((fpct*(t-1))/10)%(t-2*!!t+!t));
               m->setFrameLoop(frame,frame+4);m->setCurrentFrame(frame);
               frames=mes2->getFrameCount();}
              }fnamen=fname;fname=name;return fpct*10;}
//////////////////////////////////acube
int acube(scene::ISceneManager*smgrc,f32 cubeSize=5.0f){
    video::SColor cubeColour(255,255,255,255);
/*
    6---------7
   /|        /|
 /  |      /  |
2---------3   |
|   |     |   |
|   4-----|---5
|  /      |  /
|/        |/
0---------1
*/
    SMeshBuffer*bfrc=new SMeshBuffer();
    u16 u[36]={4,0,5, 5,0,1, 5,1,6, 6,1,2, 6,2,7,
     7,2,3, 7,3,4, 4,3,0, 7,4,6, 6,4,5, 0,3,1, 1,3,2};
    v3f v[8],n=v3f(0.02f,-0.1f,0);vector2df v2=vector2df(0,1);
    v[0]=v3f(0,0,1);v[1]=v3f(1,0,1);v[2]=v3f(1,1,1);v[3]=v3f(0,1,1);
    v[4]=v3f(0,0,0);v[5]=v3f(1,0,0);v[6]=v3f(1,1,0);v[7]=v3f(0,1,0);
    bfrc->Indices.set_used(36);
    s32 i=0;while(i<36){bfrc->Indices[i]=u[i];i++;}

    bfrc->Vertices.set_used(8);
    bfrc->Vertices[0]=video::S3DVertex(v[0],n,cubeColour,vector2df(0,0));
    bfrc->Vertices[1]=video::S3DVertex(v[1],n,cubeColour,vector2df(1,1));
    bfrc->Vertices[2]=video::S3DVertex(v[2],n,cubeColour,vector2df(0,0));
    bfrc->Vertices[3]=video::S3DVertex(v[3],n,cubeColour,vector2df(1,1));
    bfrc->Vertices[4]=video::S3DVertex(v[4],n,cubeColour,vector2df(0,0));
    bfrc->Vertices[5]=video::S3DVertex(v[5],n,cubeColour,vector2df(1,1));
    bfrc->Vertices[6]=video::S3DVertex(v[6],n,cubeColour,vector2df(0,0));
    bfrc->Vertices[7]=video::S3DVertex(v[7],n,cubeColour,vector2df(1,1));

    bfrc->BoundingBox.reset(0,0,0);
    i=0;while(i<8){
     bfrc->Vertices[i].Pos-=core::vector3df(0.5f,0.5f,0.5f);
     bfrc->Vertices[i].Pos*=cubeSize;
     bfrc->BoundingBox.addInternalPoint(bfrc->Vertices[i].Pos);i++;}
    SMesh*cubMesh=new SMesh();cubMesh->addMeshBuffer(bfrc);
    ISceneNode*cubSceneNode=smgrc->addMeshSceneNode(cubMesh);}

//=====================================main=====================================
int argc;char**argv;int _m8mexp_();
int main(int argc0,char*argv0[]){argc=argc0;argv=argv0;_m8mexp_();return 0;}
int _m8mexp_(){static s32 sw=0;
    video::E_DRIVER_TYPE driverType=video::EDT_BURNINGSVIDEO;
    printf("m8mexp :%d|\n %s|\n %s|\n\n",argc,argv[0],argv[1]);
    SWC:sw+=(argc>=3);
    if(sw==2){driverType=video::EDT_OPENGL;}
    if(sw!=0&&sw!=2){
    printf("Please select the driver you want for this example:\n"\
        " (c) OpenGL 1.5\n"\
        " (e) Burning's Software Renderer\n"\
        " (otherKey) exit\n\n");
    char key;
    std::cin >> key;
    switch(key)
    {
		case 'a': driverType=video::EDT_DIRECT3D9;break;
		case 'b': driverType=video::EDT_DIRECT3D8;break;
		case 'c': driverType=video::EDT_OPENGL;break;
		case 'd': driverType=video::EDT_SOFTWARE;break;
		case 'e': driverType=video::EDT_BURNINGSVIDEO;break;
        default: driverType=video::EDT_BURNINGSVIDEO;break;
    }}
    einput ev;if(device){device->closeDevice();}
    device=createDevice(driverType,core::dimension2d<s32>(800,600),32,false,true,false,&ev);

    if(!device){printf("Could not create selected driver.\n");sw=1;goto SWC;}
    device->setResizeAble(true);
    device->setWindowCaption(L"Irrlicht Engine - Loading...");
//////P
    IVideoDriver*driver=device->getVideoDriver();
    ISceneManager*smgr=device->getSceneManager();
    IGUIEnvironment*guienv=device->getGUIEnvironment();
    IOSOperator*osp=device->getOSOperator();
    smgr->addExternalMeshLoader(new CM8MMeshLoader(smgr));
//////p

//////////////extload
    smgr->addExternalMeshLoader(new CMDL7MeshLoader(smgr));
    smgr->addExternalMeshLoader(new CMDL3MeshLoader(smgr));
    smgr->addExternalMeshLoader(new CASCIIMeshConverter(smgr));
//////////////extload

//////////////exedir
    if(!sw||exedir.size()<2){exedir=device->getFileSystem()->getWorkingDirectory();}
//////////////exedir

//////////////font
    edir();IGUIFont*font=guienv->getFont("fontex.bmp");
    if(font){guienv->getSkin()->setFont(font);}
//////////////font

// fog
    driver->setFog(SColor(255,255,255,255),true,100,100000,0.01f);
    driver->setAmbientLight(SColor(255,255,255,255));
    acube(smgr,0.01f);edir();

// tex
    if(emptytex&&driver){if(emptytex->getDriverType()!=driver->getDriverType()){
     driver->removeTexture(emptytex);emptytex=0;}}
    if(!emptytex&&driver){s32 szx=8,szy=8,o0=0;while(o0<szx*szy){
      TEXBFR[o0]=video::SColor(255,255,255,255).color;o0++;}
     skbn=driver->createImageFromData(ECF_A8R8G8B8,core::dimension2d<s32>(szx,szy),TEXBFR,0,1);
     if(skbn){emptytex=driver->addTexture("empty.bmp",skbn);}
     if(!emptytex){emptytex=driver->addTexture(core::dimension2d<s32>(szx,szy),"empty.bmp",
      ECF_A8R8G8B8);}emptytex->grab();}
///////////////////////////tt
    printf("\nTest:\n -%s",stringc(osp->getOperationSystemVersion()).c_str());
    u32 ttx=0,tax=0;if(osp->getProcessorSpeedMHz(&ttx)){printf("\n -Cpu Speed = %d",ttx);}
    if(osp->getSystemMemory(&ttx,&tax)){ttx/=1024;tax/=1024;
     printf(" Memory (free/Total) = %dMb/%dMb",tax,ttx);}
    printf("\n -MxTris = %d",driver->getMaximalPrimitiveCount());
    printf(" MxDlig = %d",driver->getMaximalDynamicLightAmount());
    printf("\n Drv HARDWARE_TL = %d",driver->queryFeature(EVDF_HARDWARE_TL));
    printf("\n Drv MULTITEXTURE = %d",driver->queryFeature(EVDF_MULTITEXTURE));
    printf("\n Drv BILINEAR_FILTER = %d",driver->queryFeature(EVDF_BILINEAR_FILTER));
    printf("\n Drv RENDER_TO_TARGET = %d",driver->queryFeature(EVDF_RENDER_TO_TARGET));
    printf("\n Drv ARB_FRAGMENT_PROGRAM = %d",driver->queryFeature(EVDF_ARB_FRAGMENT_PROGRAM_1));
    printf("\n Drv ARB_VERTEX_PROGRAM = %d",driver->queryFeature(EVDF_ARB_VERTEX_PROGRAM_1));
    printf("\n Drv VERTEX_SHADER_1_1 = %d",driver->queryFeature(EVDF_VERTEX_SHADER_1_1));
    printf("\n Drv PIXEL_SHADER_1_1 = %d",driver->queryFeature(EVDF_PIXEL_SHADER_1_1));
    printf("\n Drv VERTEX_SHADER_2_0 = %d",driver->queryFeature(EVDF_VERTEX_SHADER_2_0));
    printf("\n Drv PIXEL_SHADER_2_0 = %d",driver->queryFeature(EVDF_PIXEL_SHADER_2_0));
    printf("\n Drv VERTEX_SHADER_3_0 = %d",driver->queryFeature(EVDF_VERTEX_SHADER_3_0));
    printf("\n Drv PIXEL_SHADER_3_0 = %d",driver->queryFeature(EVDF_PIXEL_SHADER_3_0));
    printf("\n Drv HLSL = %d",driver->queryFeature(EVDF_HLSL));
    printf("\n Drv GLSL = %d",driver->queryFeature(EVDF_ARB_GLSL));
    printf("\n Drv TEXTURE_NPOT = %d",driver->queryFeature(EVDF_TEXTURE_NPOT));
    printf("\n Drv FRAMEBUFFER_OBJECT = %d",driver->queryFeature(EVDF_FRAMEBUFFER_OBJECT));
    printf("\n Drv STENCIL_BUFFER = %d",driver->queryFeature(EVDF_STENCIL_BUFFER));
    printf("\n Drv MIP_MAP = %d",driver->queryFeature(EVDF_MIP_MAP));
    printf("\n Drv MIP_MAP_AUTO_UPDATE = %d\n",driver->queryFeature(EVDF_MIP_MAP_AUTO_UPDATE));
    IVideoModeList*vml=device->getVideoModeList();
    s32 vmc0=vml->getVideoModeCount(),vmc1=0;
    while(vmc1<vmc0){printf("\n");s32 o0=0;while(o0<2){
      while(vmc1<vmc0-1&&vml->getVideoModeDepth(vmc1)<32){vmc1++;}
      if(vmc1<vmc0){printf(" Drv VM%d = %dx%d",vmc1,vml->getVideoModeResolution(vmc1).Width
        ,vml->getVideoModeResolution(vmc1).Height);
       printf("x%d",vml->getVideoModeDepth(vmc1));vmc1++;}vmc1++;o0++;}}printf("\n");
/////////////////////////tt
    createGrid(smgr);smgr->addLightSceneNode();
    ILightSceneNode*sun=smgr->addLightSceneNode(0,v3f(0,100,0),SColorf(1.0f,1.0f,1.0f),5000000);
    SLight lsun;lsun.AmbientColor=SColorf(0.5f,0.5f,0.5f);
    lsun.DiffuseColor=SColorf(1.0f,1.0f,1.0f);
    lsun.SpecularColor=SColorf(0.2f,0.2f,0.2f);
    lsun.Attenuation=v3f(0.0001f,0.0001f,0.0001f);
    lsun.CastShadows=true;lsun.Radius=50000000;
    lsun.Type=ELT_POINT;sun->setLightData(lsun);

    spr=0;sprx=0;sprb0=0;sprb1=0;sprb2=0;sprb3=0;sprb4=0;sprb5=0;
    sprtn=0;sprs0=0;sprs1=0;sprcr=0;sprad=0;sprde=0;

    // create menu
    gui::IGUIContextMenu*menu=guienv->addMenu();
    menu->addItem(L"File",-1,true,true);
    menu->addItem(L"View",-1,true,true);
    menu->addItem(L"Window",-1,true,true);
    menu->addItem(L"Help",-1,true,true);

    gui::IGUIContextMenu*submenu;
    submenu=menu->getSubMenu(0);
    submenu->addItem(L"Open model...",101);
    submenu->addItem(L"Clear model...",102);
    submenu->addSeparator();
    submenu->addItem(L"Export model as",-1,true,true);
    submenu->addItem(L"Export frame as",-1,true,true);
    submenu->addItem(L"Export skin",-1,true,true);
    submenu->addItem(L"Set texture...",9999);
    submenu->addItem(L"Set alpha...",9998);
    submenu->addSeparator();
    submenu->addItem(L"Quit",200);

    submenu=menu->getSubMenu(0)->getSubMenu(3);
    submenu->addItem(L"Export as .M8M ...",10000,false);
    submenu->addItem(L"Export as .MDL5 ...",10001,false);
    submenu->addItem(L"Export as .MD2 ...",10002,false);
    submenu->addItem(L"Export as .ASCII ...",10003,false);

    submenu=menu->getSubMenu(0)->getSubMenu(4);
    submenu->addItem(L"Export as .irrmesh ...",10102,false);
    submenu->addItem(L"Export as .COLLADA ...",10103,false);
    submenu->addItem(L"Export as .STL ...",10104,false);

    submenu=menu->getSubMenu(0)->getSubMenu(5);
    submenu->addItem(L"Texture BMP",10010,true);
    submenu->addItem(L"Texture TGA",10011,true);
    submenu->addItem(L"Alpha",10012,true);

    submenu=menu->getSubMenu(1);
    submenu->addItem(L"Toggle model debug information",-1,true,true);
    submenu->addItem(L"Model material",-1,true,true);
    submenu->addItem(L"View",-1,true,true);
    submenu->addSeparator();
    submenu->addItem(L"Model animation",-1,true,true);
    submenu->addSeparator();
    submenu->addItem(L"Show/hide grid", 301);
    submenu->addItem(L"Show/hide skinviewer",302);

    submenu=menu->getSubMenu(1)->getSubMenu(0);
    submenu->addItem(L"Off", 400);
    submenu->addItem(L"Bounding Box", 410);
    submenu->addItem(L"Normals", 420);
    submenu->addItem(L"Skeleton", 430);
    submenu->addItem(L"Wire overlay", 440);
    submenu->addItem(L"Half-Transparent", 450);
    submenu->addItem(L"Buffers bounding boxes", 460);
    submenu->addItem(L"All", 499);

    submenu=menu->getSubMenu(1)->getSubMenu(1);
    submenu->addItem(L"Solid", 610);
    submenu->addItem(L"Transparent", 620);
    submenu->addItem(L"Reflection", 630);

    submenu = menu->getSubMenu(1)->getSubMenu(2);
    submenu->addItem(L"Single view", 2000);
    submenu->addItem(L"Quad view", 2010);

    submenu=menu->getSubMenu(1)->getSubMenu(4);
    submenu->addItem(L"Stop Animation", 2100);
    submenu->addSeparator();
    submenu->addItem(L"Forward 5 FPS", 2101);
    submenu->addItem(L"Forward 15 FPS", 2102);
    submenu->addItem(L"Forward 30 FPS", 2103);
    submenu->addSeparator();
    submenu->addItem(L"Reverse 5 FPS", 2104);
    submenu->addItem(L"Reverse 15 FPS", 2105);
    submenu->addItem(L"Reverse 30 FPS", 2106);

    submenu=menu->getSubMenu(3);
    submenu->addItem(L"About", 500);
    submenu=menu->getSubMenu(2);
    submenu->addItem(L"Toolbar", 6001);

    // create toolbar
    gui::IGUIToolBar*bar=guienv->addToolBar();

    s32 scrnx=driver->getScreenSize().Width;
    s32 scrny=driver->getScreenSize().Height;
    IGUIButton*cccx=guienv->addButton(rect<s32>(15,4,50,23),bar,1102,L"Open",L"Button Tooltip");

    // create a combobox with some senseless texts
    gui::IGUIComboBox*box=guienv->addComboBox(core::rect<s32>(610,4,710,23),bar,108);
    box->addItem(L"No filtering");
    box->addItem(L"Bilinear");
    box->addItem(L"Trilinear");
    box->addItem(L"Anisotropic");
    box->addItem(L"Isotropic");

    // disable alpha
    s32 i=0;while(i<EGDC_COUNT){
     video::SColor col=guienv->getSkin()->getColor((gui::EGUI_DEFAULT_COLOR)i);
     col.setAlpha(255);guienv->getSkin()->setColor((gui::EGUI_DEFAULT_COLOR)i,col);
     i++;}
    IGUIStaticText*fpstext=guienv->addStaticText(L" ",rect<s32>(120,4,265,23),true,false,bar);
    IGUIStaticText*typtext=guienv->addStaticText(L" ",rect<s32>(270,4,495,23),true,false,bar);
    IGUIStaticText*flntext=guienv->addStaticText(L" ",rect<s32>(500,4,605,23),true,false,bar);

    IGUIStaticText*sbar=guienv->addStaticText(L" ",rect<s32>(0,scrny-16,scrnx,scrny),
     1,0,0,-1,1);
     sbar->setBackgroundColor(SColor(110,120,120,120));
    IGUIStaticText*ccc0c=guienv->addStaticText(L" ",rect<s32>(500,scrny-15,530,scrny),
     1,0,0,-1,1);
     ccc0c->setTextAlignment(EGUIA_UPPERLEFT,EGUIA_SCALE);
    IGUIStaticText*ccc0t=guienv->addStaticText(L" ",rect<s32>(530,scrny-15,560,scrny),
     1,0,0,-1,1);
     ccc0t->setTextAlignment(EGUIA_UPPERLEFT,EGUIA_SCALE);
    IGUIStaticText*ccc0n=guienv->addStaticText(L" ",rect<s32>(560,scrny-15,610,scrny),
     1,0,0,-1,1);
     ccc0n->setTextAlignment(EGUIA_UPPERLEFT,EGUIA_SCALE);

    IGUIScrollBar*framesbar=guienv->addScrollBar(true,rect<s32>(145,scrny-15,350,scrny),
     0,11001);
    IGUIEditBox*framesed=guienv->addEditBox(L"stand",rect<s32>(51,scrny-16,144,scrny-1),
     true,0,11002);
    framesed->setOverrideColor(SColor(255,128,64,0));
    framesed->setTextAlignment(EGUIA_UPPERLEFT,EGUIA_SCALE);
    framesed->setEnabled(false);
    IGUIButton*frameseb=guienv->addButton(rect<s32>(35,scrny-15,50,scrny),0,
     11003,L"|>",L"Play");
    frameseb->setEnabled(true);frameseb->setIsPushButton(1);

    // set window caption
    Caption=".";
    Caption += " - [";
    Caption += driver->getName();
    Caption += "]";
    device->setWindowCaption(Caption.c_str());

    IBillboardSceneNode*bill=smgr->addBillboardSceneNode();
    bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
    bill->setMaterialFlag(video::EMF_LIGHTING,false);
    bill->setMaterialFlag(video::EMF_ZBUFFER,false);
    bill->setSize(core::dimension2d<f32>(0.2f,0.2f));

    // add a camera scene node
    Camera[0]=smgr->addCameraSceneNode();
    Camera[1]=smgr->addCameraSceneNode();
     Camera[1]->setPosition(v3f(0,100,0));Camera[1]->setTarget(v3f(0,0,0));
    Camera[2]=smgr->addCameraSceneNode();
     Camera[2]->setPosition(v3f(0,0,-100));Camera[2]->setTarget(v3f(0,0,0));
    Camera[3]=smgr->addCameraSceneNode();
     Camera[3]->setPosition(v3f(100,0,0));Camera[3]->setTarget(v3f(0,0,0));

    Camera[0]->setTarget(v3f(0,0,0));
    Camera[1]->setTarget(v3f(0,0,0));Camera[0]->setPosition(v3f(80,20,0));
    Camera[2]->setTarget(v3f(0,0,0));
    Camera[3]->setTarget(v3f(0,0,0));

    Camera[0]->setNearValue(0.1f);Camera[0]->setFarValue(800000);Camera[0]->setFOV(fv);
    Camera[1]->setNearValue(0.1f);Camera[1]->setFarValue(800000);Camera[1]->setFOV(fv);
    Camera[2]->setNearValue(0.1f);Camera[2]->setFarValue(800000);Camera[2]->setFOV(fv);
    Camera[3]->setNearValue(0.1f);Camera[3]->setFarValue(800000);Camera[3]->setFOV(fv);
    setActiveCamera(Camera[0]);

//********************************run*****************************************//
/////////////////////////////////run
    if(device){s32 s=0;while(s<2){device->run();_sleep(15);s++;}printf("\neKEY_CTRL=");
     if(eKEY_CTRL&&!sw){printf("true");sw=2;}else{sw=0;printf("false");}}
    eKEY_CTRL=0;eKEY_S=0;while(!sw&&!vquit){
     if(spr&&device&&driver&&smgr){spr->setMaterialTexture(0,emptytex);}
     if(!eKEY_F){s32 s=0;while(s<2){device->run();_sleep(15);s++;}}else{device->run();}
     if(eKEY_F10||eKEY_SHIFT&&eKEY_F11){if(device){device->closeDevice();}return 1;}
     if(device->isWindowActive()){m=Model;
      driver->beginScene(true,true,video::SColor(255,55,55,55));
////////////////////////draw
     scrnx=driver->getScreenSize().Width;scrny=driver->getScreenSize().Height;
     if(viewports){drawViewPorts();}
     else{s32 scrnx=driver->getScreenSize().Width,scrny=driver->getScreenSize().Height;
      f32 aspect=(f32)scrnx/(f32)scrny;
      Camera[0]->setAspectRatio(aspect);
      fv=fv<=0?1:fv;Camera[0]->setFOV(fv);
      if(eKEY_CTRL&&eKEY_Z-eKEY_X){fv+=(eKEY_Z-eKEY_X)/50.0f;fv=!eKEY_C?fv:1;printf("\nFOV %f",fv);}
      smgr->drawAll();}device->run();

     //skin viewer
     static s32 msr,t1;s32 ntx=0;const s16 sz0=256,sz1=448,sv0=262,sv1=55;
     if(m){ITexture*tex=m->getMaterial(0).getTexture(0);
      if(tex&&tex->getPitch()>0){if(tex->getDriverType()!=driver->getDriverType()){ntx=1;}}}
     if(m){if(!spr){spr=smgr->addBillboardSceneNode();spr->setPosition(v3f(0,-201034.5f,0));
       if(!sprx){sprx=smgr->addBillboardSceneNode();sprx->setColor(video::SColor(5,80,80,80),
        video::SColor(5,50,50,50));sprx->setPosition(v3f(0,-201501.1f,0));
       sprx->setSize(core::dimension2d<f32>(1024,1024));sprx->setMaterialType(video::EMT_SOLID);
       sprx->setMaterialFlag(video::EMF_LIGHTING,false);
       sprx->setMaterialFlag(video::EMF_ZBUFFER,false);}}
      ITexture*Sktx[2]={m->getMaterial(0).getTexture(0),m->getMaterial(0).getTexture(1)};
      if(ntx){Sktx[0]=emptytex;}
      s32 o0=0,pos0=0;static s32 skd[2]={1,1},skdz[2],posm[2],vwmx,sps[2]={scrnx-sv0+posm[0],
       sv1+posm[1]};rect<s32> rxy;if(vwmx<sz0){vwmx=sz0;}if(vwmx>sz1){vwmx=sz1;}

      if(Sktx[0]&&Sktx[1]){o0=sknum%2;
       if((eKEY_SPACE||o0)&&!ntx){if(!stex){m->setMaterialTexture(3,Sktx[0]);stex=1;}
        m->setMaterialTexture(0,Sktx[1]);}
       if(!eKEY_SPACE&&!o0&&!ntx){if(m->getMaterial(0).getTexture(3)&&stex){
         m->setMaterialTexture(0,m->getMaterial(0).getTexture(3));stex=0;}}}
      if(eKEY_SPACE&&spr&&sprx){spr->setVisible(1);sprx->setVisible(1);}
      scene::IMeshCache*mc=device->getSceneManager()->getMeshCache();stringc strx=" ";
      if(mc){strx=mc->getMeshFilename(m->getMesh());}
      core::dimension2d<s32> d2dx=core::dimension2d<s32>(8,8),d2d=core::dimension2d<s32>(8,8);
      if(spr&&m->getMaterial(0).getTexture(0)&&m->isVisible()&&!ntx){
       spr->setMaterialType(video::EMT_SOLID);
       ITexture*Sktz=Sktx[0];
       spr->setMaterialTexture(0,Sktz);
       spr->setMaterialFlag(video::EMF_LIGHTING,false);
       spr->setMaterialFlag(video::EMF_ZBUFFER,false);d2dx=Sktz->getOriginalSize();
       d2d.Width=s32(min(d2dx.Width,512));d2d.Height=s32(min(d2dx.Height,512));
       spr->setSize(core::dimension2d<f32>((f32)d2d.Width,(f32)d2d.Height));
       if(spr->isVisible()){
        f32 i=201034.5f+0.9195f*(max(d2d.Width,d2d.Height)-256.0f);
        if(stringc(spr->getName())!=strx){spr->setPosition(v3f(0,-i,0));
         spr->setName(strx.c_str());}
        i=max(min(spr->getPosition().Y+(eKEY_PLUS-eKEY_MINUS)*2*!eKEY_CTRL,-i),-201500);
        if(eKEY_SHIFT&&(eKEY_PLUS||eKEY_MINUS)){printf("\n i=%f",i);}
        if(eKEY_CTRL&&(eKEY_PLUS||eKEY_MINUS)){spr->setMaterialTexture(0,emptytex);}
        spr->setPosition(v3f(0,i,0));
        Camera[1]->setPosition(v3f(0,-200800,0));
        Camera[1]->setTarget(v3f(0,i,0));
        Camera[1]->setAspectRatio(1);
        Camera[1]->setNearValue(0.1f);Camera[1]->setFarValue(800000);
        Camera[1]->setFOV(1);
        Camera[1]->setUpVector(v3f(1,0,0));Camera[1]->setInputReceiverEnabled(1);
        ICameraSceneNode*cmr=smgr->getActiveCamera();
        smgr->setActiveCamera(Camera[1]);
        sps[0]=scrnx-sv0+posm[0];sps[1]=sv1+posm[1];if(!eMOUSE_RIGHT){msr=0;}
        if(eMOUSE_RIGHT&&!msr&&eMOUSE_X>sps[0]+6&&eMOUSE_X<sps[0]+vwmx-6&&eMOUSE_Y>sps[1]+6
         &&eMOUSE_Y<sps[1]+vwmx-6||msr==1){msr=1;posm[0]=eMOUSE_X-(scrnx-sv0)-vwmx/2;
         posm[1]=eMOUSE_Y-sv1-vwmx/2;sps[0]=scrnx-sv0+posm[0];sps[1]=sv1+posm[1];}
        if(eMOUSE_LEFT&&!msr&&eMOUSE_X>sps[0]+6&&eMOUSE_X<sps[0]+vwmx-6&&eMOUSE_Y>sps[1]+6
         &&eMOUSE_Y<sps[1]+vwmx-6||msr==-1){msr=-1;}//edit
        driver->setViewPort(core::rect<s32>(sps[0],sps[1],sps[0]+vwmx,sps[1]+vwmx));
        driver->clearZBuffer();smgr->drawAll();
        driver->setViewPort(core::rect<s32>(0,0,scrnx,scrny));
        smgr->setActiveCamera(cmr);
        driver->draw2DLine(core::position2d<s32>(sps[0],sps[1]),core::position2d<s32>(
         sps[0]+vwmx,sps[1]),video::SColor(255,180,180,180));
        driver->draw2DLine(core::position2d<s32>(sps[0],sps[1]+vwmx),core::position2d<s32>(
         sps[0]+vwmx,sps[1]+vwmx),video::SColor(255,180,180,180));
        driver->draw2DLine(core::position2d<s32>(sps[0],sps[1]),core::position2d<s32>(
         sps[0],sps[1]+vwmx),video::SColor(255,180,180,180));
        driver->draw2DLine(core::position2d<s32>(sps[0]+vwmx,sps[1]),core::position2d<s32>(
         sps[0]+vwmx,sps[1]+vwmx),video::SColor(255,180,180,180));
        //skinpoints
        core::position2d<s32> p2d0,p2d1,p2d2;s32 skdm[2]={vwmx,vwmx};
        if(d2d.Width>d2d.Height){skdm[1]=s32(vwmx*(d2d.Height/f32(d2d.Width)));}
        if(d2d.Height>d2d.Width){skdm[0]=s32(vwmx*(d2d.Width/f32(d2d.Height)));}
        s32 offsdm[2]={skdm[0]<vwmx?vwmx/2-(skdm[0]/2):0,skdm[1]<vwmx?vwmx/2-(skdm[1]/2):0};
        if(m->getMesh()){s32 nnx=0;E_ANIMATED_MESH_TYPE mtp=m->getMesh()->getMeshType();
        if(mtp!=EAMT_MD3){
         IMeshBuffer*mb=m->getMesh()->getMeshBuffer(0);
         u32 vc=mb->getVertexCount();vc=vc<65536?vc:0;
         video::S3DVertex*tr=reinterpret_cast<video::S3DVertex*>(mb->getVertices());
         //poi
         struct Sp2ddraw{core::position2d<s32> p2d0,p2d1;};
         Sp2ddraw*p2ddraw=new Sp2ddraw[2*vc+!vc];
         s32 tmp2[2]={sps[0]+offsdm[0],sps[1]+offsdm[1]},ppx=0,ppy=0;f32 pf=0;
         nnx=0;while(nnx<vc&&skd[0]){
          pf=tr[nnx].TCoords.X;ppx=s32((pf<1?pf*(pf>0):1)*skdm[0])+tmp2[0];
          pf=tr[nnx].TCoords.Y;ppy=s32((pf<1?pf*(pf>0):1)*skdm[1])+tmp2[1];
          p2d0.X=ppx;p2d0.Y=ppy-2;p2d1.X=ppx;p2d1.Y=ppy+3;
          p2ddraw[nnx].p2d0=p2d0;p2ddraw[nnx].p2d1=p2d1;nnx++;
          p2d0.X=ppx-2;p2d0.Y=ppy;p2d1.X=ppx+3;p2d1.Y=ppy;
          p2ddraw[nnx].p2d0=p2d0;p2ddraw[nnx].p2d1=p2d1;nnx++;}
         nnx=0;while(nnx<vc&&skd[0]){
          driver->draw2DLine(p2ddraw[nnx].p2d0,p2ddraw[nnx].p2d1,
           SColor(255,255,235,120));nnx++;
          driver->draw2DLine(p2ddraw[nnx].p2d0,p2ddraw[nnx].p2d1,
           SColor(255,255,235,120));nnx++;}delete p2ddraw;
         //lin
         nnx=0;while(nnx<vc&&skd[1]){
          pf=tr[nnx+0].TCoords.X;p2d0.X=s32((pf<1?pf*(pf>0):1)*skdm[0])+tmp2[0];
          pf=tr[nnx+0].TCoords.Y;p2d0.Y=s32((pf<1?pf*(pf>0):1)*skdm[1])+tmp2[1];
          pf=tr[nnx+1].TCoords.X;p2d1.X=s32((pf<1?pf*(pf>0):1)*skdm[0])+tmp2[0];
          pf=tr[nnx+1].TCoords.Y;p2d1.Y=s32((pf<1?pf*(pf>0):1)*skdm[1])+tmp2[1];
          pf=tr[nnx+2].TCoords.X;p2d2.X=s32((pf<1?pf*(pf>0):1)*skdm[0])+tmp2[0];
          pf=tr[nnx+2].TCoords.Y;p2d2.Y=s32((pf<1?pf*(pf>0):1)*skdm[1])+tmp2[1];
          driver->draw2DLine(p2d0,p2d1,SColor(80,200,200,200));
          driver->draw2DLine(p2d1,p2d2,SColor(80,200,200,200));
          driver->draw2DLine(p2d2,p2d0,SColor(80,200,200,200));nnx+=3;}
         }}}
       rxy=rect<s32>(0,0,0,0);
       if(!sprb0){sprb0=guienv->addButton(rxy,0,0,L"+");}
       if(!sprb1){sprb1=guienv->addButton(rxy,0,0,L"-");}
       if(!sprb2){sprb2=guienv->addButton(rxy,0,0,L".");}
       if(!sprb3){sprb3=guienv->addButton(rxy,0,0,L"/");}
       if(!sprb4){sprb4=guienv->addButton(rxy,0,0,L"=");}
       if(!sprb5){sprb5=guienv->addButton(rxy,0,0,L"x");}
       if(!sprtn){sprtn=guienv->addButton(rxy,0,0,L"Skin0");sprtn->setVisible(0);}
       if(!sprs0){sprs0=guienv->addButton(rxy,0,0,L"<");}
       if(!sprs1){sprs1=guienv->addButton(rxy,0,0,L">");}
       if(!sprcr){sprcr=guienv->addButton(rxy,0,0,L"clear");}
       if(!sprad){sprad=guienv->addButton(rxy,0,0,L"add");}
       if(!sprde){sprde=guienv->addButton(rxy,0,0,L"del");}
       if(sprb0){rxy=rect<s32>(sps[0]+75,sps[1]+vwmx+1,sps[0]+85,sps[1]+vwmx+11);
        sprb0->setRelativePosition(rxy);if(sprb0->isPressed()){
         f32 i=min(spr->getPosition().Y+5,-i);spr->setPosition(v3f(0,i,0));}}
       if(sprb1){rxy=rect<s32>(sps[0]+86,sps[1]+vwmx+1,sps[0]+96,sps[1]+vwmx+11);
        sprb1->setRelativePosition(rxy);if(sprb1->isPressed()){
         f32 i=max(spr->getPosition().Y-5,-201500);spr->setPosition(v3f(0,i,0));}}
       if(sprb2){rxy=rect<s32>(sps[0]+97,sps[1]+vwmx+1,sps[0]+107,sps[1]+vwmx+11);
        sprb2->setRelativePosition(rxy);if(sprb2->isPressed()){skdz[0]=1;}
        if(!sprb2->isPressed()&&skdz[0]){skd[0]=!skd[0];skdz[0]=0;}}
       if(sprb3){rxy=rect<s32>(sps[0]+108,sps[1]+vwmx+1,sps[0]+118,sps[1]+vwmx+11);
        sprb3->setRelativePosition(rxy);if(sprb3->isPressed()){skdz[1]=1;}
        if(!sprb3->isPressed()&&skdz[1]){skd[1]=!skd[1];skdz[1]=0;}}
       //resize
       if(sprb4){rxy=rect<s32>(sps[0]+vwmx-33,sps[1]-10,sps[0]+vwmx-18,sps[1]);
        sprb4->setRelativePosition(rxy);
        s32 static t0;if(sprb4->isPressed()){t0=1;}if(!sprb4->isPressed()&&t0==1){t0=2;}
        if(vwmx<=sz0&&t0==2){t0=0;pos0=-1;posm[0]=-(scrnx/2+1-38);
         posm[1]=scrny/2+1-(sz1/2+30);if(sprtn){sprtn->setVisible(1);}}
        if(vwmx>=sz1&&t0==2){t0=0;pos0=(sz1-sz0)/2;if(sprtn){sprtn->setVisible(0);}}
        posm[0]+=pos0;posm[1]+=pos0;}
       if(sprtn){rxy=rect<s32>(sps[0]+vwmx-64,sps[1]-10,sps[0]+vwmx-34,sps[1]);
        sprtn->setRelativePosition(rxy);
        s32 static t0;if(sprtn->isPressed()){t0=1;}if(!sprtn->isPressed()&&t0==1){t0=2;}
         if(t0==2){t0=0;
          IGUIFileOpenDialog*d=guienv->addFileOpenDialog(L"Please select texture file");
          ltex=1;gwact=1;}}
        if(!sknum){sprtn->setText(L"Skin0");}else{sprtn->setText(L"Skin1");}}
       if(sprs0){s32 o0=30*!sprtn->isVisible();
        rxy=rect<s32>(sps[0]+vwmx-96+o0,sps[1]-10,sps[0]+vwmx-81+o0,sps[1]);
        sprs0->setRelativePosition(rxy);if(Sktx[0]&&Sktx[1]){sprs0->setEnabled(1);}
        else{sprs0->setEnabled(0);sknum=0;}if(sprs0->isPressed()){sknum=0;}}
       if(sprs1){s32 o0=30*!sprtn->isVisible();
        rxy=rect<s32>(sps[0]+vwmx-80+o0,sps[1]-10,sps[0]+vwmx-65+o0,sps[1]);
        sprs1->setRelativePosition(rxy);if(Sktx[0]&&Sktx[1]){sprs1->setEnabled(1);}
        else{sprs1->setEnabled(0);sknum=0;}if(sprs1->isPressed()){sknum=1;}}
       if(sprcr){rxy=rect<s32>(sps[0]+2,sps[1]+vwmx+1,sps[0]+32,sps[1]+vwmx+11);
        sprcr->setRelativePosition(rxy);
        if(sprcr->isPressed()){s32 szx=d2d.Width,szy=d2d.Height,o0=0;while(o0<szx*szy){
          TEXBFR[o0]=video::SColor(255,255,255,255).color;o0++;}
         skbn=driver->createImageFromData(ECF_A8R8G8B8,core::dimension2d<s32>(szx,szy),
          TEXBFR,0,1);
         m->setMaterialTexture(sknum,image2tex(skbn,driver));
         E_ANIMATED_MESH_TYPE mtp=m->getMesh()->getMeshType();
         if(mtp==(E_ANIMATED_MESH_TYPE)88){IM8MMesh*mes2=(IM8MMesh*)m->getMesh();
          mes2->setSkin(sknum,skbn);}
         if(mtp==(E_ANIMATED_MESH_TYPE)89){IMDL7Mesh*mes2=(IMDL7Mesh*)m->getMesh();
          mes2->setSkin(sknum,skbn);}
         if(mtp==(E_ANIMATED_MESH_TYPE)90){IMDL3Mesh*mes2=(IMDL3Mesh*)m->getMesh();
          mes2->setSkin(sknum,skbn);}
         if(mtp==(E_ANIMATED_MESH_TYPE)91){IASCIIMesh*mes2=(IASCIIMesh*)m->getMesh();
          mes2->setSkin(sknum,skbn);}delete skbn;
         }}
       if(sprad){rxy=rect<s32>(sps[0]+33,sps[1]+vwmx+1,sps[0]+53,sps[1]+vwmx+11);
        sprad->setRelativePosition(rxy);
        if(sprad->isPressed()&&Sktx[0]){skbn=tex2image(Sktx[0],driver);
         device->getVideoDriver()->removeTexture(m->getMaterial(0).getTexture(1));
         m->setMaterialTexture(1,0);m->setMaterialTexture(1,image2tex(skbn,driver));Sktx[1]=0;
         E_ANIMATED_MESH_TYPE mtp=m->getMesh()->getMeshType();
         if(mtp==(E_ANIMATED_MESH_TYPE)88){IM8MMesh*mes2=(IM8MMesh*)m->getMesh();
          mes2->setSkin(1,skbn);}
         if(mtp==(E_ANIMATED_MESH_TYPE)89){IMDL7Mesh*mes2=(IMDL7Mesh*)m->getMesh();
          mes2->setSkin(1,skbn);}
         if(mtp==(E_ANIMATED_MESH_TYPE)90){IMDL3Mesh*mes2=(IMDL3Mesh*)m->getMesh();
          mes2->setSkin(1,skbn);}
         if(mtp==(E_ANIMATED_MESH_TYPE)91){IASCIIMesh*mes2=(IASCIIMesh*)m->getMesh();
          mes2->setSkin(1,skbn);}delete skbn;
         }}
       if(sprde){rxy=rect<s32>(sps[0]+54,sps[1]+vwmx+1,sps[0]+74,sps[1]+vwmx+11);
        sprde->setRelativePosition(rxy);static s32 rmx;
        if(!sknum&&rmx==-1){rmx=0;
         device->getVideoDriver()->removeTexture(m->getMaterial(0).getTexture(1));
         m->setMaterialTexture(1,0);Sktx[1]=0;}
        if(sprde->isPressed()){sknum=0;rmx=-1;
         E_ANIMATED_MESH_TYPE mtp=m->getMesh()->getMeshType();
         if(mtp==(E_ANIMATED_MESH_TYPE)88){IM8MMesh*mes2=(IM8MMesh*)m->getMesh();
          mes2->setSkin(1,0);}
         if(mtp==(E_ANIMATED_MESH_TYPE)89){IMDL7Mesh*mes2=(IMDL7Mesh*)m->getMesh();
          mes2->setSkin(1,0);}
         if(mtp==(E_ANIMATED_MESH_TYPE)90){IMDL3Mesh*mes2=(IMDL3Mesh*)m->getMesh();
          mes2->setSkin(1,0);}
         if(mtp==(E_ANIMATED_MESH_TYPE)91){IASCIIMesh*mes2=(IASCIIMesh*)m->getMesh();
          mes2->setSkin(1,0);}
         }}

      if(spr){if(!m->getMaterial(0).getTexture(0)||!m->isVisible()){spr->setVisible(0);}
       bool svis=spr->isVisible();if(sprb0){sprb0->setVisible(svis);}
       if(sprb1){sprb1->setVisible(svis);}
       if(sprb2){sprb2->setVisible(svis);}if(sprb3){sprb3->setVisible(svis);}
       if(sprb4){sprb4->setVisible(svis);}if(sprtn){
        if(vwmx>sz0){sprtn->setVisible(svis);}else{sprtn->setVisible(0);}}
       if(sprs0){sprs0->setVisible(svis);}if(sprs1){sprs1->setVisible(svis);}
       if(sprad){sprad->setVisible(svis);}if(sprde){sprde->setVisible(svis);}
       if(sprcr){sprcr->setVisible(svis);}}
      //close
      if(sprb5){rxy=rect<s32>(sps[0]+vwmx-17,sps[1]-10,sps[0]+vwmx-2,sps[1]);
       sprb5->setVisible(1);sprb5->setRelativePosition(rxy);
       if(sprb5->isPressed()){t1=1;}if(!sprb5->isPressed()&&t1==1){t1=2;}
       if(t1==2){if(spr){spr->setVisible(!spr->isVisible());
         if(!spr->isVisible()){sprb5->setText(L"v");}else{sprb5->setText(L"x");}}t1=0;}}
      if(pos0>0){vwmx=sz0;}if(pos0<0){vwmx=sz1;}}
     else{if(spr){spr->setMaterialTexture(0,0);spr->setVisible(0);
       spr->setPosition(v3f(0,-200976.1f,0));
       sprb0->setVisible(0);sprb1->setVisible(0);
       sprb2->setVisible(0);sprb3->setVisible(0);sprb4->setVisible(0);
       sprtn->setVisible(0);sprs0->setVisible(0);sprs1->setVisible(0);
       sprad->setVisible(0);sprde->setVisible(0);sprcr->setVisible(0);
       if(sprb5){sprb5->setVisible(0);t1=1;}}}

     s32 scrnx=driver->getScreenSize().Width;
     s32 scrny=driver->getScreenSize().Height;
     f32 aspect=(f32)scrnx/(f32)scrny;
     guienv->drawAll();driver->endScene();
////////////////////////draw

/////////////////////////////////ted
     scene::IMeshCache*mc=device->getSceneManager()->getMeshCache();s16 nmsh=mc->getMeshCount();
     if(argc>=2){argc=1;edir();loadModel(argv[1]);}
     if(eKEY_I){IAnimatedMeshSceneNode*you;
      IAnimatedMesh*mesh=smgr->getMesh("../x.m8m");
      if(mesh&&!you){you=smgr->addAnimatedMeshSceneNode(mesh);}
      if(mesh&&you){you->setVisible(true);}}
     //////////////////
     s32 i=0;
     if(eKEY_O){edir();i=1;buildx();acube(smgr);}
     if(i||argc){argc=0;continue;}
     //////////////////
     if(m){static s32 dbv,dbv1;if(eKEY_SPACE){dbv=1;}
      if(!eKEY_SPACE&&dbv){dbv=0;dbv1=!dbv1;if(dbv1){m->setDebugDataVisible(EDS_BBOX);}
       else{m->setDebugDataVisible(EDS_OFF);}}}
     if(eKEY_CTRL&&eKEY_S){sw=1;}
/////////////////////////////////ted
     if(m&&eKEY_SHIFT&&eKEY_Z){framesed->setText(L"stand");Anim(m,"stand");}
     if(m&&eKEY_SHIFT&&eKEY_X){framesed->setText(L"run");Anim(m,"run");}
     if(m&&eKEY_SHIFT&&eKEY_C){framesed->setText(L"attack");Anim(m,"attack");}
     if(m&&eKEY_SHIFT&&eKEY_V){framesed->setText(L"death");Anim(m,"death");}
     framesed->setEnabled(!frameseb->isPressed());
     if(m&&(eKEY_ENTER||frameseb->isPressed())){
      stringc strx(framesed->getText());strx.make_lower();char cc[20];s32 o0=0;
      while(o0<20){if(o0<strx.size()){cc[o0]=strx.c_str()[o0];}else{cc[o0]=0;}o0++;}
      Anim(m,cc);stringw str(cc);framesed->setText(str.c_str());}
/////////////////////////////////ted2

     stringw str,lfnm;s32 rvt=0;
//frames
     frames=0;frame=1;stringc strx=" ";c8*cn;
     if(m){
      frames=m->getMesh()->getFrameCount();
      frame=(s32)m->getFrameNr();s32 o0=0,o1=0,s=0,e=0,f=0;c8*co;
      if(m->getMesh()->getMeshType()==(E_ANIMATED_MESH_TYPE)88){
       IM8MMesh*mes2=(IM8MMesh*)m->getMesh();rvt=mes2->getnvt();
       while(e<frame+1&&o0<200){cn=(c8*)mes2->getAnimationName(o0);
        mes2->getFrameLoop(cn,s,e,f);co=(c8*)mes2->getAnimationName(o0-1);o0++;}}
      if(m->getMesh()->getMeshType()==EAMT_MD2){
       IAnimatedMeshMD2*mes2=(IAnimatedMeshMD2*)m->getMesh();
       while(e<frame+1&&o0<200){cn=(c8*)mes2->getAnimationName(o0);
        mes2->getFrameLoop(cn,s,e,f);co=(c8*)mes2->getAnimationName(o0-1);o0++;}}
      if(m->getMesh()->getMeshType()==(E_ANIMATED_MESH_TYPE)89){
       IMDL7Mesh*mes2=(IMDL7Mesh*)m->getMesh();rvt=mes2->getnvt();
       while(e<frame+1&&o0<200){cn=(c8*)mes2->getAnimationName(o0);
        mes2->getFrameLoop(cn,s,e,f);co=(c8*)mes2->getAnimationName(o0-1);o0++;}}
      if(m->getMesh()->getMeshType()==(E_ANIMATED_MESH_TYPE)90){
       IMDL3Mesh*mes2=(IMDL3Mesh*)m->getMesh();rvt=mes2->getnvt();
       while(e<frame+1&&o0<200){cn=(c8*)mes2->getAnimationName(o0);
        mes2->getFrameLoop(cn,s,e,f);co=(c8*)mes2->getAnimationName(o0-1);o0++;}}
      if(m->getMesh()->getMeshType()==(E_ANIMATED_MESH_TYPE)91){
       IASCIIMesh*mes2=(IASCIIMesh*)m->getMesh();rvt=mes2->getnvt();
       while(e<frame+1&&o0<200){cn=(c8*)mes2->getAnimationName(o0);
        mes2->getFrameLoop(cn,s,e,f);co=(c8*)mes2->getAnimationName(o0-1);o0++;}}
      if(m->getMesh()->getMeshType()==EAMT_SKINNED){
       ISkinnedMesh*mes2=(ISkinnedMesh*)m->getMesh();
       while(e<frame+1&&o0<200){cn=(c8*)mes2->getJointName(o0);
        co=(c8*)mes2->getJointName(o0-1);o0++;}}
      strx=cn;if((frame-s)/4>0){strx+=(frame-s)/4;}if((frame-s)/4<0){cn=co;strx=cn;}
      strx=strx.subString(0,10);
     }
     framesbar->setMax(frames/4-1);framesbar->setLargeStep(2);framesbar->setSmallStep(1);
     if(framesbar->getPos()!=frame/4&&random(80)>75){framesbar->setPos(frame/4);}
     if(framesed->isEnabled()&&framesed->getTabOrder()!=frame){framesed->setTabOrder(frame);
      str=cn;str=str.subString(0,10);framesed->setText(str.c_str());}
     str="";str+=frame/4;ccc0c->setText(str.c_str());
     str="";str+=frames/4;ccc0t->setText(str.c_str());
     str=strx.c_str();ccc0n->setText(str.c_str());
//frames
//flname
     str=L" unknown";
     if(m&&mc){str=mc->getMeshFilename(m->getMesh());
      str.make_lower();s32 s0=str.findLast(47)+1;s0=s0?s0:str.findLast(92)+1;
      str=str.subString(s0,(str.size()-s0));lfnm=str;}
     flntext->setText(str.c_str());
//flname
//type
     core::stringc strtype=L"Type: NONE";
     submenu=menu->getSubMenu(0)->getSubMenu(3);
     submenu->setItemEnabled(0,false);submenu->setItemEnabled(1,false);
     submenu->setItemEnabled(2,false);submenu->setItemEnabled(3,false);
     submenu=menu->getSubMenu(0)->getSubMenu(4);
     submenu->setItemEnabled(0,false);submenu->setItemEnabled(1,false);
     submenu->setItemEnabled(2,false);
     E_ANIMATED_MESH_TYPE mtp=(E_ANIMATED_MESH_TYPE)-1;
     if(m){mtp=m->getMesh()->getMeshType();strtype=L"Type: UNKNOWN";
      submenu=menu->getSubMenu(0)->getSubMenu(4);
      submenu->setItemEnabled(0,true);submenu->setItemEnabled(1,true);
      submenu->setItemEnabled(2,true);
      if(mtp==(E_ANIMATED_MESH_TYPE)88){
       if(((IM8MMesh*)m->getMesh())->getFileType()==1){strtype=L"Type: M8M";}
       if(((IM8MMesh*)m->getMesh())->getFileType()==2){strtype=L"Type: MDL5";}
       if(((IM8MMesh*)m->getMesh())->getFileType()==3){strtype=L"Type: MDL4";}
       submenu=menu->getSubMenu(0)->getSubMenu(3);
       submenu->setItemEnabled(0,true);submenu->setItemEnabled(1,true);
       submenu->setItemEnabled(2,true);submenu->setItemEnabled(3,true);
      }
      if(mtp==EAMT_MD2){strtype=L"Type: MD2";
       submenu=menu->getSubMenu(0)->getSubMenu(3);
       submenu->setItemEnabled(0,true);submenu->setItemEnabled(1,true);
       submenu->setItemEnabled(2,true);submenu->setItemEnabled(3,true);
      }
      if(mtp==(E_ANIMATED_MESH_TYPE)89){strtype=L"Type: MDL7";
       submenu=menu->getSubMenu(0)->getSubMenu(3);
       submenu->setItemEnabled(0,true);submenu->setItemEnabled(1,true);
      }
      if(mtp==(E_ANIMATED_MESH_TYPE)90){
       if(((IMDL3Mesh*)m->getMesh())->getFileType()==1){strtype=L"Type: MDL3";}
       if(((IMDL3Mesh*)m->getMesh())->getFileType()==2){strtype=L"Type: MDL2";}
       if(((IMDL3Mesh*)m->getMesh())->getFileType()==3){strtype=L"Type: IDP2";}
       if(((IMDL3Mesh*)m->getMesh())->getFileType()==4){strtype=L"Type: IDPO";}
       submenu=menu->getSubMenu(0)->getSubMenu(3);
       submenu->setItemEnabled(0,true);submenu->setItemEnabled(1,true);
      }
      if(mtp==(E_ANIMATED_MESH_TYPE)91){strtype=L"Type: ASCII";
       submenu=menu->getSubMenu(0)->getSubMenu(3);
       submenu->setItemEnabled(0,true);submenu->setItemEnabled(1,true);
       submenu->setItemEnabled(2,true);submenu->setItemEnabled(3,true);
      }
      if(mtp==EAMT_OBJ){strtype=L"Type: OBJ";}
      if(mtp==EAMT_MY3D){strtype=L"Type: MY3D";}
      if(mtp==EAMT_3DS){strtype=L"Type: 3DS";}
      if(mtp==EAMT_MD3){strtype=L"Type: MD3";}
      if(mtp==EAMT_BSP){strtype=L"Type: BSP";}
      if(mtp==EAMT_CSM){strtype=L"Type: CSM";}
      if(mtp==EAMT_LMTS){strtype=L"Type: LMTS";}
      if(mtp==EAMT_OCT){strtype=L"Type: OCT";}
     }str=strtype.c_str();static s32 nm;
     str+=L" | ntr: ";if(!m){str+=0;nm=0;}
     if(m){u32 pol=smgr->getMeshManipulator()->getPolyCount(m->getMesh());str+=pol;
      if(eKEY_N){smgr->getMeshManipulator()->setVertexColors(m->getMesh(),
       video::SColor(255,255,120,1));
       smgr->getMeshManipulator()->recalculateNormals(m->getMesh(),true);}}
     str+=L" | nvt: ";
     if(m&&strtype!="Type: UNKNOWN"&&mtp!=(E_ANIMATED_MESH_TYPE)-1&&mtp!=EAMT_MD3){
      s32 o0=m->getMesh()->getMeshBuffer(0)->getVertexCount();str+=o0;}else{str+=0;}
     if(rvt>0){str+=L"/";str+=rvt;}typtext->setText(str.c_str());
//type
//cam
     if(m){//camera
      ebox=m->getBoundingBox();
      v0.X=ebox.MaxEdge.X;v0.Z=ebox.MaxEdge.Z;v0.Y=ebox.MaxEdge.Y;
      v1.X=ebox.MinEdge.X;v1.Z=ebox.MinEdge.Z;v1.Y=ebox.MinEdge.Y;
      v0.X=v0.X<5?5:v0.X;v0.Y=v0.Y<5?5:v0.Y;v0.Z=v0.Z<5?5:v0.Z;
      v1.X=v1.X>-5?-5:v1.X;v1.Y=v1.Y>-5?-5:v1.Y;v1.Z=v1.Z>-5?-5:v1.Z;
      static s32 posm[3],poss[2],offstg[3];static f32 zoom;static stringc sfnm;
      v3f vt=v3f(m->getPosition().X+offstg[0],m->getPosition().Y+offstg[1],
       m->getPosition().Z+offstg[2]);if(sfnm!=stringc(lfnm.c_str())){sfnm=lfnm.c_str();nm=0;}
      if(eKEY_M||eKEY_SPACE||nm<50){nm++;if(eMOUSE_LEFT||eMOUSE_RIGHT){nm=50;}zoom=0;
       if(nm==1){printf("\n Size %f",v0.Y-v1.Y);}
       if(Camera[0]){vx=Camera[0]->getPosition();offstg[0]=0;offstg[1]=0;offstg[2]=0;
        m->setPosition(v3f(0,0,0));f32 dis=vec_dis(v3f(0,0,0),vx);
        if(eKEY_M){printf("\n Dist %d",(s32)dis);}
        f32 o0=max(v0.Y-v1.Y,max(v0.X-v1.X,v0.Z-v1.Z)*0.8f)*2.5f;
        dis=max(o0/(dis+!dis),0.5f);o0=max(20-o0,0)/62;if(dis<0.98f-o0||dis>1.02f+o0){
         vx=v3f(vx.X*dis,vx.Y*dis,vx.Z*dis);}
        Camera[0]->setPosition(vx);Camera[0]->setTarget(m->getPosition());}}
      if(eMOUSE_LEFT&&!eMOUSE_RIGHT&&!msr&&eMOUSE_Y<scrny-30&&eMOUSE_Y>50||
       ((eKEY_UP||eKEY_DOWN||eKEY_LEFT||eKEY_RIGHT)&&!eKEY_CTRL&&!eKEY_SHIFT&&!msr)){
       static s32 k[2];k[1]+=2*(eKEY_DOWN-eKEY_UP);k[0]+=3*(eKEY_RIGHT-eKEY_LEFT);
       if(Camera[0]){if(!posm[0]||!posm[1]){k[0]=0;k[1]=0;posm[0]=eMOUSE_X;posm[1]=eMOUSE_Y;}
        vx=Camera[0]->getPosition();zoom=max(min(zoom+!zoom,8),0.2f);
        vx-=vt;f32 dis=vec_dis(v3f(0,0,0),vx);
        f32 o0=max(v0.Y-v1.Y,max(v0.X-v1.X,v0.Z-v1.Z)*0.8f)*2.5f*zoom;
        dis=max(o0/(dis+!dis),0.5f);o0=max(20-o0,0)/62;if(dis<0.98f-o0||dis>1.02f+o0){
         vx=v3f(vx.X*dis,vx.Y*dis,vx.Z*dis);}vx+=vt;
        s32 posx=((posm[0]-eMOUSE_X+k[0])/2)-poss[0];poss[0]=((posm[0]-eMOUSE_X+k[0])/2);
        s32 posy=((posm[1]-eMOUSE_Y+k[1])/2)-poss[1];poss[1]=((posm[1]-eMOUSE_Y+k[1])/2);
        if(posx||posy){vx-=vt;vec_rot(vx,v3f(0,posx,0));
         f32 ang=0;vec_ang(ang,vx);s32 o=abs((s32)ang);
         vec_rot(vx,v3f((o<90?1:-1)*posy,0,(ang<0?-1:1)*posy));vx+=vt;}
        Camera[0]->setPosition(vx);Camera[0]->setTarget(vt);}}
      else{if(!eMOUSE_RIGHT&&!eMOUSE_LEFT){posm[0]=0;posm[1]=0;poss[0]=0;poss[1]=0;}}
      if(eMOUSE_RIGHT&&!eMOUSE_LEFT&&(!msr||msr==2)&&eMOUSE_Y<scrny-30&&eMOUSE_Y>50){msr=2;
       if(Camera[0]){if(!posm[3]){posm[3]=eMOUSE_X;}
        vx=Camera[0]->getPosition();zoom=max(min(1+f32(posm[3]-eMOUSE_X)/50.0f,8),0.2f);
        vx-=vt;f32 dis=vec_dis(v3f(0,0,0),vx);
        f32 o0=max(v0.Y-v1.Y,max(v0.X-v1.X,v0.Z-v1.Z)*0.8f)*2.5f*zoom;
        dis=max(o0/(dis+!dis),0.5f);o0=max(20-o0,0)/62;if(dis<0.98f-o0||dis>1.02f+o0){
         vx=v3f(vx.X*dis,vx.Y*dis,vx.Z*dis);}vx+=vt;
        Camera[0]->setPosition(vx);Camera[0]->setTarget(vt);}}
      else{msr*=msr==1;posm[3]=0;}
      }
//cam
//set pos
     sbar->setRelativePosition(rect<s32>(0,scrny-16,scrnx,scrny));
     ccc0c->setRelativePosition(rect<s32>(500,scrny-15,530,scrny));
     ccc0t->setRelativePosition(rect<s32>(530,scrny-15,560,scrny));
     ccc0n->setRelativePosition(rect<s32>(560,scrny-15,610,scrny));
     framesbar->setRelativePosition(rect<s32>(145,scrny-15,350,scrny));
     framesed->setRelativePosition(rect<s32>(51,scrny-16,144,scrny-1));
     frameseb->setRelativePosition(rect<s32>(35,scrny-15,50,scrny));
//set pos
     str=L"M: ";
     str+=nmsh;
     str+=L" | Tri: ";
     str.append(core::stringw(driver->getPrimitiveCountDrawn()));
     str+=L" | FPS: ";
     str.append(core::stringw(driver->getFPS()));
     fpstext->setText(str.c_str());
    }else{device->yield();}
   if(rvm){if(rvm==Model){Model=0;}
    scene::IMeshCache*mc=device->getSceneManager()->getMeshCache();
    rvm->remove();rvm=0;mc->clearUnusedMeshes();}

   }printf("\nTest:%d",sw);
   if(sw!=0){device->closeDevice();_m8mexp_();}
   return 1;
}
