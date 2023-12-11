<style type="text/css">
table {
	border: 1px solid #cccccc;
	background-color: #eeeeee;
	margin: 1em;
}
</style>

# Usage

Much of IrrAgg revolves around rendering vector paths onto an IImage, an abstract object created using Irrlicht via IVideoDriver::createImage. Please see [Creating and Using IImage](#creating-and-using-iimage) for more info.

Rendering to an IImage is can be done in one of two ways:

1. via the function irr::vecg::renderVectorPath.
2. via the class irr::vecg::engine::RendererARGB32.

Both methods can be used on the same IImage instance.

A number of other convenience functions and structures are available to aid in rendering with either approach. Please see [Useful Objects and Functions](#useful-objects-and-functions).

Most of your rendering needs can be done using pre-existing components, such as ArrayVertexSource. However, you may wish to create specialized classes for handling vertices. See [AGG Vertex Sources](#agg-vertex-sources) for more details.

**WARNING:** Using templates can hide bugs that only appear as segmentation faults. AGG uses templates for speed, but IrrAgg provides its rendering engine as an alternative to direct rendering (using renderVectorPath) to help prevent these hidden bugs.

## Drawing Modes

Anti-Grain Geometry provides various modes of drawing, most of which are not wrapped by this library.

There are three modes of drawing provided by IrrAgg: *NORMAL*, *BRUSH*, and *BSPLINE*. These are given as values of the enum *EPathStroke* (file: irragg_path_stroke.h) and can be passed to a number of rendering functions to indicate how the path should be drawn.

- NORMAL: Renders a solid, closed polygon.
- BRUSH: Renders the vector path with lines. Functions rendering this way will also accept a *stroke width* value to determine how thick the lines will be.
- BSPLINE: Renders a smooth curve. Functions rendering this way will also accept a *stroke width* value and a *resolution* value, which indicates how many segments should be used between primary points in order to smooth out the curve.

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

## Useful Objects and Functions

All functions and objects belonging to IrrAgg are in the sub namespace "vecg" within the namespace "irr" with the exception of the matrix conversion functions.

### Standard Rendering Functions

#### Function List

- renderVectorPath() < class VertexSource >
- renderTriangle()
- renderEllipse()
- renderRoundedRectangle()

### Standard Rendering Objects

Most IrrAgg objects are intended to fulfill the role of VertexSource, which is a generic template name referring to any class with the following methods:
- void rewind( unsigned )
- unsigned vertex( double* x, double* y )

See [Creating a Standard Vertex Source](#creating-a-standard-vertex-source) for more information.

#### Object List

- VertexSourceBase
- VertexSourcePump < class VertexSource >
- VertexSourceFuncPump
- TrianglePath

### Engine-Based Rendering

These utilities are form a simple, convenient package for rendering vector graphics.

#### Object List

- PointSource
- RendererARGB32

---

## AGG Vertex Sources

Nearly if not all of the functionality of AGG is based on templates. The basic rendering functions of IrrAgg - such as renderVectorPath - limit the need for templates but are templated in order to mimic the functionality of AGG. On the other hand, structures like RendererARGB32 are not templated and don't require any templates to perform rendering.

For rendering vectors, AGG requires a path of some sort. This path can be created from a standard vertex source (used with renderVectorPath) or by a class inheriting PointSource (and rendered with RendererARGB32).

### Creating a Standard Vertex Source

In this method, the path of vertices for AGG is defined by user-created class with two special methods that are called within AGG. Such classes are called "vertex sources" because they control the vertices that define the path rendered by AGG. IrrAGG has a number of rendering functions that accept these "vertex sources.

The vertex source class must have the following two methods:

- *unsigned vertex( double* x, double* y )*
- *void rewind( unsigned )*

The vertex method must set the values of "x" and "y" and be "incremented". It must then return an unsigned integer represeting a flag. Flag values are:

- *agg::path_cmd_move_to*
- *agg::path_cmd_line_to*
- *agg::path_cmd_end_poly*
- *agg::path_cmd_stop*

The flag agg::path_cmd_stop must eventually be returned or AGG will run indefinitely.

### Implementing PointSource

In this method, the path of vertices for rendering is defined by the following virtual methods from class irr::vecg::PointSource:

- size_t *getPointCount*()
- Point2D *getPoint*( unsigned *step* )

Both of these methods should be overridden.

Rather than being passed directly to AGG or renderVectorPath, descendents of PointSource are passed to small rendering engines.

```C++

class MyShape : public irr::vecg::PointSource
{
public:
	virtual size_t
		getPointCount() const
	{
		return 5;
	}

	virtual irr::vecg::Point2D
		getPoint( unsigned  step )
	{
		switch(step) {
		case 0: return irr::vecg::Point2D(50, 30);
		case 1: return irr::vecg::Point2D(200, 50);
		case 2: return irr::vecg::Point2D(150, 300);
		case 3: return irr::vecg::Point2D(80, 200);
		default: return irr::vecg::Point2D(100,100);
		}
	}
};

int main()
{
	//...

	MyShape  shape;
	irr::vecg::engine::RendererARGB32  renderer;
	renderer.setRenderTarget( image );
	renderer.setDrawingColor( irr::video::SColor(0xffffffff) );
	renderer.bufferPoints( shape );
	renderer.draw();

	//...
}

```
