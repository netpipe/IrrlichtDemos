License/credits
---------------
Grass Node by G Davidson Feb 2006
Originally based on Irrlicht's particle system source code by Nikolaus Gebhardt
Uses Klasker's IWindGenerator

Intro
-----
The grass patch node is a small square of turf on your terrain.
The idea is that you make some kind of a manager to spawn and kill
them as you walk around.
The demo uses a 10x10 grid of nodes, which store 3000 blades of 
grass each = overkill + memory hog. I plan on making a block
manager that deals with creating and destroying lots of layers
(terrain, grass, trees, world blocks), but I'm in no hurry to do
this, so you'll have to make your own grass tile manager :P


Instructions for use
--------------------
This is the boring bit. I'll update this some other time :P


Performance options
-------------------
Theres a lot to gain (and lose) by tweaking the performance options.

setDrawDistance
  This sets the distance to draw blades at. When a patch is in range, 
blades of grass within that square must be checked for distance from
the camera - this is the reason the whole terrain isn't one big 
patch.

setMaxDensity
  This sets the maximum number of blades of grass that can be drawn 
within the patch. It's probably a good idea to keep this set at the 
max - the number made in create(), as its a waste of memory otherwise. 
Remember that grass particles are currently uploaded to the card every 
frame, the more you send the more it hurts.

setRefreshDelay
  This function sets how often the grass is updated. The default is
30ms, any more looks jerky. If might be an idea to tune this depending on
the distance of the patch to the camera. 
If you're using IrrSpintz, hardware mesh buffers (sh|c|w)ould give a 
significant performance/drawdistance boost here.

setWindRes
  This function sets how many wind sensors wide the wind grid is. The 
more there are, the nicer the wind effect, but the more it hurts 
your cpu. Default is a 5x5 grid. The manager could change this as you 
approach and retreat from a patch.



To Do
-----
Stick grass legs to terrain instead of middle = wider grass :)
Binary interpolate wind position rather than linear interpolate
Account for terrain position and rotation.


changelog
---------

0.3.2 - 10th Nov 2007
Again no changes, just update for 1.4

0.3.1 - 14th Mar 2007
No real changes, just updated it to work with Irrlicht 1.3

0.3   - 18th Feb 2006
shorter grass is stiffer, longer grass is more flexible
renamed get/setMaxFPS to get/setRefreshDelay.. makes more sense i think
demo code: changes wind speed and regularity (at last!)
demo code: set/getRefreshDelay and wind resolution for performance


inherits IBlockWorldNode


0.2.5 - 17th Feb 2006
Grass better aligned to terrain floor - thanks Xaron!
Wind is now bilinear interpolated too

0.2.4 - 13th Feb 2006
constrained to heightmap size
grass height and density controlled by grass map image

0.2.3
grass shadow map
colour difference between top and bottom of blades
wind is now interpolated between grid
more settings functions
changed the create function so uncommon blades have highest priority

0.2.2
speed increase! grass and wind now only update every 30ms
wind is rendered to a grid

0.2.1
new wind... thank you Klasker :)

0.2
faster grass generation -  files are now optional :)

0.1.3
bounding box check
ditched billboards, now blades have real positions
fixed distance code

0.1.2
keyboard input
fall off, changed scale

0.1
Initial version
