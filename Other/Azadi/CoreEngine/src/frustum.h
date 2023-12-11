#ifndef FRUSTUM_H
#define FRUSTUM_H

#include <cmath>
#include "vector.h"

#ifndef PIOVER180
#define PIOVER180 0.0174532925f
#endif

namespace Geometry
{
    struct Frustum
    {
        float focalLength;
        float aspect;
        float fov;
        float ndist;
        float fdist;

        Math::Vector3<float> nearPlane;
        Math::Vector3<float> farPlane;
        Math::Vector3<float> leftPlane;
        Math::Vector3<float> rightPlane;
        Math::Vector3<float> topPlane;
        Math::Vector3<float> bottomPlane;

        void update(const float &w, const float &h, const float &FOV, const float &n, const float &f)
        {
            aspect = w / h;
            fov = FOV;
            ndist = n; fdist = f;

            focalLength = (w / 2) / (tan((fov / 2) * PIOVER180));

            nearPlane = Math::Vector3<float>(0, 0, -1);
            farPlane = Math::Vector3<float>(0, 0, 1);

            float t1 = sqrt((focalLength * focalLength) + 1);
            float t2 = sqrt((focalLength * focalLength) + (aspect * aspect));

            leftPlane = Math::Vector3<float>(focalLength / t1, 0, -( 1 / t1));
            rightPlane = Math::Vector3<float>(-(focalLength / t1), 0, -(1 / t1));
            topPlane = Math::Vector3<float>(0, -(focalLength / t2), -(aspect / t2));
            bottomPlane = Math::Vector3<float>(0, focalLength / t2, -(aspect / t2));
        }

        void extractFrustum(const float projectionMatrix[16], const float modelviewMatrix[16])
        {
            float finalMatrix[16]={ projectionMatrix[0] * modelviewMatrix[0], projectionMatrix[1] * modelviewMatrix[1], projectionMatrix[2] * modelviewMatrix[2], projectionMatrix[3] * modelviewMatrix[3],
                                   projectionMatrix[4] * modelviewMatrix[4], projectionMatrix[5] * modelviewMatrix[5], projectionMatrix[6] * modelviewMatrix[6], projectionMatrix[7] * modelviewMatrix[7],
                                   projectionMatrix[8] * modelviewMatrix[8], projectionMatrix[9] * modelviewMatrix[9], projectionMatrix[10] * modelviewMatrix[10], projectionMatrix[11] * modelviewMatrix[11],
                                   projectionMatrix[12] * modelviewMatrix[12], projectionMatrix[13] * modelviewMatrix[13], projectionMatrix[14] * modelviewMatrix[14], projectionMatrix[15] * modelviewMatrix[15] };

            nearPlane = Math::Vector3<float>(finalMatrix[12] + finalMatrix[8], finalMatrix[13] + finalMatrix[9], finalMatrix[14] + finalMatrix[10]);
            farPlane = Math::Vector3<float>(finalMatrix[12] - finalMatrix[8], finalMatrix[13] - finalMatrix[9], finalMatrix[14] - finalMatrix[10]);
            leftPlane = Math::Vector3<float>(finalMatrix[12] + finalMatrix[0], finalMatrix[13] + finalMatrix[1], finalMatrix[14] + finalMatrix[2]);
            rightPlane = Math::Vector3<float>(finalMatrix[12] - finalMatrix[0], finalMatrix[13] - finalMatrix[1], finalMatrix[14] - finalMatrix[2]);
            topPlane = Math::Vector3<float>(finalMatrix[12] + finalMatrix[4], finalMatrix[13] + finalMatrix[5], finalMatrix[14] + finalMatrix[6]);
            bottomPlane = Math::Vector3<float>(finalMatrix[12] - finalMatrix[4], finalMatrix[13] - finalMatrix[5], finalMatrix[14] - finalMatrix[6]);
        }

        bool pointInFrustum(const float &x, const float &y, const float &z)
        {
            Math::Vector3<float> temp(x, y, z);
            if ((nearPlane.dot(temp)) <= 0)
                return false;
            else if ((farPlane.dot(temp)) <= 0)
                return false;
            else if ((leftPlane.dot(temp)) <= 0)
                return false;
            else if ((rightPlane.dot(temp)) <= 0)
                return false;
            else if ((topPlane.dot(temp)) <= 0)
                return false;
            else if ((bottomPlane.dot(temp)) <= 0)
                return false;

            return true;
        }

        bool sphereInFrustum(const float &x, const float &y, const float &z, const float &radius)
        {
            Math::Vector3<float> temp(x, y, z);
            if ((nearPlane.dot(temp)) <= -radius)
                return false;
            else if ((farPlane.dot(temp)) <= -radius)
                return false;
            else if ((leftPlane.dot(temp)) <= -radius)
                return false;
            else if ((rightPlane.dot(temp)) <= -radius)
                return false;
            else if ((topPlane.dot(temp)) <= -radius)
                return false;
            else if ((bottomPlane.dot(temp)) <= -radius)
                return false;

            return true;
        }

        bool cubeInFrustum(const float &bx, const float &by, const float &bz, const float &tx, const float &ty, const float &tz)
        {
            float radius = Math::Vector3<float>(tx, by, bz).dot(Math::Vector3<float>(1, 0, 0)) + Math::Vector3<float>(bx, ty, bz).dot(Math::Vector3<float>(1, 0, 0)) + Math::Vector3<float>(bz, by, tz).dot(Math::Vector3<float>(1, 0, 0));
            Math::Vector3<float> temp((tx - bx) / 2, (ty - by) / 2, (tz - bz) / 2);

            if ((nearPlane.dot(temp)) <= -radius)
                return false;
            else if ((farPlane.dot(temp)) <= -radius)
                return false;
            else if ((leftPlane.dot(temp)) <= -radius)
                return false;
            else if ((rightPlane.dot(temp)) <= -radius)
                return false;
            else if ((topPlane.dot(temp)) <= -radius)
                return false;
            else if ((bottomPlane.dot(temp)) <= -radius)
                return false;

            return true;
        }
    };

    extern Frustum vFrustum;
};

#endif
