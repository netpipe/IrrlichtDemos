//============================================================================//
//
// date: 22.12.2008
//============================================================================//
//////////////////////////////////////////////////cpp
#include "CASCIIMeshConverter.h"
#ifdef _IRR_COMPILE_WITH_ASCII_MESH_LOADER_

#include "IReadFile.h"
#include "ISceneManager.h"
#include "IMeshCache.h"

//////////////////////////////////////////////////cpp
namespace irr
{
namespace scene
{
//////////////////////////////////////////////////cpp
    const u32 ASCII_MTRIS=21845;
    const u32 ASCII_MVERTS=88888;
    const u16 ASCII_FRAME2=2;
    const f32 ASCII_FRAME2RPC=1.0f/(1<<ASCII_FRAME2);
//////////////
    struct ASCIIVertex
    {
        f32   vtx[3];
    };

//! constructor
IASCIIMesh::IASCIIMesh(scene::ISceneManager*smgr):SceneManager(smgr),IntpBfr(0),
    VertList(0),FData(0),nsk(0),nvt(0),ntr(0),nfr(0),nsv(0),skt(0),skw(0),skh(0){
    IntpBfr=new SMeshBuffer;MTCoords=0;MTriangs=0;skbm[0]=0;skbm[1]=0;
    NormList=0;}

//! destructor
IASCIIMesh::~IASCIIMesh()
{
    video::IVideoDriver*driver=SceneManager->getVideoDriver();
    delete[]VertList;VertList=0;delete[]MTCoords;MTCoords=0;
    delete[]MTriangs;MTriangs=0;delete skbm[0];skbm[0]=0;delete skbm[1];skbm[1]=0;
    driver->removeTexture(IntpBfr->Material.getTexture(1));
    driver->removeTexture(IntpBfr->Material.getTexture(0));if(IntpBfr){delete IntpBfr;}
    delete[]NormList;NormList=0;
}

//! returns the amount of frames in milliseconds. If the amount is 1, it is a static mesh.
u32 IASCIIMesh::getFrameCount()const
{
    return nfr<<ASCII_FRAME2;
}

IMesh*IASCIIMesh::getMesh(s32 frame,s32 detailLevel,s32 startFrame,s32 endFrame)
{
    if((u32)frame>getFrameCount()){frame=(frame%getFrameCount());}

    if(startFrame==-1&&endFrame==-1){
        startFrame=0;
        endFrame=getFrameCount();
    }

    updateIntpBfr(frame,startFrame,endFrame);
    return this;
}

u32 IASCIIMesh::getMeshBufferCount()const
{
    return 1;
}

IMeshBuffer*IASCIIMesh::getMeshBuffer(u32 nr)const
{
    return IntpBfr;
}

//! Returns pointer to a mesh buffer which fits a material
IMeshBuffer*IASCIIMesh::getMeshBuffer(const video::SMaterial &material)const
{
    if(IntpBfr->Material==material){getMeshBuffer(1);}return 0;
}

// updates the interpolation buffer
void IASCIIMesh::updateIntpBfr(s32 frame,s32 startFrame,s32 endFrame)
{
    u32 firstFrame,secondFrame;
    f32 div;

    // TA: resolve missing ipol in loop between end-start

    if(endFrame-startFrame==0)
    {
        firstFrame=frame>>ASCII_FRAME2;
        secondFrame=frame>>ASCII_FRAME2;
        div=1.0f;
    }
    else
    {
        // key frames
        u32 s=startFrame>>ASCII_FRAME2;
        u32 e=endFrame>>ASCII_FRAME2;

        firstFrame=frame>>ASCII_FRAME2;
        secondFrame=firstFrame+1>e?s:firstFrame+1;

        firstFrame=core::s32_min(nfr-1,firstFrame);
        secondFrame=core::s32_min(nfr-1,secondFrame);

        frame&=(1<<ASCII_FRAME2)-1;
        div=frame*ASCII_FRAME2RPC;
    }

    video::S3DVertex*target=static_cast<video::S3DVertex*>(IntpBfr->getVertices());
    v3f*fr1=VertList[firstFrame].pointer(),*fr2=VertList[secondFrame].pointer();
    v3f*nfr1=NormList[firstFrame].pointer(),*nfr2=NormList[secondFrame].pointer();

    v3f v1,v2;
    s32 t=0;while(t<ntr){
     s32 s=0;while(s<3){s32 o0=MTriangs[t].vertexIndices[s],o1=t*3+s;
      v1=fr1[o0];v2=fr2[o0];
      target[o1].Pos=(v2-v1)*div+v1;
      v1=nfr1[o0];v2=nfr2[o0];
      target[o1].Normal=(v2-v1)*div+v1;
      s++;}
     t++;}
}


//! loads an ASCII file
namespace IASCII
{
bool ltr(c8 &ch){return (ch>='A'&&ch<='Z'||ch>='a'&&ch<='z'||ch=='_');}
bool num(c8 &ch){return (ch>='0'&&ch<='9');}
}
void IASCIIMesh::Trisx(STriangx*t0,STriangx*t1,s32 i){
    if(!t0||!t1){return;}i*=i>0;s32 o=0,o0=0;while(o0<3){
     o=t0[i].vertexIndices[o0];o*=o<=nvt;t1[i].vertexIndices[o0]=o;
     o=t0[i].textureIndices[o0];o*=o<=nsv;t1[i].textureIndices[o0]=o;o0++;}}

bool IASCIIMesh::loadFile(io::IReadFile*file){
    if(!file){return 0;}const u16 M_MSZ=256;u32*M_BFR=0;
    s32 FLMT=file->getSize();if(FLMT<=0){return 0;}printf("\n>>>>>>..ASCII Loader:(%d)",FLMT);

    s32 cp=0,i=0;char ch=0;core::stringc n1;ASCIIVertex*vertx=0,*vertx1=0;
    core::array<v3f>*vertices=0;

    while(cp<FLMT){n1="";file->read(&ch,1);cp++;
     while(!IASCII::ltr(ch)&&cp<FLMT){file->read(&ch,1);cp++;}
     while(IASCII::ltr(ch)&&cp<FLMT){n1+=ch;file->read(&ch,1);cp++;}n1.make_lower();
     if(n1=="skins"){FTYPE=1;printf("\n 3DGS ASCII");break;}
     if(n1=="milkshape"){FTYPE=2;printf("\n MilkShape 3D ASCII");break;}}


//3dgs ascii
    file->seek(0);cp=0;while(cp<FLMT&&FTYPE==1){file->read(&ch,1);cp++;
//num skins
     while(n1!="skins"&&cp<FLMT){n1="";
      while(!IASCII::ltr(ch)&&cp<FLMT){file->read(&ch,1);cp++;}
      while(IASCII::ltr(ch)&&cp<FLMT){n1+=ch;file->read(&ch,1);cp++;}
      n1.make_lower();}
     if(n1=="skins"){n1="";
      while((ch<=' '||ch==':')&&cp<FLMT){file->read(&ch,1);cp++;}nsk=0;
      f32 ml=1;while(IASCII::num(ch)&&cp<FLMT){nsk=s32(nsk*ml+(ch-48));
       if(nsk){ml=10;}file->read(&ch,1);cp++;}
      printf("\n>Skins %d",nsk);}

      video::IVideoDriver*driver=SceneManager->getVideoDriver();
      IntpBfr->Material.MaterialType=video::EMT_SOLID;
      video::SColor color;s32 numsk=0,skbx=0;
      skbm[0]=0;skbm[1]=0;M_BFR=new u32[(M_MSZ+1)*M_MSZ];nsk=1;skw=8;skh=8;
      f32 o0=0,o1=0,x0=0,x1=0,y0=0,y1=0;s32 o=0,c=M_MSZ*M_MSZ<skw*skh?M_MSZ*M_MSZ:skw*skh,i=0;
      o=0;while(o<c){M_BFR[o]=video::SColor(255,255,255,255).color;o++;}

      video::IImage*skbn=driver->createImageFromData(video::ECF_A8R8G8B8,core::
       dimension2d<s32>(skw,skh),M_BFR,0,1);
      if(numsk<=0){skbm[0]=skbn;//driver->writeImageToFile(skbn,"s0.bmp");
       IntpBfr->Material.setTexture(0,driver->addTexture("s0.bmp",skbm[0]));
       printf("\n..Skin0Form...%dx%dx%d.",skw,skh,skbx);}
      delete M_BFR;printf("..Skin.OK");
//vertices
     while(n1!="vertices"&&cp<FLMT){n1="";
      while(!IASCII::ltr(ch)&&cp<FLMT){file->read(&ch,1);cp++;}
      while(IASCII::ltr(ch)&&cp<FLMT){n1+=ch;file->read(&ch,1);cp++;}
      n1.make_lower();}
     if(n1=="vertices"){n1="";
      while((ch<=' '||ch==':')&&cp<FLMT){file->read(&ch,1);cp++;}nvt=0;
      f32 ml=1;while(IASCII::num(ch)&&cp<FLMT){nvt=s32(nvt*ml+(ch-48));
       if(nvt){ml=10;}file->read(&ch,1);cp++;}
      if(nvt>ASCII_MVERTS){return 0;}
      vertx=new ASCIIVertex[nvt+!nvt];printf("\n>Vertices %d",nvt);

      s32 vct=0;while(vct<nvt&&cp<FLMT){
       f32 v3[3]={0,0,0};s32 ct=0;
       while(ct<3&&cp<FLMT){f32 t=0;s32 s=1;
        while(!IASCII::num(ch)&&ch!=' '&&ch!='.'&&ch!='-'&&cp<FLMT){n1+=ch;
         file->read(&ch,1);cp++;}
        if(ch=='-'){s=-1;file->read(&ch,1);cp++;}
        if(IASCII::num(ch)){ml=1;while(IASCII::num(ch)&&cp<FLMT){t=t*ml+(ch-48);
         if(t){ml=10;}file->read(&ch,1);cp++;}}
        if(ch=='.'){file->read(&ch,1);cp++;
         if(IASCII::num(ch)){ml=10;while(IASCII::num(ch)&&cp<FLMT){t+=(ch-48)/(ml+!ml);
          ml*=10;file->read(&ch,1);cp++;}}}
        v3[ct]=t*s;ct++;file->read(&ch,1);cp++;}
       vertx[vct].vtx[0]=v3[0];vertx[vct].vtx[1]=v3[1];vertx[vct].vtx[2]=v3[2];
      vct++;}}
//triangles vertices index
     while(n1!="triangles"&&cp<FLMT){n1="";
      while(!IASCII::ltr(ch)&&cp<FLMT){file->read(&ch,1);cp++;}
      while(IASCII::ltr(ch)&&cp<FLMT){n1+=ch;file->read(&ch,1);cp++;}
      n1.make_lower();}
     if(n1=="triangles"){n1="";
      while((ch<=' '||ch==':')&&cp<FLMT){file->read(&ch,1);cp++;}ntr=0;
      f32 ml=1;while(IASCII::num(ch)&&cp<FLMT){ntr=s32(ntr*ml+(ch-48));
       if(ntr){ml=10;}file->read(&ch,1);cp++;}
      if(ntr>ASCII_MTRIS){return 0;}
      MTriangs=new STriangx[ntr+!ntr];
      printf("\n>Triangles %d",ntr);

      s32 vct=0;while(vct<ntr&&cp<FLMT){
       s32 v3[3]={0,0,0};s32 ct=0;
       while(ct<3&&cp<FLMT){s32 t=0;
        while(!IASCII::num(ch)&&ch!=' '&&cp<FLMT){n1+=ch;
         file->read(&ch,1);cp++;}
        if(IASCII::num(ch)){ml=1;while(IASCII::num(ch)&&cp<FLMT){t=s32(t*ml+(ch-48));
         if(t){ml=10;}file->read(&ch,1);cp++;}}
        v3[ct]=t;ct++;file->read(&ch,1);cp++;}
       MTriangs[vct].vertexIndices[0]=(v3[0]<=nvt?v3[0]:0);
       MTriangs[vct].vertexIndices[1]=(v3[1]<=nvt?v3[1]:0);
       MTriangs[vct].vertexIndices[2]=(v3[2]<=nvt?v3[2]:0);
      vct++;}}
//tcoords
     while(n1!="skinpoints"&&cp<FLMT){n1="";
      while(!IASCII::ltr(ch)&&cp<FLMT){file->read(&ch,1);cp++;}
      while(IASCII::ltr(ch)&&cp<FLMT){n1+=ch;file->read(&ch,1);cp++;}
      n1.make_lower();}
     if(n1=="skinpoints"){n1="";
      while((ch<=' '||ch==':')&&cp<FLMT){file->read(&ch,1);cp++;}nsv=0;
      f32 ml=1;while(IASCII::num(ch)&&cp<FLMT){nsv=s32(nsv*ml+(ch-48));
       if(nsv){ml=10;}file->read(&ch,1);cp++;}
      MTCoords=new STCoordx[nsv+!nsv];printf("\n>TCoords %d",nsv);

      s32 vct=0;while(vct<nsv&&cp<FLMT){
       f32 v2[2]={0,0};s32 ct=0;
       while(ct<2&&cp<FLMT){f32 t=0;s32 s=1;
        while(!IASCII::num(ch)&&ch!=' '&&ch!='.'&&ch!='-'&&cp<FLMT){n1+=ch;
         file->read(&ch,1);cp++;}
        if(ch=='-'){s=-1;file->read(&ch,1);cp++;}
        if(IASCII::num(ch)){ml=1;while(IASCII::num(ch)&&cp<FLMT){t=t*ml+(ch-48);
         if(t){ml=10;}file->read(&ch,1);cp++;}}
        if(ch=='.'){file->read(&ch,1);cp++;
         if(IASCII::num(ch)){ml=10;while(IASCII::num(ch)&&cp<FLMT){t+=(ch-48)/(ml+!ml);
          ml*=10;file->read(&ch,1);cp++;}}}
        v2[ct]=t*s;ct++;file->read(&ch,1);cp++;}
       MTCoords[vct].s=v2[0];MTCoords[vct].t=v2[1];
      vct++;}}
//triangles skin index
     while(n1!="skintriangles"&&cp<FLMT){n1="";
      while(!IASCII::ltr(ch)&&cp<FLMT){file->read(&ch,1);cp++;}
      while(IASCII::ltr(ch)&&cp<FLMT){n1+=ch;file->read(&ch,1);cp++;}
      n1.make_lower();}
     if(n1=="skintriangles"){n1="";
      while((ch<=' '||ch==':')&&cp<FLMT){file->read(&ch,1);cp++;}s32 triscnt=0;
      f32 ml=1;while(IASCII::num(ch)&&cp<FLMT){triscnt=s32(triscnt*ml+(ch-48));
       if(triscnt){ml=10;}file->read(&ch,1);cp++;}
      if(triscnt==ntr){printf("\n>Skin Triangles %d",ntr);}
      else{printf("\n>Error...");return 0;}if(!MTriangs){return 0;}

      s32 vct=0;while(vct<ntr&&cp<FLMT){
       s32 v4[4]={0,0,0,0};s32 ct=0;
       while(ct<4&&cp<FLMT){s32 t=0;
        while(!IASCII::num(ch)&&ch!=' '&&cp<FLMT){
         file->read(&ch,1);cp++;}
        if(IASCII::num(ch)){ml=1;while(IASCII::num(ch)&&cp<FLMT){t=s32(t*ml+(ch-48));
         if(t){ml=10;}file->read(&ch,1);cp++;}}if(ch==','){file->read(&ch,1);cp++;}
        v4[ct]=t;ct++;file->read(&ch,1);cp++;}
       MTriangs[vct].textureIndices[0]=(v4[1]<=nsv?v4[1]:0);
       MTriangs[vct].textureIndices[1]=(v4[2]<=nsv?v4[2]:0);
       MTriangs[vct].textureIndices[2]=(v4[3]<=nsv?v4[3]:0);
      vct++;}}
//num frames
     while(n1!="frames"&&cp<FLMT){n1="";
      while(!IASCII::ltr(ch)&&cp<FLMT){file->read(&ch,1);cp++;}
      while(IASCII::ltr(ch)&&cp<FLMT){n1+=ch;file->read(&ch,1);cp++;}
      n1.make_lower();}
     if(n1=="frames"){n1="";
      while((ch<=' '||ch==':')&&cp<FLMT){file->read(&ch,1);cp++;}nfr=0;
      f32 ml=1;while(IASCII::num(ch)&&cp<FLMT){nfr=s32(nfr*ml+(ch-48));
       if(nfr){ml=10;}file->read(&ch,1);cp++;}
      printf("\n>Frames %d",nfr);}
//def
     nfr=nfr+!nfr;
     if(!nfr||!nvt){printf("Error...(frm==0||nvt==0)");return 0;}
     vertices=new core::array<v3f>[nfr];
     ASCIIVertex*vertx1=new ASCIIVertex[nvt+!nvt];
     printf("\n FRM0{:");i=0;while(i<nfr){s32 s=0;while(s<nvt){v3f v;v.X=vertx[s].vtx[0];
      v.Z=vertx[s].vtx[1];v.Y=vertx[s].vtx[2];vertices[i].push_back(v);s++;}i++;}printf(":}");
//def
//frame name
     if(nfr){
      while(n1!="frame"&&cp<FLMT){n1="";
       while(!IASCII::ltr(ch)&&cp<FLMT){file->read(&ch,1);cp++;}
       while(IASCII::ltr(ch)&&cp<FLMT){n1+=ch;file->read(&ch,1);cp++;}
       n1.make_lower();}
      s32 i=-1;redo:i++;s32 ii=0;printf(";");
      if(n1=="frame"){n1="";
       while((ch<=' '||ch==':')&&cp<FLMT){file->read(&ch,1);cp++;}s32 frn=0;
       f32 ml=1;while(IASCII::num(ch)&&cp<FLMT){frn=s32(frn*ml+(ch-48));
        if(frn){ml=10;}file->read(&ch,1);cp++;}//printf("\n>Frame %d",frn);
       //name
       while(!IASCII::ltr(ch)&&cp<FLMT){file->read(&ch,1);cp++;}
       while((IASCII::ltr(ch)||IASCII::num(ch)||ch==' ')&&cp<FLMT){
        n1+=ch;file->read(&ch,1);cp++;}n1.make_lower();//printf(" %s",n1.c_str());
        c8 name[10];s32 o0=0;while(o0<10){name[o0]=0;
         if(o0<n1.size()){name[o0]=n1.c_str()[o0];}o0++;}
        // store frame data
        SFDatax fdata;fdata.start=i;fdata.end=i;fdata.fps=1;
        if(name[0]){
         s32 s=0;while(s<10&&!!name[s]&&name[s]!=' '&&(name[s]<'0'||
          name[s]>'9'||name[s+1]>=58&&s<=8)){
          fdata.name+=name[s];fdata.name.make_lower();s++;}
         if(!FData.empty()&&FData[FData.size()-1].name==fdata.name){++FData[FData.size()-1].end;}
         else{FData.push_back(fdata);}
        }
       while(!IASCII::ltr(ch)&&!IASCII::num(ch)&&cp<FLMT){file->read(&ch,1);cp++;}n1="";
       if(IASCII::ltr(ch)){
        while(IASCII::ltr(ch)&&cp<FLMT){n1+=ch;file->read(&ch,1);cp++;}
        n1.make_lower();
        if(n1=="frame"){
         s32 s=0;while(s<nvt){v3f v;
          v.X=vertx[s].vtx[0];v.Z=vertx[s].vtx[1];v.Y=vertx[s].vtx[2];
          vertices[i].pointer()[s]=v;s++;}
         goto redo;}
        if(n1=="pointsanimation"){n1="";f32 ml=0;s32 t=0,t0=0;
         while((ch<=' '||ch==':')&&cp<FLMT){file->read(&ch,1);cp++;}
         ml=1;t=0;while(IASCII::num(ch)&&cp<FLMT){t=s32(t*ml+(ch-48));
          if(t){ml=10;}file->read(&ch,1);cp++;}t0=t;
         while((ch<=' '||ch==':')&&cp<FLMT){file->read(&ch,1);cp++;}
         ml=1;t=0;while(IASCII::num(ch)&&cp<FLMT){t=s32(t*ml+(ch-48));
          if(t){ml=10;}file->read(&ch,1);cp++;}
         printf(" > %d %d(frn %d:%d)",t0,t,frn,nfr);
         //copy vertices
         while(ii<nvt){
          while((ch<=' '||ch==':')&&cp<FLMT){file->read(&ch,1);cp++;}
          ml=1;t=0;while(IASCII::num(ch)&&cp<FLMT){t=s32(t*ml+(ch-48));
           if(t){ml=10;}file->read(&ch,1);cp++;}
          while(ii<t){vertx1[ii].vtx[0]=vertx[ii].vtx[0];
           vertx1[ii].vtx[1]=vertx[ii].vtx[1];
           vertx1[ii].vtx[2]=vertx[ii].vtx[2];ii++;}
          s32 vct=ii;f32 v3[3]={0,0,0};s32 ct=0;
          if(ch==','){file->read(&ch,1);cp++;if(ch==' '){file->read(&ch,1);cp++;}}
          while(ct<3&&cp<FLMT){f32 t=0;s32 s=1;
           while(!IASCII::num(ch)&&ch!=' '&&ch!='.'&&ch!='-'&&cp<FLMT){n1+=ch;
            file->read(&ch,1);cp++;}
           if(ch=='-'){s=-1;file->read(&ch,1);cp++;}
           if(IASCII::num(ch)){ml=1;while(IASCII::num(ch)&&cp<FLMT){t=t*ml+(ch-48);
            if(t){ml=10;}file->read(&ch,1);cp++;}}
           if(ch=='.'){file->read(&ch,1);cp++;
            if(IASCII::num(ch)){ml=10;while(IASCII::num(ch)&&cp<FLMT){t+=(ch-48)/(ml+!ml);
             ml*=10;file->read(&ch,1);cp++;}}}if(ch==','){file->read(&ch,1);cp++;}
           v3[ct]=t*s;ct++;file->read(&ch,1);cp++;}
          vertx1[vct].vtx[0]=v3[0];vertx1[vct].vtx[1]=v3[1];vertx1[vct].vtx[2]=v3[2];
          ii++;}
         //copy vertices
         printf("\n svert{");s32 s=0;while(s<nvt){v3f v;
          v.X=vertx1[s].vtx[0];v.Z=vertx1[s].vtx[1];v.Y=vertx1[s].vtx[2];
          vertices[i].pointer()[s]=v;s++;}printf("}=f %d",i);
         //next
         if(frn<nfr){
          while(!IASCII::ltr(ch)&&!IASCII::num(ch)&&cp<FLMT){file->read(&ch,1);cp++;}n1="";
          if(IASCII::ltr(ch)){
           while(IASCII::ltr(ch)&&cp<FLMT){n1+=ch;file->read(&ch,1);cp++;}
           n1.make_lower();goto redo;}}
        }
       }
      }}
    if(!vertices||!nvt){printf("Error Vertices...");return 0;}
    if(!MTriangs||!ntr){printf("Error Indices...");return 0;}
    if(!MTCoords||!nsv){printf("Error Tcoords...");delete[]MTCoords;
     nsv=1;MTCoords=new STCoordx[nsv];MTCoords[0].s=0;MTCoords[0].t=0;}
    i=0;while(i<ntr){Trisx(MTriangs,MTriangs,i);i++;}
   }//endwh


//milkshape ascii
    file->seek(0);cp=0;while(cp<FLMT&&FTYPE==2){file->read(&ch,1);cp++;
//num frames
     while(n1!="frames"&&cp<FLMT){n1="";
      while(!IASCII::ltr(ch)&&cp<FLMT){file->read(&ch,1);cp++;}
      while(IASCII::ltr(ch)&&cp<FLMT){n1+=ch;file->read(&ch,1);cp++;}
      n1.make_lower();}
     if(n1=="frames"){n1="";
      while((ch<=' '||ch==':')&&cp<FLMT){file->read(&ch,1);cp++;}s32 o0=0;
      f32 ml=1;while(IASCII::num(ch)&&cp<FLMT){o0=s32(o0*ml+(ch-48));
       if(o0){ml=10;}file->read(&ch,1);cp++;}
      printf("\n>Frames S %d",o0);}
     while(n1!="frame"&&cp<FLMT){n1="";
      while(!IASCII::ltr(ch)&&cp<FLMT){file->read(&ch,1);cp++;}
      while(IASCII::ltr(ch)&&cp<FLMT){n1+=ch;file->read(&ch,1);cp++;}
      n1.make_lower();}
     if(n1=="frame"){n1="";
      while((ch<=' '||ch==':')&&cp<FLMT){file->read(&ch,1);cp++;}nfr=0;
      f32 ml=1;while(IASCII::num(ch)&&cp<FLMT){nfr=s32(nfr*ml+(ch-48));
       if(nfr){ml=10;}file->read(&ch,1);cp++;}
      printf("\n>Frames %d",nfr);}
//meshes vertices
     s32 normn=0,trind=0;
     while(n1!="meshes"&&cp<FLMT){n1="";
      while(!IASCII::ltr(ch)&&cp<FLMT){file->read(&ch,1);cp++;}
      while(IASCII::ltr(ch)&&cp<FLMT){n1+=ch;file->read(&ch,1);cp++;}
      n1.make_lower();}
     if(n1=="meshes"){n1="";s32 o0=0;
      while(o0<2&&cp<FLMT){file->read(&ch,1);if(ch==13){file->read(&ch,1);
       if(ch==10){file->read(&ch,1);o0++;}}cp++;}nvt=0;
      f32 ml=1;while(IASCII::num(ch)&&cp<FLMT){nvt=s32(nvt*ml+(ch-48));
       if(nvt){ml=10;}file->read(&ch,1);cp++;}
      if(nvt>ASCII_MVERTS){return 0;}
      vertx=new ASCIIVertex[nvt+!nvt];printf("\n>Vertices %d",nvt);
      nsv=nvt;MTCoords=new STCoordx[nsv+!nsv];printf("\n>TCoords %d",nsv);

      s32 vct=0;while(vct<nvt&&cp<FLMT){
       f32 v3[7]={0,0,0};s32 ct=0;
       while(ct<7&&cp<FLMT){f32 t=0;s32 s=1;
        while(!IASCII::num(ch)&&ch!=' '&&ch!='.'&&ch!='-'&&cp<FLMT){n1+=ch;
         file->read(&ch,1);cp++;}
        if(ch=='-'){s=-1;file->read(&ch,1);cp++;}
        if(IASCII::num(ch)){ml=1;while(IASCII::num(ch)&&cp<FLMT){t=t*ml+(ch-48);
         if(t){ml=10;}file->read(&ch,1);cp++;}}
        if(ch=='.'){file->read(&ch,1);cp++;
         if(IASCII::num(ch)){ml=10;while(IASCII::num(ch)&&cp<FLMT){t+=(ch-48)/(ml+!ml);
          ml*=10;file->read(&ch,1);cp++;}}}
        v3[ct]=t*s;ct++;file->read(&ch,1);cp++;}
       vertx[vct].vtx[0]=v3[3];vertx[vct].vtx[1]=v3[1];vertx[vct].vtx[2]=v3[2];
       MTCoords[vct].s=v3[4];MTCoords[vct].t=v3[5];
      vct++;}

      s32 ct=0,c1=1;while(ct<c1&&cp<FLMT){f32 t=0;s32 s=1;
       while(!IASCII::num(ch)&&ch!=' '&&ch!='.'&&ch!='-'&&cp<FLMT){n1+=ch;
        file->read(&ch,1);cp++;}
       if(ch=='-'){s=-1;file->read(&ch,1);cp++;}
       if(IASCII::num(ch)){ml=1;while(IASCII::num(ch)&&cp<FLMT){t=t*ml+(ch-48);
        if(t){ml=10;}file->read(&ch,1);cp++;}}
       if(ch=='.'){file->read(&ch,1);cp++;
        if(IASCII::num(ch)){ml=10;while(IASCII::num(ch)&&cp<FLMT){t+=(ch-48)/(ml+!ml);
         ml*=10;file->read(&ch,1);cp++;}}}
       normn=s32(t*s);file->read(&ch,1);cp++;ct++;
       if(ct==1){c1=normn*3+2;printf("\n norms num %d :",normn);}
       if(ct==c1){trind=normn;}}
     }//end if mesh

     if(trind){
//triangles vertices index
      ntr=trind;
      if(ntr>ASCII_MTRIS){return 0;}
      MTriangs=new STriangx[ntr+!ntr];
      printf("\n>Triangles %d",ntr);
      s32 vct=0;while(vct<ntr&&cp<FLMT){
       s32 v3[8]={0,0,0};s32 ct=0;f32 ml=1;
       while(ct<8&&cp<FLMT){s32 t=0;
        while(!IASCII::num(ch)&&ch!=' '&&cp<FLMT){n1+=ch;
         file->read(&ch,1);cp++;}
        if(IASCII::num(ch)){ml=1;while(IASCII::num(ch)&&cp<FLMT){t=s32(t*ml+(ch-48));
         if(t){ml=10;}file->read(&ch,1);cp++;}}
        v3[ct]=t;ct++;file->read(&ch,1);cp++;}
       MTriangs[vct].vertexIndices[0]=v3[1]<=nvt?v3[1]:0;
       MTriangs[vct].vertexIndices[1]=v3[3]<=nvt?v3[3]:0;
       MTriangs[vct].vertexIndices[2]=v3[2]<=nvt?v3[2]:0;
       MTriangs[vct].textureIndices[0]=v3[1]<=nsv?v3[1]:0;
       MTriangs[vct].textureIndices[1]=v3[3]<=nsv?v3[3]:0;
       MTriangs[vct].textureIndices[2]=v3[2]<=nsv?v3[2]:0;
      vct++;}
     }//end if trind

//sk
      video::IVideoDriver*driver=SceneManager->getVideoDriver();
      IntpBfr->Material.MaterialType=video::EMT_SOLID;
      video::SColor color;s32 numsk=0,skbx=0;
      skbm[0]=0;skbm[1]=0;M_BFR=new u32[(M_MSZ+1)*M_MSZ];nsk=1;skw=8;skh=8;
      f32 o0=0,o1=0,x0=0,x1=0,y0=0,y1=0;s32 o=0,c=M_MSZ*M_MSZ<skw*skh?M_MSZ*M_MSZ:skw*skh,i=0;
      o=0;while(o<c){M_BFR[o]=video::SColor(255,255,255,255).color;o++;}
      video::IImage*skbn=driver->createImageFromData(video::ECF_A8R8G8B8,core::
       dimension2d<s32>(skw,skh),M_BFR,0,1);
      if(numsk<=0){skbm[0]=skbn;
       IntpBfr->Material.setTexture(0,driver->addTexture("s0.bmp",skbm[0]));
       printf("\n..Skin0Form...%dx%dx%d.",skw,skh,skbx);}
      delete M_BFR;printf("..Skin.OK");
//sk
//frmdt
     if(1){n1="Frame0";
        c8 name[10];s32 o0=0;while(o0<10){name[o0]=0;
         if(o0<n1.size()){name[o0]=n1.c_str()[o0];}o0++;}
        // store frame data
        SFDatax fdata;fdata.start=i;fdata.end=i;fdata.fps=1;
        if(name[0]){
         s32 s=0;while(s<10&&!!name[s]&&name[s]!=' '&&(name[s]<'0'||
          name[s]>'9'||name[s+1]>=58&&s<=8)){
          fdata.name+=name[s];fdata.name.make_lower();s++;}
         if(!FData.empty()&&FData[FData.size()-1].name==fdata.name){++FData[FData.size()-1].end;}
         else{FData.push_back(fdata);}
        }}
//frmdt

//def
     nfr=nfr+!nfr;
     if(!nfr||!nvt){printf("Error...(frm==0||nvt==0)");return 0;}
     vertices=new core::array<v3f>[nfr];
     printf("\n FRM0{:");s32 s=0;while(s<nvt){v3f v;v.X=vertx[s].vtx[0];
      v.Z=vertx[s].vtx[1];v.Y=vertx[s].vtx[2];vertices[0].push_back(v);s++;}printf(":}");
//def

    if(!vertices||!nvt){printf("Error Vertices...");return 0;}
    if(!MTriangs||!ntr){printf("Error Indices...");return 0;}
    if(!MTCoords||!nsv){printf("Error Tcoords...");delete[]MTCoords;
     nsv=1;MTCoords=new STCoordx[nsv];MTCoords[0].s=0;MTCoords[0].t=0;}
    i=0;while(i<ntr){Trisx(MTriangs,MTriangs,i);i++;}
   }//endwh

   if(vertx){delete vertx;}if(vertx1){delete vertx1;}
printf("\n..ident....");
printf("\nversi....");
printf("\nscae0._");printf(" 1._");
 printf(" 2._");
printf("\nscao0._");printf(" 1._");
 printf(" 2._");

printf("\n..frsiz....");
printf("\noftec....");
printf("\nnull1....");
printf("\nnull2....");

printf("\n.nskis...%d.",nsk);
printf("\nskiwi...%d.",0);printf(" skwx...%d.",0);
printf("\nskihe...%d.",0);printf(" skhx...%d.",0);
printf("\nnveti...%d.",nvt);

printf("\n.ntria...%d.",ntr);
printf("\nnfrms...%d.",nfr);
printf("\nnskve...%d.",nsv);
printf("\nnull3....");

printf("\n.null4....");
printf("\nskbpt...%d.",0);
printf("\nskiwi...%d.",0);
printf("\nskihe...%d.\n",0);
/////////////////
     VertList=new core::array<v3f>[nfr];

     i=0;while(i<nfr){

     // add vertices
     s32 s=0;while(s<nvt){v3f v=v3f(0,0,0);
      v=vertices[i].pointer()[s];
      VertList[i].push_back(v);s++;}

     i++;}delete vertices;printf("..Vertices.OK\n\n");
    return createMesh(VertList,FData,MTCoords,MTriangs,nvt,ntr,nfr,nsv,skbm[0]);
}


bool IASCIIMesh::createMesh(core::array<v3f>*VertListx,
    const core::array<SFDatax> &FDatax,STCoordx*MTCoordsx,STriangx*MTriangsx,
    const s32 &nvtx,const s32 &ntrx,const u32 &nfrx,const s32 &nsvx,video::IImage*im){
    if(!VertListx||!MTCoordsx||!MTriangsx||nvtx<=0||ntrx<=0||nsvx<=0||!im||
     nvtx>ASCII_MVERTS||ntrx>ASCII_MTRIS){printf("\nError ...");return 0;}
    const u16 M_MSZ=256;s32 i=0;setSkin(0,im);
    IntpBfr->Material.Lighting=1;IntpBfr->Material.ZWriteEnable=1;
    IntpBfr->Material.Wireframe=0;IntpBfr->Material.BackfaceCulling=1;
    IntpBfr->Material.GouraudShading=1;IntpBfr->Material.FogEnable=1;
    IntpBfr->Material.NormalizeNormals=1;IntpBfr->Material.ZBuffer=1;
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

    else{
    // create normals slow
    printf("\n creating normals<");const s32 tr3=ntr*3;
    if(NormList){delete NormList;}NormList=new core::array<v3f>[nfr];
    i=0;while(i<nfr){NormList[i].set_used(nvt);i++;}
    s32 vi[tr3];i=0;while(i<ntr){s32 o0=0;while(o0<3){vi[i*3+o0]=
     MTriangs[i].vertexIndices[o0];o0++;}i++;}
    v3f*vnlst=new v3f[nfr*ntr];core::triangle3df tn;
    i=0;while(i<nfr){s32 o0=0,o1=0;v3f v3[3];
     o0=0;while(o0<ntr){
      o1=0;while(o1<3){v3[o1]=VertList[i].pointer()[vi[o0*3+o1]];o1++;}
      tn=core::triangle3df(v3[0],v3[1],v3[2]);vnlst[i*ntr+o0]=tn.getNormal();
     o0++;}i++;}printf(":");
    i=0;while(i<nfr){printf(".");
     s32 o0=0,o1=0,o2=0,acn=0,arr[tr3];
     while(o0<nvt){acn=0;v3f vn=v3f(0,0,0);
      o1=0;while(o1<ntr){
       o2=0;while(o2<3){if(o0==vi[o1*3+o2]){break;}o2++;
        if(o2>2){o1++;if(o1<ntr){o2=0;continue;}break;}}
       if(o2>2){break;}o2+=o1*3;
       if(acn<tr3){arr[acn]=o2*(o2>=0);acn++;}vn+=vnlst[i*ntr+o1];
       o1++;}vn/=f32(acn+!acn);
      o1=0;while(o1<acn){NormList[i].pointer()[o0]=vn;o1++;}
      o0++;}
     i++;}delete vnlst;printf(":>\n");
    }

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
     v2f v;count/=3;
     s32 n=0;while(n<count){
      s32 i=0;while(i<3){
       v.X=MTCoords[MTriangs[n].textureIndices[i]].s;
       v.Y=MTCoords[MTriangs[n].textureIndices[i]].t;
       IntpBfr->Vertices[n*3+i].TCoords=v;
       IntpBfr->Vertices[n*3+i].Color=video::SColor(255,255,255,255);
       IntpBfr->Vertices[n*3+i].Normal=v3f(0,-0.1f,0);
      i++;}
     n++;}}
    calculateBoundingBox();return 1;
}


///md2
    struct SMDL3Vertex0
    {
        u8    vtx[3];
        u8    nmid;
    };
    struct IDP2_FRAME0
    {
        f32   scale3[3];
        f32   scalo3[3];
        c8    name[16];
        SMDL3Vertex0 vts[1];
    };

bool IASCIIMesh::createMesh(IAnimatedMesh*im){E_ANIMATED_MESH_TYPE mtp=im->getMeshType();
    if(!SceneManager||!im){printf("\nError creating mesh..\n");return 0;}s32 i=0;
    const u16 M_MSZ=256;u32*M_BFR=0;

    if(mtp==(E_ANIMATED_MESH_TYPE)88){
     IM8MMesh*m=(IM8MMesh*)im;
     core::array<v3f>*VertListx=m->getVertList();
     IM8MMesh::STCoordx*MTCoordsx=m->getMTCoords();
     IM8MMesh::STriangx*MTriangsx=m->getMTriangs();
     if(!MTCoordsx||!MTriangsx||!VertListx||!FData.const_pointer()){return 0;}
     ntr=m->getntr();nvt=m->getnvt();nsv=m->getnsv();
     nfr=m->getFrameCount()/4;if(MTCoords){delete MTCoords;}MTCoords=new STCoordx[nsv];
     i=0;while(i<nsv){MTCoords[i].s=(MTCoordsx[i].s+0.5f)/2048.0f;
      MTCoords[i].t=(MTCoordsx[i].t+0.5f)/2048.0f;i++;}
     if(MTriangs){delete MTriangs;}MTriangs=new STriangx[ntr];
     i=0;while(i<ntr){
      MTriangs[i].vertexIndices[0]=MTriangsx[i].vertexIndices[0];
      MTriangs[i].vertexIndices[1]=MTriangsx[i].vertexIndices[1];
      MTriangs[i].vertexIndices[2]=MTriangsx[i].vertexIndices[2];
      MTriangs[i].textureIndices[0]=MTriangsx[i].textureIndices[0];
      MTriangs[i].textureIndices[1]=MTriangsx[i].textureIndices[1];
      MTriangs[i].textureIndices[2]=MTriangsx[i].textureIndices[2];
     i++;}SFDatax fdata;
     i=0;while(i<m->getFData().size()){fdata.name=m->getFData()[i].name;
      fdata.start=m->getFData()[i].start;fdata.end=m->getFData()[i].end;
      fdata.fps=m->getFData()[i].fps;FData.push_back(fdata);i++;}
     i=createMesh(VertListx,FData,MTCoords,MTriangs,nvt,ntr,nfr,nsv,m->getSkin(0));
     return i;}//m8m_to_ascii

    if(mtp==EAMT_MD2){
     scene::IMeshCache*mc=SceneManager->getMeshCache();
     core::stringw str=mc->getMeshFilename(im);printf("\nmd2 %ls",str.c_str());
     core::stringc fnm=str.c_str();
    IM8M::CMRWFile*file=file->createMRWFile(fnm.c_str(),1);
    if(!file){printf("\nError reading file..");return 0;}
     c8 ident[4];file->read(&ident,sizeof(ident));core::stringc n1(ident);n1[4]=0;
     printf("\ntype %s",n1.c_str());if(n1!="IDP2"){file->drop();return 0;}s32 o0=0;
     file->read(&o0,sizeof(o0));file->read(&skw,sizeof(skw));file->read(&skh,sizeof(skh));
     s32 sskw=skw,sskh=skh;file->seek(24);file->read(&nvt,sizeof(nvt));printf("\nnvt %d",nvt);
     file->read(&nsv,sizeof(nsv));printf("\nnsv %d",nsv);
     file->read(&ntr,sizeof(ntr));
     printf("\ntris %d",ntr);file->read(&o0,sizeof(o0));
     file->read(&nfr,sizeof(nfr));printf("\nfrm %d",nfr);
     file->read(&o0,sizeof(o0));printf("\nofssk %d",o0);
     file->read(&o0,sizeof(o0));printf("\nofstex %d",o0);s32 ofstex=o0;
     file->read(&o0,sizeof(o0));printf("\nofstri %d",o0);s32 ofstri=o0;
     file->read(&o0,sizeof(o0));printf("\nofsfrm %d",o0);s32 ofsfrm=o0;
     file->read(&o0,sizeof(o0));file->read(&o0,sizeof(o0));
     c8 TEXNAM[64];file->read(&TEXNAM,sizeof(TEXNAM));n1=TEXNAM;printf("\ntex %s",n1.c_str());
     if(1){
      video::IVideoDriver*driver=SceneManager->getVideoDriver();
      IntpBfr->Material.MaterialType=video::EMT_SOLID;
      video::SColor color;s32 numsk=0,skbx=0;
      skbm[0]=0;skbm[1]=0;M_BFR=new u32[(M_MSZ+1)*M_MSZ];nsk=1;skw=8;skh=8;
      f32 o0=0,o1=0,x0=0,x1=0,y0=0,y1=0;s32 o=0,c=M_MSZ*M_MSZ<skw*skh?M_MSZ*M_MSZ:skw*skh,i=0;
       o=0;while(o<c){M_BFR[o]=video::SColor(255,255,255,255).color;o++;}
      video::IImage*skbn=driver->createImageFromData(video::ECF_A8R8G8B8,core::
       dimension2d<s32>(skw,skh),M_BFR,0,1);
      skbm[0]=skbn;IntpBfr->Material.setTexture(0,driver->addTexture("s0.bmp",skbm[0]));
      printf("\n..Skin0Form...%dx%dx%d.",skw,skh,skbx);
      delete M_BFR;printf("..Skin.OK");
     }

     nfr+=!nfr;nsk+=!nsk;nsv+=!nsv;ntr+=!ntr;nvt+=!nvt;
     VertList=new core::array<v3f>[nfr];s32 i=0;
    // read TextureCoords
    file->seek(ofstex);
    MTCoords=new STCoordx[nsv];
    IM8MMesh::STCoordx*MTCoordsx=new IM8MMesh::STCoordx[nsv];
    if(!file->read(MTCoordsx,sizeof(IM8MMesh::STCoordx)*nsv)){
     printf(" Error reading TextureCoords.");return 0;}
    printf("..TCoords.OK");
    i=0;while(i<nsv){
     MTCoords[i].s=MTCoordsx[i].s/f32(sskw+!sskw);
     MTCoords[i].t=MTCoordsx[i].t/f32(sskh+!sskh);
    i++;}delete MTCoordsx;
    // read Triangles
    MTriangs=new STriangx[ntr];
    if(file->read(MTriangs,sizeof(STriangx)*ntr)){
     i=0;while(i<ntr){Trisx(MTriangs,MTriangs,i);i++;}}
    else{printf(" Error reading triangles.");return 0;}
    printf("..Triangles.OK");
    // read Vertices
    IDP2_FRAME0*frame=(IDP2_FRAME0*)new u8[ASCII_MVERTS*5+40];

    s32 framsiz=nvt*4+40;
    i=0;while(i<nfr){
     // read vertices
     file->read(frame,framsiz);
     f32 scl[3];scl[0]=frame->scale3[0];scl[1]=frame->scale3[1];scl[2]=frame->scale3[2];
     f32 sco[3];sco[0]=frame->scalo3[0];sco[1]=frame->scalo3[1];sco[2]=frame->scalo3[2];

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

     i++;}delete frame;printf("..Vertices.OK\n\n");file->drop();
    i=createMesh(VertList,FData,MTCoords,MTriangs,nvt,ntr,nfr,nsv,skbm[0]);
    return i;}//md2_to_ascii
    printf(" endc");return 0;}


//! calculates the bounding box
void IASCIIMesh::calculateBoundingBox()
{
    IntpBfr->BoundingBox.reset(0,0,0);
    if(VertList[0].size()){s32 i=0;while(i<VertList[0].size()){
      IntpBfr->BoundingBox.addInternalPoint(VertList[0].pointer()[i]);i++;}
    }
}


//! sets a flag of all contained materials to a new value
void IASCIIMesh::setMaterialFlag(video::E_MATERIAL_FLAG flag,bool newvalue)
{
    IntpBfr->Material.setFlag(flag,newvalue);
}


//! returns an axis aligned bounding box
const core::aabbox3d<f32>&IASCIIMesh::getBoundingBox() const
{
    return IntpBfr->BoundingBox;
}


//! set user axis aligned bounding box
void IASCIIMesh::setBoundingBox(const core::aabbox3df& box)
{
    IntpBfr->BoundingBox=box;
}


//! Returns the type of the animated mesh.
E_ANIMATED_MESH_TYPE IASCIIMesh::getMeshType()const
{
    return (E_ANIMATED_MESH_TYPE)91;
}

//! Returns frame loop data for a special ASCII animation type.
bool IASCIIMesh::getFrameLoop(const c8*name,s32 &outStart,s32 &outEnd,s32 &outFPS)const{
    s32 i=0;while(i<FData.size()){
     if(FData[i].name==name){
      outStart=FData[i].start<<ASCII_FRAME2;
      outEnd=FData[i].end<<ASCII_FRAME2;
      outEnd+=!ASCII_FRAME2?1:(1<<ASCII_FRAME2)-1;
      outFPS=FData[i].fps<<ASCII_FRAME2;
      return 1;}i++;}
    return 0;
}


//! Returns amount of ASCII animations in this file.
u32 IASCIIMesh::getAnimationCount()const
{
    return FData.size();
}


//! Returns name of ASCII animation.
const c8*IASCIIMesh::getAnimationName(u32 nr)const
{
    if(nr>=(u16)FData.size()){return 0;}
    return FData[nr].name.c_str();
}


video::ITexture*IASCIIMesh::getTexture(bool nr)const
{
    if(IntpBfr->Material.getTexture(1)&&nr&&nsk>1){return IntpBfr->Material.getTexture(1);}
    return IntpBfr->Material.getTexture(0);
}


video::IImage*IASCIIMesh::getSkin(bool nr)const
{
    if(skbm[1]&&nr){return skbm[1];}
    return skbm[0];
}


void IASCIIMesh::setSkin(bool nr,video::IImage*im)
{
    video::IVideoDriver*driver=SceneManager->getVideoDriver();nr*=!!skbm[0];
    if(!nr&&im&&im!=skbm[0]){delete skbm[0];skbm[0]=0;skbm[0]=driver->createImageFromData(
     video::ECF_A8R8G8B8,im->getDimension(),im->lock(),0,1);im->unlock();
     IntpBfr->Material.setTexture(0,driver->addTexture("s",skbm[0]));}
    if(nr&&im!=skbm[1]){delete skbm[1];skbm[1]=0;if(im){skbm[1]=driver->createImageFromData(
     video::ECF_A8R8G8B8,im->getDimension(),im->lock(),0,1);im->unlock();
     IntpBfr->Material.setTexture(1,driver->addTexture("s",skbm[1]));}}
}


bool IASCIIMesh::WriteASCII(const c8*fname){
    //write ASCII header
    const u16 M_MSZ=256;
    if(!SceneManager||nvt>ASCII_MVERTS||ntr>ASCII_MTRIS){return 0;}

    core::stringc fnm,src=fname,fnmx;
    fnm=src.subString(0,src.findLast('.')<0?src.size():src.findLast('.'));
    fnmx=fnm;fnm+=".txt";
    IM8M::CMRWFile*file=file->createMRWFile(fnm.c_str(),0);
    if(!file){printf("\nError writing file..");return 0;}
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
/////writ
    core::stringc str="";s32 i=0,fr=0;
    str="Skins: 1;\n";file->write(str.c_str(),str.size());
    str="\"Skin0\";\n\n";file->write(str.c_str(),str.size());
    str="Groups: 1;\n";file->write(str.c_str(),str.size());

    str="Group: 0 {\n";
    file->write(str.c_str(),str.size());
    //1-vertlist
    str="Vertices: ";str+=nvt;str+=";\n";
    file->write(str.c_str(),str.size());
    i=0;while(i<nvt){
     vtm=VertList[0].pointer()[i];
     str="";str+=vtm.X;str+=" ";str+=vtm.Z;str+=" ";str+=vtm.Y;str+=";";str+="\n";
     file->write(str.c_str(),str.size());i++;}file->write("\n",1);
    //triang
    str="Triangles: ";str+=ntr;str+=";\n";
    file->write(str.c_str(),str.size());
    i=0;while(i<ntr){
     str="";str+=MTriangs[i].vertexIndices[0];str+=" ";
     str+=MTriangs[i].vertexIndices[1];str+=" ";
     str+=MTriangs[i].vertexIndices[2];str+=";";str+="\n";
     file->write(str.c_str(),str.size());i++;}file->write("\n",1);
//////write tex
    video::IVideoDriver*driver=SceneManager->getVideoDriver();core::stringc tn=fnmx;
    tn+=".bmp";if(skbm[0]){driver->writeImageToFile(skbm[0],tn.c_str());}
    //skin
    str="Skin: \"Skin0\" ";str+=ntr;str+=";\n";file->write(str.c_str(),str.size());
    i=0;while(i<ntr){str="";str+=i;str+=";\n";file->write(str.c_str(),str.size());i++;}
    file->write("\n",1);
    //skinvert
    str="SkinPoints: ";str+=nsv;str+=";\n";file->write(str.c_str(),str.size());
    i=0;while(i<nsv){
     str="";str+=MTCoords[i].s;str+=" ";
     str+=MTCoords[i].t;str+=";";str+="\n";
     file->write(str.c_str(),str.size());i++;}file->write("\n",1);
    //skintriang
    str="SkinTriangles: ";str+=ntr;str+=";\n";file->write(str.c_str(),str.size());
    i=0;while(i<ntr){str="";str+=i;str+=", ";
     str+=MTriangs[i].textureIndices[0];str+=" ";
     str+=MTriangs[i].textureIndices[1];str+=" ";
     str+=MTriangs[i].textureIndices[2];str+=";";str+="\n";
     file->write(str.c_str(),str.size());i++;}
    str="};\n\n";file->write(str.c_str(),str.size());
    //fram
    str="Frames: ";str+=nfr;str+=";\n";file->write(str.c_str(),str.size());
    s32 fn=0,fnx=0;c8 name[10];
    fr=0;while(fr<nfr){s32 fc=fr;
    //fnm
    if(fr>FData[fn].end){fn++;fnx=0;}else{fnx=!!fr*(fnx+1)%10;}
    i=0;while(i<10){
     if(i<FData[fn].name.size()){name[i]=FData[fn].name[i];}
     else{if(fc==fr&&!!fnx){fc++;name[i]=(fnx+48);}else{name[i]=0;}}i++;}
    //fnm
    str="Frame: ";str+=fr;str+=" ";str+=0;str+=" ";str+="\"";str+=name;str+="\"\n";
    file->write(str.c_str(),str.size());
    str="PointsAnimation: ";str+=0;str+=" ";str+=nvt;str+="\n";
    file->write(str.c_str(),str.size());

    i=0;while(i<nvt){str="";str+=i;str+=", ";
     vtm=VertList[fr].pointer()[i];
     str+=vtm.X;str+=" ";str+=vtm.Z;str+=" ";str+=vtm.Y;str+=";";str+="\n";
     file->write(str.c_str(),str.size());i++;}

    fr++;}file->write("\n",1);

    file->drop();
    return 1;
}


const u16 NormTbsize=162;// md2 normal list
static const f32 NormTb[NormTbsize][3]={
	{-0.525731f, 0.000000f, 0.850651f},{-0.442863f, 0.238856f, 0.864188f},
	{-0.295242f, 0.000000f, 0.955423f},{-0.309017f, 0.500000f, 0.809017f},
	{-0.162460f, 0.262866f, 0.951056f},{0.000000f, 0.000000f, 1.000000f},
	{0.000000f, 0.850651f, 0.525731f},{-0.147621f, 0.716567f, 0.681718f},
	{0.147621f, 0.716567f, 0.681718f},{0.000000f, 0.525731f, 0.850651f},
	{0.309017f, 0.500000f, 0.809017f},{0.525731f, 0.000000f, 0.850651f},
	{0.295242f, 0.000000f, 0.955423f},{0.442863f, 0.238856f, 0.864188f},
	{0.162460f, 0.262866f, 0.951056f},{-0.681718f, 0.147621f, 0.716567f},
	{-0.809017f, 0.309017f, 0.500000f},{-0.587785f, 0.425325f, 0.688191f},
	{-0.850651f, 0.525731f, 0.000000f},{-0.864188f, 0.442863f, 0.238856f},
	{-0.716567f, 0.681718f, 0.147621f},{-0.688191f, 0.587785f, 0.425325f},
	{-0.500000f, 0.809017f, 0.309017f},{-0.238856f, 0.864188f, 0.442863f},
	{-0.425325f, 0.688191f, 0.587785f},{-0.716567f, 0.681718f, -0.147621f},
	{-0.500000f, 0.809017f, -0.309017f},{-0.525731f, 0.850651f, 0.000000f},
	{0.000000f, 0.850651f, -0.525731f},{-0.238856f, 0.864188f, -0.442863f},
	{0.000000f, 0.955423f, -0.295242f},{-0.262866f, 0.951056f, -0.162460f},
	{0.000000f, 1.000000f, 0.000000f},{0.000000f, 0.955423f, 0.295242f},
	{-0.262866f, 0.951056f, 0.162460f},{0.238856f, 0.864188f, 0.442863f},
	{0.262866f, 0.951056f, 0.162460f},{0.500000f, 0.809017f, 0.309017f},
	{0.238856f, 0.864188f, -0.442863f},{0.262866f, 0.951056f, -0.162460f},
	{0.500000f, 0.809017f, -0.309017f},{0.850651f, 0.525731f, 0.000000f},
	{0.716567f, 0.681718f, 0.147621f},{0.716567f, 0.681718f, -0.147621f},
	{0.525731f, 0.850651f, 0.000000f},{0.425325f, 0.688191f, 0.587785f},
	{0.864188f, 0.442863f, 0.238856f},{0.688191f, 0.587785f, 0.425325f},
	{0.809017f, 0.309017f, 0.500000f},{0.681718f, 0.147621f, 0.716567f},
	{0.587785f, 0.425325f, 0.688191f},{0.955423f, 0.295242f, 0.000000f},
	{1.000000f, 0.000000f, 0.000000f},{0.951056f, 0.162460f, 0.262866f},
	{0.850651f, -0.525731f, 0.000000f},{0.955423f, -0.295242f, 0.000000f},
	{0.864188f, -0.442863f, 0.238856f},{0.951056f, -0.162460f, 0.262866f},
	{0.809017f, -0.309017f, 0.500000f},{0.681718f, -0.147621f, 0.716567f},
	{0.850651f, 0.000000f, 0.525731f},{0.864188f, 0.442863f, -0.238856f},
	{0.809017f, 0.309017f, -0.500000f},{0.951056f, 0.162460f, -0.262866f},
	{0.525731f, 0.000000f, -0.850651f},{0.681718f, 0.147621f, -0.716567f},
	{0.681718f, -0.147621f, -0.716567f},{0.850651f, 0.000000f, -0.525731f},
	{0.809017f, -0.309017f, -0.500000f},{0.864188f, -0.442863f, -0.238856f},
	{0.951056f, -0.162460f, -0.262866f},{0.147621f, 0.716567f, -0.681718f},
	{0.309017f, 0.500000f, -0.809017f},{0.425325f, 0.688191f, -0.587785f},
	{0.442863f, 0.238856f, -0.864188f},{0.587785f, 0.425325f, -0.688191f},
	{0.688191f, 0.587785f, -0.425325f},{-0.147621f, 0.716567f, -0.681718f},
	{-0.309017f, 0.500000f, -0.809017f},{0.000000f, 0.525731f, -0.850651f},
	{-0.525731f, 0.000000f, -0.850651f},{-0.442863f, 0.238856f, -0.864188f},
	{-0.295242f, 0.000000f, -0.955423f},{-0.162460f, 0.262866f, -0.951056f},
	{0.000000f, 0.000000f, -1.000000f},{0.295242f, 0.000000f, -0.955423f},
	{0.162460f, 0.262866f, -0.951056f},{-0.442863f, -0.238856f, -0.864188f},
	{-0.309017f, -0.500000f, -0.809017f},{-0.162460f, -0.262866f, -0.951056f},
	{0.000000f, -0.850651f, -0.525731f},{-0.147621f, -0.716567f, -0.681718f},
	{0.147621f, -0.716567f, -0.681718f},{0.000000f, -0.525731f, -0.850651f},
	{0.309017f, -0.500000f, -0.809017f},{0.442863f, -0.238856f, -0.864188f},
	{0.162460f, -0.262866f, -0.951056f},{0.238856f, -0.864188f, -0.442863f},
	{0.500000f, -0.809017f, -0.309017f},{0.425325f, -0.688191f, -0.587785f},
	{0.716567f, -0.681718f, -0.147621f},{0.688191f, -0.587785f, -0.425325f},
	{0.587785f, -0.425325f, -0.688191f},{0.000000f, -0.955423f, -0.295242f},
	{0.000000f, -1.000000f, 0.000000f},{0.262866f, -0.951056f, -0.162460f},
	{0.000000f, -0.850651f, 0.525731f},{0.000000f, -0.955423f, 0.295242f},
	{0.238856f, -0.864188f, 0.442863f},{0.262866f, -0.951056f, 0.162460f},
	{0.500000f, -0.809017f, 0.309017f},{0.716567f, -0.681718f, 0.147621f},
	{0.525731f, -0.850651f, 0.000000f},{-0.238856f, -0.864188f, -0.442863f},
	{-0.500000f, -0.809017f, -0.309017f},{-0.262866f, -0.951056f, -0.162460f},
	{-0.850651f, -0.525731f, 0.000000f},{-0.716567f, -0.681718f, -0.147621f},
	{-0.716567f, -0.681718f, 0.147621f},{-0.525731f, -0.850651f, 0.000000f},
	{-0.500000f, -0.809017f, 0.309017f},{-0.238856f, -0.864188f, 0.442863f},
	{-0.262866f, -0.951056f, 0.162460f},{-0.864188f, -0.442863f, 0.238856f},
	{-0.809017f, -0.309017f, 0.500000f},{-0.688191f, -0.587785f, 0.425325f},
	{-0.681718f, -0.147621f, 0.716567f},{-0.442863f, -0.238856f, 0.864188f},
	{-0.587785f, -0.425325f, 0.688191f},{-0.309017f, -0.500000f, 0.809017f},
	{-0.147621f, -0.716567f, 0.681718f},{-0.425325f, -0.688191f, 0.587785f},
	{-0.162460f, -0.262866f, 0.951056f},{0.442863f, -0.238856f, 0.864188f},
	{0.162460f, -0.262866f, 0.951056f},{0.309017f, -0.500000f, 0.809017f},
	{0.147621f, -0.716567f, 0.681718f},{0.000000f, -0.525731f, 0.850651f},
	{0.425325f, -0.688191f, 0.587785f},{0.587785f, -0.425325f, 0.688191f},
	{0.688191f, -0.587785f, 0.425325f},{-0.955423f, 0.295242f, 0.000000f},
	{-0.951056f, 0.162460f, 0.262866f},{-1.000000f, 0.000000f, 0.000000f},
	{-0.850651f, 0.000000f, 0.525731f},{-0.955423f, -0.295242f, 0.000000f},
	{-0.951056f, -0.162460f, 0.262866f},{-0.864188f, 0.442863f, -0.238856f},
	{-0.951056f, 0.162460f, -0.262866f},{-0.809017f, 0.309017f, -0.500000f},
	{-0.864188f, -0.442863f, -0.238856f},{-0.951056f, -0.162460f, -0.262866f},
	{-0.809017f, -0.309017f, -0.500000f},{-0.681718f, 0.147621f, -0.716567f},
	{-0.681718f, -0.147621f, -0.716567f},{-0.850651f, 0.000000f, -0.525731f},
	{-0.688191f, 0.587785f, -0.425325f},{-0.587785f, 0.425325f, -0.688191f},
	{-0.425325f, 0.688191f, -0.587785f},{-0.425325f, -0.688191f, -0.587785f},
	{-0.587785f, -0.425325f, -0.688191f},{-0.688191f, -0.587785f, -0.425325f}};
bool IASCIIMesh::WriteMD2(const c8*fname){
    //write MD2 header
    const u16 M_MSZ=256;
    if(!SceneManager||nvt>ASCII_MVERTS||ntr>ASCII_MTRIS){
     printf(" error");return 0;}

    core::stringc fnm,src=fname,fnmx;
    fnm=src.subString(0,src.findLast('.')<0?src.size():src.findLast('.'));
    fnmx=fnm;fnm+=".md2";
    IM8M::CMRWFile*file=file->createMRWFile(fnm.c_str(),0);
    if(!file){printf("\nError writing file..");return 0;}
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
    s32 i=0,fr=0,fn=0;core::aabbox3d<f32> ebox=IntpBfr->BoundingBox;
    ntr=ntr+!ntr;nvt=nvt+!nvt;nsv=nsv+!nsv;
    c8 ident[4];s32 version;f32 scale3[3],scalo3[3];
    s32 framsiz,oftexco,nullv1,nullv2,nskins,nverts,ntris,nframs,ntskve,skinw,skinh;

    ident[0]='I';ident[1]='D';ident[2]='P';ident[3]='2';version=8;
    nskins=!!skbm[0]+(!skbm[1]?0:!(skbm[0]->getDimension().Width-
     skbm[1]->getDimension().Width+skbm[0]->getDimension().Height-
     skbm[1]->getDimension().Height));if(!nskins){return 0;}
    s32 sww=getSkin(0)->getDimension().Width,shh=getSkin(0)->getDimension().Height;
    skw=256;skh=256;

    v3f vmax,vmin;i=0;while(i<nfr){v3f v;
    s32 n=0;while(n<nvt){v=VertList[i].pointer()[n];
     if(v.X>vmax.X){vmax.X=v.X;}if(v.Y>vmax.Y){vmax.Y=v.Y;}if(v.Z>vmax.Z){vmax.Z=v.Z;}
     if(v.X<vmin.X){vmin.X=v.X;}if(v.Y<vmin.Y){vmin.Y=v.Y;}if(v.Z<vmin.Z){vmin.Z=v.Z;}
    n++;}i++;}

    scale3[0]=(vmax.X-vmin.X)/255.0f;
    scale3[1]=(vmax.Z-vmin.Z)/255.0f;
    scale3[2]=(vmax.Y-vmin.Y)/255.0f;
    scalo3[0]=vmin.X;
    scalo3[1]=vmin.Z;
    scalo3[2]=vmin.Y;
    framsiz=nvt*4+40;
    oftexco=132;
    nullv1=0;
    nullv2=0;
    nverts=nvt;
    ntris=ntr;
    nframs=nfr;
    ntskve=nsv;
    skinw=skw;printf(" :skinw:%d",skinw);
    skinh=skh;printf(" :skinh:%d",skinh);
    //write
    file->write(&ident,sizeof(ident));
    file->write(&version,sizeof(version));
    file->write(&skinw,sizeof(skinw));
    file->write(&skinh,sizeof(skinh));
    file->write(&framsiz,sizeof(framsiz));
    file->write(&nskins,sizeof(nskins));
    file->write(&nverts,sizeof(nverts));
    file->write(&ntskve,sizeof(ntskve));
    file->write(&ntris,sizeof(ntris));
    file->write(&nullv1,sizeof(nullv1));
    file->write(&nframs,sizeof(nframs));
    s32 ofssk=68;
    file->write(&ofssk,sizeof(ofssk));
    file->write(&oftexco,sizeof(oftexco));
    s32 ofstri=oftexco+ntskve*4;
    file->write(&ofstri,sizeof(ofstri));
    s32 ofsfrm=ofstri+ntris*12;
    file->write(&ofsfrm,sizeof(ofsfrm));
    file->write(&nullv2,sizeof(nullv2));
    s32 filesz=ofsfrm+framsiz*nframs;
    file->write(&ofsfrm,sizeof(ofsfrm));
    char TEXNAM[64];core::stringc tn=fnmx;tn+=".bmp";i=0;
    while(i<64){TEXNAM[i]=0;if(i<tn.size()){TEXNAM[i]=tn.c_str()[i];}i++;}
    file->write(&TEXNAM,sizeof(TEXNAM));

    ////////////////////////////////////////////////////////////////////////////
//////write tex
    video::IVideoDriver*driver=SceneManager->getVideoDriver();
    if(skbm[0]){driver->writeImageToFile(skbm[0],tn.c_str());}
//////TCoords
    STCoordx*MTCoords1=new STCoordx[nsv];

    f32 fc[2]={skw+!skw,skh+!skh};
    i=0;while(i<nsv){s16 st[2];
     st[0]=(s16)(s32)(MTCoords[i].s*fc[0]+0.5f);
     st[1]=(s16)(s32)(MTCoords[i].t*fc[1]+0.5f);
     file->write(&st,sizeof(st));
    i++;}

    i=0;while(i<ntr){file->write(&MTriangs[i],12);i++;}
    delete[]MTCoords1;printf("\n :TCoords OK");
    // normal indices{
    if(!NormList){
    // create normals slow
    printf("\n creating normals<");const s32 tr3=ntr*3;
    if(NormList){delete NormList;}NormList=new core::array<v3f>[nfr];
    i=0;while(i<nfr){NormList[i].set_used(nvt);i++;}
    s32 vi[tr3];i=0;while(i<ntr){s32 o0=0;while(o0<3){vi[i*3+o0]=
     MTriangs[i].vertexIndices[o0];o0++;}i++;}
    v3f*vnlst=new v3f[nfr*ntr];core::triangle3df tn;
    i=0;while(i<nfr){s32 o0=0,o1=0;v3f v3[3];
     o0=0;while(o0<ntr){
      o1=0;while(o1<3){v3[o1]=VertList[i].pointer()[vi[o0*3+o1]];o1++;}
      tn=core::triangle3df(v3[0],v3[1],v3[2]);vnlst[i*ntr+o0]=tn.getNormal();
     o0++;}i++;}printf(":");
    i=0;while(i<nfr){printf(".");
     s32 o0=0,o1=0,o2=0,acn=0,arr[tr3];
     while(o0<nvt){acn=0;v3f vn=v3f(0,0,0);
      o1=0;while(o1<ntr){
       o2=0;while(o2<3){if(o0==vi[o1*3+o2]){break;}o2++;
        if(o2>2){o1++;if(o1<ntr){o2=0;continue;}break;}}
       if(o2>2){break;}o2+=o1*3;
       if(acn<tr3){arr[acn]=o2*(o2>=0);acn++;}vn+=vnlst[i*ntr+o1];
       o1++;}vn/=f32(acn+!acn);
      o1=0;while(o1<acn){NormList[i].pointer()[o0]=vn;o1++;}
      o0++;}
     i++;}delete vnlst;printf(":>\n");
     }
    printf("\n creating normal indices.");
    core::array<u8>*norml=new core::array<u8>[nfr];
    i=0;while(i<nfr){norml[i].set_used(nvt);i++;}
    fr=0;while(fr<nfr&&NormList){printf(".");
     i=0;while(i<nvt){v3f v;norml[fr].pointer()[i]=5;f32 v0=10;
      s32 o0=0,o1=0;while(o0<NormTbsize){
       v.X=NormList[fr].pointer()[i].X-NormTb[o0][0];
       v.Y=NormList[fr].pointer()[i].Y-NormTb[o0][1];
       v.Z=NormList[fr].pointer()[i].Z-NormTb[o0][2];
       if(v.X<v0&&v.X>-v0&&v.Y<v0&&v.Y>-v0&&v.Z<v0&&v.Z>-v0){v0-=0.04f*(v0>0);
        norml[fr].pointer()[i]=o0;}
      o0++;if(!o1&&o0>=NormTbsize){o1=1;o0=0;}}
     i++;}
    fr++;}
    printf(".OK\n");
    // }normal indices
//////Frames
    c8 name[10];SMDL3Vertex0 vts[1];
    fr=0;s32 fnx=0;while(fr<nfr){s32 fc=fr;
     f32 sc[3]={scale3[0],scale3[1],scale3[2]};
     sc[0]+=!sc[0];sc[1]+=!sc[1];sc[2]+=!sc[2];
     if(fr>FData[fn].end){fn++;fnx=0;}else{fnx=!!fr*(fnx+1)%10;}
     i=0;while(i<10){if(i<FData[fn].name.size()){name[i]=FData[fn].name[i];}
      else{if(fc==fr&&!!fnx){fc++;name[i]=(fnx+48);}else{name[i]=0;}}i++;}
     i=0;while(i<nvt){
      vts[0].vtx[0]=(u8)u16((((VertList[fr].pointer()[i].X-vmin.X)*10.0f)/sc[0])/9.99998f);
      vts[0].vtx[1]=(u8)u16((((VertList[fr].pointer()[i].Z-vmin.Z)*10.0f)/sc[1])/9.99998f);
      vts[0].vtx[2]=(u8)u16((((VertList[fr].pointer()[i].Y-vmin.Y)*10.0f)/sc[2])/9.99998f);
      if(!i){file->write(&scale3,12);file->write(&scalo3,12);file->write(&name,10);
       u16 o1[3]={0,0,0};file->write(&o1,6);}
      file->write(&vts,3);u8 o0=norml[fr].pointer()[i];file->write(&o0,1);
      if(!fr&&!i){printf(" Precision (%f:%f)",f32((f32)vts[0].vtx[0]*scale3[0]+vmin.X),
       VertList[0].pointer()[0].X);}
     i++;}
    fr++;}
    file->drop();delete norml;
    if(nvt>2048){printf("\n Warning real num verts/irrlicht md2 max verts(%d/%d))",nvt,2048);}
    return 1;
}


//////////////
bool IASCIIMesh::WriteM8M(const c8*fname,bool nr,const c8*mnx,const u16*mvx,bool ni){
    IAnimatedMesh*msh=new IM8MMesh(SceneManager);s32 i=0;
    if(msh){
     IM8MMesh::STCoordx*MTCoordsx=new IM8MMesh::STCoordx[nsv];
     i=0;while(i<nsv){
      MTCoordsx[i].s=(s16)(s32)(MTCoords[i].s*2048.0f);
      MTCoordsx[i].t=(s16)(s32)(MTCoords[i].t*2048.0f);
     i++;}

     IM8MMesh::STriangx*MTriangsx=new IM8MMesh::STriangx[ntr];
     i=0;while(i<ntr){
      MTriangsx[i].vertexIndices[0]=MTriangs[i].vertexIndices[0];
      MTriangsx[i].vertexIndices[1]=MTriangs[i].vertexIndices[1];
      MTriangsx[i].vertexIndices[2]=MTriangs[i].vertexIndices[2];
      MTriangsx[i].textureIndices[0]=MTriangs[i].textureIndices[0];
      MTriangsx[i].textureIndices[1]=MTriangs[i].textureIndices[1];
      MTriangsx[i].textureIndices[2]=MTriangs[i].textureIndices[2];
     i++;}

     core::array<IM8MMesh::SFDatax> FDatax;IM8MMesh::SFDatax fdata;
     i=0;while(i<FData.size()){fdata.name=FData[i].name;fdata.start=FData[i].start;
      fdata.end=FData[i].end;fdata.fps=FData[i].fps;FDatax.push_back(fdata);i++;}

     i=((IM8MMesh*)msh)->createMesh(
      VertList,
      FDatax,
      MTCoordsx,
      MTriangsx,
      nvt,
      ntr,
      nfr,
      nsv,
      skbm[0]);

     delete MTCoordsx;delete MTriangsx;
     if(i){if(skbm[1]){((IM8MMesh*)msh)->setSkin(1,skbm[1]);}
      i=((IM8MMesh*)msh)->WriteM8M(fname,nr,mnx,mvx,ni);
      msh->drop();return i;}
     msh->drop();}return 0;
}

core::array<IASCIIMesh::v3f>*IASCIIMesh::getVertList()const{return VertList;}
core::array<IASCIIMesh::SFDatax> IASCIIMesh::getFData()const{return FData;}
IASCIIMesh::STCoordx*IASCIIMesh::getMTCoords()const{return MTCoords;}
IASCIIMesh::STriangx*IASCIIMesh::getMTriangs()const{return MTriangs;}
s32 IASCIIMesh::getnsk()const{return nsk;}
s32 IASCIIMesh::getnvt()const{return nvt;}
s32 IASCIIMesh::getntr()const{return ntr;}
s32 IASCIIMesh::getnsv()const{return nsv;}
s32 IASCIIMesh::getskt()const{return skt;}
s32 IASCIIMesh::getskw()const{return skw;}
s32 IASCIIMesh::getskh()const{return skh;}

} // end namespace scene
} // end namespace irr

//////////////////////////////////////////////////cpp
namespace irr
{
namespace scene
{

//! Constructor
CASCIIMeshConverter::CASCIIMeshConverter(scene::ISceneManager*smgr):SceneManager(smgr){}


bool CASCIIMeshConverter::isALoadableFileExtension(const c8*filename) const
{
    return (strstr(filename,".txt"));
}


//! creates/loads an animated mesh from the file.
//! \return Pointer to the created mesh. Returns 0 if loading failed.
//! If you no longer need the mesh, you should call IAnimatedMesh::drop().
//! See IReferenceCounted::drop() for more information.
IAnimatedMesh*CASCIIMeshConverter::createMesh(io::IReadFile*file)
{
    IAnimatedMesh*msh=new IASCIIMesh(SceneManager);
    if(msh)
    {
        if(((IASCIIMesh*)msh)->loadFile(file)){return msh;}
        msh->drop();
    }
    return 0;
}

} // end namespace scene
} // end namespace irr

#endif // _IRR_COMPILE_WITH_ASCII_MESH_LOADER_
