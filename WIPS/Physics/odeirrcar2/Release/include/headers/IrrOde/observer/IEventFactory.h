#ifndef _I_MESSAGE_FACTORY
  #define _I_MESSAGE_FACTORY

  #include <event/IIrrOdeEvent.h>

namespace irr {
namespace ode {

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


class CIrrOdeEventFactory;

/**
 * @class IIrrOdeEventFactory
 * @author Christian Keimel / bulletbyte.de
 * This is the interface of the IrrOde event factory. This factory is needed to replay
 * a file recorded with the CIrrOdeRecorder and for networking.
 */
class IIrrOdeEventFactory {
  public:
    /**
     * Override this function in order to create user defined events.
     * @param pData the data to serialize
     * @param pSmgr the scene manager to use
     * @see CSerializer
     */
    virtual IIrrOdeEvent *createMessage(CSerializer *pData, ISceneManager *pSmgr)=0;
};

}
}

#endif

