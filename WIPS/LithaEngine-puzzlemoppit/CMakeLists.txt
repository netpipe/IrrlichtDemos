cmake_minimum_required(VERSION 2.6)

#set(CMAKE_CXX_FLAGS "-Wall")
#set(CMAKE_CXX_FLAGS_DEBUG "-g") # for line numbers
#set(CMAKE_CXX_FLAGS_RELEASE "")

#set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE Release)


if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(CMAKE_CXX_FLAGS "-arch i386")
endif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")


project(LithaEngine)


if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	# On Linux, force it to search the exe directory for .so files.
	# (Slackware at least was not searching the exe directory)
	set(CMAKE_INSTALL_RPATH "$ORIGIN")
	set(CMAKE_BUILD_WITH_INSTALL_RPATH true)
endif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")


# find the absolute litha engine root directory
set(rootDir ${CMAKE_HOME_DIRECTORY})


########## COMPILE TARGETS #######

# specify the directory to output libraries and executables

if (WIN32)
	set(lithaTargetName "Win32-gcc")
	set(irrlichtLibDir "Win32-gcc")
	set(exeExtension ".exe")
endif (WIN32)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(lithaTargetName "Linux")
	set(irrlichtLibDir "Linux")
	set(exeExtension "")
endif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(lithaTargetName "MacOSX")
	set(irrlichtLibDir "MacOSX")
	set(exeExtension "")
endif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")



########## COMMON LIBS ###########

# (things required by both litha engine and individual projects)
# (actually some of these probably aren't required by both...)

include_directories(${rootDir} ${rootDir}/include ${rootDir}/thirdparty/irrlicht-1.6.1/include)

link_directories(${rootDir}/thirdparty/irrlicht-1.6.1/lib/${irrlichtLibDir})



########## LITHA ENGINE CONFIG ########

# Litha engine static library output directory
set(LIBRARY_OUTPUT_PATH ${rootDir}/lib/${lithaTargetName})

# Compile litha engine...
# (specific libraries needed only by Litha engine are set in this directory's CMakeLists.txt)
add_subdirectory(source)



########### PROJECTS CONFIG ############

# Project executable output path
set(EXECUTABLE_OUTPUT_PATH ${rootDir}/bin)

# Link to Litha Engine
link_directories(${rootDir}/lib/${lithaTargetName})

# Project libraries
set(projectLibs Litha Irrlicht)

# Project properties
if (exeExtension)
	set(projectProperties PROPERTIES SUFFIX ${exeExtension})
endif (exeExtension)



########### PLATFORM SPECIFIC PROJECTS CONFIG ############

# Hide console window in Windows.
if (WIN32)
	set(projectProperties ${projectProperties} WIN32_EXECUTABLE true)
endif (WIN32)

# Linux needs GL for some reason.
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(projectLibs ${projectLibs} GL Xxf86vm Xrandr)
endif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")



########### BUILD ALL PROJECTS ############
add_subdirectory(projects)






