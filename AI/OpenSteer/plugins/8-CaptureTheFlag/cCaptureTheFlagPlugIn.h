/*! Irrlicht implementation by A.Buschhüter (http://abusoft.g0dsoft.com)*/
#ifndef C_CtfBase_PLUGIN_H
#define C_CtfBase_PLUGIN_H

#include <irrlicht.h>
#include "../../cOpenSteerDemo.h"
#include "cCaptureTheFlag.h"

using namespace OpenSteer;

// PlugIn for OpenSteerDemo
class CtfPlugIn : public PlugIn{
  private:
    //! GridSceneNode for the ground
#ifdef useIrrExtensions13
    irr::scene::IGridSceneNode* nGround;
#endif

  public:
    const char* name(){           return "8 - Capture the Flag";}
    float selectionOrderSortKey(){ return 8.0; } /*!< the selection order for "Next PlugIn" */

    void setInfoText(irr::gui::IGUIStaticText* st);

    //! a group (STL vector) of all vehicles in the PlugIn
    std::vector<CtfBase*>all;

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

    virtual ~CtfPlugIn(){}
};

#endif // C_CtfBase_PLUGIN_H
