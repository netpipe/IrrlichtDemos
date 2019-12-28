  #include <CAdvancedParticleSystemNode.h>

  #include <irrlicht.h>
  #include <CParticleSystemSceneNode.h>
  #include <CAttributes.h>
  #include "os.h"

  #include <CParticleAnimatedMeshSceneNodeEmitter.h>
  #include <CParticleCylinderEmitter.h>
  #include <CParticleSphereEmitter.h>
  #include <CParticlePointEmitter.h>
  #include <CParticleMeshEmitter.h>
  #include <CParticleRingEmitter.h>
  #include <CParticleBoxEmitter.h>

  #include <CParticleAttractionAffector.h>
  #include <CParticleRotationAffector.h>
  #include <CParticleFadeOutAffector.h>
  #include <CParticleGravityAffector.h>
  #include <CParticleScaleAffector.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

const c8 *CAdvancedParticleSystemNode::m_sEmitterNames[eEmitterCount]={
  "EmitterNone","EmitterPoint","EmitterSphere","EmitterRing","EmitterBox",
  "EmitterCylinder","EmitterMesh","EmitterAnimtedMesh","EmitterExplosion",NULL
};

const c8 *CAdvancedParticleSystemNode::m_sAffectorNames[eAffectorCount]={
  "AffectorNone","AffectorAttraction","AffectorFadeOut",
  "AffectorGravity","AffectorRotation","AffectorScale"
};

void CAdvancedParticleSystemNode::setEmitter(IParticleEmitter* pEmitter) {
  if (m_pEmitter==pEmitter) return;

  if (m_pEmitter) {
    m_pEmitter->drop();
    m_pEmitter=NULL;
    m_eEmitter=eEmitterNone;
  }

  if (pEmitter) {
    m_pEmitter=pEmitter;

    switch (pEmitter->getType()) {
      case EPET_POINT: m_eEmitter=eEmitterPoint; break;
      case EPET_ANIMATED_MESH: m_eEmitter=eEmitterAnimatedMesh; break;
      case EPET_BOX: m_eEmitter=eEmitterBox; break;
      case EPET_CYLINDER: m_eEmitter=eEmitterCylinder; break;
      case EPET_MESH: m_eEmitter=eEmitterMesh; break;
      case EPET_RING: m_eEmitter=eEmitterRing; break;
      case EPET_SPHERE: m_eEmitter=eEmitterSphere; break;
      default:
        if (pEmitter->getType()==EPET_EXPLOSION) m_eEmitter=eEmitterExplosion;
        break;
    }
  }
}

void CAdvancedParticleSystemNode::reallocateBuffers()
{
	if (m_aParticles.size() * 4 > m_pBuffer->getVertexCount() ||
			m_aParticles.size() * 6 > m_pBuffer->getIndexCount())
	{
		u32 oldSize = m_pBuffer->getVertexCount();
		m_pBuffer->Vertices.set_used(m_aParticles.size() * 4);

		u32 i;

		// fill remaining vertices
		for (i=oldSize; i<m_pBuffer->Vertices.size(); i+=4)
		{
			m_pBuffer->Vertices[0+i].TCoords.set(0.0f, 0.0f);
			m_pBuffer->Vertices[1+i].TCoords.set(0.0f, 1.0f);
			m_pBuffer->Vertices[2+i].TCoords.set(1.0f, 1.0f);
			m_pBuffer->Vertices[3+i].TCoords.set(1.0f, 0.0f);
		}

		// fill remaining indices
		u32 oldIdxSize = m_pBuffer->getIndexCount();
		u32 oldvertices = oldSize;
		m_pBuffer->Indices.set_used(m_aParticles.size() * 6);

		for (i=oldIdxSize; i<m_pBuffer->Indices.size(); i+=6)
		{
			m_pBuffer->Indices[0+i] = (u16)0+oldvertices;
			m_pBuffer->Indices[1+i] = (u16)2+oldvertices;
			m_pBuffer->Indices[2+i] = (u16)1+oldvertices;
			m_pBuffer->Indices[3+i] = (u16)0+oldvertices;
			m_pBuffer->Indices[4+i] = (u16)3+oldvertices;
			m_pBuffer->Indices[5+i] = (u16)2+oldvertices;
			oldvertices += 4;
		}
	}
}

CAdvancedParticleSystemNode::CAdvancedParticleSystemNode(ISceneNode *parent,ISceneManager *mgr,s32 id,
                         const core::vector3df &position, const core::vector3df &rotation, const core::vector3df &scale)
                         :IParticleSystemSceneNode(parent?parent:mgr->getRootSceneNode(),mgr,id,position,rotation,scale) {
  m_eEmitter=eEmitterNone;
  m_pEmitter=NULL;
  m_pSmgr=mgr;
  m_iMaxAngleDegrees=0;
  m_iAffectorCount=0;
  m_iMinTtl=2000;
  m_iMaxTtl=4000;
  m_bParticlesAreGlobal=true;
  m_bIsActive=true;
  m_cParticleSize=dimension2df(5.0f,5.0f);
  m_iLastEmitTime=0;
  m_iLastMove=0;
  m_iThreshold=0;
  m_fMove=0.0f;
  m_vMove=core::vector3df(0.0f,0.0f,0.0f);
  m_vMoveDir=core::vector3df(0.0f,0.0f,1.0f);

  m_pBuffer=new SMeshBuffer();

  m_cScaleTarget=dimension2df(1.0f,1.0f);
  m_iAtlasSize=1;
  m_bAtlasTexture=false;
  m_bInterpolate=false;
  m_bStepped=false;
  m_vLastPos=core::vector3df(0.0f,0.0f,0.0f);
  m_vScale=getAbsoluteTransformation().getScale();
}

CAdvancedParticleSystemNode::~CAdvancedParticleSystemNode() {
	if (m_pEmitter) m_pEmitter->drop();
	if (m_pBuffer ) m_pBuffer ->drop();
}

void CAdvancedParticleSystemNode::deserializeAttributes(io::IAttributes *in, io::SAttributeReadWriteOptions *options) {
  ISceneNode::deserializeAttributes(in,options);

  m_bParticlesAreGlobal=in->getAttributeAsBool("globalParticles");
  setEmitter(NULL);

  //get the type of emitter
  m_eEmitter=(eParticleEmitter)in->getAttributeAsEnumeration("EmitterType",(const c8 *const *)m_sEmitterNames);

  m_iMaxAngleDegrees=in->getAttributeAsInt("maxAngleDegrees");

  m_bInterpolate=in->getAttributeAsBool("Interpolate");
  if (m_bInterpolate) m_iThreshold=in->getAttributeAsInt("Threshold"); else m_iThreshold=0;
  m_bStepped=in->getAttributeAsBool("stepped");
  if (m_bStepped) m_vMoveDir=in->getAttributeAsVector3d("MoveDirection");

  m_iMinTtl=in->getAttributeAsInt("minTtl");
  m_iMaxTtl=in->getAttributeAsInt("maxTtl");

  m_bAtlasTexture=in->getAttributeAsBool("AtlasTexture");
  if (m_bAtlasTexture) {
    m_iAtlasSize=in->getAttributeAsInt("AtlasCount");
    if (m_iAtlasSize!=0) {
      m_aAtlasPos.clear();
      setAtlasSize(m_iAtlasSize);
    }
  }

  vector3df vDirection=vector3df(0.0f,0.03f,0.0f);
  u32 iMaxParticle=15,iMinParticle=5;
  SColor cMaxStartColor=SColor(0xFF,0xFF,0,0),cMinStartColor(0xFF,0xFF,0xFF,0xFF);
  dimension2df cMinStartSize=dimension2df(1.0f,1.0f),cMaxStartSize=dimension2df(5.0f,5.0f);

  if (m_eEmitter!=eEmitterNone) {
    if (in->existsAttribute("Direction")) vDirection=in->getAttributeAsVector3d("Direction");

    if (in->existsAttribute("MaxParticles" )) iMaxParticle=in->getAttributeAsInt("MaxParticles");
    if (in->existsAttribute("MinParticles" )) iMinParticle=in->getAttributeAsInt("MinParticles");
    if (in->existsAttribute("MaxStartColor")) cMaxStartColor=in->getAttributeAsColor("MaxStartColor");
    if (in->existsAttribute("MinStartColor")) cMinStartColor=in->getAttributeAsColor("MinStartColor");

    //Too bad we can't directly (de)serialize dimension2df...
    f32 fMinX=in->existsAttribute("MinSizeX")?in->getAttributeAsFloat("MinSizeX"):1.0f,
        fMinY=in->existsAttribute("MinSizeY")?in->getAttributeAsFloat("MinSizeY"):1.0f,
        fMaxX=in->existsAttribute("MaxSizeX")?in->getAttributeAsFloat("MaxSizeX"):5.0f,
        fMaxY=in->existsAttribute("MaxSizeY")?in->getAttributeAsFloat("MaxSizeY"):5.0f;

    if (fMaxX!=5.0f || fMaxY!=5.0f) cMaxStartSize=dimension2df(fMaxX,fMaxY);
    if (fMinX!=1.0f || fMaxY!=1.0f) cMinStartSize=dimension2df(fMinX,fMinY);
  }

  switch (m_eEmitter) {
    case eEmitterPoint:
      createPointEmitter(vDirection,iMinParticle,iMaxParticle,
                         cMinStartColor,cMaxStartColor,m_iMinTtl,m_iMaxTtl,
                         m_iMaxAngleDegrees,cMinStartSize,cMaxStartSize);
      break;


    case eEmitterSphere: {
        vector3df vCenter=in->getAttributeAsVector3d("center");
        f32 fRadius=in->existsAttribute("radius")?in->getAttributeAsFloat("radius"):1.0f;
        createSphereEmitter(vCenter,fRadius,vDirection,iMinParticle,iMaxParticle,
                            cMinStartColor,cMaxStartColor,m_iMinTtl,m_iMaxTtl,m_iMaxAngleDegrees,
                            cMinStartSize,cMaxStartSize);
      }
      break;

    case eEmitterRing: {
        vector3df vCenter=in->getAttributeAsVector3d("center");
        f32 fRadius=in->existsAttribute("radius")?in->getAttributeAsFloat("radius"):5,
            fThickness=in->existsAttribute("thickness")?in->getAttributeAsFloat("thickness"):0.5f;

        createRingEmitter(vCenter,fRadius,fThickness,vDirection,iMinParticle,iMaxParticle,
                          cMinStartColor,cMaxStartColor,m_iMinTtl,m_iMaxTtl,m_iMaxAngleDegrees,
                          cMinStartSize,cMaxStartSize);
      }
      break;

    case eEmitterBox: {
        aabbox3df cBox=in->existsAttribute("box")?in->getAttributeAsBox3d("box"):core::aabbox3df(core::vector3df(-1,-1,-1),core::vector3df(1,1,1));
        createBoxEmitter(cBox,vDirection,iMinParticle,iMaxParticle,
                         cMinStartColor,cMaxStartColor,m_iMinTtl,m_iMaxTtl,m_iMaxAngleDegrees,
                         cMinStartSize,cMaxStartSize);
      }
      break;

    case eEmitterCylinder: {
        vector3df vCenter=in->existsAttribute("center")?in->getAttributeAsVector3d("center"):core::vector3df(0,0,0),
                  vNormal=in->existsAttribute("normal")?in->getAttributeAsVector3d("normal"):vector3df(0,1,0);

        f32 fRadius=in->existsAttribute("radius")?in->getAttributeAsFloat("radius"):5.0f,
            fLength=in->existsAttribute("length")?in->getAttributeAsFloat("length"):2.5f;

        bool bOutline=in->getAttributeAsBool("outlineOnly");

        createCylinderEmitter(vCenter,fRadius,vNormal,fLength,bOutline,vDirection,iMinParticle,iMaxParticle,
                              cMinStartColor,cMaxStartColor,m_iMinTtl,m_iMaxTtl,m_iMaxAngleDegrees,cMinStartSize,cMaxStartSize);
      }
      break;

    case eEmitterMesh:  {
        ISceneNode *pParent=getParent();
        if (pParent->getType()==ESNT_MESH) {
          bool bUseNormal=in->getAttributeAsBool("UseNormalDirection"),
               bUseAll=in->getAttributeAsBool("EveryMeshVertex");
          f32 fModifier=in->getAttributeAsFloat("NormalDirectionModifier");
          s32 iMbNumber=in->existsAttribute("MbNumber")?in->getAttributeAsInt("MbNumber"):-1;

          createMeshEmitter((IMesh *)pParent,bUseNormal,vDirection,fModifier,iMbNumber,bUseAll,iMinParticle,
                            iMaxParticle,cMinStartColor,cMaxStartColor,m_iMinTtl,m_iMaxTtl,
                            m_iMaxAngleDegrees,cMinStartSize,cMaxStartSize);
        }
      }
      break;

    case eEmitterAnimatedMesh: {
        ISceneNode *pParent=getParent();
        if (pParent->getType()==ESNT_ANIMATED_MESH) {
          bool bUseAll=in->getAttributeAsBool("EveryMeshVertex"),
               bUseNormal=in->getAttributeAsBool("UseNormalDirection");

          f32 fModifier=in->getAttributeAsFloat("NormalDirectionModifier");
          s32 iMbNumber=in->existsAttribute("MbNumber")?in->getAttributeAsInt("MbNumber"):-1;

          createAnimatedMeshSceneNodeEmitter((IAnimatedMeshSceneNode *)pParent,bUseNormal,vDirection,fModifier,
                                             iMbNumber,bUseAll,iMinParticle,iMaxParticle,cMinStartColor,
                                             cMaxStartColor,m_iMinTtl,m_iMaxTtl,m_iMaxAngleDegrees,
                                             cMinStartSize,cMaxStartSize);
        }
      }
      break;

    case eEmitterExplosion: {
        vector3df vCenter=in->getAttributeAsVector3d("center");
        f32 fRadius=in->existsAttribute("radius")?in->getAttributeAsFloat("radius"):1.0f;

        createExplosionEmitter(vCenter,fRadius,vDirection,iMinParticle,iMaxParticle,
                               cMinStartColor,cMaxStartColor,m_iMinTtl,m_iMaxTtl,
                               cMinStartSize,cMaxStartSize);
      }
      break;

    default: break;
  }

  removeAllAffectors();
  u32 iMax=in->getAttributeAsInt("AffectorCount")+1;
  char s[0xFF];

  for (u32 i=0; i<iMax; i++) {
    sprintf(s,"AffectorType_%i",i);
    eParticleAffectors eAffector=(eParticleAffectors)in->getAttributeAsEnumeration(s,(const c8 *const *)m_sAffectorNames);
    switch (eAffector) {
      case eAffectorAttraction: {
          sprintf(s,"Point_%i",i);
          vector3df vPoint=in->getAttributeAsVector3d(s);
          IParticleAttractionAffector *p=createAttractionAffector(vPoint);
          sprintf(s,"attracts_%i",i);
          p->setAttract(in->getAttributeAsBool(s));
          sprintf(s,"affectX_%i",i);
          p->setAffectX(in->getAttributeAsBool(s));
          sprintf(s,"affectY_%i",i);
          p->setAffectY(in->getAttributeAsBool(s));
          sprintf(s,"affectZ_%i",i);
          p->setAffectZ(in->getAttributeAsBool(s));
        }
        break;

      case eAffectorFadeOut: {
          IParticleFadeOutAffector *p=createFadeOutParticleAffector();
          sprintf(s,"targetColor_%i",i);
          SColor cTarget=in->getAttributeAsColor(s);
          p->setTargetColor(cTarget);
          sprintf(s,"fadeOutTime_%i",i);
          p->setFadeOutTime(in->getAttributeAsFloat(s));
        }
        break;

      case eAffectorGravity: {
          IParticleGravityAffector *p=createGravityAffector();
          sprintf(s,"gravity_%i",i);
          p->setGravity(in->getAttributeAsVector3d(s));
          sprintf(s,"timeForceLost_%i",i);
          p->setTimeForceLost(in->getAttributeAsFloat(s));
        }
        break;

      case eAffectorRotation: {
          IParticleRotationAffector *p=createRotationAffector();
          sprintf(s,"speed_%i",i);
          p->setSpeed(in->getAttributeAsVector3d(s));
          sprintf(s,"pivotPoint_%i",i);
          p->setPivotPoint(in->getAttributeAsVector3d(s));
        }
        break;

      case eAffectorScale: {
          sprintf(s,"TargetWidth_%i",i);
          f32 fTargetWidth=in->getAttributeAsFloat(s);
          sprintf(s,"TargetHeight_%i",i);
          f32 fTargetHeight=in->getAttributeAsFloat(s);
          m_cScaleTarget=dimension2df(fTargetWidth,fTargetHeight);
          createScaleParticleAffector(dimension2df(fTargetWidth,fTargetHeight));
        }
        break;

      default: break;
    }
  }
  m_bIsActive=!in->existsAttribute("active") || in->getAttributeAsBool("active");
}

void CAdvancedParticleSystemNode::serializeAttributes(io::IAttributes *out, io::SAttributeReadWriteOptions *options) const {
  ISceneNode::serializeAttributes(out,options);

  out->addBool("globalParticles",m_bParticlesAreGlobal);
  //write the emitter type
  out->addEnum("EmitterType",(s32)m_eEmitter,(const c8 *const *)m_sEmitterNames);

  out->addInt("minTtl",m_iMinTtl);
  out->addInt("maxTtl",m_iMaxTtl);

  out->addBool("AtlasTexture",m_bAtlasTexture);
  out->addInt("AtlasCount",m_iAtlasSize);

  out->addBool("Interpolate",m_bInterpolate);
  if (m_bInterpolate) out->addInt("Threshold",m_iThreshold);
  out->addBool("stepped",m_bStepped);
  if (m_bStepped) out->addVector3d("MoveDirection",m_vMoveDir);

  if (m_pEmitter) {
    out->addVector3d("Direction",m_pEmitter->getDirection());
    out->addInt("MaxParticles",m_pEmitter->getMaxParticlesPerSecond());
    out->addInt("MinParticles",m_pEmitter->getMinParticlesPerSecond());
    out->addColorf("MaxStartColor",m_pEmitter->getMaxStartColor());
    out->addColorf("MinStartColor",m_pEmitter->getMinStartColor());

    //Too bad we can't directly (de)serialize dimension2df...
    f32 fMinX=m_pEmitter->getMinStartSize().Width,
        fMinY=m_pEmitter->getMinStartSize().Height,
        fMaxX=m_pEmitter->getMaxStartSize().Width,
        fMaxY=m_pEmitter->getMaxStartSize().Height;

    out->addFloat("MaxSizeX",fMaxX);
    out->addFloat("MaxSizeY",fMaxY);
    out->addFloat("MinSizeX",fMinX);
    out->addFloat("MinSizeY",fMinY);

    out->addFloat("maxAngleDegrees",m_iMaxAngleDegrees);
    switch (m_eEmitter) {
      case eEmitterPoint: break;

      case eEmitterSphere: {
          IParticleSphereEmitter *p=(IParticleSphereEmitter *)m_pEmitter;
          out->addVector3d("center",p->getCenter());
          out->addFloat("radius",p->getRadius());
        }
        break;

      case eEmitterRing: {
          IParticleRingEmitter *p=(IParticleRingEmitter *)m_pEmitter;
          out->addVector3d("center",p->getCenter());
          out->addFloat("radius",p->getRadius());
          out->addFloat("thickness",p->getRingThickness());
        }
        break;

      case eEmitterBox: {
          IParticleBoxEmitter *p=(IParticleBoxEmitter *)m_pEmitter;
          out->addBox3d("box",p->getBox());
        }
        break;

      case eEmitterCylinder: {
          IParticleCylinderEmitter *p=(IParticleCylinderEmitter *)m_pEmitter;

          out->addVector3d("center",p->getCenter());
          out->addFloat("radius",p->getRadius());
          out->addFloat("length",p->getLength());
          out->addVector3d("normal",p->getNormal());
          out->addBool("outlineOnly",p->getOutlineOnly());
        }
        break;

      case eEmitterMesh: {
          IParticleMeshEmitter *p=(IParticleMeshEmitter *)m_pEmitter;
          out->addBool("EveryMeshVertex",p->getEveryMeshVertex());
          out->addFloat("NormalDirectionModifier",p->getNormalDirectionModifier());
          out->addBool("UseNormalDirection",p->isUsingNormalDirection());
        }
        break;

      case eEmitterAnimatedMesh: {
          IParticleAnimatedMeshSceneNodeEmitter *p=(IParticleAnimatedMeshSceneNodeEmitter *)m_pEmitter;
          out->addBool("EveryMeshVertex",p->getEveryMeshVertex());
          out->addFloat("NormalDirectionModifier",p->getNormalDirectionModifier());
          out->addBool("UseNormalDirection",p->isUsingNormalDirection());
        }
        break;

      case eEmitterExplosion: {
          CParticleExplosionEmitter *p=(CParticleExplosionEmitter *)m_pEmitter;
          out->addVector3d("center",p->getCenter());
          out->addFloat("radius",p->getRadius());
        }

      default: break;
    }
  }

  out->addInt("AffectorCount",m_iAffectorCount);
  list<IParticleAffector *>::Iterator ita;
  char s[0xFF];
  u32 i=0;
  list<IParticleAffector *>::ConstIterator it;
  for (it=m_lAffectors.begin(); it!=m_lAffectors.end(); it++) {
    IParticleAffector *pAffector=*it;
    switch (pAffector->getType()) {
      case EPAT_ATTRACT: {
          sprintf(s,"AffectorType_%i",i);
          out->addEnum(s,(s32)eAffectorAttraction,(const c8 *const *)m_sAffectorNames);

          IParticleAttractionAffector *p=(IParticleAttractionAffector *)pAffector;
          sprintf(s,"Point_%i",i);
          out->addVector3d(s,p->getPoint());
          sprintf(s,"attracts_%i",i);
          out->addBool(s,p->getAttract());
          sprintf(s,"affectX_%i",i);
          out->addBool(s,p->getAffectX());
          sprintf(s,"affectY_%i",i);
          out->addBool(s,p->getAffectY());
          sprintf(s,"affectZ_%i",i);
          out->addBool(s,p->getAffectZ());
        }
        break;

      case EPAT_FADE_OUT: {
          sprintf(s,"AffectorType_%i",i);
          out->addEnum(s,(s32)eAffectorFadeOut,(const c8 *const *)m_sAffectorNames);

          IParticleFadeOutAffector *p=(IParticleFadeOutAffector *)pAffector;
          sprintf(s,"targetColor_%i",i);
          out->addColor(s,p->getTargetColor());
          sprintf(s,"fadeOutTime_%i",i);
          out->addFloat(s,p->getFadeOutTime());
        }
        break;

      case EPAT_GRAVITY: {
          sprintf(s,"AffectorType_%i",i);
          out->addEnum(s,(s32)eAffectorGravity,(const c8 *const *)m_sAffectorNames);

          IParticleGravityAffector *p=(IParticleGravityAffector *)pAffector;
          sprintf(s,"gravity_%i",i);
          out->addVector3d(s,p->getGravity());
          sprintf(s,"timeForceLost_%i",i);
          out->addFloat(s,p->getTimeForceLost());
        }
        break;

      case EPAT_ROTATE: {
          sprintf(s,"AffectorType_%i",i);
          out->addEnum(s,(s32)eAffectorRotation,(const c8 *const *)m_sAffectorNames);

          IParticleRotationAffector *p=(IParticleRotationAffector *)pAffector;
          sprintf(s,"speed_%i",i);
          out->addVector3d(s,p->getSpeed());
          sprintf(s,"pivotPoint_%i",i);
          out->addVector3d(s,p->getPivotPoint());
        }
        break;

      case EPAT_SCALE: {
          sprintf(s,"AffectorType_%i",i);
          out->addEnum(s,(s32)eAffectorScale,(const c8 *const *)m_sAffectorNames);

          sprintf(s,"TargetWidth_%i",i);
          out->addFloat(s,m_cScaleTarget.Width);
          sprintf(s,"TargetHeight_%i",i);
          out->addFloat(s,m_cScaleTarget.Height);
        }
        break;

      default: break;
    }
    i++;
  }
  sprintf(s,"AffectorType_%i",m_iAffectorCount);
  out->addEnum(s,(s32)eAffectorNone,(const c8 *const *)m_sAffectorNames);
  out->addBool("active",m_bIsActive);
}

ISceneNode *CAdvancedParticleSystemNode::clone(ISceneNode *newParent, ISceneManager *newManager) {
  CAdvancedParticleSystemNode *pRet=new CAdvancedParticleSystemNode(newParent!=NULL?newParent:getParent(),newManager!=NULL?newManager:m_pSmgr);
  pRet->cloneMembers((ISceneNode *)this,m_pSmgr);

  CAttributes *pAttr=new CAttributes(m_pSmgr->getVideoDriver());
  this->serializeAttributes(pAttr,NULL);
  pRet->deserializeAttributes(pAttr);
  SMaterial cMat=getMaterial(0);

  E_MATERIAL_FLAG theFlags[]={EMF_WIREFRAME,EMF_POINTCLOUD,EMF_GOURAUD_SHADING,
                              EMF_LIGHTING,EMF_ZBUFFER,EMF_ZWRITE_ENABLE,
                              EMF_BACK_FACE_CULLING,EMF_FRONT_FACE_CULLING,
                              EMF_BILINEAR_FILTER,EMF_TRILINEAR_FILTER,
                              EMF_ANISOTROPIC_FILTER,EMF_FOG_ENABLE,
                              EMF_NORMALIZE_NORMALS,EMF_TEXTURE_WRAP,/*EMF_ANTI_ALIASING,
                              EMF_COLOR_MASK,EMF_COLOR_MATERIAL, */(E_MATERIAL_FLAG)-1 };

  for (u32 i=0; theFlags[i]!=(E_MATERIAL_FLAG)-1; i++)
    pRet->setMaterialFlag(theFlags[i],cMat.getFlag(theFlags[i]));

  pRet->setMaterialType(cMat.MaterialType);
  for (u32 i=0; i<MATERIAL_MAX_TEXTURES; i++)
    pRet->setMaterialTexture(i,cMat.getTexture(i));

  return pRet;
}

IParticleAnimatedMeshSceneNodeEmitter *CAdvancedParticleSystemNode::createAnimatedMeshSceneNodeEmitter(
        scene::IAnimatedMeshSceneNode *node, bool useNormalDirection,
        const core::vector3df &direction, f32 normalDirectionModifier, s32 mbNumber,bool everyMeshVertex,
        u32 minParticlesPerSecond, u32 maxParticlesPerSecond,
        const video::SColor &minStartColor, const video::SColor &maxStartColor,
        u32 lifeTimeMin, u32 lifeTimeMax, s32 maxAngleDegrees,
        const core::dimension2df &minStartSize,
        const core::dimension2df &maxStartSize) {
  CParticleAnimatedMeshSceneNodeEmitter *pRet=new CParticleAnimatedMeshSceneNodeEmitter(node,useNormalDirection,direction,normalDirectionModifier,mbNumber,everyMeshVertex,minParticlesPerSecond,maxParticlesPerSecond,minStartColor,maxStartColor,lifeTimeMin,lifeTimeMax,maxAngleDegrees,minStartSize,maxStartSize);
  setEmitter(pRet);

  m_iMinTtl=lifeTimeMin;
  m_iMaxTtl=lifeTimeMax;

  m_iMaxAngleDegrees=maxAngleDegrees;
  return pRet;
}

IParticleCylinderEmitter *CAdvancedParticleSystemNode::createCylinderEmitter(
        const core::vector3df &center, f32 radius, const core::vector3df &normal, f32 length,
        bool outlineOnly, const core::vector3df &direction,
        u32 minParticlesPerSecond, u32 maxParticlesPerSecond,
        const video::SColor &minStartColor, const video::SColor &maxStartColor,
        u32 lifeTimeMin, u32 lifeTimeMax, s32 maxAngleDegrees,
        const core::dimension2df &minStartSize,
        const core::dimension2df &maxStartSize) {
  CParticleCylinderEmitter *pRet=new CParticleCylinderEmitter(center,radius,normal,length,outlineOnly,direction,minParticlesPerSecond,maxParticlesPerSecond,minStartColor,maxStartColor,lifeTimeMin,lifeTimeMax,maxAngleDegrees,minStartSize,maxStartSize);
  setEmitter(pRet);

  m_iMinTtl=lifeTimeMin;
  m_iMaxTtl=lifeTimeMax;

  m_iMaxAngleDegrees=maxAngleDegrees;
  return pRet;
}

IParticleMeshEmitter *CAdvancedParticleSystemNode::createMeshEmitter(scene::IMesh *mesh, bool useNormalDirection,
        const core::vector3df &direction,
        f32 normalDirectionModifier, s32 mbNumber, bool everyMeshVertex,
        u32 minParticlesPerSecond, u32 maxParticlesPerSecond,
        const video::SColor &minStartColor, const video::SColor &maxStartColor,
        u32 lifeTimeMin, u32 lifeTimeMax, s32 maxAngleDegrees,
        const core::dimension2df &minStartSize,
        const core::dimension2df &maxStartSize) {
  CParticleMeshEmitter *pRet=new CParticleMeshEmitter(mesh,useNormalDirection,direction,normalDirectionModifier,mbNumber,everyMeshVertex,minParticlesPerSecond,maxParticlesPerSecond,minStartColor,maxStartColor,lifeTimeMin,lifeTimeMax,maxAngleDegrees,minStartSize,maxStartSize);

  m_iMinTtl=lifeTimeMin;
  m_iMaxTtl=lifeTimeMax;

  setEmitter(pRet);
  m_iMaxAngleDegrees=maxAngleDegrees;
  return pRet;
}

IParticlePointEmitter *CAdvancedParticleSystemNode::createPointEmitter(const core::vector3df &direction,
        u32 minParticlesPerSecond, u32 maxParticlesPerSecond,
        const video::SColor &minStartColor,
        const video::SColor &maxStartColor,
        u32 lifeTimeMin, u32 lifeTimeMax, s32 maxAngleDegrees,
        const core::dimension2df &minStartSize, const core::dimension2df &maxStartSize) {

  CParticlePointEmitter *pRet=new CParticlePointEmitter(direction,minParticlesPerSecond,maxParticlesPerSecond,minStartColor,maxStartColor,lifeTimeMin,lifeTimeMax,maxAngleDegrees,minStartSize,maxStartSize);
  setEmitter(pRet);

  m_iMinTtl=lifeTimeMin;
  m_iMaxTtl=lifeTimeMax;

  m_iMaxAngleDegrees=maxAngleDegrees;
  return pRet;
}

IParticleBoxEmitter *CAdvancedParticleSystemNode::createBoxEmitter (const core::aabbox3df &box,
        const core::vector3df &direction,
        u32 minParticlesPerSecond, u32 maxParticlesPerSecond,
        const video::SColor &minStartColor, const video::SColor &maxStartColor,
        u32 lifeTimeMin, u32 lifeTimeMax, s32 maxAngleDegrees,
        const core::dimension2df &minStartSize, const core::dimension2df &maxStartSize) {
  CParticleBoxEmitter *pRet=new CParticleBoxEmitter(box,direction,minParticlesPerSecond,maxParticlesPerSecond,minStartColor,maxStartColor,lifeTimeMin,lifeTimeMax,maxAngleDegrees,minStartSize,maxStartSize);
  setEmitter(pRet);
  m_iMaxAngleDegrees=maxAngleDegrees;

  m_iMinTtl=lifeTimeMin;
  m_iMaxTtl=lifeTimeMax;

  return pRet;
}

IParticleRingEmitter *CAdvancedParticleSystemNode::createRingEmitter(const core::vector3df &center, f32 radius, f32 ringThickness,
        const core::vector3df &direction, u32 minParticlesPerSecond,
        u32 maxParticlesPerSecond, const video::SColor &minStartColor,
        const video::SColor &maxStartColor, u32 lifeTimeMin,
        u32 lifeTimeMax, s32 maxAngleDegrees,
        const core::dimension2df &minStartSize, const core::dimension2df &maxStartSize) {
  CParticleRingEmitter *pRet=new CParticleRingEmitter(center,radius,ringThickness,direction,minParticlesPerSecond,maxParticlesPerSecond,minStartColor,maxStartColor,lifeTimeMin,lifeTimeMax,maxAngleDegrees,minStartSize,maxStartSize);
  setEmitter(pRet);

  m_iMinTtl=lifeTimeMin;
  m_iMaxTtl=lifeTimeMax;

  m_iMaxAngleDegrees=maxAngleDegrees;
  return pRet;
}

IParticleSphereEmitter *CAdvancedParticleSystemNode::createSphereEmitter (const core::vector3df &center, f32 radius,
        const core::vector3df &direction,
        u32 minParticlesPerSecond, u32 maxParticlesPerSecond,
        const video::SColor &minStartColor,
        const video::SColor &maxStartColor,
        u32 lifeTimeMin, u32 lifeTimeMax, s32 maxAngleDegrees,
        const core::dimension2df &minStartSize, const core::dimension2df &maxStartSize) {

  CParticleSphereEmitter *pRet=new CParticleSphereEmitter(center,radius,direction,minParticlesPerSecond,maxParticlesPerSecond,minStartColor,maxStartColor,lifeTimeMin,lifeTimeMax,maxAngleDegrees,minStartSize,maxStartSize);
  setEmitter(pRet);

  m_iMinTtl=lifeTimeMin;
  m_iMaxTtl=lifeTimeMax;

  m_iMaxAngleDegrees=maxAngleDegrees;
  return pRet;
}

CParticleExplosionEmitter *CAdvancedParticleSystemNode::createExplosionEmitter(const core::vector3df &center,
        f32 radius,vector3df vDirection,
        u32 minParticles, u32 maxParticles,
        const video::SColor &minStartColor,
        const video::SColor &maxStartColor,
        u32 lifeTimeMin, u32 lifeTimeMax,
        const core::dimension2df &minStartSize, const core::dimension2df &maxStartSize) {

  CParticleExplosionEmitter *pRet=new CParticleExplosionEmitter(center,radius,vDirection,minParticles,maxParticles,minStartColor,maxStartColor,lifeTimeMin,lifeTimeMax,minStartSize,maxStartSize);
  setEmitter(pRet);

  m_iMinTtl=lifeTimeMin;
  m_iMaxTtl=lifeTimeMax;

  return pRet;
}

IParticleAttractionAffector *CAdvancedParticleSystemNode::createAttractionAffector(
        const core::vector3df &point, f32 speed, bool attract, bool affectX, bool affectY, bool affectZ) {
  CParticleAttractionAffector *pRet=new CParticleAttractionAffector(point,speed,attract,affectX,affectY,affectZ);
  addAffector(pRet);
  return pRet;
}

IParticleFadeOutAffector *CAdvancedParticleSystemNode::createFadeOutParticleAffector(
        const video::SColor &targetColor, u32 timeNeededToFadeOut) {
  CParticleFadeOutAffector *pRet=new CParticleFadeOutAffector(targetColor,timeNeededToFadeOut);
  addAffector(pRet);
  return pRet;
}

IParticleGravityAffector *CAdvancedParticleSystemNode::createGravityAffector(const core::vector3df &gravity, u32 timeForceLost) {
  CParticleGravityAffector *pRet=new CParticleGravityAffector(gravity,timeForceLost);
  addAffector(pRet);
  return pRet;
}

IParticleRotationAffector *CAdvancedParticleSystemNode::createRotationAffector(const core::vector3df &speed, const core::vector3df &pivotPoint) {
  CParticleRotationAffector *pRet=new CParticleRotationAffector(speed,pivotPoint);
  addAffector(pRet);
  return pRet;
}

IParticleAffector *CAdvancedParticleSystemNode::createScaleParticleAffector(const core::dimension2df &scaleTo) {
  CParticleScaleAffector *pRet=new CParticleScaleAffector(scaleTo);
  addAffector(pRet);
  return pRet;
}

void CAdvancedParticleSystemNode::removeAllAffectors() {
  while (m_lAffectors.getSize()>0) {
    list<IParticleAffector *>::Iterator it=m_lAffectors.begin();
    IParticleAffector *p=*it;
    m_lAffectors.erase(it);
    delete p;
  }
  m_iAffectorCount=0;
}

void CAdvancedParticleSystemNode::OnRegisterSceneNode() {
  os::Timer::tick();

  updateAbsolutePosition();

	if (IsVisible) {
		SceneManager->registerNodeForRendering(this);
		ISceneNode::OnRegisterSceneNode();
	}
}

void CAdvancedParticleSystemNode::addAffector(IParticleAffector* pAffector) {
  if (pAffector==NULL) return;
	m_lAffectors.push_back(pAffector);
  m_iAffectorCount++;
}

void CAdvancedParticleSystemNode::render() {
	video::IVideoDriver* driver = m_pSmgr->getVideoDriver();
	ICameraSceneNode* camera = m_pSmgr->getActiveCamera();

	if (!camera || !driver)
		return;

  updateAbsolutePosition();

#ifdef _IRREDIT_PLUGIN
	// calculate vectors for letting particles look to camera
	const core::matrix4 &m = camera->getViewFrustum()->Matrices [ video::ETS_VIEW ];

	const core::vector3df view ( -m[2], -m[6] , -m[10] );

#else

	const core::matrix4 &m = camera->getViewFrustum()->getTransform( video::ETS_VIEW );

	const core::vector3df view ( -m[2], -m[6] , -m[10] );

#endif

	// reallocate arrays, if they are too small
	reallocateBuffers();

	// create particle vertex data
	s32 idx = 0;
	for (u32 i=0; i<m_aParticles.size(); ++i)
	{
		const SParticle& particle = m_aParticles[i];

		//printf("%i\n",iIdx);
		#if 0
			core::vector3df horizontal = camera->getUpVector().crossProduct(view);
			horizontal.normalize();
			horizontal *= 0.5f * particle.size.Width;

			core::vector3df vertical = horizontal.crossProduct(view);
			vertical.normalize();
			vertical *= 0.5f * particle.size.Height;

		#else
			f32 f;

			f = 0.5f * particle.size.Width;
			const core::vector3df horizontal ( m[0] * f, m[4] * f, m[8] * f );

			f = -0.5f * particle.size.Height;
			const core::vector3df vertical ( m[1] * f, m[5] * f, m[9] * f );
		#endif

    this->updateAbsolutePosition();

		m_pBuffer->Vertices[0+idx].Pos = particle.pos + horizontal + vertical;
		m_pBuffer->Vertices[0+idx].Color = particle.color;
		m_pBuffer->Vertices[0+idx].Normal = view;

		m_pBuffer->Vertices[1+idx].Pos = particle.pos + horizontal - vertical;
		m_pBuffer->Vertices[1+idx].Color = particle.color;
		m_pBuffer->Vertices[1+idx].Normal = view;

		m_pBuffer->Vertices[2+idx].Pos = particle.pos - horizontal - vertical;
		m_pBuffer->Vertices[2+idx].Color = particle.color;
		m_pBuffer->Vertices[2+idx].Normal = view;

		m_pBuffer->Vertices[3+idx].Pos = particle.pos - horizontal + vertical;
		m_pBuffer->Vertices[3+idx].Color = particle.color;
		m_pBuffer->Vertices[3+idx].Normal = view;

		if (m_bAtlasTexture) {
      u32 iIdx=(u32)(((f32)m_iAtlasCount)*(f32)(m_iCurrentTime-particle.startTime)/(f32)(particle.endTime-particle.startTime));

      if (iIdx>=m_iAtlasCount) iIdx=m_iAtlasCount-1;

      m_pBuffer->getTCoords(0+idx).X=m_aAtlasPos[iIdx].X+m_fAtlasSize;
      m_pBuffer->getTCoords(0+idx).Y=m_aAtlasPos[iIdx].Y+m_fAtlasSize;

      m_pBuffer->getTCoords(1+idx).X=m_aAtlasPos[iIdx].X+m_fAtlasSize;
      m_pBuffer->getTCoords(1+idx).Y=m_aAtlasPos[iIdx].Y;

      m_pBuffer->getTCoords(2+idx).X=m_aAtlasPos[iIdx].X;
      m_pBuffer->getTCoords(2+idx).Y=m_aAtlasPos[iIdx].Y;

      m_pBuffer->getTCoords(3+idx).X=m_aAtlasPos[iIdx].X;
      m_pBuffer->getTCoords(3+idx).Y=m_aAtlasPos[iIdx].Y+m_fAtlasSize;
		}

		idx +=4;
	}

	// render all
	core::matrix4 mat;
	if (!m_bParticlesAreGlobal)
		mat.setTranslation(AbsoluteTransformation.getTranslation());
	driver->setTransform(video::ETS_WORLD, mat);

	driver->setMaterial(m_pBuffer->Material);

	driver->drawVertexPrimitiveList(m_pBuffer->getVertices(), m_aParticles.size()*4,
		m_pBuffer->getIndices(), m_aParticles.size()*2, video::EVT_STANDARD, EPT_TRIANGLES,m_pBuffer->getIndexType());

	// for debug purposes only:
	if ( DebugDataVisible & scene::EDS_BBOX )
	{
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
		video::SMaterial deb_m;
		deb_m.Lighting = false;
		driver->setMaterial(deb_m);
		driver->draw3DBox(m_pBuffer->BoundingBox, video::SColor(0,255,255,255));
	}
}

const core::aabbox3d<f32> &CAdvancedParticleSystemNode::getBoundingBox() const {
  return m_pBuffer->getBoundingBox();
}

void CAdvancedParticleSystemNode::setParticlesAreGlobal(bool bGlobal) {
  m_bParticlesAreGlobal=bGlobal;
}

void CAdvancedParticleSystemNode::setParticleSize( const dimension2d<f32> &cSize) {
	if (m_pEmitter) {
		m_pEmitter->setMinStartSize(cSize);
		m_pEmitter->setMaxStartSize(cSize);
	}
	m_cParticleSize=cSize;
}

void CAdvancedParticleSystemNode::doParticleSystem(u32 iTime) {
  m_iCurrentTime=iTime;
  updateAbsolutePosition();
	if (m_iLastEmitTime==0)
	{
		m_iLastEmitTime = iTime;
		m_vLastPos=getAbsolutePosition();
		m_iLastMove=m_iLastEmitTime;
		m_vScale=getAbsoluteTransformation().getScale();
		return;
	}

	u32 now = iTime;
	u32 timediff = iTime - m_iLastEmitTime;
	m_iLastEmitTime = iTime;

	core::vector3df vPos=getAbsolutePosition(),vOff=core::vector3df(0.0f,0.0f,0.0f);

	if (m_bInterpolate) {
    if (vPos!=m_vLastPos) {
      m_vVel=(vPos-m_vLastPos)/(f32)(iTime-m_iLastMove);
      m_vLastPos=vPos;
      m_iLastMove=iTime;
    }
    else {
      u32 iDiff=iTime-m_iLastMove;
      if (m_iThreshold==0 || iDiff<m_iThreshold) {
        f32 fDiff=(f32)iDiff;
        vOff=m_vVel*fDiff;
      }
    }
	}

	if (m_bStepped) {
	  if (vPos!=m_vLastPos) {
      m_vMove=vPos-m_vLastPos;
      m_fMove=m_vMove.getLength();
      m_vMove.normalize();
	    m_vLastPos=vPos;
	  }
	}
	// run emitter

	if (m_pEmitter && IsVisible)
	{
		SParticle* array = 0;
		s32 newParticles=0;

		if (m_bIsActive) newParticles=m_pEmitter->emitt(now, timediff, array);

		if (newParticles && array)
		{
			s32 j=m_aParticles.size();
			if (newParticles > 16250-j)
				newParticles=16250-j;
			m_aParticles.set_used(j+newParticles);
			for (s32 i=j; i<j+newParticles; ++i)
			{
				m_aParticles[i]=array[i-j];

				if (m_bInterpolate) {
          m_aParticles[i].pos+=vOff;
				}
				if (m_bStepped) {
				  f32 f=((f32)(os::Randomizer().rand()%1000))/1000.0f;
				  m_aParticles[i].pos-=f*m_fMove*m_vMoveDir;
				}
				AbsoluteTransformation.rotateVect(m_aParticles[i].startVector);
				if (m_bParticlesAreGlobal)
					AbsoluteTransformation.transformVect(m_aParticles[i].pos);
			}
		}
	}

	// run affectors
	core::list<IParticleAffector*>::Iterator ait = m_lAffectors.begin();
	for (; ait != m_lAffectors.end(); ++ait)
		(*ait)->affect(now, m_aParticles.pointer(), m_aParticles.size());

	if (m_bParticlesAreGlobal)
		m_pBuffer->BoundingBox.reset(AbsoluteTransformation.getTranslation());
	else
		m_pBuffer->BoundingBox.reset(core::vector3df(0,0,0));

	// animate all particles
	f32 scale = (f32)timediff;

	for (u32 i=0; i<m_aParticles.size();)
	{
		// erase is pretty expensive!
		if (now > m_aParticles[i].endTime)
			m_aParticles.erase(i);
		else
		{
			m_aParticles[i].pos += (m_aParticles[i].vector * scale);
			m_pBuffer->BoundingBox.addInternalPoint(m_aParticles[i].pos);
			++i;
		}
	}

	const f32 m = (m_cParticleSize.Width > m_cParticleSize.Height ? m_cParticleSize.Width : m_cParticleSize.Height) * 0.5f;
	m_pBuffer->BoundingBox.MaxEdge.X += m;
	m_pBuffer->BoundingBox.MaxEdge.Y += m;
	m_pBuffer->BoundingBox.MaxEdge.Z += m;

	m_pBuffer->BoundingBox.MinEdge.X -= m;
	m_pBuffer->BoundingBox.MinEdge.Y -= m;
	m_pBuffer->BoundingBox.MinEdge.Z -= m;

	if (m_bParticlesAreGlobal)
	{
		core::matrix4 absinv( AbsoluteTransformation, core::matrix4::EM4CONST_INVERSE );
		absinv.transformBoxEx(m_pBuffer->BoundingBox);
	}
}

const char *CAdvancedParticleSystemNode::getTypeName() const {
  return ADVANCED_PARTICLE_NODE_NAME;
}

ESCENE_NODE_TYPE CAdvancedParticleSystemNode::getType() const {
  return (ESCENE_NODE_TYPE)ADVANCED_PARTICLE_NODE_ID;
}

void CAdvancedParticleSystemNode::OnAnimate(u32 timeMs) {
  doParticleSystem(timeMs);//os::Timer::getTime());
}

void CAdvancedParticleSystemNode::setAtlasSize(u32 iSize) {
  m_iAtlasSize=iSize;
  m_bAtlasTexture=iSize!=0;
  if (m_bAtlasTexture) {
    m_aAtlasPos.clear();

    for (u32 i=0; i<m_iAtlasSize; i++) {
      for (u32 j=0; j<m_iAtlasSize; j++) {
        m_aAtlasPos.push_back(vector2df(((f32)j)/m_iAtlasSize,((f32)i)/m_iAtlasSize));
      }
    }
    m_iAtlasCount=m_aAtlasPos.size();
    m_fAtlasSize=1.0f/((f32)m_iAtlasSize);
  }
}

// *******************************************************
// Here starts the implementation of the scenenode factory
// *******************************************************

CAdvancedParticleSystemNodeFactory::CAdvancedParticleSystemNodeFactory(ISceneManager *pSmgr) {
  m_pSmgr=pSmgr;
}

ISceneNode *CAdvancedParticleSystemNodeFactory::addSceneNode(const c8 *typeName, ISceneNode *parent) {
  if (!strcmp(ADVANCED_PARTICLE_NODE_NAME,typeName))
    return new CAdvancedParticleSystemNode(parent,m_pSmgr);
  else
    return NULL;
}

ISceneNode *CAdvancedParticleSystemNodeFactory::addSceneNode(ESCENE_NODE_TYPE type, ISceneNode *parent) {
  if (type==ADVANCED_PARTICLE_NODE_ID)
    return new CAdvancedParticleSystemNode(parent,m_pSmgr);
  else
    return NULL;
}

u32 CAdvancedParticleSystemNodeFactory::getCreatableSceneNodeTypeCount() const {
  return 1;
}

ESCENE_NODE_TYPE CAdvancedParticleSystemNodeFactory::getCreateableSceneNodeType(u32 idx) const {
  return idx==0?(ESCENE_NODE_TYPE)ADVANCED_PARTICLE_NODE_ID:ESNT_UNKNOWN;
}

const c8 *CAdvancedParticleSystemNodeFactory::getCreateableSceneNodeTypeName(ESCENE_NODE_TYPE type) const {
  return type==ADVANCED_PARTICLE_NODE_ID?ADVANCED_PARTICLE_NODE_NAME:NULL;
}

const c8 *CAdvancedParticleSystemNodeFactory::getCreateableSceneNodeTypeName(u32 idx) const {
  return idx==0?ADVANCED_PARTICLE_NODE_NAME:NULL;
}
