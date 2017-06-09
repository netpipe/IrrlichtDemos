FunCollision -- Collision detection and response system -- readme.txt
__________________________________________________________________________

  Content:

  1. Overview
  2. The basics
  3. How to start?
  4. Performance tips
  5. Release notes
  6. License
  7. Contact

__________________________________________________________________________
1. Overview
__________________________________________________________________________

- FunCollision is a collision detection and response system for sphere and
  axis-aligned ellipsoid characters VS each other and static environment.
  It was created for Irrlicht, for C++. Designed to be really easy to use.


- It offers continuous collision detection. 
  No matter how fast your entities are, they won't pass any objects or 
  entities; also they won't sink into the geomery or stick into it. It 
  also means that it's a bit slower than dicrete collision detection, but
  the overall look-and-feel is much better. It's 99% jittering-free, the
  remaining 1% is caused by the maintanence vector.


- Collision response.
  Characters gets pulled out of each other or from the static geometry.
  Characters can push each other, what is affected by their mass and speed;
  they also can bounce. Gravity affects the entities as a constant force.
  There's a base friction currently only on the whole system level.

  
- Gives you framerate independent smooth motion.
  Due to syncronization, your characters will act the same way on every
  computer. And due to friction, they'll have a smooth movement, like what
  the commercial games has. Entities are moved by forces and impulses.

__________________________________________________________________________
2. The basics
__________________________________________________________________________

- Manager.
  You should put your objects, entities, selectors and samplers into the 
  manager to store, handle, make them collide, and update them. It's also
  responsible for syncronisation, and more.


- Object.
  It's currently only for static objects. It's attached to a scene node.
  It also should have a selector which stores it's geometry. It's selector
  can be shared among objects so you can have e.g. 20 barrels while they
  have only one selector. An object is like a scene node, a selector is like
  a meshbuffer or a mesh. Objects can be activated or deactivated.


- Entity.
  It's a dynamic (moveing) instance for your character model to put into.
  They can collide with each other, and with the objects. You can bind your
  scene node to and entity, so that it's position will be automatically
  updated. It should have a sampler, which is for describing the entity's
  shape and material (say: behaviour). Samplers can be shared among entities,
  so that you can have e.g. 10 zombies which shares only one sampler.
  Entities can be activated or deactivated.


- Selector.
  Selectors are usually polygon soups (meshbuffers, meshes) describing an
  object's geometry. A basic geometry clean-up is available. Selectors can
  be shared among objects. There are 4 types of selectors currently:
  
  1) Simple selector. For really low-poly models, e.g. a barrel.
     It's slow, it has no acceleration. You should avoid using it if there
     are other alternatives.

  2) Bounding box selector. Mostly for smaller models, if their geometry
     doesn't really matter, or if it has a box-like shape, like a fridge.
     Since it's only a box, it's fast.

  3) Octree selector. The only alternative for large or detailed objects,
     like a building, or a level. You can control it's detail level by 
     setting the number of triangles packed together.
     This selector is fast (...it depends), because it's accelerated with
     an octree structure.

  4) Terrain selector. Guess what: it's for terrains. :) Any type of, as long
     as it's being built from a meshbuffer, and if this meshbuffer has axis
     aligned data and the terrain heights are y-axis based. It also doesn't
     matter if the quads were triangulated clockwise or anti-clockwise or 
     mixed. It's really fast, accelerated by a 2D grid stucture.
    
  
- Sampler.
  They're for describing an entity's shape and material. They can be shared 
  among entities. They're usually binded to scene nodes, so that they can 
  automatically update the positions of the scene nodes. Samplers have mass, 
  scale, origin, bouncyness, etc. properties. There are 2 type of them:
  Sphere sampler, and axis-aligned ellipsoid sampler (so that you cannot
  rotate them). 


- Contacts.
  You can access the collision contacts (results) via entities. These 
  contacts contains a lot of information about the collision, such as what 
  did the entity hit, where did the impact happen, what was the direction,
  which triangle did it collide into, etc.


- Ray casting.
  For e.g. selecting objects/entities or fireing bullets. An exception can be
  set to avoid self-collision, i.e. if you're a character who's shooting 
  bullets, not to shoot yourself. :))
  Ray casting is fast, because the ray is divided into segments.

__________________________________________________________________________
3. How to start?
__________________________________________________________________________

  1) If you're like me, first try out the demos. The binaries are located 
     in the "FunCollision\bin" folder. They cover almost all the things you
     can do with FunCollision. What's not in them, it cannot be done (yet!).


  2) Next, run Code::Blocks or your preferred IDE and open up a tutorial.
     The tutorials are in the "FunCollision\" folder.

     If you want to compile, first go to [Project/Build Options] menu because
     you might want to change the following things:

     In [Linker Settings] tab change the line:
     "..\..\..\..\irrlicht-1.6\IrrDX_v1_6\libIrrlicht.a"
     to where your Irrlicht lib is located.

     In [Search Directories/Compiler] tab change the lines:
     "..\..\..\MinGW\include"
     "..\..\..\..\irrlicht-1.6\include"
     "..\..\..\..\irrlicht-1.6\source"
     to where your MinGW, Irrlicht include and source folders are located.

     Don't forget to put the following line into your .cpp or .h file:
       include "FunCollision.h"
     
     Now you should be able to compile the tutorials.

     Play around with them until you're familiar with all the basic things 
     and possibilities.

     >>>>> Does anybody want to create project files for other IDEs? If so,
     >>>>> please contact me. (See my e-mail address at the bottom.)


  3) If you're setting up a new project, and you want to use FunCollision,
     go to [Project/Build Options] menu, 
     click on [Search Directories/Compiler] tab,
     and add the folder where the "FunCollision\source" is located.

     Then, go to [Project/Add files recursively] menu,
     and add the folder where the "FunCollision\source" is located.

     Don't forget to put the following line into your .cpp or .h file:
       include "FunCollision.h"

     Now you have added FunCollision to your project, and if you have 
     already added Irrlicht, you should be able to use it.


  4) For more information please read the tutorials and the sources, they're
     very well commented; or visit the Irrlicht forums.

__________________________________________________________________________
4. Performance tips
__________________________________________________________________________

- Use you own collision models, if it's possible, not the one you're gonna
  render. It could more-or-less keep the same shape as the render-model has,
  but it should have way less polygons.


- Not even in commercial games have all the objects collision shapes: think
  of some bricks over the floor or other garbage; think about the books on a
  book-shelf (only the book-shelf should have e.g. a bounding box selector);
  or think about some plates and glasses on a table, etc. They should have 
  no selectors at all.


- Use bounding box selector where it's possible! They're cheap. :)


- Trees -- only the trunk or maybe also the main branches should have a 
  collision model. Why? Because it would be really strange otherwise.


- Don't forget that you can put together more meshes into one object when 
  adding meshes or meshbuffers or triangles into a selector by setting their
  matrices. It comes handy if these are different, small meshes, close to each
  other, or whatever reason.

__________________________________________________________________________
5. Release notes
__________________________________________________________________________

  Information about the current release in changes.txt.  

  Please note that some of the textures, and 3D models are copyright
  by their authors and not covered by the FunCollision license.

__________________________________________________________________________
6. License
__________________________________________________________________________

  FunCollision -- Collision detection and response system

  Created by Peter Szollosi, 2009-2010.

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
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

__________________________________________________________________________
7. Contact
__________________________________________________________________________

- Should you have any problems, questions, suggestions, or patches (!!!),
  don't hesitate to tell me on the Irrlicht forums or contact me at:

  pi24.contact@gmail.com


- Visit my homepage for updates at:

  http://pport.hostei.com


  Have fun! :)

  Cheers,
  PI
