bool Compute(IBoneSceneNode* boneNode, IBoneSceneNode* rootBoneNode, vector3df target)
{
    vector3df       rootPos, curEnd, targetVector, desiredEnd, curVector, crossResult, endPos = target;
    double          cosAngle,turnAngle;
    IBoneSceneNode* link;
    IBoneSceneNode* end;

    // start at the last link in the chain
    end = boneNode;
    link = (IBoneSceneNode*)end->getParent();
    int tries = 0;
    double norm = 2;

    int maxTries = 1000;
    double error = 0.1;
    double maxAngleChange = PI / 12;

    while (++tries < maxTries && norm > 0.1)
    {
        link->updateAbsolutePosition();
        end->updateAbsolutePosition();

        rootPos = link->getAbsolutePosition();
        curEnd = end->getAbsolutePosition();
        desiredEnd = endPos;
        norm = curEnd.getDistanceFromSQ(desiredEnd);

        //std::cout << norm << std::endl;

        // see if i'm already close enough
        if (norm > error)
        {
            // create the vector to the current effector pos
            curVector = curEnd - rootPos;
            // create the desired effector position vector
            targetVector = endPos - rootPos;

            // normalize the vectors (expensive, requires a sqrt)
            curVector.normalize();
            targetVector.normalize();

            // the dot product gives me the cosine of the desired angle
            cosAngle = curVector.dotProduct(targetVector);
            // if the dot product returns 1.0, i don't need to rotate as it is 0 degrees
            if (cosAngle < 0.9999999)
            {
                // use the cross product to check which way to rotate
                crossResult = curVector.crossProduct(targetVector);

                crossResult.normalize();
                turnAngle = acos(cosAngle);   // get the angle

                if(turnAngle < maxAngleChange)
                {
                    turnAngle = maxAngleChange;
                }

                quaternion rotation;
                rotation = rotation.fromAngleAxis(turnAngle, crossResult);
                rotation.normalize();
                vector3df euRotation;
                rotation.toEuler(euRotation);
                link->setRotation(link->getRotation()+vector3df(euRotation.Z, euRotation.X, euRotation.Y));

                //TODO: check DOF restrictions for link joint here.
            }

            if (link == rootBoneNode)
            {
                link = (IBoneSceneNode*)end->getParent();   // start of the chain, restart
            }
            else
            {
                link = (IBoneSceneNode*)link->getParent();
            }
        }
    }

    if (tries == maxTries)
    {
        return false;
    }

    return true;
}
