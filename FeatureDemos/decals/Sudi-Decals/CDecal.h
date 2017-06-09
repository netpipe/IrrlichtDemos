#ifndef CDECAL_H
#define CDECAL_H

#include <vector3d.h>
#include <matrix4.h>
#include <triangle3d.h>
#include <irrArray.h>
namespace irr
{
    namespace scene
    {
        class IMeshBuffer;
    }
    namespace video
    {
        class IVideoDriver;
    }
}

class CDecal
{
    public:
        /** Default constructor */
        CDecal(const irr::core::vector3df& pos, const irr::core::vector3df& normal, const irr::f32& size, irr::video::IVideoDriver* driver);
        CDecal(irr::video::IVideoDriver* driver);
        /** Default destructor */
        virtual ~CDecal();

        irr::scene::IMeshBuffer* createBuffer(const irr::core::triangle3df* triangles, const irr::u32& count);

        void drawTripod(void);
        void updateTransform(const irr::core::vector3df& pos, const irr::core::vector3df& view, const irr::f32& size);
    protected:
        irr::core::vector3df X;
        irr::core::vector3df Y;
        irr::core::vector3df Z;
        irr::core::vector3df Position;
        irr::f32 Size;

        irr::core::matrix4 TransformTotal;
        irr::core::matrix4 TransformTotalInvers;
        irr::core::matrix4 Transform;
        irr::core::matrix4 Projection;

        irr::video::IVideoDriver* Driver;

        void processTriangle(const irr::core::triangle3df& triangle, irr::scene::IMeshBuffer* b);
        void processTriangles(const irr::core::array<irr::core::triangle3df>& triangles, irr::scene::IMeshBuffer* b);
        void addTriangle(const irr::core::triangle3df& triangleProj, const irr::core::triangle3df& triangle, irr::scene::IMeshBuffer* b);
        irr::core::array<irr::core::triangle3df> clipTriangle(const irr::core::triangle3df& triangle);
    private:
};

#endif // CDECAL_H
