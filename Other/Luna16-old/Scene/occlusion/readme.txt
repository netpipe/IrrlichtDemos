
Author : mccauley404@gmail.com Bertrand M.


This is something that has probably been done before but was fun to implement.

what is it
**********

	Irrlicht has already frustrum culling for the camera. However performances
can be much improved with another culling method : occlusion culling.

Basically : dont render polygons that can't be seen.

After having looked at the VIS process, it seemed to me that the bsp format is
great, but tends to be generic and is loosing performances there.
However bsp is nice because there are good intuitive editors around...

This method is not using bsp at all. But the scene is stored in a binary file,
like bsp does.


Implementation
**************

	So, the map is divided in several virtual areas.
Each area (leaf) is a bloc defined with just 2 points and it holds a list of models.

The idea is that when you are not inside or in front of a room, then you don't
have to render the polygons located in other rooms that can't be seen. This is
how bsp works. Render leaves that can be seen from the current leaf you're in.

Here it shows only one leaf at a time. Basic : when you leave a room, load the
next one and unload the previous one.

while (Game loops)
	check every 500 milliseconds
	{
		leaf = getCurrentLeaf(); //what leaf am I in?
		if (leaf != currentLeaf)
		{
			unload(currentLeaf);
			load (currentleaf + 1);
			currentLeaf = leaf;
		}
		renderFrame();
	}
}


The test Scene
**************

	A skydome as half-sphere with a special gradient texture (generated with
code from an old day&night cycle project) ...

A first model is used for static geometry that is always drawn (350 polies),
models/etage.b3d, it is in an octree and consists of three empty rooms, plus
the ground is marked with 256X256 units pink squares (in Max and irrlicht) and
this about how I determined the position of the 3 leaves.

Then 3 models, one for each room.

Room1 : a bedroom. 11518 triangles.

Room2 : High poly sofa. 39532 triangles. Looks nice in Max, not that much here ...

Room3 : a table and 4 chairs. 26436 triangles.

Running the demo

	without occlusion culling : 77892 triangles, 205 FPS
	with occlusion culling : 406 triangles, 956 FPS
	How much do you get?

[IMG]http://i44.tinypic.com/ogxawi.jpg[/IMG]
[IMG]http://i44.tinypic.com/2hwf94y.jpg[/IMG]


Conclusion
**********

	Frustrum culling + occlusion culling = great gain of performance


Todo
****
	* Create BXP file easily with a basic editor (if someone is interested ...)
    * Add light info into bxp file, as well as fog (and whole entity list)
    * Add special effects and lights, stopped on demand!
    * Add LOD, should be optimal for the equation then
	* Release the code


