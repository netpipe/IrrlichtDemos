#ifndef CAPPLICATION_H
#define CAPPLICATION_H

#include <irrlicht.h>
class CStateManager;
class CApplication
{
    public:
        /** Default constructor */
        CApplication();
        /** Default destructor */
        virtual ~CApplication();

        void init(int x, int y);

        void run(void);
    protected:
        irr::IrrlichtDevice* Device;
        CStateManager* Manager;

        virtual void registerStates(void);
    private:
};

#endif // CAPPLICATION_H
