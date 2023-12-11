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

### class Point2D

**File:** *irragg_point.h*

A generic point class used especially for engine-based rendering but also stored in other classes.

Member | Type | Purpose
-------|------|--------
x | double | X-axis coordinate
y | double | Y-axis coordinate

---

### class StaticSizeVertexSource< size_t >

**File:** *irragg_array_path2d.h*

**Inherits:** *VertexSourceBase*

A static-size storage class that can be rendered using either renderVectorPath() or a rendering engine. Its template parameter is its size.

See [Example usage](examples/#using-staticsizevertexsource).

Member | Type | Purpose
-------|------|--------
_x | double[] | Contains the X-axis values of the path points
_y | double[] | Contains the Y-axis values of the path points

Method | Return
-------|-------
(cstor) |
set( size_t, double, double ) | StaticSizeVertexSource&
getPointCount() | size_t
rewind( unsigned ) | void
vertex( double*, double* ) | unsigned
getPoint( size_t ) | Point2D

---

#### (cstor)

Initializes the class.

---

#### set( size_t, double, double ): StaticSizeVertexSource&

Set point coordinates of the point at the given index.

Parameter | Type | Default
----------|------|--------
index | size_t
x | double
y | double

---

#### getPointCount(): size_t

Returns the size of the class.

---

#### rewind( unsigned ): void

Resets the iterator incrementing along the path.

Parameter | Type | Default
----------|------|--------
id | unsigned

---

#### vertex( double*, double* ): unsigned

Sets the axial positions of the next point in the path.

Parameter | Type | Default
----------|------|--------
x | double*
y | double*

---
		
#### getPoint( size_t ): Point2D

Get the point at the given index.

Parameter | Type | Default
----------|------|--------
index | size_t

---

### ArrayVertexSource

**File:** *irragg_array_path2d.h*

**Inherits:** *VertexSourceBase*

A dynamically-sized storage class that can be rendered using either renderVectorPath() or a rendering engine.

Method | Return
-------|-------
(cstor) |
(copy cstor) |
(cstor) ( const array< Point2D >& ) |
(cstor) ( const array< vector2di >& ) |
(cstor) ( const size_t, const double*, const double* ) |
setFrom( const ArrayVertexSource& ) | void
setFrom( const array< Point2D >& ) | void
setFrom( const array< vector2di >& ) | void
setFrom( const size_t, const double*, const double* ) | void
add( double, double ) | ArrayVertexSource&
getPointCount() | size_t
rewind( unsigned ) | void
vertex( double*, double* ) | unsigned
getPoint( size_t ) | Point2D

---

#### (cstor)

Initializes the class.

---

#### (copy cstor)

Initializes the class by copying data from another instance.

---

#### (cstor) ( const array< Point2D >& )

Initializes the class using points data from the given array.

Parameter | Type | Default
----------|------|--------
data_array | const irr::core::array< Point2D >&

---

#### (cstor) ( const array< vector2di >& )

Parameter | Type | Default
----------|------|--------
data_array | const irr::core::array< irr::core::vector2di >&

---

#### (cstor) ( const size_t, const double*, const double* )

Construct a vertex source from two arrays of size num_values.

WARNING: Does NOT check for premature array end!

Parameter | Type | Default
----------|------|--------
num_values | const size_t
x_values | const double*
y_values | const double*

---

#### setFrom( const ArrayVertexSource& ): void

Sets this vertex source from the points in the given array.

Parameter | Type | Default
----------|------|--------
other | const ArrayVertexSource&

---

#### setFrom( const array< Point2D >& ): void

Initializes the class using points data from the given array.

Parameter | Type | Default
----------|------|--------
data_array | const irr::core::array< Point2D >&

---

#### setFrom( const array< vector2di >& ): void

Parameter | Type | Default
----------|------|--------
data_array | const irr::core::array< irr::core::vector2di >&

---

#### setFrom( const size_t, const double*, const double* ): void

Construct a vertex source from two arrays of size num_values.

WARNING: Does NOT check for premature array end!

Parameter | Type | Default
----------|------|--------
num_values | const size_t
x_values | const double*
y_values | const double*

---

#### add( double, double ): ArrayVertexSource&

Add the point (x,y) to the array.

Parameter | Type | Default
----------|------|--------
x | double
y | double

---

#### add( const Point2D& ): ArrayVertexSource&

Add the point (x,y) to the array.

Parameter | Type | Default
----------|------|--------
point | const Point2D&

---

#### pop( size_t ): size_t

Remove last points in the array. Returns the number of remaining points.

Parameter | Type | Default
----------|------|--------
num_points_to_remove | size_t | 1

---

#### clear(): void

Delete all points stored in this instance.

---

#### getPointCount(): size_t

Return the number of points stored in this instance.

---

#### prepare( size_t ): void

Performs space allocation in preparation for adding points.

Parameter | Type | Default
----------|------|--------
num_points_to_expect | size_t

---

## Standard Vertex Source Rendering

---

### Standalone functions

---

#### renderVectorPath( < VertexSource >&, IImage, SColor, EPathStroke::Value, unsigned, unsigned ): bool

**File:** *irragg_standalone.h*

Render the path given by *vertex_source* onto *target*. There will automatically be a check for supported image type and rendering will be delegated to the function with the appropriate path stroke type. Returns true if successful.

Parameter | Type | Default
----------|------|--------
vertex_source | template < VertexSource >&
target | irr::video::IImage*
color | irr::video::SColor
path_stroke_type | EPathStroke::Value | EPathStroke::NORMAL
stroke_width | unsigned | 1
resolution | unsigned | 20

---

#### renderTriangle( double, double, double, double, double, double, SColor, EPathStroke::Value, u32, u32 ): bool

**File:** *irragg_shapes.h*

Render a triangle defined by the points *(x1, y1), (x2, y2), (x3, y3)* with the given color *color* onto *target*. Returns true if successful.

Parameter | Type | Default
----------|------|--------
target | irr::video::IImage*
x1 | double
y1 | double
x2 | double
y2 | double
x3 | double
y3 | double
color | irr::video::SColor
path_stroke_type | EPathStroke::Value | EPathStroke::NORMAL
stroke_width | irr::u32
resolution | irr::u3

---

#### renderEllipse( IImage, double, double, double, double, SColor, EPathStroke::Value, u32, u32 ): bool

**File:** *irragg_shapes.h*

Render an ellipse centered at the point *(x,y)* with the given vertical radius of *altitude*, horizontal radius of *radius*, and color *color* onto *target*. Returns true if successful.

Parameter | Type | Default
----------|------|--------
target | irr::video::IImage*
x | double
y | double
altitude | double
radius | double
color | irr::video::SColor
path_stroke_type | EPathStroke::Value | EPathStroke::NORMAL
stroke_width | irr::u32
resolution | irr::u32

---

#### renderRoundedRectangle( IImage*, double, double, double, double, double, SColor, EPathStroke::Value, u32, u32 ): bool

**File:** *irragg_shapes.h*

Render a rectangle - defined by the given x,y coordinates - with rounded corners of the given radius and color onto *target*. Returns true if successful.

Parameter | Type | Default
----------|------|--------
target | irr::video::IImage*
x1 | double
y1 | double
x2 | double
y2 | double
radius | double
color | irr::video::SColor
path_stroke_type | EPathStroke::Value | EPathStroke::NORMAL
stroke_width | irr::u32
resolution | irr::u32

---

#### renderRoundedRectangle( IImage*, double, double, double, double, double, double, double, double, SColor, EPathStroke::Value, u32, u32 ): bool

**File:** *irragg_shapes.h*

Render a rectangle - defined by the given x,y coordinates - with rounded corners of the given radius and color onto *target*. Returns true if successful.

Parameter | Type | Default
----------|------|--------
target | irr::video::IImage*
x1 | double
y1 | double
x2 | double
y2 | double
radius_x_bottom | double
radius_y_bottom | double
radius_x_top | double
radius_y_top | double
color | irr::video::SColor
path_stroke_type | EPathStroke::Value | EPathStroke::NORMAL
stroke_width | irr::u32
resolution | irr::u32

---

#### renderRoundedRectangle( IImage*, double, double, double, double, double, double, double, double, double, double, double, double, SColor, EPathStroke::Value, u32, u32 ): bool

**File:** *irragg_shapes.h*

Render a rectangle - defined by the given x,y coordinates - with rounded corners of the given radius and color onto *target*. Returns true if successful.

Parameter | Type | Default
----------|------|--------
target | irr::video::IImage*
x1 | double
y1 | double
x2 | double
y2 | double
radius_upper_left_x | double
radius_upper_left_y | double
radius_upper_right_x | double
radius_upper_right_y | double
radius_lower_left_x | double
radius_lower_left_y | double
radius_lower_right_x | double
radius_lower_right_y | double
color | irr::video::SColor
path_stroke_type | EPathStroke::Value | EPathStroke::NORMAL
stroke_width | irr::u32
resolution | irr::u32

---

### VertexSourceBase

**File:** *irragg_vertexsources.h*

Meant to be inherited, it contains two primitives representing a polygon's origin value (absolute positioning) and a "step" value, used for tracking incrementing.

WARNING: It does not contain any methods necessary for rendering via renderVectorPath. Those methods must be implemented by the class inheriting this one. See [Creating a Standard Vertex Source](usage/#creating-a-standard-vertex-source) for more details.

Member | Type | Purpose
-------|------|--------
_step | unsigned | The current increment along the vector path.
_origin_x | double | The X-axis value to which all points along the path are relative.
_origin_y | double | The Y-axis value to which all points along the path are relative.

Method | Return
-------|-------
(cstor) () |
(cstor) ( double, double ) |
setOrigin( double, double ) | void
getOrigin() | Point2D

---

#### (cstor)

Initializes the class.

---

#### (cstor) ( double, double )

Initializes the class and sets the coordinates of the origin.

Parameter | Type | Default
----------|------|--------
x | double
y | double

---

#### setOrigin( double, double ): void

Sets the absolute central position of the polygon (for shapes) or the start of a path.

Parameter | Type | Default
----------|------|--------
x | double
y | double

---

#### getOrigin(): Point2D

Gets the absolute central position of the polygon (for shapes) or the start of a path.

---

### VertexSourcePump < class VertexSource >

**File:** *irragg_vertexsources.h*

**Inherits:** *VertexSourceBase*

This class acts as a Vertex Source that can be passed directly to certain rendering functions such as *renderVertexSource< VertexSource >*. Meant for creating a single polygon, this class is a vertex source template for wrapping a class having the following methods:

- *getPoint*( unsigned ): Point2D
- *getPointCount*(): unsigned

The method *getPoint* will be passed *step* - an unsigned int representing the increment along the path/curve/polygon that is currently being requested. The method must return an instance of Point2D to represent that point.

The method *getPointCount* should return the number of points along the path/curve/polygon that the object is able to return. This value can be varied but should not change when the polygon is being rendered.

Method | Return
-------|-------
(cstor) ( template < VertexSource > ) |
rewind( unsigned ) | void
vertex( double*, double* ) | void

---

#### (cstor) ( < VertexSource > )

Initializes the class and sets the vertex source.

Parameter | Type | Default
----------|------|--------
source | template < VertexSource >

---

#### rewind( unsigned ): void

Resets the iterator incrementing along the path.

Parameter | Type | Default
----------|------|--------
id | unsigned

---

#### vertex( double*, double* ): unsigned

Sets the axial positions of the next point in the path.

Parameter | Type | Default
----------|------|--------
x | double*
y | double*

---

### VertexSourceFunction

**File:** *irragg_vertexsources.h*

A function pointer that accepts unsigned and returns Point2D.

```C++
typedef Point2D (*VertexSourceFunction)(unsigned)
```

---

### VertexSourceFuncPump

**File:** *irragg_vertexsources.h*

This class acts as a Vertex Source that can be passed directly to certain rendering functions such as *renderVertexSource< VertexSource >*. This class acts as a wrapper for functions that define a vector path so that they do not need to be implemented as a class.

Method | Return
-------|-------
(cstor) ( unsigned, VertexSourceFunction ) |
rewind( unsigned ) | void
vertex( double*, double* ) | unsigned

---

#### (cstor) ( unsigned,  VertexSourceFunction )

Initializes the class and sets the point count and the function to be used.

Parameter | Type | Default
----------|------|--------
num_points | unsigned
function | VertexSourceFunction

---

#### rewind( unsigned ): void

Resets the iterator incrementing along the path.

Parameter | Type | Default
----------|------|--------
id | unsigned

---

#### vertex( double*, double* ): unsigned

Sets the axial positions of the next point in the path.

Parameter | Type | Default
----------|------|--------
x | double*
y | double*

---

### TrianglePath

**File:** *irragg_shapes.h*

**Inherits:** *VertexSourceBase*

This class acts as a Vertex Source that can be passed directly to *renderVertexSource< VertexSource >* to render a triangle defined by the points *point_a*, *point_b*, and *point_c*. Its origin can be set via the members it inherits from *VertexSourceBase*: *setOrigin()*.

Member | Type | Purpose
-------|------|--------
point_a | irr::core::vector2d< double >
point_b | irr::core::vector2d< double >
point_c | irr::core::vector2d< double >


Method | Return
-------|-------
(cstor) () |
(cstor) ( vector2d< double >, vector2d< double >, vector2d< double > ) |
rewind( unsigned ) | void
vertex( double*, double* ) | unsigned

---

#### (cstor) ()

---

#### (cstor) ( vector2d< double >, vector2d< double >, vector2d< double > )

Initializes the class and sets the defining points of the triangle.

Parameter | Type | Default
----------|------|--------
a | irr::core::vector2d< double >
b | irr::core::vector2d< double >
c | irr::core::vector2d< double >

---

#### rewind( unsigned ): void

Resets the iterator incrementing along the path.

Parameter | Type | Default
----------|------|--------
id | unsigned

---

#### vertex( double*, double* ): unsigned

Sets the axial positions of the next point in the path.

Parameter | Type | Default
----------|------|--------
x | double*
y | double*

---

## Engine-Based Rendering

---

### class PointSource

**File:** *irragg_point.h*

**Namespace:** irr::vecg

Method | Return
-------|-------
virtual getPointCount | size_t
virtual getPoint( size_t ) | Point2D

---

#### virtual getPointCount(): size_t

Returns the number of points in the path.

---

#### virtual getPoint( size_t ): Point2D

Returns the point at the given step value.

Parameter | Type | Default
----------|------|--------
step | size_t

---

### class RendererARGB32

**File:** *irragg_engine.h*

**Namespace:** irr::vecg::engine

This class is a self-contained structure that provides a convenient drawing interface for rendering vector graphics onto an IImage. It can render paths designated by descendents of PointSource or ArrayVertexSource. All of the paths drawn are affected by the set drawing color and drawing matrix.

Method | Return
-------|-------
[cstor] () |
[cstor] ( IImage*, bool ) |
[dstor] () |
setRenderTarget( IImage*, bool ) | bool
setDrawingColor( SColor ) | void
setDrawingColor( agg::rgba ) | void
setStrokeWidth( double ) | void
setDrawingMatrixFull( CMatrix4< double > ) | void
setDrawingMatrixFull( agg::trans_affine ) | void
setDrawingMatrix( double, double, double, double ) | void
setDrawingMatrixTranslation( double, double ) | void
setDrawingMatrixFull( double, double, double, double, double, double ) | void
bufferPoints( PointSource* ) | bool
bufferPoints( ArrayVertexSource& ) | bool
draw( EPathStroke::Value ) | bool
drawNormal() | bool
drawStroked() | bool
drawBSpline() | bool
drawPointSource( PointSource*, EPathStroke::Value ) | bool
drawArrayVertexSource( ArrayVertexSource&, EPathStroke::Value ) | bool
drawTriangle( vector2d< double >, vector2d< double >, vector2d< double >, EPathStroke::Value ) | bool
drawRectangle( vector2d< double >, vector2d< double >, EPathStroke::Value ) | bool
drawCircle( double, vector2d< double >, EPathStroke::Value ) | bool

---

#### (cstor) ()

---

#### (cstor) ( IImage*, bool )

Initializes the class with the given image as a render target and sets whether to drop it upon class destruction. It rejects target images of the wrong color format.

Parameter | Type | Default
----------|------|--------
target | IImage*
drop_with_destruct | bool

---

#### (dstor) ()

Destructor, which also destroys the target if set to do so.

---

#### setRenderTarget( IImage*, bool ): bool

Sets the render target (if it is of the correct color format) and whether to drop it on class destruction.

Parameter | Type | Default
----------|------|--------
target | IImage*
drop_with_destruct | bool

---

#### setDrawingColor( SColor ): void

Sets the color for rendering paths and shapes.

Parameter | Type | Default
----------|------|--------
color | irr::video::SColor 

---

#### setDrawingColor( rgba ): void

Sets the color for rendering paths and shapes.

Parameter | Type | Default
----------|------|--------
color | agg::rgba

---

#### setStrokeWidth( double ): void

Sets the width of rendered paths.

Parameter | Type | Default
----------|------|--------
stroke_width | double

---

#### setDrawingMatrixFull( drawing_matrix: irr::core::CMatrix4< double > ): void

Sets the matrix used for rendering shapes and paths.

Parameter | Type | Default
----------|------|--------


---

#### setDrawingMatrixFull( drawing_matrix: agg::trans_affine ): void

Sets the matrix used for rendering shapes and paths.

Parameter | Type | Default
----------|------|--------


---

#### setDrawingMatrix( s00: double, s10: double, s01: double, s11: double ): void

Sets the rotation/scale/shear part of the matrix used for rendering shapes and paths.

Parameter | Type | Default
----------|------|--------


---

#### setDrawingMatrixTranslation( x: double, y: double ): void

Sets the translation part of the matrix used for rendering shapes and paths.

Parameter | Type | Default
----------|------|--------


---

#### setDrawingMatrixFull( s00: double, s10: double, s01: double, s11: double, tx: double, ty: double ): void

Sets the matrix used for rendering shapes and paths.

Parameter | Type | Default
----------|------|--------
s00 | double
s10 | double
s01 | double
s11 | double
tx | double
ty | double

---

#### bufferPoints( PointSource* ): bool

Copies and prepares-for-rendering points taken from the given source.

Parameter | Type | Default
----------|------|--------
source | PointSource*

---

#### bufferPoints( ArrayVertexSource& ): bool

Copies and prepares-for-rendering points taken from the given source.

Parameter | Type | Default
----------|------|--------
source | ArrayVertexSource&

---

#### draw( EPathStroke::Value ): bool

Draws the path formed by the buffered points onto the target image.

Parameter | Type | Default
----------|------|--------
path_stroke_type | EPathStroke::Value

---

#### drawNormal(): bool

Uses standard-fill-style to draw the path formed by the buffered points.

---

#### drawStroked(): bool

Draws as line segments the path formed by the buffered points.

---

#### drawBSpline(): bool

Draws as a bezier curve the path formed by the buffered points.

---

#### drawPointSource( PointSource*, EPathStroke::Value ): bool

Draws the points from the given point source onto the target image.

Parameter | Type | Default
----------|------|--------
source | PointSource*
path_stroke_type | EPathStroke::Value

---

#### drawArrayVertexSource( ArrayVertexSource&, EPathStroke::Value ): bool

Draws the points from the given array vertex source onto the target image.

Parameter | Type | Default
----------|------|--------
source | ArrayVertexSource&
path_stroke_type | EPathStroke::Value

---

#### drawTriangle( vector2d< double >, vector2d< double >, vector2d< double >, EPathStroke::Value ): bool

Draws a triangle onto the target image.

Parameter | Type | Default
----------|------|--------
point_a | irr::core::vector2d< double >
point_b | irr::core::vector2d< double >
point_c | irr::core::vector2d< double >
path_stroke_type | EPathStroke::Value | EPathStroke::NORMAL

---

#### drawRectangle( vector2d< double >, vector2d< double >, EPathStroke::Value ): bool

Draws a rectangle onto the target image.

Parameter | Type | Default
----------|------|--------
upper_left_corner | irr::core::vector2d< double >
lower_right_corner | irr::core::vector2d< double >
path_stroke_type | EPathStroke::Value | EPathStroke::NORMAL

---

#### drawCircle( double, vector2d< double >, EPathStroke::Value ): bool

Draws a circle of the given radius and center onto the target image.

Parameter | Type | Default
----------|------|--------
radius | double
center | irr::core::vector2d< double > | irr::core::vector2d< double >(0)
path_stroke_type | EPathStroke::Value | EPathStroke::NORMAL

