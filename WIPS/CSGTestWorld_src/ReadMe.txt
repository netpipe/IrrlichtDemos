
CSG Test World, written by Greg Santucci in 2008, based on the Java CSG implementation of Danilo Balby (http://www.geocities.com/danbalby/)

This program demonstrates an algorithm for performing CSG (Constructive Solid Geometry) operations.

I found this excellent CSG implementation written in Java many months ago. Since it is very readable, and elucidates the concepts very clearly, I thought it may be worth translating this implementation into C++, so that this library can be used in C++ projects.

After months of procrastination and occasional work, the translation is completed. This is thanks to the excellently designed, crystal clear architecture of the original Java code, and the helpful comments.

Dealing with a large chunk of another programmer's source code is a very interesting excercise. There is an algorithm in the java code which implements the face cutting, but it runs all at once. If you want to figure out the reason for a particular problem, or simply want to peer into the inner workings of the algorithm, the standard line debugger is the wrong tool for the task. I found a simple and very useful technique for peering into code that runs all at once. I built a diagnostic object which allows me to take snapshots of the various geometric calculations it is performing, in order to "play it back" later, once the algorithm has finished. This was immensely helpful, and I will use this technique again. It is especially useful for reviewing algorithms that do things with geometry in 3D space.

Controls for the program:

The controls are subtly different from my other programs, to allow finer control of navigation through the 3D space.

Moving the camera with the mouse:

Left mouse + mouse move ROTATES the camera.
Holding space + holding left mouse ROTATES the camera so it points to the selected location.
Holding space + holding left mouse + mouse move ORBITS the camera around the selected location.
Holding right mouse + mouse move PANS the camera along the camera plane, anchored on the selected point.
Mouse wheel MOVES the camera forward and back, based on the distance to the selected location. This means that before using the mouse wheel, make sure the mouse is hovering over the object you are interested in, because the distance of the step is a proportion of the distance between the camera and the object the mouse is hovering over.

Controlling diagnostic tool playback:

'd' toggles rendering of the diagnostic information.
Left arrow key steps the diagnostic playback backward one step.
Right arrow key steps the diagnostic playback forward one step.
Up arrow key steps the diagnostic playback forward continuously. (~20 steps per second, 1 per frame)
Down arrow key steps the diagnostic playback backward continuously. (~20 steps per second, 1 per frame)

A note on the diagnostic tool rendering:

Naturally it is up to the programmer what is being investigated, and I found myself frequently changing what the diagnostic tool should focus on. As you try to filter the volumes of data the diagnostic tool generates, I felt I had many options with how to proceed to learn more. I could either find some fancy ways to filter the data for the information I was interested in, leaving the snapshot taking as it was, or I could look at another aspect of the algorithm by modifying what I was choosing to record from the algorithm, and how I was going to present it.

There are 4 "episodes" in the diagnostic tool playback in this program.
1) Reveal the sequence of cuts made to the faces of the cone
2) Reveal each facet indivudually and all the facets that have come before, to enable me to visually ensure that there are no duplicates, and to inspect weird constructions of the algorithm
3) and 4), same as 1) and 2), except for the cube.

Please post any comments to http://createuniverses.blogspot.com/
Or email me directly: thecodewitch@gmail.com

Enjoy!
