#ifndef IEMITTERREGION_H
#define IEMITTERREGION_H

#include <IReferenceCounted.h>
#include <IMesh.h>
#include <CMeshBuffer.h>
namespace irr
{
    namespace scene
    {
    }
}


class IEmitterRegion : public irr::IReferenceCounted
{
public:
    IEmitterRegion() {}
    virtual ~IEmitterRegion() {}

    ///Get a Spawn point from the spawn region
    virtual irr::core::vector3df getPoint(const irr::core::matrix4& transform) = 0;
    ///Call this function to check if there is direction vector avaiable for the last spawnpoint
    virtual bool isForceField(void) const
    {
        return false;
    }
    ///Only returns usefull stuff after calling getPoint and isForceField() returns true
    virtual irr::core::vector3df getVector(void)
    {
        return irr::core::vector3df();
    }
};

class CPointRegion : public IEmitterRegion
{
public:
    ///Get a Spawn point from the spawn region
    virtual irr::core::vector3df getPoint(const irr::core::matrix4& transform)
    {
        return transform.getTranslation();
    }
};

class CBoxRegion : public IEmitterRegion
{
public:
    CBoxRegion(const irr::core::aabbox3df& box = irr::core::aabbox3df(-1,-1,-1,1,1,1))
    {
        Box = box;
    }

    virtual irr::core::vector3df getPoint(const irr::core::matrix4& transform)
    {
        irr::core::vector3df into = Box.MaxEdge - Box.MinEdge;
        into.X = 0;
        into.Y = 0;
        irr::core::vector3df left = Box.MaxEdge - Box.MinEdge;
        left.Z = 0;
        left.Y = 0;
        irr::core::vector3df up = Box.MaxEdge - Box.MinEdge;
        up.X = 0;
        up.Z = 0;

        transform.rotateVect(into);
        transform.rotateVect(left);
        transform.rotateVect(up);

        irr::core::vector3df start = Box.getCenter()-into*0.5f-left*0.5f-up*0.5f;

        irr::f32 r0 = ((rand()%100)+1)/100.f;
        irr::f32 r1 = ((rand()%100)+1)/100.f;
        irr::f32 r2 = ((rand()%100)+1)/100.f;

        //printf("Create location: %f, %f, %f\n", point.X, point.Y, point.Z);

        irr::core::vector3df point = start+into*r0+left*r1+up*r2;

        return transform.getTranslation()+point;
    }

    irr::core::aabbox3df Box;
};

class CRingRegion : public IEmitterRegion
{
public:
    CRingRegion(const irr::f32& radius)
    {
        Radius = radius;
    }
    virtual irr::core::vector3df getPoint(const irr::core::matrix4& transform)
    {
        irr::f32 r0 = ((irr::f32)(rand()%100))/100.f;
        irr::core::vector3df point(cos(2*irr::core::PI*r0)*Radius, 0, sin(2*irr::core::PI*r0)*Radius);
        transform.rotateVect(point);
        //printf("add: %f - %f, %f, %f\n", r0, point.X, point.Y, point.Z);
        return transform.getTranslation()+point;
    }
    irr::f32 Radius;
};

class CMeshRegion : public IEmitterRegion
{
public:
    CMeshRegion(irr::scene::IMesh* mesh, bool emitTriangle)
    {
        Mesh = mesh;
        EmitTriangle = emitTriangle;
    }
    virtual irr::core::vector3df getPoint(const irr::core::matrix4& transform)
    {
        if (Mesh->getMeshBufferCount() == 0)
            return irr::core::vector3df();
        ///pick buffer
        irr::scene::IMeshBuffer* buffer = Mesh->getMeshBuffer(rand()%Mesh->getMeshBufferCount());
        if (buffer && buffer->getIndexCount() > 0 && buffer->getVertexCount() > 0)
        {
            irr::core::vector3df point;
            if (EmitTriangle)
            {
                ///pick triangle
                irr::u32 i = (rand()%(buffer->getIndexCount()/3))*3;

                irr::core::vector3df p0 = buffer->getPosition(buffer->getIndices()[i]);
                irr::core::vector3df p1 = buffer->getPosition(buffer->getIndices()[i+1]);
                irr::core::vector3df p2 = buffer->getPosition(buffer->getIndices()[i+2]);
                transform.rotateVect(p0);
                transform.rotateVect(p1);
                transform.rotateVect(p2);

                irr::core::vector3df v1 = p1-p0;
                irr::core::vector3df v2 = p2-p0;

                irr::f32 r0 = ((irr::f32)(rand()%1000))/1000.f;
                irr::f32 r1 = ((irr::f32)(rand()%1000))/1000.f;

                point = p0+v1*r0+v2*r1;
            }
            else
            {
                point = buffer->getPosition(rand()%(buffer->getVertexCount()));
                transform.rotateVect(point);
            }
            return transform.getTranslation()+point;
        }
        return irr::core::vector3df();
    }
    irr::scene::IMesh* Mesh;
    bool EmitTriangle;
};

class CLineRegion : public IEmitterRegion
{
public:
    CLineRegion(const irr::core::vector3df& s, const irr::core::vector3df& e)
    {
        Start = s;
        End = e;
    }
    CLineRegion(const irr::f32& x, const irr::f32& y, const irr::f32& z, const irr::f32& a, const irr::f32& b, const irr::f32& c)
    {
        Start = irr::core::vector3df(x,y,z);
        End = irr::core::vector3df(a,b,c);
    }
    virtual irr::core::vector3df getPoint(const irr::core::matrix4& transform)
    {
        irr::f32 r0 = ((irr::f32)(rand()%100))/100.f;
        irr::core::vector3df vec = End-Start;
        irr::core::vector3df point = Start+vec*0.5f;
        transform.rotateVect(vec);
        point += vec*(r0-0.5f);
        //printf("add: %f - %f, %f, %f\n", r0, point.X, point.Y, point.Z);
        return transform.getTranslation()+point;
    }
    irr::core::vector3df Start;
    irr::core::vector3df End;
};

class CPlaneRegion : public IEmitterRegion
{
public:
    CPlaneRegion(const irr::core::plane3df& plane = irr::core::plane3df(0,0,0,0,1,0), irr::f32 width = 20, irr::f32 height = 20)
    {
        Plane = plane;
        Width = width;
        Height = height;
    }
    ///Get a Spawn point from the spawn region
    virtual irr::core::vector3df getPoint(const irr::core::matrix4& transform)
    {
        irr::core::vector3df normal;
        irr::core::vector3df forward(0,0,1);
        transform.rotateVect(normal, Plane.Normal);
        irr::core::vector3df left = normal.crossProduct(forward);
        left.normalize();
        irr::core::vector3df up = normal.crossProduct(left);
        up.normalize();

        irr::core::vector3df start = Plane.getMemberPoint()+left*Width*0.5f+up*Height*0.5f;
        irr::f32 r0 = ((rand()%100)+1)/100.f;
        irr::f32 r1 = ((rand()%100)+1)/100.f;
        irr::core::vector3df point   = Plane.getMemberPoint()-left*Width*r0-up*Height*r1;

        //printf("Create location: %f, %f, %f\n", point.X, point.Y, point.Z);

        return transform.getTranslation()+point;
    }
    irr::core::plane3df Plane;
    irr::f32 Width;
    irr::f32 Height;
};

#endif // IEMITTERREGION_H
