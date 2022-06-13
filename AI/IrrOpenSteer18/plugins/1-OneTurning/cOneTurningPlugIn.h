#ifdef OPENSTEER
/*! Irrlicht implementation by A.Buschhüter (http://abusoft.g0dsoft.com)*/
#ifndef CONETURNINGPLUGIN_H
#define CONETURNINGPLUGIN_H

#include <irrlicht.h>
#include "../../cOpenSteerDemo.h"
#include "cOneTurning.h"

using namespace OpenSteer;

// PlugIn for OpenSteerDemo
class OneTurningPlugIn : public PlugIn{
  private:
    //! GridSceneNode for the ground
#ifdef useIrrExtensions13
    irr::scene::IGridSceneNode* nGround;
#endif

  public:
    const char* name(){           return "1 - One Turning Away";}
    float selectionOrderSortKey(){ return 1.0; } /*!< the selection order for "Next PlugIn" */
    //! return 'true' to be default plugin !!!
    bool requestInitialSelection(void){ return true; }

    void setInfoText(irr::gui::IGUIStaticText* st);

    OneTurning* gOneTurning;
    std::vector<OneTurning*> theVehicle; // list of all Vehicles

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

    virtual ~OneTurningPlugIn(){}
};

#endif // CONETURNINGPLUGIN_H
#endif
