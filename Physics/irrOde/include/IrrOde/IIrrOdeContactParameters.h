#ifndef _I_IRR_ODE_CONTACT_PARAMETERS
  #define _I_IRR_ODE_CONTACT_PARAMETERS

  #include <CIrrOdeSurfaceParameters.h>

namespace irr {
namespace ode {
 
class IIrrOdeContactParameters {
  public:
    virtual void calculateContactParameters(CIrrOdeSurfaceParameters *pParam1, CIrrOdeSurfaceParameters *pParam2, CIrrOdeSurfaceParameters *pParams)=0;
};

} //namespace ode
} //namespace irr

#endif
