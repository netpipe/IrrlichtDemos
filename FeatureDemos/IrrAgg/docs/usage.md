<style type="text/css">
table {
	border: 1px solid #cccccc;
	background-color: #eeeeee;
	margin: 1em;
}
</style>

# Usage

Much of IrrAgg revolves around rendering vector paths onto an IImage, an abstract object created using Irrlicht via IVideoDriver::createImage. Please see [Creating and Using IImage](#creating-and-using-iimage) for more info.

Rendering to an IImage is done by means of a Painter object. A Painter object can either be initialized with an image or will create one using a given video driver and size. The Painter object contains basic settings, not all of which apply.

Setting Method | Application
---------------|------------
setColor | Sets a color for all rendered path types
setStrokeWidth | Sets the thickness of the path for stroked or bspline paths
setBSplineResolution | Sets the density of increments in a bspline path

The Painter object can take any object inheriting from IShape and implementing its virtual methods. See [Inheriting IShape](#inheriting-ishape) for more details.

A basic example:
```C++
irr::vecg::Triangle  triangle( point_t(10,10), point_t(400,30), point_t(390,450) );
irr::vecg::Painter  painter( irrlichtDevice->getVideoDriver(), irr::core::dimension2du(512,512) );
painter.drawFilled(triangle);
```

## Creating and Using IImage

### Creating

An IImage is created using the *IVideoDriver* interface method *createImage*. Once the IImage has been created, you can render to it with IrrAgg functions. Currently, IrrAgg only supports the color format ECF_A8R8G8B8.

```C++
irr::video::E_DRIVER_TYPE  driverType = irr::video::EDT_BURNINGSVIDEO;
irr::core::dimension2d  screenSize(1200, 800);
irr::core::dimension2d  imageSize(512, 512);

irr::IrrlichtDevice*  device = irr::createDevice(driverType, screenSize);
irr::IVideoDriver*  videoDriver = device->getVideoDriver();
irr::video::IImage*  image = videoDriver->createImage(irr::video::ECF_A8R8G8B8, imageSize);
```

### Rendering

These images must be converted to driver textures prior to rendering. By default, Irrlicht will resize images to powers of 2. This can be changed by setting a driver feature. The following example is a continuation of the previous one and shows disabling mipmap creation, power-of-two resizing, and rendering.
```C++
irr::video::ITexture*  texture;
irr::core::vectore2di  placeOnScreen(30,30);

videoDriver->setTextureCreationFlag(irr::video::ETCF_CREATE_MIP_MAPS, false);
videoDriver->setTextureCreationFlag(irr::video::ETCF_ALLOW_NON_POWER_2, true);
texture = videoDriver->addTexture(irr::io::path("texture name"), image);

// ... Later, usually within the main rendering loop
videoDriver->draw2DImage(texture, placeOnScreen);
```

### Exporting

An IImage can be written to file using the *IVideoDriver* interface of Irrlicht. Some common formats such as PNG and JPEG are supported. The following example illustrates how to do this and assumes the image creation example code has been used.

```C++
videoDriver->writeImageToFile(image, irr::io::path("desired/file/path.png");
```

---

## Inheriting IShape

The basic purpose of IShape is to provide points to the Painter. IShape has two methods:

- getPathMotion( size_t ): PathMotion
- getPathStepCount(): size_t

*getPathMotion()* returns the actual points belonging to the path as well as a flag indicating what the Painter should do in regard to that point:

- LINE_TO - Draw a line from the previous location to this new location.
- MOVE_TO - Set the start of a new polygon.
- END_POLY - Reset starting drawing location to the origin.
- STOP - End painting.

*getPathStepCount()* should return the number of steps along the path, not necessary the number of points. For closed paths, the return of this function should exceed the number of points in the path by one and that last point should be the same as the first.

The Painter will automatically end paths once its own progress along the path exceeds *getPathStepCount()*.

