cmake_minimum_required(VERSION 2.8.11)
project(Vehicle)

set(TARGET_NAME helloworld)

set(TARGET_VERSION_MAJOR 0)
set(TARGET_VERSION_MINOR 1)
set(TARGET_VERSION_PATCH 0)

# Enable debug symbols by default
if(CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

# Making the compiler as strict as possible
set(CMAKE_CXX_FLAGS " -std=c++14")
set(CMAKE_CXX_FLAGS_DEBUG " -std=c++14 -g -Wall -O0")
set(CMAKE_CXX_FLAGS_RELEASE " -std=c++14 -O3")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO " -std=c++14 -g -Wall -O3")
set(CMAKE_CXX_FLAGS_MINSIZEREL " -std=c++14 -Os")

# Locate and require Irrlicht
find_package(Irrlicht REQUIRED)
find_package(Bullet REQUIRED)

# Location of source files
set(SRCS
    Main.cpp
    VehicleExample.cpp
    ExampleFramework.cpp
)
set(HDRS
    VehicleExample.hpp
    ExampleFramework.hpp
)

# Specify include directories
include_directories(
    ${IRRLICHT_INCLUDE_DIR}
    ${BULLET_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/build/include/
)

# Create the binary
add_executable(${TARGET_NAME} ${SRCS} ${HDRS})

# Link libraries
target_link_libraries(${TARGET_NAME}
    ${IRRLICHT_LIBRARY}
    ${BULLET_LIBRARIES}
)

#set(CMAKE_EXE_LINKER_FLAGS " -static-libgcc -static-libstdc++")
target_link_libraries(${TARGET_NAME}
    ${CMAKE_SOURCE_DIR}/build/libs/libirrBullet.a
)
