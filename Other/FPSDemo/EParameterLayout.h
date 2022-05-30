#ifndef _EPARAMETERLAYOUT_H_
#define _EPARAMETERLAYOUT_H_

namespace agEngine
{
    namespace entities
    {
        /**********************************

            Parameter types for entities

        ***********************************/
        enum stringParams
        {
            nodeName = 0,
            modelFile = 1,
            scriptFile = 1,
        };

        enum intParams
        {
            physicsCollisionType,
            physicsCollideWith,
        };

        enum floatParams
        {
            currentAnimationFrame,
            xPosition,
            yPosition,
            zPosition,
            xRotation,
            yRotation,
            zRotation,
        };

        enum boolParams
        {
            isAnimated,
            isActivatedOnTouch,
        };
    }
}

#endif
