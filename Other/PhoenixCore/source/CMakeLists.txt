cmake_minimum_required (VERSION 2.6)
project (PhoenixCore)


# Build Poenix Core
set(sources
	2dGraphicsFactory.cpp
	BatchRenderer.cpp
	BitmapFont.cpp
	BMFontLoader.cpp
	DebugConsole.cpp
	DroidSansMono.cpp
	EventReceiver.cpp
	Font.cpp
	Polygon.cpp
	Rectangle.cpp
	RenderSystem.cpp
	ResourceManager.cpp
	Shader.cpp
	Texture.cpp
	WindowManager.cpp
	GLFWWindowManager.cpp
	soil/image_DXT.c
	soil/image_helper.c
	soil/SOIL.c
	soil/stb_image_aug.c
        glew/glew.c
	)

add_library(PhoenixCore_static STATIC ${sources})
add_library(PhoenixCore SHARED ${sources})

install(TARGETS PhoenixCore
        LIBRARY DESTINATION lib)
