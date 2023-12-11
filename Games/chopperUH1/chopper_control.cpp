
    #include "chopper_control.h"


    using namespace irr;


    ChopperControl::ChopperControl
    (
            IrrlichtDevice* device,
            scene::ISceneNode* node
    )
    :       Device(device),
            Smgr(Device->getSceneManager()),
            Node(node),

            RootEmpty(0),
            YawEmpty(0),
            PitchEmpty(0),
            RollEmpty(0),

            PitchChange(0.0f),

            Speed(1.0f),
            Power(HOVER_POWER),
            LastTime(0),

            ResetRollStartTime(0),
            OldRollAngleDegs(0.0f),
            IsResettingRoll(false),

            ResetPitchStartTime(0),
            OldPitchAngleDegs(0.0f),
            IsResettingPitch(false),

            ResetHoverStartTime(0),
            OldPower(0.0f),
            IsResettingHover(false)

    {

            RootEmpty = Smgr->addEmptySceneNode(0);
            YawEmpty = Smgr->addEmptySceneNode(RootEmpty);
            PitchEmpty = Smgr->addEmptySceneNode(YawEmpty);
            RollEmpty = Smgr->addEmptySceneNode(PitchEmpty);
            Node->setParent(RollEmpty);

            //Node->setVisible(true);
    }


    ChopperControl::~ChopperControl()
    {

    }

    bool ChopperControl::OnEvent(const SEvent& event)
    {
            if
            (
                    (event.EventType == EET_KEY_INPUT_EVENT)
                    && (event.KeyInput.PressedDown == true)
            )
            {
                    if (event.KeyInput.Key == irr::KEY_KEY_W)

                    {

                            this->pitchDown();

                    }

                    else if (event.KeyInput.Key == irr::KEY_KEY_S)

                    {

                            this->pitchUp();

                    }

                    else if (event.KeyInput.Key == irr::KEY_KEY_H)

                    {

                            this->hover();

                    }





                    else if (event.KeyInput.Key == irr::KEY_KEY_Q) //

                    {

                            this->rollLeft();

                    }

                    else if (event.KeyInput.Key == irr::KEY_KEY_E)

                    {

                            this->rollRight();

                    }



                    else if (event.KeyInput.Key == irr::KEY_KEY_R) //

                    {

                            this->resetRoll();

                            this->resetPitch();

                    }



                    else if (event.KeyInput.Key == irr::KEY_KEY_A)

                    {

                            this->turnLeft();

                    }

                    else if (event.KeyInput.Key == irr::KEY_KEY_D)

                    {

                            this->turnRight();

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
                            this->throttle();
                    }
            }
            return false;
    }


    bool ChopperControl::onCollision(const scene::ISceneNodeAnimatorCollisionResponse& anim)
    {
            // place your collision callback here:

            /***
            const core::vector3df pos = anim.getCollisionResultPosition();

            const core::triangle3df tri = anim.getCollisionTriangle();

            core::vector3df normal = tri.getNormal().normalize();
            core::vector3df vect(0.0f, 1.0f, 0.0f);


            core::quaternion q;
            q.rotationFromTo(vect, normal);

            scene::ISceneNode* node = anim.getTargetNode();
            if (!node)
                    return false;

            core::vector3df eulerRadians;
            q.toEuler(eulerRadians);

            core::vector3df rot = eulerRadians * core::RADTODEG;
            float angle
            = acos(fabs(vect.dotProduct(normal))) * core::RADTODEG;
            if (angle >= 30.0f)
                    printf("crashed!\n");
            else
            {
                    printf("chopper landed\n");
            }
            ***/
            return false;
    }


    void ChopperControl::ascent()
    {

    }

    void ChopperControl::descent()
    {

    }

    void ChopperControl::hover()
    {
            IsResettingHover = true;
            ResetHoverStartTime = Device->getTimer()->getTime();
            OldPower = Power;
            //Power = HOVER_POWER;
    }

    void ChopperControl::pitchDown()
    {
            if (IsResettingPitch)
                    IsResettingPitch = false;
            this->pitch(-1.99f);
    }

    void ChopperControl::pitchUp()
    {
            if (IsResettingPitch)
                    IsResettingPitch = false;
            this->pitch(1.99f);
    }

    void ChopperControl::slowDown()
    {

    }

    void ChopperControl::speedUp()
    {

    }

    void ChopperControl::turnLeft()
    {
            this->yaw(-1.99f);
    }

    void ChopperControl::turnRight()
    {
            this->yaw(1.99f);
    }

    void ChopperControl::rollLeft()
    {
            if (IsResettingRoll)
                    IsResettingRoll = false;
            this->roll(1.99f);
    }

    void ChopperControl::rollRight()
    {
            if (IsResettingRoll)
                    IsResettingRoll = false;
            this->roll(-1.99f);
    }

    void ChopperControl::resetRoll()
    {
            IsResettingRoll = true;
            ResetRollStartTime = Device->getTimer()->getTime();
            OldRollAngleDegs = clampAngleDegs(RollEmpty->getRotation().Z);
    }

    void ChopperControl::resetPitch()
    {
            IsResettingPitch = true;
            ResetPitchStartTime = Device->getTimer()->getTime();
            OldPitchAngleDegs = clampAngleDegs(PitchEmpty->getRotation().X);
    }

    void ChopperControl::resetRotation()
    {
            Node->setRotation(core::vector3df(0.0f));
            RootEmpty->setRotation(core::vector3df(0.0f));
            YawEmpty->setRotation(core::vector3df(0.0f));
            PitchEmpty->setRotation(core::vector3df(0.0f));
            RollEmpty->setRotation(core::vector3df(0.0f));

    }


    void ChopperControl::showDebugInfo()
    {
            f32 x = PitchEmpty->getRotation().X;
            f32 z = RollEmpty->getRotation().Z;
            printf("PitchRotX=%f, RollRotZ=%f\n", x, z);
    }


    ////////////////////////////////////////////////////////////////////////////////
    void ChopperControl::update()
    {
            u32 time = Device->getTimer()->getTime();
            f32 deltaTime = f32(time - LastTime) * 0.001f;

            if (IsResettingRoll)
            {
                    f32 t = (time - ResetRollStartTime) * 0.001f;
                    f32 interpolate = 1.0f - exp(- RATE_CONSTANT * t);
                    //printf("interpolate=%f\n", interpolate);

                    f32 rollAngleDegs = (1.0f - interpolate) * OldRollAngleDegs;
                    RollEmpty->setRotation(core::vector3df(0.0f, 0.0f, rollAngleDegs));

                    if (interpolate > STOPPING_INTERPOLATE)
                    {
                            RollEmpty->setRotation(core::vector3df(0.0f));
                            IsResettingRoll = false;
                    }
            }

            if (IsResettingPitch)
            {
                    f32 t = (time - ResetPitchStartTime) * 0.001f;
                    f32 interpolate = 1.0f - exp(- RATE_CONSTANT * t);

                    f32 pitchAngleDegs = (1.0f - interpolate) * OldPitchAngleDegs;
                    PitchEmpty->setRotation(core::vector3df(pitchAngleDegs, 0.0f, 0.0f));

                    if (interpolate > STOPPING_INTERPOLATE)
                    {
                            PitchEmpty->setRotation(core::vector3df(0.0f));
                            IsResettingPitch = false;
                    }
            }

            if (IsResettingHover)
            {
                    f32 t = (time - ResetHoverStartTime) * 0.001f;
                    f32 interpolate = 1.0f - exp(- RATE_CONSTANT * t);

                    Power = (1.0f - interpolate) * (OldPower - HOVER_POWER) + HOVER_POWER;

                    if (interpolate > STOPPING_INTERPOLATE)
                    {
                            Power = HOVER_POWER;
                            IsResettingHover = false;
                    }
            }

            f32 pitchEmptyRotX = PitchEmpty->getRotation().X;
            f32 z = SIDEWAY_SPEED_CONST * sin(pitchEmptyRotX * core::DEGTORAD) * Power * deltaTime;
            this->moveInLocalZDir(z);

            f32 rollEmptyRotZ = RollEmpty->getRotation().Z;
            f32 x = - SIDEWAY_SPEED_CONST * sin(rollEmptyRotZ * core::DEGTORAD) * Power * deltaTime;
            this->moveInLocalXDir(x);

            // ascend:
            this->ascend(deltaTime);

            LastTime = time;
    }

    ////////////////////////////////////////////////////////////////////////////////
    void ChopperControl::moveInLocalXDir(f32 distance)
    {
            updateAbsolutePosition();
            core::matrix4 matrix = YawEmpty->getAbsoluteTransformation();
            core::vector3df left(1.0f, 0.0f, 0.0f);
            matrix.rotateVect(left);

            core::vector3df pos = RootEmpty->getAbsolutePosition() + left * distance;
            RootEmpty->setPosition(pos);

    }


    void ChopperControl::ChopperControl::moveInLocalZDir(f32 distance)
    {
            updateAbsolutePosition();
            core::matrix4 matrix = YawEmpty->getAbsoluteTransformation();
            core::vector3df heading(0.0f, 0.0f, 1.0f);
            matrix.rotateVect(heading);

            core::vector3df pos = RootEmpty->getAbsolutePosition() + heading * distance;
            RootEmpty->setPosition(pos);

    }



    void ChopperControl::move(f32 distance)
    {

            updateAbsolutePosition();
            core::matrix4 matrix = RollEmpty->getAbsoluteTransformation();
            core::vector3df vect(0.0f, 0.0f, 1.0f);
            matrix.rotateVect(vect);
            vect.setLength(distance);

            core::vector3df pos = RootEmpty->getPosition() + vect;
            RootEmpty->setPosition(pos);
    }

    void ChopperControl::ascend(f32 deltaTime)
    {
            if (core::iszero(Power - HOVER_POWER))
                    return;
            f32 dy = (Power - HOVER_POWER) * deltaTime * cos(this->getVerticalAngleRads()) * ASCEND_SPEED_CONST;

            updateAbsolutePosition();
            core::vector3df pos = RootEmpty->getAbsolutePosition();
            pos.Y += dy;
            RootEmpty->setPosition(pos);
    }


    void ChopperControl::rotate(scene::ISceneNode* node, const core::vector3df& rot)
    {
            core::matrix4 m;
            m.setRotationDegrees(node->getRotation());

            core::matrix4 n;
            n.setRotationDegrees(rot);
            m *= n;

            node->setRotation(m.getRotationDegrees());
    }


    void ChopperControl::yaw(f32 degs)
    {
            rotate(YawEmpty, core::vector3df(0.0f, degs, 0.0f));
            //printf("YawEmpty->getRotation().Y=%f\n", YawEmpty->getRotation().Y);
    }


    void ChopperControl::pitch(f32 degs)
    {
            f32 z = RollEmpty->getRotation().Z;
            if ((z>90.0f) && (z<270.0f))
                    rotate(PitchEmpty, core::vector3df(degs, 0.0f, 0.0f));
            else
                    rotate(PitchEmpty, core::vector3df(-degs, 0.0f, 0.0f));
            //printf("PitchEmpty->getRotation().X=%f\n", PitchEmpty->getRotation().X);
    }


    void ChopperControl::roll(f32 degs)
    {
            rotate(RollEmpty, core::vector3df(0.0f, 0.0f, degs));
            //printf("RollEmpty->getRotation().Z=%f\n", RollEmpty->getRotation().Z);

    }


    void ChopperControl::throttle()
    {
            if (IsResettingHover)
                    IsResettingHover = false;

            Power += ACCELERATION;
    }


    void ChopperControl::brake()
    {
            if (IsResettingHover)
                    IsResettingHover = false;

            Power -= DECELERATION;
    }

    void ChopperControl::stop()
    {
            Power = 0.0f;
    }



    core::vector3df ChopperControl::getVelocity()
    {
            core::matrix4 matrix = RollEmpty->getAbsoluteTransformation();
            core::vector3df heading(0.0f, 0.0f, 1.0f);
            matrix.rotateVect(heading);

            return Speed * heading;
    }


    void ChopperControl::updateAbsolutePosition()
    {
            Node->updateAbsolutePosition();
            RootEmpty->updateAbsolutePosition();
            YawEmpty->updateAbsolutePosition();
            PitchEmpty->updateAbsolutePosition();
            RollEmpty->updateAbsolutePosition();
    }


    f32 ChopperControl::getRollAngleDegs() const
    {
            return RollEmpty->getRotation().Z;
    }


    f32 ChopperControl::getVerticalAngleRads() const
    {
            core::vector3df yaxis(0.0f, 1.0f, 0.0f);

            core::vector3df normal(0.0f, 1.0f, 0.0f);
            core::matrix4 matrix = Node->getAbsoluteTransformation();
            matrix.rotateVect(normal);
            normal.normalize();

            //return acos(yaxis.dotProduct(normal)) * core::RADTODEG;
            return acos(yaxis.dotProduct(normal));
    }


    bool ChopperControl::isInverted()
    {
            core::vector3df yaxis(0.0f, 1.0f, 0.0f);
            core::matrix4 matrix = Node->getAbsoluteTransformation();
            matrix.rotateVect(yaxis);
                    return (yaxis.Y < 0.0f);
    }


    f32 ChopperControl::clampAngleDegs(f32 degs)
    {
            if ((degs >= -180.0f) && (degs <= 180.0f))
                    return degs;

            while (degs > 180.0f)
                    degs -= 360.0f;

            while (degs < -180.0f)
                    degs += 360.0f;

            return degs;
    }

    void ChopperControl::drawLandingGuide(scene::ITriangleSelector* selector, video::SColor color)
    {
            // cmgr:
            scene::ISceneCollisionManager* cmgr = Smgr->getSceneCollisionManager();

            core::line3d<f32> ray;
            ray.start = this->getRootEmpty()->getAbsolutePosition();
            core::vector3df dir(0.0f, -1.0f, 0.0f);
            ray.end = ray.start + (dir * 10000.0f);

            core::vector3df intersection;
            core::triangle3df hitTriangle;

            scene::ISceneNode* outNode;


            bool ok = cmgr->getCollisionPoint
            (
                    ray, selector, intersection, hitTriangle, outNode
            );

            if (!ok) // nothing was hit
                    return;

            // driver:
            video::IVideoDriver* driver = Smgr->getVideoDriver();
            driver->setTransform(video::ETS_WORLD, core::matrix4());

            // set mat:
            video::SMaterial lineMaterial;
            lineMaterial.Lighting = false;
            lineMaterial.Thickness = 2.0f;
            lineMaterial.FrontfaceCulling = false;
            lineMaterial.BackfaceCulling = false;
            lineMaterial.ZBuffer = video::ECFN_NEVER;

            driver->setMaterial(lineMaterial);

            // draw vertical line:
            driver->draw3DLine(intersection, ray.start, color);

            // draw cross:
            float r = 10.0f;
            float ix = intersection.X;
            float iy = intersection.Y;
            float iz = intersection.Z;
            driver->draw3DLine(core::vector3df(ix+r,iy,iz+r), core::vector3df(ix-r,iy,iz-r), color);
            driver->draw3DLine(core::vector3df(ix+r,iy,iz-r), core::vector3df(ix-r,iy,iz+r), color);
    }


