//============================================================================//
//
// date: 21.12.2008
//============================================================================//
//////////////////////////////////////////////////cpp
#include "CMDL7MeshLoader.h"
#ifdef _IRR_COMPILE_WITH_MDL7_MESH_LOADER_

#include "IReadFile.h"
#include "ISceneManager.h"

//////////////////////////////////////////////////cpp
namespace irr
{
namespace scene
{
//////////////////////////////////////////////////cpp
    const u32 MDL7_MTRIS=21845;
    const u32 MDL7_MVERTS=88888;
    const u16 MDL7_FRAME2=2;
    const f32 MDL7_FRAME2RPC=1.0f/(1<<MDL7_FRAME2);
//////////////
    struct SMDL7Header
    {
        c8    ident[4];        //mdl7
        s32   version;         //0
        s32   nbones;          //0
        s32   ngroup;          //1

        s32   filesz;          //282660
        s32   entlsz;          //0
        s32   medlsz;          //0
        u16   nullvx0;         //36
        u16   nullvx1;         //28
//
        u16   nullvx2;         //?0
        u16   nullv1;          //?0
        u16   nullv2;          //?0
        u16   typex;           //16

        u16   typey;           //0
        u16   typez;           //0
        u16   scale3[3];       //[3]
        u16   scalo3[3];       //[3]
//
        s32   framsiz;         //?
        s32   oftexco;         //?
        u16   skinwx;          //0
        u16   skinhx;          //0

        u16   nullv3;          //0
        u16   nullv4;          //0
        s32   nskins;          //1    //NUMSKINS
        s32   ntskve;          //39   //NUMSKVERTICES
        s32   ntris;           //164  //NUMTRIANGLES

        s32   nverts;          //86   //NUMVERTICES
        s32   nframs;          //6    //NUMFRAMES
        s32   skbpt;           //88   //SKBPPTYPE
        s32   skinw;           //256  //SKWIDTH

        s32   skinh;           //256  //SKHEIGHT
        c8    txname[16];      //256  //texname
    };
    struct SMDL7Vertex
    {
        f32   vtx[3];
        u16   vtid;
        union{
         u16  nmid;
         f32  norm[3];
        };
    };
    struct SMDL7Frame
    {
        c8    name[16];
        s32   vcnt;
        s32   tcnt;
        SMDL7Vertex vts[1];
    };
//////////////

//! constructor
IMDL7Mesh::IMDL7Mesh(scene::ISceneManager*smgr):SceneManager(smgr),IntpBfr(0),
    VertList(0),FData(0),nsk(0),nvt(0),ntr(0),nfr(0),nsv(0),skt(0),skw(0),skh(0){
    IntpBfr=new SMeshBuffer;MTCoords=0;MTriangs=0;skbm[0]=0;skbm[1]=0;
    NormList=0;}

//! destructor
IMDL7Mesh::~IMDL7Mesh()
{
    video::IVideoDriver*driver=SceneManager->getVideoDriver();
    delete[]VertList;VertList=0;delete[]MTCoords;MTCoords=0;
    delete[]MTriangs;MTriangs=0;delete skbm[0];skbm[0]=0;delete skbm[1];skbm[1]=0;
    driver->removeTexture(IntpBfr->Material.getTexture(1));
    driver->removeTexture(IntpBfr->Material.getTexture(0));if(IntpBfr){delete IntpBfr;}
    delete[]NormList;NormList=0;
}

//! returns the amount of frames in milliseconds. If the amount is 1, it is a static mesh.
u32 IMDL7Mesh::getFrameCount()const
{
    return nfr<<MDL7_FRAME2;
}

IMesh*IMDL7Mesh::getMesh(s32 frame,s32 detailLevel,s32 startFrame,s32 endFrame)
{
    if((u32)frame>getFrameCount()){frame=(frame%getFrameCount());}

    if(startFrame==-1&&endFrame==-1){
        startFrame=0;
        endFrame=getFrameCount();
    }

    updateIntpBfr(frame,startFrame,endFrame);
    return this;
}

u32 IMDL7Mesh::getMeshBufferCount()const
{
    return 1;
}

IMeshBuffer*IMDL7Mesh::getMeshBuffer(u32 nr)const
{
    return IntpBfr;
}

//! Returns pointer to a mesh buffer which fits a material
IMeshBuffer*IMDL7Mesh::getMeshBuffer(const video::SMaterial &material)const
{
    if(IntpBfr->Material==material){getMeshBuffer(1);}return 0;
}

// updates the interpolation buffer
void IMDL7Mesh::updateIntpBfr(s32 frame,s32 startFrame,s32 endFrame)
{
    u32 firstFrame, secondFrame;
    f32 div;

    // TA: resolve missing ipol in loop between end-start

    if(endFrame-startFrame==0)
    {
        firstFrame=frame>>MDL7_FRAME2;
        secondFrame=frame>>MDL7_FRAME2;
        div=1.0f;
    }
    else
    {
        // key frames
        u32 s=startFrame>>MDL7_FRAME2;
        u32 e=endFrame>>MDL7_FRAME2;

        firstFrame=frame>>MDL7_FRAME2;
        secondFrame=firstFrame+1>e?s:firstFrame+1;

        firstFrame=core::s32_min(nfr-1,firstFrame);
        secondFrame=core::s32_min(nfr-1,secondFrame);

        frame&=(1<<MDL7_FRAME2)-1;
        div=frame*MDL7_FRAME2RPC;
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

void IMDL7Mesh::Trisx(STriangx*t0,STriangx*t1,s32 i){
    if(!t0||!t1){return;}i*=i>0;s32 o=0,o0=0;while(o0<3){
     o=t0[i].vertexIndices[o0];o*=o<=nvt;t1[i].vertexIndices[o0]=o;
     o=t0[i].textureIndices[o0];o*=o<=nsv;t1[i].textureIndices[o0]=o;o0++;}}

//! loads an mdl7 file
bool IMDL7Mesh::loadFile(io::IReadFile*file){
    if(!file){return 0;}SMDL7Header hdrx;const u16 M_MSZ=256;u32*M_BFR=0;

    file->read(&hdrx,sizeof(SMDL7Header));s32 SZH=120;
    core::stringc n1(hdrx.ident);n1[4]=0;
    if(n1!="MDL7"){return 0;}
    n1=file->getFileName();s32 s0=n1.findLast(47)+1;s0=s0?s0:n1.findLast(92)+1;
    n1=n1.subString(s0,(n1.size()-s0));
    printf("\n>>>>>>..MDL7 Loader: %s (%s)|%d",hdrx.ident,n1.c_str(),hdrx.framsiz);
    //skip bones
    if(hdrx.nbones>0){printf("\n Warning bones is not supported.(%d)",hdrx.nbones);
     if(hdrx.nbones>500){return 0;}file->seek(48+hdrx.nbones*36+4);
     file->read(&hdrx.scalo3[1],sizeof(hdrx.scalo3[1]));
     file->read(&hdrx.scalo3[2],sizeof(hdrx.scalo3[2]));
     file->read(&hdrx.framsiz,sizeof(hdrx.framsiz));
     file->read(&hdrx.oftexco,sizeof(hdrx.oftexco));
     file->read(&hdrx.skinwx,sizeof(hdrx.skinwx));
     file->read(&hdrx.skinhx,sizeof(hdrx.skinhx));
     file->read(&hdrx.nullv3,sizeof(hdrx.nullv3));
     file->read(&hdrx.nullv4,sizeof(hdrx.nullv4));
     file->read(&hdrx.nskins,sizeof(hdrx.nskins));
     file->read(&hdrx.ntskve,sizeof(hdrx.ntskve));
     file->read(&hdrx.ntris,sizeof(hdrx.ntris));
     file->read(&hdrx.nverts,sizeof(hdrx.nverts));
     file->read(&hdrx.nframs,sizeof(hdrx.nframs));
     file->read(&hdrx.skbpt,sizeof(hdrx.skbpt));
     file->read(&hdrx.skinw,sizeof(hdrx.skinw));
     file->read(&hdrx.skinh,sizeof(hdrx.skinh));
     file->read(&hdrx.txname,sizeof(hdrx.txname));}
    if(hdrx.nskins<0||hdrx.nverts>MDL7_MVERTS||hdrx.ntris>MDL7_MTRIS){s32 o0=hdrx.nskins,
     o1=hdrx.nverts;printf("\n Not a supported file format..sk(%d).vt(%d).",
     o0,o1);return 0;}
    if(hdrx.ngroup>1){printf("\n Warning multi-groups is not supported.");}
    s32 offtyp[2]={0,0};printf("\n typex %d/y %d/z %d",
     hdrx.typex,hdrx.typey,hdrx.typez);
    if(hdrx.skbpt>=32){hdrx.skbpt-=32;offtyp[0]+=2;}
    if(hdrx.skbpt>=16){hdrx.skbpt-=16;offtyp[0]+=1;}
    s32 skb[2]={0,0};skt=hdrx.skbpt;skb[0]=(skt<=1)+(skt>1&&skt<4||skt>=10&&skt<12)*2+
     (skt==4||skt==12||skt==88)*3+(skt>4&&skt<10&&skt!=6&&skt!=7||skt>12&&skt!=88)*4;
    skw=hdrx.skinw>M_MSZ&&skb[0]?M_MSZ:hdrx.skinw<8||!skb[0]?8:hdrx.skinw;
    skh=hdrx.skinh>M_MSZ&&skb[0]?M_MSZ:hdrx.skinh<8||!skb[0]?8:hdrx.skinh;
    s32 sw[2]={hdrx.skinw,0},sh[2]={hdrx.skinh,0};
    nsk=hdrx.nskins;nvt=hdrx.nverts;nsv=hdrx.ntskve;
//dds
    s32 cdds[2]={0,0};
    if(skt==6){s32 o[5];file->seek(SZH+hdrx.nbones*36);file->read(&o,20);
     skb[0]=hdrx.skinw/((o[4]*o[3])+!(o[4]*o[3]));
     if(!skb[0]){cdds[0]=hdrx.skinw;}//compressed
     hdrx.skinw=o[4];hdrx.skinh=o[3];
     skw=hdrx.skinw>M_MSZ&&skb[0]?M_MSZ:hdrx.skinw<8||!skb[0]?8:hdrx.skinw;
     skh=hdrx.skinh>M_MSZ&&skb[0]?M_MSZ:hdrx.skinh<8||!skb[0]?8:hdrx.skinh;
     sw[0]=hdrx.skinw;sh[0]=hdrx.skinh;}
//
//no skin
     if(hdrx.nskins<1||hdrx.nskins>32768){nsk=-1;hdrx.skbpt=0;skt=hdrx.skbpt;skb[0]=0;
      hdrx.skinw=8;hdrx.skinh=8;skw=hdrx.skinw;skh=hdrx.skinh;sw[0]=hdrx.skinw;
      sh[0]=hdrx.skinh;offtyp[0]=0;}
//
    s32 mip=(skt>=10&&skt!=88)*((hdrx.skinw*hdrx.skinh*skb[0])/4+
     (hdrx.skinw*hdrx.skinh*skb[0])/16+(hdrx.skinw*hdrx.skinh*skb[0])/64);
    printf("\n..v(%d,%d,%d).frms(%d).skb[0](%d)",skt,skw,skh,hdrx.nframs,skb[0]);
    hdrx.nframs=hdrx.nframs>2048?2048:hdrx.nframs+!hdrx.nframs;hdrx.framsiz=hdrx.nverts*26;
    hdrx.nframs=hdrx.nframs*!hdrx.nbones+!!hdrx.nbones;
    if(hdrx.typex==12){hdrx.framsiz=hdrx.nverts*16;}
    hdrx.oftexco=SZH+(skt==6&&!cdds[0])*128+(skb[0]*hdrx.skinw*hdrx.skinh)+
     (cdds[0]*!skb[0])+(skt==7&&!skb[0])*hdrx.skinw+mip+
     (offtyp[0]==1||offtyp[0]==3)*68+hdrx.nbones*36;
    if(offtyp[0]==2||offtyp[0]==3){s32 v;file->seek(hdrx.oftexco);file->read(&v,4);
     if(v<524288){hdrx.oftexco+=4+v;printf("..voffs(%d,offtyp%d)",v,offtyp[0]);}
     else{printf("Error...%d pos%d",v,file->getPos());return 0;}}s32 esk=0;
    printf("..mip(%d,offt%d)",mip,hdrx.oftexco);nsk=nsk>0?nsk:0;
    while(esk<(nsk-1)){s32 v[3],mipx=0,offtx=0,o0=0;
     file->seek(hdrx.oftexco);file->read(&v,12);
     if(v[0]>=32){v[0]-=32;offtx+=2;}if(v[0]>=16){v[0]-=16;offtx+=1;}mipx=v[0];o0=v[0];
     printf("\n..v(%d,%d,%d)-%d/%d",v[0],v[1],v[2],esk+2,nsk);
     v[0]=(v[0]<=1)+(v[0]>1&&v[0]<4||v[0]>=10&&v[0]<12)*2+(v[0]==4||v[0]==12||v[0]==88)*3
      +(v[0]>4&&v[0]<10&&v[0]!=6&&v[0]!=7||v[0]>12&&v[0]!=88)*4;
     mipx=(mipx>=10&&mipx!=88)*((v[0]*v[1]*v[2])/4+(v[0]*v[1]*v[2])/16+(v[0]*v[1]*v[2])/64);
     offtyp[1]=!esk?offtx:offtyp[1];
//dds
     if(o0==6){s32 o[5];file->read(&o,16);file->read(&o,20);
      v[0]=v[1]/((o[4]*o[3])+!(o[4]*o[3]));
      if(v[0]<2){cdds[1]=v[1];v[0]=0;}v[1]=o[4];v[2]=o[3];
     printf("..dds...v(%d,%d,%d)-%d/%d o4: %d",v[0],v[1],v[2],esk+2,nsk,o[4]);}
//
     hdrx.oftexco+=28+(o0==6&&!cdds[1])*128+(v[0]*v[1]*v[2])+(cdds[1]*!v[0])
      +(o0==7&&!v[0])*v[1]+mipx+(offtx==1||offtx==3)*68;
     if(offtx==2||offtx==3){s32 v;file->seek(hdrx.oftexco);file->read(&v,4);
      if(v<524288){hdrx.oftexco+=4+v;}else{printf("Error...");return 0;}}
     skb[1]=!esk?v[0]:skb[1];sw[1]=!esk?v[1]*!!skb[1]:sw[1];
     sh[1]=!esk?v[2]*!!skb[1]:sh[1];esk++;}printf("\nskb%d\n",skb[1]);
    nsk=!!skb[1]+1;printf("\n..cdds[0]..%d.cdds[1]..%d.",cdds[0],cdds[1]);
//no skin
    if(hdrx.nskins<1){hdrx.oftexco=SZH-28+hdrx.nbones*36;}
//
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
     printf("\n%d %d %d %d\n",numsk,skbx,sww,shh);
     if(!numsk){file->seek(SZH+(skt==6&&!cdds[0])*128+hdrx.nbones*36);}
     s32 sktx=0,mipx=mip,fpos=(skbx*hdrx.skinw*hdrx.skinh)+(cdds[0]*!skbx)
      +(skt==7&&!skbx)*hdrx.skinw;
     if(numsk&&skbx){s32 v[7];file->read(&v,28);
      if(v[0]>=32){v[0]-=32;}if(v[0]>=16){v[0]-=16;}sktx=v[0];
      mipx=(v[0]>=10&&v[0]!=88)*((v[1]*v[2]*skbx)/4+(v[1]*v[2]*skbx)/16+(v[1]*v[2]*skbx)/64);
      fpos=(skbx*v[1]*v[2])+(cdds[1]*!skbx)+(sktx==7&&!skbx)*v[1];
      printf("\n---7 %d %d %d \n",v[0],v[1],v[2]);}
     if(numsk&&sktx==6&&!cdds[1]){file->seek(file->getPos()+128);
      printf("\nsdds %d ,%d %d %d - %d %d\n",file->getPos(),skbx,sww,shh,skw,skh);}
     s32 spos=file->getPos();
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
     spos+=fpos;file->seek(spos+mipx);
     printf("\nemip(%d) %d offtyp[]%d",mipx,file->getPos(),offtyp[numsk]);//mip
     if(offtyp[numsk]==1||offtyp[numsk]==3){if(!numsk){f32 v[4],v0=255.000001f;s32 vt=255;
       file->read(&v,sizeof(v));v[0]*=v0;v[1]*=v0;v[2]*=v0;v[3]*=v0;
       printf("\n  Dif..(%f,%f,%f)-%f",v[0],v[1],v[2],v[3]);
       IntpBfr->Material.DiffuseColor=video::SColor(vt,(s32)v[0],(s32)v[1],(s32)v[2]);
       file->read(&v,sizeof(v));v[0]*=v0;v[1]*=v0;v[2]*=v0;v[3]*=v0;
       printf("\n  Amb..(%f,%f,%f)-%f",v[0],v[1],v[2],v[3]);vt=(s32)v[3];
       IntpBfr->Material.EmissiveColor=video::SColor(vt,(s32)v[0],(s32)v[1],(s32)v[2]);
       IntpBfr->Material.AmbientColor=video::SColor(vt,(s32)v[0],(s32)v[1],(s32)v[2]);
       file->read(&v,sizeof(v));v[0]*=v0;v[1]*=v0;v[2]*=v0;v[3]*=v0;
       printf("\n  Spe..(%f,%f,%f)-%f",v[0],v[1],v[2],v[3]);
       IntpBfr->Material.SpecularColor=video::SColor(vt,(s32)v[0],(s32)v[1],(s32)v[2]);
       file->read(&v,sizeof(v));v[0]*=v0;v[1]*=v0;v[2]*=v0;v[3]*=v0;
       printf("\n  Emi..(%f,%f,%f)-%f",v[0],v[1],v[2],v[3]);
       if(v[0]>0||v[1]>0||v[2]>0){
        IntpBfr->Material.EmissiveColor=video::SColor(vt,(s32)v[0],(s32)v[1],(s32)v[2]);}
       file->read(&v[0],4);printf("\n  Shi..(%f)",v[0]);IntpBfr->Material.Shininess=v[0];}
      else{file->seek(file->getPos()+68);}}
     printf("\nemat %d",file->getPos());//material

     if(offtyp[numsk]==2||offtyp[numsk]==3){s32 v;file->read(&v,4);
      if(v>0&&v<524288){printf("..(ex %d)",v);file->seek(file->getPos()+v);}
      else{printf("..(ex %d,p %d)..Errror.",v,file->getPos());return 0;}}
     printf("\neeff %d\n",file->getPos());//effect

     video::IImage*skbn=driver->createImageFromData(video::ECF_A8R8G8B8,core::
      dimension2d<s32>(skw,skh),M_BFR,0,1);
     if(numsk<=0){skbm[0]=skbn;
      if(skt!=7||!hdrx.skinw||hdrx.skinw>32){IntpBfr->Material.setTexture(0,
        driver->addTexture("s0.bmp",skbm[0]));
       printf("\n..Skin0Form...%dx%dx%d.",skw,skh,skbx);}}
     if(numsk>0&&numsk<nsk){skbm[1]=skbn;IntpBfr->Material.setTexture(1,
       driver->addTexture("s1.bmp",skbm[1]));
      printf("..Skin1Form...%dx%dx%d.",skw,skh,skbx);}
    numsk++;}delete M_BFR;printf("..Skin.OK");
//////SkinTex
//ext texture
    if(skt==7&&hdrx.skinw>0&&hdrx.skinw<32){
     c8 name[hdrx.skinw];file->seek(SZH+hdrx.nbones*36);file->read(&name,hdrx.skinw);
     video::ITexture*tex=driver->getTexture(name);
     if(tex){skbm[0]=driver->createImageFromData(tex->getColorFormat(),
      tex->getSize(),tex->lock(),0,1);tex->unlock();}
     IntpBfr->Material.setTexture(0,driver->addTexture("s0.bmp",skbm[0]));
     printf("\n..Skin0Form...%dx%dx%d?.",skw,skh,skbx);}
//ext texture

    // create Memory for indices and frames
    ntr=hdrx.ntris;nfr=hdrx.nframs;
    VertList=new core::array<v3f>[nfr];s32 i=0;

    // read TextureCoords
    file->seek(hdrx.oftexco);
    MTCoords=new STCoordx[nsv+!nsv];
    if(!file->read(MTCoords,sizeof(STCoordx)*nsv)){
     printf(" Error reading TextureCoords.");}
    printf("..TCoords.OK");
    // read Triangles
    MTriangs=new STriangx[ntr];
    if(hdrx.typex==12){
     i=0;while(i<ntr){
      file->read(MTriangs[i].vertexIndices,6);
      file->read(MTriangs[i].textureIndices,6);
      MTriangs[i].null=0;i++;}}
    else if(hdrx.typex==26){
     i=0;while(i<ntr){
      file->read(MTriangs[i].vertexIndices,6);
      file->read(MTriangs[i].textureIndices,6);
      file->read(&MTriangs[i].null,4);s16 t[3];file->read(&t,6);
      file->read(&MTriangs[i].null,4);MTriangs[i].null=0;i++;}}
    else if(!file->read(MTriangs,sizeof(STriangx)*ntr)){
     printf(" Error reading triangles.");return 0;}
    i=0;while(i<ntr){Trisx(MTriangs,MTriangs,i);i++;}//prevents bug
    printf("..Triangles.OK");

    // read Vertices
    SMDL7Frame*frame=(SMDL7Frame*)new u8[MDL7_MVERTS*26];

//listframe1
    if(hdrx.typex<12||hdrx.typex>26||hdrx.typey<12||hdrx.typey>26){
     printf("\nError invalid typex %d/typey %d",hdrx.typex,hdrx.typey);}
    s32 seekv=hdrx.oftexco+(nsv*8+ntr*hdrx.typex);
    SMDL7Vertex*vertx=new SMDL7Vertex[nvt];file->seek(seekv);
    s32 ii=0;while(ii<nvt){v3f v;file->read(&v,12);
     vertx[ii].vtx[0]=v.X;vertx[ii].vtx[1]=v.Y;
     vertx[ii].vtx[2]=v.Z;file->read(&v,4);
     if(hdrx.typey==26){file->read(&v,10);}ii++;}
//listframe1
    SMDL7Vertex*vertx1=new SMDL7Vertex[nvt];
    u32 ofsfrn[nfr][2];
    ii=0;while(ii<nfr){ofsfrn[ii][0]=0;ofsfrn[ii][1]=0;ii++;}
//listframen
/////
    if(hdrx.nbones){hdrx.framsiz=hdrx.nverts*hdrx.typey;}
    s32 seekx=hdrx.oftexco+hdrx.framsiz+(nsv*8+ntr*hdrx.typex),seekd=0;
    //printf("\n>||Z*.skx(%d)[%d %d]\n [%d %d]\n pos(%d)",seekx,hdrx.oftexco,
     //hdrx.framsiz,nsv*8,ntr*hdrx.typex,file->getPos());
    i=0;frame->vcnt=0;while(i<nfr+1){s32 o0=0;
     // store frame data
     SFDatax fdata;fdata.start=i;fdata.end=i;fdata.fps=1;
     //s32 rr=0;printf("\n");while(rr<=16){printf("%c",frame->name[rr]);rr++;}//spec
     if(frame->vcnt<=0){o0++;file->seek(seekx+seekd);
      file->read(frame,sizeof(SMDL7Frame));seekd+=24;}//printf("next-> %d",frame->vcnt);}
     if(frame->vcnt>nvt&&!o0){printf("\nError-%d>nvt",frame->vcnt);}
     if(frame->vcnt>0&&frame->vcnt<=nvt&&!o0){ofsfrn[i-1][0]=frame->vcnt;
      ofsfrn[i-1][1]=seekx+seekd;if(hdrx.typey!=26){seekd+=16*frame->vcnt;}
      else{seekd+=26*frame->vcnt;}file->seek(seekx+seekd);
      //printf("\n>*.%d.(%d) ofsfrn:(%d)(%d)",frame->vcnt,seekx+seekd,ofsfrn[i][0],ofsfrn[i][1]);
      file->read(frame,sizeof(SMDL7Frame));seekd+=24;}//printf("next-> %d",frame->vcnt);}
     if(frame->name[0]&&i<nfr){
      s32 s=0;while(s<10&&!!frame->name[s]&&frame->name[s]!=' '&&(frame->name[s]<'0'||
       frame->name[s]>'9'||frame->name[s+1]>=58&&s<=8)){
       fdata.name+=frame->name[s];fdata.name.make_lower();s++;}
      if(!FData.empty()&&FData[FData.size()-1].name==fdata.name){++FData[FData.size()-1].end;}
      else{FData.push_back(fdata);}
     }i++;}
/////


    i=0;while(i<nfr){
//tt
     ii=0;while(ii<nvt){
      vertx1[ii].vtx[0]=vertx[ii].vtx[0];
      vertx1[ii].vtx[1]=vertx[ii].vtx[1];
      vertx1[ii].vtx[2]=vertx[ii].vtx[2];ii++;}
     if(ofsfrn[i][0]>0){
      file->seek(ofsfrn[i][1]);ii=0;
      while(ii<ofsfrn[i][0]&&hdrx.typey==26){v3f v;u16 i0;
       file->read(&v,12);file->read(&i0,2);
       vertx1[i0].vtx[0]=v.X;vertx1[i0].vtx[1]=v.Y;
       vertx1[i0].vtx[2]=v.Z;file->read(&v,12);ii++;}
      while(ii<ofsfrn[i][0]&&hdrx.typey!=26){v3f v;u16 i0;
       file->read(&v,12);file->read(&i0,2);
       vertx1[i0].vtx[0]=v.X;vertx1[i0].vtx[1]=v.Y;
       vertx1[i0].vtx[2]=v.Z;file->read(&i0,2);ii++;}
     }
//tt
     // add vertices
     s32 s=0;while(s<nvt){v3f v;
      v.X=vertx1[s].vtx[0];
      v.Z=vertx1[s].vtx[1];
      v.Y=vertx1[s].vtx[2];
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

    // nsv
    if(!nsv){nsv=1;}

    // return
    calculateBoundingBox();
    return 1;
}


//! calculates the bounding box
void IMDL7Mesh::calculateBoundingBox()
{
    IntpBfr->BoundingBox.reset(0,0,0);
    if(VertList[0].size()){s32 i=0;while(i<VertList[0].size()){
      IntpBfr->BoundingBox.addInternalPoint(VertList[0].pointer()[i]);i++;}
    }
}


//! sets a flag of all contained materials to a new value
void IMDL7Mesh::setMaterialFlag(video::E_MATERIAL_FLAG flag,bool newvalue)
{
    IntpBfr->Material.setFlag(flag,newvalue);
}


//! returns an axis aligned bounding box
const core::aabbox3d<f32>&IMDL7Mesh::getBoundingBox() const
{
    return IntpBfr->BoundingBox;
}


//! set user axis aligned bounding box
void IMDL7Mesh::setBoundingBox(const core::aabbox3df& box)
{
    IntpBfr->BoundingBox=box;
}


//! Returns the type of the animated mesh.
E_ANIMATED_MESH_TYPE IMDL7Mesh::getMeshType() const
{
    return (E_ANIMATED_MESH_TYPE)89;
}

//! Returns frame loop data for a special MDL7 animation type.
bool IMDL7Mesh::getFrameLoop(const c8*name,s32 &outStart,s32 &outEnd,s32 &outFPS)const{
    s32 i=0;while(i<FData.size()){
     if(FData[i].name==name){
      outStart=FData[i].start<<MDL7_FRAME2;
      outEnd=FData[i].end<<MDL7_FRAME2;
      outEnd+=!MDL7_FRAME2?1:(1<<MDL7_FRAME2)-1;
      outFPS=FData[i].fps<<MDL7_FRAME2;
      return 1;}i++;}
    return 0;
}


//! Returns amount of mdl7 animations in this file.
u32 IMDL7Mesh::getAnimationCount()const
{
    return FData.size();
}


//! Returns name of mdl7 animation.
const c8*IMDL7Mesh::getAnimationName(u32 nr)const
{
    if(nr>=FData.size()){return 0;}

    return FData[nr].name.c_str();
}


video::ITexture*IMDL7Mesh::getTexture(bool nr)const
{
    if(IntpBfr->Material.getTexture(1)&&nr&&nsk>1){return IntpBfr->Material.getTexture(1);}
    return IntpBfr->Material.getTexture(0);
}


video::IImage*IMDL7Mesh::getSkin(bool nr)const
{
    if(skbm[1]&&nr){return skbm[1];}
    return skbm[0];
}


void IMDL7Mesh::setSkin(bool nr,video::IImage*im)
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
bool IMDL7Mesh::WriteM8M(const c8*fname,bool nr,const c8*mnx,const u16*mvx,bool ni){
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

core::array<IMDL7Mesh::v3f>*IMDL7Mesh::getVertList()const{return VertList;}
core::array<IMDL7Mesh::SFDatax> IMDL7Mesh::getFData()const{return FData;}
IMDL7Mesh::STCoordx*IMDL7Mesh::getMTCoords()const{return MTCoords;}
IMDL7Mesh::STriangx*IMDL7Mesh::getMTriangs()const{return MTriangs;}
s32 IMDL7Mesh::getnsk()const{return nsk;}
s32 IMDL7Mesh::getnvt()const{return nvt;}
s32 IMDL7Mesh::getntr()const{return ntr;}
s32 IMDL7Mesh::getnsv()const{return nsv;}
s32 IMDL7Mesh::getskt()const{return skt;}
s32 IMDL7Mesh::getskw()const{return skw;}
s32 IMDL7Mesh::getskh()const{return skh;}

} // end namespace scene
} // end namespace irr

//////////////////////////////////////////////////cpp
namespace irr
{
namespace scene
{

//! Constructor
CMDL7MeshLoader::CMDL7MeshLoader(scene::ISceneManager*smgr):SceneManager(smgr){}


bool CMDL7MeshLoader::isALoadableFileExtension(const c8*filename) const
{
    return (strstr(filename,".mdl"));
}


//! creates/loads an animated mesh from the file.
//! \return Pointer to the created mesh. Returns 0 if loading failed.
//! If you no longer need the mesh, you should call IAnimatedMesh::drop().
//! See IReferenceCounted::drop() for more information.
IAnimatedMesh*CMDL7MeshLoader::createMesh(io::IReadFile*file)
{
    IAnimatedMesh*msh=new IMDL7Mesh(SceneManager);
    if(msh)
    {
        if(((IMDL7Mesh*)msh)->loadFile(file)){return msh;}
        msh->drop();
    }
    return 0;
}

} // end namespace scene
} // end namespace irr

#endif // _IRR_COMPILE_WITH_MDL7_LOADER_
