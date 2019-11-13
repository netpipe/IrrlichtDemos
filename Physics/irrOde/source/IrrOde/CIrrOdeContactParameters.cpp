  #include <CIrrOdeContactParameters.h>
  #include <geom/CIrrOdeGeom.h>

namespace irr {
namespace ode {

void CIrrOdeContactParameters::calculateContactParameters(CIrrOdeSurfaceParameters *pParam1, CIrrOdeSurfaceParameters *pParam2, CIrrOdeSurfaceParameters *pParams) {
  pParams->setMode     (pParam1->getMode()&pParam2->getMode()                );
  pParams->setMu       (sqrt(pParam1->getMu()       *pParam2->getMu()       ));
  pParams->setMu2      (sqrt(pParam1->getMu2()      *pParam2->getMu2()      ));
  pParams->setBounce   (sqrt(pParam1->getBounce()   *pParam2->getBounce()   ));
  pParams->setBounceVel(sqrt(pParam1->getBounceVel()*pParam2->getBounceVel()));
  pParams->setSoftErp  (sqrt(pParam1->getSoftErp()  *pParam2->getSoftErp()  ));
  pParams->setSoftCfm  (sqrt(pParam1->getSoftCfm()  *pParam2->getSoftCfm()  ));
  pParams->setMotion1  (sqrt(pParam1->getMotion1()  *pParam2->getMotion1()  ));
  pParams->setMotion2  (sqrt(pParam1->getMotion2()  *pParam2->getMotion2()  ));
  pParams->setMotionN  (sqrt(pParam1->getMotionN()  *pParam2->getMotionN()  ));
  pParams->setSlip1    (sqrt(pParam1->getSlip1()    *pParam2->getSlip1()    ));
  pParams->setSlip2    (sqrt(pParam1->getSlip2()    *pParam2->getSlip2()    ));
}

} //namespace ode
} //namespace irr
