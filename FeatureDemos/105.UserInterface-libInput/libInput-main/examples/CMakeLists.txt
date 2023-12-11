set(CMAKE_BUILD_TYPE Debug)

find_package(PNG REQUIRED)
find_package(EXPAT REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(CAIRO REQUIRED cairo)
pkg_check_modules(PIXMAN REQUIRED pixman-1)
pkg_check_modules(FONTCONFIG REQUIRED fontconfig)

add_executable(query_state query_state.cpp)
add_executable(multi-pointers multi-pointers.cpp)

target_link_libraries(query_state PRIVATE Input)

target_link_directories(multi-pointers PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/../install/usr/local/lib)
target_include_directories(multi-pointers PUBLIC ${CAIRO_INCLUDE_DIRS}
	 ${CMAKE_CURRENT_BINARY_DIR}/../install/usr/local/include)

target_link_libraries(multi-pointers PUBLIC Input
	${CAIRO_LIBRARIES}
	${PIXMAN_LIBRARIES}
	${PNG_LIBRARIES}
	${FONTCONFIG_LIBRARIES}
	${EXPAT_LIBRARIES}
	xdo)

add_custom_command(TARGET multi-pointers POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${CMAKE_CURRENT_SOURCE_DIR}/cursor1.png
		${CMAKE_CURRENT_SOURCE_DIR}/cursor2.png
		$<TARGET_FILE_DIR:multi-pointers>)

if (X11_BACKEND)
    add_compile_definitions(USE_X11=1)
    target_compile_definitions(query_state PRIVATE USE_X11=1)
endif()
