  #include <CIrrOdeSurfaceParameters.h>
  #include <CIrrOdeManager.h>

namespace irr {
namespace ode {

CIrrOdeSurfaceParameters::CIrrOdeSurfaceParameters() {
	m_sName=L"";

  mode=0;

  mu        =0;
  mu2       =0;
  bounce    =0;
  bounce_vel=0;
  soft_erp  =0;
  soft_cfm  =0;
  motion1   =0;
  motion2   =0;
  motionN   =0;
  slip1     =0;
  slip2     =0;

	m_bUseOther=false;

  m_fLinearDamp   =0.0f;
  m_fAngularDamp  =0.0f;
  m_fMaxLinearVel =0.0f;
  m_fMaxAngularVel=0.0f;
  m_fMaxLVelDamp  =0.0f;
  m_fMaxAVelDamp  =0.0f;

  m_bDamps =false;
  m_bStatic=false;
}

CIrrOdeSurfaceParameters::~CIrrOdeSurfaceParameters() { }

void CIrrOdeSurfaceParameters::setModeMu2(bool bIn) {
  if (bIn) mode|=eContactMu2; else mode^=(eContactMu2&mode);
}

void CIrrOdeSurfaceParameters::setModeFDir1(bool bIn) {
  if (bIn) mode|=eContactFDir1; else mode^=(eContactFDir1&mode);
}

void CIrrOdeSurfaceParameters::setModeBounce(bool bIn) {
  if (bIn) mode|=eContactBounce; else mode^=(eContactBounce&mode);
}

void CIrrOdeSurfaceParameters::setModeSoftERP(bool bIn) {
  if (bIn) mode|=eContactSoftERP; else mode^=(eContactSoftERP&mode);
}

void CIrrOdeSurfaceParameters::setModeSoftCFM(bool bIn) {
  if (bIn) mode|=eContactSoftCFM; else mode^=(eContactSoftCFM&mode);
}

void CIrrOdeSurfaceParameters::setModeMotion1(bool bIn) {
  if (bIn) mode|=eContactMotion1; else mode^=(eContactMotion1&mode);
}

void CIrrOdeSurfaceParameters::setModeMotion2(bool bIn) {
  if (bIn) mode|=eContactMotion2; else mode^=(eContactMotion2&mode);
}

void CIrrOdeSurfaceParameters::setModeMotionN(bool bIn) {
  if (bIn) mode|=eContactMotionN; else mode^=(eContactMotionN&mode);
}

void CIrrOdeSurfaceParameters::setModeSlip1(bool bIn) {
  if (bIn) mode|=eContactSlip1; else mode^=(eContactSlip1&mode);
}

void CIrrOdeSurfaceParameters::setModeSlip2(bool bIn) {
  if (bIn) mode|=eContactSlip2; else mode^=(eContactSlip2&mode);
}

void CIrrOdeSurfaceParameters::setModeApprox1_1(bool bIn) {
  if (bIn) mode|=eContactApprox1_1; else mode^=(eContactApprox1_1&mode);
}

void CIrrOdeSurfaceParameters::setModeApprox1_2(bool bIn) {
  if (bIn) mode|=eContactApprox1_2; else mode^=(eContactApprox1_2&mode);
}

void CIrrOdeSurfaceParameters::setModeApprox1(bool bIn) {
  if (bIn) mode|=eContactApprox1; else mode^=(eContactApprox1&mode);
}

bool CIrrOdeSurfaceParameters::getModeMu2() {
  return mode&eContactMu2;
}

bool CIrrOdeSurfaceParameters::getModeFDir1() {
  return (bool)(mode&eContactFDir1);
}

bool CIrrOdeSurfaceParameters::getModeBounce() {
  return (bool)(mode&eContactBounce);
}

bool CIrrOdeSurfaceParameters::getModeSoftERP() {
  return (bool)(mode&eContactSoftERP);
}

bool CIrrOdeSurfaceParameters::getModeSoftCFM() {
  return (bool)(mode&eContactSoftCFM);
}

bool CIrrOdeSurfaceParameters::getModeMotion1() {
  return (bool)(mode&eContactMotion1);
}

bool CIrrOdeSurfaceParameters::getModeMotion2() {
  return (bool)(mode&eContactMotion2);
}

bool CIrrOdeSurfaceParameters::getModeMotionN() {
  return (bool)(mode&eContactMotionN);
}

bool CIrrOdeSurfaceParameters::getModeSlip1() {
  return (bool)(mode&eContactSlip1);
}

bool CIrrOdeSurfaceParameters::getModeSlip2() {
  return (bool)(mode&eContactSlip2);
}

bool CIrrOdeSurfaceParameters::getModeApprox1_1() {
  return (bool)(mode&eContactApprox1_1);
}

bool CIrrOdeSurfaceParameters::getModeApprox1_2() {
  return (bool)(mode&eContactApprox1_2);
}

bool CIrrOdeSurfaceParameters::getModeApprox1() {
  return (bool)(mode&eContactApprox1);
}

void CIrrOdeSurfaceParameters::initPhysics() {
  #ifndef _IRR_ODE_SURFACE_EDIT
    CIrrOdeSurfaceParameters *pOther=CIrrOdeManager::getSharedInstance()->getSurfaceParameter(m_sName);
    if (pOther) pOther->copy(this);
  #endif
}

void CIrrOdeSurfaceParameters::serializeAttributes(irr::io::IAttributes* out, irr::io::SAttributeReadWriteOptions* options, u32 iIdx) const {
  c8 sGroup[0xFF],sName[0xFF];
  CIrrOdeSurfaceParameters *pOther=NULL;
  #ifndef _IRR_ODE_SURFACE_EDIT
    pOther=CIrrOdeManager::getSharedInstance()->getSurfaceParameter(m_sName);
  #endif

  if (iIdx==0) strcpy(sGroup,""); else sprintf(sGroup,"_mat%i",iIdx+1);

	sprintf(sName,"name%s",sGroup); out->addString(sName,m_sName.c_str());

	sprintf(sName,"useDefined%s",sGroup); out->addBool(sName,(pOther==this || m_sName=="")?false:m_bUseOther);

	if (!m_bUseOther || m_sName=="" || pOther==NULL || pOther==this) {
		sprintf(sName,"mode_mu2%s"      ,sGroup); out->addBool(sName,(bool)(mode&eContactMu2      ));
		sprintf(sName,"mode_fdir1%s"    ,sGroup); out->addBool(sName,(bool)(mode&eContactFDir1    ));
		sprintf(sName,"mode_bounce%s"   ,sGroup); out->addBool(sName,(bool)(mode&eContactBounce   ));
		sprintf(sName,"mode_softERP%s"  ,sGroup); out->addBool(sName,(bool)(mode&eContactSoftERP  ));
		sprintf(sName,"mode_softCFM%s"  ,sGroup); out->addBool(sName,(bool)(mode&eContactSoftCFM  ));
		sprintf(sName,"mode_motion1%s"  ,sGroup); out->addBool(sName,(bool)(mode&eContactMotion1  ));
		sprintf(sName,"mode_motion2%s"  ,sGroup); out->addBool(sName,(bool)(mode&eContactMotion2  ));
		sprintf(sName,"mode_motionN%s"  ,sGroup); out->addBool(sName,(bool)(mode&eContactMotionN  ));
		sprintf(sName,"mode_slip1%s"    ,sGroup); out->addBool(sName,(bool)(mode&eContactSlip1    ));
		sprintf(sName,"mode_slip2%s"    ,sGroup); out->addBool(sName,(bool)(mode&eContactSlip2    ));
		sprintf(sName,"mode_approx1_1%s",sGroup); out->addBool(sName,(bool)(mode&eContactApprox1_1));
		sprintf(sName,"mode_approx1_2%s",sGroup); out->addBool(sName,(bool)(mode&eContactApprox1_2));

		sprintf(sName,"mu%s"        ,sGroup); out->addFloat(sName,mu        );
		sprintf(sName,"mu2%s"       ,sGroup); out->addFloat(sName,mu2       );
		sprintf(sName,"bounce%s"    ,sGroup); out->addFloat(sName,bounce    );
		sprintf(sName,"bounce_vel%s",sGroup); out->addFloat(sName,bounce_vel);
		sprintf(sName,"soft_erp%s"  ,sGroup); out->addFloat(sName,soft_erp  );
		sprintf(sName,"soft_cfm%s"  ,sGroup); out->addFloat(sName,soft_cfm  );
		sprintf(sName,"motion1%s"   ,sGroup); out->addFloat(sName,motion1   );
		sprintf(sName,"motion2%s"   ,sGroup); out->addFloat(sName,motion2   );
		sprintf(sName,"motionN%s"   ,sGroup); out->addFloat(sName,motionN   );
		sprintf(sName,"slip1%s"     ,sGroup); out->addFloat(sName,slip1     );
		sprintf(sName,"slip2%s"     ,sGroup); out->addFloat(sName,slip2     );

		if (m_bStatic) {
		  sprintf(sName,"damps%s",sGroup); out->addBool(sName,m_bDamps);
		  if (m_bDamps) {
		    sprintf(sName,"linearDamping%s" ,sGroup); out->addFloat(sName,m_fLinearDamp   );
		    sprintf(sName,"angularDamping%s",sGroup); out->addFloat(sName,m_fAngularDamp  );
		    sprintf(sName,"maxLinearVel%s"  ,sGroup); out->addFloat(sName,m_fMaxLinearVel );
		    sprintf(sName,"maxAngularVel%s" ,sGroup); out->addFloat(sName,m_fMaxAngularVel);

		    if (m_fMaxLinearVel >0.0f) { sprintf(sName,"maxLVelDamping%s",sGroup); out->addFloat(sName,m_fMaxLVelDamp); }
		    if (m_fMaxAngularVel>0.0f) { sprintf(sName,"maxAVelDamping%s",sGroup); out->addFloat(sName,m_fMaxAVelDamp); }
		  }
		}
	}
}

void CIrrOdeSurfaceParameters::deserializeAttributes(irr::io::IAttributes* in, irr::io::SAttributeReadWriteOptions* options, u32 iIdx) {
  c8 sGroup[0xFF],sName[0xFF];
  CIrrOdeSurfaceParameters *pOther=NULL;
  #ifndef _IRR_ODE_SURFACE_EDIT
    pOther=CIrrOdeManager::getSharedInstance()->getSurfaceParameter(m_sName);
  #endif

  if (iIdx==0) strcpy(sGroup,""); else sprintf(sGroup,"_mat%i",iIdx+1);

	sprintf(sName,"name%s",sGroup); m_sName=in->getAttributeAsString(sName);

	if (pOther==this || m_sName=="") m_bUseOther=false;
	sprintf(sName,"useDefined%s",sGroup); m_bUseOther=in->getAttributeAsBool(sName);

	if (!m_bUseOther || m_sName=="" || pOther==NULL || pOther==this) {
		sprintf(sName,"mode_mu2%s"      ,sGroup); setModeMu2      (in->getAttributeAsBool(sName));
		sprintf(sName,"mode_fdir1%s"    ,sGroup); setModeFDir1    (in->getAttributeAsBool(sName));
		sprintf(sName,"mode_bounce%s"   ,sGroup); setModeBounce   (in->getAttributeAsBool(sName));
		sprintf(sName,"mode_softERP%s"  ,sGroup); setModeSoftERP  (in->getAttributeAsBool(sName));
		sprintf(sName,"mode_softCFM%s"  ,sGroup); setModeSoftCFM  (in->getAttributeAsBool(sName));
		sprintf(sName,"mode_motion1%s"  ,sGroup); setModeMotion1  (in->getAttributeAsBool(sName));
		sprintf(sName,"mode_motion2%s"  ,sGroup); setModeMotion2  (in->getAttributeAsBool(sName));
		sprintf(sName,"mode_motionN%s"  ,sGroup); setModeMotionN  (in->getAttributeAsBool(sName));
		sprintf(sName,"mode_slip1%s"    ,sGroup); setModeSlip1    (in->getAttributeAsBool(sName));
		sprintf(sName,"mode_slip2%s"    ,sGroup); setModeSlip2    (in->getAttributeAsBool(sName));
		sprintf(sName,"mode_approx1_1%s",sGroup); setModeApprox1_1(in->getAttributeAsBool(sName));
		sprintf(sName,"mode_approx1_2%s",sGroup); setModeApprox1_2(in->getAttributeAsBool(sName));

		sprintf(sName,"mu%s"        ,sGroup); mu        =in->getAttributeAsFloat(sName);
		sprintf(sName,"mu2%s"       ,sGroup); mu2       =in->getAttributeAsFloat(sName);
		sprintf(sName,"bounce%s"    ,sGroup); bounce    =in->getAttributeAsFloat(sName);
		sprintf(sName,"bounce_vel%s",sGroup); bounce_vel=in->getAttributeAsFloat(sName);
		sprintf(sName,"soft_erp%s"  ,sGroup); soft_erp  =in->getAttributeAsFloat(sName);
		sprintf(sName,"soft_cfm%s"  ,sGroup); soft_cfm  =in->getAttributeAsFloat(sName);
		sprintf(sName,"motion1%s"   ,sGroup); motion1   =in->getAttributeAsFloat(sName);
		sprintf(sName,"motion2%s "  ,sGroup); motion2   =in->getAttributeAsFloat(sName);
		sprintf(sName,"motionN%s"   ,sGroup); motionN   =in->getAttributeAsFloat(sName);
		sprintf(sName,"slip1%s"     ,sGroup); slip1     =in->getAttributeAsFloat(sName);
		sprintf(sName,"slip2%s"     ,sGroup); slip2     =in->getAttributeAsFloat(sName);

		if (m_bStatic) {
		  sprintf(sName,"damps%s",sGroup); m_bDamps=in->getAttributeAsBool(sName);
		  if (m_bDamps) {
		    sprintf(sName,"linearDamping%s" ,sGroup); m_fLinearDamp   =in->getAttributeAsFloat(sName);
		    sprintf(sName,"angularDamping%s",sGroup); m_fAngularDamp  =in->getAttributeAsFloat(sName);
		    sprintf(sName,"maxLinearVel%s"  ,sGroup); m_fMaxLinearVel =in->getAttributeAsFloat(sName);
		    sprintf(sName,"maxAngularVel%s" ,sGroup); m_fMaxAngularVel=in->getAttributeAsFloat(sName);

		    if (m_fMaxLinearVel >0.0f) { sprintf(sName,"maxLVelDamping%s",sGroup); m_fMaxLVelDamp=in->getAttributeAsFloat(sName); }
		    if (m_fMaxAngularVel>0.0f) { sprintf(sName,"maxAVelDamping%s",sGroup); m_fMaxAVelDamp=in->getAttributeAsFloat(sName); }
		  }
		}
	}
	else {
		pOther->copy(this);
	}
}

void CIrrOdeSurfaceParameters::copy(CIrrOdeSurfaceParameters *pDest) {
  pDest->setUseDefined(m_bUseOther);
  pDest->setName(getName());
  pDest->mode=mode;
  pDest->mu=mu;
  pDest->mu2=mu2;
  pDest->bounce=bounce;
  pDest->bounce_vel=bounce_vel;
  pDest->soft_cfm=soft_cfm;
  pDest->soft_erp=soft_erp;
  pDest->motion1=motion1;
  pDest->motion2=motion2;
  pDest->motionN=motionN;
  pDest->slip1=slip1;
  pDest->slip2=slip2;
  pDest->m_bDamps=m_bDamps;
  //pDest->m_bStatic=m_bStatic;
  pDest->m_fLinearDamp=m_fLinearDamp;
  pDest->m_fAngularDamp=m_fAngularDamp;
  pDest->m_fMaxLinearVel=m_fMaxLinearVel;
  pDest->m_fMaxAngularVel=m_fMaxAngularVel;
  pDest->m_fMaxAVelDamp=m_fMaxAVelDamp;
  pDest->m_fMaxLVelDamp=m_fMaxLVelDamp;
}

const wchar_t *CIrrOdeSurfaceParameters::getName() {
	return m_sName.c_str();
}

void CIrrOdeSurfaceParameters::setName(const wchar_t *sName) {
  m_sName=sName;
}

bool CIrrOdeSurfaceParameters::doesUseDefined() {
  return m_bUseOther;
}

void CIrrOdeSurfaceParameters::setUseDefined(bool b) {
  m_bUseOther=b;
}

} //namespace ode
} //namespace irr
