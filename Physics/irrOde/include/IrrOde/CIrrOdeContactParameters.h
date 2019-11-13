#ifndef _IRR_ODE_CONTACT_PARAMETERS
  #define _IRR_ODE_CONTACT_PARAMETERS

  #include <IIrrOdeContactParameters.h>

namespace irr {
namespace ode {

/**
 * @class CIrrOdeContactParameters
 * This class is an implementation of IIrrOdeContactParameters and used to calculate the contact parameters if two
 * geoms collide.
 */
class CIrrOdeContactParameters : public IIrrOdeContactParameters {
  public:
    /**
     * A callback function that is called if two geoms collide
     * @param pGeom1 the first geom
     * @param pGeom2 the second geom
     * @param pParams a pointer to a CIrrOdeSurfaceParameters class. This class will hold the resulting contact parameters
     */
    virtual void calculateContactParameters(CIrrOdeSurfaceParameters *pParam1, CIrrOdeSurfaceParameters *pParam2, CIrrOdeSurfaceParameters *pParams);
};

} //namespace ode
} //namespace irr

#endif

