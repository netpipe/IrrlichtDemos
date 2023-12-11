
# Collect sources from this directory and all sub directories

file(GLOB_RECURSE lithaSources "*.cpp")
file(GLOB_RECURSE lithaSourcesC "*.c")


# some more include directories

include_directories(${rootDir}/source)

# here we recurse into all sub directories of the source folder, adding as include directories.
# (GLOB_RECURSE doesn't seem to work for directories)
function(include_all_subdirs currentDir)
	file(GLOB allFilesInCurrentDir "${currentDir}/*")
	foreach (foundFile ${allFilesInCurrentDir})
		if (IS_DIRECTORY ${foundFile})
			message("Adding source directory: ${foundFile}")
			include_directories(${foundFile})
			include_all_subdirs(${foundFile})
		endif (IS_DIRECTORY ${foundFile})
	endforeach(foundFile)
endfunction(include_all_subdirs)
# start the recursion
include_all_subdirs("${rootDir}/source")



# Build the library

add_library(Litha STATIC ${lithaSources} ${lithaSourcesC})

target_link_libraries(Litha Irrlicht)

if (WIN32)
	target_link_libraries(Litha ode_single OpenAL32)
endif (WIN32)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	target_link_libraries(Litha ode openal)
endif (UNIX)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	target_link_libraries(Litha ode /System/Library/Frameworks/OpenAL.framework/OpenAL /System/Library/Frameworks/OpenGL.framework/OpenGL /System/Library/Frameworks/Cocoa.framework/Cocoa /System/Library/Frameworks/Carbon.framework/Carbon /System/Library/Frameworks/IOKit.framework/IOKit)
endif (UNIX)

