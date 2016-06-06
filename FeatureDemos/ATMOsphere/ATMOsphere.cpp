
/*
Autor: Jonas Abramavicius aka Pazystamo
Unfinished version
2006 06 01
*/

#include <irrlicht.h>
#include <IVideoDriver.h>
#include <math.h>
#include <ITimer.h>

using namespace irr;
/*
Autor: Jonas Abramavicius aka Pazystamo
Unfinished version
2006 06 01
*/

#include <irrlicht.h>
#include <IVideoDriver.h>
#include <math.h>
#include <ITimer.h>

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;

class CATMOskySceneNode : public scene::ISceneNode
{
   core::aabbox3d<f32> Box;
   video::S3DVertex *Vertices;
   video::SMaterial Material;
    u16 *indices;
    s32 vert;//number of vertices
    s32 face;//number of faces
    f32 posX,sizeX;
    f32 uvX;
public:

   CATMOskySceneNode(video::ITexture* tex,scene::ISceneNode* parent, scene::ISceneManager* mgr,s32 faces, s32 id)
      : scene::ISceneNode(parent, mgr, id)
   {
        //AutomaticCullingEnabled = false;
        video::SMaterial mat;
        mat.Lighting = false;
        //mat.Wireframe = true;
        mat.ZBuffer = false;
        //mat.ZWriteEnable = false;
        mat.setFlag(EMF_BILINEAR_FILTER,true);
        //mat.NormalizeNormals=true;
        //mat.AnisotropicFilter=true;
        //mat.GouraudShading=false;
        //mat.TrilinearFilter = true;
        //mat.BackfaceCulling = false;
        face=faces;
        Vertices = new video::S3DVertex[face+1];
        indices = new u16[face*3];
        Material = mat;
        Material.setTexture(0,tex);
        f64 angle = 0.0f;                //start positions
        f64 angle2 = 360.0f/face;        //angle to add
        vert=0;                          //vertice nr
        s32 nr = -3;                     //indices nr
        //top vertice
        Vertices[0]=video::S3DVertex(0.0f, 100.0f, -0.0f,
         0.0568988f, 0.688538f, -0.722965f,
          video::SColor(255,255,255,255), 0.0f, 0.1f);
        for (u16 n=1;n<face+1;n++){
            vert=vert+1;
            nr=nr+3;                        //indices number
            f64 x=cos(angle*0.017453292519943295769236907684886f)*100;//vertice x coord
            f64 z=sin(angle*0.017453292519943295769236907684886f)*100;//vertice z coord
            Vertices[vert]=video::S3DVertex(x, -5.0f, z,
               0.0568988f, 0.688538f, -0.722965f,
               video::SColor(255,255,255,255), 0.0f, 0.9f);
            angle=angle+angle2;
            //connects face
            indices[nr]=0;                  //top vertice
            indices[nr+1]=vert;             //bottom vertice
            indices[nr+2]=vert+1;           //next bottom vertice
        }
        indices[nr+2]=1;                 //connect last bottom vertice with first
   }

   virtual void OnRegisterSceneNode()
   {
      if (IsVisible)
         SceneManager->registerNodeForRendering(this,ESNRP_SKY_BOX);
      ISceneNode::OnRegisterSceneNode();
   }

   virtual void render()
   {
      video::IVideoDriver* driver = SceneManager->getVideoDriver();
        scene::ICameraSceneNode* camera = SceneManager->getActiveCamera();
       if (!camera || !driver)
         return;
       core::matrix4 mat;
       mat.setTranslation(camera->getAbsolutePosition());
       //attach node to camera
       driver->setTransform(video::ETS_WORLD, mat);
       //don't attach camera
       //driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
       driver->setMaterial(Material);
       //update uv maping
        for (int i=1;i<face+1;i++){
            Vertices[i].TCoords=core::vector2d< f32 >(uvX,0.98f);
        }
        Vertices[0].TCoords=core::vector2d< f32 >(uvX,0.01f);
        driver->drawIndexedTriangleList(&Vertices[0],vert+1,&indices[0],face);
   }

    virtual const core::aabbox3d<f32>& getBoundingBox() const
   {
      return Box;
   }

   virtual u32 getMaterialCount() const
   {
      return 1;
   }

   virtual video::SMaterial& getMaterial(u32 i)
   {
      return Material;
   }

   //change sky texture
   virtual SMaterial& setMaterial(ITexture* tex)
   {
      Material.setTexture(0, tex);
      return Material;
   }

   //update uv maping x coordinate
   void setuvX(f64 v)
    {
      uvX=v;
    }
};
//******************************************************************************
//almost all code from CBillboardSceneNode
class CATMOstarSceneNode : public scene::ISceneNode
{
   core::aabbox3d<f32> BBox;
   core::dimension2d<f32> Size;
   video::S3DVertex Vertices[4];
   video::SMaterial Material;
   u16 indices[6];
   video::S3DVertex vertices[4];
public:

   CATMOstarSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr,
    s32 id,vector3df position,const core::dimension2d<f32>& size)
      : scene::ISceneNode(parent, mgr, id,position)
   {
        Material.ZBuffer = false;

   setSize(size);

//   AutomaticCullingEnabled = false;

   indices[0] = 0;
   indices[1] = 2;
   indices[2] = 1;
   indices[3] = 0;
   indices[4] = 3;
   indices[5] = 2;

   vertices[0].TCoords.set(1.0f, 1.0f);
   vertices[0].Color = 0xffffffff;

   vertices[1].TCoords.set(1.0f, 0.0f);
   vertices[1].Color = 0xffffffff;

   vertices[2].TCoords.set(0.0f, 0.0f);
   vertices[2].Color = 0xffffffff;

   vertices[3].TCoords.set(0.0f, 1.0f);
   vertices[3].Color = 0xffffffff;

   }
//virtual void setSize(const core::dimension2d<f32>& size);
   virtual void OnRegisterSceneNode()
   {
      if (IsVisible)
         SceneManager->registerNodeForRendering(this,ESNRP_SKY_BOX);
        ISceneNode::updateAbsolutePosition();//realy helps from sun pos lag
      ISceneNode::OnRegisterSceneNode();
   }

   virtual void render()
   {
   video::IVideoDriver* driver = SceneManager->getVideoDriver();
   ICameraSceneNode* camera = SceneManager->getActiveCamera();

   if (!camera || !driver)
      return;

   // make billboard look to camera
   core::vector3df pos = getAbsolutePosition();
   core::vector3df campos = camera->getAbsolutePosition();
   core::vector3df target = camera->getTarget();
   core::vector3df up = camera->getUpVector();
   core::vector3df view = target - campos;
   view.normalize();

   core::vector3df horizontal = up.crossProduct(view);
   horizontal.normalize();

   core::vector3df vertical = horizontal.crossProduct(view);
   vertical.normalize();

   horizontal *= 0.5f * Size.Width;
   vertical *= 0.5f * Size.Height;

   vertices[0].Pos = pos + horizontal + vertical;
   vertices[1].Pos = pos + horizontal - vertical;
   vertices[2].Pos = pos - horizontal - vertical;
   vertices[3].Pos = pos - horizontal + vertical;

   view *= -1.0f;

   for (s32 i=0; i<4; ++i)
      vertices[i].Normal = view;

   // draw

   if (DebugDataVisible)
   {
      driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
      video::SMaterial m;
      m.Lighting = false;
      driver->setMaterial(m);
      driver->draw3DBox(BBox, video::SColor(0,208,195,152));
   }

   core::matrix4 mat;
    driver->setTransform(video::ETS_WORLD, mat);

   driver->setMaterial(Material);

   driver->drawIndexedTriangleList(vertices, 4, indices, 2);
   }

    virtual const core::aabbox3d<f32>& getBoundingBox() const
   {
      return BBox;
   }
//! sets the size of the billboard
void setSize(const core::dimension2d<f32>& size)
{
   Size = size;

   if (Size.Width == 0.0f)
      Size.Width = 1.0f;

   if (Size.Height == 0.0f )
      Size.Height = 1.0f;

   f32 avg = (size.Width + size.Height)/6;
   BBox.MinEdge.set(-avg,-avg,-avg);
   BBox.MaxEdge.set(avg,avg,avg);
}
   virtual u32 getMaterialCount() const
   {
      return 1;
   }

   virtual video::SMaterial& getMaterial(u32 i)
   {
      return Material;
   }

   //change sky texture
   virtual video::SMaterial& setMaterial(video::ITexture* tex)
   {
        Material.setTexture(0,tex);
        return Material;
   }

   //update uv maping x coordinate
const core::dimension2d<f32>& getSize()
{
   return Size;
}
};
//******************************************************************************
class ATMOsphere{

struct PIXEL
{
   u8  B,G,R,A;
};
CATMOstarSceneNode* bill;
  u32 kin,ilgis;
  u32 i;
//u8 palete[256][3];
//u8 prspalva[3];
//u8 pbspalva[3];
//u8 vspalva[3];
float step[3];
float pspalva[3];
CATMOskySceneNode *Sky;
CATMOstarSceneNode *Sun;
scene::ILightSceneNode *sunlight;
video::IImage* image;
s32 skyid;
video::ITexture* dangus;
IrrlichtDevice* device;
IVideoDriver* driver;
scene::ISceneManager* smgr;
double rad;//radian
u32 tex_size;//texture size
u32 half_tex;//half texture size
//--Timer----
ITimer *Atimer;//ATMOsphere timer
f32 currentTime, startTime, dTime;
//how many virtual day per real day. 1440 = day per 1 min.
//24*60=1440/dayspeed=1day-1min
f64 dayspeed; // how long goes day in minutes

u32 ptime,gtime;
f64 time_int_step;//sun place interpolation position (0-1)
f64 counter_time;//loop variable,count time from interpolation start in J
f64 J1minute;
//--Sun position---
double J;//julias date
double sun_angle[2];//sun pos in angles
f32 sun_interpolation_speed;//how minutes in virtual time takes interpolate sun from start to end position
double J1;
vector3df sun_pos_from,sun_pos_to;//interpolations points
f64 sun_angle_from,sun_angle_to;//sun height angle for interpolation
//--Sky texture----
//setSkyImage
video::IImage* skyimage;//sky texture
PIXEL *pixels;//palete
f32 uvX;
//---Date conversions
u16 Ndate[5];//stores date converted from Julian calendar
//-------------
public:
 ATMOsphere(){
              //default values
 kin=0;
 ilgis=0;
 uvX=0.0f;
 i=0;
 rad=0.017453292519943295769236907684886f;//degree to radian (PI/180);
/* prspalva[0]=0;//tamsi
 prspalva[1]=128;
 prspalva[2]=255;
 pbspalva[0]=128;//sviesi
 pbspalva[1]=255;
 pbspalva[2]=255;
 vspalva[0]=113;//vidurys
 vspalva[1]=184;
 vspalva[2]=255;*/
 J=DateToJulian(2007,6,29,7,50);//start time
  dayspeed=60.0f;
  time_int_step=0.0f;//start sun pos interpolation
  sun_interpolation_speed=30.0f;//make sun pos interpolation every 30 virtual min
  J1minute=1.0f/1440.0f;//one minute in Julian time
 }

 //###rounds angle to fit 360 degrees
 f32 round360(f32 angle){
  if (angle>360){
   while (angle>360){
    angle-=360;
   }
  }
  return angle;
 }

 vector3df getInterpolated3df(vector3df from,vector3df to, f32 d)
 {
 f32 inv = 1.0f - d;
 vector3df rez;
 rez.X=from.X *inv + to.X*d;
 rez.Y=from.Y*inv + to.Y*d;
 rez.Z=from.Z*inv + to.Z*d;
  return  rez;
 }
 //prepare sun position interpolation (find start and end positions)
 void prep_interpolation(f64 Jdate, f64 time)//time-time from 1st sun pos to 2nd
 {
  core::matrix4 mat;
core::vector3df kampas;
saule(52.0f,-5.0f,Jdate);//52.0 -5.0 kaunas 54.54 -23.54
kampas.X=-sun_angle[1];//heigh
kampas.Y=sun_angle[0];//0.0f;-
kampas.Z=0.0f;
mat.setRotationDegrees(kampas);
f32 vieta[4];
vieta[0]=0.0f;
vieta[1]=0.0f;
vieta[2]=1000.0f;
vieta[3]=0.0f;
mat.multiplyWith1x4Matrix(vieta);
  sun_pos_from.X=vieta[0];
  sun_pos_from.Y=vieta[1];
  sun_pos_from.Z=vieta[2];
  sun_angle_from=sun_angle[1];
  saule(52.0f,-5.0f,Jdate+time);//52.0 -5.0 kaunas 54.54 -23.54
  kampas.X=-sun_angle[1];//heigh
kampas.Y=sun_angle[0];//0.0f;-
kampas.Z=0.0f;
core::matrix4 mat2;
mat2.setRotationDegrees(kampas);
vieta[0]=0.0f;
vieta[1]=0.0f;
vieta[2]=1000.0f;
vieta[3]=0.0f;
sun_angle_to=sun_angle[1];
mat2.multiplyWith1x4Matrix(vieta);
  sun_pos_to.X=vieta[0];
  sun_pos_to.Y=vieta[1];
  sun_pos_to.Z=vieta[2];
 }
//calculate sun position
 void saule(f64 pl,f64 lw,f64 J){
  //lw - longitude
  //pl - latitude
  //double J=2453097;
  f64 M = 357.5291f + 0.98560028*(J - 2451545);//degree
  M=round360(M);//degree
  f64 Mrad=M*rad;//radian
  f64 C = 1.9148f* sin(Mrad) + 0.02f* sin(2*Mrad) + 0.0003f*sin(3* Mrad);//degree
  //printf("C %3.4f\n",C);
  C=round360(C);//degree
  //f64 Crad=C*rad;//radian
  f64 lemda = M + 102.9372f + C + 180.0f;//degree
  lemda=round360(lemda);//degree
  f64 lemdarad=lemda*rad;//radian
  f64 alfa =lemda - 2.468f *sin(2* lemdarad) + 0.053f* sin(4* lemdarad)-0.0014f *sin(6 *lemdarad);//degree
  alfa=round360(alfa);//degree
  f64 sigma=22.8008f* sin(lemdarad) + 0.5999f* sin(lemdarad)*sin(lemdarad)*sin(lemdarad)
  + 0.0493f* sin(lemdarad)*sin(lemdarad)*sin(lemdarad)*sin(lemdarad)*sin(lemdarad);//degree
  sigma=round360(sigma);//degree
  f64 sigmarad=sigma*rad;//radian
  f64 zv=280.16f+360.9856235f*(J-2451545.0f)-lw;//degree
  zv=round360(zv);//degree
  f64 H = zv - alfa;//degree
  H=round360(H);//degree
  f64 Hrad=H*rad;//radian
  f64 A = atan2(sin(Hrad), cos(Hrad)* sin(pl*rad) - tan(sigmarad)*cos(pl*rad))/rad;
  f64 h = asin(sin(pl*rad)*sin(sigmarad) + cos(pl*rad)*cos(sigmarad)*cos(Hrad))/rad;
  //A from 0..180,-180..0
  //printf("M %3.4f C %3.4f lemda %3.4f alfa %3.4f sigma %3.4f\n",M,C,lemda,alfa,sigma);
  //printf("zv %3.4f H %3.4f A %3.4f h %3.15f\n",zv,H,A,h);
  sun_angle[0]=A;
  sun_angle[1]=h;//height
 }


 void setSkyImage(const char *filename){
  skyimage=driver->createImageFromFile(filename);
 }

void CreateSkyPallete(){//Psize-paletes dydis
if (dangus!=NULL){driver->removeTexture(dangus);}

dangus=driver->getTexture("sky2.tga");

//stars box
smgr->addSkyBoxSceneNode(
      driver->getTexture("stars.bmp"),
      driver->getTexture("stars.bmp"),
      driver->getTexture("stars.bmp"),
      driver->getTexture("stars.bmp"),
      driver->getTexture("stars.bmp"),
      driver->getTexture("stars.bmp"));

Sky = new CATMOskySceneNode(dangus,smgr->getRootSceneNode(), smgr,80, skyid);
//sun billboard
    bill=new CATMOstarSceneNode(smgr->getRootSceneNode(),smgr,0, core::vector3df(0,0,0),core::dimension2d<f32>(150,150));
    bill->setMaterialFlag(video::EMF_LIGHTING, false);
    bill->setMaterialTexture(0, driver->getTexture("sun.tga"));
    bill->getMaterial(0).MaterialTypeParam = 0.01f;
    bill->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);

Sky->getMaterial(0).MaterialTypeParam = 0.01f;
Sky->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);

sunlight = smgr->addLightSceneNode(bill, core::vector3df(0,100,0),
video::SColorf(1.0f, 0.6f, 0.7f, 1.0f), 10000.0f);

 }
 //###Starts ATMOsphere and prepares for work
 void start(IrrlichtDevice* device2,video::IVideoDriver* Driver,scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id)
 {
  skyid=id;
  device=device2;
  driver=Driver;
  smgr=mgr;
  setSkyImage("sky2.tga");
  CreateSkyPallete();
  startTimer();
  setAmbientLight2(SColor(255,255,255,255));//bug fix
 }

 //###starts timer
 void startTimer(){
 // Atimer=new ITimer();
 Atimer=device->getTimer();
 // Atimer->start();
  //Atimer->setTime(0);
  currentTime=Atimer->getRealTime();
  startTime=Atimer->getRealTime();
  dTime=0.0f;
  J1=J;//force update sun first time
  ptime=Atimer->getRealTime();
  gtime=Atimer->getRealTime();
  JulianToDate(J);
 }

 //###Calculates delta time (time from last frame) for timer
 void updateTimer(){
  currentTime =Atimer->getRealTime();
  dTime=currentTime-startTime;
 }

 //###returns sun rotation about y axis
 f64 getSunXAngle(){
  return sun_angle[0]*rad;//angle in radians
 }

 //###returns sun rotation about x axis (sun height above horizont)
 f64 getSunYAngle(){
  return sun_angle[1]*rad;// angle in radians
 }

 void setDaysPerDay(f64 days){
  dayspeed=days;
 }

 f64 getDayspeed(){
 return dayspeed;
 }

 void update(video::IVideoDriver* driver)
 {

   updateTimer();
   SColor sp;
   J=J+(((double)dayspeed/86400)/1000.0f)*dTime;

   //if interpolation is finished then start again
   if(time_int_step==0.0f){//calculate sun interpolation positions
    prep_interpolation(J,sun_interpolation_speed*J1minute);
   JulianToDate(J);
   counter_time=0.0f;
   }//1440
   //printf("%8.4f %4.8f\n",J,time_int_step);
   //---move sun billboard to sun place
   counter_time+=J-J1;//1440
   time_int_step=counter_time/(sun_interpolation_speed*J1minute);//(1.0f/(sun_interpolation_speed*(1.0f/1440.0f)))*dTime;
   vector3df sun_place=getInterpolated3df(sun_pos_from,sun_pos_to, time_int_step);
   J1=J;
   ICameraSceneNode *cam=smgr->getActiveCamera();
   core::vector3df cameraPos = cam->getAbsolutePosition();
   core::vector3df vt;//billboard position
   vt.X=sun_place.X+cameraPos.X;
   vt.Y=sun_place.Y+cameraPos.Y;
   vt.Z=sun_place.Z+cameraPos.Z;
   bill->setPosition(vt);
  // sunlight->setPosition(vt);
   //---sun movement end
   f32 inv = 1.0f - time_int_step;
   uvX=((sun_angle_from *inv + sun_angle_to*time_int_step)+90.0f)/180;
   if(time_int_step>=1.0f){time_int_step=0.0f;}
   sp=skyimage->getPixel((int)round(128*uvX),123);
   //driver->setAmbientLight(SColor(255,sp.getRed(),sp.getGreen(),sp.getBlue()));
   //printf("vt %3.4f",getSunYAngle());
   if (getSunYAngle()<0.0042){//isjungti lenpa kai naktis
      sunlight->setVisible(false);
      setAmbientLight2(SColor(255,sp.getRed(),sp.getGreen(),sp.getBlue()));
     }
   else{sunlight->setVisible(true);
        setAmbientLight2(SColor(255,sp.getRed(),sp.getGreen(),sp.getBlue()));//bug fix
   }
  // smgr->setShadowColor(SColor(50,sp.getRed(),sp.getGreen(),sp.getBlue()));
   //sunlight->getLightData().DiffuseColor=SColor(255,sp.getRed(),sp.getGreen(),sp.getBlue());
   Sky->setuvX(uvX);

  startTime = currentTime;
 }

wchar_t getTextDate(){
  JulianToDate(J);
  return 1;
 }

 //###Converts normal date tu Julian calendar date
 f64 DateToJulian(u16 y,u16 m,u16 d,u16 h,u16 min){
  //http://www.phys.uu.nl/~strous/AA/en//reken/juliaansedag.html
  f64 hh=h*60+min;             //to minutes
  f64 dd=d+(hh/1440.0f);
  printf("dd= %8.8f %8.8f\n",dd,hh);
  if (m<3){
   m=m+12;
   y=y-1;
  }
  f64 c=2-floor(y/100)+floor(y/400);
  f64 dt=floor(1461.0f*(y+4716.0f)/4)+floor(153*(m+1)/5)+dd+c-1524.5f;
  return dt;
 }

 //###Converts Julian calendar date to normal calendar date
 void JulianToDate(f64 x){
  //http://www.phys.uu.nl/~strous/AA/en//reken/juliaansedag.html
  f64 p = floor(x + 0.5);
  f64 s1 = p + 68569;
  f64 n = floor(4*s1/146097);
  f64 s2 = s1 - floor((146097*n + 3)/4);
  f64 i = floor(4000*(s2 + 1)/1461001);
  f64 s3 = s2 - floor(1461*i/4) + 31;
  f64 q = floor(80*s3/2447);
  f64 e = s3 - floor(2447*q/80);
  f64 s4 = floor(q/11);
  f64 m = q + 2 - 12*s4;
  f64 y = 100*(n - 49) + i + s4;
  f64 d = e + x - p + 0.5;
  double rr;
  f64 h = ((modf(d,&rr)*1440)/60);
  d=floor(d);
  f64 min=floor(modf(h,&rr)*60);
  h=floor(h);
  printf("update time:%4.0f %2.0f %2.0f %2.0f %2.0f\n",y,m,d,h,min);
  Ndate[0]=(u16)y;
  Ndate[1]=(u16)m;
  Ndate[2]=(u16)d;
  Ndate[3]=(u16)h;
  Ndate[4]=(u16)min;
 }
void setAmbientLight2(video::SColor color)
{
  io::IFileSystem* files=device->getFileSystem();
  io::IAttributes* a = files->createEmptyAttributes();

  // get the current attributes
  scene::ISceneNode* self = smgr->getRootSceneNode();

  self->serializeAttributes(a);

  // set the color attribute
  a->setAttribute("AmbientLight", color);

  self->deserializeAttributes(a);

  // destroy attributes
  a->drop();
}
};
