#ifdef OPENSTEER3
/*! Irrlicht implementation by A.Buschhüter (http://abusoft.g0dsoft.com)*/
#ifndef C_TEMPLATE_PLUGIN_H
#define C_TEMPLATE_PLUGIN_H

#include <irrlicht.h>
#include "../../cOpenSteerDemo.h"
#include "cBoids.h"
#include "../../cMeshTool.h"

using namespace OpenSteer;

class SO : public SphereObstacle{
  void draw(const bool filled, const Color& color, const Vec3& vp) const{
  }
};
class RO : public RectangleObstacle{
  void draw(const bool filled, const Color& color, const Vec3& vp) const{
  }
};
class BO : public BoxObstacle{
  void draw(const bool filled, const Color& color, const Vec3& vp) const{
  }
};

// PlugIn for OpenSteerDemo
class BoidsPlugIn : public PlugIn{
  private:
    //! GridSceneNode for the ground
#ifdef useIrrExtensions13
    irr::scene::IGridSceneNode* nGround;
#endif

    //! Obstacles
    RO bigRectangle;
    BO outsideBigBox, insideBigBox;
    SO insideBigSphere, outsideSphere0, outsideSphere1, outsideSphere2,
       outsideSphere3, outsideSphere4, outsideSphere5, outsideSphere6;

  public:
    const char* name(){           return "3 - Boids";}
    float selectionOrderSortKey(){ return 3.0; } /*!< the selection order for "Next PlugIn" */

    void setInfoText(irr::gui::IGUIStaticText* st);

    // flock: a group (STL vector) of pointers to all boids
    Boid::groupType flock;
    typedef Boid::groupType::const_iterator iterator;
    // pointer to database used to accelerate proximity queries
    ProximityDatabase* pd;
    // keep track of current flock size
    int population;
    // which of the various proximity databases is currently in use
    int cyclePD;
    enum ConstraintType{none, insideSphere,
                        outsideSphere, outsideSpheres, outsideSpheresNoBig,
                        rectangle, rectangleNoBig,
                        outsideBox, insideBox};
    ConstraintType constraint;

    //! Create Scene
    void open();
    //! Reset Scene
    void reset();
    //! Clear Scene
    void close();
    //! Update Simulation
    void update(const float currentTime, const float elapsedTime);
    //! Redraw Scene (Irrlicht does all the stuff)
    void redraw(const float currentTime, const float elapsedTime);
    //! Returns a list of all vehicles
    const AVGroup& allVehicles();

    void nextPD();
    void handleFunctionKeys(int keyNumber);
    void printLQbinStats();
    void printMiniHelpForFunctionKeys();
    void addBoidToFlock();
    void removeBoidFromFlock();
    void nextBoundaryCondition();

    void initObstacles();
    // update Boid::obstacles list when constraint changes
    void updateObstacles();

    virtual ~BoidsPlugIn(){}
};

#endif // C_TEMPLATE_PLUGIN_H
#endif
