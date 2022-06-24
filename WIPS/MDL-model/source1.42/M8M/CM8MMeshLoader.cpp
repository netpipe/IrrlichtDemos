//============================================================================//
//
// date: 17.12.2008
//============================================================================//
//////////////////////////////////////////////////cpp
#include "CM8MMeshLoader.h"
#ifdef _IRR_COMPILE_WITH_M8M_MESH_LOADER_

#include "IReadFile.h"
#include "ISceneManager.h"

//////////////////////////////////////////////////cpp
namespace irr
{
namespace scene
{
//////////////////////////////////////////////////cpp
    const u32 M8M_MTRIS=21845;
    const u32 M8M_MVERTS=88888;
    const u16 M8M_FRAME2=2;
    const f32 M8M_FRAME2RPC=1.0f/(1<<M8M_FRAME2);
//////////////
    struct SM8MHeader
    {
        c8    ident[4];        //m8m
        s32   version;         //1
        f32   scale3[3];       //[3]  //SCALEMULTI
        f32   scalo3[3];       //[3]  //SCALEORIGIN

        s32   framsiz;         //526  //FRAMESIZE
        s32   oftexco;         //0
        s32   nullv1;          //0
        s32   nullv2;          //0

        s32   nskins;          //1    //NUMSKINS
        s32   skinwx;          //0
        s32   skinhx;          //0
        s32   nverts;          //86   //NUMVERTICES

        s32   ntris;           //164  //NUMTRIANGLES
        s32   nframs;          //6    //NUMFRAMES
        s32   ntskve;          //39   //NUMSKVERTICES
        s32   nullv3;          //0

        s32   nullv4;          //0
        s32   skbpt;           //88   //SKBPPTYPE
        s32   skinw;           //256  //SKWIDTH
        s32   skinh;           //256  //SKHEIGHT
    };
    struct SM8MVertex
    {
        u16   vtx[3];
    };
    struct SM8MFrame
    {
        c8    name[10];
        SM8MVertex vts[1];
    };
//////////////

//! constructor
IM8MMesh::IM8MMesh(scene::ISceneManager*smgr):SceneManager(smgr),IntpBfr(0),
    VertList(0),FData(0),nsk(0),nvt(0),ntr(0),nfr(0),nsv(0),skt(0),skw(0),skh(0){
    IntpBfr=new SMeshBuffer;MTCoords=0;MTriangs=0;skbm[0]=0;skbm[1]=0;}

//! destructor
IM8MMesh::~IM8MMesh()
{
    video::IVideoDriver*driver=SceneManager->getVideoDriver();
    delete[]VertList;VertList=0;delete[]MTCoords;MTCoords=0;
    delete[]MTriangs;MTriangs=0;delete skbm[0];skbm[0]=0;delete skbm[1];skbm[1]=0;
    driver->removeTexture(IntpBfr->Material.getTexture(1));
    driver->removeTexture(IntpBfr->Material.getTexture(0));if(IntpBfr){delete IntpBfr;}
}

//! returns the amount of frames in milliseconds. If the amount is 1, it is a static mesh.
u32 IM8MMesh::getFrameCount()const
{
    return nfr<<M8M_FRAME2;
}

IMesh*IM8MMesh::getMesh(s32 frame,s32 detailLevel,s32 startFrame,s32 endFrame)
{
    if((u32)frame>getFrameCount()){frame=(frame%getFrameCount());}

    if(startFrame==-1&&endFrame==-1){
        startFrame=0;
        endFrame=getFrameCount();
    }

    updateIntpBfr(frame,startFrame,endFrame);
    return this;
}

u32 IM8MMesh::getMeshBufferCount()const
{
    return 1;
}

IMeshBuffer*IM8MMesh::getMeshBuffer(u32 nr)const
{
    return IntpBfr;
}

//! Returns pointer to a mesh buffer which fits a material
IMeshBuffer*IM8MMesh::getMeshBuffer(const video::SMaterial &material)const
{
    if(IntpBfr->Material==material){getMeshBuffer(1);}return 0;
}

// updates the interpolation buffer
void IM8MMesh::updateIntpBfr(s32 frame,s32 startFrame,s32 endFrame)
{
    u32 firstFrame,secondFrame;
    f32 div;

    // TA: resolve missing ipol in loop between end-start

    if(endFrame-startFrame==0)
    {
        firstFrame=frame>>M8M_FRAME2;
        secondFrame=frame>>M8M_FRAME2;
        div=1.0f;
    }
    else
    {
        // key frames
        u32 s=startFrame>>M8M_FRAME2;
        u32 e=endFrame>>M8M_FRAME2;

        firstFrame=frame>>M8M_FRAME2;
        secondFrame=firstFrame+1>e?s:firstFrame+1;

        firstFrame=core::s32_min(nfr-1,firstFrame);
        secondFrame=core::s32_min(nfr-1,secondFrame);

        frame&=(1<<M8M_FRAME2)-1;
        div=frame*M8M_FRAME2RPC;
    }

    video::S3DVertex*target=static_cast<video::S3DVertex*>(IntpBfr->getVertices());
    v3f*fr1=VertList[firstFrame].pointer(),*fr2=VertList[secondFrame].pointer();

    v3f v1,v2;
    s32 t=0;while(t<ntr){
     s32 s=0;while(s<3){s32 o0=MTriangs[t].vertexIndices[s],o1=t*3+s;
      v1=fr1[o0];v2=fr2[o0];
      target[o1].Pos=(v2-v1)*div+v1;
      s++;}
     t++;}
}

void IM8MMesh::Trisx(STriangx*t0,STriangx*t1,s32 i){
    if(!t0||!t1){return;}i*=i>0;s32 o=0,o0=0;while(o0<3){
     o=t0[i].vertexIndices[o0];o*=o<=nvt;t1[i].vertexIndices[o0]=o;
     o=t0[i].textureIndices[o0];o*=o<=nsv;t1[i].textureIndices[o0]=o;o0++;}}

//! loads an m8m file
bool IM8MMesh::loadFile(io::IReadFile*file){
    if(!file){return 0;}SM8MHeader hdrx;const u16 M_MSZ=256;u32*M_BFR=0;

    file->read(&hdrx,sizeof(SM8MHeader));s32 SZH=96;
    core::stringc n1(hdrx.ident);n1[4]=0;
    FTYPE=(n1=="M8M")+(n1=="MDL5")*2+(n1=="MDL4")*3;if(FTYPE<1||FTYPE>3){return 0;}
    n1=file->getFileName();s32 s0=n1.findLast(47)+1;s0=s0?s0:n1.findLast(92)+1;
    n1=n1.subString(s0,(n1.size()-s0));
    printf("\n>>>>>>..M8M Loader: %s (%s)|%d",hdrx.ident,n1.c_str(),hdrx.framsiz);
    if(FTYPE==1&&hdrx.version!=1){
     printf("\n Wrong version.(%d|%d)... loading...",hdrx.version,1);}
    if(hdrx.nskins<1||hdrx.nverts>M8M_MVERTS||hdrx.ntris>M8M_MTRIS||FTYPE==1&&hdrx.skbpt!=88){
     printf("\n Not a supported file format...");return 0;}
    if(FTYPE==3){SZH=88;hdrx.skinw=hdrx.skinwx;hdrx.skinh=hdrx.skinhx;}//MDL4
    if(FTYPE!=1&&hdrx.framsiz==88){hdrx.skbpt=88;}
    s32 skb[2]={0,0};skt=hdrx.skbpt;skb[0]=(skt<=1)+(skt>1&&skt<4||skt>=10&&skt<12)*2+
     (skt==4||skt==12||skt==88)*3+(skt>4&&skt<10&&skt!=6&&skt!=7||skt>12&&skt!=88)*4;
    skw=hdrx.skinw>M_MSZ&&skb[0]?M_MSZ:hdrx.skinw<8||!skb[0]?8:hdrx.skinw;
    skh=hdrx.skinh>M_MSZ&&skb[0]?M_MSZ:hdrx.skinh<8||!skb[0]?8:hdrx.skinh;
    s32 sw[2]={hdrx.skinw,0},sh[2]={hdrx.skinh,0};
    nsk=hdrx.nskins;nvt=hdrx.nverts;nsv=hdrx.ntskve;
    s32 mip=(skt>=10&&skt!=88)*((hdrx.skinw*hdrx.skinh*skb[0])/4+
     (hdrx.skinw*hdrx.skinh*skb[0])/16+(hdrx.skinw*hdrx.skinh*skb[0])/64);
    printf("\n..v(%d,%d,%d).frms(%d)",skt,skw,skh,hdrx.nframs);
    hdrx.nframs=hdrx.nframs>2048?2048:hdrx.nframs+!hdrx.nframs;hdrx.framsiz=hdrx.nverts*6+10;
    hdrx.oftexco=SZH+(FTYPE==1)*768+(skb[0]*hdrx.skinw*hdrx.skinh)+(hdrx.skinw*!skb[0])+mip;
    if(FTYPE==1){file->read(mn,10);file->read(mv,22);
     printf("\n..i++...%s %d ",mn,mv[0]);
     printf("\n     ...%d %d %d %d %d",mv[1],mv[2],mv[3],mv[4],mv[5]);
     printf("\n     ...%d %d %d %d %d",mv[6],mv[7],mv[8],mv[9],mv[10]);}
    s32 esk=0;while(esk<(nsk-1)){s32 v[3]={skt,hdrx.skinw,hdrx.skinh},mipx=0;
     file->seek(hdrx.oftexco);if(FTYPE!=1){file->read(&v,12);mipx=v[0];}
     printf("\n..v(%d,%d,%d)-%d/%d",v[0],v[1],v[2],esk+2,nsk);
     v[0]=(v[0]<=1)+(v[0]>1&&v[0]<4||v[0]>=10&&v[0]<12)*2+(v[0]==4||v[0]==12||v[0]==88)*3
      +(v[0]>4&&v[0]<10&&v[0]!=6&&v[0]!=7||v[0]>12&&v[0]!=88)*4;
     mipx=(mipx>=10&&mipx!=88)*((v[0]*v[1]*v[2])/4+(v[0]*v[1]*v[2])/16+(v[0]*v[1]*v[2])/64);
     hdrx.oftexco+=(FTYPE!=1)*12+(v[0]*v[1]*v[2])+(v[1]*!v[0])+mipx;skb[1]=!esk?v[0]:skb[1];
     sw[1]=!esk?v[1]*!!skb[1]:sw[1];sh[1]=!esk?v[2]*!!skb[1]:sh[1];esk++;}
    nsk=!!skb[1]+1;

///////
printf("\n..ident...%s.",hdrx.ident);
printf("\nversi...%d.",hdrx.version);
printf("\nscae0._%f.",hdrx.scale3[0]);printf(" 1._%f.",hdrx.scale3[1]);
 printf(" 2._%f.",hdrx.scale3[2]);
printf("\nscao0.%f.",hdrx.scalo3[0]);printf(" 1.%f.",hdrx.scalo3[1]);
 printf(" 2.%f.",hdrx.scalo3[2]);

printf("\n..frsiz...%d.",hdrx.framsiz);
printf("\noftec...%d.",hdrx.oftexco);
printf("\nnull1...%d.",hdrx.nullv1);
printf("\nnull2...%d.",hdrx.nullv2);

printf("\n.nskis...%d.",hdrx.nskins);
printf("\nskiwi...%d.",hdrx.skinw);printf(" skwx...%d.",hdrx.skinwx);
printf("\nskihe...%d.",hdrx.skinh);printf(" skhx...%d.",hdrx.skinhx);
printf("\nnveti...%d.",hdrx.nverts);

printf("\n.ntria...%d.",hdrx.ntris);
printf("\nnfrms...%d.",hdrx.nframs);
printf("\nnskve...%d.",hdrx.ntskve);
printf("\nnull3...%d.",hdrx.nullv3);

printf("\n.null4...%d.",hdrx.nullv4);
printf("\nskbpt...%d.",hdrx.skbpt);
printf("\nskiwi...%d.",hdrx.skinw);
printf("\nskihe...%d.\n",hdrx.skinh);

//////SkinTex
    video::IVideoDriver*driver=SceneManager->getVideoDriver();
    IntpBfr->Material.MaterialType=video::EMT_SOLID;
    video::SColor color;s32 numsk=0,skbx=0;skbm[0]=0;skbm[1]=0;M_BFR=new u32[(M_MSZ+1)*M_MSZ];
    while(numsk<nsk&&numsk<2){skbx=skb[numsk];s32 sww=sw[numsk],shh=sh[numsk];
     file->seek(SZH+(FTYPE==1)*768+numsk*(skb[0]*sw[0]*sh[0]+(FTYPE!=1)*(12+mip)));
     f32 xs=f32(skw+0.002f)/f32(sww+!sww);f32 ys=f32(skh+0.002f)/f32(shh+!shh);u8 a=0;
     f32 o0=0,o1=0,x0=0,x1=0,y0=0,y1=0;s32 o=0,c=M_MSZ*M_MSZ<skw*skh?M_MSZ*M_MSZ:skw*skh,i=0;
     o=0;while(o<c&&skbx){u8 dat[skbx];file->read(&dat,sizeof(dat));
      if(skbx>=3&&skt==88){i=(dat[0]-s32(dat[0]/10)*10==3)*120+(dat[1]-
       s32(dat[1]/10)*10==3)*40;i=!i*255+(i<121)*i;
       color=video::SColor(i,dat[2],dat[1],dat[0]);M_BFR[o]=color.color;}
      if(skbx>=3&&skt!=88){i=skbx>3?dat[3]:255;
       color=video::SColor(i,dat[2],dat[1],dat[0]);M_BFR[o]=color.color;}
      if(skbx<3){color=video::R5G6B5toA8R8G8B8(dat[!(skbx-2)]*
       256+!(skbx-2)*dat[0]);M_BFR[o]=color.color;}o++;a=i>254?a+!a:(i||a)*2;
      //scale
      o0++;x1+=xs-1;while(xs-1>0&&(s32)x1>0){M_BFR[o]=M_BFR[o-1];o+=o<c;x1--;}x0+=xs;
      o1=(s32)x0;while(xs<1&&x0-o1+xs<1.15f&&o0<sww){file->read(&dat,sizeof(dat));x0+=xs;o0++;}
      if(o0>=sww){o0=0;x0=0;x1=0;if(ys-1>0){y1+=ys-1;
        while((s32)y1>0){o1=0;while(o1<skw){M_BFR[o]=M_BFR[o-skw];o+=o<c;o1++;}y1--;}}
       if(ys<1){y0+=ys;o1=(s32)y0;while(y0-o1+ys<1){
         file->seek(file->getPos()+sww*skbx);y0+=ys;}}}}
      //scale
     if(!numsk&&a==2){IntpBfr->Material.MaterialType=video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF;}
     o=0;while(o<c&&(!skbx||!a)){if(!skbx){M_BFR[o]=video::SColor(255,255,255,255).color;}
      else{color=M_BFR[o];color.setAlpha(255);M_BFR[o]=color.color;}o++;}
     video::IImage*skbn=driver->createImageFromData(video::ECF_A8R8G8B8,core::
      dimension2d<s32>(skw,skh),M_BFR,0,1);
     if(numsk<=0){skbm[0]=skbn;IntpBfr->Material.setTexture(0,
       driver->addTexture("s0.bmp",skbm[0]));
      printf("\n..Skin0Form...%dx%dx%d.",skw,skh,skbx);}
     if(numsk>0&&numsk<nsk){skbm[1]=skbn;IntpBfr->Material.setTexture(1,
       driver->addTexture("s1.bmp",skbm[1]));
      printf("..Skin1Form...%dx%dx%d.",skw,skh,skbx);}
    numsk++;}delete M_BFR;
//////SkinTex

    // create Memory for indices and frames
    ntr=hdrx.ntris;nfr=hdrx.nframs;
    VertList=new core::array<v3f>[nfr];s32 i=0;

    // read TextureCoords
    file->seek(hdrx.oftexco);
    MTCoords=new STCoordx[nsv];
    if(!file->read(MTCoords,sizeof(STCoordx)*nsv)){
     printf(" Error reading TextureCoords.");return 0;}
    printf("..TCoords.OK");
    //MDL5-MDL4
    if(FTYPE!=1){f32 fc[2]={2048.0f/f32(hdrx.skinw+!hdrx.skinw),
      2048.0f/f32(hdrx.skinh+!hdrx.skinh)};
     i=0;while(i<nsv){
      MTCoords[i].s=(s16)(s32)(MTCoords[i].s*fc[0]+0.5f);
      MTCoords[i].t=(s16)(s32)(MTCoords[i].t*fc[1]+0.5f);
     i++;}}
    // read Triangles
    MTriangs=new STriangx[ntr];
    if(file->read(MTriangs,sizeof(STriangx)*ntr)){
     i=0;while(i<ntr){Trisx(MTriangs,MTriangs,i);i++;}}
    else{printf(" Error reading triangles.");return 0;}
    printf("..Triangles.OK");

    // read Vertices
    SM8MFrame*frame=(SM8MFrame*)new u8[M8M_MVERTS*6+10];

    f32 scl[3];scl[0]=hdrx.scale3[0];scl[1]=hdrx.scale3[1];scl[2]=hdrx.scale3[2];
    f32 sco[3];sco[0]=hdrx.scalo3[0];sco[1]=hdrx.scalo3[1];sco[2]=hdrx.scalo3[2];
    i=0;while(i<nfr){
     // read vertices
     if(FTYPE==1){file->read(frame,hdrx.framsiz);}
     if(FTYPE!=1){file->read(frame,20);file->read(frame->name,16);s32 o0=0;
      while(o0<nvt){u16 o1=0;file->read(&frame->vts[o0],6);file->read(&o1,2);o0++;}}

     // store frame data
     SFDatax fdata;fdata.start=i;fdata.end=i;fdata.fps=1;
     if(frame->name[0]){
      s32 s=0;while(s<10&&!!frame->name[s]&&frame->name[s]!=' '&&(frame->name[s]<'0'||
       frame->name[s]>'9'||frame->name[s+1]>=58&&s<=8)){
       fdata.name+=frame->name[s];fdata.name.make_lower();s++;}
      if(!FData.empty()&&FData[FData.size()-1].name==fdata.name){++FData[FData.size()-1].end;}
      else{FData.push_back(fdata);}
     }

     // add vertices
     s32 s=0;while(s<nvt){v3f v;
      v.X=frame->vts[s].vtx[0]*scl[0]+sco[0];
      v.Z=frame->vts[s].vtx[1]*scl[1]+sco[1];
      v.Y=frame->vts[s].vtx[2]*scl[2]+sco[2];
      VertList[i].push_back(v);s++;}

     i++;}delete frame;printf("..Vertices.OK\n\n");
    return createMesh(VertList,FData,MTCoords,MTriangs,nvt,ntr,nfr,nsv,skbm[0]);
}


bool IM8MMesh::createMesh(core::array<v3f>*VertListx,
    const core::array<SFDatax> &FDatax,STCoordx*MTCoordsx,STriangx*MTriangsx,
    const s32 &nvtx,const s32 &ntrx,const u32 &nfrx,const s32 &nsvx,video::IImage*im){
    if(!VertListx||!MTCoordsx||!MTriangsx||nvtx<=0||ntrx<=0||nsvx<=0||!im||
     nvtx>M8M_MVERTS||ntrx>M8M_MTRIS){return 0;}
    const u16 M_MSZ=256;s32 i=0;setSkin(0,im);FTYPE=FTYPE+!FTYPE;
    IntpBfr->Material.Lighting=0;IntpBfr->Material.ZWriteEnable=1;
    IntpBfr->Material.Wireframe=0;IntpBfr->Material.BackfaceCulling=1;
    IntpBfr->Material.GouraudShading=0;IntpBfr->Material.FogEnable=0;
    IntpBfr->Material.NormalizeNormals=0;IntpBfr->Material.ZBuffer=1;
    IntpBfr->Material.PointCloud=0;

    skw=skbm[0]->getDimension().Width,skh=skbm[0]->getDimension().Height;
    skw=skw>M_MSZ?M_MSZ:skw<8?8:skw;skh=skh>M_MSZ?M_MSZ:skh<8?8:skh;skt=88;
    nvt=nvtx;nsv=nsvx;nsk=1;ntr=ntrx;nfr=nfrx>2048?2048:nfrx+!nfrx;

    if(MTCoords!=MTCoordsx){delete MTCoords;MTCoords=new STCoordx[nsv];
     i=0;while(i<nsv){MTCoords[i].s=MTCoordsx[i].s;MTCoords[i].t=MTCoordsx[i].t;i++;}}
    if(MTriangs!=MTriangsx){delete MTriangs;MTriangs=new STriangx[ntr];
     i=0;while(i<ntr){Trisx(MTriangsx,MTriangs,i);i++;}}
    if(FData.const_pointer()!=FDatax.const_pointer()){SFDatax fdata;
     i=0;while(i<FDatax.size()){fdata.name=FDatax[i].name;fdata.start=FDatax[i].start;
      fdata.end=FDatax[i].end;fdata.fps=FDatax[i].fps;FData.push_back(fdata);i++;}}
    if(VertList!=VertListx){delete VertList;VertList=new core::array<v3f>[nfr];
     v3f v;i=0;while(i<nfr){s32 o0=0;while(o0<nvt){
       v=VertListx[i].pointer()[o0];VertList[i].push_back(v);o0++;}i++;}}

    // create indices
    u32 count=ntr*3;
    i=0;while(i<count){
     IntpBfr->Indices.push_back(i);
     IntpBfr->Indices.push_back(i+1);
     IntpBfr->Indices.push_back(i+2);
     i+=3;}

    // interpolate buffer
    if(nfr){
     u32 count=ntr*3;IntpBfr->Vertices.set_used(count);
     v2f v;f32 mmaxs=1.0f/2048.0f,mmaxt=1.0f/2048.0f;count/=3;
     s32 n=0;while(n<count){
      s32 i=0;while(i<3){
       v.X=(MTCoords[MTriangs[n].textureIndices[i]].s+0.02f)*mmaxs;
       v.Y=(MTCoords[MTriangs[n].textureIndices[i]].t+0.02f)*mmaxt;
       IntpBfr->Vertices[n*3+i].TCoords=v;
       IntpBfr->Vertices[n*3+i].Color=video::SColor(255,255,255,255);
       IntpBfr->Vertices[n*3+i].Normal=v3f(0,-0.1f,0);
      i++;}
     n++;}}
    calculateBoundingBox();return 1;
}


//! calculates the bounding box
void IM8MMesh::calculateBoundingBox()
{
    IntpBfr->BoundingBox.reset(0,0,0);
    if(VertList[0].size()){s32 i=0;while(i<VertList[0].size()){
      IntpBfr->BoundingBox.addInternalPoint(VertList[0].pointer()[i]);i++;}
    }
}


//! sets a flag of all contained materials to a new value
void IM8MMesh::setMaterialFlag(video::E_MATERIAL_FLAG flag,bool newvalue)
{
    IntpBfr->Material.setFlag(flag,newvalue);
}


//! returns an axis aligned bounding box
const core::aabbox3d<f32>&IM8MMesh::getBoundingBox() const
{
    return IntpBfr->BoundingBox;
}


//! set user axis aligned bounding box
void IM8MMesh::setBoundingBox(const core::aabbox3df& box)
{
    IntpBfr->BoundingBox=box;
}


//! Returns the type of the animated mesh.
E_ANIMATED_MESH_TYPE IM8MMesh::getMeshType()const
{
    return (E_ANIMATED_MESH_TYPE)88;
}

//! Returns frame loop data for a special M8M animation type.
bool IM8MMesh::getFrameLoop(const c8*name,s32 &outStart,s32 &outEnd,s32 &outFPS)const{
    s32 i=0;while(i<FData.size()){
     if(FData[i].name==name){
      outStart=FData[i].start<<M8M_FRAME2;
      outEnd=FData[i].end<<M8M_FRAME2;
      outEnd+=!M8M_FRAME2?1:(1<<M8M_FRAME2)-1;
      outFPS=FData[i].fps<<M8M_FRAME2;
      return 1;}i++;}
    return 0;
}


//! Returns amount of m8m animations in this file.
u32 IM8MMesh::getAnimationCount()const
{
    return FData.size();
}


//! Returns name of m8m animation.
const c8*IM8MMesh::getAnimationName(u32 nr)const
{
    if(nr>=FData.size()){return 0;}
    return FData[nr].name.c_str();
}


video::ITexture*IM8MMesh::getTexture(bool nr)const
{
    if(IntpBfr->Material.getTexture(1)&&nr&&nsk>1){return IntpBfr->Material.getTexture(1);}
    return IntpBfr->Material.getTexture(0);
}


video::IImage*IM8MMesh::getSkin(bool nr)const
{
    if(skbm[1]&&nr){return skbm[1];}
    return skbm[0];
}


void IM8MMesh::setSkin(bool nr,video::IImage*im)
{
    video::IVideoDriver*driver=SceneManager->getVideoDriver();nr*=!!skbm[0];
    if(!nr&&im&&im!=skbm[0]){delete skbm[0];skbm[0]=0;skbm[0]=driver->createImageFromData(
     video::ECF_A8R8G8B8,im->getDimension(),im->lock(),0,1);im->unlock();
     IntpBfr->Material.setTexture(0,driver->addTexture("s0.bmp",skbm[0]));}
    if(nr&&im!=skbm[1]){delete skbm[1];skbm[1]=0;if(im){skbm[1]=driver->createImageFromData(
     video::ECF_A8R8G8B8,im->getDimension(),im->lock(),0,1);im->unlock();
     IntpBfr->Material.setTexture(1,driver->addTexture("s1.bmp",skbm[1]));}}
}


bool IM8MMesh::WriteM8M(const c8*fname,bool nr,const c8*mnx,const u16*mvx,bool ni){
    //write M8M header
    const u16 M_MSZ=256;u32*M_BFR=0;
    if(!SceneManager||nvt>M8M_MVERTS||ntr>M8M_MTRIS){return 0;}
    ni*=!nr*!!mnx;mvx=mvx&&!mvx[10]?mvx:0;

    core::stringc fnm,src=fname;
    fnm=src.subString(0,src.findLast('.')<0?src.size():src.findLast('.'));
    if(!nr){fnm+=".m8m";}if(nr){fnm+=".mdl";}
    if(ni){ni=0;IM8M::CMRWFile*filex=filex->createMRWFile(fnm.c_str(),1);
	 if(filex){c8 hdc[4];filex->read(&hdc,4);core::stringc n1=hdc;n1[4]=0;
      if(n1=="M8M"){ni=1;}filex->drop();}}
    IM8M::CMRWFile*file=file->createMRWFile(fnm.c_str(),ni);
    if(!file){printf("\nError writing file..");return 0;}
//////mn
    if(ni){file->seek(96);
     if(!nr){src="none";if(mnx){src=mnx;}while(src.size()<10){src+=(c8)0;}
      file->write(src.c_str(),10);
      u16 mvz[11];s32 i=0;while(i<11){mvz[i]=0;if(mnx&&mvx){mvz[i]=mvx[i];}i++;}
      file->write(mvz,22);
      c8 ec[736];i=0;while(i<736){ec[i]=0;i++;}file->write(ec,736);}
     file->drop();return 1;}
//////mn
    ////////////////////////////////////////////////////////////////////////////
    printf("\n Please wait...\n Converting <");nfr=nfr>2048?2048:nfr+!nfr;const s32 tr3=ntr*3;
    nvt=VertList[0].size();s32 m=0;while(m<ntr){Trisx(MTriangs,MTriangs,m);m++;}printf(".");
    s32 vi[tr3],ti[tr3];m=0;while(m<ntr){s32 i=0;while(i<3){vi[m*3+i]=
     MTriangs[m].vertexIndices[i];ti[m*3+i]=MTriangs[m].textureIndices[i];i++;}m++;}
    u32 boost[nvt];m=0;while(m<nvt){boost[m]=0;s32 i=0;while(i<tr3){
       if(m==vi[i]){boost[m]=m;break;}i++;}m++;}printf(".");
    v3f vtm;f32 mmaxs=1.0f/2048.0f,mmaxt=1.0f/2048.0f;printf(".");
    //remov duplicate vertices
    m=0;s32 ii=0;while(m<tr3){
     if(m<nvt&&!ii){s32 bs=boost[m];bs*=bs<VertList[0].size();vtm=VertList[0].pointer()[bs];
      s32 o0=0;while(m>o0){s32 bx=boost[o0];bx*=bx<VertList[0].size();
       if(vtm==VertList[0].pointer()[bx]){
        s32 o1=0;while(o1<nfr){if(vtm!=VertList[o1].pointer()[bx]){
          o1=-1;break;}o1++;}if(o1<0){o0++;continue;}
        s32 i=m+1;while(i<nvt){o1=boost[i];boost[i-1]=o1;i++;}nvt=nvt>1?nvt-1:1;
        i=0;while(i<ntr){o1=0;while(o1<3){
          if(MTriangs[i].vertexIndices[o1]==m){MTriangs[i].vertexIndices[o1]=o0;}
          if(MTriangs[i].vertexIndices[o1]>m){MTriangs[i].vertexIndices[o1]--;}o1++;}
         i++;}m-=m>=0;break;}o0++;}}
     if(m<nsv){vtm.X=MTCoords[m].s;vtm.Y=MTCoords[m].t;
      s32 bs=0;while(bs<tr3&&!ii){if(ti[bs]==m){bs=-1;break;}bs++;}
      s32 o0=0;while(m>o0){
       if(vtm.X==(f32)MTCoords[o0].s&&vtm.Y==(f32)MTCoords[o0].t&&ii||bs>=0&&!ii){
        s32 o1=0;if(bs>=0){o1=0;while(o1<tr3){if(ti[o1]>m){ti[o1]--;}o1++;}}bs=-1;
        s32 i=m+1;while(i<nsv){MTCoords[i-1]=MTCoords[i];i++;}nsv=nsv>1?nsv-1:1;
        i=0;while(i<ntr){o1=0;while(o1<3){
          if(MTriangs[i].textureIndices[o1]==m){MTriangs[i].textureIndices[o1]=o0;}
          if(MTriangs[i].textureIndices[o1]>m){MTriangs[i].textureIndices[o1]--;}o1++;}
         i++;}m-=m>=0;break;}o0++;}}m++;if(!ii&&m>=tr3){ii=1;m=0;}}
    //remov duplicate vertices
    f32 ts=0;m=0;while(m<nfr){ts+=1.0f/(f32)nfr;if(ts>=0.1f){printf("-");ts=0;}
     s32 i=0;while(i<nvt){u32 bs=boost[i];bs*=bs<VertList[0].size();
      vtm=VertList[m].pointer()[bs];VertList[m].pointer()[i]=vtm;i++;}m++;}
    v2f v;m=0;while(m<ntr){s32 i=0;while(i<3){
      v.X=(MTCoords[MTriangs[m].textureIndices[i]].s+0.02f)*mmaxs;
      v.Y=(MTCoords[MTriangs[m].textureIndices[i]].t+0.02f)*mmaxt;
      IntpBfr->Vertices[m*3+i].TCoords=v;i++;}
     m++;}printf("> nvt(%d):tris(%d):nsv(%d)\n",nvt,ntr,nsv);
    ////////////////////////////////////////////////////////////////////////////
    SM8MHeader hdrx;s32 i=0,fr=0,fn=0;core::aabbox3d<f32> ebox=IntpBfr->BoundingBox;
    ntr=ntr+!ntr;nvt=nvt+!nvt;nsv=nsv+!nsv;
    if(!nr){hdrx.ident[0]='M';hdrx.ident[1]='8';hdrx.ident[2]='M';hdrx.ident[3]=0;
     hdrx.version=1;hdrx.skbpt=88;}
    if(nr){hdrx.ident[0]='M';hdrx.ident[1]='D';hdrx.ident[2]='L';hdrx.ident[3]='5';
     hdrx.version=1;hdrx.skbpt=5;if(mnx){hdrx.skbpt=4;}}
    hdrx.nskins=!!skbm[0]+(!skbm[1]?0:!(skbm[0]->getDimension().Width-
     skbm[1]->getDimension().Width+skbm[0]->getDimension().Height-
     skbm[1]->getDimension().Height));if(!hdrx.nskins){return 0;}
    s32 sww=getSkin(0)->getDimension().Width,shh=getSkin(0)->getDimension().Height;
    skw=sww>M_MSZ?M_MSZ:sww<8?8:sww;skh=shh>M_MSZ?M_MSZ:shh<8?8:shh;

    v3f vmax,vmin;i=0;while(i<nfr){v3f v;
    s32 n=0;while(n<nvt){v=VertList[i].pointer()[n];
     if(v.X>vmax.X){vmax.X=v.X;}if(v.Y>vmax.Y){vmax.Y=v.Y;}if(v.Z>vmax.Z){vmax.Z=v.Z;}
     if(v.X<vmin.X){vmin.X=v.X;}if(v.Y<vmin.Y){vmin.Y=v.Y;}if(v.Z<vmin.Z){vmin.Z=v.Z;}
    n++;}i++;}

    hdrx.scale3[0]=(vmax.X-vmin.X)/65535.0f;
    hdrx.scale3[1]=(vmax.Z-vmin.Z)/65535.0f;
    hdrx.scale3[2]=(vmax.Y-vmin.Y)/65535.0f;
    hdrx.scalo3[0]=vmin.X;
    hdrx.scalo3[1]=vmin.Z;
    hdrx.scalo3[2]=vmin.Y;
    hdrx.framsiz=!nr?nvt*6+10:nr&&mnx?88:0;
    hdrx.oftexco=96;
    hdrx.nullv1=0;
    hdrx.nullv2=0;
    hdrx.skinwx=0;
    hdrx.skinhx=0;
    hdrx.nverts=nvt;
    hdrx.ntris=ntr;
    hdrx.nframs=nfr;
    hdrx.ntskve=nsv;
    hdrx.nullv3=0;
    hdrx.nullv4=0;
    hdrx.skinw=skw;printf(" :skinw:%d",hdrx.skinw);
    hdrx.skinh=skh;printf(" :skinh:%d",hdrx.skinh);
    file->write(&hdrx,sizeof(SM8MHeader));
    ////////////////////////////////////////////////////////////////////////////
//////mn
    if(!nr){src="none";if(mnx){src=mnx;}while(src.size()<10){src+=(c8)0;}
     file->write(src.c_str(),10);
     u16 mvz[11];s32 i=0;while(i<11){mvz[i]=0;if(mnx&&mvx){mvz[i]=mvx[i];}i++;}
     file->write(mvz,22);
     c8 ec[736];i=0;while(i<736){ec[i]=0;i++;}file->write(ec,736);}
//////SkinTex
    M_BFR=new u32[(M_MSZ+1)*M_MSZ];
    s32 nm=0;while(nm<hdrx.nskins&&nm<2){video::SColor color;
     f32 xs=f32(skw+0.002f)/f32(sww+!sww);f32 ys=f32(skh+0.002f)/f32(shh+!shh);u8 a=0;
     f32 o0=0,o1=0,x0=0,x1=0,y0=0,y1=0;s32 o=0,c=M_MSZ*M_MSZ<skw*skh?M_MSZ*M_MSZ:skw*skh,i=0;
     o=0;while(o<c){s32 ix=o+i,iy=0;while(ix>=sww){ix-=sww;iy+=(iy<shh);}
      color=getSkin(nm)->getPixel(ix,iy).color;M_BFR[o]=color.color;o++;
      o1=color.getAlpha();a+=o1<256&&o1>0&&!a;
      //scale
      o0++;x1+=xs-1;while(xs-1>0&&(s32)x1>0){M_BFR[o]=M_BFR[o-1];o+=o<c;x1--;}x0+=xs;
      o1=(s32)x0;while(xs<1&&x0-o1+xs<1.15f&&o0<sww){i++;x0+=xs;o0++;}
      if(o0>=sww){o0=0;x0=0;x1=0;if(ys-1>0){y1+=ys-1;
        while((s32)y1>0){o1=0;while(o1<skw){M_BFR[o]=M_BFR[o+i-skw];o+=o<c;o1++;}y1--;}}
       if(ys<1){y0+=ys;o1=(s32)y0;while(y0-o1+ys<1){i+=sww;y0+=ys;}}}}
      //scale
     o=0;while(o<c&&!a){color=M_BFR[o];color.setAlpha(255);M_BFR[o]=color.color;o++;}
     if(!nr||mnx){s32 xi=0,yi=0;//M8M format RGBA 8772
      o=0;while(yi<skh){s32 c=0,a=0;video::SColor color=M_BFR[o];
       a=color.getAlpha();a=a==255?0:a<255&&a>40?1:a<=40&&a>0?2:3;
       c=color.getBlue();if(c-(s32(c/10)*10)==3){c--;}
       if(a==1||a==3){c=(s32(c/10)*10)+3;}file->write(&c,1);
       c=color.getGreen();if(c-(s32(c/10)*10)==3){c--;}
       if(a==2||a==3){c=(s32(c/10)*10)+3;}file->write(&c,1);
       c=color.getRed();file->write(&c,1);xi=xi+1<skw?xi+1:0;yi+=!xi;o++;}}
     else{s32 xi=0,yi=0;//MDL5 format ARGB 8888
      if(hdrx.nskins>0&&nm!=0){s32 o0[3]={5,skw,skh};file->write(&o0,12);}
      o=0;while(yi<skh){s32 c=0;video::SColor color=M_BFR[o];
       c=color.getBlue();file->write(&c,1);c=color.getGreen();file->write(&c,1);
       c=color.getRed();file->write(&c,1);c=color.getAlpha();file->write(&c,1);
       xi=xi+1<skw?xi+1:0;yi+=!xi;o++;}}
     nm++;}delete M_BFR;printf(" :SkinTex OK");
//////TCoords
    STCoordx*MTCoords1=new STCoordx[nsv];

    if(!nr){
    i=0;while(i<nsv){
     MTCoords1[i].s=MTCoords[i].s;
     MTCoords1[i].t=MTCoords[i].t;
     file->write(&MTCoords1[i],4);
    i++;}}
    else{f32 fc[2]={1.0f/(2048.0f/f32(skw+!skw)),1.0f/(2048.0f/f32(skh+!skh))};
    i=0;while(i<nsv){
     MTCoords1[i].s=(s16)(s32)(MTCoords[i].s*fc[0]+0.5f);
     MTCoords1[i].t=(s16)(s32)(MTCoords[i].t*fc[1]+0.5f);
     file->write(&MTCoords1[i],4);
    i++;}}

    i=0;while(i<ntr){file->write(&MTriangs[i],12);i++;}
    delete[]MTCoords1;printf("\n :TCoords OK");
//////Frames
    SM8MFrame frame;
    fr=0;s32 fnx=0;while(fr<nfr){s32 fc=fr;
     f32 sc[3]={hdrx.scale3[0],hdrx.scale3[1],hdrx.scale3[2]};
     sc[0]+=!sc[0];sc[1]+=!sc[1];sc[2]+=!sc[2];
     if(fr>FData[fn].end){fn++;fnx=0;}else{fnx=!!fr*(fnx+1)%10;}
     i=0;while(i<10){if(i<FData[fn].name.size()){frame.name[i]=FData[fn].name[i];}
      else{if(fc==fr&&!!fnx){fc++;frame.name[i]=(fnx+48);}else{frame.name[i]=0;}}i++;}
     i=0;while(i<nvt){
      frame.vts[0].vtx[0]=u16((((VertList[fr].pointer()[i].X-vmin.X)*10.0f)/sc[0])/9.99998f);
      frame.vts[0].vtx[1]=u16((((VertList[fr].pointer()[i].Z-vmin.Z)*10.0f)/sc[1])/9.99998f);
      frame.vts[0].vtx[2]=u16((((VertList[fr].pointer()[i].Y-vmin.Y)*10.0f)/sc[2])/9.99998f);
      if(!i){if(!nr){file->write(&frame.name,10);}
       else{s32 o0[5]={2,0,0,0,0};file->write(&o0,20);file->write(&frame.name,10);
        u16 o1[3]={0,0,0};file->write(&o1,6);}}
      file->write(&frame.vts,6);if(nr){u16 o0=0;file->write(&o0,2);}
      if(!fr&&!i){printf(" Precision (%f:%f)",f32((f32)frame.vts[0].vtx[0]*hdrx.scale3[0]+
       vmin.X),VertList[0].pointer()[0].X);}
     i++;}
    fr++;}
    file->drop();
    return 1;
}

core::array<IM8MMesh::v3f>*IM8MMesh::getVertList()const{return VertList;}
core::array<IM8MMesh::SFDatax> IM8MMesh::getFData()const{return FData;}
IM8MMesh::STCoordx*IM8MMesh::getMTCoords()const{return MTCoords;}
IM8MMesh::STriangx*IM8MMesh::getMTriangs()const{return MTriangs;}
s32 IM8MMesh::getnsk()const{return nsk;}
s32 IM8MMesh::getnvt()const{return nvt;}
s32 IM8MMesh::getntr()const{return ntr;}
s32 IM8MMesh::getnsv()const{return nsv;}
s32 IM8MMesh::getskt()const{return skt;}
s32 IM8MMesh::getskw()const{return skw;}
s32 IM8MMesh::getskh()const{return skh;}

u8 IM8MMesh::getFileType()const{return FTYPE;}
const c8*IM8MMesh::getMName()const{return mn;}
const u16*IM8MMesh::getMv()const{return mv;}

} // end namespace scene
} // end namespace irr

//////////////////////////////////////////////////cpp
namespace irr
{
namespace scene
{

//! Constructor
CM8MMeshLoader::CM8MMeshLoader(scene::ISceneManager*smgr):SceneManager(smgr){}


bool CM8MMeshLoader::isALoadableFileExtension(const c8*filename) const
{
    return (strstr(filename,".m8m")||strstr(filename,".mdl"));
}


//! creates/loads an animated mesh from the file.
//! \return Pointer to the created mesh. Returns 0 if loading failed.
//! If you no longer need the mesh, you should call IAnimatedMesh::drop().
//! See IReferenceCounted::drop() for more information.
IAnimatedMesh*CM8MMeshLoader::createMesh(io::IReadFile*file)
{
    IAnimatedMesh*msh=new IM8MMesh(SceneManager);
    if(msh)
    {
        if(((IM8MMesh*)msh)->loadFile(file)){return msh;}
        msh->drop();
    }
    return 0;
}

} // end namespace scene
} // end namespace irr

#endif // _IRR_COMPILE_WITH_M8M_MESH_LOADER_
