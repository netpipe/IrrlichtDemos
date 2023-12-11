# IrrAssimp
Load 3D models in Irrlicht with the Assimp library


### Requirements
To use it, you should have a working project with :
- Irrlicht (should work with most of the versions)
- Assimp (should work with most of the versions)
- The files of the subfolder "IrrAssimp" included in your project.


### Usage
The easiest way to import your model is to use IrrAssimp as a mesh loader, you can do this easily by adding a simple line in your code :  
``` cpp
smgr->addExternalMeshLoader(new IrrAssimpImport(smgr));
```

If you want to have a better control in which loader you want to use to load a specific model (the built-in loaders of Irrlicht or Assimp), you can also load your model with assimp like this :<br/>
``` cpp
IrrAssimp assimp(smgr);
IAnimatedMesh* mesh = assimp.getMesh("Media/dwarf.x");
```
The export is similar :  
``` cpp
assimp.exportMesh(mesh, "obj", "Media/export.obj");
```

There is an example program to show how to use these features in main.cpp.  
You can build it with CMake by adding some entries to your CMakeCache :

```
irrlicht_include_dir
irrlicht_lib_dir
irrlicht_lib_name
assimp_include_dir
assimp_lib_dir
assimp_lib_name
```


### Known issues
The export of animated meshes isn't supported yet.  
Due to the difficulty to adapt the data given by Assimp to the animation system of Irrlicht, the animation support isn't perfect and some models can have problems with it.
