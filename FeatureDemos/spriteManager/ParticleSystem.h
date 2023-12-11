#ifndef _PARTICLESYSTEM_H_
#define _PARTICLESYSTEM_H_

#include <irrlicht.h>

class SpriteManager;

// Represents a single particle.
struct particle
{
    float lifetime;
    float life;

    float position[2];
    float direction;

    irr::core::vector2df speed;
    float gravity;
    float radialAcceleration;
    float tangentialAcceleration;

    float size;
    float sizeStart;
    float sizeEnd;

    float rotation;
    float spinStart;
    float spinEnd;

    float color[4];
};

/**
* A class for creating, moving and drawing particles.
* A big thanks to bobthebloke.org
**/
class ParticleSystem
{
protected:

    // The max amount of particles.
    unsigned int bufferSize;

    // Pointer to the first particle.
    particle * pStart;

    // Pointer to the next available free space.
    particle * pLast;

    // Pointer to the end of the memory allocation.
    particle * pEnd;

    // The sprite to be drawn.
    SpriteManager * sprite;

    // Whether the particle emitter is active.
    bool active;

    // The emission rate (particles/sec).
    int emissionRate;

    // Used to determine when a particle should be emitted.
    float emitCounter;

    // The relative position of the particle emitter.
    irr::core::vector2df position;

    // The lifetime of the particle emitter (-1 means infinite) and the life it has left.
    float lifetime;
    float life;

    // The particle life.
    float particleLifeMin;
    float particleLifeMax;

    // The direction (and spread) the particles will be emitted in. Measured in radians.
    float direction;
    float spread;

    // Whether the direction should be relative to the emitter's movement.
    bool relative;

    // The speed.
    float speedMin;
    float speedMax;

    // Acceleration towards the bottom of the screen
    float gravityMin;
    float gravityMax;

    // Acceleration towards the emitter's center
    float radialAccelerationMin;
    float radialAccelerationMax;

    // Acceleration perpendicular to the particle's direction.
    float tangentialAccelerationMin;
    float tangentialAccelerationMax;

    // Size.
    float sizeStart;
    float sizeEnd;
    float sizeVariation;

    // Rotation
    float rotationMin;
    float rotationMax;

    // Spin.
    float spinStart;
    float spinEnd;
    float spinVariation;

    // Offsets
    float offsetX;
    float offsetY;

    // Color.
    unsigned char colorStart[4];
    unsigned char colorEnd[4];

    void add();
    void remove(particle * p);

    float tangentialX;
    float tangentialY;

    //float pposX;
    //float pposY;

public:

    /**
    * Creates a particle system with the specified buffersize and sprite.
    **/
    ParticleSystem(SpriteManager * sprite, unsigned int buffer);

    /**
    * Deletes any allocated memory.
    **/
    virtual ~ParticleSystem();

    /**
    * Sets the sprite used in the particle system.
    * @param sprite The new sprite.
    **/
    void setSprite(SpriteManager * image, int sx, int sy, int sw, int sh);

    /**
    * Clears the current buffer and allocates the appropriate amount of space for the buffer.
    * @param size The new buffer size.
    **/
    void setBufferSize(unsigned int size);

    /**
    * Sets the emission rate.
    * @param rate The amount of particles per second.
    **/
    void setEmissionRate(int rate);

    /**
    * Sets the lifetime of the particle emitter (-1 means eternal)
    * @param life The lifetime (in seconds).
    **/
    void setLifetime(float life);

    /**
    * Sets the life range of the particles.
    * @param lifeMin The minimum life.
    * @param lifeMax The maximum life (if 0, then becomes the same as minimum life).
    **/
    void setParticleLife(float min, float max = 0);

    /**
    * Sets the position of the center of the emitter and the direction (if set to relative).
    * Used to move the emitter without changing the position of already existing particles.
    * @param x The x-coordinate.
    * @param y The y-coordinate.
    **/
    void setPosition(float x, float y);

    /**
    * Sets the direction and the spread of the particle emitter.
    * @param direction The direction (in degrees).
    **/
    void setDirection(float direction);

    /**
    * Sets the spread of the particle emitter.
    * @param spread The spread (in degrees).
    **/
    void setSpread(float spread);

    /**
    * Sets whether the direction should be relative to the particle emitters movement. Used in conjunction with setPosition.
    * @param relative Whether to have relative direction.
    **/
    void setRelativeDirection(bool relative);

    /**
    * Sets the speed of the particles.
    * @param speed The speed.
    **/
    void setSpeed(float speed);

    /**
    * Sets the speed of the particles.
    * @param min The minimum speed.
    * @param max The maximum speed.
    **/
    void setSpeed(float min, float max);

    /**
    * Sets the gravity of the particles (the acceleration along the y-axis).
    * @param gravity The amount of gravity.
    **/
    void setGravity(float gravity);

    /**
    * Sets the gravity of the particles (the acceleration along the y-axis).
    * @param min The minimum gravity.
    * @param max The maximum gravity.
    **/
    void setGravity(float min, float max);

    /**
    * Sets the radial acceleration (the acceleration towards the particle emitter).
    * @param acceleration The amount of acceleration.
    **/
    void setRadialAcceleration(float acceleration);

    /**
    * Sets the radial acceleration (the acceleration towards the particle emitter).
    * @param min The minimum acceleration.
    * @param max The maximum acceleration.
    **/
    void setRadialAcceleration(float min, float max);

    /**
    * Sets the tangential acceleration (the acceleration perpendicular to the particle's direction).
    * @param acceleration The amount of acceleration.
    **/
    void setTangentialAcceleration(float acceleration);

    /**
    * Sets the tangential acceleration (the acceleration perpendicular to the particle's direction).
    * @param min The minimum acceleration.
    * @param max The maximum acceleration.
    **/
    void setTangentialAcceleration(float min, float max);

    /**
    * Sets the size of the sprite (1.0 being the default size).
    * @param size The size of the sprite.
    **/
    void setSize(float size);

    /**
    * Sets the size of the sprite upon creation and upon death (1.0 being the default size).
    * @param start The size of the sprite upon creation
    * @param end The size of the sprite upon death.
    **/
    void setSize(float start, float end);

    /**
    * Sets the size of the sprite upon creation and upon death (1.0 being the default size) and any variation.
    * @param start The size of the sprite upon creation
    * @param end The size of the sprite upon death.
    * @param variation The amount of variation on the starting size (0 being no variation and 1.0 a random size between start and end).
    **/
    void setSize(float start, float end, float variation);

    /**
    * Sets the amount of variation to the sprite's beginning size (0 being no variation and 1.0 a random size between start and end).
    * @param variation The amount of variation.
    **/
    void setSizeVariation(float variation);

    /**
    * Sets the amount of rotation a sprite starts out with.
    * @param rotation The amount of rotation.
    **/
    void setRotation(float rotation);

    /**
    * Sets the amount of rotation a sprite starts out with (a random value between min and max).
    * @param min The minimum amount of rotation.
    * @param max The maximum amount of rotation.
    **/
    void setRotation(float min, float max);

    /**
    * Sets the spin of the sprite.
    * @param spin The spin of the sprite (in degrees).
    **/
    void setSpin(float spin);

    /**
    * Sets the spin of the sprite upon particle creation and death.
    * @param start The spin of the sprite upon creation (in degrees).
    * @param end The spin of the sprite upon death (in degrees).
    **/
    void setSpin(float start, float end);

    /**
    * Sets the spin of the sprite upon particle creation and death and the variation.
    * @param start The spin of the sprite upon creation (in degrees).
    * @param end The spin of the sprite upon death (in degrees).
    * @param variation The variation of the start spin (0 being no variation and 1 beign a random spin between start and end).
    **/
    void setSpin(float start, float end, float variation);

    /**
    * Sets the variation of the start spin (0 being no variation and 1 beign a random spin between start and end).
    * @param variation The variation in degrees.
    **/
    void setSpinVariation(float variation);

    /**
    * Sets the color of the particles.
    * @param color The color.
    **/
    void setColor(unsigned char * color);

    /**
    * Sets the particles' offsets for rotation.
    * @param x The x offset.
    * @param y The y offset.
    **/
    void setOffset(float x, float y);

    /**
    * Sets the color of the particles.
    * @param start The color of the particle when created.
    * @param end The color of the particle upon death.
    **/
    void setColor(unsigned char * start, unsigned char * end);

    /**
    * Returns the x-coordinate of the emitter's position.
    **/
    float getX() const;

    /**
    * Returns the y-coordinate of the emitter's position.
    **/
    float getY() const;

    /**
    * Returns the direction of the emitter (in degrees).
    **/
    float getDirection() const;

    /**
    * Returns the directional spread of the emitter (in degrees).
    **/
    float getSpread() const;

    /**
    * Returns the X offset of the particles.
    **/
    float getOffsetX() const;

    /**
    * Returns the Y offset of the particles.
    **/
    float getOffsetY() const;

    /**
    * Returns the amount of particles that are currently active in the system.
    **/
    int count() const;

    /**
    * Starts/resumes the particle emitter.
    **/
    void start();

    /**
    * Stops the particle emitter and resets.
    **/
    void stop();

    /**
    * Pauses the particle emitter.
    **/
    void pause();

    /**
    * Resets the particle emitter.
    **/
    void reset();

    /**
    * Returns whether the particle emitter is active.
    **/
    bool isActive() const;

    /**
    * Returns whether the particle system is empty of particles or not.
    **/
    bool isEmpty() const;

    /**
    * Returns whether the amount of particles has reached the buffer limit or not.
    **/
    bool isFull() const;

    /**
    * Draws the particle emitter at the specified position.
    * @param x The x-coordinate.
    * @param y The y-coordinate.
    **/
    virtual void draw(float x, float y, float angle, float sx, float sy, float ox, float oy) const;

    /**
    * Updates the particle system.
    * @param dt Time since last update.
    **/
    void update(float dt);

    int sx;
    int sy;
    int sw;
    int sh;
};


#endif // _PARTICLESYSTEM_H_
