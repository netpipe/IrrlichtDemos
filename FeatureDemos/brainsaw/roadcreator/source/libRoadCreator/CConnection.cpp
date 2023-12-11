  #include <CTextureParameters.h>
  #include <CConnection.h>
  #include <CSegment.h>

  #define _RED video::SColor(0xFF,0xFF,0xE0,0xE0)
  #define _GREEN video::SColor(0xFF,0xE0,0xFF,0xE0)
  #define _BLUE video::SColor(0xFF,0xE0,0xE0,0xFF)

using namespace irr;


void CConnection::addToTempVectorBuffer(core::array<core::vector3df> *vTemp, core::vector3df *p1, core::vector3df vNormBase, core::vector3df vNormWall) {
  for (u32 j=0; j<_CONNECTION_NUMBER_OF_BUFFERS; j++) {
    switch (j) {
      //Road
      case 0: {
          core::vector3df point1=p1[0],point2=p1[1];
          if (m_bWalls[0]) {
            core::vector3df vDir=p1[0]-p1[1];
            vDir.normalize();
            point1=p1[0]-m_fWallWidth*vDir;
          }

          if (m_bWalls[1]) {
            core::vector3df vDir=p1[0]-p1[1];
            vDir.normalize();
            point2=p1[1]+m_fWallWidth*vDir;
          }
          vTemp[0].push_back(point1);
          vTemp[0].push_back(point2);
        }
        break;

      //Basement
      case 1:
        vTemp[1].push_back(p1[0]+m_fOffset*vNormBase);
        vTemp[1].push_back(p1[1]+m_fOffset*vNormBase);
        break;

      //Left Basement
      case 2:
        vTemp[2].push_back(p1[0]);
        vTemp[2].push_back(p1[0]+m_fOffset*vNormBase);
        break;

      //Right Basement
      case 3:
        vTemp[3].push_back(p1[1]);
        vTemp[3].push_back(p1[1]+m_fOffset*vNormBase);
        break;

      //Left outer wall
      case 4:
        if (m_bWalls[0]) {
          vTemp[4].push_back(p1[0]);
          vTemp[4].push_back(p1[0]-m_fWallHeight*vNormWall);
        }
        break;

      //Right outer wall
      case 5:
        if (m_bWalls[1]) {
          vTemp[5].push_back(p1[1]);
          vTemp[5].push_back(p1[1]-m_fWallHeight*vNormWall);
        }
        break;

      //Left inner wall
      case 6:
        if (m_bWalls[0]) {
          core::vector3df vDir=p1[0]-p1[1];
          vDir.normalize();
          vTemp[6].push_back(p1[0]-m_fWallWidth*vDir);
          vTemp[6].push_back(p1[0]-m_fWallWidth*vDir-m_fWallHeight*vNormWall);
        }
        break;

      //Right inner wall
      case 7:
        if (m_bWalls[1]) {
          core::vector3df vDir=p1[1]-p1[0];
          vDir.normalize();
          vTemp[7].push_back(p1[1]-m_fWallWidth*vDir);
          vTemp[7].push_back(p1[1]-m_fWallWidth*vDir-m_fWallHeight*vNormWall);
        }
        break;

      //Left wall top
      case 8:
        if (m_bWalls[0]) {
          vTemp[8].push_back(vTemp[4][vTemp[4].size()-1]);
          vTemp[8].push_back(vTemp[6][vTemp[6].size()-1]);
        }
        break;

      //Right wall top
      case 9:
        if (m_bWalls[1]) {
          vTemp[9].push_back(vTemp[5][vTemp[5].size()-1]);
          vTemp[9].push_back(vTemp[7][vTemp[7].size()-1]);
        }
        break;
    }
  }
}

void CConnection::fillMeshBuffer(scene::IMeshBuffer *pBuffer, core::array<core::vector3df> vTemp, u32 iIdx) {
  if (pBuffer==NULL || vTemp.size()<4) return;

  bool bFlip=iIdx==1 || iIdx==2 || iIdx==5 || iIdx==6 || iIdx==9;
  if (m_bFlipVertices) bFlip=!bFlip;
  bool bFlipNorm=iIdx==1 || iIdx==2 || iIdx==5 || iIdx==6 || iIdx==9;
  if (m_bFlipVertices) bFlipNorm=!bFlipNorm;

  core::array<video::S3DVertex> aVerts;
  core::array<u16> aIndices;

  f32 fWidth=(vTemp[0]-vTemp[1]).getLength(),fTex=0.0f;

  bool bColor=false;

  for (u32 i=0; i<vTemp.size(); i++) {
    core::vector3df v1,v2;

    if (i<vTemp.size()-2) {
      if (i%2) {
        v1=vTemp[i]-vTemp[i-1];
        v2=vTemp[i]-vTemp[i+2];
      }
      else {
        v1=vTemp[i]-vTemp[i+1];
        v2=vTemp[i]-vTemp[i+2];
      }
    }
    else {
      if (i==vTemp.size()-2) {
        v1=vTemp[i]-vTemp[i+1];
        v2=vTemp[i]-vTemp[i-2];
      }
      else {
        v1=vTemp[i]-vTemp[i-1];
        v2=vTemp[i]-vTemp[i-2];
      }
    }

    core::vector3df vNorm=v1.crossProduct(v2);
    vNorm.normalize();
    if (bFlipNorm) vNorm=-vNorm;
    if (i>=vTemp.size()-2) vNorm=-vNorm;
    bFlipNorm=!bFlipNorm;

    video::SColor cCol=bColor?video::SColor(0xFF,0xFF,0xFF,0xFF):video::SColor(0xFF,0xFF,0,0);
    core::vector2df vTex;
    
    switch (m_pTexParams[iIdx]->getRotate()) {
      case 1: vTex=core::vector2df( fTex         , i%2?1.0f:0.0f); break;
      case 2: vTex=core::vector2df( i%2?1.0f:0.0f,-fTex         ); break;
      case 3: vTex=core::vector2df(-fTex         , i%2?1.0f:0.0f); break;
      default: vTex=core::vector2df(i%2?1.0f:0.0f, fTex         ); break;
    }

    vTex.X*=m_pTexParams[iIdx]->getScaleX();
    vTex.Y*=m_pTexParams[iIdx]->getScaleY();
    vTex.X+=m_pTexParams[iIdx]->getOffsetX();
    vTex.Y+=m_pTexParams[iIdx]->getOffsetY();

    video::S3DVertex v=video::S3DVertex(vTemp[i],vNorm,cCol,vTex);
    aVerts.push_back(v);
    if (i%2) {
      fTex+=m_pTexParams[iIdx]->getStretch()?1.0f/vTemp.size():v2.getLength()/fWidth;
      bColor=!bColor;
    }
  }

  for (u16 i=0; i<vTemp.size()-2; i+=2) {
    if (bFlip) {
      aIndices.push_back(i  );
      aIndices.push_back(i+2);
      aIndices.push_back(i+1);

      aIndices.push_back(i+2);
      aIndices.push_back(i+3);
      aIndices.push_back(i+1);
    }
    else {
      aIndices.push_back(i  );
      aIndices.push_back(i+1);
      aIndices.push_back(i+2);

      aIndices.push_back(i+2);
      aIndices.push_back(i+1);
      aIndices.push_back(i+3);
    }
  }

  pBuffer->append(aVerts.const_pointer(),aVerts.size(),aIndices.const_pointer(),aIndices.size());

  if (pBuffer->getVertexCount()>0) {
    video::ITexture *pTex=m_pDrv->getTexture(m_pTexParams[iIdx]->getTexture());
    m_pMeshBuffer[iIdx]->getMaterial().setTexture(0,pTex);
  }
}

/**
 * Recalculates the meshbuffer after some attribute was changed
 * @see CConnection::update
 */
void CConnection::recalcMeshBuffer() {
  //Delete all meshbuffers allocated previously
  for (u32 i=0; i<_CONNECTION_NUMBER_OF_BUFFERS; i++) {
    //if (m_pMeshBuffer[i]!=NULL)  m_pMeshBuffer[i]->drop();
    m_pMeshBuffer[i]=NULL;
  }

  //Return without doing any further things if the connection is not connected
  if (m_pSegment1==NULL || m_pSegment2==NULL) return;

  //Return without doing any further things if the connection is not connected
  if (m_pSegment1==NULL || m_pSegment2==NULL) return;

  core::vector3df p1[2],p2[2];

  //initialize the beginning and the ending point of the first line that is calculated
  switch (m_iSegment1Border) {
    case 0: p1[0]=m_pSegment1->getPoint(0); p1[1]=m_pSegment1->getPoint(1); break;
    case 1: p1[0]=m_pSegment1->getPoint(0); p1[1]=m_pSegment1->getPoint(3); break;
    case 2: p1[0]=m_pSegment1->getPoint(3); p1[1]=m_pSegment1->getPoint(2); break;
    case 3: p1[0]=m_pSegment1->getPoint(2); p1[1]=m_pSegment1->getPoint(1); break;
  }

  //initialize the beginning and the ending point of the second line that is calculated
  switch (m_iSegment2Border) {
    case 0: p2[0]=m_pSegment2->getPoint(0); p2[1]=m_pSegment2->getPoint(1); break;
    case 1: p2[0]=m_pSegment2->getPoint(0); p2[1]=m_pSegment2->getPoint(3); break;
    case 2: p2[0]=m_pSegment2->getPoint(3); p2[1]=m_pSegment2->getPoint(2); break;
    case 3: p2[0]=m_pSegment2->getPoint(2); p2[1]=m_pSegment2->getPoint(1); break;
  }

  if (m_bFlipConnection) {
    core::vector3df v=p2[1];
    p2[1]=p2[0];
    p2[0]=v;
  }

  core::vector3df vBezierL[4],vBezierR[4];

  switch (m_eType) {
    case eBezier1:
      vBezierL[0]=p1[0]; vBezierL[1]=p2[0];
      vBezierR[0]=p1[1]; vBezierR[1]=p2[1];
      break;

    case eBezier2:
      vBezierL[0]=p1[0]; vBezierL[1]=m_vHelpPoints[0]; vBezierL[2]=p2[0];
      vBezierR[0]=p1[1]; vBezierR[1]=m_vHelpPoints[2]; vBezierR[2]=p2[1];
      break;

    case eBezier3:
      vBezierL[0]=p1[0]; vBezierL[1]=m_vHelpPoints[0]; vBezierL[2]=m_vHelpPoints[1]; vBezierL[3]=p2[0];
      vBezierR[0]=p1[1]; vBezierR[1]=m_vHelpPoints[2]; vBezierR[2]=m_vHelpPoints[3]; vBezierR[3]=p2[1];
      break;
  }

  core::array<core::vector3df> vTemp[_CONNECTION_NUMBER_OF_BUFFERS];
  core::vector3df vNormBase=m_pSegment1->getNormalBaseVector(),
                  vNormWall=m_pSegment1->getWallNormal();
  //if (m_bFlipVertices) vNorm=-vNorm;

  for (u32 i=0; i<_CONNECTION_NUMBER_OF_BUFFERS; i++) vTemp[i].clear();

  for (u32 i=1; i<=m_iSteps; i++) {
    addToTempVectorBuffer(vTemp,p1,vNormBase,-vNormWall);

    f32 fStep=((f32)i)*(1.0f/((f32)m_iSteps));

    switch (m_eType) {
      case eBezier1:
        p1[0]=getBezier1(vBezierL,fStep);
        p1[1]=getBezier1(vBezierR,fStep);
        break;

      case eBezier2:
        p1[0]=getBezier2(vBezierL,fStep);
        p1[1]=getBezier2(vBezierR,fStep);
        break;

      case eBezier3:
        p1[0]=getBezier3(vBezierL,fStep);
        p1[1]=getBezier3(vBezierR,fStep);
        break;
    }

    core::vector3df v1=p1[0]-p1[1],v2=p1[0]-vTemp[0][vTemp[0].size()-1];
    vNormBase=v1.crossProduct(v2);
    vNormWall=-vNormBase;
    vNormBase.normalize();
    vNormWall.normalize();
    if (m_bFlipVertices) {
      vNormBase=-vNormBase;
      vNormWall=-vNormWall;
    }
  }

  addToTempVectorBuffer(vTemp,p2,
                        m_bFlipVertices? m_pSegment2->getNormalBaseVector(): m_pSegment2->getNormalBaseVector(),
                        m_bFlipVertices?-m_pSegment2->getWallNormal      ():-m_pSegment2->getWallNormal());

  for (u32 i=0; i<_CONNECTION_NUMBER_OF_BUFFERS; i++) {
    if (m_pMeshBuffer[i]==NULL) m_pMeshBuffer[i]=new scene::SMeshBuffer();
    fillMeshBuffer(m_pMeshBuffer[i],vTemp[i],i);
  }
}

/**
 * Init the Bezier helppoints. Nothing special here. We just
 * calculate the point in the middle between the chosen segment borders
 * @see CConnection::setSegment1
 * @see CConnection::setSegment2
 * @see CConnection::setSegment1Border
 * @see CConnection::setSegment2Border
 * @see CConnection::setType
 */
void CConnection::calculateHelpPoints() {
  if (m_pSegment1==NULL || m_pSegment2==NULL) return;

  core::vector3df v[4],v2[2];

  switch (m_iSegment1Border) {
    case 0: v[0]=m_pSegment1->getPoint(0); v[1]=m_pSegment1->getPoint(1); v2[0]=m_pSegment1->getPoint(3); break;
    case 1: v[0]=m_pSegment1->getPoint(0); v[1]=m_pSegment1->getPoint(3); v2[0]=m_pSegment1->getPoint(1); break;
    case 2: v[0]=m_pSegment1->getPoint(3); v[1]=m_pSegment1->getPoint(2); v2[0]=m_pSegment1->getPoint(0); break;
    case 3: v[0]=m_pSegment1->getPoint(2); v[1]=m_pSegment1->getPoint(1); v2[0]=m_pSegment1->getPoint(3); break;
    default: printf("Invalid value for \"segment 1 border\"!\n"); return;
  }

  switch (m_iSegment2Border) {
    case 0: v[2]=m_pSegment2->getPoint(0); v[3]=m_pSegment2->getPoint(1); v2[1]=m_pSegment2->getPoint(3); break;
    case 1: v[2]=m_pSegment2->getPoint(0); v[3]=m_pSegment2->getPoint(3); v2[1]=m_pSegment2->getPoint(1); break;
    case 2: v[2]=m_pSegment2->getPoint(3); v[3]=m_pSegment2->getPoint(2); v2[1]=m_pSegment2->getPoint(0); break;
    case 3: v[2]=m_pSegment2->getPoint(2); v[3]=m_pSegment2->getPoint(1); v2[1]=m_pSegment2->getPoint(3); break;
    default: printf("Invalid value for \"segment 2 border\"!\n"); return;
  }

  switch (m_eType) {
    case eBezier1: break;

    case eBezier2:
      if (m_bFlipConnection) {
        core::vector3df vDir=v2[0]-v[0],
                        vLen=v[0]-v[2];

        f32 fLen=vLen.getLength();

        vDir.normalize();

        core::line3df cLine[2]={
                        core::line3df(v[0],v[0]-fLen*vDir),
                        core::line3df(v[1],v[1]-fLen*vDir)
                      };

        vDir=v2[1]-v[2];
        vDir.normalize();

        core::vector3df vMinDist[2]={ core::vector3df(0.0f), core::vector3df(0.0f) };
        f32 fMinDist[2]={ 1000.0f*fLen, 1000.0f*fLen };

        for (u32 i=0; i<2; i++)
          for (f32 f=0.0f; f<fLen; f+=0.01f) {
            core::vector3df vDummy=v[i+2]-f*vDir;
            if (cLine[i==0?1:0].getClosestPoint(vDummy).getDistanceFrom(vDummy)<fMinDist[i]) {
              fMinDist[i]=cLine[i==0?1:0].getClosestPoint(vDummy).getDistanceFrom(vDummy);
              vMinDist[i]=vDummy;
            }
          }

        m_vHelpPoints[2]=cLine[1].start+(m_fHpOff[0]*(vMinDist[0]-cLine[1].start));
        m_vHelpPoints[0]=cLine[0].start+(m_fHpOff[2]*(vMinDist[1]-cLine[0].start));
      }
      else {
        core::vector3df vDir=v2[1]-v[2],
                        vLen=v[0]-v[2];

        f32 fLen=vLen.getLength();

        vDir.normalize();

        core::line3df cLine[2]={
                        core::line3df(v[2],v[2]-fLen*vDir),
                        core::line3df(v[3],v[3]-fLen*vDir)
                      };

        m_pDrv->draw3DLine(cLine[0].start,cLine[0].end,video::SColor(0xFF,0xFF,0,0));
        m_pDrv->draw3DLine(cLine[1].start,cLine[1].end,video::SColor(0xFF,0xFF,0,0));

        vDir=v2[0]-v[0];
        vDir.normalize();

        core::vector3df vMinDist[2]={ core::vector3df(0.0f), core::vector3df(0.0f) };
        f32 fMinDist[2]={ 1000.0f*fLen, 1000.0f*fLen };

        for (u32 i=0; i<2; i++)
          for (f32 f=0.0f; f<fLen; f+=0.01f) {
            core::vector3df vDummy=v[i]-f*vDir;
            if (cLine[i].getClosestPoint(vDummy).getDistanceFrom(vDummy)<fMinDist[i]) {
              fMinDist[i]=cLine[i].getClosestPoint(vDummy).getDistanceFrom(vDummy);
              vMinDist[i]=vDummy;
            }
          }

        m_pDrv->draw3DLine(v[0],vMinDist[0],video::SColor(0xFF,0,0xFF,0));
        m_pDrv->draw3DLine(v[1],vMinDist[1],video::SColor(0xFF,0,0xFF,0));

        //m_vHelpPoints[0]=vMinDist[0];
        //m_vHelpPoints[2]=vMinDist[1];

        m_vHelpPoints[0]=cLine[1].start+(m_fHpOff[0]*(vMinDist[0]-cLine[1].start));
        m_vHelpPoints[2]=cLine[0].start+(m_fHpOff[2]*(vMinDist[1]-cLine[0].start));
      }

      break;

    case eBezier3: {
        core::vector3df vDir=v2[0]-v[0],vLen=v[0]-v[2];
        vLen.Y=0;
        f32 fLen=vLen.getLength()/2.0f;

        vDir.normalize();

        m_vHelpPoints[0]=v[0]-fLen*m_fHpOff[0]*vDir;
        m_vHelpPoints[2]=v[1]-fLen*m_fHpOff[2]*vDir;

        vDir=v2[1]-v[2];
        vDir.normalize();

        if (m_bFlipConnection) {
          m_vHelpPoints[1]=v[3]-fLen*m_fHpOff[1]*vDir;
          m_vHelpPoints[3]=v[2]-fLen*m_fHpOff[3]*vDir;
        }
        else {
          m_vHelpPoints[1]=v[2]-fLen*m_fHpOff[1]*vDir;
          m_vHelpPoints[3]=v[3]-fLen*m_fHpOff[3]*vDir;
        }
      }
      break;
  }
}

/**
 * Get the next point using the Bezier1 algorithm
 * @param p array with the two points for the algorithm
 * @param fStep step number (0..1)
 * @return the next point
 * @see recalcMeshBuffer
 * @see render
 */
core::vector3df CConnection::getBezier1(core::vector3df p[], f32 fStep) {
  return (1-fStep)*p[0]+fStep*p[1];
}

/**
 * Get the next point using the Bezier2 algorithm
 * @param p array with the three points (start, help, end) for the algorithm
 * @param fStep step number (0..1)
 * @return the next point
 * @see recalcMeshBuffer
 * @see render
 */
core::vector3df CConnection::getBezier2(core::vector3df p[], f32 fStep) {
  return ((1-fStep)*(1-fStep))*p[0]+2*fStep*(1-fStep)*p[1]+fStep*fStep*p[2];
}

/**
 * Get the next point using the Bezier3 algorithm
 * @param p array with the four points (start, help1, help2, end) for the algorithm
 * @param fStep step number (0..1)
 * @return the next point
 * @see recalcMeshBuffer
 * @see render
 */
core::vector3df CConnection::getBezier3(core::vector3df p[], f32 fStep) {
  return (-p[0]+3*p[1]-3*p[2]+p[3])*fStep*fStep*fStep+(3*p[0]-6*p[1]+3*p[2])*fStep*fStep+(-3*p[0]+3*p[1])*fStep+p[0];
}

/**
 * Constructor
 * @param pDrv used Irrlicht videodriver
 */
CConnection::CConnection(IrrlichtDevice *pDevice, CTextureParameters *pInitTexture) : IRoadPart(pDevice->getVideoDriver()) {
  //initialize the segments
  m_pSegment1=NULL;
  m_pSegment2=NULL;

  m_pDevice=pDevice;
  m_pDrv=m_pDevice->getVideoDriver();
  m_pFs=m_pDevice->getFileSystem();

  //initialize the segment names
  m_sSegment1="";
  m_sSegment2="";

  //initialize the border indices and the selected index
  m_iSegment1Border=0;
  m_iSegment2Border=0;
  m_iSelectedPoint=-1;

  //Initialize the flip and the selected flags
  m_bFlipConnection=false;
  m_bFlipVertices  =false;
  m_bSelected      =false;

  //Initialize the helppoints
  m_vHelpPoints[0]=core::vector3df(0.0f,0.0f,0.0f);
  m_vHelpPoints[1]=core::vector3df(0.0f,0.0f,0.0f);
  m_vHelpPoints[2]=core::vector3df(0.0f,0.0f,0.0f);
  m_vHelpPoints[3]=core::vector3df(0.0f,0.0f,0.0f);

  //Default: 10 steps, offset 3.0
  m_iSteps=10;
  m_fOffset=3.0f;
  m_fWallWidth=2.5f;

  //Default type= Bezier1
  m_eType=eBezier1;

  //initialize the meshbuffer members and initialize the texture parameter objects
  for (u32 i=0; i<_CONNECTION_NUMBER_OF_BUFFERS; i++) {
    m_pMeshBuffer[i]=NULL;
    m_pTexParams[i]=new CTextureParameters();
    if (pInitTexture) pInitTexture[i].copyTo(m_pTexParams[i]);
  }

  m_fRoadWidth=0.0f;

  for (u32 i=0; i<4; i++) m_fHpOff[i]=1.0f;

  m_fWallHeight=10.0f;
  for (u32 i=0; i<2; i++) m_bWalls[i]=false;
}

/**
 * The destructor
 */
CConnection::~CConnection() {
  //Notify the segments of the deletion
  if (m_pSegment1!=NULL) m_pSegment1->delNotify(this);
  if (m_pSegment2!=NULL) m_pSegment2->delNotify(this);

  //Delete the texture parameters and the meshbuffers
  for (u32 i=0; i<6; i++) {
    if (m_pMeshBuffer[i]!=NULL) m_pMeshBuffer[i]->drop();
    delete m_pTexParams[i];
  }
}

void CConnection::setSegment1(CSegment *p) { m_iSegment1Border=0; setSegment1NoInit(p); calculateHelpPoints(); }
void CConnection::setSegment2(CSegment *p) { m_iSegment2Border=0; setSegment2NoInit(p); calculateHelpPoints(); }

void CConnection::setSegment1NoInit(CSegment *p) {
  //If are already assigned to a segment we must tell this segment we're leaving it
  if (m_pSegment1!=NULL) m_pSegment1->delNotify(this);
  m_pSegment1=p;
  //Inform the new segment of the connection
  m_pSegment1->addNotify(this);
  //recreate the object
  attributesChanged(m_pSegment1);
}

void CConnection::setSegment2NoInit(CSegment *p) {
  //If are already assigned to a segment we must tell this segment we're leaving it
  if (m_pSegment2!=NULL) m_pSegment2->delNotify(this);
  m_pSegment2=p;
  //Inform the new segment of the connection
  m_pSegment2->addNotify(this);
  //recreate the object
  attributesChanged(m_pSegment2);
}

void CConnection::setSegment1Border(u32 i) { if (i<4) m_iSegment1Border=i; calculateHelpPoints(); update(); }
void CConnection::setSegment2Border(u32 i) { if (i<4) m_iSegment2Border=i; calculateHelpPoints(); update(); }

void CConnection::setType(eConnectionType eType) { m_eType=eType; calculateHelpPoints(); }

void CConnection::setNumberOfSteps(u32 i) { m_iSteps=i; update(); }

void CConnection::setOffset(f32 f) {
  m_fOffset=f;
  update();
}

f32 CConnection::getOffset() { return m_fOffset; }

void CConnection::setHelpPoint(u32 i, core::vector3df v) {
  if (i<4) {
    m_vHelpPoints[i]=v;
    update();
  }
}

void CConnection::setFlipConnection(bool b) { m_bFlipConnection=b; update(); }
void CConnection::setFlipVertices  (bool b) { m_bFlipVertices  =b; update(); }
void CConnection::setSelected      (bool b) { m_bSelected      =b; }

CSegment *CConnection::getSegment1() { return m_pSegment1; }
CSegment *CConnection::getSegment2() { return m_pSegment2; }

u32 CConnection::getSegment1Border() { return m_iSegment1Border; }
u32 CConnection::getSegment2Border() { return m_iSegment2Border; }

CConnection::eConnectionType CConnection::getConnectionType() { return m_eType; }

u32 CConnection::getNumberOfSteps() { return m_iSteps; }

core::vector3df CConnection::getHelpPoint(u32 i) {
  static core::vector3df vErr=core::vector3df(0.0f,0.0f,0.0f);
  return i<4?m_vHelpPoints[i]:vErr;
}

bool CConnection::getFlipConnection() { return m_bFlipConnection; }
bool CConnection::getFlipVertices  () { return m_bFlipVertices  ; }


void CConnection::render() {
  switch (m_eType) {
    case eBezier1: break;
    case eBezier2: {
        video::SMaterial cMat;
        cMat.Lighting=false;
        m_pDrv->setMaterial(cMat);

        if (m_bSelected)
          for (u32 i=0; i<2; i++) {
            m_pDrv->draw3DLine(m_vDraw[i  ],m_vDraw[i+1],video::SColor(0xFF,0xFF,0xFF,0xFF));
            m_pDrv->draw3DLine(m_vDraw[i+3],m_vDraw[i+4],video::SColor(0xFF,0xFF,0xFF,0xFF));
          }
      }
      break;

    case eBezier3: {
        video::SMaterial cMat;
        cMat.Lighting=false;
        m_pDrv->setMaterial(cMat);

        if (m_bSelected)
          for (u32 i=0; i<3; i++) {
            m_pDrv->draw3DLine(m_vDraw[i  ],m_vDraw[i+1],video::SColor(0xFF,0xFF,0xFF,0xFF));
            m_pDrv->draw3DLine(m_vDraw[i+4],m_vDraw[i+5],video::SColor(0xFF,0xFF,0xFF,0xFF));
          }
      }
      break;
  }

  //Render the meshbuffers
  for (u32 i=0; i<_CONNECTION_NUMBER_OF_BUFFERS; i++)
    if (m_pMeshBuffer[i]!=NULL && (m_iMeshBufferToDraw==-1 || m_iMeshBufferToDraw==(s32)i)) {
      video::SMaterial cMat;
      cMat.Lighting=false;
      //cMat.Wireframe=true;

      //draw the normals
      for (u32 j=0; j<m_pMeshBuffer[i]->getVertexCount(); j++) {
        core::vector3df pos=m_pMeshBuffer[i]->getPosition(j),
                        normal=m_pMeshBuffer[i]->getNormal(j);

        m_pDrv->setMaterial(cMat);
        m_pDrv->setTransform(video::ETS_WORLD,core::CMatrix4<f32>());
        m_pDrv->draw3DLine(pos,pos+normal,video::SColor(0xFF,0xFF,0xFF,0xFF));
      }

      //draw the meshbuffers
      cMat.setTexture(0,m_pMeshBuffer[i]->getMaterial().getTexture(0));

      m_pDrv->setMaterial(cMat);
      m_pDrv->drawMeshBuffer(m_pMeshBuffer[i]);
    }

  //if we're not the currently selected connection we don't render anything else
  if (!m_bSelected) return;

  //draw the bounding box to indicate we're selected
  m_pDrv->draw3DBox(m_cBox,video::SColor(0xFF,0x60,0x60,0xFF));

  if (m_pSegment1==NULL || m_pSegment2==NULL) return;

  //If this is the selected connection we should also display the helppoints
  //and highlight the selected one
  switch (m_eType) {
    case eBezier1: break;

    case eBezier2: {
        core::vector3df size=core::vector3df(0.5f,0.5f,0.5f);
        s32 iSelect[]={ 1, 3 },iIdx[]={ 0, 2 };

        for (u32 i=0; i<2; i++) {
          core::aabbox3df box=core::aabbox3df(m_vHelpPoints[iIdx[i]]-size,m_vHelpPoints[iIdx[i]]+size);
          m_pDrv->draw3DBox(box,m_iSelectedPoint==iSelect[i]?video::SColor(0xFF,0xFF,0xFF,0):video::SColor(0xFF,0,0,0xFF));
        }
      }
      break;

    case eBezier3: {
        core::vector3df size=core::vector3df(0.5f,0.5f,0.5f);
        s32 iSelect[]={ 1, 2, 3, 0 };

        for (u32 i=0; i<4; i++) {
          core::aabbox3df box=core::aabbox3df(m_vHelpPoints[i]-size,m_vHelpPoints[i]+size);
          m_pDrv->draw3DBox(box,m_iSelectedPoint==iSelect[i]?video::SColor(0xFF,0xFF,0xFF,0):video::SColor(0xFF,0,0,0xFF));
        }
      }
      break;
  }
}

void CConnection::setSelectedPoint(s32 iPoint) {
  m_iSelectedPoint=iPoint;
}

/**
 * Save the segment
 * @param out the attribute object to fill
 */
void CConnection::save(io::IAttributes *out) {
  out->addString("Segment1",m_pSegment1->getName().c_str());
  out->addString("Segment2",m_pSegment2->getName().c_str());

  out->addInt("Type",m_eType);
  out->addInt("S1Border",m_iSegment1Border);
  out->addInt("S2Border",m_iSegment2Border);
  out->addInt("Steps",m_iSteps);

  out->addBool("FlipC",m_bFlipConnection);
  out->addBool("FlipV",m_bFlipVertices  );

  for (u32 i=0; i<4; i++) {
    core::stringc s="HelpPoint"; s+=i;
    out->addVector3d(s.c_str(),m_vHelpPoints[i]);
  }

  for (u32 i=0; i<4; i++) {
    core::stringc s="HpOffset"; s+=i;
    out->addFloat(s.c_str(),m_fHpOff[i]);
  }

  out->addFloat("WallHeight",m_fWallHeight);
  out->addFloat("WallWidth" ,m_fWallWidth );

  for (u32 i=0; i<2; i++) {
    core::stringc s="CreateWall"; s+=i;
    out->addBool(s.c_str(),m_bWalls[i]);
  }
}

/**
 * Load the segment
 * @param in the attribute object with the necessary data
 */
void CConnection::load(io::IAttributes *in) {
  m_sSegment1=in->getAttributeAsString("Segment1");
  m_sSegment2=in->getAttributeAsString("Segment2");

  m_eType          =(CConnection::eConnectionType)in->getAttributeAsInt("Type");
  m_iSegment1Border=in->getAttributeAsInt("S1Border");
  m_iSegment2Border=in->getAttributeAsInt("S2Border");
  m_iSteps         =in->getAttributeAsInt("Steps");

  m_bFlipConnection=in->getAttributeAsBool("FlipC");
  m_bFlipVertices  =in->getAttributeAsBool("FlipV");

  for (u32 i=0; i<4; i++) {
    core::stringc s="HelpPoint"; s+=i;
    m_vHelpPoints[i]=in->getAttributeAsVector3d(s.c_str());
  }

  for (u32 i=0; i<4; i++) {
    core::stringc s="HpOffset"; s+=i;
    if (in->existsAttribute(s.c_str())) m_fHpOff[i]=in->getAttributeAsFloat(s.c_str());
  }

  m_fWallHeight=in->getAttributeAsFloat("WallHeight");
  m_fWallWidth =in->getAttributeAsFloat("WallWidth" );

  for (u32 i=0; i<2; i++) {
    core::stringc s="CreateWall"; s+=i;
    m_bWalls[i]=in->getAttributeAsBool(s.c_str());
  }
}

const core::stringc &CConnection::getSegment1Name() { return m_sSegment1; }
const core::stringc &CConnection::getSegment2Name() { return m_sSegment2; }

void CConnection::attributesChanged(CSegment *p) {
  update();
}

void CConnection::objectDeleted(CSegment *p) {
  //If one of the segments was deleted we update the member
  if (m_pSegment1==p) m_pSegment1=NULL;
  if (m_pSegment2==p) m_pSegment2=NULL;

  //and recalculate everything
  update();
}

void CConnection::update() {
  //If something has changed we recalculate the meshbuffers
  recalcMeshBuffer();
}

CTextureParameters *CConnection::getTextureParameters(u32 i) {
  return i<_CONNECTION_NUMBER_OF_BUFFERS?m_pTexParams[i]:NULL;
}

scene::IMeshBuffer *CConnection::getMeshBuffer(u32 i) {
  return i<_CONNECTION_NUMBER_OF_BUFFERS?m_pMeshBuffer[i]:NULL;
}
