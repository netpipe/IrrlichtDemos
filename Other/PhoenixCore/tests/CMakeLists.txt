############################################
#
# oenixCore Tests CMake
#
############################################
cmake_minimum_required( VERSION 2.6 )

project( PhoenixCore )

############################################
# Source files for each test.
############################################
set( CORETEST_SOURCES
	main.cpp
	Demo.h
	TextureTest.h
	PolygonTest.h
	ResourceTest.h
	ChangeTest.h
	ViewTest.h
	EventTest.h
	VectorTest.h
	EventTest.h
	FontTest.h
	TristripTest.h
	ShaderTest.h
	ResizeTest.h
	FullscreenTest.h
	GeometryTest.h
)

############################################
# Tests
############################################

#Demo Test
add_executable( PhoenixCoreDemo ${CORETEST_SOURCES} )
target_link_libraries( PhoenixCoreDemo PhoenixCore_static ${LIBRARIES} )
set_property(
	TARGET PhoenixCoreDemo
	APPEND PROPERTY COMPILE_DEFINITIONS _TESTS_DEMO_
)

#Font Test
add_executable( FontTest ${CORETEST_SOURCES} )
target_link_libraries( FontTest PhoenixCore_static ${LIBRARIES} )
set_property(
	TARGET FontTest
	APPEND PROPERTY COMPILE_DEFINITIONS _TESTS_FONT_
)

#Texture Test
add_executable( TextureTest ${CORETEST_SOURCES} )
target_link_libraries( TextureTest PhoenixCore_static ${LIBRARIES} )
set_property(
	TARGET TextureTest
	APPEND PROPERTY COMPILE_DEFINITIONS _TESTS_TEXTURE_
)

#Event Test
add_executable( EventTest ${CORETEST_SOURCES} )
target_link_libraries( EventTest PhoenixCore_static ${LIBRARIES} )
set_property(
	TARGET EventTest
	APPEND PROPERTY COMPILE_DEFINITIONS _TESTS_EVENT_
)

#Polygon Test
add_executable( PolygonTest ${CORETEST_SOURCES} )
target_link_libraries( PolygonTest PhoenixCore_static ${LIBRARIES} )
set_property(
	TARGET PolygonTest
	APPEND PROPERTY COMPILE_DEFINITIONS _TESTS_POLYGON_
)

#Vector Test
add_executable( VectorTest ${CORETEST_SOURCES} )
target_link_libraries( VectorTest PhoenixCore_static ${LIBRARIES} )
set_property(
	TARGET VectorTest
	APPEND PROPERTY COMPILE_DEFINITIONS _TESTS_VECTOR_
)

#Resource Test
add_executable( ResourceTest ${CORETEST_SOURCES} )
target_link_libraries( ResourceTest PhoenixCore_static ${LIBRARIES} )
set_property(
	TARGET ResourceTest
	APPEND PROPERTY COMPILE_DEFINITIONS _TESTS_RESOURCE_ ENABLECONSOLE
)

#Change Test
add_executable( ChangeTest ${CORETEST_SOURCES} )
target_link_libraries( ChangeTest PhoenixCore_static ${LIBRARIES} )
set_property(
	TARGET ChangeTest
	APPEND PROPERTY COMPILE_DEFINITIONS _TESTS_CHANGE_ ENABLECONSOLE
)

#View Test
add_executable( ViewTest ${CORETEST_SOURCES} )
target_link_libraries( ViewTest PhoenixCore_static ${LIBRARIES} )
set_property(
	TARGET ViewTest
	APPEND PROPERTY COMPILE_DEFINITIONS _TESTS_VIEW_
)

#Fullscreen Test
add_executable( FullscreenTest ${CORETEST_SOURCES} )
target_link_libraries( FullscreenTest PhoenixCore_static ${LIBRARIES} )
set_property(
	TARGET FullscreenTest
	APPEND PROPERTY COMPILE_DEFINITIONS _TESTS_FULLSCREEN_
)

#Tristrip Test
add_executable( TristripTest ${CORETEST_SOURCES} )
target_link_libraries( TristripTest PhoenixCore_static ${LIBRARIES} )
set_property(
	TARGET TristripTest
	APPEND PROPERTY COMPILE_DEFINITIONS _TESTS_TRISTRIP_
)

#Shader Test
add_executable( ShaderTest ${CORETEST_SOURCES} )
target_link_libraries( ShaderTest PhoenixCore_static ${LIBRARIES} )
set_property(
	TARGET ShaderTest
	APPEND PROPERTY COMPILE_DEFINITIONS _TESTS_SHADER_
)

#Resize Test
add_executable( ResizeTest ${CORETEST_SOURCES} )
target_link_libraries( ResizeTest PhoenixCore_static ${LIBRARIES} )
set_property(
	TARGET ResizeTest
	APPEND PROPERTY COMPILE_DEFINITIONS _TESTS_RESIZE_
)

#Geometry Test
add_executable( GeometryTest ${CORETEST_SOURCES} )
target_link_libraries( GeometryTest PhoenixCore_static ${LIBRARIES} )
set_property(
	TARGET GeometryTest
	APPEND PROPERTY COMPILE_DEFINITIONS _TESTS_GEOMETRY_
)

######################################
# Windows stuff
######################################
#WINDOWS (MSVC)
if(WIN32)
	if(MSVC)
	    set_target_properties( PhoenixCore_staticDemo TextureTest EventTest PolygonTest VectorTest ViewTest TristripTest ResizeTest GeometryTest
	        PROPERTIES WIN32_EXECUTABLE true )
	endif()
endif()
