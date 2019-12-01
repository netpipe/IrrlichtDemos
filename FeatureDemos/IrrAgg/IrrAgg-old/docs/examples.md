# Examples

There are two approaches to using IrrAGG: [using the standalone functions](#using-the-standalone-functions) or [using a rendering engine](#using-a-rendering-engine). For all of the examples, you should need to create an IImage, which you can do using the [initial setup](#initial-setup).

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

## Using the Standalone Functions

### Rendering a B-Spline from static const arrays.

```C++
//... Assuming the initial setup

bool drawPath( irr::video::IImage*  target )
{
  irr::video::SColor  color(255,255,0,0); // Red
  irr::f32  width = 2; // width of rendered curve
  irr::u32  resolution = 20; // number of segments between points for a bspline curve

  static const double  x_values[] = { 10, 100, 200, 300, 400, 500 };
  static const double  y_values[] = { 50, 200, 350, 400, 200, 100 };

  // Using an irragg structure
  irr::vecg::ArrayVertexSource  vertex_source(
    5, // number of points
    (const double*)&x_values,
    (const double*)&y_values
  );

  // Using an irragg function
  return irr::vecg::renderVectorPath(vertex_source, target, color, irr::vecg::EPathStroke::BSPLINE, width, resolution);
}
```

---

## Using a Rendering Engine

### Drawing a Skewed Rectangle Outline

```C++
//... Assuming the initial setup

bool drawPath( irr::video::IImage*  target )
{
  irr::video::SColor  green(255,0,255,0); // Green

  // Using an irragg structure
  irr::vecg::engine::RendererARGB32  renderer;

  // Pre-drawing necessities
  //   Make the drawing matrix skewed by 0.5 on top
  renderer.setDrawingMatrix(1.0, 0.5, 0, 1.0);

  //   Set the outline thickness
  renderer.setStrokeWidth(5);

  //   Drawing color must also be set in advance
  renderer.setDrawingColor(green);

  // For clarity and convenience
  typedef irr::core::vector2d<double>  vector2dd;

  // Draw the actual rectangle outline
  return renderer.drawRectangle(
    vector2dd(0,0),
    vector2dd(100,100),
    irr::vecg::EPathStroke::BRUSH
  );
}
```

---

## Using Helper Classes

### Using StaticSizeVertexSource

```C++
bool drawPath( irr::video::IImage*  target )
{
  // Instantiation requires size as the template argument
  irr::vecg::StaticSizeVertexSource<3> triangle;

  // Point value setting can be chained
  triangle
    .set(0, 150, 30)
    .set(1, 300, 150)
    .set(2, 0, 150);

  // Rendering requires a color
  irr::video::SColor  blue(255,0,0,255);

  // Render using any function that can render vertex sources
  // or descendents of PointSource
  return irr::vecg::renderVectorPath(triangle, target, blue);
}
```
