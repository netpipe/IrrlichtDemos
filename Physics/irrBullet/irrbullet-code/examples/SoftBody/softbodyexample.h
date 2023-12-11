// This example is part of irrBullet by Josiah Hartzell (fighterstw@hotmail.com or josiah.hartzell@gmail.com)

#ifndef __SOFT_BODY_EXAMPLE_H_INCLUDED__
#define __SOFT_BODY_EXAMPLE_H_INCLUDED__

#include "../framework/exampleframework.h"
#include <irrString.h>
#include <irrTypes.h>
#include <irrbulletcommon.h>

class ISoftBody;


class CSoftbodyExample : public CExampleFramework
{
    public:
        CSoftbodyExample();
        ~CSoftbodyExample();

        virtual void runExample();

        virtual bool OnEvent(const irr::SEvent& event);

        ISoftBody* createSoftbodyType(irr::u32 type, const irr::core::vector3df& position);

    private:
        int softVersusSoft;
};

#endif // __SOFT_BODY_EXAMPLE_H_INCLUDED__
