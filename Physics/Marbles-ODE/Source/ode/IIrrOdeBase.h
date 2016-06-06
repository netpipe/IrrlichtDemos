#ifndef _IRRODEBASE
  #define _IRRODEBASE

  #include <ode/ode.h>
  #include <irrlicht.h>

namespace irr
{
  namespace ode
  {
    const double GRAD_PI =180.0 / 3.1415926535897932384626433832795;
    const double GRAD_PI2=3.1415926535897932384626433832795 / 180.0;

    /**
     * This is the base interface for all irr::ode objects. It holds the member
     * and methods for memory management
     */
    class IIrrOdeBase
    {
      private:
        irr::c8 m_sName[50];  /**< Name of the Object */

      public:
        virtual ~IIrrOdeBase() { }       /**< Virtual Destructor */

        void setName(irr::c8 *sName) { strcpy(m_sName,sName); }
        const irr::c8 *getName() { return m_sName; }
    };

  }
}

#endif