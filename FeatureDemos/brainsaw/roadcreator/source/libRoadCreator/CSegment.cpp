  #include <CSegment.h>
  #include <INotification.h>
  #include <CTextureParameters.h>

using namespace irr;

/**
 * The construtor. This one is used by the GUI of the editor
 * @param sName name of the segment
 * @param vPosition position of the segment
 * @param pDrv the Irrlicht video driver
 */
CSegment::CSegment(core::stringc sName, core::vector3df vPosition, IrrlichtDevice *pDevice, CTextureParameters *pInitParam) : IRoadPart(pDevice->getVideoDriver()) {
  //Default values: width: 50, length: 50, direction: (0,0,1), normal: (0,1,0), base offset: 10, level base: true
  m_fWidth=50.0f;
  m_fLength=50.0f;
  m_vPosition=vPosition;
  m_vDirection=core::vector3df(0.0f,0.0f,1.0f);
  m_vNormal=core::vector3df(0.0f,1.0f,0.0f);
  m_sName=sName;
  m_fBaseOffset=10.0f;
  m_bLevelBase=true;
  m_bNormalBase=false;
  m_bNormalWall=false;

  m_pDevice=pDevice;
  m_pDrv=m_pDevice->getVideoDriver();
  m_pFs=m_pDevice->getFileSystem();

  //initialize the buffer and texture parameter members
  for (u32 i=0; i<_SEGMENT_NUMBER_OF_BUFFERS; i++) {
    m_pBuffer[i]=NULL;
    m_pTexParams[i]=new CTextureParameters();
    pInitParam[i].copyTo(m_pTexParams[i]);
    m_pTexInit[i]=&pInitParam[i];
  }

  for (u32 i=0; i<4; i++) { m_bWalls[i]=false; m_bCorner[i]=false; }
  m_fWallHeight=10.0f;
  m_fWallWidth=2.5f;
}

/**
 * The construtor. This one is used by the road loading routine
 * @param pDrv the Irrlicht video driver
 */
CSegment::CSegment(IrrlichtDevice *pDevice) : IRoadPart(pDevice->getVideoDriver()) {
  //Default values: width: 10, length: 25, direction: (0,0,1), normal: (0,1,0), base offset: 10, level base: true
  m_fWidth=10.0f;
  m_fLength=25.0f;
  m_vPosition=core::vector3df(0.0f,0.0f,0.0f);
  m_vDirection=core::vector3df(0.0f,0.0f,1.0f);
  m_vNormal=core::vector3df(0.0f,1.0f,0.0f);
  m_sName="name";
  m_pDevice=pDevice;
  m_pDrv=m_pDevice->getVideoDriver();
  m_pFs=m_pDevice->getFileSystem();
  m_fBaseOffset=10.0f;
  m_bLevelBase=true;

  //initialize the buffer and texture parameter members
  for (u32 i=0; i<_SEGMENT_NUMBER_OF_BUFFERS; i++) {
    m_pBuffer[i]=NULL;
    m_pTexParams[i]=new CTextureParameters();
    m_pTexInit[i]=NULL;
  }

  for (u32 i=0; i<4; i++) { m_bWalls[i]=true; m_bCorner[i]=false; }
}

/**
 * The destructor
 */
CSegment::~CSegment() {
  core::list<INotification *>::Iterator it;
  for (it=m_lNotify.begin(); it!=m_lNotify.end(); it++) {
    (*it)->objectDeleted(this);
  }

  for (u32 i=0; i<_SEGMENT_NUMBER_OF_BUFFERS; i++) {
    if (m_pBuffer[i]!=NULL) m_pBuffer[i]->drop();
    delete m_pTexParams[i];
  }
}

void CSegment::attributeChanged() {
  recalcMeshBuffer();

  core::list<INotification *>::Iterator it;
  for (it=m_lNotify.begin(); it!=m_lNotify.end(); it++) {
    (*it)->attributesChanged(this);
  }
}

void CSegment::setWidth (f32 f) { m_fWidth =f; attributeChanged(); }
void CSegment::setLength(f32 f) { m_fLength=f; attributeChanged(); }

f32 CSegment::getWidth () { return m_fWidth ; }
f32 CSegment::getLength() { return m_fLength; }

void CSegment::setBaseOffset(f32 f) { m_fBaseOffset=f; attributeChanged(); }

f32 CSegment::getBaseOffset() { return m_fBaseOffset; }

void CSegment::setLevelBase(bool b) { m_bLevelBase=b; attributeChanged(); }

bool CSegment::getLevelBase() { return m_bLevelBase; }

void CSegment::setNormalBase(bool b) { m_bNormalBase=b; attributeChanged(); }
bool CSegment::getNormalBase() { return m_bNormalBase; }

void CSegment::setNormalWall(bool b) { m_bNormalWall=b; attributeChanged(); }
bool CSegment::getNormalWall() { return m_bNormalWall; }

void CSegment::setPosition (core::vector3df v) { m_vPosition =v;                           attributeChanged(); }
void CSegment::setDirection(core::vector3df v) { m_vDirection=v; m_vDirection.normalize(); attributeChanged(); }
void CSegment::setNormal   (core::vector3df v) { m_vNormal   =v; m_vNormal   .normalize(); attributeChanged(); }

const core::vector3df &CSegment::getPosition () { return m_vPosition ; }
const core::vector3df &CSegment::getDirection() { return m_vDirection; }
const core::vector3df &CSegment::getNormal   () { return m_vNormal   ; }

void CSegment::setName(const core::stringc &sName) { m_sName=sName; }

const core::stringc &CSegment::getName() { return m_sName; }

/**
 * This method takes an array of vectors and converts them to vertices
 * @param vec the array of vectors
 * @param pTex the texture parameters to be used
 * @param vert the output vertex array
 */
void CSegment::fillVertexArray(core::vector3df vec[], CTextureParameters *pTex, video::S3DVertex *vert, bool bTop, core::vector3df vNorm) {
  //initialize some help variables to help finding the right points for the top shape
  core::vector3df calcLength=(m_fLength/2)*m_vDirection,
                  calcWidth =(m_fWidth /2)*(m_vNormal.crossProduct(m_vDirection));

  core::vector3df v1=vec[0]-vec[1],v2=vec[3]-vec[2];
  f32 fDiff=v1.getLength()-v2.getLength();
  if (v1.getLength()>v2.getLength()) fDiff/=v1.getLength(); else fDiff/=-v2.getLength();

  //Some locals for texture creation
  f32 f=calcWidth.getLength()!=0 && !pTex->getStretch()?calcLength.getLength()/calcWidth.getLength():1.0f,
      fStartX=pTex->getOffsetX(),fStartY=pTex->getOffsetY(),
      fScaleX=pTex->getScaleX (),fScaleY=pTex->getScaleY ();

  f32 fTex[4][2];
  
  printf("fScaleX=%.2f\n",fScaleX);
  //Calculate the texture positions of the shape points depending on the rotation
  fTex[0][0]=fScaleX*(  fDiff)+fStartX; fTex[0][1]=fScaleY*  fStartY;
  fTex[1][0]=fScaleX*(f-fDiff)+fStartX; fTex[1][1]=fScaleY*  fStartY;
  fTex[2][0]=fScaleX*(f-fDiff)+fStartX; fTex[2][1]=fScaleY*f+fStartY;
  fTex[3][0]=fScaleX*(  fDiff)+fStartX; fTex[3][1]=fScaleY*f+fStartY;

  if (v1.getLength()<v2.getLength()) {
    fTex[0][0]=fStartX*fScaleX;
    fTex[3][0]=fStartX*fScaleX;
  }
  else {
    fTex[1][0]=(f+fStartX)*fScaleX;
    fTex[2][0]=(f+fStartX)*fScaleX;
  }

  if (bTop) {
    f32 fDummy;
    fDummy=fTex[2][0];
    fTex[2][0]=fTex[3][0];
    fTex[3][0]=fDummy;

    fDummy=fTex[2][1];
    fTex[2][1]=fTex[3][1];
    fTex[3][1]=fDummy;
  }

  if (pTex->getRotate()!=0) {
    f32 fDummy[4][2]={ { 0.0f,0.0f }, { 0.0f,0.0f }, { 0.0f,0.0f}, { 0.0f,0.0f } };

    switch (pTex->getRotate()) {
      case 1:
        fDummy[0][0]=fTex[3][0]; fDummy[0][1]=fTex[3][1];
        fDummy[1][0]=fTex[0][0]; fDummy[1][1]=fTex[0][1];
        fDummy[2][0]=fTex[1][0]; fDummy[2][1]=fTex[1][1];
        fDummy[3][0]=fTex[2][0]; fDummy[3][1]=fTex[2][1];
        break;

      case 2:
        fDummy[0][0]=fTex[2][0]; fDummy[0][1]=fTex[2][1];
        fDummy[1][0]=fTex[3][0]; fDummy[1][1]=fTex[3][1];
        fDummy[2][0]=fTex[0][0]; fDummy[2][1]=fTex[0][1];
        fDummy[3][0]=fTex[1][0]; fDummy[3][1]=fTex[1][1];
        break;

      case 3:
        fDummy[0][0]=fTex[1][0]; fDummy[0][1]=fTex[1][1];
        fDummy[1][0]=fTex[2][0]; fDummy[1][1]=fTex[2][1];
        fDummy[2][0]=fTex[3][0]; fDummy[2][1]=fTex[3][1];
        fDummy[3][0]=fTex[0][0]; fDummy[3][1]=fTex[0][1];
        break;
    }

    for (u32 x=0; x<4; x++) for (u32 y=0; y<2; y++) fTex[x][y]=fDummy[x][y];
  }

  if (vNorm.getLength()==0.0f) vNorm=(vec[0]-vec[1]).crossProduct(vec[0]-vec[2]);
  vNorm.normalize();
  //Create vertices inside the output array
  for (u32 i=0; i<4; i++) {
    vert[i]=video::S3DVertex(vec[i],vNorm,video::SColor(0xFF,0xE0,0xFF,0xE0),core::vector2df(fTex[i][0],fTex[i][1]));
  }
}

/**
 * This method recalculates the segment's meshbuffers
 * @see CSegment::CSegment
 * @see CSegment::attributeChanged
 * @see CSegment::load
 * @see CSegment::update
 */
void CSegment::recalcMeshBuffer() {
  //delete the meshbuffers
  for (u32 i=0; i<_SEGMENT_NUMBER_OF_BUFFERS; i++) {
    //if (m_pBuffer[i]!=NULL) m_pBuffer[i]->drop();
    m_pBuffer[i]=NULL;
    vTemp[i].clear();
  }

  core::vector3df pos=m_vPosition,
                  calcLength=(m_fLength/2)*m_vDirection,
                  calcWidth =(m_fWidth /2)*(m_vNormal.crossProduct(m_vDirection));

  //Calculate the four top points
  m_vPoints[0]=pos+calcLength-calcWidth;
  m_vPoints[1]=pos+calcLength+calcWidth;
  m_vPoints[2]=pos-calcLength+calcWidth;
  m_vPoints[3]=pos-calcLength-calcWidth;

  m_vWallNorm=m_bNormalWall? (m_vPoints[2]-m_vPoints[1]).crossProduct(m_vPoints[0]-m_vPoints[1]).normalize():core::vector3df(0.0f, 1.0f,0.0f);
  m_vBaseNorm=m_bNormalBase?-(m_vPoints[2]-m_vPoints[1]).crossProduct(m_vPoints[0]-m_vPoints[1]).normalize():core::vector3df(0.0f,-1.0f,0.0f);

  //Same procedure for the bottom polygon...
  core::vector3df vBasePoints[4];

  for (u32 i=0; i<4; i++) vBasePoints[i]=m_vPoints[3-i]+m_vBaseNorm*m_fBaseOffset;

  if (m_bLevelBase && !m_bNormalBase) {
    //if the "level base" attribute is set we calculate the average Y of all four
    //bottom points and use that value for all of them
    f32 fNewBase=(vBasePoints[0].Y+vBasePoints[1].Y+vBasePoints[2].Y+vBasePoints[3].Y)/4;
    for (u32 i=0; i<4; i++) vBasePoints[i].Y=fNewBase;
  }

  core::vector3df vCalcPoints[4]={ m_vPoints[0], m_vPoints[1], m_vPoints[2], m_vPoints[3] };

  u32 iDir[]={ 3, 0, 1, 2 };

  for (u32 i=0; i<4; i++) {
    if (m_bWalls[i]) {
      core::vector3df vDir=m_vPoints[i]-m_vPoints[iDir[i]];
      vDir.normalize();

      u32 iNext=i<3?i+1:0;
      vCalcPoints[i    ]=vCalcPoints[i    ]-m_fWallWidth*vDir;
      vCalcPoints[iNext]=vCalcPoints[iNext]-m_fWallWidth*vDir;
    }
  }

  //convert the calculated points to vertices and add them to the meshbuffer
  video::S3DVertex vVerts[4];

  fillVertexArray(vCalcPoints,m_pTexParams[0],vVerts,false);

  u16 iIdx[]={ 0,1,3, 3,1,2 };

  m_pBuffer[0]=new scene::SMeshBuffer();
  m_pBuffer[0]->append(vVerts,4,iIdx,6);
  m_pBuffer[0]->recalculateBoundingBox();

  u16 iBaseIdx[]={ 2,0,1, 0,2,3 };
  fillVertexArray(vBasePoints,m_pTexParams[1],vVerts,false);

  m_pBuffer[1]=new scene::SMeshBuffer();
  m_pBuffer[1]->append(vVerts,4,iBaseIdx,6);
  m_pBuffer[1]->recalculateBoundingBox();

  //Now for the automatic part: the four sides can be calculated
  //within a loop, so we do this.
  for (u32 i=0; i<4; i++) {
    u16 iSideIdx[]={ 0,1,2, 2,3,0 },
        iWallIdO[]={ 0,1,3, 2,3,1 },  //Outer walls
        iWallIdI[]={ 3,0,1, 2,3,1 },  //Inner walls
        iWallTop[]={ 0,2,1, 1,2,3 },
        iWallEn1[]={ 1,0,2, 3,2,0 },
        iWallEn2[]={ 0,1,2, 2,3,0 };

    core::vector3df vSide[4],vWallIn[4],vWallOut[4],vWallTop[4],vWallEnd[4],vCornerHelp;

    //Depending on the counter value we have to use different values so we switch a little
    switch (i) {
      case 0: {
          vSide[0]=m_vPoints[1];
          vSide[1]=m_vPoints[0];

          vSide[2]=vBasePoints[3];
          vSide[3]=vBasePoints[2];

          vWallIn[2]=vCalcPoints[1];
          vWallIn[3]=vCalcPoints[0];

          vWallIn[0]=vCalcPoints[0]+m_fWallHeight*m_vWallNorm;
          vWallIn[1]=vCalcPoints[1]+m_fWallHeight*m_vWallNorm;

          vWallOut[2]=m_vPoints[0];
          vWallOut[3]=m_vPoints[1];

          vWallOut[0]=m_vPoints[1]+m_fWallHeight*m_vWallNorm;
          vWallOut[1]=m_vPoints[0]+m_fWallHeight*m_vWallNorm;

          vCornerHelp=m_vPoints[2];
        }
        break;

      case 1: {
          vSide[0]=m_vPoints[2];
          vSide[1]=m_vPoints[1];
          vSide[2]=vBasePoints[2];
          vSide[3]=vBasePoints[1];

          vWallIn[2]=vCalcPoints[2];
          vWallIn[3]=vCalcPoints[1];

          vWallIn[0]=vCalcPoints[1]+m_fWallHeight*m_vWallNorm;
          vWallIn[1]=vCalcPoints[2]+m_fWallHeight*m_vWallNorm;

          vWallOut[2]=m_vPoints[1];
          vWallOut[3]=m_vPoints[2];

          vWallOut[0]=m_vPoints[2]+m_fWallHeight*m_vWallNorm;
          vWallOut[1]=m_vPoints[1]+m_fWallHeight*m_vWallNorm;

          vCornerHelp=m_vPoints[3];
        }
        break;

      case 2: {
          vSide[0]=m_vPoints[3];
          vSide[1]=m_vPoints[2];
          vSide[2]=vBasePoints[1];
          vSide[3]=vBasePoints[0];

          vWallIn[2]=vCalcPoints[3];
          vWallIn[3]=vCalcPoints[2];

          vWallIn[0]=vCalcPoints[2]+m_fWallHeight*m_vWallNorm;
          vWallIn[1]=vCalcPoints[3]+m_fWallHeight*m_vWallNorm;

          vWallOut[2]=m_vPoints[2];
          vWallOut[3]=m_vPoints[3];

          vWallOut[0]=m_vPoints[3]+m_fWallHeight*m_vWallNorm;
          vWallOut[1]=m_vPoints[2]+m_fWallHeight*m_vWallNorm;

          vCornerHelp=m_vPoints[0];
        }
        break;

      case 3: {
          vSide[0]=m_vPoints[0];
          vSide[1]=m_vPoints[3];
          vSide[2]=vBasePoints[0];
          vSide[3]=vBasePoints[3];

          vWallIn[2]=vCalcPoints[0];
          vWallIn[3]=vCalcPoints[3];

          vWallIn[0]=vCalcPoints[3]+m_fWallHeight*m_vWallNorm;
          vWallIn[1]=vCalcPoints[0]+m_fWallHeight*m_vWallNorm;

          vWallOut[2]=m_vPoints[3];
          vWallOut[3]=m_vPoints[0];

          vWallOut[0]=m_vPoints[0]+m_fWallHeight*m_vWallNorm;
          vWallOut[1]=m_vPoints[3]+m_fWallHeight*m_vWallNorm;

          vCornerHelp=m_vPoints[1];
        }
        break;
    }

    vWallTop[0]=vWallIn [0];
    vWallTop[1]=vWallIn [1];
    vWallTop[2]=vWallOut[1];
    vWallTop[3]=vWallOut[0];

    //Fill the array, create the meshbuffer, add the created array and do something about the texture...
    fillVertexArray(vSide,m_pTexParams[i+2],vVerts,false);
    m_pBuffer[i+2]=new scene::SMeshBuffer();
    m_pBuffer[i+2]->append(vVerts,4,iSideIdx,6);
    m_pBuffer[i+2]->recalculateBoundingBox();

    if (m_bWalls[i]) {
      fillVertexArray(vWallOut,m_pTexParams[i+6],vVerts,false);
      m_pBuffer[i+6]=new scene::SMeshBuffer();
      m_pBuffer[i+6]->append(vVerts,4,iWallIdO,6);
      m_pBuffer[i+6]->recalculateBoundingBox();

      fillVertexArray(vWallIn,m_pTexParams[i+10],vVerts,false);
      m_pBuffer[i+10]=new scene::SMeshBuffer();
      m_pBuffer[i+10]->append(vVerts,4,iWallIdI,6);
      m_pBuffer[i+10]->recalculateBoundingBox();

      core::vector3df vTopNorm=(vWallTop[0]-vWallTop[2]).crossProduct(vWallTop[0]-vWallTop[1]);
      fillVertexArray(vWallTop,m_pTexParams[i+14],vVerts,true,vTopNorm);
      m_pBuffer[i+14]=new scene::SMeshBuffer();
      m_pBuffer[i+14]->append(vVerts,4,iWallTop,6);
      m_pBuffer[i+14]->recalculateBoundingBox();

      vWallEnd[0]=vWallIn [2];
      vWallEnd[1]=vWallOut[3];
      vWallEnd[2]=vWallOut[0];
      vWallEnd[3]=vWallIn [1];

      fillVertexArray(vWallEnd,m_pTexParams[i+14],vVerts,false);
      m_pBuffer[i+14]->append(vVerts,4,iWallEn1,6);

      vWallEnd[0]=vWallIn [3];
      vWallEnd[1]=vWallOut[2];
      vWallEnd[2]=vWallOut[1];
      vWallEnd[3]=vWallIn [0];

      fillVertexArray(vWallEnd,m_pTexParams[i+14],vVerts,false);
      m_pBuffer[i+14]->append(vVerts,4,iWallEn2,6);
    }

    u32 iHelpIdx[][3]={ { 0,1,3 }, { 1,2,0 }, { 2,3,1 }, { 3,0,2 } },
        iWall[][2]={ { 0,3 }, { 0,1 }, { 1,2 }, { 2,3 } };

    if (m_bCorner[i] && !m_bWalls[iWall[i][0]] && !m_bWalls[iWall[i][1]]) {
      core::vector3df v[]={ m_vPoints[iHelpIdx[i][0]], m_vPoints[iHelpIdx[i][1]], m_vPoints[iHelpIdx[i][2]] };

      v[1]-=v[0];
      v[2]-=v[0];

      v[1].normalize();
      v[2].normalize();

      v[1]*=m_fWallWidth;
      v[2]*=m_fWallWidth;

      core::vector3df vCornerBot[]={ v[0]+v[1], v[0], v[0]+v[2], v[0]+v[1]+v[2] },
                      vCornerTop[]={
                        vCornerBot[0]+m_fWallHeight*m_vWallNorm,
                        vCornerBot[1]+m_fWallHeight*m_vWallNorm,
                        vCornerBot[2]+m_fWallHeight*m_vWallNorm,
                        vCornerBot[3]+m_fWallHeight*m_vWallNorm
                      };

      u16 idx[]={ 1,0,2, 2,0,3 };

      core::vector3df vTopNorm=(vCornerTop[0]-vCornerTop[2]).crossProduct(vCornerTop[0]-vCornerTop[1]);
      fillVertexArray(vCornerTop,m_pTexParams[i+14],vVerts,false,vTopNorm);
		if (m_pBuffer[i+14]==NULL) m_pBuffer[i+14]=new scene::SMeshBuffer();
      m_pBuffer[i+14]->append(vVerts,4,idx,6);

      for (u32 x=0; x<4; x++) {
        u16 idxSide[]={ 0,1,2, 0,2,3 };
        core::vector3df vTmp[]={ vCornerTop[x], vCornerTop[x<3?x+1:0], vCornerBot[x<3?x+1:0], vCornerBot[x] };
        fillVertexArray(vTmp,m_pTexParams[i+6],vVerts,false);
        if (m_pBuffer[i+6]==NULL) m_pBuffer[i+6]=new scene::SMeshBuffer();
        m_pBuffer[i+6]->append(vVerts,4,idxSide,6);
      }
    }
  }

  for (u32 i=0; i<_SEGMENT_NUMBER_OF_BUFFERS; i++)
    if (m_pBuffer[i]!=NULL && m_pBuffer[i]->getVertexCount()>0) {
      if (core::stringc("")==m_pTexParams[i]->getTexture() && m_pTexInit[i]!=NULL)
        m_pTexInit[i]->copyTo(m_pTexParams[i]);

      video::ITexture *pTex=m_pDrv->getTexture(m_pTexParams[i]->getTexture().c_str());
      m_pBuffer[i]->getMaterial().setTexture(0,pTex);
    }
}

/**
 * Render method. Just for the editor
 */
void CSegment::render() {
  //Render all meshbuffers
  for (s32 i=0; i<_SEGMENT_NUMBER_OF_BUFFERS; i++) {
    if (m_pBuffer[i]!=NULL && m_pBuffer[i]->getVertexCount()>0 && (m_iMeshBufferToDraw==-1 || m_iMeshBufferToDraw==(s32)i)) {
      video::SMaterial cMat;

      cMat.Lighting=false;
      cMat.setTexture(0,m_pBuffer[i]->getMaterial().getTexture(0));

      m_pDrv->setMaterial(cMat);
      m_pDrv->setTransform(video::ETS_WORLD,core::CMatrix4<f32>());
      m_pDrv->drawMeshBuffer(m_pBuffer[i]);

      //draw the normals
      for (u32 j=0; j<m_pBuffer[i]->getVertexCount(); j++) {
        core::vector3df pos=m_pBuffer[i]->getPosition(j),
                        normal=m_pBuffer[i]->getNormal(j);

        m_pDrv->setMaterial(cMat);
        m_pDrv->setTransform(video::ETS_WORLD,core::CMatrix4<f32>());
        m_pDrv->draw3DLine(pos,pos+normal,video::SColor(0xFF,0xFF,0xFF,0xFF));
      }
    }
  }
}

const core::vector3df &CSegment::getPoint(u32 i) {
  static const core::vector3df vErr=core::vector3df(0.0f,0.0f,0.0f);
  if (i>=4) return vErr; else return m_vPoints[i];
}

void CSegment::save(io::IAttributes *out) {
  out->addString("Name"   ,m_sName   .c_str());

  out->addFloat("Width" ,m_fWidth     );
  out->addFloat("Length",m_fLength    );
  out->addFloat("Offset",m_fBaseOffset);

  out->addBool("LevelBase" ,m_bLevelBase );
  out->addBool("NormalBase",m_bNormalBase);
  out->addBool("NormalWall",m_bNormalWall);

  out->addVector3d("Position" ,m_vPosition );
  out->addVector3d("Direction",m_vDirection);
  out->addVector3d("Normal"   ,m_vNormal   );

  out->addFloat("WallHeight",m_fWallHeight);
  out->addFloat("WallWidth" ,m_fWallWidth );
  for (u32 i=0; i<4; i++) {
    core::stringc s="CreateWall"; s+=i;
    out->addBool(s.c_str(),m_bWalls[i]);
  }

  for (u32 i=0; i<4; i++) {
    core::stringc s="CreateCorner"; s+=i;
    out->addBool(s.c_str(),m_bCorner[i]);
  }
}

void CSegment::load(io::IAttributes *in) {
  m_sName   =in->getAttributeAsString("Name"   );

  m_fWidth     =in->getAttributeAsFloat("Width" );
  m_fLength    =in->getAttributeAsFloat("Length");
  m_fBaseOffset=in->getAttributeAsFloat("Offset");

  m_bLevelBase =in->getAttributeAsBool("LevelBase" );
  m_bNormalBase=in->getAttributeAsBool("NormalBase");

  if (in->existsAttribute("NormalWall"))
    m_bNormalWall=in->getAttributeAsBool("NormalWall");
  else
    m_bNormalWall=m_bNormalBase;

  if (!in->existsAttribute("NormalBase")) m_bNormalBase=false;

  m_vPosition =in->getAttributeAsVector3d("Position" );
  m_vDirection=in->getAttributeAsVector3d("Direction");
  m_vNormal   =in->getAttributeAsVector3d("Normal"   );

  m_fWallHeight=in->getAttributeAsFloat("WallHeight");
  m_fWallWidth =in->getAttributeAsFloat("WallWidth" );
  for (u32 i=0; i<4; i++) {
    core::stringc s="CreateWall"; s+=i;
    m_bWalls[i]=in->getAttributeAsBool(s.c_str());

    s="CreateCorner"; s+=i;
    m_bCorner[i]=in->getAttributeAsBool(s.c_str());
  }
}

void CSegment::addNotify(INotification *p) {
  core::list<INotification *>::Iterator it;
  for (it=m_lNotify.begin(); it!=m_lNotify.end(); it++) {
    if (*it==p) return;
  }
  m_lNotify.push_back(p);
}

void CSegment::delNotify(INotification *p) {
  core::list<INotification *>::Iterator it;
  for (it=m_lNotify.begin(); it!=m_lNotify.end(); it++) {
    if (*it==p) {
      m_lNotify.erase(it);
      return;
    }
  }
}

void CSegment::update() {
  attributeChanged();
}

CTextureParameters *CSegment::getTextureParameters(u32 i) {
  return i<_SEGMENT_NUMBER_OF_BUFFERS?m_pTexParams[i]:NULL;
}

scene::IMeshBuffer *CSegment::getMeshBuffer(u32 i) {
  return i<_SEGMENT_NUMBER_OF_BUFFERS?m_pBuffer[i]:NULL;
}

CSegment *CSegment::clone() {
  io::IAttributes *p=m_pFs->createEmptyAttributes();
  this->save(p);

  CSegment *pRet=new CSegment(m_pDevice);
  pRet->load(p);
  p->drop();
  return pRet;
}
