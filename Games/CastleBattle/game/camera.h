#ifndef CAMERA_H
#define CAMERA_H
#include <irrlicht.h>
using namespace irr;
/** Camera based on ICameraSceneNode of Irrlicht */
class Camera{
    private:
        scene::ISceneManager* smgr;

    public:
        /** Irrlicht ICameraSceneNode */
        scene::ICameraSceneNode* camera;

        Camera(scene::ICameraSceneNode* camera, scene::ISceneManager* smgr);

        /** Create camera node and bind with the  camera specified in the arguments.
        *  @param camera ICameraSceneNode camera to copy
        *  @param parent ISceneNode  parent object of the camera(or the visual object of what the camera is referring)
        *  @param smgr ISceneManager Irrlicht scene manager
        */
        Camera(scene::ICameraSceneNode* camera, scene::ISceneNode* parent, scene::ISceneManager* smgr);

        /** Create camera node with a parent, rotation and offset from the parent specified in the argument.
        * @param offset vector3df offset from the parent position
        * @param rotation vector3df rotation to look at
        * @param smgr ISceneManager Irrlicht scene manager
        * @param parent ISceneNode parent visual object
        */
        Camera(core::vector3df offset, core::vector3df rotation,scene::ISceneManager* smgr, scene::ISceneNode* parent);
        ~Camera();
        /** Set active camera in irrlicht */
        void activeCamera();
        /** @deprecated Get camera irrlicht object  */
        scene::ICameraSceneNode* getCamera();

};
#endif // CAMERA_H
