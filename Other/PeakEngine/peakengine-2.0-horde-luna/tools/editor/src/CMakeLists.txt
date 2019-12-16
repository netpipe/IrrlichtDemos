
project(peakeditor)
set (MAIN_SRC main.cpp
	MainWindow.cpp
	Horde3DWidget.cpp)

EXEC_PROGRAM("pkg-config"
      ARGS "--cflags gtkmm-2.4 libglademm-2.4 gdk-2.0 gtkglextmm-1.2"
      OUTPUT_VARIABLE GTKMM_CFLAGS
      RETURN_VALUE PKGCONFIG_RETURN)
IF (NOT PKGCONFIG_RETURN STREQUAL "0")
SET (GTKMM_CFLAGS "")
ENDIF (NOT PKGCONFIG_RETURN STREQUAL "0")

EXEC_PROGRAM("pkg-config"
      ARGS "--libs gtkmm-2.4 libglademm-2.4 gdk-2.0 gthread-2.0 gtkglextmm-1.2"
      OUTPUT_VARIABLE GTKMM_LIBS
      RETURN_VALUE PKGCONFIG_RETURN)
IF (NOT PKGCONFIG_RETURN STREQUAL "0")
SET (GTKMM_LIBS "")
ENDIF (NOT PKGCONFIG_RETURN STREQUAL "0")

set (CMAKE_CXX_FLAGS "${GTKMM_CFLAGS} -g -Wall")
set (CMAKE_EXE_LINKER_FLAGS "-L/usr/local/lib -lHorde3D -lHorde3DUtils -Wl,-rpath,/usr/local/lib -g")

set (EXECUTABLE_OUTPUT_PATH ../../..)

include_directories(../../engine/include ../include /usr/local/include/horde3d)

add_executable (peakeditor ${MAIN_SRC})
target_link_libraries (peakeditor ${GTKMM_LIBS})

