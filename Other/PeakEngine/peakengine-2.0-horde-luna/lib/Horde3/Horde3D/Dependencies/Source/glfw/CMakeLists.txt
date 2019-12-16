set(GLFW_SOURCES
	enable.c
	fullscreen.c
	glext.c
	image.c
	init.c
	input.c
	joystick.c
	stream.c
	tga.c
	thread.c
	time.c
	window.c
	)


if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	include_directories(./ ./win32)
	set(GLFW_SOURCES
		${GLFW_SOURCES}
		win32/win32_dllmain.c
		win32/win32_enable.c
		win32/win32_fullscreen.c
		win32/win32_glext.c
		win32/win32_init.c
		win32/win32_joystick.c
		win32/win32_thread.c
		win32/win32_time.c
		win32/win32_window.c
		)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")


if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	include_directories(./ ./x11)
	set(GLFW_SOURCES
		${GLFW_SOURCES}
		x11/x11_enable.c
		x11/x11_fullscreen.c
		x11/x11_glext.c
		x11/x11_init.c
		x11/x11_joystick.c
		x11/x11_keysym2unicode.c
		x11/x11_thread.c
		x11/x11_time.c
		x11/x11_window.c
		)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")



if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	include_directories(./ ./macosx)
	set(GLFW_SOURCES
		${GLFW_SOURCES}
		macosx/macosx_enable.c
		macosx/macosx_fullscreen.c
		macosx/macosx_glext.c
		macosx/macosx_init.c
		macosx/macosx_joystick.c
		macosx/macosx_thread.c
		macosx/macosx_time.c
		macosx/macosx_window.c
		)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")


add_library(glfw STATIC
	${GLFW_SOURCES}
	)

