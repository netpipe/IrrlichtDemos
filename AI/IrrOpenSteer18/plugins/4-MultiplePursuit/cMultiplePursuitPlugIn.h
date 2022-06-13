#ifdef OPENSTEER
/*! Irrlicht implementation by A.Buschhüter (http://abusoft.g0dsoft.com)*/
#ifndef C_MpPlugIn_H
#define C_MpPlugIn_H

#include <irrlicht.h>
#include "../../cOpenSteerDemo.h"
#include "cMultiplePursuit.h"

using namespace OpenSteer;

// PlugIn for OpenSteerDemo
class MpPlugIn : public PlugIn{
  private:
    //! GridSceneNode for the ground
#ifdef useIrrExtensions13
    irr::scene::IGridSceneNode* nGround;
#endif

  public:
    const char* name(){           return "4 - Multiple Pursuit";}
    float selectionOrderSortKey(){ return 4.0; } /*!< the selection order for "Next PlugIn" */

    void setInfoText(irr::gui::IGUIStaticText* st);

    // a group (STL vector) of all vehicles
    std::vector<MpBase*> allMP;
    typedef std::vector<MpBase*>::const_iterator iterator;
    iterator pBegin, pEnd;
    MpWanderer* wanderer;

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

    virtual ~MpPlugIn(){}
};

#endif // C_MpPlugIn_H
#endif
