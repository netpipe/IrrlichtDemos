#ifndef IRR_OPENSTEER_H
#define IRR_OPENSTEER_H

/*! \mainpage

    \section global constants
      const LocalSpace gGlobalSpace;

*/

#include <irrlicht.h>
#include "osSteerLibrary.h"
#include "osAbstractVehicle.h"
#include "osClock.h"
#include "osColor.h"
#include "oslq.h"
#include "osObstacle.h"
#include "osPath.h"
#include "osPathway.h"
#include "osPlugIn.h"
#include "osPolylineSegmentedPath.h"
#include "osProximity.h"
#include "osQueryPathAlike.h"
#include "osQueryPathAlikeBaseDataExtractionPolicies.h"
#include "osQueryPathAlikeMappings.h"
#include "osSegmentedPath.h"
#include "osSegmentedPathAlikeUtilities.h"
#include "osSegmentedPathway.h"
#include "osSharedPointer.h"
#include "osSimpleVehicle.h"
#include "osStandardTypes.h"
#include "osTerrainRayTest.h"
#include "osUnusedParameter.h"
#include "osUtilities.h"
#include "osVec3.h"
#include "osVec3Utilities.h"
#include "osQueryPathAlikeUtilities.h"
#include "osOldPathway.h"
#include "osLocalSpace.h"
#include "osAnnotation.h"
#include "osPolylineSegmentedPathwaySingleRadius.h"
#include "osPolylineSegmentedPathwaySegmentRadii.h"
#include "osWaypointMap.h"

namespace OpenSteer{
  //! convert rotation vector to forward vector
  static Vec3 getForwardFromRotation(Vec3 r) {
    irr::core::matrix4 mat;
    mat.setRotationDegrees(r.vector3df());
    Vec3 ff;
    ff.set(mat(2,0), mat(2,1), mat(2,2));
    ff.normalize();
    return ff;
  }
  //! convert forward vector to rotation vector
  static Vec3 getRotationFromForward(Vec3 r) {
    Vec3 ff = r.vector3df().getHorizontalAngle();
    return ff;
  }

} // namespace OpenSteer

#endif // IRR_OPENSTEER_H
