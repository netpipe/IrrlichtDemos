// ********************************************************************************************************************
Newton Game Dynamic 2.0


- fixed custom joint in SDK demos had improper limits and parameters. 
- fixed vehicle were not frozzen when leaving the world box 
- fixed collison tree optimized bug, sometime creating extrange polygon on collision mesh like quake BSP.
- fixed collsion between convex objects with ratio over 10000 to one were causing memory over run of the stack.
- fixed collision tree malfuntion when adding polygons with multiple face IDs.
- fixed contac calculation were filtered out if closer than 0.034 units, now they are filtered if they are closer than 0.0001 units
- improved contac calculation for continue collision now generating better contacts set.
- improved calculation of bouyancy forces, (upto to 10 time faster).
- improved convex hull construction much faster and rubust.
- funtion NewtonAddBodyImpulse and been changed to NewtonBodyAddImpulse.
- funtion callback NewtonContactProcess now pass the colliding bodies as arguments
- funtion callback NewtonContactEnd now pass the colliding bodies as arguments
- added came is now mulicores support using opemMP
- added material can set distance tolerance between contact points.
- added function to read triangles in aabb  from a collision tree (vertex list index list format).
- added performance counters for profiling.
- added convex hull can pass a tolerance parameter for making a loose aproximation and improve performance.
- added inteface to collect infomation from collision objects
- added inteface to collect infomation from joint objects
- added linear time solver stable constraint for handling lareg scale game scenes
- added island update callback funtionality and interface
- removed NewtonGetActiveBodiesCount
- removed NewtonBodyGetGyroscopicForcesMode;
- removed NewtonWorldFreezeBody as it can be added as external force in the callback
- removed NewtonWorldUnfreezeBody as it can be added as external force in the callback 
- removed NewtonGetActiveConstraintsCount as it can be added as external force in the callback
- removed NewtonBodySetAutoactiveCallback as it can be added as external force in the callback
- removed NewtonBodyCoriolisForcesMode as it can be added as external force in the callback
- removed NewtonBodySetAutoFreeze as it can be controled form the force callback
- removed NewtonBodySetFreezeTreshold as it can be also controll for the force call back
- removed NewtonBodyGetSleepingState
- removed NewtonBodyGetAutoFreeze
- removed NewtonBodyGetFreezeTreshold



 

// ********************************************************************************************************************
Newton Game Dynamic 1.53


- fixed documentation on function NewtonBodySetMatrixRecursive, now agree with header file description.
- fixed function NewtonBodyGetForce now returns correct net force aplied by the solver to the body.
- fixed function NewtonBodyGetTorque now returns correct net torque applied by the solver to the body.
- fixed separate collision with compound collision was not working correctly.
- fixed collision tree fases are single sided for raycastin and also for contact points.
- fixed spherical surfaced objects do not jumps when crossing across chared edge of collision tree adjacent faces.
- fixed vehicle joint no longer explode due to tires speening out of control
  when the vehicle was airborne and continue torque was applid to tires.
  tire max speed is clamped at 500 m/s per secund on idle. 
- fixed sphere and polygon edge contacts were rejected when contact was exactly over the edge
  now contacs on edge are included as valid. 
- fixed NewtonConvexCollisionCalculateInertialMatrix on compound collision agregates was using 
  a matrix from a last update instead of the normal primitive offset.
- fixed contact point between sphere collision were calculatedted incorrecly, they could cause big 
  misbehavior on games likes pool. (special thanks to user LtJax, for reporting teh bug privaltly) 
- Improvement tree collidder is not longer based on Bounding Boxes hierarkies for polygoin querie, 
  a proprietarty faster algorithm with smaller data structure is used instead. 
- API change function NewtonWorldRayCast now take an extra prefilter parameter for smarter and faster filtering.
- SDK raycast now show normals and ray at ray hit position on static geometries.
- Added Linux build now recognize SSE instrictuon set if present in eh system.
- Added Mac build now worlks with universal binary, it also make use of altivec and SEE, if present in the system
- Mac build is comaptible with G3/G4/G5 runing OS10
- modification linear and angular damping, are campled between 0.0 and 1.0 (allowing for not damping forces in simulations).



// ********************************************************************************************************************
Newton Game Dynamic 1.52

-license has been modified to include the specific engine release.
-license clause specifying the Newton logo should be on any splash screen has been change to say the logo should be on any credit screen.
-changed lower limit of mass to be considered static body form 0.01 to 0.001
-slight overall engine optimization be replacing some of the standard math function with custom inline versions.
-fixed SetMinimunFrameRate now clamp fps bewteen 60 and 1000 frame per secund. 
-fixed convex hull prototype did not have const argument in front of imput arguments.
-fixed documentation on NewtonBodyGetSleepingState and NewtonBodyGetAutoFreeze was incorrect.
-fixed buyancy force calculation now works with transform modifiers. 
-added funtion NewtonWorldForEachBodyInAABBDo


// ********************************************************************************************************************
Newton Game Dynamic 1.51

-Fixed vehicles tires friction was too weak.
-Fixed Vehicle collision with other dynamic bodies was too hard.
-Fixed ray cast normal in some primitives were inconsistent or un-rotated.
-Fixed debug display of comvex modifier wih non identity matrix ofsset were displaying incerrectly.
-Fixed setting center of mass after the body matrix was not shifting the initial matrix position of the body.
-Fixed continues collision was ignoring collision objects that where touching but not colliding. Allowing for fast moving body pass trough each other’s. 
-Fixed trivial rejection of convex primitives incorrectly rejecting collision with large flat convex polygon, leading to tire lost of traction.
-Fixed bug skipping clean contact generation across flat polygons between collision tree and convex primitives.
-Added marking of flat and concave edges on user defined meshes (see height field tutorial)
-Removed compiler option /Gs10000000 from visual studio (crash in some windows system).
-Removed Virtual memory used because horrendous vs8 does not handle virtual memory correctly.
 (this may low down the collision tree generation of windows systems)


// ********************************************************************************************************************
Newton Game Dynamic 1.5

- Improvement simple vehicle joint has been redesigned for easy implementation.
- Improvement optimized contact calculation for, capsules, cylinders and cones.
- Improvement tree optimizer now generates nicer optimized convex polygonal meshes.
- Improvement bilateral joints constraint are much more stable and less prompt to blow up.
- Improvement collision tree filter contact across coplanar and concave faces. 
- Improvement implemented new unique algorithm for contact selection, now does not have upper bound in contacts calculation.
- Fixed bug ray cast on convex hulls geometry is pixel accurate now.
- Fixed bug unstable contact point calculation for capsules collision.
- Fixed bug ray cast on boxes on rare occasion producing wrong intersection.
- Fixed collision tree optimizer much faster at building optimized collision.   
- Fixed bug crash when adding NULL collision geometry to a compound aggregates.
- Fixed bug convex hull on point clouds of 4 vertices not longer crash the engine.
- Fixed bug ray casting on all collision geometries not longer is one frame behind.
- Fixed bug debug displayed collision primitive were rendered one frame behind the update.
- Fixed bug in collision tree optimizer adding multiple triangle layers when multiple ID where used.
- Fixed bug in convex collision primitives not been cached properly (very important for memory saving)
- Fixed bug with ray cast occasionally missing collision when called from outside of a Newton Update call.
- Fixed random crash bug due to pointers alias between the solver and the collision system charing the same memory buffer.  
- Fixed collision tree optimizer not longer run out of memory for reasonable size memory meshes (2 million triangles or larger)  
- API all dll version are now mutithreaded.
- API vehicle joint function NewtonVehicleBalanceTires has been deprecated.
- API change function NewtonWorldCollide is changed to NewtonCollisionCollide.
- API change function NewtonWorldCollide is changed to NewtonCollisionCollide.
- API change function tire ID are pointers instead of unsigned integers.
- API change funtion Buoyancy callback funtion now report the collision ID of the current geometry
- API change function NewtonCreateConvexHull not longer take a matrix offset as parameter.
- API change function NewtonConvexCollisionCalculateVolume is changed to NewtonConvexCollisionCalculateVolume
- New feature added calculation of the closest point and separating vector from a point to convex geometries.
- New feature added calculation of the closest point and separating vector between two convex geometries.
- New feature the ability to offset the center of mass of a rigid body.
- New feature custom joint now can act like spring/damper system.
- New feature custom joint jacobina matrix is fully exposed to teh application.
- New feature implementation of continue collision base of translational speed.
- New feature added function to calculate inertia matrix of a convex collision objects.


// ********************************************************************************************************************
Newton Game Dynamic 1.32a

- engine is ported to linux on PC
- create unix make files for all SDK tutorials
- all SDK tutorial are conveted to use GLUT, intead of specialized OS calls.
 

// ********************************************************************************************************************
Newton Game Dynamic 1.32

- made SDK compatible with Win32 and Mac OS X 
- added single and double precision libraies. (pc only for now)
  for double precision mode the app should define __USE_DOUBLE_PRECISION__ 
  before including Newton.h, and also link to any of the double precision libraries.
- fixed bug when calculating isotropic friction correction.
- fixed strange bug that made joints and contacts acting too soft.
- fixed crash bug on collision tree with degenerated triangles (slivers).
- significant improvement of collision tree builder optimizer, now much faster.
- refactoring all tutorials samples code (added the tool box some utility functions)
- remove the P4 optimization option from the comipler, library should now work on celerons.
- fixed crash bug when calculating over 128 contacts between a convex primitive and a tree mesh.
- added the dry rolling friction to the custom joint library, good for ball games like pool, or pinballs etc.


// ********************************************************************************************************************
Newton Game Dynamic beta 1.31

- improved character controller. 
- improved coulomb kinetic friction model.
- added support for user defined custom joints.
- added global adaptive coumlomb friction model.
- re-arranged sample tutorials to add the custom joint library.
- fixed ray casting cone primitives now return the correct intersection.
- added solver configuration mode, allowing for speed vs acuracy setups.
- fixed access violation crash bug when building very small convex hulls.
- fixed collision tree modifier now inhered collision ID from the child geometry.
- fixed collision primitive ray cast now return the correct normal at the intersection point
- reduced minimum viscous damping coefficient on rigid body from 0,1 to 1.0e-4, default still 0.1
- fixed bug with collision tree allowing them to act like dynamics body if non zero mass was assigned.
- fixed Collision bug. When the origin of sphere was inside a box collision returned not contact points.
- fixed un-initialized Jacobian when setting angular constraint row on bodies attached to world by joints.
- replaced vehicle tires integrator from RK2 to implicit, this allow for more tight and easier to setup suspensions.
- fixed bug with slider and corkscrew joint jacobian using the wrong direction vector, causing the objects to wobble.
- fixed ray cast with collision tree sometime reporting false collision when a ray was close to a very small polygon.


// ********************************************************************************************************************
Newton Game Dynamic beta 1.30

- lots of more features
- vehicle joint 
- added library version.
- improve character controller (in progress)
- added more implicit collision primitives
- added collision primitive deformation matrix
- added arbitrary convex hull collision primitive
- Ragdoll joint now can take an arbitrary collision primitive
- added continues collision mode for high speed objects (in progress)
- fixed spurious crash bug in collision tree optimizer
- added ray cast function 
- function add impulse no longer take the world as argument
- function callback onbodyLeaveworld does not take the world as argument
- fixed bug in utility callback tracking bodies actives in the scene
- several internal bug fixes.
- move to visual studio net 2003
- To link with libraries you will need to declare _NEWTON_USE_LIB before you include newton.h in your project
- if you are linking visual with VS 6. You will need to declare the following in any of you C or C++ modules
	#if (_MSC_VER >= 1300) 
		//VC7 or later, building with pre-VC7 runtime libraries
		//defined by VC6 C libs
		extern "C" long _ftol (double); 
		extern "C" long _ftol2( double dblSource ) 
		{ 
			return _ftol( dblSource ); 
		}
	#endif

// ********************************************************************************************************************
Newton Game Dynamic beta 1.23

- fixed bug with up vector joint in some case producing random crashes  
- added notify application when a body is about to be activate or deactivated
- fixed another bug in collision tree causing to drop face is some cases
- fixed camera stouter one frame behind the player in all tutorials.    

// ********************************************************************************************************************
Newton Game Dynamic beta 1.22

- Fixed Collision tree optimizer crash when large mesh with lot of non manifold faces
- Improve dynamics solver much faster especially when handling large pile of objects 
- Increase max number objects in a single pile to 512, from 256 (use about 200k more of memory, no performance degradation)
- Combined common joint functionality in to generic function (collision state, user data, destruction)
- Added up vector constraint, this is useful to control first and third person characters.
- Added corkscrew constraint.
- Added collision tree tutorial A
- Added material tutorial
- Added up character tutorial using vector (advance tutorial
- Added using joints tutorial 
- Improved existing tutorials
- Fixed bug on existing tutorials that was causing bodies to spin the wrong direction making it looks like the physics solution was inacurate


// ********************************************************************************************************************
Newton Game Dynamic beta 1.21

- Fixed engine hangs in a infinite loop when calling NewtonDestroBody
- Fixed pointer to a vector of 3 dgFloat32 know can be literally an array of tree floats and not the first three element of a 4d vector


// ********************************************************************************************************************
Newton Game Dynamic beta 1.20

-Fixed bodies can be destroy from any call back or at any time
-Fixed joints can be destroy from any call back or at any time
-Remove DX9 tutorials and replaced with open gl tutorial
-Added Joints can disable collision of the linked bodies
-Added Bodies can disable collision with other bodies in the same chain or hierarchy 
-Added Body iterator function for debugging and for other application purpose
-Added Collision Iterator function for debugging and other purpose
-Added context parameter to the buoyancy force call back
-Added Inertia tensor is clipped to between the interval [0.01 * mass - 100.0 * mass], 
 this prevent extreme coriolis forces behavior that could lead to explosion.	
-Added full ball and socket joint with user feedback
-Added full hinge joint with user feedback
-Added full slider joint with user feedback
-Added full Rag doll constraint 



// ********************************************************************************************************************
Newton Game Dynamic beta 1.10

-Remove time limit from beta (this is still a beta version, no responsible for mal functions if you use it in an final project)
-Fixed bug in optimize Collision tree. When there were too many degenerated faces.
-Fixed bug in optimize Collision tree. When the surface does not conform a manifold (like Quake3 BSPs)
-Fixed bug: filter polygon with zero area before passing then to tree collision

// ********************************************************************************************************************
Newton Game Dynamic beta 1.00

This is a beta demo of Newton Game dynamics. the simulation runs for 10 minutes of is launch and the it shut down.
This is done in order to prevent user for trying to uses an incomplete product in a final product.
This time limit will be removed for the beta in the final release.

-Fixed jitter bug on non Intel CPU
-Fixed jitter on slow CPU
-Better function documentation
-Fixed bug when passing zero mass to a rigid body


Enjoy
Newton



