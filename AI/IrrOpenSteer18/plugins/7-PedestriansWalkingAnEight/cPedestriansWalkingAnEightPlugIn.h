#include "../../../../config.h"
#ifdef OPENSTEER2
/*! Irrlicht implementation by A.Buschhüter (http://abusoft.g0dsoft.com)*/
#ifndef C_TEMPLATE_PLUGIN_H
#define C_TEMPLATE_PLUGIN_H

#include <irrlicht.h>
#include "../../cOpenSteerDemo.h"
#include "../../cMeshTool.h"
#include "cPedestriansWalkingAnEight.h"

// creates a path for the PlugIn
PolylineSegmentedPathwaySingleRadius* getTestPath8();

using namespace OpenSteer;

// PlugIn for OpenSteerDemo
class Pedestrian8PlugIn : public PlugIn{
  private:
    //! GridSceneNode for the ground
#ifdef useIrrExtensions13
    irr::scene::IGridSceneNode* nGround;
#endif

  public:
    const char* name(){return            "7 - Pedestrian Walking an Eight";}
    float selectionOrderSortKey(){ return 7.0; } /*!< the selection order for "Next PlugIn" */

    void setInfoText(irr::gui::IGUIStaticText* st);

    // crowd: a group (STL vector) of all Pedestrians
    Pedestrian8::groupType crowd;
    typedef Pedestrian8::groupType::const_iterator iterator;
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

    void nextPD();
    void removePedestrianFromCrowd();
    void addPedestrianToCrowd();

    void printMiniHelpForFunctionKeys();
    void handleFunctionKeys(int keyNumber);

    virtual ~Pedestrian8PlugIn(){}
};

#endif // C_TEMPLATE_PLUGIN_H
#endif
