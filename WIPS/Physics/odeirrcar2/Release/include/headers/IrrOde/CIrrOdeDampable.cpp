  #include <CIrrOdeDampable.h>
	#include <CIrrOdeManager.h>

namespace irr {
namespace ode {

CIrrOdeDampable::CIrrOdeDampable(ISceneNode *parent,ISceneManager *mgr,s32 id,
                                 const vector3df &position, const vector3df &rotation, const vector3df &scale) :
                                 CIrrOdeSceneNode(parent, mgr, id, position, rotation, scale) {

  #ifdef _TRACE_CONSTRUCTOR_DESTRUCTOR
    printf("CIrrOdeDampable constructor\n");
  #endif
  //set the damping and auto disable members to the default values
  m_fDampingLinear              =_DEFAULT_DAMPING_LINEAR                ;
  m_fDampingAngular             =_DEFAULT_DAMPING_ANGULAR               ;
  m_fDampingLinearThreshold     =_DEFAULT_DAMPING_LINEAR_THRESHOLD      ;
  m_fDampingAngularThreshold    =_DEFAULT_DAMPING_ANGULAR_THRESHOLD     ;
  m_fAutoDisableLinearThreshold =_DEFAULT_AUTO_DISABLE_LINEAR_THRESHOLD ;
  m_fAutoDisableAngularThreshold=_DEFAULT_AUTO_DISABLE_ANGULAR_THRESHOLD;
  m_fAutoDisableTime            =_DEFAULT_AUTO_DISABLE_TIME             ;
  m_iAutoDisableSteps           =_DEFAULT_AUTO_DISABLE_STEPS            ;
  m_iAutoDisableFlag            =_DEFAULT_AUTO_DISABLE_FLAG             ;
  m_fMaxAngularSpeed            =_DEFAULT_MAX_ANGULAR_SPEED             ;

	m_sParamName=L"";
	m_bUseDefined=false;
}

CIrrOdeDampable::~CIrrOdeDampable() {
  #ifdef _TRACE_CONSTRUCTOR_DESTRUCTOR
    printf("CIrrOdeDampable destructor\n");
  #endif
}

void CIrrOdeDampable::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const {
  CIrrOdeSceneNode::serializeAttributes(out,options);

	out->addString("paramName",stringc(m_sParamName).c_str());
	out->addBool("useDefined",m_bUseDefined);

	CIrrOdeDampable *p=NULL;
	if (m_bUseDefined) p=CIrrOdeManager::getSharedInstance()->getDampableWithParamName(m_sParamName.c_str());

	if (p==NULL || p==this) {
		out->addInt("AutoDisableSteps",m_iAutoDisableSteps);
		out->addInt("AutoDisableFlag" ,m_iAutoDisableFlag );

		out->addFloat("LinearDamping"              ,m_fDampingLinear              );
		out->addFloat("AngularDamping"             ,m_fDampingAngular             );
		out->addFloat("LinearDampingThreshold"     ,m_fDampingLinearThreshold     );
		out->addFloat("AngularDampingThreshold"    ,m_fDampingAngularThreshold    );
		out->addFloat("AutoDisableLinearThreshold" ,m_fAutoDisableLinearThreshold );
		out->addFloat("AutoDisableAngularThreshold",m_fAutoDisableAngularThreshold);
		out->addFloat("AutoDisableTime"            ,m_fAutoDisableTime            );
		out->addFloat("MaxAngularSpeed"            ,m_fMaxAngularSpeed            );
	}
}

void CIrrOdeDampable::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options) {
  CIrrOdeSceneNode::deserializeAttributes(in,options);

	m_sParamName=in->getAttributeAsString("paramName");
	m_bUseDefined=in->getAttributeAsBool("useDefined");

	CIrrOdeDampable *p=NULL;
	if (m_bUseDefined) p=CIrrOdeManager::getSharedInstance()->getDampableWithParamName(m_sParamName.c_str());

	if (p!=NULL && p!=this) {
		m_iAutoDisableSteps=p->getAutoDisableSteps();
		m_iAutoDisableFlag =p->getAutoDisableFlag();

		m_fDampingLinear              =p->getLinearDamping();
		m_fDampingAngular             =p->getAngularDamping();
		m_fDampingLinearThreshold     =p->getLinearDampingThreshold();
		m_fDampingAngularThreshold    =p->getAngularDampingThreshold();
		m_fAutoDisableLinearThreshold =p->getAutoDisableLinearThreshold();
		m_fAutoDisableAngularThreshold=p->getAutoDisableAngularThreshold();
		m_fAutoDisableTime            =p->getAutoDisableTime();
		m_fMaxAngularSpeed            =p->getMaxAngularSpeed();
	}
	else {
		m_iAutoDisableSteps=in->getAttributeAsInt("AutoDisableSteps");
		m_iAutoDisableFlag =in->getAttributeAsInt("AutoDisableFlag" );

		m_fDampingLinear              =in->getAttributeAsFloat("LinearDamping"              );
		m_fDampingAngular             =in->getAttributeAsFloat("AngularDamping"             );
		m_fDampingLinearThreshold     =in->getAttributeAsFloat("LinearDampingThreshold"     );
		m_fDampingAngularThreshold    =in->getAttributeAsFloat("AngularDampingThreshold"    );
		m_fAutoDisableLinearThreshold =in->getAttributeAsFloat("AutoDisableLinearThreshold" );
		m_fAutoDisableAngularThreshold=in->getAttributeAsFloat("AutoDisableAngularThreshold");
		m_fAutoDisableTime            =in->getAttributeAsFloat("AutoDisableTime"            );
		m_fMaxAngularSpeed            =in->getAttributeAsFloat("MaxAngularSpeed"            );
	}
}

void CIrrOdeDampable::copyParams(CIrrOdeSceneNode *pDest, bool bRecurse) {
  #ifdef _TRACE_COPY_PARAMS
    printf("CIrrOdeDampable copy params\n");
  #endif

  if (bRecurse) CIrrOdeSceneNode::copyParams(pDest);
  CIrrOdeDampable *pDst=(CIrrOdeDampable *)pDest;

  pDst->setLinearDamping              (m_fDampingLinear              );
  pDst->setAngularDamping             (m_fDampingAngular             );
  pDst->setLinearDampingThreshold     (m_fDampingLinearThreshold     );
  pDst->setAngularDampingThreshold    (m_fDampingAngularThreshold    );
  pDst->setAutoDisableLinearThreshold (m_fAutoDisableLinearThreshold );
  pDst->setAutoDisableAngularThreshold(m_fAutoDisableAngularThreshold);
  pDst->setAutoDisableTime            (m_fAutoDisableTime            );
  pDst->setAutoDisableSteps           (m_iAutoDisableSteps           );
  pDst->setAutoDisableFlag            (m_iAutoDisableFlag            );
  pDst->setMaxAngularSpeed            (m_fMaxAngularSpeed            );
}

} //namespace ode
} //namespace irr
