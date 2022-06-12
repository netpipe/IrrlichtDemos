
#ifdef OPENSTEER
/*! Irrlicht implementation by A.Buschhüter (http://abusoft.g0dsoft.com)*/
#ifndef C_LowSpeedTurnPlugIn_H
#define C_LowSpeedTurnPlugIn_H

#include <irrlicht.h>
#include "../../cOpenSteerDemo.h"
#include "cLowSpeedTurn.h"

using namespace OpenSteer;

// PlugIn for OpenSteerDemo
class LowSpeedTurnPlugIn : public PlugIn{
  private:
    //! GridSceneNode for the ground
#ifdef useIrrExtensions13
    irr::scene::IGridSceneNode* nGround;
#endif

  public:
    const char* name(){           return "2 - Low Speed Turn";}
    float selectionOrderSortKey(){ return 2.0; } /*!< the selection order for "Next PlugIn" */

    void setInfoText(irr::gui::IGUIStaticText* st);

    std::vector<LowSpeedTurn*> all; // list of all Vehicles
    typedef std::vector<LowSpeedTurn*>::const_iterator iterator;

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

    virtual ~LowSpeedTurnPlugIn(){}
};

#endif // C_LowSpeedTurnPlugIn_H
#endif
