Irrlicht 3D Engine / Newton Physics Demo
========================================

sio2 'at' users.sourceforge.net

This is a simple demo of using the Newton Physics Engine in the Irrlicht 3D Engine. A level map, in Quake3 format, is loaded into Irrlicht (for Irrlicht to render) and the mesh defining this level is used to build a Newton "collision map" for its physics calculations. The user can move the viewing camera around the level and can press the LMB (left-mouse button) to create a new crate (box) and "throw" it into the scene. Crates interact with the level map and other existing crates based on physics calculations by the Newton physics library. A new crate is created each time the LMB is pressed; the maximum crates that can be created is limited by system memory, graphics card and cpu.

Usage
=====
WASD keys to move camera.
Left-Mouse button to throw a crate (box) into the scene.
Alt-F4 or Escape key to exit.

Requirements
============
Drivers:
    Latest DirectX 9.0b drivers [www.microsoft.com/directx],
    Latest DirectX9 drivers for your graphics hardware [from your HW vendor].

Notes
=====
This demo may require the Visual Studio 2005 Redistributable Files installed on your machine. These can be obtained from http://www.sio2.g0dsoft.com in the "Misc" section in "Downloads".

This Irrlicht build is based on Irrlicht Subversion revision 426.

Building this demo from the source provided also requires the Newton SDK version 1.53 or later:
http://www.newtondynamics.com

This demo gives the choice of running fullscreen or in a window. If running fullscreen then the demo will try and ru at the same resolution as the Windows Desktop. If the demo fails when trying to run fullscreen then retry the demo and answer 'n' (no) when asked to run fullscreen.
