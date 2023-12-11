  #include "CRandomForestNode.h"
  #ifdef _IRREDIT_PLUGIN
    #define _USE_MATH_DEFINES
    #include <cmath>
    #include <windows.h>
  #endif

void CRandomForest::CRandom::init(unsigned long s) {
  m_iMatrixIdx=m_iN+1;
  m_aMatrix[0]=s& 0xffffffffUL;
  for (m_iMatrixIdx=1; m_iMatrixIdx<m_iN; m_iMatrixIdx++) {
    m_aMatrix[m_iMatrixIdx]=(1812433253UL*(m_aMatrix[m_iMatrixIdx-1]^(m_aMatrix[m_iMatrixIdx-1]>>30))+m_iMatrixIdx);
    m_aMatrix[m_iMatrixIdx] &= 0xffffffffUL;
  }
}

CRandomForest::CRandom::CRandom(irr::core::array<irr::u32> aInit) {
  m_iN=624;
  m_iM=397;

  m_aMatrix=new irr::u32[m_iN];

  m_iMatrixIdx=m_iN+1;
  irr::s32 i,j,k;
  init(19650218UL);
  i=1; j=0;
  k=(m_iN>(irr::s32)aInit.size()?m_iN:aInit.size());
  for (; k; k--) {
    m_aMatrix[i]=(m_aMatrix[i]^((m_aMatrix[i-1]^(m_aMatrix[i-1]>>30))*1664525UL))+aInit[j]+j;
    m_aMatrix[i]&=0xffffffffUL;
    i++; j++;
    if (i>=m_iN) { m_aMatrix[0]=m_aMatrix[m_iN-1]; i=1; }
    if (j>=(irr::s32)aInit.size()) j=0;
  }
  for (k=m_iN-1; k; k--) {
    m_aMatrix[i]=(m_aMatrix[i]^((m_aMatrix[i-1]^(m_aMatrix[i-1]>>30))*1566083941UL))-i;
    m_aMatrix[i]&=0xffffffffUL; /* for WORDSIZE > 32 machines */
    i++;
    if (i>=m_iN) { m_aMatrix[0]=m_aMatrix[m_iN-1]; i=1; }
  }

  m_aMatrix[0]=0x80000000UL;
}

CRandomForest::CRandom::~CRandom() {
  delete m_aMatrix;
}

irr::u32 CRandomForest::CRandom::next() {
  static irr::u32 s_iUpperMask=0x80000000UL,s_iLowerMask=0x7fffffffUL;

  irr::u32 y;
  static irr::u32 mag01[2]={0x0UL,0x9908b0dfUL};

  if (m_iMatrixIdx>=m_iN) {
    irr::s32 kk;

    if (m_iMatrixIdx==m_iN+1) init(5489UL);

    for (kk=0;kk<m_iN-m_iM;kk++) {
      y = (m_aMatrix[kk]&s_iUpperMask)|(m_aMatrix[kk+1]&s_iLowerMask);
      m_aMatrix[kk]=m_aMatrix[kk+m_iM]^(y>>1)^mag01[y&0x1UL];
    }
    for (;kk<m_iN-1;kk++) {
      y = (m_aMatrix[kk]&s_iUpperMask)|(m_aMatrix[kk+1]&s_iLowerMask);
      m_aMatrix[kk]=m_aMatrix[kk+(m_iM-m_iN)]^(y>>1)^mag01[y & 0x1UL];
    }
    y=(m_aMatrix[m_iN-1]&s_iUpperMask)|(m_aMatrix[0]&s_iLowerMask);
    m_aMatrix[m_iN-1]=m_aMatrix[m_iM-1]^(y>>1)^mag01[y & 0x1UL];

    m_iMatrixIdx=0;
  }

  y=m_aMatrix[m_iMatrixIdx++];

  y ^= (y >> 11);
  y ^= (y << 7) & 0x9d2c5680UL;
  y ^= (y << 15) & 0xefc60000UL;
  y ^= (y >> 18);

  return y;
}

/**
 * Calculate the 2d angle between two position points
 * @param cPos1 the first position
 * @param cPos2 the second position
 * @return the angle between the 2d vectors
 */
irr::f32 CRandomForest::Angle2D(irr::core::position2di cPos1, irr::core::position2di cPos2) {
  irr::f32 dtheta,theta1,theta2;

  theta1=atan2((double)cPos1.Y,(double)cPos1.X);
  theta2=atan2((double)cPos2.Y,(double)cPos2.X);
  dtheta=theta2 - theta1;
  #ifdef _IRREDIT_PLUGIN
    while (dtheta> M_PI) dtheta -= 2*M_PI;
    while (dtheta<-M_PI) dtheta += 2*M_PI;
  #else
    while (dtheta> M_PI) dtheta -= 2*M_PI;
    while (dtheta<-M_PI) dtheta += 2*M_PI;
  #endif
  return dtheta;
}

/**
 * Determine whether or not a point is within a polygon
 * @param polygon an array of 2d positions that form a polygon
 * @param p the position
 * @return "true" if the point is inside the polygon, "false" otherwise
 */
bool CRandomForest::InsidePolygon(irr::core::array<irr::core::position2di> polygon, irr::core::position2di p) {
  irr::u32 i;
  irr::f32 angle=0;
  irr::core::position2di p1,p2;

  for (i=0; i<polygon.size(); i++) {
    p1.X = polygon[i].X - p.X;
    p1.Y = polygon[i].Y - p.Y;
    p2.X = polygon[(i+1)%polygon.size()].X - p.X;
    p2.Y = polygon[(i+1)%polygon.size()].Y - p.Y;
    angle += Angle2D(p1,p2);
  }

  if (angle<M_PI) return false; else return true;
}

/**
 * Determine whether or not a rectanble is within a polygon, i.e. all corners of the rectangle
 * are inside.
 * @param polygon array of 2d positions that make up the polygon
 * @param rect the rectangle
 * @return "true" if the rectangle is inside the polygon, "false" otherwise
 * @see CRandomForest::InsidePolygon
 */
bool CRandomForest::RectInsidePolygon(irr::core::array<irr::core::position2di> polygon, irr::core::rect<irr::s32> rect) {
  irr::core::position2di p;

  p.X=rect.UpperLeftCorner .X; p.Y=rect.UpperLeftCorner .Y; if (!InsidePolygon(polygon,p)) return false;
  p.X=rect.UpperLeftCorner .X; p.Y=rect.LowerRightCorner.Y; if (!InsidePolygon(polygon,p)) return false;
  p.X=rect.LowerRightCorner.X; p.Y=rect.UpperLeftCorner .Y; if (!InsidePolygon(polygon,p)) return false;
  p.X=rect.LowerRightCorner.X; p.Y=rect.LowerRightCorner.Y; if (!InsidePolygon(polygon,p)) return false;

  return true;
}

CRandomForest::CRandomForest(irr::scene::ISceneNode *pParent,irr::scene::ISceneManager *pSmgr,irr::s32 iId,
            const irr::core::vector3df &cPos, const irr::core::vector3df &cRot, const irr::core::vector3df &cScale)
            : irr::scene::ISceneNode(pParent!=NULL?pParent:pSmgr->getRootSceneNode(),pSmgr,
                                                 iId,cPos,cRot,cScale), m_cBox() {
  m_pDriver=pSmgr->getVideoDriver();
  m_pSmgr=pSmgr;
  m_iStepsizeX=16;
  m_iStepsizeY=16;
  m_iThreshold=125;

  m_aSeed.push_back(0x123);
  m_aSeed.push_back(0x234);
  m_aSeed.push_back(0x345);
  m_aSeed.push_back(0x456);

  m_cBox.reset(getPosition());

  m_pTerrain=NULL;
  m_pTemplate=NULL;
  m_pParentNode=NULL;
  m_sParentName="";

  m_cMat.setFlag(irr::video::EMF_LIGHTING,false);
  m_cMat.Wireframe=false;
  m_cMat.EmissiveColor=irr::video::SColor(0xFF,0xFF,0,0);

  m_fResize=0.0f;
  m_bSerializeTrees=false;
  #ifdef _IRREDIT_PLUGIN
    c8 sResources[1024];
  	GetModuleFileName(NULL,sResources,1024);
	  if (strstr(sResources,"IrrEdit.exe")) *strstr(sResources,"IrrEdit.exe")='\0';
  	strcat(sResources,"resources\\");
	  c8 sFileName[1024],sTexture[1024];
	  sprintf(sFileName,"%spole.obj",sResources);
    sprintf(sTexture,"%spole.jpg",sResources);
    m_pMesh=m_pSmgr->getMesh(sFileName);
    m_cMat.setTexture(0,m_pDriver->getTexture(sTexture));
    this->setAutomaticCulling(scene::EAC_OFF);
  #endif
}

CRandomForest::~CRandomForest() {
  for (irr::u32 i=0; i<m_aRects.size(); i++) delete m_aRects[i];
}

void CRandomForest::setSeed(irr::core::array<irr::u32> aSeed) {
  m_aSeed=aSeed;
}

void CRandomForest::setStepSize(irr::u32 x, irr::u32 y) {
  m_iStepsizeX=x;
  m_iStepsizeY=y;
}

void CRandomForest::setThreshold(irr::u32 i) {
  m_iThreshold=i;
}

void CRandomForest::addToPolygon(irr::core::position2di newVert) {
  m_aPoly.push_back(newVert);
  irr::f32 f=m_pTerrain!=NULL?m_pTerrain->getHeight(newVert.X,newVert.Y):0.0f;
  m_cBox.addInternalPoint(irr::core::vector3df(newVert.X,f,newVert.Y)+getAbsolutePosition());
}

void CRandomForest::clearForest() {
  for (irr::u32 i=0; i<m_aRects.size(); i++) delete m_aRects[i];
  m_aRects.clear();

  while (m_aGeneratedTrees.size()>0) {
    irr::scene::ISceneNode *p=m_aGeneratedTrees[0];
    m_aGeneratedTrees.erase(0);
    p->remove();
  }
}

void CRandomForest::createForest() {
  CRandom rand(m_aSeed);

  irr::core::position2di cMin,cMax;

  cMax.X=0; cMax.Y=0;

  for (irr::u32 i=0; i<m_aPoly.size(); i++) {
    if (m_aPoly[i].X>cMax.X) { cMax.X=m_aPoly[i].X; cMin.X=cMax.X; }
    if (m_aPoly[i].Y>cMax.Y) { cMax.Y=m_aPoly[i].Y; cMin.Y=cMax.Y; }
  }

  for (irr::u32 i=0; i<m_aPoly.size(); i++) {
    if (m_aPoly[i].X<cMin.X) cMin.X=m_aPoly[i].X;
    if (m_aPoly[i].Y<cMin.Y) cMin.Y=m_aPoly[i].Y;
  }

  if (m_aTrees.size()!=0)
    for (irr::s32 y=cMin.Y; y<cMax.Y; y+=m_iStepsizeY)
      for (irr::s32 x=cMin.X; x<cMax.X; x+=m_iStepsizeX) {
        irr::core::rect<irr::s32> cRect(x,y,x+m_iStepsizeX,y+m_iStepsizeY);
        if (RectInsidePolygon(m_aPoly,cRect)) {
          irr::f32 fFact=m_fResize*((irr::f32)(rand.next()%2000)-1000)/1000.0f,fSize=1.0f+fFact;
          CCheckRect *r=new CCheckRect(cRect,fSize);
          irr::u32 iRand=rand.next()%1000;
          r->m_bTree=iRand>m_iThreshold;
          r->m_iIndex=rand.next()%m_aTrees.size();

          irr::f32 ox=rand.next()%m_iStepsizeX,oy=rand.next()%m_iStepsizeY;
          r->m_cOffset=irr::core::position2df(ox,oy);

          m_aRects.push_back(r);
        }
      }
}

void CRandomForest::OnRegisterSceneNode() {
  if (IsVisible) m_pSmgr->registerNodeForRendering(this);
  ISceneNode::OnRegisterSceneNode();

}

const irr::core::aabbox3d<irr::f32> &CRandomForest::getBoundingBox() const {
  return m_cBox;
}

void CRandomForest::deserializeAttributes(irr::io::IAttributes *in, irr::io::SAttributeReadWriteOptions *options) {
  irr::scene::ISceneNode::deserializeAttributes(in,options);

  irr::core::stringc s=in->getAttributeAsString("tree_template");
  setTemplateNodeName(s.c_str());

  s=in->getAttributeAsString("terrain");
  setTerrainNodeName(s.c_str());

  s=in->getAttributeAsString("parent_node");
  irr::scene::ISceneNode *pNode=m_pSmgr->getSceneNodeFromName(s.c_str());
  if (pNode!=NULL && s!="") {
    m_sParentName=s;
    m_pParentNode=pNode;
  }
  else {
    m_sParentName="";
    m_pParentNode=NULL;
  }

  m_iStepsizeX=in->getAttributeAsInt("stepsize_x");
  m_iStepsizeY=in->getAttributeAsInt("stepsize_y");
  m_iThreshold=in->getAttributeAsInt("threshold");
  m_fResize=in->getAttributeAsFloat("resize");

  irr::u32 count=in->getAttributeAsInt("seed_count");

  m_aSeed.clear();
  for (irr::u32 i=0; i<count; i++) {
    irr::c8 s[0xFF];
    sprintf(s,"seed_%i",i);
    if (in->existsAttribute(s))
      m_aSeed.push_back(in->getAttributeAsInt(s));
    else
      m_aSeed.push_back(0);
  }

  count=in->getAttributeAsInt("poly_count");

  m_aPoly.clear();
  for (irr::u32 i=0; i<count; i++) {
    irr::c8 s[0xFF];
    sprintf(s,"poly_%i",i);
    if (in->existsAttribute(s)) {
      irr::core::vector3df v=in->getAttributeAsVector3d(s);
      this->addToPolygon(core::position2di(v.X,v.Z));
    }
    else m_aPoly.push_back(irr::core::position2di(0.0f,0.0f));

    if (i==0) {
      m_cBox.reset(core::vector3df(m_aPoly[m_aPoly.size()-1].X,0.0f,m_aPoly[m_aPoly.size()-1].Y)+getAbsolutePosition());
    }
  }

  if (in->getAttributeAsBool("generate_trees") || m_aGeneratedTrees.size()==0) plantTrees();
  m_bSerializeTrees=in->getAttributeAsBool("serialize_trees");
  #ifdef _IRREDIT_PLUGIN
    this->setAutomaticCulling(scene::EAC_OFF);
  #endif
}

void CRandomForest::serializeAttributes(irr::io::IAttributes *out, irr::io::SAttributeReadWriteOptions *options) const {
  irr::scene::ISceneNode::serializeAttributes(out,options);

  out->addString("tree_template",m_sTemplate.c_str());
  out->addInt("trees",m_aTrees.size());
  out->addString("terrain",m_sTerrain.c_str());
  out->addString("parent_node",m_sParentName.c_str());
  out->addInt("generated_trees",m_aGeneratedTrees.size());

  out->addInt("stepsize_x",m_iStepsizeX);
  out->addInt("stepsize_y",m_iStepsizeY);
  out->addInt("threshold",m_iThreshold);
  out->addFloat("resize",m_fResize);

  out->addInt("seed_count",m_aSeed.size());

  for (irr::u32 i=0; i<m_aSeed.size(); i++) {
    irr::c8 s[0xFF];
    sprintf(s,"seed_%i",i);
    out->addInt(s,m_aSeed[i]);
  }

  out->addInt("poly_count",m_aPoly.size());

  for (irr::u32 i=0; i<m_aPoly.size(); i++) {
    irr::c8 s[0xFF];
    sprintf(s,"poly_%i",i);
    core::position2di p=m_aPoly[i];
    out->addVector3d(s,irr::core::vector3df(p.X,0.0f,p.Y));
  }

  out->addBool("generate_trees",false);
  out->addBool("serialize_trees",m_bSerializeTrees);
}

void CRandomForest::render() {
  #ifdef _IRREDIT_PLUGIN
    core::list<scene::ISceneNode *> lChild=getChildren();
    core::list<scene::ISceneNode *>::Iterator it;

    for (it=lChild.begin(); it!=lChild.end(); it++) (*it)->render();
    if (m_pMesh && m_pMesh->getMesh(0) && m_pMesh->getMesh(0)->getMeshBuffer(0)) {
      for (irr::u32 i=0; i<m_aPoly.size(); i++) {

        irr::core::position2di p=m_aPoly[i];

        irr::f32 y=0.0f;
        irr::core::vector3df vPos=getPosition();

        if (m_pTerrain) {
          y=m_pTerrain->getHeight(p.X+vPos.X,p.Y+vPos.Z);
        }

        irr::core::matrix4 tmpMatrix;
        tmpMatrix.setTranslation(AbsoluteTransformation.getTranslation()+irr::core::vector3df(p.X,y+vPos.Y,p.Y));
        tmpMatrix.setScale(irr::core::vector3df(25.0f,30.0f,25.0f));
        m_pDriver->setMaterial(m_cMat);
        m_pDriver->setTransform(video::ETS_WORLD,tmpMatrix);
        m_pDriver->drawMeshBuffer(m_pMesh->getMesh(0)->getMeshBuffer(0));
      }
    }

    if (m_aPoly.size()<3) return;

    m_pDriver->setMaterial(m_cMat);
    irr::core::matrix4 tmpMatrix;
    m_pDriver->setTransform(video::ETS_WORLD,tmpMatrix);

    for (irr::u32 i=0; i<m_aPoly.size(); i++) {
      irr::u32 iIdx1=i<m_aPoly.size()-1?i+1:0;

      irr::core::position2di p1=m_aPoly[i],p2=m_aPoly[iIdx1];

      irr::f32 y1=0.0f,y2=0.0f;
      irr::core::vector3df vPos=getPosition();

      if (m_pTerrain) {
        y1=m_pTerrain->getHeight(p1.X+vPos.X,p1.Y+vPos.Z);
        y2=m_pTerrain->getHeight(p2.X+vPos.X,p2.Y+vPos.Z);
      }

      y1+=25.0f;
      y2+=25.0f;

      irr::core::vector3df v1=irr::core::vector3df(p1.X,y1,p1.Y)+getPosition(),
                           v2=irr::core::vector3df(p2.X,y2,p2.Y)+getPosition();

      m_pDriver->draw3DLine(v1,v2,video::SColor(0xFF,0xFF,0,0));
    }
  #endif
}

void CRandomForest::setTemplateNodeName(const irr::c8 *sTemplate) {
  irr::scene::ISceneNode *p=m_pSmgr->getSceneNodeFromName(sTemplate);

  if (p!=NULL) {
    m_pTemplate=p;
    m_aTrees.clear();
    m_sTemplate=sTemplate;
    irr::core::list<irr::scene::ISceneNode *> children=m_pTemplate->getChildren();
    irr::core::list<irr::scene::ISceneNode *>::Iterator it;

    for (it=children.begin(); it!=children.end(); it++)
       m_aTrees.push_back(*it);
  }
  else printf("template==NULL!\n");
  printf("%i trees found!\n",m_aTrees.size());
}

void CRandomForest::setTerrainNodeName(const irr::c8 *sTerrain) {
  irr::scene::ISceneNode *p=m_pSmgr->getSceneNodeFromName(sTerrain);
  if (p!=NULL && p->getType()==irr::scene::ESNT_TERRAIN) {
    m_sTerrain=sTerrain;
    m_pTerrain=(irr::scene::ITerrainSceneNode *)p;
    printf("got terrain!\n");
  }
  else printf("invalid terrain node name \"%s\"\n",sTerrain);
}

void CRandomForest::plantTrees() {
  clearForest();
  createForest();

  for (irr::u32 i=0; i<m_aRects.size(); i++) {
    if (m_aRects[i]->m_bTree) {
      irr::core::position2di cPos=m_aRects[i]->m_cRect.UpperLeftCorner;
      irr::core::vector3df v=getPosition();

      irr::f32 x=cPos.X+m_aRects[i]->m_cOffset.X+v.X,
               y=0.0f,
               z=cPos.Y+m_aRects[i]->m_cOffset.Y+v.Z;

      if (m_pTerrain)
        y=m_pTerrain->getHeight(x,z);
      else
        y=getAbsolutePosition().Y;

      irr::core::vector3df vPos=irr::core::vector3df(x,y,z);

      irr::u32 idx=m_aRects[i]->m_iIndex;
      irr::scene::ISceneNode *clone=NULL;

      #ifdef _IRREDIT_PLUGIN
        clone=m_aTrees[idx]->clone();
        clone->setParent(m_pParentNode!=NULL?m_pParentNode:m_pSmgr->getRootSceneNode());
      #else
        clone=m_aTrees[idx]->clone(m_pParentNode!=NULL?m_pParentNode:m_pSmgr->getRootSceneNode());
      #endif

      irr::scene::IMesh *pMesh=NULL;

      if (clone->getType()==irr::scene::ESNT_MESH) {
        irr::scene::IMeshSceneNode *pAnimClone=(irr::scene::IMeshSceneNode *)clone;
        pMesh=pAnimClone->getMesh();
      }

      if (clone->getType()==irr::scene::ESNT_ANIMATED_MESH) {
        irr::scene::IAnimatedMeshSceneNode *pAnimClone=(irr::scene::IAnimatedMeshSceneNode *)clone;
        pMesh=pAnimClone->getMesh();
      }

      if (pMesh!=NULL) pMesh->setHardwareMappingHint(irr::scene::EHM_STATIC);

      clone->setIsDebugObject(!m_bSerializeTrees);
      clone->setScale(m_aRects[i]->m_fResize*clone->getScale());
      clone->setPosition(vPos);
      m_aGeneratedTrees.push_back(clone);
    }
  }
}




CRandomForestFactory::CRandomForestFactory(irr::scene::ISceneManager *pManager) {
  m_pSmgr=pManager;
}

irr::scene::ISceneNode *CRandomForestFactory::addSceneNode(irr::scene::ESCENE_NODE_TYPE type, irr::scene::ISceneNode *parent) {
  if (type==(irr::scene::ESCENE_NODE_TYPE)RANDOM_FOREST_ID)
    return new CRandomForest(parent,m_pSmgr);
  else
    return NULL;
}

irr::scene::ISceneNode *CRandomForestFactory::addSceneNode (const irr::c8 *typeName, irr::scene::ISceneNode *parent) {
  if (!strcmp(typeName,"RandomForestNode"))
    return new CRandomForest(parent,m_pSmgr);
  else
    return NULL;
}

irr::u32 CRandomForestFactory::getCreatableSceneNodeTypeCount() const {
  return 1;
}

irr::scene::ESCENE_NODE_TYPE CRandomForestFactory::getCreateableSceneNodeType (irr::u32 idx) const {
  if (idx==0)
    return (irr::scene::ESCENE_NODE_TYPE)RANDOM_FOREST_ID;
  else
    return irr::scene::ESNT_UNKNOWN;
}

const irr::c8 *CRandomForestFactory::getCreateableSceneNodeTypeName(irr::scene::ESCENE_NODE_TYPE type) const {
	static c8 sName[]="RandomForestNode";
	return type==(scene::ESCENE_NODE_TYPE)RANDOM_FOREST_ID?sName:NULL;
}

const irr::c8 *CRandomForestFactory::getCreateableSceneNodeTypeName(irr::u32 idx) const {
	return idx==0?getCreateableSceneNodeTypeName(RANDOM_FOREST_ID):NULL;
}

CRandomForestFactory::~CRandomForestFactory() {
}
