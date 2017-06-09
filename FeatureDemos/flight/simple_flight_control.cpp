
    #include "simple_flight_control.h"

    using namespace irr;


    SimpleFlightControl::SimpleFlightControl
    (
        scene::ISceneManager* smgr,
        scene::ISceneNode* node,
        f32 speed
    )
    :    Smgr(smgr),
        Node(node),

        RootEmpty(0),
        YawEmpty(0),
        PitchEmpty(0),
        RollEmpty(0),

        PitchChange(0.0f),

        Speed(speed),
        LastTime(0)
    {

        RootEmpty = Smgr->addEmptySceneNode(0);
        YawEmpty = Smgr->addEmptySceneNode(RootEmpty);
        PitchEmpty = Smgr->addEmptySceneNode(YawEmpty);
        RollEmpty = Smgr->addEmptySceneNode(PitchEmpty);
        Node->setParent(RollEmpty);

        //RollEmpty = Smgr->addEmptySceneNode(YawEmpty);
        //PitchEmpty = Smgr->addEmptySceneNode(RollEmpty);
        //Node->setParent(PitchEmpty);

        Node->setVisible(true);
    }


    SimpleFlightControl::~SimpleFlightControl()
    {

    }

    bool SimpleFlightControl::OnEvent(const SEvent& event)
    {
        if

        (

            (event.EventType == EET_KEY_INPUT_EVENT)

            && (event.KeyInput.PressedDown == true)

        )

        {

            if (event.KeyInput.Key == KEY_KEY_Q)

            {

            }

            else if (event.KeyInput.Key == KEY_KEY_E)

            {

            }




            else if (event.KeyInput.Key == KEY_KEY_A)

            {

                this->roll(3.99f);

            }

            else if (event.KeyInput.Key == KEY_KEY_D)

            {

                this->roll(-3.99f);

            }




            else if (event.KeyInput.Key == KEY_KEY_W)

            {

                this->pitch(1.99f);

            }

            else if (event.KeyInput.Key == KEY_KEY_S)

            {

                this->pitch(-1.99f);

            }



            else if (event.KeyInput.Key == KEY_KEY_Z)

            {

                this->brake();
            }

            else if (event.KeyInput.Key == KEY_KEY_X)

            {

                this->stop();
            }

            else if (event.KeyInput.Key == KEY_KEY_C)

            {

                this->accelerate();
            }

        }
        return false;
    }


    void SimpleFlightControl::resetRotation()
    {
        Node->setRotation(core::vector3df(0.0f));
        RootEmpty->setRotation(core::vector3df(0.0f));
        YawEmpty->setRotation(core::vector3df(0.0f));
        PitchEmpty->setRotation(core::vector3df(0.0f));
        RollEmpty->setRotation(core::vector3df(0.0f));

    }


    void SimpleFlightControl::showDebugInfo()
    {
        f32 x = PitchEmpty->getRotation().X;
        f32 z = RollEmpty->getRotation().Z;
        printf("PitchRotX=%f, RollRotZ=%f\n", x, z);
    }


    ////////////////////////////////////////////////////////////////////////////////
    void SimpleFlightControl::update(u32 time)
    {
        f32 deltaTime = f32(time - LastTime);


        f32 yawDegs = 0.0f;

        f32 pitchEmptyRotX = PitchEmpty->getRotation().X;
        if ((pitchEmptyRotX > 90.0f) && (pitchEmptyRotX < 270.0f)) // inverted
            yawDegs = sin(RollEmpty->getRotation().Z * core::DEGTORAD) * deltaTime * 0.001f * YAW_RATE;
        else
            yawDegs = - sin(RollEmpty->getRotation().Z * core::DEGTORAD) * deltaTime * 0.001f * YAW_RATE;

        this->yaw(yawDegs);

        if (core::isnotzero(Speed))
            move(deltaTime * Speed);

        LastTime = time;
    }

    ////////////////////////////////////////////////////////////////////////////////





    void SimpleFlightControl::move(f32 distance)
    {

        updateAbsolutePosition();
        core::matrix4 matrix = RollEmpty->getAbsoluteTransformation();
        core::vector3df vect(0.0f, 0.0f, 1.0f);
        matrix.rotateVect(vect);
        vect.setLength(distance);

        core::vector3df pos = RootEmpty->getPosition() + vect;
        RootEmpty->setPosition(pos);
    }


    void SimpleFlightControl::rotate(scene::ISceneNode* node, const core::vector3df& rot)
    {
        core::matrix4 m;
        m.setRotationDegrees(node->getRotation());

        core::matrix4 n;
        n.setRotationDegrees(rot);
        m *= n;

        node->setRotation(m.getRotationDegrees());
    }


    void SimpleFlightControl::yaw(f32 degs)
    {
        rotate(YawEmpty, core::vector3df(0.0f, degs, 0.0f));
        //printf("YawEmpty->getRotation().Y=%f\n", YawEmpty->getRotation().Y);
    }


    void SimpleFlightControl::pitch(f32 degs)
    {
        f32 z = RollEmpty->getRotation().Z;
        if ((z>90.0f) && (z<270.0f))
            rotate(PitchEmpty, core::vector3df(degs, 0.0f, 0.0f));
        else
            rotate(PitchEmpty, core::vector3df(-degs, 0.0f, 0.0f));
        //printf("PitchEmpty->getRotation().X=%f\n", PitchEmpty->getRotation().X);
    }


    void SimpleFlightControl::roll(f32 degs)
    {
        rotate(RollEmpty, core::vector3df(0.0f, 0.0f, degs));
        //printf("RollEmpty->getRotation().Z=%f\n", RollEmpty->getRotation().Z);

    }


    void SimpleFlightControl::accelerate()
    {
        Speed += ACCELERATION;
        if (Speed > MAX_SPEED)
            Speed = MAX_SPEED;
    }


    void SimpleFlightControl::brake()
    {
        if (core::iszero(Speed))
        {
            Speed = MIN_SPEED;
            return;
        }

        Speed -= DECELERATION;
        if (Speed < MIN_SPEED)
            Speed = MIN_SPEED;
    }


    core::vector3df SimpleFlightControl::getVelocity()
    {
        core::matrix4 matrix = RollEmpty->getAbsoluteTransformation();
        core::vector3df heading(0.0f, 0.0f, 1.0f);
        matrix.rotateVect(heading);

        return Speed * heading;
    }


    void SimpleFlightControl::updateAbsolutePosition()
    {
        Node->updateAbsolutePosition();
        RootEmpty->updateAbsolutePosition();
        YawEmpty->updateAbsolutePosition();
        PitchEmpty->updateAbsolutePosition();
        RollEmpty->updateAbsolutePosition();
    }


    f32 SimpleFlightControl::getRollAngleDegs() const
    {
        return RollEmpty->getRotation().Z;
    }


    bool SimpleFlightControl::isInverted()
    {
        core::vector3df yaxis(0.0f, 1.0f, 0.0f);
        core::matrix4 matrix = Node->getAbsoluteTransformation();
        matrix.rotateVect(yaxis);
            return (yaxis.Y < 0.0f);
    }
