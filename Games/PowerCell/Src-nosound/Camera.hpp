#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include "Globals.hpp"

class Camera
{
  private:

    position2d<f32> cursorPos;
    ICameraSceneNode* camera;

    float change_x;
    float change_y;

    float direction;
    float zdirection;

    vector3df playerPos;

    float xf;
    float yf;
    float zf;

    IrrlichtDevice *irrDevice;

  public:

    static Camera *Instance();

    void moveCameraControl(void);
    void createCamera(void);

    __inline void setDevice(IrrlichtDevice *device)
    {
      irrDevice = device;
    }

  protected:

    static Camera *m_camera;
    static Camera m_default_camera_buffer;

};

#endif
