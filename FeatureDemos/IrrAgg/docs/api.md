<style type="text/css">
table {
	border: 1px solid #cccccc;
	background-color: #eeeeee;
	margin: 1em;
	padding-left: 5%;
	font-size: 1em;
	/*table-layout: fixed;*/
	width: 95%;
}
</style>

# API

All functions and objects belonging to IrrAgg are within the sub-namespace "vecg" within the namespace "irr" unless otherwise specified. Some exceptions are the conversion functions, which are within the sub-namespace "core" in the namespace "irr".

---

## Standard Utilities

---

### Standalone functions

---

#### isImageSupported( IImage* ): bool

**File:** *irragg_format_support.h*

Returns true if the current format of the image is supported by IrrAgg.

Parameter | Type | Default
----------|------|--------
image | irr::video::IImage*

---

#### isImageARGB32( irr::video::IImage* ): bool

**File:** *irragg_format_support.h*

Returns true if the image is in the color format irr::video::ECF_A8R8G8B8, arranged as alpha-red-green-blue with 8 bits each.

Parameter | Type | Default
----------|------|--------
image | irr::video::IImage*

---

#### toPoint_t( irr::core::vector2df ): point_t

**File:** *irragg_types.h"

Converts The given vector into a point_t.

Parameter | Type | Default
----------|------|--------
v | irr::core::vector2df

---

#### convertSColorToAGGrgba( const SColor& ): agg::rgba

**File:** *irragg_color.h*

**Namespace:** irr::core

Parameter | Type | Default
----------|------|--------
color | const irr::video::SColor&

---

#### convertAGGrgbaToSColor( const rgba& ): irr::video::SColor

**File:** *irragg_color.h*

**Namespace:** irr::core

Parameter | Type | Default
----------|------|--------
color | const agg::rgba&

---

#### convertSColorfToAGGrgba( const SColorf& ): agg::rgba

**File:** *irragg_color.h*

**Namespace:** irr::core

Parameter | Type | Default
----------|------|--------
color | const irr::video::SColorf&

---

#### convertAGGrgbaToSColorf( const rgba& ): irr::video::SColorf

**File:** *irragg_color.h*

**Namespace:** irr::core

Parameter | Type | Default
----------|------|--------
color | const ::agg::rgba&

---

#### convertSColorHSLtoSColor( const SColorHSL& ): irr::video::SColor

**File:** *irragg_color.h*

**Namespace:** irr::core

Parameter | Type | Default
----------|------|--------
color | const irr::video::SColorHSL&

---

#### convertSColorToSColorHSL( const SColor& ): irr::video::SColorHSL

**File:** *irragg_color.h*

**Namespace:** irr::core

Parameter | Type | Default
----------|------|--------
color | const irr::video::SColor&

---

#### convertCMatrix4ToAGGaffine( const CMatrix4< double >& ): agg::trans_affine

**File:** *irragg_matrix.h*

**Namespace:** irr::core

Parameter | Type | Default
----------|------|--------
m | const irr::core::CMatrix4< double >&

---

#### convertAGGaffineToCMatrix4( const trans_affine& ): core::CMatrix4< double >

**File:** *irragg_matrix.h*

**Namespace:** irr::core

Parameter | Type | Default
----------|------|--------
m | const agg::trans_affine&

---

### struct PathMotion

**File:** *irragg_types.h*

**Namespace:** irr::vecg

This class is used for indicating the next point along a path as well as what that point signifies in the path. What it signifies is indicated by the motion flags, LINE_TO, MOVE_TO, END_POLY, and STOP.

Member | Type | Purpose
-------|------|--------
LINE_TO | static const unsigned | Indicates a line should be drawn to this point
MOVE_TO | static const unsigned | Indicates the start of a new polygon
END_POLY | static const unsigned | Reset to zero
STOP | static const unsigned | Close the polygon (and maybe fill)
motion | unsigned
x | double | X-axis coordinate
y | double | Y-axis coordinate

---

### struct IShape

**File:** *irragg_shape.h*

**Namespace:** irr::vecg

This class is meant to be inherited from and its member functions overridden.

Method | Return
-------|-------
getPathMotion( size_t ) | PathMotion
getPathStepCount() | size_t

---

### struct Painter

**File:** *irragg_painter.h*

**Namespace:** irr::vecg

The primary class for rendering IShape objects to an Irrlicht image (IImage).

Member | Type | Purpose
-------|------|--------
Ready | bool | Indicates when rendering can be performed.
Size | core::dimension2du | Size of the canvas.
Canvas | video::IImage* | Target on which the vector graphics will be rendered.
Color | agg::rgba | Color used for rendering.
StrokeWidth | double | Thickness of rendered vector graphic lines.
BSplineStep | double | The amount to increment when spacing points on a BSpline
Step | size_t | Index of the current point on the vector path being rendered.
Shape | const IShape* | Pointer to the vector path provider.
Buffer | agg::rendering_buffer | Accessor to the data of the canvas.
Format | PixelFormat | Coloring mechanism for how to treat the buffer.
Base | agg::renderer_base<PixelFormat> | Provides an interface for the rasterize functions.


Method | Return
-------|-------
(cstor)( irr::video::IVideoDriver*, irr::core::dimension2du ) |
(cstor)( irr::video::IImage* ) |
(dstor) |
setCanvas( irr::video::IImage* ): void |
getCanvas(): irr::video::IImage* |
setColor( irr::video::SColor ): void |
getColor(): irr::video::SColor |
setStrokeWidth( double ) | void
setBSplineResolution( u32 ): void |
drawFilled( const IShape& ) | bool
drawStroked( const IShape& ) | bool
drawBSpline( const IShape& ) | bool
drawBSplineFilled( const IShape& ) | bool
rewind( unsigned ) | void
vertex( double*, double* ) | unsigned

---

#### (cstor) ( IVideoDriver*, dimension2du )

Initializes the class by creating a canvas of the given dimension using the given video driver.

Parameter | Type | Default
----------|------|--------
driver | irr::video::IVideoDriver*
size | irr::core::dimension2du

---

#### (cstor) ( IImage* )

Initializes the class with the given image as a render target and sets whether to drop it upon class destruction. It rejects target images of the wrong color format.

Parameter | Type | Default
----------|------|--------
image | IImage*

---

#### (dstor) ()

Destructor, which deferences the canvas image.

---

#### setCanvas( IImage* ): bool

Sets the canvas image. The image must be of format irr::video::ECF_A8R8G8B8, or the canvas will be set to null.

Parameter | Type | Default
----------|------|--------
image | irr::video::IImage*

---

#### setColor( SColor ): void

Sets the color for rendering paths and shapes.

Parameter | Type | Default
----------|------|--------
color | irr::video::SColor 

---

#### getColor(): irr::video::SColor

Sets the color for rendering paths and shapes.

Parameter | Type | Default
----------|------|--------
color | agg::rgba

---

#### setStrokeWidth( double ): void

Sets the thickness of paths rendered as strokes or bsplines.

Parameter | Type | Default
----------|------|--------
width | double

---

### setBSplineResolution( u32 ): void

Sets the density of points in a bspline path rendering.

Parameter | Type | Default
----------|------|--------
resolution | irr::u32

---

#### drawFilled( const IShape& ): bool

Uses standard-fill-style to draw the path from the given shape.

Parameter | Type | Default
----------|------|--------
shape | irr::vecg::IShape&

---

#### drawStroked( const IShape& ): bool

Draws as line segments the path from the given shape.

Parameter | Type | Default
----------|------|--------
shape | irr::vecg::IShape&

---

#### drawBSpline( const IShape& ): bool

Draws as a bezier curve the path formed by the given shape.

Parameter | Type | Default
----------|------|--------
shape | irr::vecg::IShape&

---

#### drawBSplineFilled( const IShape& ): bool

Draws the given shape as a filled bezier curve.

Parameter | Type | Default
----------|------|--------
shape | irr::vecg::IShape&

---

#### rewind(unsigned): void

AGG-required method. Do not use.

---

#### vertex(double, double): unsigned

AGG-required method. Do not use because the shape will not be set.
