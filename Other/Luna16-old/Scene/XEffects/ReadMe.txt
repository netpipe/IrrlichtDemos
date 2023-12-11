Welcome to the first Cross-Platform release of BlindSide's shader pack. 
----------------------------------------------------------------------------------
This release allows you to do shadow mapping in OpenGL and utilize the power of FBO's
in order to produce shadow maps as large as 4096x4096!

As the interface has not changed, the original docs are included in the doc folder, please refer
to them when you want to make use of this wrapper.

NOTE FOR OPENGL: Only shadows work in OpenGL, the other effects have no yet been
ported from Direct3D9. 
(Also 12 sample filtering is limited to Direct3D9, but who cares about 12 PCF, its not needed.)

A side note on shadow maps: Shadow maps are very complicated in their nature and require
fine, fiiiine tweaking in order to look good. The demo looks ok because it is set in a simple
enviroment (Infact normal projective shadows (Coming up later!)) will work fine and better
here as they will have better performance and looks, but they do not have the versitility of
shadow maps offcourse, so these things are still necessary. When integrating into your own
project, please keep in mind that many variables will effect how it looks, or whether it works
at all. The MaxShadowDistance variable is essential, and you most likely 100% will need to
modify it when adapting to your own project. It does not just specify the max distance,
but also the scaling at which the depth buffering occurs. This means that if your objects are
very spaced apart, then you must set a high value, else you will not see anything. If your objects
are very close together you must ALSO change it to a very low value, else you will ALSO not
see anything, or may see alot of garbage and artifacts. Other things to keep in mind is the
light cameras' near and far values, FOV and other settings. The shadow map is almost
entirely dependant on the properties of the light camera, so make sure to modify it using
effectHandler->getLightCamera(), you are free to do as you wish with it then.

I hope that was an insightful chapter to read, see you next time.

- Cheers