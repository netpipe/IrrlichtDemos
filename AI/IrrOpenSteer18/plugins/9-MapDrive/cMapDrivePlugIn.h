#include "../../../../config.h"
#ifdef OPENSTEER2
/*! Irrlicht implementation by A.Buschhüter (http://abusoft.g0dsoft.com)*/
#ifndef C_MapDrive_PLUGIN_H
#define C_MapDrive_PLUGIN_H

#include <irrlicht.h>
#include "../../cOpenSteerDemo.h"
#include "cMapDrive.h"

using namespace OpenSteer;

// PlugIn for OpenSteerDemo
class MapDrivePlugIn : public PlugIn{
  private:
    //! GridSceneNode for the ground
#ifdef useIrrExtensions13
    irr::scene::IGridSceneNode* nGround;
#endif

  public:
    const char* name(){return "           9 - Driving through map based obstacles";}
    float selectionOrderSortKey(){ return 9.0; } /*!< the selection order for "Next PlugIn" */

    void setInfoText(irr::gui::IGUIStaticText* st);

    MapDriver* vehicle;
    std::vector<MapDriver*> vehicles; // for allVehicles
    float initCamDist, initCamElev;
    bool usePathFences;
    bool useRandomRocks;

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

    void reversePathFollowDirection();
    void togglePathFences();
    void toggleRandomRocks();
    void toggleCurvedSteering();
    void regenerateMap();
    void clearCenterOfMap(TerrainMap& map);
    void selectNextDemo();

    void printMiniHelpForFunctionKeys();
    void handleFunctionKeys(int keyNumber);

    virtual ~MapDrivePlugIn(){}
};

#endif // C_MapDrive_PLUGIN_H
#endif
