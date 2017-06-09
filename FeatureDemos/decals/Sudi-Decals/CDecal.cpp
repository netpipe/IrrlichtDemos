/*
  Copyright (C) 2011 Daniel Sudmann

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Daniel Sudmann suddani@googlemail.com
*/
#include "CDecal.h"
#include <CMeshBuffer.h>
#include <IVideoDriver.h>
#include <line2d.h>
#include <set>

typedef irr::core::array<irr::core::vector3df>      VectorArray;
typedef irr::core::array<irr::core::triangle3df>    TriangleArray;

irr::core::triangle3df transformTriangle(const irr::core::triangle3df& triangle, const irr::core::matrix4& mat);

template<class T>
void append(irr::core::array<T>& target, const irr::core::array<T>& source)
{
    for (irr::u32 i=0;i<source.size();++i)
        target.push_back(source[i]);
}

CDecal::CDecal(const irr::core::vector3df& pos, const irr::core::vector3df& normal, const irr::f32& size, irr::video::IVideoDriver* driver)
{
    //ctor
    Driver = driver;

    updateTransform(pos, -normal, size);
}

CDecal::CDecal(irr::video::IVideoDriver* driver)
{
    //ctor
    Driver = driver;
}

CDecal::~CDecal()
{
    //dtor
}

void CDecal::drawTripod(void)
{
    Driver->setTransform(irr::video::ETS_WORLD, irr::core::IdentityMatrix);
    irr::video::SMaterial material;
    material.Lighting = false;
    Driver->setTransform(irr::video::ETS_WORLD, irr::core::IdentityMatrix);

    material.DiffuseColor = irr::video::SColor(255,255,0,0);
    Driver->setMaterial(material);
    Driver->draw3DLine(Position, Position+X*10.f, irr::video::SColor(255,255,0,0));

    material.DiffuseColor = irr::video::SColor(255,0,255,0);
    Driver->setMaterial(material);
    Driver->draw3DLine(Position, Position+Y*10.f, irr::video::SColor(255,0,255,0));

    material.DiffuseColor = irr::video::SColor(255,0,255,255);
    Driver->setMaterial(material);
    Driver->draw3DLine(Position, Position+Z*10.f), irr::video::SColor(255,0,255,255);
}

void CDecal::updateTransform(const irr::core::vector3df& pos, const irr::core::vector3df& view, const irr::f32& size)
{
    Z = view;
    Z.normalize();
    irr::core::matrix4 mat = irr::core::IdentityMatrix;
    mat.setRotationDegrees(Z.getHorizontalAngle());
    X = irr::core::vector3df(1,0,0);
    mat.rotateVect(X);
    X.normalize();
    Y = irr::core::vector3df(0,1,0);
    mat.rotateVect(Y);
    Y.normalize();
    Size = size;
    Position = pos;


    Transform[0]    = X.X;
    Transform[1]    = X.Y;
    Transform[2]    = X.Z;
    Transform[3]    = 0;

    Transform[4]    = Y.X;
    Transform[5]    = Y.Y;
    Transform[6]    = Y.Z;
    Transform[7]    = 0;

    Transform[8]    = Z.X;
    Transform[9]    = Z.Y;
    Transform[10]   = Z.Z;
    Transform[11]   = 0;

    Transform[12]   = Position.X;
    Transform[13]   = Position.Y;
    Transform[14]   = Position.Z;
    Transform[15]   = 1;

    Transform.makeInverse();
    Projection.buildProjectionMatrixOrthoLH(Size, Size, -Size/2.f, Size/2.f);

    TransformTotal = Projection*Transform;
    TransformTotalInvers = TransformTotal;
    TransformTotalInvers.makeInverse();
}

irr::scene::IMeshBuffer* CDecal::createBuffer(const irr::core::triangle3df* triangles, const irr::u32& count)
{
    irr::scene::SMeshBuffer* buffer = new irr::scene::SMeshBuffer();

    printf("Call func: %i\n", count);
    irr::u32 pCount = 0;

    irr::core::aabbox3df box(Position);
    box.addInternalPoint(Position+irr::core::vector3df(Size/2.f));
    box.addInternalPoint(Position-irr::core::vector3df(Size/2.f));

    irr::core::array<irr::u32> Tris;
    std::set<irr::core::vector3df> Normals;
    irr::core::vector3df Normal(0.f);
    for (irr::u32 i=0; i<count; ++i)
    {
        irr::core::vector3df normal = triangles[i].getNormal().normalize();
        if (triangles[i].isFrontFacing(Z) && !triangles[i].isTotalOutsideBox(box))
        {
            Tris.push_back(i);
            Normals.insert(normal);
            pCount++;
        }
    }

    std::set<irr::core::vector3df>::iterator ntor = Normals.begin();
    irr::u32 nCount = 0;
    while (ntor != Normals.end())
    {
        Normal += *ntor;
        nCount++;
        ++ntor;
    }
    Normal /= (irr::f32)(nCount);
    printf("Normal Count: %i\n", nCount);

    updateTransform(Position, -Normal, Size);

    for (irr::u32 i=0; i<pCount; ++i)
    {
        processTriangle(transformTriangle(triangles[Tris[i]], TransformTotal), buffer);
    }

    printf("Triangles to process %i\n", pCount);
    printf("--------------------------------------------------------------------------------\n", pCount);
    return buffer;
}

irr::core::plane3df LeftPlane   (-1, 0, 0, 1, 0, 0);
irr::core::plane3df UpPlane     ( 0, 1, 0, 0,-1, 0);
irr::core::plane3df RightPlane  ( 1, 0, 0, 1, 0, 0);
irr::core::plane3df DownPlane   ( 0,-1, 0, 0, 1, 0);

bool isViewPortInsideTriangle(const irr::core::triangle3df& triangle)
{
    irr::core::vector3df forward(0,0,1);
    irr::core::vector3df point;
    if (!triangle.getIntersectionWithLine(irr::core::vector3df(-1,-1,0), forward, point))
        return false;
    if (!triangle.getIntersectionWithLine(irr::core::vector3df(-1,1,0), forward, point))
        return false;
    if (!triangle.getIntersectionWithLine(irr::core::vector3df(1,1,0), forward, point))
        return false;
    if (!triangle.getIntersectionWithLine(irr::core::vector3df(1,-1,0), forward, point))
        return false;
    return true;
}

bool isPointInsideViewPort(const irr::core::vector3df& point)
{
    if (point.X > 1.1f || point.X < -1.1f)
        return false;

    if (point.Y > 1.1f || point.Y < -1.1f)
        return false;

    return true;
}

bool isTriangleInsideViewPort(const irr::core::triangle3df& triangle)
{
    if (!isPointInsideViewPort(triangle.pointA))
        return false;
    if (!isPointInsideViewPort(triangle.pointB))
        return false;
    if (!isPointInsideViewPort(triangle.pointC))
        return false;
    return true;
}

bool doViewportAndTriangleIntersect(const irr::core::triangle3df& triangleProj)
{
    irr::core::vector3df point;

    ///Lower left Corner
    if (LeftPlane.getIntersectionWithLimitedLine(triangleProj.pointA, triangleProj.pointB, point))
        if (isPointInsideViewPort(point))
            return true;
    if (LeftPlane.getIntersectionWithLimitedLine(triangleProj.pointA, triangleProj.pointC, point))
        if (isPointInsideViewPort(point))
            return true;
    if (LeftPlane.getIntersectionWithLimitedLine(triangleProj.pointC, triangleProj.pointB, point))
        if (isPointInsideViewPort(point))
            return true;

    ///Lower left Corner
    if (UpPlane.getIntersectionWithLimitedLine(triangleProj.pointA, triangleProj.pointB, point))
        if (isPointInsideViewPort(point))
            return true;
    if (UpPlane.getIntersectionWithLimitedLine(triangleProj.pointA, triangleProj.pointC, point))
        if (isPointInsideViewPort(point))
            return true;
    if (UpPlane.getIntersectionWithLimitedLine(triangleProj.pointC, triangleProj.pointB, point))
        if (isPointInsideViewPort(point))
            return true;

    ///Right upper corner
    if (RightPlane.getIntersectionWithLimitedLine(triangleProj.pointA, triangleProj.pointB, point))
        if (isPointInsideViewPort(point))
            return true;
    if (RightPlane.getIntersectionWithLimitedLine(triangleProj.pointA, triangleProj.pointC, point))
        if (isPointInsideViewPort(point))
            return true;
    if (RightPlane.getIntersectionWithLimitedLine(triangleProj.pointC, triangleProj.pointB, point))
        if (isPointInsideViewPort(point))
            return true;

    ///Right lower corner
    if (DownPlane.getIntersectionWithLimitedLine(triangleProj.pointA, triangleProj.pointB, point))
        if (isPointInsideViewPort(point))
            return true;
    if (DownPlane.getIntersectionWithLimitedLine(triangleProj.pointA, triangleProj.pointC, point))
        if (isPointInsideViewPort(point))
            return true;
    if (DownPlane.getIntersectionWithLimitedLine(triangleProj.pointC, triangleProj.pointB, point))
        if (isPointInsideViewPort(point))
            return true;
    return false;
}

irr::core::triangle3df transformTriangle(const irr::core::triangle3df& triangle, const irr::core::matrix4& mat)
{
    irr::core::triangle3df out = triangle;
    mat.transformVect(out.pointA);
    mat.transformVect(out.pointB);
    mat.transformVect(out.pointC);
    return out;
}

void insertByDistance(VectorArray& points, const irr::core::vector3df& point, const irr::core::vector3df& origin)
{
    for (irr::u32 i=0; i<points.size(); ++i)
    {
        if (points[i].getDistanceFromSQ(origin) > point.getDistanceFromSQ(origin))
        {
            points.insert(point, i);
            return;
        }
    }
    points.push_back(point);
}

void CDecal::processTriangles(const TriangleArray& triangles, irr::scene::IMeshBuffer* b)
{
    printf("Process %i generated Triangles\n", triangles.size());
    for (irr::u32 i=0;i<triangles.size();++i)
        processTriangle(triangles[i], b);
}

void getTriangleViewPortRelation(const irr::core::triangle3df& triangleProj, VectorArray& inside, VectorArray& outside)
{
    if (isPointInsideViewPort(triangleProj.pointA))
        inside.push_back(triangleProj.pointA);
    else
        outside.push_back(triangleProj.pointA);

    if (isPointInsideViewPort(triangleProj.pointB))
        inside.push_back(triangleProj.pointB);
    else
        outside.push_back(triangleProj.pointB);

    if (isPointInsideViewPort(triangleProj.pointC))
        inside.push_back(triangleProj.pointC);
    else
        outside.push_back(triangleProj.pointC);
}

void makeTriangleFaceFront(irr::core::triangle3df& triangleProj)
{
    if (triangleProj.isFrontFacing(irr::core::vector3df(0,0,1)))
        return;
    irr::core::vector3df tmp = triangleProj.pointC;
    triangleProj.pointC = triangleProj.pointB;
    triangleProj.pointB = tmp;
}

VectorArray getIntersections(const irr::core::triangle3df& triangleProj)
{
    irr::core::vector3df point;
    VectorArray vecs;
    VectorArray vecsTmp;
    if (LeftPlane.getIntersectionWithLimitedLine(triangleProj.pointA, triangleProj.pointB, point))
    {
        if (isPointInsideViewPort(point))
        {
            insertByDistance(vecsTmp, point, irr::core::vector3df(-1,-1,0));
        }
    }
    if (LeftPlane.getIntersectionWithLimitedLine(triangleProj.pointB, triangleProj.pointC, point))
    {
        if (isPointInsideViewPort(point))
        {
            insertByDistance(vecsTmp, point, irr::core::vector3df(-1,-1,0));
        }
    }
    if (LeftPlane.getIntersectionWithLimitedLine(triangleProj.pointC, triangleProj.pointA, point))
    {
        if (isPointInsideViewPort(point))
        {
            insertByDistance(vecsTmp, point, irr::core::vector3df(-1,-1,0));
        }
    }
    if (triangleProj.getIntersectionWithLine(irr::core::vector3df(-1,1,0), irr::core::vector3df(0,0,1), point))
        vecsTmp.push_back(point);
    append(vecs, vecsTmp);
    vecsTmp.clear();

    if (UpPlane.getIntersectionWithLimitedLine(triangleProj.pointA, triangleProj.pointB, point))
    {
        if (isPointInsideViewPort(point))
        {
            insertByDistance(vecsTmp, point, irr::core::vector3df(-1,1,0));
        }
    }
    if (UpPlane.getIntersectionWithLimitedLine(triangleProj.pointB, triangleProj.pointC, point))
    {
        if (isPointInsideViewPort(point))
        {
            insertByDistance(vecsTmp, point, irr::core::vector3df(-1,1,0));
        }
    }
    if (UpPlane.getIntersectionWithLimitedLine(triangleProj.pointC, triangleProj.pointA, point))
    {
        if (isPointInsideViewPort(point))
        {
            insertByDistance(vecsTmp, point, irr::core::vector3df(-1,1,0));
        }
    }
    if (triangleProj.getIntersectionWithLine(irr::core::vector3df(1,1,0), irr::core::vector3df(0,0,1), point))
        vecsTmp.push_back(point);
    append(vecs, vecsTmp);
    vecsTmp.clear();

    if (RightPlane.getIntersectionWithLimitedLine(triangleProj.pointA, triangleProj.pointB, point))
    {
        if (isPointInsideViewPort(point))
        {
            insertByDistance(vecsTmp, point, irr::core::vector3df(1,1,0));
        }
    }
    if (RightPlane.getIntersectionWithLimitedLine(triangleProj.pointB, triangleProj.pointC, point))
    {
        if (isPointInsideViewPort(point))
        {
            insertByDistance(vecsTmp, point, irr::core::vector3df(1,1,0));
        }
    }
    if (RightPlane.getIntersectionWithLimitedLine(triangleProj.pointC, triangleProj.pointA, point))
    {
        if (isPointInsideViewPort(point))
        {
            insertByDistance(vecsTmp, point, irr::core::vector3df(1,1,0));
        }
    }
    if (triangleProj.getIntersectionWithLine(irr::core::vector3df(1,-1,0), irr::core::vector3df(0,0,1), point))
        vecsTmp.push_back(point);
    append(vecs, vecsTmp);
    vecsTmp.clear();

    if (DownPlane.getIntersectionWithLimitedLine(triangleProj.pointA, triangleProj.pointB, point))
    {
        if (isPointInsideViewPort(point))
        {
            insertByDistance(vecsTmp, point, irr::core::vector3df(1,-1,0));
        }
    }
    if (DownPlane.getIntersectionWithLimitedLine(triangleProj.pointB, triangleProj.pointC, point))
    {
        if (isPointInsideViewPort(point))
        {
            insertByDistance(vecsTmp, point, irr::core::vector3df(1,-1,0));
        }
    }
    if (DownPlane.getIntersectionWithLimitedLine(triangleProj.pointC, triangleProj.pointA, point))
    {
        if (isPointInsideViewPort(point))
        {
            insertByDistance(vecsTmp, point, irr::core::vector3df(1,-1,0));
        }
    }
    if (triangleProj.getIntersectionWithLine(irr::core::vector3df(-1,-1,0), irr::core::vector3df(0,0,1), point))
        vecsTmp.push_back(point);
    append(vecs, vecsTmp);
    vecsTmp.clear();
    return vecs;
}

void CDecal::processTriangle(const irr::core::triangle3df& triangleProj, irr::scene::IMeshBuffer* b)
{
    irr::scene::SMeshBuffer* buffer = (irr::scene::SMeshBuffer*)b;

    irr::core::triangle3df triangle = transformTriangle(triangleProj, TransformTotalInvers);

    if (isTriangleInsideViewPort(triangleProj))
    {
        printf("Triangle fits into the viewport\n");
        addTriangle(triangleProj, triangle, buffer);
    }
    else if (isViewPortInsideTriangle(triangleProj))
    {
        printf("Viewport fits into the triangle\n");
        irr::core::triangle3df quad;
        triangleProj.getIntersectionWithLine(irr::core::vector3df(-1,-1,0), irr::core::vector3df(0,0,1), quad.pointA);
        triangleProj.getIntersectionWithLine(irr::core::vector3df(-1,1,0), irr::core::vector3df(0,0,1), quad.pointB);
        triangleProj.getIntersectionWithLine(irr::core::vector3df(1,1,0), irr::core::vector3df(0,0,1), quad.pointC);
        addTriangle(quad, transformTriangle(quad, TransformTotalInvers), buffer);
        triangleProj.getIntersectionWithLine(irr::core::vector3df(-1,-1,0), irr::core::vector3df(0,0,1), quad.pointA);
        triangleProj.getIntersectionWithLine(irr::core::vector3df(1,1,0), irr::core::vector3df(0,0,1), quad.pointB);
        triangleProj.getIntersectionWithLine(irr::core::vector3df(1,-1,0), irr::core::vector3df(0,0,1), quad.pointC);
        addTriangle(quad, transformTriangle(quad, TransformTotalInvers), buffer);
    }
    else if (doViewportAndTriangleIntersect(triangleProj))
    {
        printf("Special case have to calc all triangles\n");
        VectorArray inside;
        VectorArray outside;
        getTriangleViewPortRelation(triangleProj, inside, outside);
        if (inside.size() == 1)
        {
            printf("1 vertix inside\n");
            VectorArray data = getIntersections(triangleProj);
            if (data.size() >= 2)
            {
                irr::core::triangle3df tmp;
                tmp.pointA = inside[0];
                tmp.pointB = data[0];
                data.erase(0);
                for (irr::u32 i=0;i<data.size();++i)
                {
                    tmp.pointC = data[i];
                    irr::core::triangle3df tt = tmp;
                    makeTriangleFaceFront(tt);
                    addTriangle(tt, transformTriangle(tt, TransformTotalInvers), b);
                    tmp.pointB = tmp.pointC;
                }
            }
        }
        if (inside.size() == 2)
        {
            printf("2 vertices inside\n");
            VectorArray data = getIntersections(triangleProj);
            if (inside[0].Y > inside[1].Y && data[0].X > inside[0].X && data[1].X > inside[0].X)
            {
                irr::core::triangle3df tmp;
                tmp.pointA = inside[0];
                tmp.pointB = data[0];
                data.erase(0);
                for (irr::u32 i=0;i<data.size();++i)
                {
                    tmp.pointC = data[i];
                    irr::core::triangle3df tt = tmp;
                    makeTriangleFaceFront(tt);
                    addTriangle(tt, transformTriangle(tt, TransformTotalInvers), b);
                    tmp.pointB = tmp.pointC;
                }
                tmp.pointC = inside[1];
                addTriangle(tmp, transformTriangle(tmp, TransformTotalInvers), b);
            }
            else
            {
                irr::core::triangle3df tmp;
                tmp.pointA = inside[0];
                tmp.pointB = inside[1];
                for (irr::u32 i=0;i<data.size();++i)
                {
                    tmp.pointC = data[i];
                    irr::core::triangle3df tt = tmp;
                    makeTriangleFaceFront(tt);
                    addTriangle(tt, transformTriangle(tt, TransformTotalInvers), b);
                    tmp.pointB = tmp.pointC;
                }
            }
        }
        else
        {
            printf("No vertices inside\n");
            VectorArray data = getIntersections(triangleProj);
            if (data.size() >= 3)
            {
                irr::core::triangle3df tmp;
                tmp.pointA = data[0];
                tmp.pointB = data[1];
                data.erase(0);
                for (irr::u32 i=0;i<data.size();++i)
                {
                    tmp.pointC = data[i];
                    irr::core::triangle3df tt = tmp;
                    makeTriangleFaceFront(tt);
                    addTriangle(tt, transformTriangle(tt, TransformTotalInvers), b);
                    tmp.pointB = tmp.pointC;
                }
            }
        }
    }
}

void CDecal::addTriangle(const irr::core::triangle3df& triangleProj, const irr::core::triangle3df& triangle, irr::scene::IMeshBuffer* b)
{
    printf("Add triangle\n");
    irr::scene::SMeshBuffer* buffer = (irr::scene::SMeshBuffer*)b;

    irr::core::vector2df UV_Mod(1.f);

    irr::core::vector3df normal = triangle.getNormal().normalize();

    buffer->Vertices.push_back(irr::video::S3DVertex(triangle.pointA+normal*0.01f, normal, irr::video::SColor(255,255,255,255), (irr::core::vector2df(triangleProj.pointA.X, triangleProj.pointA.Y)+UV_Mod)/2.f));//irr::core::vector2df(triangle.pointA.X/triangle.pointA.Z, triangle.pointA.Y/triangle.pointA.Z)));
    buffer->Indices.push_back(buffer->Vertices.size()-1);

    buffer->Vertices.push_back(irr::video::S3DVertex(triangle.pointB+normal*0.01f, normal, irr::video::SColor(255,255,255,255), (irr::core::vector2df(triangleProj.pointB.X, triangleProj.pointB.Y)+UV_Mod)/2.f));//irr::core::vector2df(triangle.pointB.X/triangle.pointB.Z, triangle.pointB.Y/triangle.pointB.Z)));
    buffer->Indices.push_back(buffer->Vertices.size()-1);

    buffer->Vertices.push_back(irr::video::S3DVertex(triangle.pointC+normal*0.01f, normal, irr::video::SColor(255,255,255,255), (irr::core::vector2df(triangleProj.pointC.X, triangleProj.pointC.Y)+UV_Mod)/2.f));//irr::core::vector2df(triangle.pointC.X/triangle.pointC.Z, triangle.pointC.Y/triangle.pointC.Z)));
    buffer->Indices.push_back(buffer->Vertices.size()-1);
}
