#include <irrlicht.h>

#include "CBox2DAnimatedImageMesh.h"

bool CBox2DAnimatedImageMesh::addImageMeshBuffer(IImage *pSrc, int iStartX, int iStartY) {
  SMesh *aMesh=new SMesh();
  aMesh->addMeshBuffer(new SMeshBuffer());

  SMeshBuffer *aBuffer=reinterpret_cast<SMeshBuffer*>(aMesh->getMeshBuffer(aMesh->getMeshBufferCount()-1)),
              *aHorBuffer=new SMeshBuffer(),
              *aBox2DBuffer=new SMeshBuffer();

  int iHit=0;
  s32 x=0,y=0;
  for (x=iStartX; x<iStartX+_TILE_SIZE_X && x<pSrc->getDimension().Width-1; x++)
    for (y=iStartY; y<iStartY+_TILE_SIZE_Y && y<pSrc->getDimension().Height-1; y++) {
      u32 red0=pSrc->getPixel(x  ,y  ).getRed(),
          red1=pSrc->getPixel(x+1,y  ).getRed(),
          red2=pSrc->getPixel(x+1,y+1).getRed(),
          red3=pSrc->getPixel(x  ,y+1).getRed();

    if (!red0 || !red1 || !red2 || !red3) iHit=1;
    if (!iHit && ((red0!=0 && red0!=0xFF) || (red1!=0 && red1!=0xFF) || (red2!=0 && red2!=0xFF) || (red3!=0 && red3!=0xFF))) iHit=2;
  }

  if (iHit==2) {
    f32 xTex=((f32)x-iStartX)/_TILE_SIZE_X,yTex=((f32)y-iStartY)/_TILE_SIZE_Y;

    S3DVertex p1=S3DVertex(5*iStartX,-(5*iStartY),_TILE_Z,5*iStartX,-(5*iStartY),_TILE_Z+1,SColor(0xFF,0xFF,0xFF,0xFF),   0,   0),
              p2=S3DVertex(5*x      ,-(5*iStartY),_TILE_Z,5*x      ,-(5*iStartY),_TILE_Z+1,SColor(0xFF,0xFF,0xFF,0xFF),xTex,   0),
              p3=S3DVertex(5*x      ,-(5*y      ),_TILE_Z,5*x      ,-(5*y      ),_TILE_Z+1,SColor(0xFF,0xFF,0xFF,0xFF),xTex,yTex),
              p4=S3DVertex(5*iStartX,-(5*y      ),_TILE_Z,5*iStartX,-(5*y      ),_TILE_Z+1,SColor(0xFF,0xFF,0xFF,0xFF),   0,yTex);

    S3DVertex vertices[]={p1,p2,p3,p4};
    u16 indices[]={0,1,2,0,2,3};
    aBuffer->append(vertices,4,indices,6);
  }
  else
    if (iHit==1) {
      s32 pIndices[_TILE_SIZE_X+2][_TILE_SIZE_Y+2][2];

      for (x=0; x<_TILE_SIZE_X+1; x++) for (y=0; y<_TILE_SIZE_Y+1; y++) for (u16 z=0; z<2; z++) pIndices[x][y][z]=-1;

      for (x=iStartX; x<iStartX+_TILE_SIZE_X+1 && x<pSrc->getDimension().Width+1; x++)
        for (y=iStartY; y<iStartY+_TILE_SIZE_Y+1 && y<pSrc->getDimension().Height+1; y++) {
          if (pSrc->getPixel(x,y).getRed()!=0xFF) {
            f32 tx=((f32)(x-iStartX))/((f32)_TILE_SIZE_X),
                ty=((f32)(y-iStartY))/((f32)_TILE_SIZE_Y),
                xCoord=_TILE_STEP*(x-1)+(_TILE_STEP),yCoord=_TILE_STEP*(y-1)+(_TILE_STEP);

            SColor col=SColor(0xFF,0xFF,0xFF,0xFF);
            S3DVertex v=S3DVertex(xCoord,-yCoord,_TILE_Z,xCoord,-yCoord,_TILE_Z+1,col,tx,ty);
            pIndices[x-iStartX][y-iStartY][0]=aBuffer->getVertexCount();
            aBuffer->Vertices.push_back(v);
          }
        }

      f32 mat2pos=0.0f;
      for (x=iStartX; x<iStartX+_TILE_SIZE_X && x<pSrc->getDimension().Width-1; x++)
        for (y=iStartY; y<iStartY+_TILE_SIZE_Y && y<pSrc->getDimension().Height-1; y++) {
          u16 addX[4]={0,1,1,0},addY[4]={0,0,1,1},i;
          u32 red[4];
          S3DVertex v[4],vb[4];
          int xx=x-iStartX,yy=y-iStartY;
          bool addToBox2D=false;

          array<u16> aVert;
          aVert.clear();

          for (i=0; i<4; i++) {
            SColor w=SColor(0xFF,0xFF,0xFF,0xFF),b=SColor(0xFF,0x80,0x80,0x80);

            red[i]=pSrc->getPixel(x+addX[i],y+addY[i]).getRed();

            f32 xCoord=_TILE_STEP*x+(_TILE_STEP*addX[i]),yCoord=_TILE_STEP*y+(_TILE_STEP*addY[i]);

            v [i]=S3DVertex(xCoord,-yCoord,_TILE_Z    ,xCoord,-yCoord,_TILE_Z    +1,w,0,mat2pos);
            vb[i]=S3DVertex(xCoord,-yCoord,_TILE_DEPTH,xCoord,-yCoord,_TILE_DEPTH+1,b,1,mat2pos);

            if (red[i]!=0xFF) aVert.push_back(pIndices[xx+addX[i]][yy+addY[i]][0]);

            if (red[i]==0) addToBox2D=true;
          }

          switch (aVert.size()) {
            case 3:
              for (i=0; i<3; i++) {
                aBuffer->Indices.push_back(aVert[i]);
                if (addToBox2D) {
                  aBox2DBuffer->Indices.push_back(aBox2DBuffer->Indices.size());
                  aBox2DBuffer->Vertices.push_back(aBuffer->Vertices[aVert[i]]);
                }
              }
              break;

            case 4:
              u16 pIdx[]={ 0,1,2,0,2,3 };
              for (i=0; i<6; i++) {
                aBuffer->Indices.push_back(aVert[pIdx[i]]);
                if (addToBox2D) {
                  aBox2DBuffer->Indices.push_back(aBox2DBuffer->Indices.size());
                  aBox2DBuffer->Vertices.push_back(aBuffer->Vertices[aVert[pIdx[i]]]);
                }
              }
              break;
          }

          u16 indexCnt=aHorBuffer->getIndexCount();
          u16 triCheck[]={ 0, 2, 1, 3, /*|*/ 3, 1, 0, 2, /*|*/ 2, 0, 3, 1, /*|*/ 1, 3, 2, 0 };
          for (i=0; i<16; i+=4) {
            if (red[triCheck[i]]!=0xFF && red[triCheck[i+1]]!=0xFF && red[triCheck[i+2]]!=0xFF && red[triCheck[i+3]]==0xFF) {
              v [triCheck[i  ]].TCoords.X=0; v [triCheck[i  ]].TCoords.Y=mat2pos;
              vb[triCheck[i  ]].TCoords.X=1; vb[triCheck[i  ]].TCoords.Y=mat2pos;
              mat2pos+=1.0f/(f32)_TILE_SIZE_Y; while (mat2pos>1) mat2pos-=1;
              v [triCheck[i+1]].TCoords.X=0; v [triCheck[i+1]].TCoords.Y=mat2pos;
              vb[triCheck[i+1]].TCoords.X=1; vb[triCheck[i+1]].TCoords.Y=mat2pos;
              mat2pos+=1.0f/(f32)_TILE_SIZE_Y; while (mat2pos>1) mat2pos-=1;

              aHorBuffer->Indices.push_back(indexCnt++); aHorBuffer->Indices.push_back(indexCnt++);
              aHorBuffer->Indices.push_back(indexCnt++); aHorBuffer->Indices.push_back(indexCnt++);
              aHorBuffer->Indices.push_back(indexCnt++); aHorBuffer->Indices.push_back(indexCnt++);

              aHorBuffer->Vertices.push_back(v [triCheck[i  ]]);
              aHorBuffer->Vertices.push_back(v [triCheck[i+1]]);
              aHorBuffer->Vertices.push_back(vb[triCheck[i  ]]);

              aHorBuffer->Vertices.push_back(v [triCheck[i+1]]);
              aHorBuffer->Vertices.push_back(vb[triCheck[i+1]]);
              aHorBuffer->Vertices.push_back(vb[triCheck[i  ]]);
            }
          }

          indexCnt=aHorBuffer->getIndexCount();
          u16 biCheck[]={ 0, 1, 2, 3, /*|*/ 3, 0, 1, 2, /*|*/ 1, 2, 0, 3, /*|*/ 2, 3, 0, 1 };

          for (i=0; i<16; i+=4) {
            if (red[biCheck[i]]!=0xFF && red[biCheck[i+1]]!=0xFF && red[biCheck[i+2]]==0xFF && red[biCheck[i+3]]==0xFF) {
              v [biCheck[i  ]].TCoords.X=0; v [biCheck[i  ]].TCoords.Y=mat2pos;
              vb[biCheck[i  ]].TCoords.X=1; vb[biCheck[i  ]].TCoords.Y=mat2pos;
              mat2pos+=1.0f/(f32)_TILE_SIZE_Y; if (mat2pos>1) mat2pos=1;
              v [biCheck[i+1]].TCoords.X=0; v [biCheck[i+1]].TCoords.Y=mat2pos;
              vb[biCheck[i+1]].TCoords.X=1; vb[biCheck[i+1]].TCoords.Y=mat2pos;
              mat2pos+=1.0f/(f32)_TILE_SIZE_Y; while (mat2pos>1) mat2pos-=1;

              aHorBuffer->Indices.push_back(indexCnt++); aHorBuffer->Indices.push_back(indexCnt++);
              aHorBuffer->Indices.push_back(indexCnt++); aHorBuffer->Indices.push_back(indexCnt++);
              aHorBuffer->Indices.push_back(indexCnt++); aHorBuffer->Indices.push_back(indexCnt++);

              aHorBuffer->Vertices.push_back(v [biCheck[i  ]]);
              aHorBuffer->Vertices.push_back(v [biCheck[i+1]]);
              aHorBuffer->Vertices.push_back(vb[biCheck[i  ]]);

              aHorBuffer->Vertices.push_back(v [biCheck[i+1]]);
              aHorBuffer->Vertices.push_back(vb[biCheck[i+1]]);
              aHorBuffer->Vertices.push_back(vb[biCheck[i  ]]);
            }
          }
        }
    }

  if (aBuffer->getVertexCount()>0) {
    aBuffer->recalculateBoundingBox();
    m_iMeshCnt+=aBuffer->getVertexCount()+aBox2DBuffer->getVertexCount()+aHorBuffer->getVertexCount();

    if (aBox2DBuffer->getVertexCount()) {
      aMesh->addMeshBuffer(aBox2DBuffer);
    }

    if (aHorBuffer->getVertexCount()) {
      if (aMesh->getMeshBufferCount()==1) aMesh->addMeshBuffer(aBox2DBuffer);
      aMesh->addMeshBuffer(aHorBuffer);
    }
    this->addMesh(aMesh);
    return true;
  }
  else {
    aMesh->drop();
    delete aBuffer;
    delete aBox2DBuffer;

    return false;
  }
}

CBox2DAnimatedImageMesh::CBox2DAnimatedImageMesh(const c8 *pImageName, IVideoDriver *pDriver) : SAnimatedMesh() {
  m_iMeshCnt=0;

  m_sImageName.append(pImageName);

  IImage *imgLevel=pDriver->createImageFromFile(pImageName);
  for (s32 x=0; x<imgLevel->getDimension().Width-1; x+=_TILE_SIZE_X)
    for (s32 y=0; y<imgLevel->getDimension().Height-1; y+=_TILE_SIZE_Y) {
      addImageMeshBuffer(imgLevel,x,y);
    }
  imgLevel->drop();

  printf("CBox2DAnimatedImageMesh: %i meshbuffers, %i meshes\n",this->getFrameCount(),m_iMeshCnt);
}


