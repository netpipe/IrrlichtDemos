#Welcome to the XEffects Reloaded shader pack. 


Change Log:

####V 1.4

Uploaded to github (After like 7 years lol) and fixed the shader variable issue.

####V 1.3
 
Added VSM shadows, screen space water and ray tracing examples.

Added some improved post-processing functionality including the ability to set custom shader constants and pre/post render callbacks.

####V 1.2.1
 
Transparent objects don't have to be affected by the shading anymore.

####V 1.2:

This new release features per-shadow light resolution settings, and the ability to change the resolution at runtime.
Shadow map resolutions are cached and if multiple lights use the same resolution only one map is created.

These added features should be used with performance in mind. Now you can change your shadow map resolution through
in-game settings, or assign lower shadow map resolutions to far away lights (Eg. 256x256) and switch to a higher one
when the light comes into close view (Eg. 1024x1024). There is also a newly added option to clear all of the current
shadow maps, allowing you to free prescious video ram. The shadow maps are automatically created again by XEffects
at runtime, when they are needed.

Another new feature is the soft spot light cone, softening the edges of the light cone if you intend to use the shadow
lights as actual spotlights/flashlights.

Also check out the new SSAO example. A newly included shader that calculates ambient occlusion on the fly!

####V 1.0:

This signifies a new revision in XEffects, with alot of changes, resulting in many new features and improved robustness. The amount of lights casting a shadow is now theoretically unlimited, even on older computers. The only limitation is performance, as each light is performed in it's own seperate additive pass.

Additive lighting is an exciting feature to have in a graphics program, as it allows very realistic simulation of lights and shadows. Normally, for example in Irrlicht's stencil shadows, one would set the color of the shadow, and the shadow will simple affect the area it covers with that color. Obviously, this is not a very realistic representation of lighting. In additive lighting, one sets the light color, and this color will cover everything except that light's
shadowed area, it will even affect other light's shadowed areas! Therefore, if you place a blue light on one side of a pillar and a red light on the other,
the red light will cast a blue shadow (Because only the blue light is lighting that area), and the blue light will cast a red shadow. This is of course,
how lighting works in real life, and can add a whole lot of depth and realism to lighting in a graphics application. The lighting is additive, so the more
lights the brighter everything will be.

Please refer to example.cpp for an introductory tutorial on using the XEffects wrapper.

Oh yeah, the shaders can be found in the "./Bin/shaders" directory, remember to copy the files to your own projects directories.

A side note on shadow maps: Shadow maps can be complicated in nature and require fine, fiiiine tweaking in order to look good. When integrating into your own project, please keep in mind that many variables will effect how it looks, or whether it works at all. The far value variable is essential, and you most likely 100% will need to modify it when adapting to your own project. It does not just specify the max distance, but also the scaling at which the depth buffering occurs. This means that if your objects are very spaced apart, then you must set a high value, else you will not see anything. If your objects are very close together you must ALSO change it to a very low value, else you will ALSO not see anything, or may see alot of garbage and artifacts. Other things to keep in mind is the light's near value and FOV.

I hope that was an insightful chapter to read, see you next time.

Cheers,

####License:

  The Irrlicht Engine License
  ---

  Copyright (C) 2007-2015 Ahmed Hilali

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgement in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be clearly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
  
  
[![Bitdeli Badge](https://d2weczhvl823v0.cloudfront.net/monstrobishi/xeffects/trend.png)](https://bitdeli.com/free "Bitdeli Badge")