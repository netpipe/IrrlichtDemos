//============================================================================//
//
// date: 17.12.2008
//============================================================================//
//////////////////////////////////////////////////cpp
#include "CMDL3MeshLoader.h"
#ifdef _IRR_COMPILE_WITH_MDL3_MESH_LOADER_

#include "IReadFile.h"
#include "ISceneManager.h"

//////////////////////////////////////////////////cpp
namespace irr
{
namespace scene
{
//////////////////////////////////////////////////cpp
    const u32 MDL3_MTRIS=21845;
    const u32 MDL3_MVERTS=88888;
    const u16 MDL3_FRAME2=2;
    const f32 MDL3_FRAME2RPC=1.0f/(1<<MDL3_FRAME2);
//////////////
    struct SMDL3Header
    {
        c8    ident[4];        //mdl3
        s32   version;         //0
        f32   scale3[3];       //[3]  //SCALEMULTI
        f32   scalo3[3];       //[3]  //SCALEORIGIN

        s32   framsiz;         //0
        s32   oftexco;         //0
        s32   nullv1;          //0
        s32   nullv2;          //0

        s32   nskins;          //1    //NUMSKINS
        s32   skinwx;          //256  //SKWIDTH
        s32   skinhx;          //256  //SKHEIGHT
        s32   nverts;          //86   //NUMVERTICES

        s32   ntris;           //164  //NUMTRIANGLES
        s32   nframs;          //6    //NUMFRAMES
        s32   ntskve;          //39   //NUMSKVERTICES
        s32   nullv3;          //0

        s32   nullv4;          //0
        s32   skbpt;           //88   //SKBPPTYPE
        s32   skinw;           //0
        s32   skinh;           //0
    };
    struct SMDL3Vertex
    {
        u8    vtx[3];
        u8    nmid;
    };
    struct SMDL3Frame
    {
        s32   group;
        s32   groupx[2];
        c8    name[16];
        SMDL3Vertex vts[1];
    };
    struct IDP2_FRAME
    {
        f32   scale3[3];
        f32   scalo3[3];
        c8    name[16];
    };
//////////////

//! constructor
IMDL3Mesh::IMDL3Mesh(scene::ISceneManager*smgr):SceneManager(smgr),IntpBfr(0),
    VertList(0),FData(0),nsk(0),nvt(0),ntr(0),nfr(0),nsv(0),skt(0),skw(0),skh(0){
    IntpBfr=new SMeshBuffer;MTCoords=0;MTriangs=0;skbm[0]=0;skbm[1]=0;
    NormList=0;}

//! destructor
IMDL3Mesh::~IMDL3Mesh()
{
    video::IVideoDriver*driver=SceneManager->getVideoDriver();
    delete[]VertList;VertList=0;delete[]MTCoords;MTCoords=0;
    delete[]MTriangs;MTriangs=0;delete skbm[0];skbm[0]=0;delete skbm[1];skbm[1]=0;
    driver->removeTexture(IntpBfr->Material.getTexture(1));
    driver->removeTexture(IntpBfr->Material.getTexture(0));if(IntpBfr){delete IntpBfr;}
    delete[]NormList;NormList=0;
}

//! returns the amount of frames in milliseconds. If the amount is 1, it is a static mesh.
u32 IMDL3Mesh::getFrameCount()const
{
    return nfr<<MDL3_FRAME2;
}

IMesh*IMDL3Mesh::getMesh(s32 frame,s32 detailLevel,s32 startFrame,s32 endFrame)
{
    if((u32)frame>getFrameCount()){frame=(frame%getFrameCount());}

    if(startFrame==-1&&endFrame==-1){
        startFrame=0;
        endFrame=getFrameCount();
    }

    updateIntpBfr(frame,startFrame,endFrame);
    return this;
}

u32 IMDL3Mesh::getMeshBufferCount()const
{
    return 1;
}

IMeshBuffer*IMDL3Mesh::getMeshBuffer(u32 nr)const
{
    return IntpBfr;
}

//! Returns pointer to a mesh buffer which fits a material
IMeshBuffer*IMDL3Mesh::getMeshBuffer(const video::SMaterial &material)const
{
    if(IntpBfr->Material==material){getMeshBuffer(1);}return 0;
}

// updates the interpolation buffer
void IMDL3Mesh::updateIntpBfr(s32 frame,s32 startFrame,s32 endFrame)
{
    u32 firstFrame, secondFrame;
    f32 div;

    // TA: resolve missing ipol in loop between end-start

    if(endFrame-startFrame==0)
    {
        firstFrame=frame>>MDL3_FRAME2;
        secondFrame=frame>>MDL3_FRAME2;
        div=1.0f;
    }
    else
    {
        // key frames
        u32 s=startFrame>>MDL3_FRAME2;
        u32 e=endFrame>>MDL3_FRAME2;

        firstFrame=frame>>MDL3_FRAME2;
        secondFrame=firstFrame+1>e?s:firstFrame+1;

        firstFrame=core::s32_min(nfr-1,firstFrame);
        secondFrame=core::s32_min(nfr-1,secondFrame);

        frame&=(1<<MDL3_FRAME2)-1;
        div=frame*MDL3_FRAME2RPC;
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

void IMDL3Mesh::Trisx(STriangx*t0,STriangx*t1,s32 i){
    if(!t0||!t1){return;}i*=i>0;s32 o=0,o0=0;while(o0<3){
     o=t0[i].vertexIndices[o0];o*=o<=nvt;t1[i].vertexIndices[o0]=o;
     o=t0[i].textureIndices[o0];o*=o<=nsv;t1[i].textureIndices[o0]=o;o0++;}}

//! loads an MDL3 file
bool IMDL3Mesh::loadFile(io::IReadFile*file){
    if(!file){return 0;}SMDL3Header hdrx;const u16 M_MSZ=256;u32*M_BFR=0;

    file->read(&hdrx,sizeof(SMDL3Header));s32 SZH=88;
    core::stringc n1(hdrx.ident);n1[4]=0;
    FTYPE=(n1=="MDL3")+(n1=="MDL2")*2+(n1=="IDP2")*3+(n1=="IDPO")*4;if(FTYPE<1||FTYPE>4){
     printf("\n..(%s).",hdrx.ident);return 0;}
    n1=file->getFileName();s32 s0=n1.findLast(47)+1;s0=s0?s0:n1.findLast(92)+1;
    n1=n1.subString(s0,(n1.size()-s0));
    printf("\n>>>>>>..MDL3 Loader: %s (%s)|%d",hdrx.ident,n1.c_str(),hdrx.framsiz);
    if(hdrx.nskins<0||hdrx.nverts>MDL3_MVERTS||hdrx.ntris>MDL3_MTRIS){s32 o0=hdrx.nskins,
     o1=hdrx.nverts;printf("\n Not a supported file format..sk(%d).vt(%d).",
     o0,o1);return 0;}
    s32 skb[2]={0,0};skt=hdrx.skbpt;skb[0]=(skt<=1)+(skt>1&&skt<4||skt>=10&&skt<12)*2+
     (skt==4||skt==12||skt==88)*3+(skt>4&&skt<10&&skt!=6&&skt!=7||skt>12&&skt!=88)*4;
    hdrx.skinw=hdrx.skinwx;hdrx.skinh=hdrx.skinhx;
    skw=hdrx.skinw>M_MSZ&&skb[0]?M_MSZ:hdrx.skinw<8||!skb[0]?8:hdrx.skinw;
    skh=hdrx.skinh>M_MSZ&&skb[0]?M_MSZ:hdrx.skinh<8||!skb[0]?8:hdrx.skinh;
    s32 sw[2]={hdrx.skinw,0},sh[2]={hdrx.skinh,0};
    nsk=hdrx.nskins;nvt=hdrx.nverts;if(FTYPE>=2){hdrx.ntskve=nvt;}nsv=hdrx.ntskve;
//no skin
     if(hdrx.nskins<1||hdrx.nskins>32768){SZH=84;nsk=-1;hdrx.skbpt=0;skt=hdrx.skbpt;skb[0]=0;
      hdrx.skinw=8;hdrx.skinh=8;skw=hdrx.skinw;skh=hdrx.skinh;sw[0]=hdrx.skinw;
      sh[0]=hdrx.skinh;}
//
    s32 mip=(skt>=10&&skt!=88)*((hdrx.skinw*hdrx.skinh*skb[0])/4+
     (hdrx.skinw*hdrx.skinh*skb[0])/16+(hdrx.skinw*hdrx.skinh*skb[0])/64);
    printf("\n..v(%d,%d,%d).frms(%d)",skt,skw,skh,hdrx.nframs);
    hdrx.nframs=hdrx.nframs>2048?2048:hdrx.nframs+!hdrx.nframs;hdrx.framsiz=hdrx.nverts*4+28
     +(FTYPE==3)*8;
    hdrx.oftexco=SZH+(skb[0]*hdrx.skinw*hdrx.skinh)+(hdrx.skinw*!skb[0]&&nsk>0)+mip;s32 esk=0;
    nsk=nsk>0?nsk:0;
    while(esk<(nsk-1)){s32 v[3],mipx=0;
     file->seek(hdrx.oftexco);file->read(&v[0],4);mipx=v[0];
     v[1]=hdrx.skinw;v[2]=hdrx.skinh;
     printf("\n..v(%d,%d,%d)-%d/%d",v[0],v[1],v[2],esk+2,nsk);
     v[0]=(v[0]<=1)+(v[0]>1&&v[0]<4||v[0]>=10&&v[0]<12)*2+(v[0]==4||v[0]==12||v[0]==88)*3
      +(v[0]>4&&v[0]<10&&v[0]!=6&&v[0]!=7||v[0]>12&&v[0]!=88)*4;
     mipx=(mipx>=10&&mipx!=88)*((v[0]*v[1]*v[2])/4+(v[0]*v[1]*v[2])/16+(v[0]*v[1]*v[2])/64);
     hdrx.oftexco+=4+(v[0]*v[1]*v[2])+(v[1]*!v[0])+mipx;skb[1]=!esk?v[0]:skb[1];
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
     file->seek(SZH+numsk*(skb[0]*sw[0]*sh[0]+4+mip));
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
    MTCoords=new STCoordx[nsv*2+!nsv];
//
    u16 fv[nsv][3];s32 o0=0,o1=0;
    while(o0<nsv){fv[o0][0]=0;fv[o0][1]=0;fv[o0][2]=0;o0++;}
    if(FTYPE>=2){
     o0=0;o1=0;while(o0<nsv+o1){u32 f=0,vi[2];
       if(o0<nsv){
         file->read(&f,4);file->read(vi,8);
         MTCoords[o0].s=(u16)vi[0];
         MTCoords[o0].t=(u16)vi[1];
         if(f>0){
           fv[o1][0]=(u16)(vi[0]+hdrx.skinw/2);
           fv[o1][1]=(u16)vi[1];fv[o1][2]=o0;o1++;}}
       if(o0>=nsv){
         MTCoords[o0].s=fv[o0-nsv][0];
         MTCoords[o0].t=fv[o0-nsv][1];}
       o0++;}nsv=o0;
    }
//
    else if(!file->read(MTCoords,sizeof(STCoordx)*nsv)){
     printf(" Error reading TextureCoords.");}
    printf("..TCoords.OK");
    f32 fc[2]={2048.0f/f32(hdrx.skinw+!hdrx.skinw),2048.0f/f32(hdrx.skinh+!hdrx.skinh)};
    i=0;while(i<nsv){
     MTCoords[i].s=(s16)(s32)(MTCoords[i].s*fc[0]+0.5f);
     MTCoords[i].t=(s16)(s32)(MTCoords[i].t*fc[1]+0.5f);
    i++;}
    // read Triangles
    MTriangs=new STriangx[ntr];
//
    if(FTYPE>=2){//bug textureIndices
     s32 o0=0,o1=0,o2=0;while(o0<ntr){
      u32 f=0,vi[3];file->read(&f,4);file->read(vi,12);
      MTriangs[o0].vertexIndices[0]=(u16)vi[0];
      MTriangs[o0].vertexIndices[1]=(u16)vi[1];
      MTriangs[o0].vertexIndices[2]=(u16)vi[2];o1=0;
      if(MTCoords[(u16)vi[0]].s/2048.0f>=0.5f){o1++;}
      if(MTCoords[(u16)vi[1]].s/2048.0f>=0.5f){o1++;}
      if(MTCoords[(u16)vi[2]].s/2048.0f>=0.5f){o1++;}
      if(o1>0){
       o2=0;while(o2>=0&&o2<nsv){
        if(fv[o2][2]==(u16)vi[0]){vi[0]=nvt+o2;}
        if(fv[o2][2]==(u16)vi[1]){vi[1]=nvt+o2;}
        if(fv[o2][2]==(u16)vi[2]){vi[2]=nvt+o2;}
        o2++;if(fv[o2][2]==0){o2=-1;}}}
      else{
      s32 o3=0;while(o3<o0){u16 o[3],o4=0;
       o[0]=MTriangs[o3].textureIndices[0];
       o[1]=MTriangs[o3].textureIndices[1];
       o[2]=MTriangs[o3].textureIndices[2];
       if(vi[0]==o[0]||vi[0]==o[1]||vi[0]==o[2]){o4++;}
       if(vi[1]==o[0]||vi[1]==o[1]||vi[1]==o[2]){o4++;}
       if(vi[2]==o[0]||vi[2]==o[1]||vi[2]==o[2]){o4++;}
       if(o4>=3){
        o2=0;while(o2>=0&&o2<nsv){
         if(fv[o2][2]==(u16)vi[0]){vi[0]=nvt+o2;}
         if(fv[o2][2]==(u16)vi[1]){vi[1]=nvt+o2;}
         if(fv[o2][2]==(u16)vi[2]){vi[2]=nvt+o2;}
         o2++;if(fv[o2][2]==0){o2=-1;}}
        }o3++;}}
      MTriangs[o0].textureIndices[0]=(u16)vi[0];
      MTriangs[o0].textureIndices[1]=(u16)vi[1];
      MTriangs[o0].textureIndices[2]=(u16)vi[2];
      o0++;}
    }
//
    else if(!file->read(MTriangs,sizeof(STriangx)*ntr)){
     printf(" Error reading triangles.");return 0;}
    i=0;while(i<ntr){Trisx(MTriangs,MTriangs,i);i++;}//prevents bug
    printf("..Triangles.OK");

    // read Vertices
    SMDL3Frame*frame=(SMDL3Frame*)new u8[MDL3_MVERTS*4+40];

    f32 scl[3];scl[0]=hdrx.scale3[0];scl[1]=hdrx.scale3[1];scl[2]=hdrx.scale3[2];
    f32 sco[3];sco[0]=hdrx.scalo3[0];sco[1]=hdrx.scalo3[1];sco[2]=hdrx.scalo3[2];
    i=0;while(i<nfr){
     // read vertices
     file->read(frame,hdrx.framsiz);

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

    IntpBfr->Material.Lighting=1;IntpBfr->Material.ZWriteEnable=1;
    IntpBfr->Material.Wireframe=0;IntpBfr->Material.BackfaceCulling=1;
    IntpBfr->Material.GouraudShading=1;IntpBfr->Material.FogEnable=1;
    IntpBfr->Material.NormalizeNormals=1;IntpBfr->Material.ZBuffer=1;
    IntpBfr->Material.PointCloud=0;

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

    // nsv
    if(!nsv){nsv=1;}

    // return
    calculateBoundingBox();
    return 1;
}


//! calculates the bounding box
void IMDL3Mesh::calculateBoundingBox()
{
    IntpBfr->BoundingBox.reset(0,0,0);
    if(VertList[0].size()){s32 i=0;while(i<VertList[0].size()){
      IntpBfr->BoundingBox.addInternalPoint(VertList[0].pointer()[i]);i++;}
    }
}


//! sets a flag of all contained materials to a new value
void IMDL3Mesh::setMaterialFlag(video::E_MATERIAL_FLAG flag,bool newvalue)
{
    IntpBfr->Material.setFlag(flag,newvalue);
}


//! returns an axis aligned bounding box
const core::aabbox3d<f32>&IMDL3Mesh::getBoundingBox() const
{
    return IntpBfr->BoundingBox;
}


//! set user axis aligned bounding box
void IMDL3Mesh::setBoundingBox(const core::aabbox3df& box)
{
    IntpBfr->BoundingBox=box;
}


//! Returns the type of the animated mesh.
E_ANIMATED_MESH_TYPE IMDL3Mesh::getMeshType() const
{
    return (E_ANIMATED_MESH_TYPE)90;
}

//! Returns frame loop data for a special MDL3 animation type.
bool IMDL3Mesh::getFrameLoop(const c8*name,s32 &outStart,s32 &outEnd,s32 &outFPS)const{
    s32 i=0;while(i<FData.size()){
     if(FData[i].name==name){
      outStart=FData[i].start<<MDL3_FRAME2;
      outEnd=FData[i].end<<MDL3_FRAME2;
      outEnd+=!MDL3_FRAME2?1:(1<<MDL3_FRAME2)-1;
      outFPS=FData[i].fps<<MDL3_FRAME2;
      return 1;}i++;}
    return 0;
}


//! Returns amount of MDL3 animations in this file.
u32 IMDL3Mesh::getAnimationCount()const
{
    return FData.size();
}


//! Returns name of MDL3 animation.
const c8*IMDL3Mesh::getAnimationName(u32 nr)const
{
    if(nr>=FData.size()){return 0;}
    return FData[nr].name.c_str();
}


video::ITexture*IMDL3Mesh::getTexture(bool nr)const
{
    if(IntpBfr->Material.getTexture(1)&&nr&&nsk>1){return IntpBfr->Material.getTexture(1);}
    return IntpBfr->Material.getTexture(0);
}


video::IImage*IMDL3Mesh::getSkin(bool nr)const
{
    if(skbm[1]&&nr){return skbm[1];}
    return skbm[0];
}


void IMDL3Mesh::setSkin(bool nr,video::IImage*im)
{
    video::IVideoDriver*driver=SceneManager->getVideoDriver();nr*=!!skbm[0];
    if(!nr&&im&&im!=skbm[0]){delete skbm[0];skbm[0]=0;skbm[0]=driver->createImageFromData(
     video::ECF_A8R8G8B8,im->getDimension(),im->lock(),0,1);im->unlock();
     IntpBfr->Material.setTexture(0,driver->addTexture("s0.bmp",skbm[0]));}
    if(nr&&im!=skbm[1]){delete skbm[1];skbm[1]=0;if(im){skbm[1]=driver->createImageFromData(
     video::ECF_A8R8G8B8,im->getDimension(),im->lock(),0,1);im->unlock();
     IntpBfr->Material.setTexture(1,driver->addTexture("s1.bmp",skbm[1]));}}
}


//////////////
bool IMDL3Mesh::WriteM8M(const c8*fname,bool nr,const c8*mnx,const u16*mvx,bool ni){
    IAnimatedMesh*msh=new IM8MMesh(SceneManager);s32 i=0;
    if(msh){
     IM8MMesh::STCoordx*MTCoordsx=new IM8MMesh::STCoordx[nsv];
     i=0;while(i<nsv){
      MTCoordsx[i].s=MTCoords[i].s;
      MTCoordsx[i].t=MTCoords[i].t;
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

core::array<IMDL3Mesh::v3f>*IMDL3Mesh::getVertList()const{return VertList;}
core::array<IMDL3Mesh::SFDatax> IMDL3Mesh::getFData()const{return FData;}
IMDL3Mesh::STCoordx*IMDL3Mesh::getMTCoords()const{return MTCoords;}
IMDL3Mesh::STriangx*IMDL3Mesh::getMTriangs()const{return MTriangs;}
s32 IMDL3Mesh::getnsk()const{return nsk;}
s32 IMDL3Mesh::getnvt()const{return nvt;}
s32 IMDL3Mesh::getntr()const{return ntr;}
s32 IMDL3Mesh::getnsv()const{return nsv;}
s32 IMDL3Mesh::getskt()const{return skt;}
s32 IMDL3Mesh::getskw()const{return skw;}
s32 IMDL3Mesh::getskh()const{return skh;}

u8 IMDL3Mesh::getFileType()const{ return FTYPE;}

} // end namespace scene
} // end namespace irr

//////////////////////////////////////////////////cpp
namespace irr
{
namespace scene
{

//! Constructor
CMDL3MeshLoader::CMDL3MeshLoader(scene::ISceneManager*smgr):SceneManager(smgr){}


bool CMDL3MeshLoader::isALoadableFileExtension(const c8*filename) const
{
    return (strstr(filename,".mdl"));
}


//! creates/loads an animated mesh from the file.
//! \return Pointer to the created mesh. Returns 0 if loading failed.
//! If you no longer need the mesh, you should call IAnimatedMesh::drop().
//! See IReferenceCounted::drop() for more information.
IAnimatedMesh*CMDL3MeshLoader::createMesh(io::IReadFile*file)
{
    IAnimatedMesh*msh=new IMDL3Mesh(SceneManager);
    if(msh)
    {
        if(((IMDL3Mesh*)msh)->loadFile(file)){return msh;}
        msh->drop();
    }
    return 0;
}

} // end namespace scene
} // end namespace irr

#endif // _IRR_COMPILE_WITH_MDL3_LOADER_
