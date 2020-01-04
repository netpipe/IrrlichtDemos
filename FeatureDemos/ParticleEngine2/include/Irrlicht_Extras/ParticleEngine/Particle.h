#ifndef PARTICLE_H
#define PARTICLE_H

#include <vector2d.h>
#include <vector3d.h>
#include <IReferenceCounted.h>
#include <SColor.h>
namespace irr
{
    namespace scene
    {
        namespace particle
        {
            struct SParticleUV
            {
                SParticleUV(void)
                {
                    UV0 = irr::core::vector2df(0,0);
                    UV1 = irr::core::vector2df(1,0);
                    UV2 = irr::core::vector2df(0,1);
                    UV3 = irr::core::vector2df(1,1);
                }
                SParticleUV(const SParticleUV& uv)
                {
                    UV0 = uv.UV0;
                    UV1 = uv.UV1;
                    UV2 = uv.UV2;
                    UV3 = uv.UV3;
                }
                SParticleUV(const irr::core::vector2df& a, const irr::core::vector2df& b, const irr::core::vector2df& c, const irr::core::vector2df& d)
                {
                    UV0 = a;
                    UV1 = b;
                    UV2 = c;
                    UV3 = d;
                }
                SParticleUV& operator=(const SParticleUV& uv)
                {
                    UV0 = uv.UV0;
                    UV1 = uv.UV1;
                    UV2 = uv.UV2;
                    UV3 = uv.UV3;
                    return *this;
                }
                void turnRight(void)
                {
                    irr::core::vector2df temp0 = UV0;
                    irr::core::vector2df temp1 = UV1;
                    irr::core::vector2df temp2 = UV2;
                    irr::core::vector2df temp3 = UV3;
                    UV0 = temp2;
                    UV1 = temp0;
                    UV2 = temp3;
                    UV3 = temp1;

                }
                irr::core::vector2df UV0;
                irr::core::vector2df UV1;
                irr::core::vector2df UV2;
                irr::core::vector2df UV3;
            };

            struct Particle : irr::IReferenceCounted
            {
                ~Particle(void)
                {
                }
                Particle(void)
                {
                    TimeLived = 0;
                    CreateTimeMs = 0;
                    //DestroyTimeMs = 0;
                    LifeTime = 0;
                    //printf("Create Particle\n");
                }
                Particle(const irr::core::vector3df& pos, const irr::core::vector3df& speed, const irr::core::vector3df& rot, const irr::core::vector3df& rotSpeed, const irr::core::vector2df& size, const irr::core::vector2df& sizespeed, const irr::video::SColor& color, const irr::u32& start, const irr::u32& end)
                {
                    Position = pos;

                    Rotation = rot;
                    StartRotation = rot;
                    RotationSpeed = rotSpeed;

                    Speed = speed;
                    StartSpeed = speed;

                    Color = color;
                    StartColor = color;

                    TimeLived = 0;
                    CreateTimeMs = start;
                    //DestroyTimeMs = end;
                    LifeTime = ((irr::f32)(end-start))/1000.f;

                    Size = size;
                    StartSize = size;
                    SizeSpeed = sizespeed;

                    UVs = SParticleUV();
                    //printf("Create Particle\n");
                }
                void reset(const irr::core::vector3df& pos, const irr::core::vector3df& speed, const irr::core::vector3df& rot, const irr::core::vector3df& rotSpeed, const irr::core::vector2df& size, const irr::core::vector2df& sizespeed, const irr::video::SColor& color, const irr::u32& start, const irr::u32& end)
                {
                    Position = pos;

                    Rotation = rot;
                    StartRotation = rot;
                    RotationSpeed = rotSpeed;

                    Speed = speed;
                    StartSpeed = speed;

                    Color = color;
                    StartColor = color;

                    TimeLived = 0;
                    CreateTimeMs = start;
                    //DestroyTimeMs = end;
                    LifeTime = ((irr::f32)(end-start))/1000.f;

                    Size = size;
                    StartSize = size;
                    SizeSpeed = sizespeed;

                    UVs = SParticleUV();
                }
                irr::core::vector2df Size;
                irr::core::vector2df SizeSpeed;
                irr::core::vector2df StartSize;

                irr::core::vector3df Position;
                irr::core::vector3df Speed;
                irr::core::vector3df StartSpeed;

                irr::core::vector3df Rotation;
                irr::core::vector3df StartRotation;
                irr::core::vector3df RotationSpeed;

                SParticleUV UVs;

                irr::video::SColor Color;
                irr::video::SColor StartColor;

                irr::u32 CreateTimeMs;
                //irr::u32 DestroyTimeMs;
                irr::f32 LifeTime;
                irr::f32 TimeLived;
            };
        }
    }
}
#endif // PARTICLE_H
