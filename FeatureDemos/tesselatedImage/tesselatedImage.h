
#ifndef TESSELATEDIMAGE_H_
#define TESSELATEDIMAGE_H_

#include <irrlicht.h>

using namespace irr;

//#include "graphicsirr.h"
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

//! Splits an image readable by Irrlicht of abitrary size into several parts and animates these parts from a given starting position to a given target destination.
class TesselatedImage
{
public:
    enum Type { Billboard, Sphere };

    //! Constructor.
    /*! @param img       - The path to the image.
        @param startFr   - Vector defining the left(x1)/bottom(y1)/front(z1) position of the cube within the parts are being randomly positioned.
        @param startTo   - Vector defining the right(x2)/top(y2)/behind(z2) position of the cube with x2 > x1, y2 > y1, z2 > z1.
        @param targetPos - The bottom left position of the final compound image.
        @param speed     - Amount of units per seconds a part is moving to its target position.
        @param maxTes    - The maximum number of parts that should be used to split the image. If resolutionX * resolutionY < maxTes then a part represents more than one pixel.
        @param colChange - How quickly a part can correct its color to reach the final color, defined in 'one unit per RGB' per second.
        @param type      - What the parts should be made of, default is using a Billboard.
        @param dim       - The quadratic dimension of the billboard or the radius of the sphere. See parameter type.
        @param gap       - The gap between parts.
    */
    TesselatedImage(IrrlichtDevice* driver,stringc img, vector3df startFr, vector3df startTo, vector3df targetPos, float speed=10.0f, int maxTes=5000, float colChange=5.0, Type type=Billboard, float dim=4.0f, float gap=0.05f);
    ~TesselatedImage();

    //! Starts the animation. Can be called right after object generation or a call to reset().
    void startAnimation();
    //! Resets the position and the color (for Billboards only). Random values are being taken for both of them. Can be called consecutively.
    void reset();
    //! Animates the objects. Delta time is the time passed in seconds since last iteration.
    void move(double deltaTime);
    //! Returns true if the animation has stopped recently.
    bool hasAnimationFinished();

private:
    //! The possible status values.
    enum Status { Reset, Animating, Finished };

    //! Representing a part of the final compound picture. Contains one Irrlicht scene node that can be a Billboard.
    struct NodeInfo
    {
        NodeInfo()
        {
            memset(color, 0, sizeof(color));
            pixelInvolved = 0;
            node = 0;
        }

        //! Target color.
        unsigned int color[4];
        //! The current color of the part.
        f32 curCol[4];
        //! The number of pixels the part is representing. Each of these pixels are affecting the final color of the part.
        int pixelInvolved;
        //! Target position.
        f32 pos[3];
        //! The Irrlicht scene node.
        ISceneNode *node;
    };

    //! Starting position from. See paramater description for constructor.
    vector3df startFrom;
    //! Starting position to. See paramater description for constructor.
    vector3df startTo;
    //! The target position. See paramater description for constructor.
    vector3df targetPos;
    //! The array containing the parts the final compound picture is being made of.
    core::array<NodeInfo*> nodeInfo;
    //! The number of rows in the array.
    int rows;
    //! The number of columns in the array.
    int columns;
    //! The radius of the sphere if one is being used. See paramater description for constructor.
    float dim;
    //! The gap between two parts. See paramater description for constructor.
    float gap;
    //! The maximum number of polygons per sphere if one is used.
    int polyCount;
    //! The maximum number of parts that should be created. See paramater description for constructor.
    int maxTesserae;
    //! How quickly a tessera moves to its target position. See paramater description for constructor.
    float speed;
    //! How quickly the final color should be reached. See paramater description for constructor.
    float colorChangesPerSecond;
    //! The current status, if has been reset, is animating or has finished the animation. See enum Status.
    Status status;
    //! The type of scene node that should be used. Can be a Billboard. See paramater description for constructor.
    Type type;
};


#endif
