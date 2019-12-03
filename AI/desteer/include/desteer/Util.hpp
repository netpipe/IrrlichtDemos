#include "Types.hpp"
#include "irrvector/matrix4.h"
class VectorTransformer
{
    public:
    static irr::core::vector3df VectorTransformWorldToLocal(irr::core::vector3df vec,
                                                            irr::core::vector3df forward,
                                                            irr::core::vector3df side,
                                                            irr::core::vector3df pos)
    {
        irr::core::matrix4 mat;
        mat.setRotation(forward,side);
        mat.setTranslation(-pos);

        mat.transformVect(vec);

        return vec;
    }

    static irr::core::vector3df VectorTransformLocalToWorld(irr::core::vector3df vec)
    {
        irr::core::vector3df x = irr::core::vector3df(1,0,0);
        irr::core::vector3df y = irr::core::vector3df(0,1,0);
        irr::core::vector3df z = irr::core::vector3df(0,0,1);
        return irr::core::vector3df(vec.dotProduct(x),vec.dotProduct(y),vec.dotProduct(z));
    }
};
