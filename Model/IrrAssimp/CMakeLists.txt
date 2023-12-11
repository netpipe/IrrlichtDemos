cmake_minimum_required(VERSION 2.6)
project("IrrAssimp Demo")

include_directories(${irrlicht_include_dir})
link_directories(${irrlicht_lib_dir})

include_directories(${assimp_include_dir})
link_directories(${assimp_lib_dir})


add_executable(
        IrrAssimpDemo
        IrrAssimp/IrrAssimp.cpp
        IrrAssimp/IrrAssimp.h
        IrrAssimp/IrrAssimpExport.cpp
        IrrAssimp/IrrAssimpExport.h
        IrrAssimp/IrrAssimpImport.cpp
        IrrAssimp/IrrAssimpImport.h
		IrrAssimp/IrrAssimpUtils.cpp
		IrrAssimp/IrrAssimpUtils.h
		main.cpp
)


target_link_libraries(
        IrrAssimpDemo
        ${irrlicht_lib_name}
		${assimp_lib_name}
)