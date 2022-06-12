
#ifdef OPENSTEER
/*! Irrlicht implementation by A.Buschhüter (http://abusoft.g0dsoft.com)*/
#ifndef C_Pedestrian_PLUGIN_H
#define C_Pedestrian_PLUGIN_H

#include <irrlicht.h>
#include "../../cOpenSteerDemo.h"
#include "cPedestrian.h"

using namespace OpenSteer;

// creates a path for the PlugIn
PolylineSegmentedPathwaySingleRadius* getTestPath();

// PlugIn for OpenSteerDemo
class PedestrianPlugIn : public PlugIn{
  private:
    //! GridSceneNode for the ground
#ifdef useIrrExtensions13
    irr::scene::IGridSceneNode* nGround;
#endif

  public:
    const char* name(){           return "6 - Pedestrians";}
    float selectionOrderSortKey(){ return 6.0; } /*!< the selection order for "Next PlugIn" */

    void setInfoText(irr::gui::IGUIStaticText* st);

    // crowd: a group (STL vector) of all Pedestrians
    Pedestrian::groupType crowd;
    typedef Pedestrian::groupType::const_iterator iterator;
    Vec3 gridCenter;
    // pointer to database used to accelerate proximity queries
    ProximityDatabase* pd;
    // keep track of current flock size
    int population;
    // which of the various proximity databases is currently in use
    int cyclePD;

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

    void handleFunctionKeys(int keyNumber);
    void printMiniHelpForFunctionKeys();
    void addPedestrianToCrowd();
    void removePedestrianFromCrowd();
    void nextPD();

    virtual ~PedestrianPlugIn(){}
};

#endif // C_Pedestrian_PLUGIN_H
#endif
