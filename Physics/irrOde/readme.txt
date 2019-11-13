Hello and welcome to "IrrODE", an ODE wrapper for Irrlicht. Here are some general information for you:

- the wrapper is compiled against Irrlicht 1.7.1 (the release version) and ODE 0.11
- all binaries of the release (except for the IrrOdEdit plugin) are compiled using Code::Blocks with gcc, so you have to replace the ODE and Irrlicht DLL by VisualC++ versions if you want to use these
- the wrapper is licensed under the Irrlicht license, so you can do whatever you want to with it
- the complete project is still work-in-progress, so API changes might appear (though I hope I can avoid it)

The directory layout:

- codeblocks: the Code::Blocks project and workspace
- data: all data for the demo applications
- data/IrrOdEdit: data for the IrrOdEdit plugin. See the tutorial on what to do with these files for installation
- doxygen: a doxygen project and documentation
- include: all include files for IrrOde and the demo applications
- source: all source files for IrrOde and the demo applications
- vc2008: the VC2008 project files
- vc2005: the VC2005 project files provided by user "Zurzaza" from the english Irrlicht forum


Thanks for contributions:

buhatkj (from Irrlicht forum) for helping me with the CIrrOdeGeomHeightfield class
tecan (from Irrlicht forum) for the Linux adaption of the Code::Blocks projects

Remark: from my development I have found out that the current ODE version 0.11.1 release doesn't work properly
with the gcc compiler. If you use the debug version everything works fine. I suggest using the statically linked
version so that gcc removes the debug symbols when compiling. This brings the filesize down quite a bit.