This demo has no instructions or tutorial, please see the hello world sample for more info on setting Bullet up.

This project has a pre-built version of Irrlicht from SVN, with a modification that fixes an .X file bounding box problem. 


Instructions:

Move the camera using the mouse and cursor. To add an object, hit 1-6:

1 - add crate (box)
2 - add sphere
3 - add log (cylinder)
4 - add cone
5 - add rock (convex shape)
6 - add compound of 2 small spheres (this seems v.slow with Bullet 2.55)

To remove an object, shoot it (mouse click)

This project is linked as multi-threaded, to avoid linker errors against Bullet due to multiple symbol definitions. 


