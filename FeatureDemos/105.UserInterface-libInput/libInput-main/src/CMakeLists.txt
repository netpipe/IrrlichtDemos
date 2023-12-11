set(common_SOURCES InputController.cpp)
set(common_HEADERS ${Input_SOURCE_DIR}/include/InputController.h
	${Input_SOURCE_DIR}/include/InputDevice.h
	${Input_SOURCE_DIR}/include/Keyboard.h
	${Input_SOURCE_DIR}/include/Mouse.h)

if (X11_BACKEND)
	set(Input_SOURCES ${common_SOURCES}
		x11/X11InputController.cpp
		x11/X11InputDevice.cpp
		x11/X11Keyboard.cpp
		x11/X11Mouse.cpp)
	set(Input_HEADERS ${common_HEADERS}
		x11/X11InputController.h
		x11/X11InputDevice.h
		x11/X11Keyboard.h
		x11/X11Mouse.h)
endif()

add_library(Input ${Input_SOURCES} ${Input_HEADERS})
	
if (X11_BACKEND)
	target_link_libraries(Input ${X11_LIBRARIES}
		${X11_Xau_LIB}
		${X11_Xdmcp_LIB}
		${X11_Xi_LIB}
		${X11_Xext_LIB}
		${X11_Xcomposite_LIB}
		${X11_Xfixes_LIB}
		${X11_Xtst_LIB}
		${X11_Xrender_LIB}
		${X11_Xinerama_LIB}
		${X11_xkbcommon_LIB}
		${CMAKE_DL_LIBS}
		Threads::Threads)
endif()

target_include_directories(Input PUBLIC
	$<BUILD_INTERFACE:${Input_SOURCE_DIR}/include>)
