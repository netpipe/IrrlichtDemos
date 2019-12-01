# Examples

For all of the examples, you should need to create an IImage, which you can do using the [initial setup](#initial-setup).

---

## Initial Setup

The following code can be used to create the IImage and pass it to the drawPath() function shown in all of the examples.

```C++
// Include Irrlicht
#include <irrlicht.h>

// Include this library
#include <irragg.h>

bool drawPath(irr::video::IImage*  target);

int main()
{
  irr::core::dimension2du  screensize(512,512);
  irr::IrrlichtDevice*  device;
  irr::video::IVideoDriver*  video_driver;
  irr::video::IImage*  image;
  irr::video::ITexture*  texture;
  irr::video::EColorFormat  color_format = irr::video::ECF_A8R8G8B8;
  irr::core::vector2di  onscreen_position(0);
  bool render_success;

  device = irr::createDevice(irr::video::EDT_BURNINGSVIDEO, screensize);
  if (!device)
    return 1;

  // Creating the image
  video_driver = device->getVideoDriver();
  image = video_driver->createImage(color_format, screensize);
  render_success = drawPath(image);

  if ( !render_success )
    return 1;

  texture = video_driver->addTexture(irr::io::path("texture name"), image);

  // Main rendering loop
  while( device->run() )
  {
    video_driver->beginScene();
    video_driver->draw2DImage(texture, onscreen_position);
    video_driver->endScene();
  }

  // cleanup
  image->drop();
  device->drop();

  return 0;
}
```

---

## Using the Painter

### Rendering a Triangle

```C++
//... Assuming the initial setup

bool drawPath( irr::video::IImage*  target )
{
  irr::video::SColor  color(255,255,0,0); // Red
  irr::f32  width = 2; // width of rendered curve
  irr::u32  resolution = 20; // number of segments between points for a bspline curve

  irr::vecg::Triangle  triangle( point_t(10,10), point_t(400,30), point_t(390,450) );
  Painter  painter(target);
  painter.setColor(color);
  painter.setStrokeWidth(width);
  painter.setBSplineResolution(resolution);

  return painter.drawBSpline(triangle);
}
```

