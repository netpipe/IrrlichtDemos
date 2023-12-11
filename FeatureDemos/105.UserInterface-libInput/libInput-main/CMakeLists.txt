cmake_minimum_required(VERSION 3.2)

include(ExternalProject)

project(Input)

option(X11_BACKEND "Build X11 Backend" ON)
option(BUILD_EXAMPLES "Build examples" ON)

if (X11_BACKEND)
    message(STATUS "Enabled X11 Backend")
    find_package(X11 COMPONENTS Xau
	Xext Xdmcp Xi
	Xtst Xfixes Xcomposite
	Xrender Xinerama
	REQUIRED)

    find_package(Threads REQUIRED)
endif()

add_subdirectory(src)
add_subdirectory(examples)

ExternalProject_Add(xdotool
	SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/external/xdotool
	INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/install
	DEPENDS X11::Xtst
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ${CMAKE_COMMAND} -E env
		HOST_CC=...   # See below
		CC=${CMAKE_C_COMPILER}
		make -C <SOURCE_DIR>
	BUILD_IN_SOURCE TRUE
	INSTALL_COMMAND make DESTDIR=<INSTALL_DIR> install
)
