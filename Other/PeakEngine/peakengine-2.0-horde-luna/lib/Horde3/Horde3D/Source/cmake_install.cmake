# Install script for directory: /run/media/netpipe/df7f53ef-2a98-4562-a498-7da578dab660/games/Luna/IrrlichtDemos-git/Other/PeakEngine/peakengine-2.0-horde-luna/lib/Horde3/Horde3D/Source

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/run/media/netpipe/df7f53ef-2a98-4562-a498-7da578dab660/games/Luna/IrrlichtDemos-git/Other/PeakEngine/peakengine-2.0-horde-luna/lib/Horde3/Horde3D/Source/Horde3DEngine/cmake_install.cmake")
  include("/run/media/netpipe/df7f53ef-2a98-4562-a498-7da578dab660/games/Luna/IrrlichtDemos-git/Other/PeakEngine/peakengine-2.0-horde-luna/lib/Horde3/Horde3D/Source/Horde3DUtils/cmake_install.cmake")
  include("/run/media/netpipe/df7f53ef-2a98-4562-a498-7da578dab660/games/Luna/IrrlichtDemos-git/Other/PeakEngine/peakengine-2.0-horde-luna/lib/Horde3/Horde3D/Source/ColladaConverter/cmake_install.cmake")

endif()

